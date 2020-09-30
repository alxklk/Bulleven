#pragma once

#define USE_MATH_DEFINES

#include <cmath>

struct float2
{
	float x;
	float y;
	float2()=default;
	float2(const float& _x, const float& _y):x(_x),y(_y){}
	float lengthSq()const{return x*x+y*y;}
	float length()const{return sqrt(lengthSq());}
	float2 perp()const{return float2(y,-x);}
	float2 norm()const
	{
		float l=length();
		if(l==0){return {0,0};}
		return{x/l,y/l};
	}

};

inline float2 operator+(const float2& l, const float2& r){return {l.x+r.x, l.y+r.y};}
inline float2 operator-(const float2& l, const float2& r){return {l.x-r.x, l.y-r.y};}
template<typename T>float2 operator*(const float2& l, const T& r){return {l.x*r, l.y*r};}
template<typename T>float2 operator*=(float2& l, const T& r){l.x*=r;l.y*=r;return l;}
inline float2 operator+=(float2& l, const float2& r){l.x+=r.x;l.y+=r.y;return l;}

inline float vdot(const float2& l,const float2& r){return l.x*r.x+l.y*r.y;}

inline float2 reflect(const float2& r, const float2& n)
{
	return n*vdot(n,r)*2.0f-r;
}

struct float3
{
	float x;
	float y;
	float z;
	float3() = default;
	float3(const float& _x, const float& _y, const float& _z):x(_x),y(_y),z(_z){}
	float2 xy()const{return {x,y};}
	float lengthSq()const{return x*x+y*y+z*z;}
	float length()const{return sqrt(lengthSq());}
	float3 norm()const
	{
		float l=length();
		if(l==0){return{0,0,0};}
		return{x/l,y/l,z/l};
	}

};

inline float3 operator-(const float3& l, const float3& r){return {l.x-r.x, l.y-r.y, l.z-r.z};}
inline float3 operator+(const float3& l, const float3& r){return {l.x+r.x, l.y+r.y, l.z+r.z};}
inline float3 operator*(const float3& l, const float3& r){return {l.x*r.x, l.y*r.y, l.z*r.z};}
inline float3 operator-=(float3& l, const float3& r){l.x-=r.x;l.y-=r.y;l.z-=r.z;return l;}
inline float3 operator+=(float3& l, const float3& r){l.x+=r.x;l.y+=r.y;l.z+=r.z;return l;}
template<typename T>float3 operator*(const float3& l, const T& r){return {l.x*r, l.y*r, l.z*r};}
template<typename T>float3 operator/(const float3& l, const T& r){return {l.x/r, l.y/r, l.z/r};}

inline float vlensq(const float3& v){return v.x*v.x+v.y*v.y+v.z*v.z;};
inline float vlen(const float3& v){return sqrt(vlensq(v));};
inline float3 vnorm(const float3& v){return v/vlen(v);};
inline float vdot(const float3& l, const float3& r){return l.x*r.x+l.y*r.y+l.z*r.z;};

