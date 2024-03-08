struct CollisionPair
{
    float4 positionA;
    float4 positionB;
    float4 colliderDimensionsA;
    float4 colliderDimensionsB;
    float4 velocityA;
    float4 velocityB;
    float4 angularVelA;
    float4 angularVelB;    
    float4 bouncinessA;
    float4 bouncinessB;
    float4 massA;
    float4 massB;
    uint4 shapeA; // 0: AABB , 1: Sphere
    uint4 shapeB; // 0: AABB , 1: Sphere

    float4 collisionPointA;
    float4 collisionPointB;
    float4 collisionNormal;
    float4 penetration;
};

struct ResponseData
{
    float4 newPositionA;
    float4 newPositionB;
    float4 newVelocityA;
    float4 newVelocityB;
    float4 linearImpulseA;
    float4 linearImpulseB;
    float4 angularImpulseA;
    float4 angularImpulseB;
    
};

StructuredBuffer<CollisionPair> collisionPairs : register(t0);
RWStructuredBuffer<ResponseData> responseData : register(u0);

float InverseMass(float mass)
{
    return mass <= 0.000001f && mass >= -0.000001f ? 0.0f : 1.0f / mass;
}

float3x3 InertiaTensor(uint shape, float mass, float3 dimensions)
{
    return shape == 0 ? 
    float3x3((1.0f / 12.0f) * mass * (dimensions.y * dimensions.y + dimensions.z * dimensions.z), 0,0,
    0, (1.0f / 12.0f) * mass * (dimensions.x * dimensions.x + dimensions.z * dimensions.z), 0,
    0, 0, (1.0f / 12.0f) * mass * (dimensions.y * dimensions.y + dimensions.x * dimensions.x))
    : 
    float3x3((2.0f / 5.0f) * mass * (dimensions.x * dimensions.x), 0,0,
    0, (2.0f / 5.0f) * mass * (dimensions.x * dimensions.x), 0,
    0, 0, (2.0f / 5.0f) * mass * (dimensions.x * dimensions.x));
}

void ApplyLinearTransformations(CollisionPair cPair, ResponseData rData)
{
    float totalInverseMass = InverseMass(cPair.massA.x) + InverseMass(cPair.massB.x);
    
    // Set new position by linearlly transforming along the collision normal
    rData.newPositionA = cPair.positionA - (cPair.collisionNormal * cPair.penetration * 
    (InverseMass(cPair.massA.x) / totalInverseMass));
    
    rData.newPositionB = cPair.positionB + (cPair.collisionNormal * cPair.penetration *
    (InverseMass(cPair.massB.x) / totalInverseMass));
}

void ResolveCollisionImpulse(CollisionPair cPair, ResponseData rData)
{
    float totalInverseMass = InverseMass(cPair.massA.x) + InverseMass(cPair.massB.x);
    
    // Get relative position vector to collision point
    float4 relativeA = distance(cPair.positionA, cPair.collisionPointA);
    float4 relativeB = distance(cPair.positionB, cPair.collisionPointA); // Use the same collisionPoint here as above!!
    
    // Calculate angular velocity
    float4 angVelA = float4(cross(cPair.angularVelA.xyz, relativeA.xyz), 1.0f);
    float4 angVelB = float4(cross(cPair.angularVelB.xyz, relativeB.xyz), 1.0f);
    
    // Calculate full velocity
    float4 fullVelocityA = cPair.velocityA + angVelA;
    float4 fullVelocityB = cPair.velocityB + angVelB;
    
    // Calculate contact velocity
    float4 contactVelocity = fullVelocityB - fullVelocityA;
    
    // Calculate impulse using the dot product
    float4 impulseForce = dot(contactVelocity, cPair.collisionNormal);
    
    // Calculate resulting inertia
    float3x3 inertiaTensorA = InertiaTensor(cPair.shapeA.x, cPair.massA.x, cPair.colliderDimensionsA.xyz);
    float3x3 inertiaTensorB = InertiaTensor(cPair.shapeB.x, cPair.massB.x, cPair.colliderDimensionsB.xyz);
    float4 inertiaA = float4(cross(mul(inertiaTensorA, cross(relativeA.xyz, cPair.collisionNormal.xyz)), relativeA.xyz), 1.0f);
    float4 inertiaB = float4(cross(mul(inertiaTensorB, cross(relativeB.xyz, cPair.collisionNormal.xyz)), relativeB.xyz), 1.0f);

    // Calculate angular effect
    float angularEff = dot((inertiaA + inertiaB).xyz, cPair.collisionNormal.xyz);
    
    // Damping (friction)
    float damping = clamp(cPair.bouncinessA.x + cPair.bouncinessA.x / 2.0f, 0.0f, 1.0f );
    
    // Impulse
    float j = (-(1.0f + damping) * impulseForce.x) / (totalInverseMass + angularEff);
    float4 fullImpulse = float4(mul(cPair.collisionNormal.xyz, j), 1.0f);
    
    rData.linearImpulseA = -fullImpulse;
    rData.linearImpulseB = fullImpulse;
    
    rData.angularImpulseA = float4(cross(relativeA.xyz, -fullImpulse.xyz), 1.0f);
    rData.angularImpulseB = float4(cross(relativeB.xyz, fullImpulse.xyz), 1.0f);
}

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    ApplyLinearTransformations(collisionPairs[DTid.x], responseData[DTid.x]);
    ResolveCollisionImpulse(collisionPairs[DTid.x], responseData[DTid.x]);
}
