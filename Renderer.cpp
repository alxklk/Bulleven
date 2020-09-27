#include "Renderer.h"
#include <stdio.h>
#include <d3dcompiler.h>

template<typename T>
inline void SafeRelease(T& ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

bool CRenderer::Init(HWND win)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(swapChainDesc));

	RECT clientRect;
	GetClientRect(win, &clientRect);

	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = win;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.Flags = 0;

	UINT createDeviceFlags = 0;

	if (::GetAsyncKeyState(VK_SHIFT))
	{
		MessageBeep(0xFFFFFFFF);
		printf("Debug runtime selected\n");
		createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
		debug = true;
	}

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &d3dSwapChain, &d3dDevice,
		&featureLevel, &d3dDeviceContext);
	if (FAILED(hr))
	{
		puts("D3D Device failed to init\n");
		return false;
	}

	printf("D3D Device Feature Level %04x\n", featureLevel);

	ID3D11Texture2D* backBuffer;
	hr = d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	hr = d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &d3dRenderTargetView);
	if (FAILED(hr))
	{
		return false;
	}

	SafeRelease(backBuffer);

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	memset(&depthStencilBufferDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = width;
	depthStencilBufferDesc.Height = height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = d3dDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &d3dDepthStencilBuffer);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	hr = d3dDevice->CreateDepthStencilView(d3dDepthStencilBuffer, nullptr, &d3dDepthStencilView);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	memset(&depthStencilStateDesc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = FALSE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = FALSE;

	hr = d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &d3dDepthStencilStateOff);

	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = FALSE;
	hr = d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &d3dDepthStencilStateOn);

	{
		D3D11_BLEND_DESC blendStateDescOff;
		memset(&blendStateDescOff, 0, sizeof(D3D11_BLEND_DESC));
		blendStateDescOff.AlphaToCoverageEnable = FALSE;
		blendStateDescOff.IndependentBlendEnable = FALSE;
		blendStateDescOff.RenderTarget[0].BlendEnable = FALSE;
		blendStateDescOff.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = d3dDevice->CreateBlendState(&blendStateDescOff, &d3dBlendStateOff);
	}
	{
		D3D11_BLEND_DESC blendStateDescOn;
		memset(&blendStateDescOn, 0, sizeof(D3D11_BLEND_DESC));
		blendStateDescOn.AlphaToCoverageEnable = FALSE;
		blendStateDescOn.IndependentBlendEnable = FALSE;
		blendStateDescOn.RenderTarget[0].BlendEnable = TRUE;
		blendStateDescOn.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendStateDescOn.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescOn.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDescOn.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescOn.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescOn.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescOn.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		hr = d3dDevice->CreateBlendState(&blendStateDescOn, &d3dBlendStateOn);
	}
	D3D11_SAMPLER_DESC sampDesc;
	memset(&sampDesc, 0, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = d3dDevice->CreateSamplerState(&sampDesc, &d3dSamplerState);



	D3D11_RASTERIZER_DESC rasterizerDesc;
	memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	hr = d3dDevice->CreateRasterizerState(&rasterizerDesc, &d3dRasterizerState);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	Viewport.Width = (float)width;
	Viewport.Height = (float)height;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	return true;
}



bool CRenderer::UseShaderSetup(const char* shaderSetup)
{
	TShaderSterupMap::iterator foundShaderSetup = shaderSetupMap.find(shaderSetup);
	if (foundShaderSetup == shaderSetupMap.end())
	{
		currentShaderSetup = 0;
		return false;
	}
	currentShaderSetup = foundShaderSetup->second;

	d3dDeviceContext->IASetInputLayout(currentShaderSetup->inputLayout);
	d3dDeviceContext->VSSetShader(currentShaderSetup->vertexShader, nullptr, 0);
	d3dDeviceContext->PSSetShader(currentShaderSetup->pixelShader, nullptr, 0);
	if (currentShaderSetup->constantBuffer)
	{
		d3dDeviceContext->PSSetConstantBuffers(0, 1, &currentShaderSetup->constantBuffer);
		d3dDeviceContext->VSSetConstantBuffers(0, 1, &currentShaderSetup->constantBuffer);
	}
	return true;
}

