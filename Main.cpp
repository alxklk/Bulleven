#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "Averager.h"
#include "Renderer.h"
#include "Texture.h"
#include "WinUI.h"
#include "BaseModel.h"
#include "Floor.h"
#include "Walls.h"
#include "Bullets.h"
#include "Font.h"
#include "Overlay.h"
#include "BulletMan.h"
#include "Camera.h"
#include "rand.h"

struct SAppState
{
	HWND win;
	int width = 1600;
	int height = 900;
	bool quitRequested = false;
	bool reloadShaders = true;
	int frameStamp = 0;
	SMouseEvent lastMouseEvent;
	CBulletMan bm;
	float btime = 0;
	CCamera cam;
	HANDLE threadHandle = INVALID_HANDLE_VALUE;
	SAppState() = default;
}gas;

void testScenario1()
{
	if (gas.threadHandle != INVALID_HANDLE_VALUE)
	{
		TerminateThread(gas.threadHandle, 0);
		gas.threadHandle = INVALID_HANDLE_VALUE;
	};
	gas.btime = 0;
	gas.bm.Clear();
	void* data = nullptr;
	CLehmerRand rnd;
	rnd.SetSeed(37087534);
	for (int i = 0; i < 1000; i++)
	{
		float2 p0(rnd.F(), rnd.F());
		p0 *= 600;
		p0 += float2(100, 100);
		float2 p1 = p0 + float2(rnd.F(), rnd.F()) * 200 - float2(100, 100);
		gas.bm.AddWall({ float2(p0.x,p0.y),float2(p1.x,p1.y) });
	}
	gas.threadHandle = CreateThread(0, 65536, [](void*)->DWORD
	{
		CLehmerRand rnd;
		rnd.SetSeed(1127741);
		for (;;)
		{
			for (int i = 0; i < 30; i++)
			{
				float2 p(rnd.F(), rnd.F());
				p *= 600;
				p += float2(100, 100);
				float2 d = float2(rnd.N10(), rnd.N10());
				gas.bm.Fire(float3(p.x, p.y, 0), float3(d.x, d.y, 0).norm(), 30 + rnd.F() * 50, gas.btime + .5f + rnd.F(), 5);
			}
			Sleep(16);
		}
		return 0;
	}
	, data, 0, 0);
	gas.bm.Update(0);
}

void testScenario2()
{
	if (gas.threadHandle != INVALID_HANDLE_VALUE)
	{
		TerminateThread(gas.threadHandle, 0);
		gas.threadHandle = INVALID_HANDLE_VALUE;
	};
	gas.btime = 0;
	gas.bm.Clear();
	void* data = nullptr;
	CLehmerRand rnd;
	rnd.SetSeed(37087534);
	for (int i = 0; i < 200; i++)
	{
		float2 p0(rnd.F(), rnd.F());
		p0 *= 600;
		p0 += float2(100, 100);
		float2 p1 = p0 + float2(rnd.F(), rnd.F()) * 200 - float2(100, 100);
		gas.bm.AddWall({ float2(p0.x,p0.y),float2(p1.x,p1.y) });
	}
	gas.threadHandle = CreateThread(0, 65536, [](void*)->DWORD
	{
		CLehmerRand rnd;
		rnd.SetSeed(1127741);

		for(int j=0;j<10;j++)
		{
			Sleep(16);
			for (int i = 0; i < 20; i++)
			{
				float2 p(rnd.F(), rnd.F());
				p *= 600;
				p += float2(100, 100);
				float2 d = float2(rnd.N10(), rnd.N10());
				gas.bm.Fire(float3(p.x, p.y, 0), float3(d.x, d.y, 0).norm(), 100 + rnd.F() * 100, rnd.F() * 10, 2.5);
			}
		}
		return 0;
	}
	, data, 0, 0);
	gas.bm.Update(0);
}

