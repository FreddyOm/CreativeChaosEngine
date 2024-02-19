cbuffer ModelConstantBuffer : register(b0)
{
    row_major float4x4 modelMatrix; // world matrix for object
    float3 color;
};

cbuffer ViewProjectionBuffer : register(b1)
{
    row_major float4x4 viewMatrix;
    row_major float4x4 projMatrix;
};

struct VertexShaderInput
{
    float3 position_local : POSITION;
    float2 uv_coordinates : TEXCOORD;
    float3 normal : NORMAL;
};

struct VertexShaderOutput
{
    float4 position_clip : SV_POSITION; // required output of VS
    float2 uv_coordinates : TEXCOORD;
    float3 color : COLOR;
};

VertexShaderOutput main(VertexShaderInput vsInput)
{
    VertexShaderOutput output;
    
    float4 pos = float4(vsInput.position_local, 1.0f);

    // Transform the position from object space to homogeneous projection space
    pos = mul(pos, modelMatrix);
    pos = mul(pos, viewMatrix);
    pos = mul(pos, projMatrix);
    output.position_clip = pos;
    output.uv_coordinates = vsInput.uv_coordinates;
    output.color = color;
    
    return output;
}