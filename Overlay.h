#pragma once

#include "MinMath.h"
#include "BaseModel.h"
#include <d3d11_1.h>

struct Vertex2D
{
	float2 pos;
	float2 uv;
};

class COverlay : public CBaseModel
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11ShaderResourceView* texture;
public:
	virtual bool Create(ID3D11Device* device);
	virtual bool Create(ID3D11Device* device, void* vertices, int nVertices, WORD* indices, int nIndices){return false;};
	COverlay()
		: vertexBuffer(nullptr)
		, indexBuffer(nullptr)
		, texture(nullptr)
	{
	}
	virtual ID3D11Buffer *const GetVertexBuffer()const { return vertexBuffer; }
	virtual ID3D11Buffer* GetIndexBuffer()const { return indexBuffer; }
	virtual int GetIndexCount()const{return 6;};
	virtual int GetVertexStride()const { return sizeof(Vertex2D); }
	virtual bool GetZEnabled()const { return false; };
	virtual bool GetAlphaEnabled()const { return true; };
	virtual const char* GetShaderSetup()const
	{
		return "overlay";
	}
	virtual ID3D11ShaderResourceView* GetTextureView(int n)const {return texture;}
	virtual void SetTextureView(int n, ID3D11ShaderResourceView* textureView){};
	void AddRect(const float2& p0, const float2& p1, const float2& t0, const float2& t1);
	virtual ~COverlay()
	{
	};
};
