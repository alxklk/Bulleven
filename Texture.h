#pragma once

#include <d3d11.h>

class CTexture
{
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* shaderResourceView;
public:
	CTexture(void);
	bool Create(ID3D11Device* device, int width, int height, const unsigned char* data);
	bool Destroy();
	ID3D11ShaderResourceView* GetShaderResourceView(){return shaderResourceView;}
	~CTexture(void);
};