void testScenario3()
{
	if (gas.threadHandle != INVALID_HANDLE_VALUE)
	{
		TerminateThread(gas.threadHandle, 0);
		gas.threadHandle = INVALID_HANDLE_VALUE;
	};
	gas.btime = 0;
	gas.bm.Clear();
	void* data = nullptr;
	CLehmerRand rnd;
	rnd.SetSeed(37087534);
	for (int i = 0; i < 4000; i++)
	{
		float2 p0(rnd.F(), rnd.F());
		p0 *= 600;
		p0 += float2(100, 100);
		float2 p1 = p0 + float2(rnd.F(), rnd.F()) * 200 - float2(100, 100);
		gas.bm.AddWall({ float2(p0.x,p0.y),float2(p1.x,p1.y) });
	}
	gas.threadHandle = CreateThread(0, 65536, [](void*)->DWORD
	{
		CLehmerRand rnd;
		rnd.SetSeed(1127741);
		for(int j=0;j<10;j++)
		{
			Sleep(16);
			for (int i = 0; i < 400; i++)
			{
				float2 p(rnd.F(), rnd.F());
				p *= 600;
				p += float2(100, 100);
				float2 d = float2(rnd.N10(), rnd.N10());
				gas.bm.Fire(float3(p.x, p.y, 0), float3(d.x, d.y, 0).norm(), 10 + rnd.F() * 10, rnd.F() * 10, 10);
			}
		}
		return 0;
	}
	, data, 0, 0);
	gas.bm.Update(0);
}

void testScenario4()
{
	if (gas.threadHandle != INVALID_HANDLE_VALUE)
	{
		TerminateThread(gas.threadHandle, 0);
		gas.threadHandle = INVALID_HANDLE_VALUE;
	};
	gas.btime = 0;
	gas.bm.Clear();
	void* data = nullptr;
	gas.threadHandle = CreateThread(0, 65536, [](void*)->DWORD
	{
		Sleep(1000);
		gas.bm.Fire(float3(253, 242.5, 0), float3(.35, .33, 0), 300, 0.2, 1000);
		gas.bm.Fire(float3(203, 202.5, 0), float3(.25, .53, 0), 600, 0, 1000);
		return 0;
	}
	, data, 0, 0);
	for (int i = 0; i < 8; i++)
	{
		float x0 = 10.f + i * 15;
		float x1 = 790.f - i * 15;
		float y0 = 10.f + i * 15;
		float y1 = 790.f - i * 15;
		gas.bm.AddWall({ float2(x0,y0),float2(x1,y0) });
		gas.bm.AddWall({ float2(x1,y0),float2(x1,y1) });
		gas.bm.AddWall({ float2(x1,y1),float2(x0,y1) });
		gas.bm.AddWall({ float2(x0,y1),float2(x0,y0) });
	}
	gas.bm.Update(0);
}

void testScenario5()
{
	if (gas.threadHandle != INVALID_HANDLE_VALUE)
	{
		TerminateThread(gas.threadHandle, 0);
		gas.threadHandle = INVALID_HANDLE_VALUE;
	};
	gas.btime = 0;
	gas.bm.Clear();
	void* data = nullptr;
	float2 p0(400+100, 400);
	for (int i = 0; i < 150; i++)
	{
		float2 p1(400+(100 + i * 2) * cos(i / 2.f), 400+(100 + i * 2) * sin(i / 2.f));
		gas.bm.AddWall({ p0,p1 });
		p0 = p1;
	}
	gas.threadHandle = CreateThread(0, 65536, [](void*)->DWORD
	{
		for (int i = 0; i < 15; i++)
		{
			Sleep(100);
			float2 d(cos(i / 15.f),sin(i / 15.f));
			gas.bm.Fire(float3(400, 400, 0), float3(d.x,d.y,0), 10.f+i*10, 0.2f+i*0.1f, 1000);
		}
		return 0;
	}
	, data, 0, 0);

	gas.bm.Update(0);
}


void stop(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar)
{
	PostQuitMessage(0);
	gas.quitRequested = true;
	if (gas.threadHandle != INVALID_HANDLE_VALUE)
	{
		TerminateThread(gas.threadHandle, 0);
	}
	::OutputDebugString("stop issued\n");
}