bool CRenderer::UpdateShaderConstants(void* constantBuffer)
{
	ID3D11Buffer* cb = 0;
	if (currentShaderSetup && currentShaderSetup->constantBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE mr;
		HRESULT hr = d3dDeviceContext->Map(currentShaderSetup->constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
		if (FAILED(hr))
			return false;
		for (int i = 0; i < currentShaderSetup->constantsSize; i++)
			((char*)mr.pData)[i] = ((char*)constantBuffer)[i];
		d3dDeviceContext->Unmap(currentShaderSetup->constantBuffer, 0);
		return true;
	}
	return false;
}

void CRenderer::RenderScene(CScene* scene)
{
	int x = 0;
	int y = 0;
	float clearColor[4] = { 0.2f,
		0.2f + 0.1f * (y & 0xff) / 255.0f,
		0.2f + 0.1f * (x & 0xff) / 255.0f, 1 };
	float clearDepth = 1.0f;
	UINT8 clearStencil = 0;
	d3dDeviceContext->ClearRenderTargetView(d3dRenderTargetView, clearColor);
	d3dDeviceContext->ClearDepthStencilView(d3dDepthStencilView, D3D11_CLEAR_DEPTH, clearDepth, 0);


	d3dDeviceContext->RSSetState(d3dRasterizerState);
	d3dDeviceContext->RSSetViewports(1, &Viewport);

	size_t modelIndex = 0;
	while (true)
	{
		CBaseModel* model = scene->GetModel(modelIndex);
		if (model == nullptr)
			break;

		const char* shaderSetup = model->GetShaderSetup();
		if (shaderSetup)
			UseShaderSetup(shaderSetup);
		else
			printf("Shader setup %s not found\n", shaderSetup);

		if (model->GetIndexBuffer() && model->GetVertexBuffer())
		{
			d3dDeviceContext->IASetIndexBuffer(model->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
			d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			ID3D11Buffer* const  vertexBuffer = model->GetVertexBuffer();
			const UINT offset = 0;
			const UINT vertexStride = model->GetVertexStride();
			d3dDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &offset);

			for (int i = 0; i < 16; i++)
			{
				ID3D11ShaderResourceView* textureView = model->GetTextureView(i);
				if (textureView)
				{
					d3dDeviceContext->PSSetSamplers(i, 1, &d3dSamplerState);
					d3dDeviceContext->PSSetShaderResources(i, 1, &textureView);
				}
			}

			if (model->GetZEnabled())
				d3dDeviceContext->OMSetDepthStencilState(d3dDepthStencilStateOn, 0);
			else
				d3dDeviceContext->OMSetDepthStencilState(d3dDepthStencilStateOff, 0);

			float blendFactor[4] = { 0,0,0,0 };
			if (model->GetAlphaEnabled())
				d3dDeviceContext->OMSetBlendState(d3dBlendStateOn, blendFactor, 0xffffffff);
			else
				d3dDeviceContext->OMSetBlendState(d3dBlendStateOff, blendFactor, 0xffffffff);

			d3dDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);
		}



		modelIndex++;
	}

	d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, d3dDepthStencilView);

	d3dSwapChain->Present(0, 0);
}

bool CRenderer::Done()
{

	SafeRelease(d3dRenderTargetView);
	SafeRelease(d3dDepthStencilView);
	SafeRelease(d3dDepthStencilBuffer);

	SafeRelease(d3dDepthStencilStateOff);
	SafeRelease(d3dDepthStencilStateOn);
	SafeRelease(d3dRasterizerState);
	SafeRelease(d3dSamplerState);

	SafeRelease(d3dDeviceContext);
	SafeRelease(d3dSwapChain);

	auto iShader = shaderSetupMap.begin();
	while (iShader != shaderSetupMap.end())
	{
		SShaderSetup& sSetup = *iShader->second;
		SafeRelease(sSetup.constantBuffer);
		SafeRelease(sSetup.inputLayout);
		SafeRelease(sSetup.pixelShader);
		SafeRelease(sSetup.vertexShader);
		++iShader;
	}

	if (debug)
	{
		ID3D11Debug* d3dDebug = nullptr;
		d3dDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)(&d3dDebug));
		if (::GetAsyncKeyState(VK_SHIFT))
		{
			d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		}
		else
		{
			d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
		}

	}
	SafeRelease(d3dDevice);
	return true;
}


