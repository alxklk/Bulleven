Texture2D tex : register(t0);
SamplerState sam : register(s0);

struct PixelShaderInput
{
	float4 color : COLOR;
	float3 pos : TEXCOORD0;
	float3 norm: TEXCOORD1;
};


float4 main( PixelShaderInput i ) : SV_TARGET
{
	return i.color;
}