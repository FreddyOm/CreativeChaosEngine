SamplerState splr : register(s0);
Texture2D Texture : register(t0);

float4 main(float2 uv_coordinates : TEXCOORD0) : SV_TARGET
{
    //return Texture.Sample(splr, uv_coordinates);
    return float4(0.5f, 0.1f, 0.6f, 1.0f);

}