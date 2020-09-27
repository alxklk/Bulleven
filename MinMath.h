#pragma once

struct float2
{
	float x;
	float y;
	float2()=default;
	float2(const float& _x, const float& _y):x(_x),y(_y){}
};

inline float2 operator+(const float2& l, const float2& r){return {l.x+r.x, l.y+r.y};}
inline float2 operator-(const float2& l, const float2& r){return {l.x-r.x, l.y-r.y};}
template<typename T>float2 operator*(const float2& l, const T& r){return {l.x*r, l.y*r};}
template<typename T>float2 operator*=(float2& l, const T& r){l.x*=r;l.y*=r;return l;}
inline float2 operator+=(float2& l, const float2& r){l.x+=r.x;l.y+=r.y;return l;}

struct float3
{
	float x;
	float y;
	float z;
	float3() = default;
	float3(const float& _x, const float& _y, const float& _z):x(_x),y(_y),z(_z){}
	float2 xy()const{return {x,y};}
};

struct float4
{
	float x;
	float y;
	float z;
	float w;
	float4() = default;
	float4(const float& _x, const float& _y, const float& _z, const float& _w)
		:x(_x),y(_y),z(_z),w(_w){}
};

struct float4x4
{
	float m[4][4];
	static float4x4 Identity()
	{
		return float4x4
		{
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f,
		};
	}
	static float4x4 Zero()
	{
		return float4x4
		{
			0.0f,0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,0.0f,
		};
	}
};

inline float4x4 operator* (const float4x4& l,const float4x4& r)
{
	int i,j,k;
	float4x4 result=float4x4::Zero();

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			for(k=0;k<4;k++)
				result.m[i][j]+=l.m[i][k]*r.m[k][j];
	return result;
}


inline void QuaternionToMatrix(const float4& q, float4x4& m)
{
	float sqw=q.w*q.w;
	float sqx=q.x*q.x;
	float sqy=q.y*q.y;
	float sqz=q.z*q.z;
	m=float4x4::Identity();

	float invs=1.0f/(sqx+sqy+sqz+sqw);
	m.m[0][0]=( sqx-sqy-sqz+sqw)*invs;
	m.m[1][1]=(-sqx+sqy-sqz+sqw)*invs;
	m.m[2][2]=(-sqx-sqy+sqz+sqw)*invs;

	float tmp1=q.x*q.y;
	float tmp2=q.z*q.w;
	m.m[1][0]=2.0f*(tmp1+tmp2)*invs;
	m.m[0][1]=2.0f*(tmp1-tmp2)*invs;

	tmp1=q.x*q.z;
	tmp2=q.y*q.w;
	m.m[2][0]=2.0f*(tmp1-tmp2)*invs;
	m.m[0][2]=2.0f*(tmp1+tmp2)*invs;
	tmp1=q.y*q.z;
	tmp2=q.x*q.w;
	m.m[2][1]=2.0f*(tmp1+tmp2)*invs;
	m.m[1][2]=2.0f*(tmp1-tmp2)*invs;
}
