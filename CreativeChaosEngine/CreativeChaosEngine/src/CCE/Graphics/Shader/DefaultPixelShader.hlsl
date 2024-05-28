SamplerState splr : register(s0);
Texture2D Albedo : register(t0);
Texture2D Normal : register(t1);

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 uv_coordinates : TEXCOORD;
    float4 color : COLOR;
};

float3 lightPos = float3(0.0f, 10.0f, 0.0f);
float3 lightColor = float3(1.0f, 1.0f, 1.0f);

float4 main(PixelShaderInput input) : SV_TARGET
{
    
    float4 _albedo = Albedo.Sample(splr, input.uv_coordinates);
    //return Normal.Sample(splr, input.uv_coordinates);
    float3 _normal = Normal.Sample(splr, input.uv_coordinates);
    float4 _diffuse = input.color * _albedo;
    return _diffuse;
}