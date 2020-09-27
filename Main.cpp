#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Averager.h"
#include "Renderer.h"
#include "Texture.h"
#include "WinUI.h"
#include "Overlay.h"
#include "Walls.h"
#include "Bullets.h"
#include "Font.h"
#include "BaseModel.h"
#include "../Bullimar/BulletMan.h"
#include "rand.h"
#include <windows.h>
#include <stdio.h>
#include <math.h>

CBulletMan bm;
float btime=0;

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
			printf("Quiting...\n");
			gAppState.quitRequested = true;
		}
		else if (wpar == VK_SPACE)
		{
			btime+=1.f/30;
			bm.Update(btime);
		}
	}
	else if (imsg == WM_KEYUP)
	{
	}
	return DefWindowProc(hwnd, imsg, wpar, lpar);
}

void AddOverlayTextLine(CTextOverlay* text, const std::string& s, int x, int y, uint32_t col = 0xffffffff)
{
	float size = 16.f;
	float w2 = gAppState.width / 2.f;
	float h2 = gAppState.height / 2.f;
	for (size_t i = 0; i < s.size(); i++)
	{
		float dx = size / w2;
		float dy = size / h2;
		float posx = x / w2 + i * (dx + 2 / w2) - 1.f;
		float posy = 1.f - y / h2;
		float tx = (s[i] % 16) / 16.f;
		float ty = (s[i] / 16) / 8.f;
		text->AddRect({ posx,posy }, { posx + dx,posy - dy }, { tx,ty }, { tx + 1.f / 16.f,ty + 1.f / 8.f }, col);
	}
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

	SetWindowLongPtr(gAppState.win, GWLP_USERDATA, (LONG_PTR) & (gAppState));

	{
		CLehmerRand rnd;
		rnd.SetSeed(37087534);
		for(int i=0;i<1000;i++)
		{
			float2 p0(rnd.F(),rnd.F());
			p0*=600;
			p0+=float2(100,100);
			float2 p1=p0+float2(rnd.F(),rnd.F())*200-float2(100,100);
			bm.walls.push_back({flt2(p0.x,p0.y),flt2(p1.x,p1.y)});
		}
		rnd.SetSeed(1127741);
		for(int i=0;i<1000;i++)
		{
			float2 p(rnd.F(),rnd.F());
			p*=600;
			p+=float2(100,100);
			float2 d=float2(rnd.N10(),rnd.N10());
			bm.Fire(flt3(p.x,p.y,0),flt3(d.x,d.y,0).norm(),100+rnd.F()*100,rnd.F()*10,5);
		}
		bm.Update(0);
	}

	CRenderer* renderer = new CRenderer;
	if (!renderer->Init(gAppState.win))
	{
		printf("Failed to initialize renderer\n");
		delete renderer;
		return 1;
	}


	CTexture* tex = new CTexture;
	unsigned char tc[fpsfontHeight * fpsfontWidth * 4];
	for (int i = 0; i < fpsfontHeight * fpsfontWidth; i++)
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

	CBaseModel* floor = new CModel;
	floor->Create(renderer->GetDevice(), renderer->GetDeviceContext(),
		(VertexPosNormCol*)vb, sizeof(vb) / sizeof(VertexPosNormCol),
		ib, sizeof(ib) / sizeof(unsigned short)
	);

	scene->AddModel(floor);

	CWalls* walls = new CWalls;
	walls->Create(renderer->GetDevice(), renderer->GetDeviceContext());
	scene->AddModel(walls);

	CBullets* bullets = new CBullets;
	bullets->Create(renderer->GetDevice(), renderer->GetDeviceContext());
	scene->AddModel(bullets);


	CTextOverlay* text= new CTextOverlay;
	text->Create(renderer->GetDevice(), renderer->GetDeviceContext());
	text->SetTextureView(0, tex->GetShaderResourceView());
	text->SetTextureView(1, 0);
	scene->AddModel(text);

	double time = GetTime();
	TAvg<double, 10> avgFps;
	bool first = true;
	while (1)
	{
		double newTime = GetTime();
		double deltaT = newTime - time;
		time = newTime;
		//while (!isPending())
		//{
		//	SleepEx(5, TRUE);
		//}
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
				renderer->CreateShaderSetup("walls", "..\\shaders\\WallsVS.hlsl",
					"..\\shaders\\WallsPS.hlsl", vertexLayoutDesc, 3, sizeof(float) * 16);
			}
			{
				D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex2D, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex2D, uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex2D, col), D3D11_INPUT_PER_VERTEX_DATA, 0}
				};
				renderer->CreateShaderSetup("overlay", "..\\shaders\\2dVS.hlsl",
					"..\\shaders\\2dPS.hlsl", vertexLayoutDesc, 3, 0);
			}
			{
				D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosUV, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosUV, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
				};
				renderer->CreateShaderSetup("bullets", "..\\shaders\\BulletsVS.hlsl",
					"..\\shaders\\BulletsPS.hlsl", vertexLayoutDesc, 2, sizeof(float) * 16);
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
				float dy = gAppState.lastMouseEvent.delta_y / 20.0f;
				float dx = gAppState.lastMouseEvent.delta_x / 20.0f;

				float l = (float)(dx * dx + dy * dy);

				float4 rotQuat((float)(-dy * sin(l)), (float)(dx * sin(l)), 0, float(cos(l)));
				float4x4 rotMat;
				QuaternionToMatrix(rotQuat, rotMat);
				cb_model.wm = cb_model.wm * rotMat;
			}
		}
		renderer->UseShaderSetup("model");
		renderer->UpdateShaderConstants((void*)&cb_model);
		renderer->UseShaderSetup("walls");
		renderer->UpdateShaderConstants((void*)&cb_model);
		renderer->UseShaderSetup("bullets");
		renderer->UpdateShaderConstants((void*)&cb_model);

		if (!first)
			avgFps.Add(1.0 / deltaT);
		first = false;

		char s[128];
		sprintf(s, "Avg FPS/100 frames: %f", avgFps.Get());
		AddOverlayTextLine(text, s, 0, 0, 0xff8080ff);
		sprintf(s, "FPS: %f", 1. / deltaT);
		AddOverlayTextLine(text, s, 0, 16);
		AddOverlayTextLine(text, "QUICK BROWN FOX JUMPS OVER THE LAZY DOG !&^#$%&#$%<>-=+", 0, 32);
		AddOverlayTextLine(text, "quick brown fox jumps over the lazy dog ? 0123456789{}()", 0, 48);
		text->Commit();
		/*
		for (int i = 0; i < 1000; i++)
		{
			float x0 = (rand() / float(RAND_MAX) * 2.f - 1.f) * 0.8f;
			float y0 = (rand() / float(RAND_MAX) * 2.f - 1.f) * 0.8f;
			float x1 = x0 + (rand() / float(RAND_MAX) * 2.f - 1.f) * 0.2f;
			float y1 = y0 + (rand() / float(RAND_MAX) * 2.f - 1.f) * 0.2f;
			walls->AddWall({ x0,y0 }, { x1,y1 });
		}
		walls->Commit();

		for (int i = 0; i < 1000; i++)
		{
			float x = (rand() / float(RAND_MAX) * 2.f - 1.f) * 0.9f;
			float y = (rand() / float(RAND_MAX) * 2.f - 1.f) * 0.9f;
			bullets->AddBullet({ x,y,0.2 });
		}
		bullets->Commit();
		*/

		for(int i=0;i<bm.walls.size();i++)
		{
			CBulletMan::Wall& w=bm.walls[i];
			walls->AddWall({ w.end0.x/400-1, w.end0.y/400-1 }, { w.end1.x/400-1, w.end1.y/400-1 });
		}
		walls->Commit();

		for(int i=0;i<bm.activeBullets.size();i++)
		{
			const CBulletMan::Bullet& b=bm.activeBullets[i];
			if((b.startTime<=btime)&&(b.endTime>btime))
			{
				float2 pos={b.pos.x,b.pos.y};
				float2 dir={b.dir.x,b.dir.y};
				pos=pos+dir*(btime-b.startTime);
				bullets->AddBullet({ pos.x/400-1, pos.y/400-1,.08 });
			}
		}
		bullets->Commit();

		renderer->RenderScene(scene);

		gAppState.frameStamp++;

	}

	renderer->Done();

	::OutputDebugString("Quiting...\n");

	return 0;
}
