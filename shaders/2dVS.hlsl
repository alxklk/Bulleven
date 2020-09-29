struct AppData
{
	float2 pos : POSITION;
	float2 uv: TEXCOORD0;
	float4 col: TEXCOORD1;
};

struct VertexShaderOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 col : TEXCOORD1;
};

VertexShaderOutput main(AppData IN)
{
	VertexShaderOutput OUT;
	OUT.pos = float4(IN.pos, 0.0f, 1.0f);
	OUT.uv = IN.uv;
	OUT.col = IN.col;
	return OUT;
}