// include the basic windows header files, Direct3D header files, ect...
#include <tchar.h>
#include <iostream>
#include <cstdlib>
#include <dsound.h>
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <thread>

// namespaces
using namespace std;

// define the screen resolution
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_POS_LEFT 100
#define WINDOW_POS_TOP 100
#define WS_WINDOW (WS_POPUP)
#define PI 3.14159

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// world size (map)
float map_width = 128.0f;
float map_height = 72.0f;

// starting camera coords (central of map)
float cam_x = map_width / 2;
float cam_y = map_height / 2;
float cam_z = -40;

// starting look position (central of map)
float look_x = map_width / 2;
float look_y = map_height / 2;
float look_z = 0;

// starting look dir
float dir_x = 0;
float dir_y = 1;
float dir_z = 0;

// mouse click pos in hwnd
int clickxPos;
int clickyPos;

// declaration of money
int penz = 7000;
ID3DXFont *cash;
RECT cash_rect1;
RECT cash_rect2;

// declaration of gametime
int evszam = 2860;
ID3DXFont *game_time;
RECT game_time_rect1;
RECT game_time_rect2;

// declaration of flotta informations
int hajok_szama;
int legenyseg_szama;

// declaration of system owners
string owner[30];
string player = "Human";
string ai = "Extraterrestrial";
int system_count = 0;

// declaration of random star position arrays
float starsX[30];
float starsY[30];
float pointSize = 2.5f; // for radar
D3DXVECTOR3 starPos1[30]; // for click triangles
D3DXVECTOR3 starPos2[30];
D3DXVECTOR3 starPos3[30];
D3DXVECTOR3 starPos4[30];
bool csillag_kivalaszt = FALSE;
int csillag_index;
int planets[30];
string name;
string names[30] = {
	"30 Arietis", "41 Lyncis", "81 Ceti", "Gliese 86", "Gamma Leonis",
	"AB Pictoris", "51 Pegasi", "HAT-P-7", "GQ Lupi", "GSC 0260-00648",
	"GJ 9827", "59 Virginis", "BD-10 3166", "Alpha Centauri", "Aldebaran",
	"91 Aquarii", "42 Draconis", "30 Arietis", "11 Ursae Minoris", "2M1207",
	"1SWASP", "COROT-2", "CVSO 30", "Eta2 Hydri", "Fomalhaut",
	"Epsilon Eridani", "Epsilon Indi", "Epsilon Tauri", "Sol", "HD6434"
};

// global declaration of text (esemenynaplo)
string esemeny;
ID3DXFont *esemenynaplo;
RECT esemenynaplo_rect;

// global declaration of flotta1pos (central of map)
float flotta1xPos = (map_width / 2);
float flotta1yPos = (map_height / 2);
bool kivalaszt = FALSE;
bool rmb = FALSE;

// global declarations
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;

// global declaration of time
SYSTEMTIME win_time;

// global declaration of Text
ID3DXFont *font;
RECT author_rectangle;
string author = "[H] Tom - 2019";

// global declaration of text (Time)
ID3DXFont *time1;
RECT time_rectangle;
string current_time;

// global declaration of text (FPS)
ID3DXFont *fps;
RECT fps_rectangle;
int kezdet = 0;
int vege = 0;
int eltelt = 0;
int frames = 0;
int the_fps = 0;

// global declaration of text (rendszerek)
ID3DXFont *rendsz;
RECT rendsz_rect;

// global declaration of text (flottainfo)
ID3DXFont *flottainfo;
RECT flottainfo_rect1;
RECT flottainfo_rect2;
RECT flottainfo_rect3;
RECT flottainfo_rect4;

// global declaration of Texture
IDirect3DTexture9 *texture1;
IDirect3DTexture9 *texture2;
IDirect3DTexture9 *texture3;
IDirect3DTexture9 *texture4;
IDirect3DTexture9 *texture5;
IDirect3DTexture9 *texture6;
IDirect3DTexture9 *texture7;
IDirect3DTexture9 *texture8;

// global declaration of Sprite (menu)
ID3DXSprite *sprite1;
D3DXVECTOR3 position = D3DXVECTOR3(SCREEN_WIDTH - 145.0f, 10.0f, 0.0f);

