float4 transform(float4x4 wm, float3 p)
{
	float4 ret=mul(wm, float4(p, 1.0f));
	return ret;
}