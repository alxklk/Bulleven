#pragma once

#include "MinMath.h"
#include "BaseModel.h"
#include <d3d11_1.h>
#include <cstdint>

class CTextOverlay : public CBaseModel
{
	static const int SIZE = 4096;
	int count = 0;
	int committed = 0;
	Vertex2D vb[SIZE * 4];
	WORD ib[SIZE * 6];
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11ShaderResourceView* texture = nullptr;
	ID3D11DeviceContext* devctx = nullptr;
public:
	virtual bool Create(ID3D11Device* device, ID3D11DeviceContext* ctx);
	virtual bool Create(ID3D11Device* device, ID3D11DeviceContext* ctx, void* vertices, int nVertices, WORD* indices, int nIndices) { return false; };
	CTextOverlay()=default;
	virtual ID3D11Buffer *const GetVertexBuffer()const { return vertexBuffer; }
	virtual ID3D11Buffer* GetIndexBuffer()const { return indexBuffer; }
	virtual int GetIndexCount()const { return committed * 6; };
	virtual int GetVertexStride()const { return sizeof(Vertex2D); }
	virtual bool GetZEnabled()const { return false; };
	virtual bool GetAlphaEnabled()const { return true; };
	virtual const char* GetShaderSetup()const{return "overlay";}
	virtual ID3D11ShaderResourceView* GetTextureView(int n)const
	{
		if (n == 0)
		{
			return texture;
		}
		return nullptr;
	}
	virtual void SetTextureView(int n, ID3D11ShaderResourceView* textureView)
	{
		if (n == 0)
		{
			texture = textureView;
		}
	};
	bool AddRect(const float2& p0, const float2& p1, const float2& t0, const float2& t1, uint32_t col=0xffffffff);
	void Commit();
	virtual ~CTextOverlay() = default;
};
