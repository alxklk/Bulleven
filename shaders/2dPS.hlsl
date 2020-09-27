Texture2D tex : register(t0);
SamplerState sam : register(s0);

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 col : TEXCOORD1;
};

float4 main( PixelShaderInput IN ) : SV_TARGET0
{
	float4 col=tex.Sample(sam,IN.uv)*IN.col;
	return float4(col);
}