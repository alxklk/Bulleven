cbuffer buf : register(b0)
{
	float4x4 wm;
}

struct AppData
{
	float3 position : POSITION;
	float3 normal: NORMAL;
	float3 col: TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 color : COLOR;
	float3 pos : TEXCOORD0;
	float3 norm: TEXCOORD1;
	float4 position : SV_POSITION;
};

VertexShaderOutput main(AppData i)
{
	VertexShaderOutput o;
	o.position=mul(wm, float4(i.position*1.6, 1.0f));
	o.position.z=0.5+o.position.z*.01;
	o.position.w=1./o.position.z;
	o.color=float4(i.col, 1.0f);
	o.pos=i.position.xyz;
	o.norm=mul(wm,i.normal);
	return o;
}