inline float3 vcross(const float3& l, const float3& r)
{
	return {
		l.y*r.z-l.z*r.y,
		l.z*r.x-l.x*r.z,
		l.x*r.y-l.y*r.x
	};
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
	void Identity()
	{
		*this=
		{
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f,
		};
	}
	void Zero()
	{
		*this=float4x4
		{
			0.0f,0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,0.0f,
		};
	}

	const float4x4& operator*= (const float4x4& r)
	{
		int i,j,k;
		float4x4 t;
		t.Zero();
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				for(k=0;k<4;k++)
					t.m[i][j]+=m[i][k]*r.m[k][j];
		*this=t;
		return *this;
	}

	void Transpose()
	{
		float dummy0=m[0][1];m[0][1]=m[1][0];m[1][0]=dummy0;
		float dummy1=m[0][2];m[0][2]=m[2][0];m[2][0]=dummy1;
		float dummy2=m[0][3];m[0][3]=m[3][0];m[3][0]=dummy2;
		float dummy3=m[1][2];m[1][2]=m[2][1];m[2][1]=dummy3;
		float dummy4=m[1][3];m[1][3]=m[3][1];m[3][1]=dummy4;
		float dummy5=m[2][3];m[2][3]=m[3][2];m[3][2]=dummy5;
	}

	void RotationZ(const float& sina, const float& cosa)
	{
		m[0][0]=cosa;m[0][1]=-sina;m[0][2]= 0.0f;m[0][3]= 0.0f;
		m[1][0]=sina;m[1][1]= cosa;m[1][2]= 0.0f;m[1][3]= 0.0f;
		m[2][0]=0.0f;m[2][1]= 0.0f;m[2][2]= 1.0f;m[2][3]= 0.0f;
		m[3][0]=0.0f;m[3][1]= 0.0f;m[3][2]= 0.0f;m[3][3]= 1.0f;
	};

	void Inverse()
	{
		float im_temp1=m[1][1]*m[3][2];
		float im_temp3=m[1][1]*m[3][3];
		float im_temp5=m[3][1]*m[1][3];
		float im_temp7=m[3][1]*m[1][2];
		float im_temp9=m[3][2]*m[2][1];
		float im_temp11=m[3][3]*m[2][1];
		float im_temp14=m[1][1]*m[0][0];
		float im_temp15=m[3][2]*m[2][3];
		float im_temp17=m[3][3]*m[2][2];
		float im_temp19=m[3][1]*m[0][0];
		float im_temp20=m[1][3]*m[2][2];
		float im_temp22=m[1][2]*m[2][3];
		float im_temp24=m[0][0]*m[3][2];
		float im_temp25=m[2][1]*m[1][3];
		float im_temp27=m[0][0]*m[3][3];
		float im_temp28=m[2][1]*m[1][2];
		float im_temp30=m[2][0]*m[0][3];
		float im_temp32=m[1][1]*m[3][0];
		float im_temp33=m[0][2]*m[2][3];
		float im_temp35=m[2][0]*m[0][2];
		float im_temp37=m[0][3]*m[2][2];
		float im_temp39=m[3][2]*m[2][0];
		float im_temp40=m[0][1]*m[1][3];
		float im_temp42=m[3][0]*m[0][2];
		float im_temp44=im_temp14*im_temp15-im_temp14*im_temp17+im_temp19*im_temp20-im_temp19*im_temp22-im_temp24*im_temp25+im_temp27*im_temp28-im_temp1*im_temp30-im_temp32*im_temp33+im_temp3*im_temp35+im_temp32*im_temp37+im_temp39*im_temp40+im_temp42*im_temp25;
		float im_temp46=m[1][0]*m[3][1];
		float im_temp48=m[1][0]*m[0][1];
		float im_temp50=m[3][3]*m[2][0];
		float im_temp51=m[0][1]*m[1][2];
		float im_temp53=m[1][0]*m[0][2];
		float im_temp55=m[3][0]*m[0][3];
		float im_temp60=m[3][0]*m[0][1];
		float im_temp62=m[1][0]*m[0][3];
		float im_temp65=im_temp7*im_temp30+im_temp46*im_temp33+im_temp17*im_temp48-im_temp50*im_temp51-im_temp11*im_temp53-im_temp55*im_temp28-im_temp5*im_temp35-im_temp15*im_temp48-im_temp46*im_temp37-im_temp60*im_temp20+im_temp9*im_temp62+im_temp60*im_temp22;
		float im_temp67=1/(im_temp44+im_temp65);
		float im_temp106=m[3][0]*m[1][3];
		float im_temp123=m[3][3]*m[1][0];
		float im_temp125=m[3][2]*m[1][0];
		float im_temp146=m[3][0]*m[2][1];
		float im_temp154=m[3][1]*m[2][0];
		float im_temp173=m[2][1]*m[0][0];
		float im_temp175=m[2][1]*m[1][0];
		float im_temp177=m[2][0]*m[0][1];

		float4x4 res;
		res.m[0][0]=-(-im_temp1*m[2][3]+im_temp3*m[2][2]-im_temp5*m[2][2]+im_temp7*m[2][3]+im_temp9*m[1][3]-im_temp11*m[1][2])*im_temp67;
		res.m[0][1]=(m[3][1]*m[0][2]*m[2][3]+im_temp17*m[0][1]-im_temp11*m[0][2]-im_temp15*m[0][1]-m[3][1]*m[0][3]*m[2][2]+im_temp9*m[0][3])*im_temp67;
		res.m[0][2]=-(im_temp1*m[0][3]-im_temp3*m[0][2]-m[3][2]*m[0][1]*m[1][3]-im_temp7*m[0][3]+m[3][3]*m[0][1]*m[1][2]+im_temp5*m[0][2])*im_temp67;
		res.m[0][3]=(-m[1][1]*m[0][2]*m[2][3]+m[1][1]*m[0][3]*m[2][2]+m[0][2]*m[2][1]*m[1][3]-m[0][3]*m[2][1]*m[1][2]-im_temp40*m[2][2]+im_temp51*m[2][3])*im_temp67;
		res.m[1][0]=-(im_temp15*m[1][0]-im_temp17*m[1][0]-m[3][0]*m[1][2]*m[2][3]+im_temp50*m[1][2]+im_temp106*m[2][2]-im_temp39*m[1][3])*im_temp67;
		res.m[1][1]=(im_temp24*m[2][3]-im_temp27*m[2][2]-im_temp39*m[0][3]-im_temp42*m[2][3]+im_temp50*m[0][2]+im_temp55*m[2][2])*im_temp67;
		res.m[1][2]=-(-im_temp27*m[1][2]+im_temp24*m[1][3]-im_temp106*m[0][2]+im_temp55*m[1][2]+im_temp123*m[0][2]-im_temp125*m[0][3])*im_temp67;
		res.m[1][3]=(-m[0][0]*m[1][2]*m[2][3]+m[0][0]*m[1][3]*m[2][2]-m[1][3]*m[2][0]*m[0][2]+m[1][2]*m[2][0]*m[0][3]+im_temp53*m[2][3]-im_temp62*m[2][2])*im_temp67;
		res.m[2][0]=(-im_temp32*m[2][3]+im_temp3*m[2][0]-im_temp11*m[1][0]+im_temp46*m[2][3]-im_temp5*m[2][0]+im_temp146*m[1][3])*im_temp67;
		res.m[2][1]=-(-im_temp27*m[2][1]+im_temp19*m[2][3]-m[2][3]*m[3][0]*m[0][1]-im_temp154*m[0][3]+im_temp50*m[0][1]+im_temp55*m[2][1])*im_temp67;
		res.m[2][2]=(-im_temp27*m[1][1]+im_temp123*m[0][1]+im_temp55*m[1][1]+im_temp19*m[1][3]-im_temp46*m[0][3]-im_temp60*m[1][3])*im_temp67;
		res.m[2][3]=-(-m[2][3]*m[0][0]*m[1][1]+m[2][3]*m[1][0]*m[0][1]+im_temp30*m[1][1]+im_temp173*m[1][3]-im_temp175*m[0][3]-im_temp177*m[1][3])*im_temp67;
		res.m[3][0]=(im_temp32*m[2][2]-im_temp1*m[2][0]-im_temp146*m[1][2]-im_temp46*m[2][2]+im_temp9*m[1][0]+im_temp7*m[2][0])*im_temp67;
		res.m[3][1]=-(-im_temp19*m[2][2]+im_temp24*m[2][1]+im_temp154*m[0][2]-im_temp42*m[2][1]+im_temp60*m[2][2]-im_temp39*m[0][1])*im_temp67;
		res.m[3][2]=(im_temp24*m[1][1]-im_temp125*m[0][1]-im_temp42*m[1][1]-im_temp19*m[1][2]+im_temp46*m[0][2]+im_temp60*m[1][2])*im_temp67;
		res.m[3][3]=-im_temp67*(m[2][2]*m[0][0]*m[1][1]-m[2][2]*m[1][0]*m[0][1]-im_temp35*m[1][1]-im_temp173*m[1][2]+im_temp175*m[0][2]+im_temp177*m[1][2]);
		*this=res;
	}

	// if zf==0, then generate infinity projection matrix.
	void Projection(float zn,float zf,float hfov,float vfov, float x0=-1, float x1=1, float y0=-1, float y1=1)
	{
		float dx=(x1-x0);
		float dy=(y1-y0);
		float xs;
		float ys;
		xs=1.0f/(hfov*0.5f);
		ys=1.0f/(vfov*0.5f);
		if(zf==0)
		{
			Zero();
			m[0][0]=xs;
			m[1][1]=ys;
			m[2][3]=-1;
			m[3][2]=zn;
		}
		else
		{
			float q;
			q=zf/(zf-zn);
			Zero();
			m[0][0]=2*xs/dx;
			m[1][1]=2*ys/dy;
			m[2][0]=-(2*x0+dx)/dx;
			m[2][1]=-(2*y0+dy)/dy;
			m[2][2]=q;
			m[2][3]=1;
			m[3][2]=-q*zn;
		}
	}

};

