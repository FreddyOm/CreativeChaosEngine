static const uint NumLights = 3;

cbuffer TransformConstants : register(b0)
{
    row_major float4x4 modelMatrix;
    row_major float4x4 viewMatrix;
    row_major float4x4 projMatrix;
};

cbuffer ShadingConstants : register(b0)
{
    struct
    {
        float3 direction;
        float3 radiance;
    } lights[NumLights];
    float3 eyePosition;
};

struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 texcoord : TEXCOORD;
};

struct PixelShaderInput
{
    float4 pixelPosition : SV_POSITION;
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3x3 tangentBasis : TBASIS;
};

PixelShaderInput main(VertexShaderInput vin)
{
    PixelShaderInput vout;
    vout.position = mul(float4(vin.position, 1.0), modelMatrix).xyz;
    vout.texcoord = float2(vin.texcoord.x, 1.0 - vin.texcoord.y);

	// Pass tangent space basis vectors (for normal mapping).
    float3x3 TBN = float3x3(vin.tangent, vin.bitangent, vin.normal);
    vout.tangentBasis = mul((float3x3) modelMatrix, transpose(TBN));

    float4x4 mvpMatrix = mul(viewMatrix, projMatrix);
    vout.pixelPosition = mul(mvpMatrix, float4(vin.position, 1.0));
    return vout;
}