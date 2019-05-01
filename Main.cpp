// include the basic windows header files, Direct3D header files, ect...
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <thread>
#include <vector>
//#include <dsound.h>
#include "Main.h"
#include "clock.cpp"
#include "fps.cpp"
#include "functions.cpp"

// namespaces
using namespace std;

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declaration for strings
string author = "[H] Tom - 2019";
string current_time;
string esemeny;
string owner[30];
string player = "Human";
string ai = "Extraterrestrial";
string name = "Flotta";
string names[30] = {
	"30 Arietis", "41 Lyncis", "81 Ceti", "Gliese 86", "Gamma Leonis",
	"AB Pictoris", "51 Pegasi", "HAT-P-7", "GQ Lupi", "GSC 0260-00648",
	"GJ 9827", "59 Virginis", "BD-10 3166", "Alpha Centauri", "Aldebaran",
	"91 Aquarii", "42 Draconis", "30 Arietis", "11 Ursae Minoris", "2M1207",
	"1SWASP", "COROT-2", "CVSO 30", "Eta2 Hydri", "Fomalhaut",
	"Epsilon Eridani", "Epsilon Indi", "Epsilon Tauri", "Sol", "HD6434"
};

// global declaration for fonts and rects
ID3DXFont *cash = 0;
RECT cash_rect;
ID3DXFont *gametime = 0;
RECT game_time_rect;
ID3DXFont *fps = 0;
RECT fps_rectangle;
ID3DXFont *esemenynaplo = 0;
RECT esemenynaplo_rect;
ID3DXFont *font = 0;
RECT author_rectangle;
ID3DXFont *time1 = 0;
RECT time_rectangle;
ID3DXFont *rendsz = 0;
RECT rendsz_rect;
ID3DXFont *flottainfo = 0;
RECT flottainfo_rect1;
RECT flottainfo_rect2;

// for click triangles (stars)
D3DXVECTOR3 starPos1[30];
D3DXVECTOR3 starPos2[30];
D3DXVECTOR3 starPos3[30];
D3DXVECTOR3 starPos4[30];

// global declarations
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;

// global declaration of time
SYSTEMTIME win_time;

// global declaration of Texture
IDirect3DTexture9 *texture1 = 0;
IDirect3DTexture9 *texture2 = 0;
IDirect3DTexture9 *texture3 = 0;
IDirect3DTexture9 *texture4 = 0;
IDirect3DTexture9 *texture5 = 0;
IDirect3DTexture9 *texture6 = 0;
IDirect3DTexture9 *texture7 = 0;
IDirect3DTexture9 *texture8 = 0;
IDirect3DTexture9 *texture9 = 0;
IDirect3DTexture9 *texture10 = 0;

// global declaration of text (rendszerek 3d)
ID3DXMesh *text1[30] = { 0 };
LPCSTR rn[30];

// global declaration of Sprite (menu)
ID3DXSprite *sprite1;
D3DXVECTOR3 position = D3DXVECTOR3(SCREEN_WIDTH - 145.0f, 10.0f, 0.0f);
ID3DXSprite *sprite2;
D3DXVECTOR3 position_1 = D3DXVECTOR3(SCREEN_WIDTH - 145.0f, 145.0f, 0.0f);

// global declaration of map triangle (for mapclick, map size x 2)
D3DXVECTOR3 map_b_also = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 map_b_felso = D3DXVECTOR3(0.0f, 144.0f, 0.0f);
D3DXVECTOR3 map_j_also = D3DXVECTOR3(256.0f, 0.0f, 0.0f);

// global declaration of collision triangles (for pick)
// player flotta
D3DXVECTOR3 b_also;
D3DXVECTOR3 b_felso;
D3DXVECTOR3 j_felso;
D3DXVECTOR3 j_also;
// ai flotta
D3DXVECTOR3 ai_b_also;
D3DXVECTOR3 ai_b_felso;
D3DXVECTOR3 ai_j_felso;
D3DXVECTOR3 ai_j_also;

