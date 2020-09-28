#pragma once

#include "MinMath.h"

class CCamera
{
public:
	float3 m_Pos;
	float3 m_LookAt;
	float3 m_R; // Local Right Vector
	float3 m_F; // Local Forward Vector
	float3 m_U; // Local Up Vector

	float W;
	float H;

	float2 crop0;
	float2 crop1;

	double m_FOV;

	double m_Near;
	double m_Far;

	float4x4 VPM; // View-Projection matrix
	float4x4 VPMT; // View-Projection matrix transposed
	float4x4 VM;  // View matrix
	float4x4 PM;  // Projection matrix

	float4x4 IVPM; // Inverse View-Projection matrix
	float4x4 IVPMT; // Inverse View-Projection matrix transposed

	enum CAMERA_MODE{CM_PROJ, CM_ORTHO, CM_COUNT};

	CAMERA_MODE mode;

	bool m_NeedUpdate;
	void Update();

public:
	void Rotate(float i_Yaw,float i_Pitch);
	void Translate(const float3& i_D);
	void Forward(float i_Dist);
	void Right(float i_Dist);
	void Up(float i_Dist);
	void Zoom(float i_Prc);
	void CalcVPM(bool overkil=false);
	int TestPoint(const float3& i_Pnt)const;
	int TestSphere(const float3& i_Pnt, float i_Rad)const;
	int TestAABB(const float3& i_Min,const float3& i_Max)const;
	void SetPos(const float3 &i_NewPos){m_Pos=i_NewPos;};
	void SetLookAt(const float3 &i_NewLookAt){m_LookAt=i_NewLookAt;};
	const float3 &GetPos()const{return m_Pos;};
	const float3 &GetLookAt()const{return m_LookAt;};
	const float4x4 &GetVPM()const{return VPM;};
	const float4x4 &GetVPMT()const{return VPMT;};
	const float4x4 &GetIVPM()const{return IVPM;};
	const float4x4 &GetIVPMT()const{return IVPMT;};
	const float4x4 &GetVM()const{return VM;};   // View matrix
	const float4x4 &GetPM()const{return PM;};   // Projection matrix

	const float3&	GetR()const{return m_R;}; // Right vector
	const float3&	GetU()const{return m_U;}; // Up vector
	const float3&	GetF()const{return m_F;}; // Forward vector

	void SetOrthoMode(){mode=CM_ORTHO;}
	void SetProjMode(){mode=CM_PROJ;}

	void SetFov(const double f){m_FOV = f;m_NeedUpdate=true;}
	void SetWH(int iW, int iH){W=(float)iW;H=(float)iH;};
	double GetFov()const{return m_FOV;}
	void SetCrop(const float2& c0,const float2& c1)
	{
		crop0=c0;
		crop1=c1;
	};

	CCamera(void);
	virtual ~CCamera(void);
};
