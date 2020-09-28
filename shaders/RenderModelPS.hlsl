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
	float v=(sin(i.pos.x*100)*sin(i.pos.y*100));
	v=smoothstep(-0.1,0.1,v);
	return float4(
		lerp(float3(.8,.7,.6), float3(0.3,0.3,.5),v),1);
	return i.color;
}