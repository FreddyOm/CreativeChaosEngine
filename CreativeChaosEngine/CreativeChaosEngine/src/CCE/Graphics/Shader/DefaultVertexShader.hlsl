cbuffer PerFrame : register(b0)
{
    row_major matrix viewprojection;
};

cbuffer PerObject : register(b1)
{
    row_major matrix modelmatrix;
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
    //matrix world = mul(modelmatrix, viewprojection);
    
    vs_out output = (vs_out) 0; // zero the memory first
    //output.position_clip = mul(world, float4(input.position_local, 1.0));
    output.position_clip = float4(input.position_local, 1.0);
    return output;
}