// mesh
ID3DXMesh *Mesh_cube = 0;
vector<D3DMATERIAL9> Mtrls(0);
vector<IDirect3DTexture9*> Textures(0);

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
void create_stars();
Ray CalcPickingRay(LPDIRECT3DDEVICE9 Device, int, int);
void TransformRay(Ray* ray, D3DXMATRIX* T);
void game_time();
void update_flottapos();
void mozog();
void flotta();
void capture_player();
void capture_ai();
void gep();
void move_ai();
void gyartas();
void rendszer();
void save();
void load();

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

	// classes
	Clock oraobj;
	FPS fpsobj;

	// create stars
	create_stars();

	// create gametime counter
	thread szal0(game_time);
	szal0.detach();

	// start capture loops
	thread szal3(capture_player);
	szal3.detach();

	thread szal4(capture_player);
	szal4.detach();

	// start ai
	gep();
	
	// enter the main loop:

	MSG msg;
	
	while (!game_over)
	{
		GetLocalTime(&win_time);
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
			case VK_UP:		if (cam_y < map_height) { cam_y = cam_y + 1; look_y = look_y + 1; } break;
			case VK_DOWN:	if (cam_y > 0) { cam_y = cam_y - 1; look_y = look_y - 1; }			break;
			case VK_RIGHT:	if (cam_x < map_width) { cam_x = cam_x + 1; look_x = look_x + 1; }	break;
			case VK_LEFT:	if (cam_x > 0) { cam_x = cam_x - 1; look_x = look_x - 1; }			break;
			case VK_PRIOR:	if (cam_z < -26) { cam_z = cam_z + 1; look_z = look_z + 1; }		break;
			case VK_NEXT:	if (cam_z > -40) { cam_z = cam_z - 1; look_z = look_z - 1; }		break;
			default: break;
			}
		default: break;
		}
		flotta();
		update_flottapos();
		current_time = oraobj.time_now(win_time);
		render_frame();
		GetLocalTime(&win_time);
		vege = win_time.wMilliseconds;
		if (frames > 30) { the_fps = fpsobj.get_fps(kezdet, vege); frames = 0; }
		frames++;
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
			if ((D3DXIntersectTri(&b_also, &b_felso, &j_felso, &ray._origin, &ray._direction, NULL, NULL, NULL) || D3DXIntersectTri(&b_also, &j_felso, &j_also, &ray._origin, &ray._direction, NULL, NULL, NULL)) && !flotta_kivalaszt)
			{
				flotta_kivalaszt = TRUE;
				nmy_flotta_kivalaszt = FALSE;
				csillag_kivalaszt = FALSE;
				name = "Flotta";
			}
			if ((D3DXIntersectTri(&ai_b_also, &ai_b_felso, &ai_j_felso, &ray._origin, &ray._direction, NULL, NULL, NULL) || D3DXIntersectTri(&ai_b_also, &ai_j_felso, &ai_j_also, &ray._origin, &ray._direction, NULL, NULL, NULL)) && !nmy_flotta_kivalaszt)
			{
				flotta_kivalaszt = FALSE;
				nmy_flotta_kivalaszt = TRUE;
				csillag_kivalaszt = FALSE;
				name = "AI Flotta";
			}
			for (int i = 0; i < 30; i++)
			{
				if (D3DXIntersectTri(&starPos1[i], &starPos2[i], &starPos3[i], &ray._origin, &ray._direction, NULL, NULL, NULL) || D3DXIntersectTri(&starPos2[i], &starPos3[i], &starPos4[i], &ray._origin, &ray._direction, NULL, NULL, NULL))
				{
					flotta_kivalaszt = FALSE;
					nmy_flotta_kivalaszt = FALSE;
					csillag_kivalaszt = TRUE;
					csillag_index = i;
					name = names[csillag_index];
				}
			}
			return 0;
		} break;
		case WM_RBUTTONDOWN:
		{
			stop_thread = FALSE;
			if (flotta_kivalaszt) 
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
			if (flotta_kivalaszt)
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
				case VK_F1: Functions fnc; esemeny = to_string(fnc.distance2d(flotta1xPos, flotta1yPos, starsX[0], starsY[0])); break; // for testing
				case VK_PAUSE: case VK_SPACE:
					if (!game_pause) { game_pause = TRUE; esemeny = "PAUSED"; } else { game_pause = FALSE; esemeny = ""; } break;
				case VK_F6: save(); esemeny = "Save"; break;
				case VK_F7: load(); esemeny = "Load"; stop_thread = TRUE; break;
				case VK_ESCAPE:
					int msgboxID = MessageBox(NULL, "Exit Program?", "EXIT?", MB_ICONQUESTION | MB_YESNO);
						switch (msgboxID)
						{
						case IDYES: game_over = TRUE; PostQuitMessage(0);  break;
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
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // D3DPRESENT_INTERVAL_IMMEDIATE

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

	HRESULT hr0 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &font);
	SetRect(&author_rectangle, 10, 10, 200, 50);

	HRESULT hr1 = D3DXCreateFont(d3ddev, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &time1);
	SetRect(&time_rectangle, 10, 50, 200, 80);

	HRESULT hr2 = D3DXCreateFont(d3ddev, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &fps);
	SetRect(&fps_rectangle, 10, 80, 200, 110);

	HRESULT hr3 = D3DXCreateTextureFromFile(d3ddev, "Pictures/back.bmp", &texture1);
	HRESULT hr5 = D3DXCreateTextureFromFile(d3ddev, "Pictures/csillag.png", &texture2);
	HRESULT hr6 = D3DXCreateTextureFromFile(d3ddev, "Pictures/flotta1.png", &texture3);
	HRESULT hr4 = D3DXCreateTextureFromFile(d3ddev, "Pictures/menu1.png", &texture4);
	HRESULT hr7 = D3DXCreateTextureFromFile(d3ddev, "Pictures/menu2.png", &texture5);
	HRESULT hr8 = D3DXCreateTextureFromFile(d3ddev, "Pictures/menu3.png", &texture6);
	HRESULT hr9 = D3DXCreateTextureFromFile(d3ddev, "Pictures/nmy_flotta.png", &texture7);
	HRESULT hr10 = D3DXCreateTextureFromFile(d3ddev, "Pictures/empty_color.bmp", &texture8);
	HRESULT hr11 = D3DXCreateTextureFromFile(d3ddev, "Pictures/ai_color.bmp", &texture9);
	HRESULT hr12 = D3DXCreateTextureFromFile(d3ddev, "Pictures/capture.png", &texture10);

	HRESULT hr13 = D3DXCreateSprite(d3ddev, &sprite1);
	HRESULT hr14 = D3DXCreateSprite(d3ddev, &sprite2);

	HRESULT hr15 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &rendsz);
	SetRect(&rendsz_rect, 10, 110, 200, 140);

	HRESULT hr16 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &flottainfo);
	SetRect(&flottainfo_rect1, 10, 140, 250, 170);
	SetRect(&flottainfo_rect2, 10, 170, 250, 200);

	HRESULT hr17 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &cash);
	SetRect(&cash_rect, 10, 200, 250, 230);

	HRESULT hr18 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &gametime);
	SetRect(&game_time_rect, 10, 230, 250, 260);

	HRESULT hr19 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &esemenynaplo);
	SetRect(&esemenynaplo_rect, SCREEN_WIDTH / 2 - 300, 10, SCREEN_WIDTH / 2 + 300, 100);

	HDC hdc;
	HFONT font1;
	hdc = CreateCompatibleDC(NULL);
	font1 = CreateFont(
		10,         //Height
		0,          //Width
		0,          //Escapement
		0,          //Orientation
		FW_NORMAL,  //Weight
		false,      //Italic
		false,      //Underline
		false,      //Strikeout
		DEFAULT_CHARSET,//Charset 
		OUT_DEFAULT_PRECIS,  //Output Precision
		CLIP_DEFAULT_PRECIS, //Clipping Precision
		ANTIALIASED_QUALITY,     //Quality
		DEFAULT_PITCH | FF_DONTCARE, //Pitch and Family
		"Times New Roman"
		);
	SelectObject(hdc, font1);
	for (int i = 0; i < 30; i++)
	{
		rn[i] = names[i].c_str();
		D3DXCreateText(d3ddev, hdc, rn[i], 0.01f, 0.05f, &text1[i], NULL, NULL);
	}

	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;
	HRESULT hr20 = D3DXLoadMeshFromX("cube.x", D3DXMESH_MANAGED, d3ddev, &adjBuffer, &mtrlBuffer, 0, &numMtrls, &Mesh_cube);
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

	D3DXMATRIX matRotateX;
	D3DXMATRIX matRotateY;
	D3DXMATRIX matRotateZ;
	D3DXMATRIX matScale;
	D3DXMATRIX matTranslate;

	// set the world transform
	D3DXMatrixTranslation(&matTranslate, 0.0f, 0.0f, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));
	// set texture
	d3ddev->SetTexture(0, texture1);
	// draw the first triangle
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 3, 1);

	// draw stars and radars
	for (int i = 0; i < 30; i++)
	{
		D3DXMatrixTranslation(&matTranslate, starsX[i], starsY[i], 0.0f);
		d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));
		d3ddev->SetTexture(0, texture2);
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 6, 1);
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 9, 1);
		d3ddev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&pointSize));
		d3ddev->SetTexture(0, texture8);
		if (owner[i] == player) { d3ddev->SetTexture(0, NULL); }
		if (owner[i] == ai) { d3ddev->SetTexture(0, texture9); }
		d3ddev->DrawPrimitive(D3DPT_POINTLIST, 18, 18);
	}

	// draw enemy flotta
	D3DXMatrixTranslation(&matTranslate, nmy_flottaxPos, nmy_flottayPos, -0.1f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));
	d3ddev->SetTexture(0, texture7);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 1);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 15, 1);

	// draw flotta1
	D3DXMatrixTranslation(&matTranslate, flotta1xPos, flotta1yPos, -0.1f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));
	d3ddev->SetTexture(0, texture3);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 1);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 15, 1);

	// draw system names
	for (int j = 0; j < 30; j++)
	{
		D3DXMatrixTranslation(&matTranslate, starsX[j] + 1.0f, starsY[j] - 0.25f, 0.0f);
		d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));
		d3ddev->SetTexture(0, texture8);
		if (owner[j] == player) { d3ddev->SetTexture(0, NULL); }
		if (owner[j] == ai) { d3ddev->SetTexture(0, texture9); }
		text1[j]->DrawSubset(0);
	}

	// mesh
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(90.0f));
	D3DXMatrixScaling(&matScale, 0.025f, 0.025f, 0.025f);
	D3DXMatrixTranslation(&matTranslate, 0.0f, 0.0f, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matRotateX * matScale * matTranslate));
	d3ddev->SetTexture(0, NULL);
	for (int k = 0; k < 24; k++)
	{
		Mesh_cube->DrawSubset(k);
	}

	// sprite (menu)
	sprite1->Begin(D3DXSPRITE_ALPHABLEND);
	if (csillag_kivalaszt)
	{
		if (owner[csillag_index] == player) { sprite1->Draw(texture5, NULL, NULL, &position, 0xFFFFFFFF); }
		if (owner[csillag_index] == ai) { sprite1->Draw(texture6, NULL, NULL, &position, 0xFFFFFFFF); }
		if (owner[csillag_index] != player && owner[csillag_index] != ai) { sprite1->Draw(texture4, NULL, NULL, &position, 0xFFFFFFFF); }
	}
	if (flotta_kivalaszt) { sprite1->Draw(texture3, NULL, NULL, &position, 0xFFFFFFFF); }
	if (nmy_flotta_kivalaszt) { sprite1->Draw(texture7, NULL, NULL, &position, 0xFFFFFFFF); }
	sprite1->End();
	sprite2->Begin(D3DXSPRITE_ALPHABLEND);
	if (cap_rendszer_player > 0)
	{
		sprite2->Draw(texture10, NULL, NULL, &position_1, 0xFFFFFFFF);
	}
	sprite2->End();
	
	// draw texts
	font->DrawTextA(NULL, author.c_str(), -1, &author_rectangle, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	time1->DrawTextA(NULL, current_time.c_str(), -1, &time_rectangle, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	fps->DrawTextA(NULL, (to_string(the_fps) + " FPS").c_str(), -1, &fps_rectangle, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	rendsz->DrawTextA(NULL, name.c_str(), -1, &rendsz_rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	if (flotta_kivalaszt) {
		string _hajok = "Ships: " + to_string(hajok_szama);
		flottainfo->DrawTextA(NULL, _hajok.c_str(), -1, &flottainfo_rect1, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
		string _legenyseg = "Crew: " + to_string(legenyseg_szama);
		flottainfo->DrawTextA(NULL, _legenyseg.c_str(), -1, &flottainfo_rect2, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	}
	if (csillag_kivalaszt) {
		string _birodalom = "Empire: " + owner[csillag_index];
		flottainfo->DrawTextA(NULL, _birodalom.c_str(), -1, &flottainfo_rect1, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
		string _rendszer = "Planets: " + to_string(planets[csillag_index]);
		flottainfo->DrawTextA(NULL, _rendszer.c_str(), -1, &flottainfo_rect2, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	}

	string _cash = "Cash: " + to_string(penz);
	cash->DrawTextA(NULL, _cash.c_str(), -1, &cash_rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	string _gametime = "Year: " + to_string(evszam);
	gametime->DrawTextA(NULL, _gametime.c_str(), -1, &game_time_rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

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
	gametime->Release();
	esemenynaplo->Release();
	Mesh_cube->Release();
	sprite2->Release();
	for (int i = 0; i < 30; i++)
	{
		text1[i]->Release();
	}
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
		{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // 3
		{ 128.0f, 72.0f, 0.0f, 1.0f, 0.0f },
		{ 128.0f, 0.0f, 0.0f, 1.0f, 1.0f },

		// for stars
		{ -3.5f, -3.5f, 0.0f, 0.0f, 1.0f }, // 6
		{ -3.5f, 3.5f, 0.0f, 0.0f, 0.0f },
		{ 3.5f, 3.5f, 0.0f, 1.0f, 0.0f },

		// for stars 2
		{ -3.5f, -3.5f, 0.0f, 0.0f, 1.0f }, // 9
		{ 3.5f, 3.5f, 0.0f, 1.0f, 0.0f },
		{ 3.5f, -3.5f, 0.0f, 1.0f, 1.0f },

		// for flotta1
		{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f }, // 12
		{ -1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },

		// for flotta1 2
		{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f }, // 15
		{ 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f, 1.0f, 1.0f },

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

// functions
void create_stars()
{
	// star positions
	srand((unsigned int)time(NULL));
	
	float x[6] = { map_width / 7, (map_width / 7) * 2, (map_width / 7) * 3, (map_width / 7) * 4, (map_width / 7) * 5, (map_width / 7) * 6 };
	float y[5] = { map_height / 6, (map_height / 6) * 2, (map_height / 6) * 3, (map_height / 6) * 4, (map_height / 6) * 5 };

	for (int i = 0; i < 6; i++)
	{
		starsX[i] = x[i] + rand() % 14 - 7;
		starsY[i] = y[0] + rand() % 10 - 3;
	}
	for (int j = 6; j < 12; j++)
	{
		starsX[j] = x[j-6] + rand() % 14 - 7;
		starsY[j] = y[1] + rand() % 10 - 3;
	}
	for (int k = 12; k < 18; k++)
	{
		starsX[k] = x[k-12] + rand() % 14 - 7;
		starsY[k] = y[2] + rand() % 10 - 3;
	}
	for (int l = 18; l < 24; l++)
	{
		starsX[l] = x[l-18] + rand() % 14 - 7;
		starsY[l] = y[3] + rand() % 10 - 3;
	}
	for (int m = 24; m < 30; m++)
	{
		starsX[m] = x[m-24] + rand() % 14 - 7;
		starsY[m] = y[4] + rand() % 10 - 3;
	}

	for (int n = 0; n < 30; n++)
	{
		// for select
		starPos1[n] = D3DXVECTOR3(starsX[n] - 1.25f, starsY[n] - 1.25f, 0.0f);
		starPos2[n] = D3DXVECTOR3(starsX[n] - 1.25f, starsY[n] + 1.25f, 0.0f);
		starPos3[n] = D3DXVECTOR3(starsX[n] + 1.25f, starsY[n] + 1.25f, 0.0f);
		starPos4[n] = D3DXVECTOR3(starsX[n] + 1.25f, starsY[n] - 1.25f, 0.0f);

		owner[n] = "Not colonized";
		planets[n] = rand() % 7 + 1;
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

	px = (((2.0f * clickxPos) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f * clickyPos) / vp.Height) + 1.0f) / proj(1, 1);

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

void update_flottapos()
{
	// Player flotta picking triangles
	b_also = D3DXVECTOR3(flotta1xPos - 1.0f, flotta1yPos -1.0f, 0.0f);
	b_felso = D3DXVECTOR3(flotta1xPos -1.0f, flotta1yPos + 1.0f, 0.0f);
	j_felso = D3DXVECTOR3(flotta1xPos + 1.0f, flotta1yPos + 1.0f, 0.0f);
	j_also = D3DXVECTOR3(flotta1xPos + 1.0f, flotta1yPos - 1.0f, 0.0f);

	// AI flotta picking triangles
	ai_b_also = D3DXVECTOR3(nmy_flottaxPos - 1.0f, nmy_flottayPos - 1.0f, 0.0f);
	ai_b_felso = D3DXVECTOR3(nmy_flottaxPos - 1.0f, nmy_flottayPos + 1.0f, 0.0f);
	ai_j_felso = D3DXVECTOR3(nmy_flottaxPos + 1.0f, nmy_flottayPos + 1.0f, 0.0f);
	ai_j_also = D3DXVECTOR3(nmy_flottaxPos + 1.0f, nmy_flottayPos - 1.0f, 0.0f);

	// check for start capture
	cap_rendszer_player = -1;
	cap_rendszer_ai = -1;
	for (int i = 0; i < 30; i++)
	{
		if ((starsX[i] < flotta1xPos + 2.0f && starsX[i] > flotta1xPos - 2.0f) && (starsY[i] < flotta1yPos + 2.0f && starsY[i] > flotta1yPos - 2.0f) && owner[i] != player)
		{
			cap_rendszer_player = i;
			if (cap_player_done)
			{
				owner[i] = player;
				penz = penz + 500;
				esemeny = names[i] + " :: System captured... Reward: 500 credit";
			}
		}
		if ((starsX[i] < nmy_flottaxPos + 2.0f && starsX[i] > nmy_flottaxPos - 2.0f) && (starsY[i] < nmy_flottayPos + 2.0f && starsY[i] > nmy_flottayPos - 2.0f) && owner[i] != ai)
		{
			cap_rendszer_ai = i;
			if (cap_ai_done)
			{
				owner[i] = ai;
				ai_penz = ai_penz + 500;
				esemeny = names[i] + " :: System captured by AI...";
			}
		}
	}
}

void mozog()
{
	float _indulX = flotta1xPos;
	float _indulY = flotta1yPos;

	float _celX = map_width * (pV * 2);
	float _celY = map_height * (pU * 2);

	int alvasido = 5; // ms ... 0.005s

	float speed = 4.0f;
	float egesz_utX = abs(_celX - _indulX);
	float egesz_utY = abs(_celY - _indulY);
	float utX = speed * ((egesz_utX / speed) / 200); // 1s/0.005s = 200
	float utY = speed * ((egesz_utY / speed) / 200);

	while (rmb && (flotta1xPos > 0 && flotta1yPos > 0) && (flotta1xPos < map_width && flotta1yPos < map_height) && !stop_thread)
		{
			if (!game_pause)
			{
				if (flotta1xPos < (_celX + utX) && flotta1xPos >(_celX - utX)) { flotta1xPos = _celX; }
				if (flotta1yPos < (_celY + utY) && flotta1yPos >(_celY - utY)) { flotta1yPos = _celY; }
				if (flotta1xPos == _celX && flotta1yPos == _celY) { rmb = FALSE; break; }
				if (_celX - flotta1xPos > 0) { flotta1xPos = flotta1xPos + utX; }
				if (_celX - flotta1xPos < 0) { flotta1xPos = flotta1xPos - utX; }
				if (_celY - flotta1yPos > 0) { flotta1yPos = flotta1yPos + utY; }
				if (_celY - flotta1yPos < 0) { flotta1yPos = flotta1yPos - utY; }
				Sleep(alvasido);
			}
		}

	return;
}

void capture_player()
{
	while (!game_over)
	{
		if (!game_pause)
		{
			if (cap_rendszer_player >= 0)
			{
				int current_1 = cap_rendszer_player;
				Sleep(3500);
				if (cap_rendszer_player == current_1) { Sleep(3500); }
				if (current_1 == cap_rendszer_player) { cap_player_done = TRUE; }
				Sleep(1000);
				cap_player_done = FALSE;
			}
		}
		Sleep(1000);
	}
}

void capture_ai()
{
	while (!game_over)
	{
		if (!game_pause)
		{
			if (cap_rendszer_ai >= 0)
			{
				int current_2 = cap_rendszer_ai;
				Sleep(3500);
				if (cap_rendszer_ai == current_2) { Sleep(3500); }
				if (current_2 == cap_rendszer_ai) { cap_ai_done = TRUE; }
				Sleep(1000);
				cap_ai_done = FALSE;
			}
		}
		Sleep(1000);
	}
}

void flotta()
{
	hajok_szama = 1;
	legenyseg_szama = 40 * hajok_szama;

	nmy_hajok_szama = 1;
	nmy_legenyseg_szama = 40 * nmy_hajok_szama;
}

void game_time()
{
	while (!game_over) 
	{
		if (!game_pause)
		{
			Sleep(100);
			nap++;
			if (nap > 30) { honap++; nap = 1; }
			if (honap > 12) { evszam++; honap = 1; }
			if (honap == 1 && nap == 1)
			{
				for (int i = 0; i < 30; i++)
				{
					if (owner[i] == player) { system_count++; }
				}
				esemeny = "New Year: " + to_string(evszam) + ", Tax revenue: " + to_string(system_count * 200) + " credit";
				penz = penz + system_count * 200;
				system_count = 0;
			}
		}
	}
}

void gep()
{
	thread szal2(move_ai);
	szal2.detach();
}

void move_ai()
{
	while ((nmy_flottaxPos > 0 && nmy_flottayPos > 0) && (nmy_flottaxPos < map_width && nmy_flottayPos < map_height))
	{
		if (!game_pause && !stop_thread)
		{
			nmy_flottaxPos = nmy_flottaxPos + 0.002f;
			nmy_flottayPos = nmy_flottayPos + 0.002f;
			Sleep(5);
		}
	}
}

void gyartas()
{

}

void rendszer()
{

}

void save()
{
	ofstream savegame;
	savegame.open("save.sav");
	savegame << flotta1xPos << endl;
	savegame << flotta1yPos << endl;
	savegame << nmy_flottaxPos << endl;
	savegame << nmy_flottayPos << endl;
	savegame << penz << endl;
	savegame << ai_penz << endl;
	savegame << evszam << endl;
	for (int i = 0; i < 30; i++)
	{
		savegame << owner[i] << endl;
		savegame << planets[i] << endl;
	}
	savegame.close();
}

void load()
{
	string adat;
	ifstream loadgame;
	loadgame.open("save.sav");
	if (loadgame.is_open())
	{
		getline(loadgame, adat);
		flotta1xPos = stof(adat);
		getline(loadgame, adat);
		flotta1yPos = stof(adat);
		getline(loadgame, adat);
		nmy_flottaxPos = stof(adat);
		getline(loadgame, adat);
		nmy_flottayPos = stof(adat);
		getline(loadgame, adat);
		penz = stof(adat);
		getline(loadgame, adat);
		ai_penz = stof(adat);
		getline(loadgame, adat);
		evszam = stof(adat);
		for (int i = 0; i < 30; i++)
		{
			getline(loadgame, adat);
			owner[i] = adat;
			getline(loadgame, adat);
			planets[i] = stof(adat);
		}
		loadgame.close();
	}
}