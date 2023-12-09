SamplerState splr : register(s0);
Texture2D Texture : register(t0);

float4 main(float2 uv_coordinates : TEXCOORD0) : SV_TARGET
{
    return Texture.Sample(splr, uv_coordinates);
}