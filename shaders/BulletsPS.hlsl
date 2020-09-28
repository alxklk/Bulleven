Texture2D tex : register(t0);
SamplerState sam : register(s0);

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 col : TEXCOORD1;
};


float4 main( PixelShaderInput i ) : SV_TARGET
{
	float a=saturate(1-length(i.uv.xy));
	return float4(1,a*1.5,a*a*a*1.3,a*2.);
}