#pragma once

#include "MinMath.h"
#include "BaseModel.h"
#include <d3d11_1.h>
#include <cstdint>

class CBullets: public CBaseModel
{
	static const int SIZE = 8192;
	int count = 0;
	int committed = 0;
	VertexPosUV vb[SIZE * 4];
	WORD ib[SIZE * 6];
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11DeviceContext* devctx = nullptr;
public:
	virtual bool Create(ID3D11Device* device, ID3D11DeviceContext* ctx);
	virtual bool Create(ID3D11Device* device, ID3D11DeviceContext* ctx, void* vertices, int nVertices, WORD* indices, int nIndices) { return false; };
	CBullets() = default;
	virtual ID3D11Buffer *const GetVertexBuffer()const { return vertexBuffer; }
	virtual ID3D11Buffer* GetIndexBuffer()const { return indexBuffer; }
	virtual int GetIndexCount()const { return committed * 6; };
	virtual int GetVertexStride()const { return sizeof(VertexPosUV); }
	virtual bool GetZEnabled()const { return true; };
	virtual bool GetZWriteEnabled()const { return false; };
	virtual bool GetAlphaEnabled()const { return true; };
	virtual const char* GetShaderSetup()const { return "bullets"; }
	virtual ID3D11ShaderResourceView* GetTextureView(int n)const{return nullptr;}
	virtual void SetTextureView(int n, ID3D11ShaderResourceView* textureView){};
	bool AddBullet(const float3& p);
	void Commit();
	virtual ~CBullets() = default;
};
