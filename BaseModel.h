#pragma once

#include <d3d11_1.h>
#include "MinMath.h"

struct VertexPosNormCol
{
	float3 pos;
	float3 normal;
	float3 col;
};

struct VertexPosUV
{
	float3 pos;
	float2 uv;
};

struct Vertex2D
{
	float2 pos;
	float2 uv;
	float4 col;
};

class CBaseModel
{
public:
	virtual bool Create(ID3D11Device* device, ID3D11DeviceContext* ctx) = 0;
	virtual bool Create(ID3D11Device* device, ID3D11DeviceContext* ctx, void* vertices, int nVertices, WORD* indices, int nIndices) = 0;
	virtual ID3D11Buffer* const GetVertexBuffer()const = 0;
	virtual ID3D11Buffer* GetIndexBuffer()const = 0;
	virtual int GetIndexCount()const = 0;
	virtual int GetVertexStride()const = 0;
	virtual bool GetZEnabled()const = 0;
	virtual bool GetZWriteEnabled()const = 0;
	virtual bool GetAlphaEnabled()const = 0;
	virtual const char* GetShaderSetup()const = 0;
	virtual ID3D11ShaderResourceView* GetTextureView(int n)const = 0;
	virtual void SetTextureView(int n, ID3D11ShaderResourceView* textureView) = 0;
	virtual ~CBaseModel() {}
};
