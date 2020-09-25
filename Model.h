#pragma once

#include "BaseModel.h"
#include "MinMath.h"
#include <d3d11_1.h>

struct VertexPosNormCol
{
	float3 pos;
	float3 normal;
	float3 col;
};

class CModel : public CBaseModel
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	unsigned int  indexCount;
	ID3D11ShaderResourceView* textures[16];
public:
	bool Create(ID3D11Device* device, ID3D11DeviceContext* ctx) override {return false;}
	bool Create(ID3D11Device* device, ID3D11DeviceContext* ctx, void* vertices, int nVertices, WORD* indices, int nIndices) override;
	CModel()
		: vertexBuffer(nullptr)
		, indexBuffer(nullptr)
		, indexCount(0)
	{
		for(int i=0;i<16;i++)
			textures[i]=0;
	}
	ID3D11Buffer *const GetVertexBuffer()const override { return vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer()const override { return indexBuffer; }
	int GetIndexCount()const override{return indexCount;};
	int GetVertexStride()const override{ return sizeof(VertexPosNormCol); }
	bool GetZEnabled()const override{ return true; };
	bool GetAlphaEnabled()const override{ return false; };
	const char* GetShaderSetup()const override{ return "model"; }
	ID3D11ShaderResourceView* GetTextureView(int n)const override{return textures[n];}
	void SetTextureView(int n, ID3D11ShaderResourceView* textureView) override{textures[n]=textureView;}
	~CModel() override = default;
};
