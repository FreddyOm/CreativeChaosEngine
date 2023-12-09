cbuffer ModelConstantBuffer : register(b0)
{
    row_major float4x4 modelMatrix; // world matrix for object
};

cbuffer ViewProjectionBuffer : register(b1)
{
    row_major float4x4 viewMatrix;
    row_major float4x4 projMatrix;
};

struct vs_out
{
    float2 uv_coordinates : TEXCOORD0;
    float4 position_clip : SV_POSITION; // required output of VS
};

vs_out main(float3 position_local : POSITION, float2 uv_coordinates : TEXCOORD0)
{
    vs_out output;
    
    float4 pos = float4(position_local, 1.0f);

    // Transform the position from object space to homogeneous projection space
    pos = mul(pos, modelMatrix);
    pos = mul(pos, viewMatrix);
    pos = mul(pos, projMatrix);
    output.position_clip = pos;
    output.uv_coordinates = uv_coordinates;

    return output;
}