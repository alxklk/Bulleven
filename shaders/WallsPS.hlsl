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
	return float4(float3(.8,.9,.95)*(.3+.7*sqrt(i.pos.z*10.)),1);
}