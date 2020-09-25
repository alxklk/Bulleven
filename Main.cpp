#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Averager.h"
#include "Renderer.h"
#include "Texture.h"
#include "WinUI.h"
#include "Overlay.h"
#include "Font.h"
#include <windows.h>
#include <stdio.h>
#include <math.h>

struct SAppState
{
public:
	HWND win;
	int width;
	int height;
	bool quitRequested;
	bool reloadShaders;
	int frameStamp;
	SMouseEvent lastMouseEvent;

	SAppState()
		: width(900)
		, height(900)
		, quitRequested(false)
		, reloadShaders(true)
		, frameStamp(0)
	{

	}

}gAppState;

void stop(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar)
{
	PostQuitMessage(0);
	gAppState.quitRequested = true;
	::OutputDebugString("stop issued\n");
}

LRESULT CALLBACK wp(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar)
{
	if (ProcessMouseEvent(hwnd, imsg, wpar, lpar, gAppState.lastMouseEvent))
	{
		gAppState.lastMouseEvent.frame = gAppState.frameStamp;
	}
	else if (imsg == WM_DESTROY)
	{
		stop(hwnd, imsg, wpar, lpar);
	}
	else if (imsg == WM_KEYDOWN)
	{
		if (wpar == VK_F5)
		{
			printf("Reload shaders\n");
			gAppState.reloadShaders = true;
		}
		else if (wpar == VK_ESCAPE)
		{
			printf("Reload shaders\n");
			gAppState.quitRequested = true;
		}
	}
	else if (imsg == WM_KEYUP)
	{
	}
	return DefWindowProc(hwnd, imsg, wpar, lpar);
}