// global declaration of map triangle (for mapclick, map size x 2)
D3DXVECTOR3 map_b_also = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 map_b_felso = D3DXVECTOR3(0.0f, 144.0f, 0.0f);
D3DXVECTOR3 map_j_also = D3DXVECTOR3(256.0f, 0.0f, 0.0f);
float pU = 0;
float pV = 0;

// global declatation of move
float indulX;
float indulY;
float celX;
float celY;

// global declaration of collision triangles (for pick)
D3DXVECTOR3 b_also;
D3DXVECTOR3 b_felso;
D3DXVECTOR3 j_felso;
D3DXVECTOR3 j_also;

// struct of Ray
struct Ray
{
	D3DXVECTOR3 _origin;
	D3DXVECTOR3 _direction;
};

// function prototypes
void initD3D(HWND hWnd);
void render_frame(void);
void cleanD3D(void);
void init_graphics(void);
void update_time();
void create_stars();
Ray CalcPickingRay(LPDIRECT3DDEVICE9 Device, int, int);
void TransformRay(Ray* ray, D3DXMATRIX* T);
void get_time();
void update_flotta1pos();
void mozog();
void rendszerek();
void flotta();
void capture();

// change VertexFormat to textured
struct CUSTOMVERTEX
{
	FLOAT X;
	FLOAT Y;
	FLOAT Z;
	FLOAT tu; // texture coordinate
	FLOAT tv; // texture coordinate
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_TEX1)

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = _T("WindowClass");

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, _T("WindowClass"), _T("DX9 Game"),
		WS_WINDOW, WINDOW_POS_LEFT, WINDOW_POS_TOP, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);

	// create stars
	create_stars();

	// create gametime counter
	thread szal0(get_time);
	szal0.detach();

	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		kezdet = win_time.wMilliseconds;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.wParam) {
			case VK_UP: if (cam_y < map_height) { cam_y = cam_y + 1; look_y = look_y + 1; } break;
			case VK_DOWN: if (cam_y > 0) { cam_y = cam_y - 1; look_y = look_y - 1; } break;
			case VK_RIGHT: if (cam_x < map_width) { cam_x = cam_x + 1; look_x = look_x + 1; } break;
			case VK_LEFT: if (cam_x > 0) { cam_x = cam_x - 1; look_x = look_x - 1; } break;
			case VK_PRIOR: if (cam_z < -25) { cam_z = cam_z + 1; look_z = look_z + 1; } break;
			case VK_NEXT: if (cam_z > -40) { cam_z = cam_z - 1; look_z = look_z - 1; } break;
			default: break;
			}
		default: break;
		}

		capture();
		flotta();
		update_flotta1pos();
		update_time();
		render_frame();
		frames++;
		vege = win_time.wMilliseconds;
		eltelt = vege - kezdet;
		if (eltelt > 0 && frames % 10 == 0 ) { the_fps = 1000 / eltelt; }
	}

	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_LBUTTONDOWN:
		{
			clickxPos = GET_X_LPARAM(lParam);
			clickyPos = GET_Y_LPARAM(lParam);
			// Calculate the picking ray
			Ray ray = CalcPickingRay(d3ddev, clickxPos, clickyPos);

			// transform the ray from view space to world space
			// get view matrix
			D3DXMATRIX view;
			d3ddev->GetTransform(D3DTS_VIEW, &view);

			// inverse it
			D3DXMATRIX viewInverse;
			D3DXMatrixInverse(&viewInverse, 0, &view);

			// apply on the ray
			TransformRay(&ray, &viewInverse);

			// collision detection
			if (D3DXIntersectTri(&b_also, &b_felso, &j_felso, &ray._origin, &ray._direction, NULL, NULL, NULL) || D3DXIntersectTri(&b_also, &j_felso, &j_also, &ray._origin, &ray._direction, NULL, NULL, NULL))
			{
				kivalaszt = TRUE;
				csillag_kivalaszt = FALSE;
				name = "Flotta";
			} else {
				kivalaszt = FALSE;
			}
			for (int i = 0; i < 30; i++)
			{
				if (D3DXIntersectTri(&starPos1[i], &starPos2[i], &starPos3[i], &ray._origin, &ray._direction, NULL, NULL, NULL) || D3DXIntersectTri(&starPos2[i], &starPos3[i], &starPos4[i], &ray._origin, &ray._direction, NULL, NULL, NULL))
				{
					csillag_kivalaszt = TRUE;
					kivalaszt = FALSE;
					csillag_index = i;
				}
			}
			if (csillag_kivalaszt) { rendszerek(); } else { if (!kivalaszt) { name = ""; } else { name = "Flotta"; } }
			return 0;
		} break;
		case WM_RBUTTONDOWN:
		{
			if (kivalaszt) 
			{
			csillag_kivalaszt = FALSE;
			rmb = FALSE;
			Sleep(8);
			}

			clickxPos = GET_X_LPARAM(lParam);
			clickyPos = GET_Y_LPARAM(lParam);
			// Calculate the picking ray
			Ray ray = CalcPickingRay(d3ddev, clickxPos, clickyPos);

			// transform the ray from view space to world space
			// get view matrix
			D3DXMATRIX view;
			d3ddev->GetTransform(D3DTS_VIEW, &view);

			// inverse it
			D3DXMATRIX viewInverse;
			D3DXMatrixInverse(&viewInverse, 0, &view);

			// apply on the ray
			TransformRay(&ray, &viewInverse);

			// get mapclick positions with ray
			D3DXIntersectTri(&map_b_also, &map_b_felso, &map_j_also, &ray._origin, &ray._direction, &pU, &pV, NULL);
			
			rmb = TRUE;
			if (kivalaszt)
			{
				csillag_kivalaszt = FALSE;
				thread szal1(mozog);
				szal1.detach();
			}
			return 0;
		} break;
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_ESCAPE:
					int msgboxID = MessageBox(NULL, "Exit Program?", "EXIT?", MB_ICONQUESTION | MB_YESNO);
						switch (msgboxID)
						{
						case IDYES: PostQuitMessage(0);  break;
						case IDNO: break;
						} break;
			}
		} break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	init_graphics();    // call the function to initialize the shapes

	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);    // turn off the 3D lighting
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer

	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);    // turn off the color blending

	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);    // set source factor
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // set dest factor

	d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);    // set the operation

	// for draw transparent textures
	d3ddev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x0000008f);
	d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	font = NULL;
	HRESULT hr0 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &font);
	SetRect(&author_rectangle, 10, 10, 200, 50);

	HRESULT hr1 = D3DXCreateFont(d3ddev, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &time1);
	SetRect(&time_rectangle, 10, 50, 200, 80);

	HRESULT hr2 = D3DXCreateFont(d3ddev, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &fps);
	SetRect(&fps_rectangle, 10, 80, 200, 110);

	texture1 = NULL;
	texture2 = NULL;
	texture3 = NULL;
	texture4 = NULL;
	texture5 = NULL;
	texture6 = NULL;
	texture7 = NULL;
	texture8 = NULL;
	HRESULT hr3 = D3DXCreateTextureFromFile(d3ddev, "back1.bmp", &texture1);
	HRESULT hr4 = D3DXCreateTextureFromFile(d3ddev, "back2.bmp", &texture2);
	HRESULT hr5 = D3DXCreateTextureFromFile(d3ddev, "menu1.png", &texture3);
	HRESULT hr6 = D3DXCreateTextureFromFile(d3ddev, "csillag.png", &texture4);
	HRESULT hr7 = D3DXCreateTextureFromFile(d3ddev, "csillag2.png", &texture5);
	HRESULT hr8 = D3DXCreateTextureFromFile(d3ddev, "flotta1a.png", &texture6);
	HRESULT hr9 = D3DXCreateTextureFromFile(d3ddev, "flotta1b.png", &texture7);
	HRESULT hr16 = D3DXCreateTextureFromFile(d3ddev, "menu2.png", &texture8);

	HRESULT hr10 = D3DXCreateSprite(d3ddev, &sprite1);

	HRESULT hr11 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &rendsz);
	SetRect(&rendsz_rect, 10, 110, 200, 140);

	HRESULT hr12 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &flottainfo);
	SetRect(&flottainfo_rect1, 10, 140, 100, 170);
	SetRect(&flottainfo_rect2, 100, 140, 250, 170);
	SetRect(&flottainfo_rect3, 10, 170, 100, 200);
	SetRect(&flottainfo_rect4, 100, 170, 250, 200);

	HRESULT hr13 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &cash);
	SetRect(&cash_rect1, 10, 200, 100, 230);
	SetRect(&cash_rect2, 100, 200, 250, 230);

	HRESULT hr14 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &game_time);
	SetRect(&game_time_rect1, 10, 230, 100, 260);
	SetRect(&game_time_rect2, 100, 230, 250, 260);

	HRESULT hr15 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &esemenynaplo);
	SetRect(&esemenynaplo_rect, SCREEN_WIDTH / 2 - 300, 10, SCREEN_WIDTH / 2 + 300, 100);
}

