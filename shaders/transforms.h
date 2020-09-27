float4 transform(float4x4 wm, float3 p)
{
	float4 ret=mul(wm, float4(p*1.6, 1.0f));
	ret.xy*=.5;
	ret.z=0.5+ret.z*.01;
	ret.w=1.;
	return ret;
}