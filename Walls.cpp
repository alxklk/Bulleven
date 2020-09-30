#include "Walls.h"
#include "Font.h"

bool CWalls::Create(ID3D11Device* device, ID3D11DeviceContext* ctx)
{
	devctx = ctx;
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosNormCol) * 4 * SIZE;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA resourceData;
	memset(&resourceData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem = vb;

	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &resourceData, &vertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = 6 * sizeof(WORD) * SIZE;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride = sizeof(WORD);
	for (int i = 0; i < SIZE; i++)
	{
		ib[i * 6 + 0] = i * 4 + 0;
		ib[i * 6 + 1] = i * 4 + 1;
		ib[i * 6 + 2] = i * 4 + 2;
		ib[i * 6 + 3] = i * 4 + 2;
		ib[i * 6 + 4] = i * 4 + 1;
		ib[i * 6 + 5] = i * 4 + 3;
	}
	resourceData.pSysMem = ib;

	hr = device->CreateBuffer(&indexBufferDesc, &resourceData, &indexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void CWalls::AddWall(const float2& p0, const float2& p1)
{
	if (count >= SIZE - 1)return;

	vb[count * 4].pos = { p0.x,p0.y,.1 };
	vb[count * 4 + 1].pos = { p1.x,p1.y,.1 };
	vb[count * 4 + 2].pos = { p0.x,p0.y,0 };
	vb[count * 4 + 3].pos = { p1.x,p1.y,0 };
	count++;
}

void CWalls::Commit()
{
	D3D11_MAPPED_SUBRESOURCE resource;
	devctx->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &vb[0], count * sizeof(VertexPosNormCol) * 4);
	devctx->Unmap(vertexBuffer, 0);
	committed = count;
	count = 0;
}