LRESULT CALLBACK wp(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar)
{
	if (ProcessMouseEvent(hwnd, imsg, wpar, lpar, gas.lastMouseEvent))
	{
		gas.lastMouseEvent.frame = gas.frameStamp;
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
			gas.reloadShaders = true;
		}
		else if (wpar == VK_ESCAPE)
		{
			printf("Quiting...\n");
			gas.quitRequested = true;
		}
		else if (wpar == '1')
		{
			testScenario1();
		}
		else if (wpar == '2')
		{
			testScenario2();
		}
		else if (wpar == '3')
		{
			testScenario3();
		}
		else if (wpar == '4')
		{
			testScenario4();
		}
		else if (wpar == '5')
		{
			testScenario5();
		}
	}
	else if (imsg == WM_KEYUP)
	{
	}
	return DefWindowProc(hwnd, imsg, wpar, lpar);
}

void AddOverlayTextLine(CTextOverlay* text, const std::string& s, float x, float y, float size, uint32_t col = 0xffffffff)
{
	float w2 = gas.width / 2.f;
	float h2 = gas.height / 2.f;
	float dx = size / w2;
	float dy = size / h2;
	int i = 0;
	for (auto c : s)
	{
		if (c == '\n')
		{
			y += size;
			i = 0;
			continue;
		}
		float posx = x / w2 + i * (dx + 2 / w2) - 1.f;
		float posy = 1.f - y / h2;
		float tx = (c % 16) / 16.f;
		float ty = (c / 16) / 8.f;
		text->AddRect({ posx,posy }, { posx + dx,posy - dy }, { tx,ty }, { tx + 1.f / 16.f,ty + 1.f / 8.f }, col);
		i++;
	}
}

static const char helpText[] =
"Test scenarios:\n"
"1: 100+ walls and add 20+ bullets each frame (16 ms)\n"
"2: 100+ walls and 100+ bullets\n"
"3: 1000+ walls and 1000+ bullets\n"
"4: Geometry test: square spiral and 2 bullets\n"
"5: Geometry test: round spiral and many bullets\n";