inline float4x4 operator* (const float4x4& l,const float4x4& r)
{
	int i,j,k;
	float4x4 result;
	result.Zero();

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			for(k=0;k<4;k++)
				result.m[i][j]+=l.m[i][k]*r.m[k][j];
	return result;
}

inline float3 operator* (const float3& x,const float4x4& m)
{
	int i,j;
	float r[3]={0,0,0};
	const float *v=&x.x;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			r[i]+=v[j]*m.m[j][i];
	return {r[0],r[1],r[2]};
}

inline bool IntersectRaySection(const float2& r00, const float2& r01,
	const float2& s10, const float2& s11, float& t)
{
	float2 d0=r01-r00;
	float2 d1=s11-s10;

	float a1=-d0.y;
	float b1= d0.x;
	float c1=-(a1*r00.x+b1*r00.y);

	float a2=-d1.y;
	float b2= d1.x;
	float c2=-(a2*s10.x+b2*s10.y);

	float d10=a2*r00.x+b2*r00.y+c2;
	float d11=a2*r01.x+b2*r01.y+c2;

	float d00=a1*s10.x+b1*s10.y+c1;
	float d01=a1*s11.x+b1*s11.y+c1;

	if((d10*d11>=0)||(d00*d01>=0))
		return false;

	t=d10/(d10-d11);

	return true;
}
