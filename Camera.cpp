#include <cmath>
#include "Camera.h"


CCamera::CCamera(void)
: m_FOV(1)
, m_Pos(-0.01f,0,0.015f)
, m_LookAt(0,0,0)
, m_Near(.1)
, m_Far(2000)
, m_NeedUpdate(1)
, crop0(-1,-1)
, crop1(1,1)
, mode(CM_PROJ)
, W(1024.0f)
, H(768.0f)
{
}

CCamera::~CCamera(void)
{
}

void CCamera::Update()
{
	if(!m_NeedUpdate)
		return;
	m_U=float3(0,0,1);
	m_F=m_LookAt-m_Pos;
	m_F=vnorm(m_F);
	m_R=vcross(m_F,m_U);
	m_R=vnorm(m_R);
	m_U=vcross(m_R,m_F);
	m_U=vnorm(m_U);

	m_NeedUpdate=0;
}



void CCamera::CalcVPM(bool overkil)
{
	Update();
	float4x4 M;
	M.Identity();

	M.m[0][0]=m_R.x; M.m[0][1]=m_U.x; M.m[0][2]=m_F.x;
	M.m[1][0]=m_R.y; M.m[1][1]=m_U.y; M.m[1][2]=m_F.y;
	M.m[2][0]=m_R.z; M.m[2][1]=m_U.z; M.m[2][2]=m_F.z;

	if(overkil)
	{
		M.m[0][1]=-M.m[0][1];
		M.m[1][1]=-M.m[1][1];
		M.m[2][1]=-M.m[2][1];
	}

	M.m[3][0]=-vdot(m_R,m_Pos);
	M.m[3][1]=-vdot(m_U,m_Pos);
	M.m[3][2]=-vdot(m_F,m_Pos);

	float4x4 l_PM;

	if(mode==CM_PROJ)
	{
		l_PM.Projection((float)m_Near,(float)m_Far,(float)m_FOV,(float)(m_FOV*H/W));
	}
	else
	{
		float3 dir=m_Pos;
		dir-=m_LookAt;
		float len=vlen(dir);
	}

	PM=l_PM;
	VM=M;
	VPM=M*l_PM;
	VPMT=VPM;
	VPMT.Transpose();
	IVPM=VPM;
	IVPM.Inverse();

};

void CCamera::Forward(float i_Dist)
{
	Update();
	float3 F=m_F*i_Dist;
	m_LookAt+=F;
	m_Pos+=F;
	m_NeedUpdate=1;
}

void CCamera::Zoom(float i_Prc)
{
	Update();
	float3 F=(m_LookAt-m_Pos)*(1+i_Prc);
	m_Pos=m_LookAt-F;
	m_NeedUpdate=1;
}

void CCamera::Right(float i_Dist)
{
	Update();
	float3 R=m_R*i_Dist;
	m_LookAt+=R;
	m_Pos+=R;
	m_NeedUpdate=1;
}

void CCamera::Up(float i_Dist)
{
	Update();
	float3 R=m_U*i_Dist;
	m_LookAt+=R;
	m_Pos+=R;
	m_NeedUpdate=1;
}

void CCamera::Translate(const float3& i_D)
{
	Update();
	m_Pos+=i_D;
	m_LookAt+=i_D;
	m_NeedUpdate=1;
}

void CCamera::Rotate(float i_Yaw,float i_Pitch)
{
	Update();
	float3 l_Dir=m_LookAt-m_Pos;
	float ly=l_Dir.z;
	float lx=sqrt(l_Dir.x*l_Dir.x+l_Dir.y*l_Dir.y);
	float lx1=lx;

	l_Dir.z= cos(i_Pitch)*ly+lx*sin(i_Pitch);
	lx     =-sin(i_Pitch)*ly+lx*cos(i_Pitch);

	lx/=lx1;
	l_Dir.x*=lx;
	l_Dir.y*=lx;



	float4x4 l_TM;
	l_TM.Identity();

	float4x4 l_RY;
	l_RY.RotationZ(sin(i_Yaw), cos(i_Yaw));
	l_TM*=l_RY;

	float3 l_TV=l_Dir*l_TM;


//	m_LookAt=m_Pos+float3(l_TV);
	m_Pos=m_LookAt-float3(l_TV);
	m_NeedUpdate=1;
}
