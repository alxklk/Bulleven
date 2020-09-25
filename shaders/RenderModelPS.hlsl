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
	float xi=saturate(abs(frac(i.pos.x*10)-.5)*10);
	float yi=saturate(abs(frac(i.pos.y*10)-.5)*10);
	float ch=1-min(xi,yi);
	float2 uv=i.pos.xy*float2(.5,-1);
	uv=(trunc(uv*float2(128,64))-float2(.5,.5))/float2(128,64);
	return tex.Sample(sam,uv).bbba;
	return float4(ch*float3(0.9,0.95,1)+(1-ch)*i.color,1);
}