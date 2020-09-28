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
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

VertexShaderOutput main(AppData i)
{
	VertexShaderOutput o;
	float3 p=i.position;
	o.position=transform(wm, p);
	float h=length(float3(wm._m00,wm._m01,wm._m02));
	float w=length(float3(wm._m10,wm._m11,wm._m12));
	o.position.xy+=float2(i.uv)*float2(1,w/h)*0.02;
	o.uv=i.uv;
	return o;
}