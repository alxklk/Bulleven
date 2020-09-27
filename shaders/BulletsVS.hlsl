#include "transforms.h"

cbuffer buf : register(b0)
{
	float4x4 wm;
}

struct AppData
{
	float3 position : POSITION;
	float2 uv: TEXCOORD0;
};

struct VertexShaderOutput
{
	float3 pos : TEXCOORD0;
	float4 position : SV_POSITION;
};

VertexShaderOutput main(AppData i)
{
	VertexShaderOutput o;
	float3 p=i.position+float3(i.uv,0)*0.01;
	o.position=transform(wm, p);
	o.pos=i.position.xyz;
	return o;
}