bool CRenderer::CreateShaderSetup(
	const char* setupName,
	const char* vertexShaderFile,
	const char* pixelShaderFile,
	const D3D11_INPUT_ELEMENT_DESC* layoutDesc,
	int nVL,
	int constantsSize)
{
	ID3DBlob* vsBlob = 0;
	ID3D11InputLayout* inputLayout = 0;

	ID3D11VertexShader* vertexShader = LoadVertexShader(vertexShaderFile, "main", "vs_4_0", &vsBlob);
	ID3D11PixelShader* pixelShader = LoadPixelShader(pixelShaderFile, "main", "ps_4_0");

	if (!vsBlob)
		return false;

	HRESULT hr;
	hr = d3dDevice->CreateInputLayout(layoutDesc, nVL, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
	if (FAILED(hr))
	{
		return false;
	}

	ID3D11Buffer* constantBuffer = 0;

	if (constantsSize)
	{
		D3D11_BUFFER_DESC constantBufferDesc;
		memset(&constantBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.ByteWidth = constantsSize;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		hr = d3dDevice->CreateBuffer(&constantBufferDesc, 0, &constantBuffer);
		if (FAILED(hr))
		{
			printf("Constant Buffer create failed\n");
			return false;
		}
	}

	SShaderSetup* setup = new SShaderSetup;
	setup->inputLayout = inputLayout;
	setup->pixelShader = pixelShader;
	setup->vertexShader = vertexShader;
	setup->constantBuffer = constantBuffer;
	setup->constantsSize = constantsSize;
	shaderSetupMap[setupName] = setup;
	SafeRelease(vsBlob);
	return true;
}

#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

ID3DBlob* LoadShader(const char* fileName, const char* entryPoint, const char* profile)
{
	ID3DBlob* pShaderBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#pragma warning(disable : 4996)

	FILE* f = fopen(fileName, "rb");
	if (!f)
	{
		char buf[1024];
		GetCurrentDirectoryA(1024, buf);
		printf("WD is %s\n", buf);
		printf("Shader %s not found\n", fileName);
		return 0;
	}
	fseek(f, 0, SEEK_END);
	auto len = ftell(f);
	fseek(f, 0, SEEK_SET);
	std::vector<char>content(len);
	fread(content.data(), 1, len, f);
	fclose(f);


	HRESULT hr = D3DCompile(content.data(), content.size(), fileName,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile, flags, 0, &pShaderBlob, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			char* errorMessage = (char*)pErrorBlob->GetBufferPointer();
			puts(errorMessage);
			SafeRelease(pShaderBlob);
			SafeRelease(pErrorBlob);
		}
		return 0;
	}
	return pShaderBlob;
}


ID3D11VertexShader* CRenderer::LoadVertexShader(const char* fileName, const char* entryPoint, const char* profile, ID3DBlob** shaderBlob)
{
	ID3D11VertexShader* newShader = nullptr;

	if (*shaderBlob = LoadShader(fileName, entryPoint, profile))
	{
		d3dDevice->CreateVertexShader((*shaderBlob)->GetBufferPointer(), (*shaderBlob)->GetBufferSize(), nullptr, &newShader);
	}
	return newShader;
}

ID3D11PixelShader* CRenderer::LoadPixelShader(const char* fileName, const char* entryPoint, const char* profile)
{
	ID3D11PixelShader* newShader = nullptr;

	if (ID3DBlob* shaderBlob = LoadShader(fileName, entryPoint, profile))
	{
		d3dDevice->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &newShader);
		SafeRelease(shaderBlob);
	}
	return newShader;
}
