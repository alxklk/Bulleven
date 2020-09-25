#include "Model.h"
#include <stdio.h>

bool CModel::Create(ID3D11Device* device, void* vertices, int nVertices, WORD* indices, int nIndices)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth=sizeof(VertexPosNormCol) * nVertices;
	vertexBufferDesc.CPUAccessFlags=0;
	vertexBufferDesc.Usage=D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	memset(&resourceData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem=vertices;

	HRESULT hr=device->CreateBuffer(&vertexBufferDesc, &resourceData, &vertexBuffer);
	if(FAILED(hr))
	{
		return false;
	}

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth=sizeof(WORD) * nIndices;
	indexBufferDesc.CPUAccessFlags=0;
	indexBufferDesc.Usage=D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride=sizeof(WORD);
	resourceData.pSysMem=indices;

	indexCount=nIndices;


	hr=device->CreateBuffer(&indexBufferDesc, &resourceData, &indexBuffer);
	if(FAILED(hr))
	{
		return false;
	}

	return true;
}
