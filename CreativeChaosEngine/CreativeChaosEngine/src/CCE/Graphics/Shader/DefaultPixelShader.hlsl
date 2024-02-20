SamplerState splr : register(s0);
Texture2D Texture : register(t0);
Texture2D Normal : register(t1);

struct Light
{
    float4 position;
    float4 color;
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 uv_coordinates : TEXCOORD;
    float4 color : COLOR;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 _albedo = Texture.Sample(splr, input.uv_coordinates);
    //return Normal.Sample(splr, input.uv_coordinates);
    return float4(_albedo.x * input.color.x, _albedo.y * input.color.y, _albedo.z * input.color.z, _albedo.w * input.color.w);
}