// this is the function used to render a single frame
void render_frame(void)
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();

	// select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);
	
	// set the view transform
	D3DXMATRIX matView;    // the view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(cam_x, cam_y, cam_z),    // the camera position
		&D3DXVECTOR3(look_x, look_y, look_z),    // the look-at position
		&D3DXVECTOR3(dir_x, dir_y, dir_z));    // the up direction
	d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

	// set the projection transform
	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),    // the horizontal field of view
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
		25.0f,    // the near view-plane
		40.0f);    // the far view-plane
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection

	// set the stream source
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

	D3DXMATRIX matTranslate;

	// set the world transform
	D3DXMatrixTranslation(&matTranslate, 0.0f, 0.0f, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));
	// set texture
	d3ddev->SetTexture(0, texture1);
	// draw the first triangle
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

	d3ddev->SetTexture(0, texture2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 3, 1);

	// draw stars and radars
	for (int i = 0; i < 30; i++)
	{
		D3DXMatrixTranslation(&matTranslate, starsX[i], starsY[i], 0.0f);
		d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));
		d3ddev->SetTexture(0, texture4);
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 6, 1);
		d3ddev->SetTexture(0, texture5);
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 9, 1);
		d3ddev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&pointSize));
		d3ddev->SetTexture(0, NULL);
		d3ddev->DrawPrimitive(D3DPT_POINTLIST, 18, 18);
	}

	// draw flotta1
	D3DXMatrixTranslation(&matTranslate, flotta1xPos, flotta1yPos, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));
	d3ddev->SetTexture(0, texture6);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 1);
	d3ddev->SetTexture(0, texture7);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 15, 1);

	// sprite (menu)
	sprite1->Begin(D3DXSPRITE_ALPHABLEND);
	if (csillag_kivalaszt && owner[csillag_index] != player)
	{
		sprite1->Draw(texture8, NULL, NULL, &position, 0xFFFFFFFF);
	} else {
		sprite1->Draw(texture3, NULL, NULL, &position, 0xFFFFFFFF);
	}
	sprite1->End();
	
	// draw texts
	font->DrawTextA(NULL, author.c_str(), -1, &author_rectangle, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	time1->DrawTextA(NULL, current_time.c_str(), -1, &time_rectangle, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	fps->DrawTextA(NULL, (to_string(the_fps) + " FPS").c_str(), -1, &fps_rectangle, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	
	rendsz->DrawTextA(NULL, name.c_str(), -1, &rendsz_rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	if (kivalaszt) {
		flottainfo->DrawTextA(NULL, "Ships: ", -1, &flottainfo_rect1, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
		flottainfo->DrawTextA(NULL, to_string(hajok_szama).c_str(), -1, &flottainfo_rect2, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
		flottainfo->DrawTextA(NULL, "Crew: ", -1, &flottainfo_rect3, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
		flottainfo->DrawTextA(NULL, to_string(legenyseg_szama).c_str(), -1, &flottainfo_rect4, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	}
	if (csillag_kivalaszt) {
		flottainfo->DrawTextA(NULL, "Empire: ", -1, &flottainfo_rect1, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
		flottainfo->DrawTextA(NULL, owner[csillag_index].c_str(), -1, &flottainfo_rect2, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
		flottainfo->DrawTextA(NULL, "Planets: ", -1, &flottainfo_rect3, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
		flottainfo->DrawTextA(NULL, to_string(planets[csillag_index]).c_str(), -1, &flottainfo_rect4, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	}

	cash->DrawTextA(NULL, "Cash: ", -1, &cash_rect1, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	cash->DrawTextA(NULL, to_string(penz).c_str(), -1, &cash_rect2, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	game_time-> DrawTextA(NULL, "Year: ", -1, &game_time_rect1, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	game_time->DrawTextA(NULL, to_string(evszam).c_str(), -1, &game_time_rect2, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	esemenynaplo->DrawTextA(NULL, esemeny.c_str(), -1, &esemenynaplo_rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	v_buffer->Release();    // close and release the vertex buffer
	d3ddev->Release();    // close and release the 3D device
	d3d->Release();    // close and release Direct3D
	font->Release();	// close and release screen texts
	time1->Release();
	fps->Release();
	sprite1->Release(); // close and release sprite
	rendsz->Release();
	flottainfo->Release();
	cash->Release();
	game_time->Release();
	esemenynaplo->Release();
}


// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		// triangle (map)
		{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // 0
		{ 0.0f, 72.0f, 0.0f, 0.0f, 0.0f },
		{ 128.0f, 72.0f, 0.0f, 1.0f, 0.0f },

		// triangle 2 (map)
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // 3
		{ 128.0f, 72.0f, 0.0f, 0.0f, 1.0f },
		{ 128.0f, 0.0f, 0.0f, 0.0f, 0.0f },

		// for stars
		{ -1.25f, -1.25f, 0.0f, 0.0f, 1.0f }, // 6
		{ -1.25f, 1.25f, 0.0f, 0.0f, 0.0f },
		{ 1.25f, 1.25f, 0.0f, 1.0f, 0.0f },

		// for stars 2
		{ -1.25f, -1.25f, 0.0f, 1.0f, 0.0f }, // 9
		{ 1.25f, 1.25f, 0.0f, 0.0f, 1.0f },
		{ 1.25f, -1.25f, 0.0f, 0.0f, 0.0f },

		// for flotta1
		{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f }, // 12
		{ -1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },

		// for flotta1 2
		{ -1.0f, -1.0f, 0.0f, 1.0f, 0.0f }, // 15
		{ 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, 0.0f, 0.0f, 0.0f },

		// pontlist (18 pont // 20 fok)
		{ 0.0f, 5.0f, 0.0f, 0.0f, 1.0f }, // 18
		{ 1.7101f, 4.698463f, 0.0f, 0.0f, 1.0f },
		{ 3.213938f, 3.830222f, 0.0f, 0.0f, 1.0f },
		{ 4.330127f, 2.5f, 0.0f, 0.0f, 1.0f },
		{ 4.924038f, 0.868240f, 0.0f, 0.0f, 1.0f },
		{ 4.924038f, -0.868240f, 0.0f, 0.0f, 1.0f },
		{ 4.330127f, -2.5f, 0.0f, 0.0f, 1.0f },
		{ 3.213938f, -3.830222f, 0.0f, 0.0f, 1.0f },
		{ 1.7101f, -4.698463f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, -5.0f, 0.0f, 0.0f, 1.0f },
		{ -1.7101f, -4.698463f, 0.0f, 0.0f, 1.0f },
		{ -3.213938f, -3.830222f, 0.0f, 0.0f, 1.0f },
		{ -4.330127f, -2.5f, 0.0f, 0.0f, 1.0f },
		{ -4.924038f, -0.868240f, 0.0f, 0.0f, 1.0f },
		{ -4.924038f, 0.868240f, 0.0f, 0.0f, 1.0f },
		{ -4.330127f, 2.5f, 0.0f, 0.0f, 1.0f },
		{ -3.213938f, 3.830222f, 0.0f, 0.0f, 1.0f },
		{ -1.7101f, 4.698463f, 0.0f, 0.0f, 1.0f },
	};

	// create a vertex buffer interface called v_buffer
	d3ddev->CreateVertexBuffer(36 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;    // a void pointer

	// lock v_buffer and load the vertices into it
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	std::memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();
}

// function for show current time
void update_time()
{
	GetLocalTime(&win_time);
	int ora = win_time.wHour;
	int perc = win_time.wMinute;
	int masodperc = win_time.wSecond;
	string h = to_string(ora);
	string m = to_string(perc);
	string s = to_string(masodperc);
	if (ora < 10) { h = "0" + h; }
	if (perc < 10) { m = "0" + m; }
	if (masodperc < 10) { s = "0" + s; }
	current_time = h + ":" + m + ":" + s;
}

void create_stars()
{
	// fill up random star positions
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 30; i++)
	{
		starsX[i] = float(int(rand() % 118 + 5));
		starsY[i] = float(int(rand() % 62 + 5));
		
		// for select
		starPos1[i] = D3DXVECTOR3(starsX[i] - 1.25f, starsY[i] - 1.25f, 0.0f);
		starPos2[i] = D3DXVECTOR3(starsX[i] - 1.25f, starsY[i] + 1.25f, 0.0f);
		starPos3[i] = D3DXVECTOR3(starsX[i] + 1.25f, starsY[i] + 1.25f, 0.0f);
		starPos4[i] = D3DXVECTOR3(starsX[i] + 1.25f, starsY[i] - 1.25f, 0.0f);

		owner[i] = "Not colonized";
		planets[i] = rand() % 7 + 1;
	}
}

Ray CalcPickingRay(LPDIRECT3DDEVICE9 Device, int clickxPos, int clickyPos)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	d3ddev->GetViewport(&vp);

	D3DXMATRIX proj;
	d3ddev->GetTransform(D3DTS_PROJECTION, &proj);

	px = (((2.0f*clickxPos) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f*clickyPos) / vp.Height) + 1.0f) / proj(1, 1);

	Ray ray;
	ray._origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction = D3DXVECTOR3(px, py, 1.0f);

	return ray;
}

void TransformRay(Ray* ray, D3DXMATRIX* invertViewMatrix)
{
	// transform the ray's origin, w = 1.
	D3DXVec3TransformCoord(
		&ray->_origin,
		&ray->_origin,
		invertViewMatrix);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(
		&ray->_direction,
		&ray->_direction,
		invertViewMatrix);

	// normalize the direction
	D3DXVec3Normalize(&ray->_direction, &ray->_direction);
}

void update_flotta1pos() 
{
	b_also = D3DXVECTOR3(flotta1xPos - 1.0f, flotta1yPos -1.0f, 0.0f);
	b_felso = D3DXVECTOR3(flotta1xPos -1.0f, flotta1yPos + 1.0f, 0.0f);
	j_felso = D3DXVECTOR3(flotta1xPos + 1.0f, flotta1yPos + 1.0f, 0.0f);
	j_also = D3DXVECTOR3(flotta1xPos + 1.0f, flotta1yPos - 1.0f, 0.0f);
}

void mozog()
{
	indulX = flotta1xPos;
	indulY = flotta1yPos;

	celX = map_width * (pV * 2);
	celY = map_height * (pU * 2);

	int alvasido = 5; // ms ... 0.005s

	float speed = 4.0f;
	float egesz_utX = abs(celX - indulX);
	float egesz_utY = abs(celY - indulY);
	float utX = speed * ((egesz_utX / speed) / 200); // 1s/0.005s = 200
	float utY = speed * ((egesz_utY / speed) / 200);


	while (rmb && (flotta1xPos > 0 && flotta1yPos > 0) && (flotta1xPos < map_width && flotta1yPos < map_height))
		{
		if (flotta1xPos < (celX + utX) && flotta1xPos > (celX - utX)) { flotta1xPos = celX; }
		if (flotta1yPos < (celY + utY) && flotta1yPos > (celY - utY)) { flotta1yPos = celY; }
		if (flotta1xPos == celX && flotta1yPos == celY) { rmb = FALSE; break; }
		if (celX - flotta1xPos > 0) { flotta1xPos = flotta1xPos + utX; }
		if (celX - flotta1xPos < 0) { flotta1xPos = flotta1xPos - utX; }
		if (celY - flotta1yPos > 0) { flotta1yPos = flotta1yPos + utY; }
		if (celY - flotta1yPos < 0) { flotta1yPos = flotta1yPos - utY; }
		Sleep(alvasido);
		}

	return;
}

void rendszerek()
{
	name = names[csillag_index];
}

void flotta()
{
	hajok_szama = 1;
	legenyseg_szama = 40 * hajok_szama;
}

void get_time()
{
	while (TRUE) 
	{ 
		Sleep(30000);
		evszam++;
		for (int i = 0; i < 30; i++)
		{
			if (owner[i] == player) { system_count++; }
		}
		esemeny = "New Year: " + to_string(evszam) + ", Tax income: " + to_string(system_count * 200) + " credit";
		penz = penz +  system_count * 200;
		system_count = 0;
	}
}

void capture()
{
	for (int i = 0; i < 30; i++)
	{
		if ((starsX[i] < flotta1xPos + 3.0f && starsX[i] > flotta1xPos - 3.0f) && (starsY[i] < flotta1yPos + 3.0f && starsY[i] > flotta1yPos - 3.0f) && owner[i] != player)
		{
			owner[i] = player;
			penz = penz + 500;
			esemeny = names[i] + " :: System captured... Reward: 500 credit";
		}
	}
}