cbuffer buf0 : register(b0)
{
	float fps;
	float d0;
	float d1;
	float d2;
}

struct AppData
{
	float2 pos : POSITION;
	float2 uv: TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

VertexShaderOutput main(AppData IN)
{
	VertexShaderOutput OUT;
	OUT.pos = float4(IN.pos, 0.0f, 1.0f);
	OUT.uv = IN.uv;
	return OUT;
}