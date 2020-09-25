Texture2D tex : register(t0);
SamplerState sam : register(s0);

cbuffer buf0 : register(b0)
{
	float in_fps;
	float d0;
	float d1;
	float d2;
	int s[64];
}

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float2 digit_uv(uint d, uint x, uint y)
{
	const float2 offs[10]={
		float2(   0,   0),
		float2(1./8,   0),
		float2(2./8,   0),
		float2(3./8,   0),
		float2(4./8,   0),
		float2(5./8,   0),
		float2(6./8,   0),
		float2(7./8,   0),
		float2(   0,1./8),
		float2(1./8,1./8)
	};
	if((x>7)||(y>7))
	{
		return .5/64;
	}
	if(d<10)
	{
		return offs[d]+float2(x,y)/64+.5/64;
	}
	return .5/64;
}

float2 dot_uv(uint d, uint x, uint y)
{
	return float2(2./8,1./8)+float2(x,y)/64+.5/64;
}

float4 main( PixelShaderInput IN ) : SV_TARGET0
{
	return float4(1,0,0,.5);
	int2 iuv=IN.pos.xy/4;
	if(iuv.y>8)
		return float4(0,0,0,0);
	float fps=in_fps*10000;
	
	int ifps=fps;
	uint shift=1;
	int ishift=0;
	while(ifps>=shift)
	{
		shift*=10;
		ishift++;
	}
	if(ishift<=4)
	{
		ishift=5;
		shift=100000;
	}
	for(int i=0;i<=iuv.x/8;i++)
	{
		shift/=10;
		ishift--;
	}

	int digit=shift?(ifps/shift)%10:0;
	float3 c=float3(1,1,0);
	float2 duv=digit_uv(digit,iuv.x%8,iuv.y);
	if(ishift==3)
	{
		duv=dot_uv(digit,iuv.x%8,iuv.y);
	}
	if(ishift<3)
	{
		c=float3(1,0,0);
		digit=shift?(ifps/10/shift)%10:0;
		duv=digit_uv(digit,iuv.x%8,iuv.y);
	}
	if(ishift<0)
	{
		return float4(0,0,0,0);
	}

	float4 col=tex.Sample(sam,duv);
	return float4(c,col.g);
}