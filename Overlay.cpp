#include "Overlay.h"
#include "Font.h"
#include <stdio.h>

bool COverlay::Create(ID3D11Device* device)
{
	static Vertex2D vb[4]={
		{{-1,-1},{0,0}},
		{{-1, 1},{0,0}},
		{{ 1, 1},{0,0}},
		{{ 1,-1},{0,0}},
	};

	static unsigned short ib[6]={0,1,2,0,2,3};

	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth=sizeof(Vertex2D) * 4;
	vertexBufferDesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.Usage=D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA resourceData;
	memset(&resourceData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem=vb;

	HRESULT hr=device->CreateBuffer(&vertexBufferDesc, &resourceData, &vertexBuffer);
	if(FAILED(hr))
	{
		return false;
	}

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth=6*sizeof(WORD);
	indexBufferDesc.CPUAccessFlags=0;
	indexBufferDesc.Usage=D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride=sizeof(WORD);
	resourceData.pSysMem=ib;

	hr=device->CreateBuffer(&indexBufferDesc, &resourceData, &indexBuffer);
	if(FAILED(hr))
	{
		return false;
	}

	return true;
}

bool AddRect(const float2& p0, const float2& p1, const float2& t0, const float2& t1)
{
	return false;
}

