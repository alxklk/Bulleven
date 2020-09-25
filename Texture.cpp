#include "Texture.h"
#include <stdio.h>

bool CTexture::Create(ID3D11Device* device, int width, int height, const unsigned char* data)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	memset(&textureDesc, 0, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = data;
	srd.SysMemPitch = width*4;
	srd.SysMemSlicePitch = 0;

	result = device->CreateTexture2D(&textureDesc, &srd, &texture);
	if(FAILED(result))
	{
		printf("FAILed to create Texture\n");
		return false;
	}

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(texture, &shaderResourceViewDesc, &shaderResourceView);
	if(FAILED(result))
	{
		printf("FAILed to create ShaderResource view\n");
		return false;
	}

	return true;
}

bool CTexture::Destroy()
{
	return true;
}

CTexture::CTexture(void)
{
}


CTexture::~CTexture(void)
{
}
