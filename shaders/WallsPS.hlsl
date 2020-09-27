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
	return float4(float3(.5,.7,.8)*sqrt(i.pos.z*10.),1);
	return i.color;
}