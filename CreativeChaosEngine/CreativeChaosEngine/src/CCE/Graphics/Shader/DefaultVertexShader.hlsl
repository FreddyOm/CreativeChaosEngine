cbuffer ModelConstantBuffer : register(b0)
{
    row_major float4x4 modelMatrix; // world matrix for object
};

cbuffer ViewProjectionBuffer : register(b1)
{
    row_major float4x4 viewMatrix;
    row_major float4x4 projMatrix;
};

struct vs_in
{
    float3 position_local : POSITION;
};

struct vs_out
{
    float4 position_clip : SV_POSITION; // required output of VS
};

vs_out main(vs_in input)
{
    vs_out output;
    
    
    float4 pos = float4(input.position_local, 1.0f);

    // Transform the position from object space to homogeneous projection space
    pos = mul(pos, modelMatrix);
    pos = mul(pos, viewMatrix);
    pos = mul(pos, projMatrix);
    output.position_clip = pos;

    return output;
}