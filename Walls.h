#pragma once

#include "MinMath.h"
#include "BaseModel.h"
#include <d3d11_1.h>
#include <cstdint>

class CWalls: public CBaseModel
{
	static const int SIZE = 8192;
	int count = 0;
	int committed = 0;
	VertexPosNormCol vb[SIZE * 4];
	WORD ib[SIZE * 6];
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11DeviceContext* devctx = nullptr;
public:
	virtual bool Create(ID3D11Device* device, ID3D11DeviceContext* ctx);
	virtual bool Create(ID3D11Device* device, ID3D11DeviceContext* ctx, void* vertices, int nVertices, WORD* indices, int nIndices) { return false; };
	CWalls() = default;
	virtual ID3D11Buffer *const GetVertexBuffer()const { return vertexBuffer; }
	virtual ID3D11Buffer* GetIndexBuffer()const { return indexBuffer; }
	virtual int GetIndexCount()const { return committed * 6; };
	virtual int GetVertexStride()const { return sizeof(VertexPosNormCol); }
	virtual bool GetZEnabled()const { return true; };
	virtual bool GetAlphaEnabled()const { return false; };
	virtual const char* GetShaderSetup()const { return "walls"; }
	virtual ID3D11ShaderResourceView* GetTextureView(int n)const{return nullptr;}
	virtual void SetTextureView(int n, ID3D11ShaderResourceView* textureView){};
	bool AddWall(const float2& p0, const float2& p1);
	void Commit();
	virtual ~CWalls() = default;
};