int main(int argc, char* argv[])
{

	HCURSOR arrowCursor = LoadCursor(0, IDC_ARROW);

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), 0, wp, 0, 0, 0, 0, 0, 0, 0, "APP01" };
	wc.hCursor = arrowCursor;
	RegisterClassEx(&wc);

	RECT wrect = { 0, 0, gAppState.width, gAppState.height };
	AdjustWindowRectEx(&wrect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW);
	gAppState.win = CreateWindowEx(WS_EX_APPWINDOW, "APP01", "Bullets", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		wrect.right - wrect.left, wrect.bottom - wrect.top, 0, 0, 0, 0);

	ShowWindow(gAppState.win, SW_SHOWNORMAL);
	UpdateWindow(gAppState.win);

	SetWindowLongPtr(gAppState.win, GWLP_USERDATA, (LONG_PTR)&(gAppState));

	CRenderer* renderer = new CRenderer;
	if (!renderer->Init(gAppState.win))
	{
		printf("Failed to initialize renderer\n");
		delete renderer;
		return 1;
	}


	CTexture* tex = new CTexture;
	unsigned char tc[fpsfontHeight*fpsfontWidth*4];
	for (int i = 0; i < fpsfontHeight*fpsfontWidth; i++)
	{
		if (fpsfontdata[i] != ' ')
		{
			tc[i * 4] = 0xff;
			tc[i * 4 + 1] = 0xff;
			tc[i * 4 + 2] = 0xff;
			tc[i * 4 + 3] = 0xff;
		}
		else
		{
			tc[i * 4] = 0xff;
			tc[i * 4 + 1] = 0x00;
			tc[i * 4 + 2] = 0x00;
			tc[i * 4 + 3] = 0x00;
		}
	}
	tex->Create(renderer->GetDevice(), fpsfontWidth, fpsfontHeight, tc);

	CScene* scene = new CScene;

	static VertexPosNormCol vb[4] = {
		{{-1,-1,0},{0,0,1},{1,1,0}},
		{{ 1,-1,0},{0,0,1},{0,1,0}},
		{{-1, 1,0},{0,0,1},{1,0,0}},
		{{ 1, 1,0},{0,0,1},{0,0,0}},
	};

	static unsigned short ib[6] = { 0,1,2,2,1,3 };

	CBaseModel* model = new CModel;
	model->Create(renderer->GetDevice(), renderer->GetDeviceContext(),
		(VertexPosNormCol*)vb, sizeof(vb) / sizeof(VertexPosNormCol),
		ib, sizeof(ib) / sizeof(unsigned short)
	);

	model->SetTextureView(0, tex->GetShaderResourceView());
	model->SetTextureView(1, 0);
	scene->AddModel(model);

	CBaseModel* overlay = new CTextOverlay;
	overlay->Create(renderer->GetDevice(),renderer->GetDeviceContext());
	overlay->SetTextureView(0, tex->GetShaderResourceView());
	overlay->SetTextureView(1, 0);
	scene->AddModel(overlay);

	double time = GetTime();
	TAvg<double, 10> avgFps;
	bool first = true;
	while (1)
	{
		double newTime = GetTime();
		double deltaT = newTime - time;
		time = newTime;
		while (!isPending())
		{
			SleepEx(5, TRUE);
		}
		while (pollQueue())
		{
			if (gAppState.quitRequested)
				break;
		}
		if (gAppState.quitRequested)
			break;
		if (gAppState.reloadShaders)
		{
			gAppState.reloadShaders = false;

			{
				D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosNormCol, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosNormCol, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosNormCol, col), D3D11_INPUT_PER_VERTEX_DATA, 0}
				};
				renderer->CreateShaderSetup("model", "..\\shaders\\RenderModelVS.hlsl",
					"..\\shaders\\RenderModelPS.hlsl", vertexLayoutDesc, 3, sizeof(float) * 16);
			}
			{
				D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex2D, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex2D, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
				};
				renderer->CreateShaderSetup("overlay", "..\\shaders\\2dVS.hlsl",
					"..\\shaders\\2dPS.hlsl", vertexLayoutDesc, 2, sizeof(float) * 4);
			}
		}

		static struct S_CBModel
		{
			float4x4 wm;
			S_CBModel() { wm = float4x4::Identity(); }
		}cb_model;

		if (gAppState.lastMouseEvent.frame == gAppState.frameStamp)
		{
			if (gAppState.lastMouseEvent.button_l)
			{
				float dy = gAppState.lastMouseEvent.delta_y / 100.0f;
				float dx = gAppState.lastMouseEvent.delta_x / 100.0f;

				float l = (float)sqrt(dx*dx + dy * dy);

				float4 rotQuat((float)(-dy * sin(l)), (float)(dx*sin(l)), 0, float(cos(l)));
				float4x4 rotMat;
				QuaternionToMatrix(rotQuat, rotMat);
				cb_model.wm = cb_model.wm*rotMat;
			}
		}
		renderer->UseShaderSetup("model");
		renderer->UpdateShaderConstants((void*)&cb_model);

		static struct S_CBOverlay
		{
			float fps;
			float x[3];
		}cb_overlay;
		if(!first)
			avgFps.Add(1.0 / deltaT);
		first = false;
		cb_overlay.fps = avgFps.Get();
		char s[64];
		sprintf(s, "%f", cb_overlay.fps);
		SetWindowText(gAppState.win, s);

		CTextOverlay* text = dynamic_cast<CTextOverlay*>(overlay);
		text->AddRect({ 0,0 }, { .5,.5 }, { 0,0 }, { 1,1 });
		float2 mp(gAppState.lastMouseEvent.x, gAppState.lastMouseEvent.y);
		mp.x = mp.x / 450 - 1;
		mp.y = - mp.y / 450 + 1;
		text->AddRect({mp.x,mp.y}, {mp.x+.1f,mp.y+.1f}, { 0,0 }, { 1,1 });
		text->Commit();

		renderer->UseShaderSetup("overlay");
		renderer->UpdateShaderConstants((void*)&cb_overlay);
		renderer->RenderScene(scene);

		gAppState.frameStamp++;

	}

	renderer->Done();

	::OutputDebugString("Quiting...\n");

	return 0;
}