void PrintUsage(const char* name)
{
	printf("Run: %s N\n", name);
	puts(helpText);
	puts("   - or press this key while running\n");
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		PrintUsage(argv[0]);
	}
	else
	{
		if (!strcmp(argv[1], "1"))
		{
			testScenario1();
		}
		else if (!strcmp(argv[1], "2"))
		{
			testScenario2();
		}
		else if (!strcmp(argv[1], "3"))
		{
			testScenario3();
		}
		else if (!strcmp(argv[1], "4"))
		{
			testScenario4();
		}
		else if (!strcmp(argv[1], "5"))
		{
			testScenario5();
		}
	}

	HCURSOR arrowCursor = LoadCursor(0, IDC_ARROW);

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), 0, wp, 0, 0, 0, 0, 0, 0, 0, "APP01" };
	wc.hCursor = arrowCursor;
	RegisterClassEx(&wc);

	RECT wrect = { 0, 0, gas.width, gas.height };
	AdjustWindowRectEx(&wrect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW);
	gas.win = CreateWindowEx(WS_EX_APPWINDOW, "APP01", "Bulleven", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		wrect.right - wrect.left, wrect.bottom - wrect.top, 0, 0, 0, 0);

	ShowWindow(gas.win, SW_SHOWNORMAL);
	UpdateWindow(gas.win);

	SetWindowLongPtr(gas.win, GWLP_USERDATA, (LONG_PTR) & (gas));

	gas.cam.SetLookAt({ 0,0,0 });
	gas.cam.SetPos({ -3,-3,3 });
	gas.cam.SetWH(gas.width, gas.height);
	gas.cam.SetFov(0.6);
	gas.cam.SetProjMode();
	gas.cam.m_Far = 10;
	gas.cam.m_Near = .1;
	gas.cam.CalcVPM();

	CRenderer* renderer = new CRenderer;
	if (!renderer->Init(gas.win))
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

	CBaseModel* floor = new CFloor;
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


	CTextOverlay* text = new CTextOverlay;
	text->Create(renderer->GetDevice(), renderer->GetDeviceContext());
	text->SetTextureView(0, tex->GetShaderResourceView());
	text->SetTextureView(1, 0);
	scene->AddModel(text);

	double time = GetTime();
	TAvg<double, 100> avgFps;
	bool first = true;
	while (1)
	{
		double newTime = GetTime();
		double deltaT = newTime - time;
		double timeBMUpdate0 = GetTime();
		gas.bm.Update(gas.btime);
		double timeBMUpdate1 = GetTime();
		gas.btime += deltaT;
		time = newTime;
		while (pollQueue())
		{
			if (gas.quitRequested)
				break;
		}
		if (gas.quitRequested)
			break;
		if (gas.reloadShaders)
		{
			gas.reloadShaders = false;

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
		}cb_model;

		if (gas.lastMouseEvent.frame == gas.frameStamp)
		{
			if (gas.lastMouseEvent.button_l)
			{
				float dy = gas.lastMouseEvent.delta_y / 20.0f;
				float dx = gas.lastMouseEvent.delta_x / 20.0f;

				//float l = (float)(dx * dx + dy * dy);
				//float4 rotQuat((float)(-dy * sin(l)), (float)(dx * sin(l)), 0, float(cos(l)));
				//float4x4 rotMat;
				//QuaternionToMatrix(rotQuat, rotMat);
				//cb_model.wm = cb_model.wm * rotMat;
				gas.cam.Rotate(dx, dy);
				gas.cam.CalcVPM();
			}
			cb_model.wm = gas.cam.GetVPM();
		}
		renderer->UseShaderSetup("model");
		renderer->UpdateShaderConstants((void*)&cb_model);
		renderer->UseShaderSetup("walls");
		renderer->UpdateShaderConstants((void*)&cb_model);
		renderer->UseShaderSetup("bullets");
		renderer->UpdateShaderConstants((void*)&cb_model);

		for (const auto& w : gas.bm.GetWalls())
		{
			walls->AddWall({ w.end0.x / 400 - 1, w.end0.y / 400 - 1 }, { w.end1.x / 400 - 1, w.end1.y / 400 - 1 });
		}
		walls->Commit();

		int bulletsN = 0;
		for (const auto& b : gas.bm.GetBullets())
		{
			if ((b.startTime <= gas.btime) && (b.endTime > gas.btime))
			{
				float2 pos = { b.pos.x,b.pos.y };
				float2 dir = { b.dir.x,b.dir.y };
				pos = pos + dir * (gas.btime - b.startTime);
				bullets->AddBullet({ pos.x / 400 - 1, pos.y / 400 - 1,.08f });
				bulletsN++;
			}
		}
		bullets->Commit();

		if (!first)
			avgFps.Add(1.0 / deltaT);
		first = false;

		char s[128];
		sprintf(s, "Avg FPS/100 frames: %f", avgFps.Get());
		AddOverlayTextLine(text, s, 0, 0, 16, 0xff8080ff);
		sprintf(s, "FPS: %f", 1. / deltaT);
		AddOverlayTextLine(text, s, 0, 16, 16);
		sprintf(s, "walls: %i bullets: %i/%i", gas.bm.GetWalls().size(), bulletsN, gas.bm.GetBullets().size());
		AddOverlayTextLine(text, s, 0, 32, 16);
		sprintf(s, "BulletManager::Update(): %f sec", timeBMUpdate1 - timeBMUpdate0);
		AddOverlayTextLine(text, s, 0, 48, 16);
		AddOverlayTextLine(text, "Press 1 to 4 for test scenario, use mouse to rotate camera", 0, 72, 8);
		AddOverlayTextLine(text, helpText, 0, 80, 8);
		text->Commit();

		renderer->RenderScene(scene);

		gas.frameStamp++;

	}

	renderer->Done();

	::OutputDebugString("Quiting...\n");

	return 0;
}
