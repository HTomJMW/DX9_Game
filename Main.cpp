// Windows és Direct3D fejállományok, stb... befordítása
#include <tchar.h>
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <thread>
#include <vector>
#include <dshow.h>
#include <cmath>
#include <algorithm>
#include <time.h>
#include "Main.h"
#include "camera.h"
#include "menu.h"
#include "struct.h"
#include "csillagok.h"
#include "bolygok.h"
#include "holdak.h"
#include "hajok.h"
#include "utkozesvizsgalat.h"
#include "hierarchy.h"

// névterek
using namespace std;

// függvénykönyvtárak linkelése
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "Strmiids.lib")

// global deklarációk
// d3d
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;

// vbuffer
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;

// meshek
LPD3DXMESH meshes[max_mesh];

// textúrák és anyagok
IDirect3DTexture9 *textures[25];

vector<D3DMATERIAL9> mtrls_mesh_0(0);
vector<IDirect3DTexture9*> textures_mesh_0(0);

vector<D3DMATERIAL9> mtrls_mesh_1(0);
vector<IDirect3DTexture9*> textures_mesh_1(0);

vector<D3DMATERIAL9> mtrls_mesh_2(0);
vector<IDirect3DTexture9*>  textures_mesh_2(0);

vector<D3DMATERIAL9> mtrls_mesh_3(0);
vector<IDirect3DTexture9*>  textures_mesh_3(0);

vector<D3DMATERIAL9> mtrls_mesh_4(0);
vector<IDirect3DTexture9*>  textures_mesh_4(0);

vector<D3DMATERIAL9> mtrls_mesh_5(0);
vector<IDirect3DTexture9*>  textures_mesh_5(0);

// cursor
D3DXIMAGE_INFO cursor_info; // képfájl infók
IDirect3DSurface9 *cursor;

// idõ
SYSTEMTIME win_time;
time_t timer;

// objektumok
Camera TheCamera(Camera::LANDOBJECT);
Menu TheMenu;
Utkozesvizsgalat TheCollision;
Hierarchy TheAllocHierarchy;

// hierarchia
D3DXMATRIXA16 world_matrix;
LPD3DXFRAME pFrameHeirarchy = NULL;
ID3DXAnimationController* pAnimController = NULL;
ID3DXAnimationSet* pAnimSet = NULL;

// obj vectorok
vector<Csillagok> csillagok_vec;
vector<Bolygok> bolygok_vec;
vector<Holdak> holdak_vec;
vector<Hajok> hajok_vec;

// vonalak
LPD3DXLINE line;

LPD3DXLINE vonalak[4]; // kijelolõ vonalak

D3DXVECTOR2 vonal_1[2];
D3DXVECTOR2 vonal_2[2];
D3DXVECTOR2 vonal_3[2];
D3DXVECTOR2 vonal_4[2];

// kijelölõ koordináták csoportos pickhez
//vector<int> x_coords;
//vector<int> y_coords;

// shader
ID3DXEffect* effect_ptr = NULL;
D3DXHANDLE tech_handle;
D3DXHANDLE WVP_handle;
D3DXHANDLE color_handle;
D3DXHANDLE texture_handle;
IDirect3DVertexDeclaration9* decl_ptr = NULL;
// shader vertex puffer
LPDIRECT3DVERTEXBUFFER9 shader_vpuffer_ptr = NULL;
//3D-koordináta vertex struktúra
D3DVERTEXELEMENT9 vertexElements[] =
{ 
	{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12,  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};

// hangok
LONGLONG start_sec = 0 * 10000000;
IGraphBuilder *s_graph;
IMediaControl *s_mediacontrol;
IMediaEvent	*s_event;
IMediaSeeking *s_seeking;

// infó kiírás
ID3DXFont *player_name = NULL;
RECT player_rect;
ID3DXFont *fps_counter = NULL;
RECT fps_rect;
ID3DXFont *esemenynaplo = NULL;
RECT esemeny_rect;
ID3DXFont *pontos_ido = NULL;
RECT pontos_ido_rect;
ID3DXFont *game_datum = NULL;
RECT game_datum_rect;
ID3DXFont *adatok = NULL;
RECT adatok_rect;

// sprites
ID3DXSprite *sprite1 = NULL;
D3DXVECTOR3 position = D3DXVECTOR3(screen_width - 165.0f, 20.0f, 0.0f);

ID3DXSprite *flares[rnd_sprite_meret];

// menü
ID3DXSprite *menu_hatter_nagy = NULL;
D3DXVECTOR3 menu_hatter_nagy_pos = D3DXVECTOR3(screen_width / 2 - 300, 100.0f, 0.0f);
RECT menu_hatter_nagy_rect;

ID3DXSprite *menu_hatter = NULL;
D3DXVECTOR3 menu_hatter_pos = D3DXVECTOR3(screen_width / 2 - 200, 200.0f, 0.0f);
RECT menu_hatter_rect;

ID3DXSprite *gomb_continue = NULL;
D3DXVECTOR3 gomb_continue_pos = D3DXVECTOR3(screen_width / 2 - 128, 200.0f, 0.0f);
RECT gomb_continue_rect;

ID3DXSprite *gomb_save = NULL;
D3DXVECTOR3 gomb_save_pos = D3DXVECTOR3(screen_width / 2 - 128, 300.0f, 0.0f);
RECT gomb_save_rect;

ID3DXSprite *gomb_load = NULL;
D3DXVECTOR3 gomb_load_pos = D3DXVECTOR3(screen_width / 2 - 128, 400.0f, 0.0f);
RECT gomb_load_rect;

ID3DXSprite *gomb_settings = NULL;
D3DXVECTOR3 gomb_settings_pos = D3DXVECTOR3(screen_width / 2 - 128, 500.0f, 0.0f);
RECT gomb_settings_rect;

ID3DXSprite *gomb_exit = NULL;
D3DXVECTOR3 gomb_exit_pos = D3DXVECTOR3(screen_width / 2 - 128, 600.0f, 0.0f);
RECT gomb_exit_rect;

// prototípusok
void initD3D(HWND hWnd);
void render_frame(void);
void cleanD3D(void);
void init_graphics(void);
void init_shader(void);
void sound(void);
void random_csillagok();
int delta_time();
int calc_fps();
Ray CalcPickingRay(LPDIRECT3DDEVICE9 Device, int, int);
void TransformRay(Ray* ray, D3DXMATRIX* T);
bool pick(ID3DXMesh*, D3DXMATRIX &);
void tav_fuggo_pick(void);
void send();
void init_light(void);
void update_cel(void);
string ora();
void kering(void);
float tavmeres(D3DXVECTOR3, D3DXVECTOR3);
void datum_ingame(void);
void init_materials(void);
D3DXVECTOR3 get_pos_from_matrix(D3DXMATRIX);
string adatkiiro();
void uj_hajo(void);
void kijelolo(void);
void utkozesvizsgalat_loop(void);
void animacio_ctrl(void);
void futasi_ido_loop(void);
void DrawFrame(IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame);
void DrawMeshContainer(IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);

// WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// windows program belépési pontja
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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

	// ablak létrehozása
	hWnd = CreateWindowEx(NULL, _T("WindowClass"), _T("DX9 Game"),
		WS_WINDOW, window_pos_left, window_pos_top, screen_width, screen_height,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow); // ablak megjelenítése

	setlocale(LC_ALL, "hun"); // ékezetes karakterek

	// set up and initialize Direct3D
	initD3D(hWnd);
	
	// enter the main loop:
	MSG msg;
	msg.message = WM_NULL;
	
	while (!game_over)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		delta_time();
		frames++;
		if (!game_start && !game_pause)
		{
			TheCamera.camera_move(cel_UWV);
			if (kijelolo_negyzet) { kijelolo(); }
			update_cel();
			kering();
			if (pAnimController != NULL) { pAnimController->AdvanceTime(0.0002, NULL); }
			if (pFrameHeirarchy != NULL) { UpdateFrameMatrices(pFrameHeirarchy, &world_matrix); }
		}
		render_frame();
		GetLocalTime(&win_time);
		if (frames % 30 == 0) { fps = calc_fps(); ido = ora(); frames = 0; }
	}

	cleanD3D(); // felszabadítás

	return msg.wParam;
}

// üzenetkezelõ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_SETCURSOR:
		{
			SetCursor(NULL); // win cursor kikapcsolása
			d3ddev->ShowCursor(TRUE);
			return TRUE; // win cursor akadályozása
		} break;

		case WM_MOUSEWHEEL:
		{
			scroll = (int)(short)HIWORD(wParam);
		} break;

		case WM_MOUSEMOVE:
		{
			moveXPos = GET_X_LPARAM(lParam);
			moveYPos = GET_Y_LPARAM(lParam);

			// kamera mozgatás egér görgõ gombbal
			if (eger_fordulas)
			{
				TheCamera.camera_turn();
			}
		} break;

		case WM_LBUTTONDOWN:
		{
			clickxPos_lmb = GET_X_LPARAM(lParam);
			clickyPos_lmb = GET_Y_LPARAM(lParam);

			if (game_pause || game_start)
			{
				TheMenu.menu_fgv(screen_width,  screen_height);
			} else {
				tav_fuggo_pick();
				kijelolo_negyzet = TRUE;
			}
		} break;

		case WM_LBUTTONUP:
		{
			kijelolo_negyzet = FALSE;
		} break;

		case WM_MBUTTONDOWN:
		{
			last_Xpos = w_click_X = GET_X_LPARAM(lParam);
			last_Ypos = w_click_Y = GET_Y_LPARAM(lParam);
			eger_fordulas = TRUE;
		} break;

		case WM_MBUTTONUP:
		{
			eger_fordulas = FALSE;
		} break;

		case WM_RBUTTONDOWN:
		{
			clickxPos_rmb = GET_X_LPARAM(lParam);
			clickyPos_rmb = GET_Y_LPARAM(lParam);

			send();
		} break;

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case 0x57: TheCamera.elore_gomb = TRUE;				break;
				case 0x53: TheCamera.hatra_gomb = TRUE;				break;
				case 0x44: TheCamera.jobbra_gomb = TRUE;			break;
				case 0x41: TheCamera.balra_gomb = TRUE;				break;
				case 0x51: TheCamera.q_gomb = TRUE;					break;
				case 0x59: TheCamera.y_gomb = TRUE;					break;
				case VK_UP: TheCamera.felnez_gomb = TRUE;			break;
				case VK_DOWN: TheCamera.lenez_gomb = TRUE;			break;
				case VK_RIGHT: TheCamera.jobbrafordul_gomb = TRUE;	break;
				case VK_LEFT: TheCamera.balrafordul_gomb = TRUE;	break;
				case VK_PRIOR: TheCamera.emelkedik_gomb = TRUE;		break;
				case VK_NEXT: TheCamera.sullyed_gomb = TRUE;		break;
				case VK_SHIFT: TheCamera.shift_gomb = TRUE;			break;
				case VK_SPACE:
					TheCamera.setPosition(&cam_pos);				break;
				case VK_ESCAPE: case VK_PAUSE:
					if (!game_start && !game_pause)
					{
						game_pause = TRUE;
						thread szal_0(&Menu::menu_loop, ref(TheMenu), screen_height, screen_height);
						szal_0.detach();
					} else {
						game_pause = FALSE;
					}												break;
			}
		} break;

		case WM_KEYUP:
		{
			switch (wParam)
			{
				case 0x57: TheCamera.elore_gomb = FALSE;			break;
				case 0x53: TheCamera.hatra_gomb = FALSE;			break;
				case 0x44: TheCamera.jobbra_gomb = FALSE;			break;
				case 0x41: TheCamera.balra_gomb = FALSE;			break;
				case 0x51: TheCamera.q_gomb = FALSE;				break;
				case 0x59: TheCamera.y_gomb = FALSE;				break;
				case VK_UP: TheCamera.felnez_gomb = FALSE;			break;
				case VK_DOWN: TheCamera.lenez_gomb = FALSE;			break;
				case VK_RIGHT: TheCamera.jobbrafordul_gomb = FALSE;	break;
				case VK_LEFT: TheCamera.balrafordul_gomb = FALSE;	break;
				case VK_PRIOR: TheCamera.emelkedik_gomb = FALSE;	break;
				case VK_NEXT: TheCamera.sullyed_gomb = FALSE;		break;
				case VK_SHIFT: TheCamera.shift_gomb = FALSE;		break;
			}
		} break;

		case WM_CHAR:
		{
			switch (wParam)
				{
				case '1':
					{
						s_seeking->SetPositions(&start_sec, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
						s_mediacontrol->Run();
					} break;
				case '2':
					{
						s_mediacontrol->Stop();
						s_seeking->SetPositions(&start_sec, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
					} break;
				case '3':
					{
						uj_hajo();
					} break;
				case '4':
					{
						pAnimController->ResetTime();
					} break;
				}
		} break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// initializes and prepares Direct3D
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION); //d3d interfész mutatója

	D3DPRESENT_PARAMETERS d3dpp; // paraméterek tárolása

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = screen_width;
	d3dpp.BackBufferHeight = screen_height;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	if (frissit_azonnal) // IMMEDIATE - 1000 FPS, DEFAULT - 60 FPS
	{ d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; } else { d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; }

	// eszköztámogatás ellenõrzése (szoftveres vagy hardveres vertex kezelés)
	D3DCAPS9 D3dCaps;

	d3d->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&D3dCaps);

	DWORD DwBehaviorFlags = 0;
	if (D3dCaps.VertexProcessingCaps != 0)
	{ DwBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING; } else { DwBehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING; }

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		DwBehaviorFlags, // szoftveres vagy hardveres vertex kezelés
		&d3dpp,
		&d3ddev);

	init_graphics();    // call the function to initialize the shapes
	init_materials();
	init_light(); // call the light
	init_shader();
	sound(); // call sound
	// menu loop a starthoz
	thread szal_2(&Menu::menu_loop, ref(TheMenu), screen_width, screen_height);
	szal_2.detach();

	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // 3d fények bekapcsolása
	d3ddev->SetRenderState(D3DRS_SPECULARENABLE, TRUE); // fényvisszaverõdés bekapcsolása
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // Z-buffer bekapcsolása
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(30, 30, 30)); // ambient light
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);    // turn off the color blending
	//d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);    // set source factor
	//d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // set dest factor
	d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);    // set the operation
	d3ddev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD); // árnyékolás beállítsa
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // vertex bejárás óra mutató járásával azonos

	// áttetszõ textúrák
	d3ddev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x0000008f);
	d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	HRESULT hr0 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &player_name);
	SetRect(&player_rect, 20, 20, 400, 60);
	HRESULT hr1 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &fps_counter);
	SetRect(&fps_rect, 20, 60, 200, 120);
	HRESULT hr2 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &esemenynaplo);
	SetRect(&esemeny_rect, (screen_width / 2) - 200, 60, (screen_width / 2) + 200, 120);
	HRESULT hr3 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &pontos_ido);
	SetRect(&pontos_ido_rect, 20, 100, 200, 160);
	HRESULT hr4 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &game_datum);
	SetRect(&game_datum_rect, 20, 140, 200, 200);
	HRESULT hr5 = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &adatok);
	SetRect(&adatok_rect, screen_width - 220, 180, screen_width - 20, 480);

	HRESULT hr6 = D3DXCreateTextureFromFile(d3ddev, "Pictures/csillag.bmp", &textures[0]);
	HRESULT hr7 = D3DXCreateTextureFromFile(d3ddev, "Pictures/player.bmp", &textures[1]); // szabad
	HRESULT hr8 = D3DXCreateTextureFromFile(d3ddev, "Pictures/player_2.bmp", &textures[2]); // szabad
	HRESULT hr9 = D3DXCreateTextureFromFile(d3ddev, "Pictures/bolygo.png", &textures[3]);
	HRESULT hr10 = D3DXCreateTextureFromFile(d3ddev, "Pictures/hold.png", &textures[4]);
	HRESULT hr11 = D3DXCreateTextureFromFile(d3ddev, "Pictures/flotta.png", &textures[5]);
	HRESULT hr12 = D3DXCreateTextureFromFile(d3ddev, "Pictures/flare.png", &textures[6]);
	HRESULT hr13 = D3DXCreateTextureFromFile(d3ddev, "Pictures/menu_hatter.png", &textures[7]);
	HRESULT hr14 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_conti.png", &textures[8]);
	HRESULT hr15 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_save.png", &textures[9]);
	HRESULT hr16 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_load.png", &textures[10]);
	HRESULT hr17 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_set.png", &textures[11]);
	HRESULT hr18 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_exit.png", &textures[12]);
	HRESULT hr19 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_conti_a.png", &textures[13]);
	HRESULT hr20 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_save_a.png", &textures[14]);
	HRESULT hr21 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_load_a.png", &textures[15]);
	HRESULT hr22 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_set_a.png", &textures[16]);
	HRESULT hr23 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_exit_a.png", &textures[17]);
	HRESULT hr24 = D3DXCreateTextureFromFile(d3ddev, "Pictures/muhold.bmp", &textures[18]); // szabad
	HRESULT hr25 = D3DXCreateTextureFromFile(d3ddev, "Pictures/flare_1.png", &textures[19]);
	HRESULT hr26 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_start.png", &textures[20]);
	HRESULT hr27 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_start_a.png", &textures[21]);
	HRESULT hr28 = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_save_b.png", &textures[22]);
	HRESULT hr29 = D3DXCreateTextureFromFile(d3ddev, "Pictures/egitest.png", &textures[23]);
	HRESULT hr30 = D3DXCreateTextureFromFile(d3ddev, "Pictures/menu_hatter_nagy.png", &textures[24]);

	ID3DXBuffer* adjBuffer[max_mesh];
	ID3DXBuffer* mtrlBuffer[max_mesh];
	DWORD numMtrls[max_mesh];

	HRESULT hr31 = D3DXLoadMeshFromX("Models/flagship_3.x", D3DXMESH_MANAGED, d3ddev, &adjBuffer[0], &mtrlBuffer[0], 0, &numMtrls[0], &meshes[0]);

	if (mtrlBuffer[0] != 0 && numMtrls[0] != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer[0]->GetBufferPointer();

		for (UINT i = 0; i < numMtrls[0]; i++)
		{
			mtrls_mesh_0.push_back(mtrls[i].MatD3D);

			mtrls_mesh_0[i].Ambient = mtrls_mesh_0[i].Diffuse;

			if (mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;

				LPSTR file_name = mtrls[i].pTextureFilename;
				TCHAR dest[40] = _T("Models/");
				StringCchCat(dest, 40, file_name);

				D3DXCreateTextureFromFile(d3ddev, dest, &tex);
				textures_mesh_0.push_back(tex);
			}
		}
	}
	mtrlBuffer[0]->Release();

	HRESULT hr32 = D3DXLoadMeshFromX("Models/gomb_nagy.x", D3DXMESH_MANAGED, d3ddev, &adjBuffer[1], &mtrlBuffer[1], 0, &numMtrls[1], &meshes[1]);

	HRESULT hr33 = D3DXLoadMeshFromX("Models/gomb_kozepes.x", D3DXMESH_MANAGED, d3ddev, &adjBuffer[2], &mtrlBuffer[2], 0, &numMtrls[2], &meshes[2]);

	HRESULT hr34 = D3DXLoadMeshFromX("Models/gomb_kicsi.x", D3DXMESH_MANAGED, d3ddev, &adjBuffer[3], &mtrlBuffer[3], 0, &numMtrls[3], &meshes[3]);

	HRESULT hr35 = D3DXLoadMeshFromX("Models/cel.x", D3DXMESH_MANAGED, d3ddev, &adjBuffer[4], &mtrlBuffer[4], 0, &numMtrls[4], &meshes[4]);
	
	HRESULT hr36 = D3DXLoadMeshFromX("Models/muhold.x", D3DXMESH_MANAGED, d3ddev, &adjBuffer[5], &mtrlBuffer[5], 0, &numMtrls[5], &meshes[5]);

	if (mtrlBuffer[5] != 0 && numMtrls[5] != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer[5]->GetBufferPointer();

		for (UINT i = 0; i < numMtrls[5]; i++)
		{
			mtrls_mesh_5.push_back(mtrls[i].MatD3D);

			mtrls_mesh_5[i].Ambient = mtrls_mesh_5[i].Diffuse;

			if (mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;

				LPSTR file_name = mtrls[i].pTextureFilename;
				TCHAR dest[40] = _T("Models/");
				StringCchCat(dest, 40, file_name);

				D3DXCreateTextureFromFile(d3ddev, dest, &tex);
				textures_mesh_5.push_back(tex);
			}
		}
	}
	mtrlBuffer[5]->Release();

	HRESULT hr49 = D3DXLoadMeshHierarchyFromX("Models/flagship_3.x", D3DXMESH_MANAGED, d3ddev, &TheAllocHierarchy, NULL, &pFrameHeirarchy, &pAnimController);
	animacio_ctrl();

	HRESULT hr37 = D3DXCreateLine(d3ddev, &line);

	HRESULT hr38 = D3DXCreateSprite(d3ddev, &sprite1);

	for (int i = 0; i < 20; i++)
	{
		D3DXCreateSprite(d3ddev, &flares[i]);
	}

	HRESULT hr39 = D3DXCreateSprite(d3ddev, &menu_hatter_nagy);
	SetRect(&menu_hatter_nagy_rect, 0, 0, 600, 750);

	HRESULT hr40 = D3DXCreateSprite(d3ddev, &menu_hatter);
	SetRect(&menu_hatter_rect, 0, 0, 400, 550);

	HRESULT hr41 = D3DXCreateSprite(d3ddev, &gomb_continue);
	SetRect(&gomb_continue_rect, 0, 0, 256, 100);

	HRESULT hr42 = D3DXCreateSprite(d3ddev, &gomb_save);
	SetRect(&gomb_save_rect, 0, 0, 256, 100);

	HRESULT hr43 = D3DXCreateSprite(d3ddev, &gomb_load);
	SetRect(&gomb_load_rect, 0, 0, 256, 100);

	HRESULT hr44 = D3DXCreateSprite(d3ddev, &gomb_settings);
	SetRect(&gomb_settings_rect, 0, 0, 256, 100);

	HRESULT hr45 = D3DXCreateSprite(d3ddev, &gomb_exit);
	SetRect(&gomb_exit_rect, 0, 0, 256, 100);

	HRESULT hr46 = D3DXCreateEffectFromFile(d3ddev, "shader.fx", 0, 0, 0, 0, &effect_ptr, NULL);
	tech_handle = effect_ptr->GetTechniqueByName("Tech");
	WVP_handle = effect_ptr->GetParameterByName(0, "WorldViewProjection");
	color_handle = effect_ptr->GetParameterByName(0, "MaterialColor");
	texture_handle = effect_ptr->GetParameterByName(0, "Texture_A");
	d3ddev->CreateVertexDeclaration(vertexElements, &decl_ptr);

	// cursor
	HRESULT hr47 = D3DXGetImageInfoFromFile("Pictures/cursor.png", &cursor_info);
	d3ddev->CreateOffscreenPlainSurface(cursor_info.Width, cursor_info.Height, cursor_info.Format, D3DPOOL_DEFAULT, &cursor, NULL);
	HRESULT hr48 = D3DXLoadSurfaceFromFile(cursor, NULL, NULL, "Pictures/cursor.png", NULL, D3DX_FILTER_NONE, feher, NULL);
	d3ddev->SetCursorProperties(0, 0, cursor);
	d3ddev->SetCursorPosition(screen_width / 2, screen_height / 3, D3DCURSOR_IMMEDIATE_UPDATE);

	// kijelölõ
	for (int i = 0; i < 4; i++)
	{
		D3DXCreateLine(d3ddev, &vonalak[i]);
	}

	// szálak
	thread szal_1(datum_ingame); // start dátum szál
	szal_1.detach();

	// ütkozésvizsgálat
	thread szal_3(utkozesvizsgalat_loop);
	szal_3.detach();

	thread szal_4(futasi_ido_loop);
	szal_4.detach();

	// fgvk
	random_csillagok();

	// init objecktumok
	Csillagok csillag_0 = { "GAX-A001", "Fõsorozat", 1, textures[0], meshes[1], D3DXVECTOR3(640.0f, 640.0f, 640.0f), 1.5f };
	Bolygok bolygo_0 = { "G-01", "Kõzet", TRUE, 5000, "Ember", 1, textures[3], meshes[2], D3DXVECTOR3(300.0f, 0.0f, 0.0f), D3DXVECTOR3(640.0f, 640.0f, 640.0f), 0.3f };
	Bolygok bolygo_1 = { "G-02", "Kõzet", TRUE, 1000, "Ember", 1, textures[4], meshes[3], D3DXVECTOR3(500.0f, 0.0f, 0.0f), D3DXVECTOR3(640.0f, 640.0f, 640.0f), 0.2f };
	Hajok hajo_0 = { "Flagship", "Ûrhajó", 2000, 500, 15.0f, 250, FALSE, "Ember", 0, 0.0f, mtrls_mesh_0, textures_mesh_0, meshes[0], D3DXVECTOR3(1024.0f, 640.0f, 640.0f), D3DXVECTOR3(1024.0f, 640.0f, 640.0f) };
	Hajok hajo_1 = { "Mh-1", "Mûhold", 500, 100, 0.0f, 0, FALSE, "Ember", 0, 270.0f, mtrls_mesh_5, textures_mesh_5, meshes[5], D3DXVECTOR3(640.0f, 720.0f, 500.0f), D3DXVECTOR3(640.0f, 720.0f, 500.0f) };

	// vectorhoz adás
	csillagok_vec.push_back(csillag_0);
	bolygok_vec.push_back(bolygo_0);
	bolygok_vec.push_back(bolygo_1);
	hajok_vec.push_back(hajo_0);
	hajok_vec.push_back(hajo_1);
}

// render a single frame
void render_frame(void)
{
	// pufferek törlése, feltöltés fekete színnel
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, fekete, 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, fekete, 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_STENCIL, fekete, 1.0f, 0);

	d3ddev->BeginScene();

	// select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);

	// set the view transform - flexcam
	D3DXMATRIX V;
	TheCamera.getViewMatrix(&V);
	d3ddev->SetTransform(D3DTS_VIEW, &V);

	// set the projection transform
	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(60),    // the horizontal field of view
		(FLOAT)screen_width / (FLOAT)screen_height, // aspect ratio
		0.001f,    // the near view-plane
		10000.0f);    // the far view-plane
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection

	if (!game_start && !game_pause)
	{
		D3DXMATRIX matRotateX;
		D3DXMATRIX matRotateY;
		D3DXMATRIX matRotateZ;
		D3DXMATRIX matScale;
		D3DXMATRIX matTranslate;

		// DX9 shader v 2.0 p 2.0

		d3ddev->SetStreamSource(0, shader_vpuffer_ptr, 0, sizeof(SVertex));
		d3ddev->SetVertexDeclaration(decl_ptr);

		D3DXMATRIXA16 _WorldViewProjection;

		D3DXMatrixTranslation(&matTranslate, 64.0f, 64.0f, 64.0f);
		d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));

		_WorldViewProjection = matTranslate * V * matProjection;

		effect_ptr->SetValue(color_handle, &sarga, sizeof(D3DXCOLOR));
		effect_ptr->SetTexture(texture_handle, textures[0]);
		effect_ptr->SetMatrix(WVP_handle, &(_WorldViewProjection));
		effect_ptr->SetTechnique(tech_handle);

		UINT passCnt = 1;
		effect_ptr->Begin(&passCnt, 0);
		for (UINT pass = 0; pass < passCnt; pass++)
		{
			effect_ptr->BeginPass(pass);
			//d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1); // disabled
			effect_ptr->EndPass();
		}
		effect_ptr->End();

		// normál DirectX 9
		// set the stream source
		d3ddev->SetStreamSource(1, v_buffer, 0, sizeof(CUSTOMVERTEX));

		d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
		d3ddev->SetLight(0, &light);
		d3ddev->LightEnable(0, TRUE);
		d3ddev->SetMaterial(&material2);
		d3ddev->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

		// mesh-ek
		for (size_t i = 0; i < csillagok_vec.size(); i++)
		{
			csillagok_vec[i].render();
		}

		d3ddev->SetMaterial(&material);

		for (size_t i = 0; i < bolygok_vec.size(); i++)
		{
			bolygok_vec[i].render();
		}

		for (size_t i = 0; i < holdak_vec.size(); i++)
		{
			holdak_vec[i].render();
		}

		for (size_t i = 0; i < hajok_vec.size(); i++)
		{
			hajok_vec[i].mozgas();
			hajok_vec[i].render();
			if (hajok_vec[i].get_pick())
			{
				hajok_vec[i].mozgas_vonal();
				hajok_vec[i].mv_render(V, matProjection);
				hajok_vec[i].cel_render(meshes[4]);
			}
		}

		// animáció
		D3DXMatrixTranslation(&world_matrix, 1024.0f, 640.0f, 600.0f);
		d3ddev->SetTransform(D3DTS_WORLD, &world_matrix);
		DrawFrame(d3ddev, pFrameHeirarchy);

		// pont csillagok
		d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
		d3ddev->SetTexture(0, NULL);
		d3ddev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&pointSize));
		for (int i = 0; i < rnd_meret; i++)
		{
			D3DXMatrixTranslation(&matTranslate, rnd[i].x, rnd[i].y, rnd[i].z);
			d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));
			d3ddev->DrawPrimitive(D3DPT_POINTLIST, 0, 1);
		}

		// kiírások
		player_name->DrawTextA(NULL, ("Player: " + p_nev).c_str(), -1, &player_rect, DT_LEFT, feher);
		fps_counter->DrawTextA(NULL, (to_string(fps) + " FPS").c_str(), -1, &fps_rect, DT_LEFT, feher);
		esemenynaplo->DrawTextA(NULL, debug.c_str(), -1, &esemeny_rect, DT_CENTER, feher);
		pontos_ido->DrawTextA(NULL, ido.c_str(), -1, &pontos_ido_rect, DT_LEFT, feher);
		game_datum->DrawTextA(NULL, datum.c_str(), -1, &game_datum_rect, DT_LEFT, feher);
		adatok->DrawTextA(NULL, (adatkiiro()).c_str(), -1, &adatok_rect, DT_LEFT, feher);

		// sprites
		sprite1->Begin(D3DXSPRITE_ALPHABLEND);
		for (size_t i = 0; i < csillagok_vec.size(); i++)
		{
			if (csillagok_vec[i].get_pick())
			{
				sprite1->Draw(textures[23], NULL, NULL, &position, feher);
				break;
			}
		}

		for (size_t i = 0; i < bolygok_vec.size(); i++)
		{
			if (bolygok_vec[i].get_pick())
			{
				sprite1->Draw(textures[23], NULL, NULL, &position, feher);
				break;
			}
		}

		for (size_t i = 0; i < holdak_vec.size(); i++)
		{
			if (holdak_vec[i].get_pick())
			{
				sprite1->Draw(textures[23], NULL, NULL, &position, feher);
				break;
			}
		}

		for (size_t i = 0; i < hajok_vec.size(); i++)
		{
			if (hajok_vec[i].get_pick())
			{
				sprite1->Draw(textures[5], NULL, NULL, &position, feher);
				break;
			}
		}
		sprite1->End();

		// flares
		D3DXMATRIX wrld;
		D3DXMATRIX vw;

		d3ddev->GetTransform(D3DTS_VIEW, &vw);

		for (int i = 0; i < rnd_sprite_meret; i++)
		{
			D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
			D3DXMatrixTranslation(&matTranslate, rnd_sprite[i].x, rnd_sprite[i].y, rnd_sprite[i].z);
			d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matTranslate));

			d3ddev->GetTransform(D3DTS_WORLD, &wrld);
			flares[i]->SetWorldViewLH(&wrld, &vw);

			flares[i]->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE | D3DXSPRITE_BILLBOARD);
			flares[i]->Draw(textures[6], NULL, NULL, NULL, feher);
			flares[i]->End();
		}

		// kijelölõ
		if (kijelolo_negyzet)
		{
			for (int i = 0; i < 4; i++)
			{
				vonalak[i]->SetWidth(1.0f);
				vonalak[i]->SetAntialias(TRUE);
				vonalak[i]->Begin();
				if (i == 0) { vonalak[i]->Draw(vonal_1, 2, feher); }
				if (i == 1) { vonalak[i]->Draw(vonal_2, 2, feher); }
				if (i == 2) { vonalak[i]->Draw(vonal_3, 2, feher); }
				if (i == 3) { vonalak[i]->Draw(vonal_4, 2, feher); }
				vonalak[i]->End();
			}
		}

	} else {
		// menü
		menu_hatter_nagy->Begin(D3DXSPRITE_ALPHABLEND);
		menu_hatter_nagy->Draw(textures[24], &menu_hatter_nagy_rect, NULL, &menu_hatter_nagy_pos, feher);
		menu_hatter_nagy->End();

		menu_hatter->Begin(D3DXSPRITE_ALPHABLEND);
		menu_hatter->Draw(textures[7], &menu_hatter_rect, NULL, &menu_hatter_pos, feher);
		menu_hatter->End();

		if (game_start)
		{
			gomb_continue->Begin(D3DXSPRITE_ALPHABLEND);
			if (TheMenu.cursor_start) {
				gomb_continue->Draw(textures[21], &gomb_continue_rect, NULL, &gomb_continue_pos, feher);
			} else {
				gomb_continue->Draw(textures[20], &gomb_continue_rect, NULL, &gomb_continue_pos, feher);
			}
			gomb_continue->End();

			gomb_save->Begin(D3DXSPRITE_ALPHABLEND);
			if (TheMenu.cursor_save || !TheMenu.cursor_save)
			{
				gomb_save->Draw(textures[22], &gomb_save_rect, NULL, &gomb_save_pos, feher);
			}
			gomb_save->End();
		} else {
			gomb_continue->Begin(D3DXSPRITE_ALPHABLEND);
			if (TheMenu.cursor_conti) {
				gomb_continue->Draw(textures[13], &gomb_continue_rect, NULL, &gomb_continue_pos, feher);
			} else {
				gomb_continue->Draw(textures[8], &gomb_continue_rect, NULL, &gomb_continue_pos, feher);
			}
			gomb_continue->End();

			gomb_save->Begin(D3DXSPRITE_ALPHABLEND);
			if (TheMenu.cursor_save)
			{
				gomb_save->Draw(textures[14], &gomb_save_rect, NULL, &gomb_save_pos, feher);
			} else {
				gomb_save->Draw(textures[9], &gomb_save_rect, NULL, &gomb_save_pos, feher);
			}
			gomb_save->End();
		}

		gomb_load->Begin(D3DXSPRITE_ALPHABLEND);
		if (TheMenu.cursor_load)
		{
			gomb_load->Draw(textures[15], &gomb_load_rect, NULL, &gomb_load_pos, feher);
		} else {
			gomb_load->Draw(textures[10], &gomb_load_rect, NULL, &gomb_load_pos, feher);
		}
		gomb_load->End();

		gomb_settings->Begin(D3DXSPRITE_ALPHABLEND);
		if (TheMenu.cursor_set)
		{
			gomb_settings->Draw(textures[16], &gomb_settings_rect, NULL, &gomb_settings_pos, feher);
		} else {
			gomb_settings->Draw(textures[11], &gomb_settings_rect, NULL, &gomb_settings_pos, feher);
		}
		gomb_settings->End();

		gomb_exit->Begin(D3DXSPRITE_ALPHABLEND);
		if (TheMenu.cursor_exit)
		{
			gomb_exit->Draw(textures[17], &gomb_exit_rect, NULL, &gomb_exit_pos, feher);
		} else {
			gomb_exit->Draw(textures[12], &gomb_exit_rect, NULL, &gomb_exit_pos, feher);
		}
		gomb_exit->End();
	}

	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL); // lapváltás
}

// erõforrások felszabadítása
void cleanD3D(void)
{
	v_buffer->Release();    // close and release the vertex buffer
	d3ddev->Release();    // close and release the 3D device
	d3d->Release();    // close and release Direct3D
	player_name->Release();
	fps_counter->Release();
	esemenynaplo->Release();
	pontos_ido->Release();
	line->Release();
	sprite1->Release();
	menu_hatter_nagy->Release();
	menu_hatter->Release();
	gomb_continue->Release();
	gomb_save->Release();
	gomb_load->Release();
	gomb_settings->Release();
	gomb_exit->Release();
	game_datum->Release();
	adatok->Release();
	effect_ptr->Release();
	decl_ptr->Release();
	shader_vpuffer_ptr->Release();
	for (int i = 0; i < 4; i++)
	{
		vonalak[i]->Release();
	}
	for (int i = 0; i < max_mesh; i++)
	{
		meshes[i]->Release();
	}
	for (int i = 0; i < rnd_sprite_meret; i++)
	{
		flares[i]->Release();
	}
}

// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		//pont csillagokhoz
		{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // 0
	};

	// create a vertex buffer interface called v_buffer
	d3ddev->CreateVertexBuffer(1 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;    // a void pointer

	// lock v_buffer and load the vertices into it
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();
}

int calc_fps()
{
	if (delta > 0)
	{
		return (1000 / delta);
	} else {
		return 1000;
	}
}

int delta_time()
{
	kezdet = timeGetTime();
	delta = kezdet - vege;
	vege = kezdet;

	return delta; // 2 frame között eltelt idõ ms-ban
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

void sound(void)
{
	// Initialize COM
	CoInitialize(NULL);
	// Create the graph
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&s_graph);
	// Query for interface objects
	s_graph->QueryInterface(IID_IMediaControl, (void **)&s_mediacontrol);
	s_graph->QueryInterface(IID_IMediaEvent, (void **)&s_event);
	s_graph->QueryInterface(IID_IMediaSeeking, (void **)&s_seeking);
	// Load the sound
	s_graph->RenderFile(L"Audio\\action.wav", NULL);
	// Set the playback rate
	s_seeking->SetRate(1);
	// Set position
	//s_seeking->SetPositions(&start_sec, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	// Start sound
	//s_mediacontrol->Run();
}

void init_light(void)
{
	ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
	light.Type = D3DLIGHT_POINT;    // make the light type 'point light'
	light.Position = D3DXVECTOR3(640.0f, 640.0f, 640.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color (RGBA)
	light.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f); // fény iránya
	light.Attenuation1 = 0.0025f;
	light.Range = 1280.0f;
}

void init_materials(void)
{
	// visszaverödõ
	ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
	material.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);    // set ambient color to white
	material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   // set specular color to white
	material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // set emissive color to black
	material.Power = 80;

	// fénylõ
	ZeroMemory(&material2, sizeof(D3DMATERIAL9));
	material2.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material2.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	material2.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material2.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material2.Power = 200;
}

void update_cel(void)
{
	// világ-ray ütközés háromszög frissítés
	map_b_also = D3DXVECTOR3(0.0f, cel_UWV.y, 0.0f);
	map_b_felso = D3DXVECTOR3(0.0f, cel_UWV.y, 2560.0f);
	map_j_also = D3DXVECTOR3(2560.0f, cel_UWV.y, 0.0f);
	
	cel_UWV.x = map_width * (pV * 2);
	cel_UWV.z = map_depth * (pU * 2);
	//cel_UWV.y = cel_UWV.y; // külön állítva

	return;
}

string ora()
{
	int _ora = win_time.wHour;
	int _perc = win_time.wMinute;
	int _masodperc = win_time.wSecond;
	string _s_ora = "00";
	string _s_perc = "00";
	string _s_masodperc = "00";

	if (_ora < 10) { _s_ora = "0" + to_string(_ora); } else { _s_ora = to_string(_ora); }
	if (_perc < 10) { _s_perc = "0" + to_string(_perc); } else { _s_perc = to_string(_perc); }
	if (_masodperc < 10) { _s_masodperc = "0" + to_string(_masodperc); } else { _s_masodperc = to_string(_masodperc); }

	return (_s_ora + ":" + _s_perc + ":" + _s_masodperc);
}

void kering(void)
{
	ksz = ksz + 0.0002f * delta;
	if (ksz >= 360.0f) { ksz = 0.0f; }

	return;
}

float tavmeres(D3DXVECTOR3 start, D3DXVECTOR3 finish)
{
	float _tavolsag_vsz = 0.0f; // vízszintes - 2d
	float _tavolsag = 0.0f; // 3d

	float _ut_x = 0.0f;
	float _ut_y = 0.0f;
	float _ut_z = 0.0f;

	if (start.x < finish.x) { _ut_x = finish.x - start.x; }
	if (start.x > finish.x) { _ut_x = start.x - finish.x; }
	if (start.x == finish.x) { _ut_x = 0.0f; }

	if (start.y < finish.y) { _ut_y = finish.y - start.y; }
	if (start.y > finish.y) { _ut_y = start.y - finish.y; }
	if (start.y == finish.y) { _ut_y = 0.0f; }

	if (start.z < finish.z) { _ut_z = finish.z - start.z; }
	if (start.z > finish.z) { _ut_z = start.z - finish.z; }
	if (start.z == finish.z) { _ut_z = 0.0f; }

	_tavolsag_vsz = sqrt((_ut_x * _ut_x) + (_ut_z * _ut_z));
	_tavolsag = sqrt((_tavolsag_vsz * _tavolsag_vsz) + (_ut_y * _ut_y));

	return _tavolsag;
}

void datum_ingame(void)
{
	string _honap = "";
	string _nap = "";

	while (!game_over)
	{
		if (!game_pause && !game_start)
		{
			if (nap > 30) { nap = 1; honap++; }
			if (nap < 10) { _nap = "0" + to_string(nap); } else { _nap = to_string(nap); }
			if (honap > 12) { honap = 1; ev++; }
			if (honap < 10) { _honap = "0" + to_string(honap); } else { _honap = to_string(honap); }

			datum = to_string(ev) + ". " + _honap + ". " + _nap + ".";

			Sleep(1000);
			nap++;
		} else {
			Sleep(1000);
		}
	}

	return;
}

D3DXVECTOR3 get_pos_from_matrix(D3DXMATRIX world)
{
	return D3DXVECTOR3(world._41, world._42, world._43);
}

string adatkiiro()
{
	string _str = "";

	for(size_t i = 0; i < csillagok_vec.size(); i++)
	{
		if (csillagok_vec[i].get_pick())
		{
			string _nev = csillagok_vec[i].get_nev();
			string _tipus = csillagok_vec[i].get_tipus();
			_str = "Név: " + _nev + "\n" + "Típus: " + _tipus;
			break;
		}
	}

	for (size_t i = 0; i < bolygok_vec.size(); i++)
	{
		if (bolygok_vec[i].get_pick())
		{
			string _nev = bolygok_vec[i].get_nev();
			string _tipus = bolygok_vec[i].get_tipus();
			string _lakhato = "";
			if (bolygok_vec[i].get_lakhato()) { _lakhato = "Igen"; } else { _lakhato = "Nem"; }
			string _nepesseg = to_string(bolygok_vec[i].get_nepesseg());
			string _birodalom = bolygok_vec[i].get_birodalom();
			_str = "Név: " + _nev + "\n" + "Típus: " + _tipus + "\n" + "Lakható: " + _lakhato + "\n" + "Népesség: " + _nepesseg + "\n" + "Birodalom: " + _birodalom;
			break;
		}
	}

	for (size_t i = 0; i < holdak_vec.size(); i++)
	{
		if (holdak_vec[i].get_pick())
		{
			string _nev = holdak_vec[i].get_nev();
			string _tipus = holdak_vec[i].get_tipus();
			string _lakhato = "";
			if (holdak_vec[i].get_lakhato()) { _lakhato = "Igen"; }
			else { _lakhato = "Nem"; }
			string _nepesseg = to_string(holdak_vec[i].get_nepesseg());
			string _birodalom = holdak_vec[i].get_birodalom();
			_str = "Név: " + _nev + "\n" + "Típus: " + _tipus + "\n" + "Lakható: " + _lakhato + "\n" + "Népesség: " + _nepesseg + "\n" + "Birodalom: " + _birodalom;
			break;
		}
	}

	for (size_t i = 0; i < hajok_vec.size(); i++)
	{
		if (hajok_vec[i].get_pick())
		{
			string _nev = hajok_vec[i].get_nev();
			string _tipus = hajok_vec[i].get_tipus();
			string _hp = to_string(hajok_vec[i].get_hp());
			string _energia = to_string(hajok_vec[i].get_energia());
			string _legenyseg = to_string(hajok_vec[i].get_legenyseg());
			string _pajzs = "";
			if (hajok_vec[i].get_pajzs()) { _pajzs = "Van"; } else { _pajzs = "Nincs"; }
			string _birodalom = hajok_vec[i].get_birodalom();
			_str = "Név: " + _nev + "\n" + "Típus: " + _tipus + "\n" + "HP: " + _hp + "\n" + "Energia: " + _energia + "\n" + "Legénység: " + _legenyseg + "\n" + "Pajzs: " + _pajzs + "\n" + "Birodalom: " + _birodalom;
			break;
		}
	}

	return _str;
}

bool pick(ID3DXMesh *mesh, D3DXMATRIX &mesh_world)
{
	// Calculate the picking ray
	Ray ray = CalcPickingRay(d3ddev, clickxPos_lmb, clickyPos_lmb);

	// transform the ray from view space to world space
	// get view matrix
	D3DXMATRIX view;
	d3ddev->GetTransform(D3DTS_VIEW, &view);

	// inverse it
	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);

	// apply on the ray
	TransformRay(&ray, &viewInverse);

	// átváltás a worldspace-re, a mesh a worldspace-ben van
	/*D3DXMATRIX world;
	d3ddev->GetTransform(D3DTS_WORLD, &world);*/

	D3DXMATRIX worldInverse;
	D3DXMatrixInverse(&worldInverse, 0, /*&world*/&mesh_world);

	TransformRay(&ray, &worldInverse);

	// collision detection
	BOOL _hit = FALSE;

	D3DXIntersect(mesh, &ray._origin, &ray._direction, &_hit, NULL, NULL, NULL, NULL, NULL, NULL);
	
	if (_hit)
	{ return TRUE; } else { return FALSE; }
}

void send()
{
	// Calculate the picking ray
	Ray ray = CalcPickingRay(d3ddev, clickxPos_rmb, clickyPos_rmb);

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

	//update UWV
	update_cel();

	for (size_t i = 0; i < hajok_vec.size(); i++)
	{
		if (hajok_vec[i].get_pick())
		{
			D3DXVECTOR3 _pos = hajok_vec[i].get_pos();

			hajok_vec[i].set_start(_pos);
			hajok_vec[i].set_cel(cel_UWV);

			float _irany = hajok_vec[i].get_irany_y();
			float _uj_irany = hajok_vec[i].iranyszog_y(_irany, _pos, cel_UWV);

			hajok_vec[i].set_irany_y(_uj_irany);

			break;
		}
	}

	return;
}

void init_shader(void)
{
	SVertex* v_ptr = NULL;

	SVertex triangleList[] =
	{
		D3DXVECTOR3(-5.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 5.0f, 0.0f),
		D3DXVECTOR3(5.0f, 0.0f, 0.0f)
	};

	//VB készítése shaderhez
	d3ddev->CreateVertexBuffer(3 * sizeof(SVertex), 0, 0, D3DPOOL_DEFAULT, &shader_vpuffer_ptr, NULL);
	
	shader_vpuffer_ptr->Lock(0, sizeof(triangleList), (VOID**)&v_ptr, 0);
	memcpy(v_ptr, triangleList, sizeof(triangleList));
	shader_vpuffer_ptr->Unlock();

	return;
}

void tav_fuggo_pick(void)
{
	D3DXVECTOR3 _cam_pos;
	TheCamera.getPosition(&_cam_pos);

	D3DXMATRIX _tr;

	int _cs_szam = -1; // legközelebbi csillag száma
	float _cs_tav = 1000.0f; // legközelebbi csillag távolsága

	int _b_szam = -1;
	float _b_tav = 1000.0f;

	int _h_szam = -1;
	float _h_tav = 1000.0f;

	int _hajo_szam = -1;
	float _hajo_tav = 1000.0f;

	for (size_t i = 0; i < csillagok_vec.size(); i++)
	{
		csillagok_vec[i].set_pick(FALSE);
		_tr = csillagok_vec[i].get_transform();
		if (pick(csillagok_vec[i].get_mesh(), _tr))
		{
			float _tav = tavmeres(_cam_pos, csillagok_vec[i].get_pos());
			if (_tav < _cs_tav) { _cs_tav = _tav; _cs_szam = i; }
		}
	}

	for (size_t i = 0; i < bolygok_vec.size(); i++)
	{
		bolygok_vec[i].set_pick(FALSE);
		_tr = bolygok_vec[i].get_transform();
		if (pick(bolygok_vec[i].get_mesh(), _tr))
		{
			float _tav = tavmeres(_cam_pos, bolygok_vec[i].get_pos());
			if (_tav < _b_tav) { _b_tav = _tav; _b_szam = i; }
		}
	}

	for (size_t i = 0; i < holdak_vec.size(); i++)
	{
		holdak_vec[i].set_pick(FALSE);
		_tr = holdak_vec[i].get_transform();
		if (pick(holdak_vec[i].get_mesh(), _tr))
		{
			float _tav = tavmeres(_cam_pos, holdak_vec[i].get_pos());
			if (_tav < _h_tav) { _h_tav = _tav; _h_szam = i; }
		}
	}

	for (size_t i = 0; i < hajok_vec.size(); i++)
	{
		hajok_vec[i].set_pick(FALSE);
		_tr = hajok_vec[i].get_transform();
		if (pick(hajok_vec[i].get_mesh(), _tr))
		{
			float _tav = tavmeres(_cam_pos, hajok_vec[i].get_pos());
			if (_tav < _hajo_tav) { _hajo_tav = _tav; _hajo_szam = i; }
		}
	}

	int _obj_tomb[4] = { _cs_szam, _b_szam, _h_szam, _hajo_szam };
	float _tav_tomb[4] = { _cs_tav, _b_tav, _h_tav, _hajo_tav };

	// legkissebb távolság elemének száma a tömbben
	int _closest = distance(_tav_tomb, min_element(_tav_tomb, _tav_tomb + 4));

	if (_closest == 0 && _cs_szam > -1) { csillagok_vec[_cs_szam].set_pick(TRUE); }
	if (_closest == 1 && _b_szam > -1) { bolygok_vec[_b_szam].set_pick(TRUE); }
	if (_closest == 2 && _h_szam > -1) { holdak_vec[_h_szam].set_pick(TRUE); }
	if (_closest == 3 && _hajo_szam > -1) { hajok_vec[_hajo_szam].set_pick(TRUE); }

	return;
}

void random_csillagok(void)
{
	srand((unsigned int)time(NULL));

	// kis csillagokhoz
	int i = 0;
	while (i < rnd_meret)
	{
		rnd[i].x = rand() % 10000 - 5000;
		rnd[i].y = rand() % 10000 - 5000;
		rnd[i].z = rand() % 10000 - 5000;
		// gömb magnitúdón kívül levõ pontok
		if (sqrt((rnd[i].x * rnd[i].x) + (rnd[i].y * rnd[i].y) + (rnd[i].z * rnd[i].z)) >= 4000) { i++; }
	}

	// nagyobb csillagokhoz
	int j = 0;
	while (j < rnd_sprite_meret)
	{
		rnd_sprite[j].x = rand() % 10000 - 5000;
		rnd_sprite[j].y = rand() % 10000 - 5000;
		rnd_sprite[j].z = rand() % 10000 - 5000;

		if (sqrt((rnd_sprite[j].x * rnd_sprite[j].x) + (rnd_sprite[j].y * rnd_sprite[j].y) + (rnd_sprite[j].z * rnd_sprite[j].z)) >= 6000) { j++; }
	}

	return;
}

void uj_hajo(void)
{
	int _sz = rand() % 2000 + 1;
	float _xx = rand() % 50 + 1024;
	float _yy = rand() % 50 + 620;
	float _zz = rand() % 50 + 620;
	string _n = "NS_" + to_string(_sz);
	Hajok hajo = { _n, "Ûrhajó", 2000, 500, 15.0f, 250, FALSE, "Ember", 0, 0.0f, mtrls_mesh_0, textures_mesh_0, meshes[0], D3DXVECTOR3(_xx, _yy, _zz), D3DXVECTOR3(_xx, _yy, _zz) };
	hajok_vec.push_back(hajo);

	return;
}

void kijelolo(void)
{
	vonal_1[0] = D3DXVECTOR2(clickxPos_lmb, moveYPos); // balos
	vonal_1[1] = D3DXVECTOR2(clickxPos_lmb, clickyPos_lmb);

	vonal_2[0] = D3DXVECTOR2(moveXPos, moveYPos); // jobbos
	vonal_2[1] = D3DXVECTOR2(moveXPos, clickyPos_lmb);

	vonal_3[0] = D3DXVECTOR2(clickxPos_lmb, clickyPos_lmb); // felsõ
	vonal_3[1] = D3DXVECTOR2(moveXPos, clickyPos_lmb);

	vonal_4[0] = D3DXVECTOR2(clickxPos_lmb, moveYPos); // alsó
	vonal_4[1] = D3DXVECTOR2(moveXPos, moveYPos);

	if (kijelolo_negyzet)
	{
		//x_coords.clear();
		//y_coords.clear();

		//// X koordináták 5 pixelenként
		//if (clickxPos_lmb < moveXPos)
		//{
		//	for (int i = clickxPos_lmb; i < moveXPos; i = i + 5)
		//	{
		//		x_coords.push_back(i);
		//	}
		//}

		//if (clickxPos_lmb > moveXPos)
		//{
		//	for (int i = moveXPos; i < clickxPos_lmb; i = i + 5)
		//	{
		//		x_coords.push_back(i);
		//	}
		//}

		//// Y koordináták 5 pixelenként
		//if (clickyPos_lmb < moveYPos)
		//{
		//	for (int i = clickyPos_lmb; i < moveYPos; i = i + 5)
		//	{
		//		y_coords.push_back(i);
		//	}
		//}

		//if (clickyPos_lmb > moveYPos)
		//{
		//	for (int i = moveYPos; i < clickyPos_lmb; i = i + 5)
		//	{
		//		y_coords.push_back(i);
		//	}
		//}

		//// rays minden hajóra minden 5. pontból
		//Ray _ray;
		//D3DXMATRIX _view;
		//d3ddev->GetTransform(D3DTS_VIEW, &_view);
		//D3DXMATRIX _viewInverse;
		//D3DXMatrixInverse(&_viewInverse, 0, &_view);
		//D3DXMATRIX _worldInverse;

		//for (size_t i = 0; i < hajok_vec.size(); i++)
		//{
		//	for (size_t j = 0; j < x_coords.size(); j++)
		//	{
		//		for (size_t k = 0; k < y_coords.size(); k++)
		//		{
		//			_ray = CalcPickingRay(d3ddev, j, k);
		//			TransformRay(&_ray, &_viewInverse);
		//			D3DXMatrixInverse(&_worldInverse, 0, &hajok_vec[i].get_transform());
		//			TransformRay(&_ray, &_worldInverse);

		//			BOOL _hit = FALSE;
		//			D3DXIntersect(hajok_vec[i].get_mesh(), &_ray._origin, &_ray._direction, &_hit, NULL, NULL, NULL, NULL, NULL, NULL);

		//			if (_hit)
		//			{
		//				hajok_vec[i].set_pick(TRUE);
		//			} else {
		//				//hajok_vec[i].set_pick(FALSE);
		//			}
		//		}
		//	}
		//}
	}

	return;
}

void utkozesvizsgalat_loop(void)
{
	float _tav = 0.0f;
	bool _utkozes = FALSE;

	while (!game_over)
	{
		if (hajok_vec.size() > 0)
		{
			for (size_t i = 0; i < hajok_vec.size(); i++)
			{
				hajok_vec[i].bbox_forgat();

				for (size_t j = 0; j < hajok_vec.size(); j++)
				{
					_tav = tavmeres(hajok_vec[i].get_pos(), hajok_vec[j].get_pos());
					if (_tav > 0.0f && _tav < 16.0f)
					{
						hajok_vec[j].bbox_forgat();
						_utkozes = TheCollision.utkozes_box_box(hajok_vec[i].get_forgatott_bbox()._min, hajok_vec[i].get_forgatott_bbox()._max, hajok_vec[i].get_pos(), hajok_vec[j].get_forgatott_bbox()._min, hajok_vec[j].get_forgatott_bbox()._max, hajok_vec[j].get_pos());
						if (_utkozes)
						{
							hajok_vec[i].set_hp(0);
						}
					}
				}
				for (size_t k = 0; k < csillagok_vec.size(); k++)
				{
					_tav = tavmeres(hajok_vec[i].get_pos(), csillagok_vec[k].get_pos());
					if (_tav > 0.0f && _tav < 80.0f)
					{
						_utkozes = TheCollision.utkozes_box_sphere(hajok_vec[i].get_forgatott_bbox()._min, hajok_vec[i].get_forgatott_bbox()._max, hajok_vec[i].get_pos(), csillagok_vec[k].get_bounding_sphere()._center, csillagok_vec[k].get_bounding_sphere()._radius, csillagok_vec[k].get_pos());
						if (_utkozes)
						{
							hajok_vec[i].set_hp(0);
						}
					}
				}
				for (size_t l = 0; l < bolygok_vec.size(); l++)
				{
					_tav = tavmeres(hajok_vec[i].get_pos(), bolygok_vec[l].get_pos());
					if (_tav > 0.0f && _tav < 40.0f)
					{
						_utkozes = TheCollision.utkozes_box_sphere(hajok_vec[i].get_forgatott_bbox()._min, hajok_vec[i].get_forgatott_bbox()._max, hajok_vec[i].get_pos(), bolygok_vec[l].get_bounding_sphere()._center, bolygok_vec[l].get_bounding_sphere()._radius, bolygok_vec[l].get_pos());
						if (_utkozes)
						{
							hajok_vec[i].set_hp(0);
						}
					}
				}
				for (size_t m = 0; m < holdak_vec.size(); m++)
				{
					_tav = tavmeres(hajok_vec[i].get_pos(), holdak_vec[m].get_pos());
					if (_tav > 0.0f && _tav < 20.0f)
					{
						_utkozes = TheCollision.utkozes_box_sphere(hajok_vec[i].get_forgatott_bbox()._min, hajok_vec[i].get_forgatott_bbox()._max, hajok_vec[i].get_pos(), holdak_vec[m].get_bounding_sphere()._center, holdak_vec[m].get_bounding_sphere()._radius, holdak_vec[m].get_pos());
						if (_utkozes)
						{
							hajok_vec[i].set_hp(0);
						}
					}
				}
			}

			int torol = -1;
			for (size_t i = 0; i < hajok_vec.size(); i++)
			{
				int _hp = hajok_vec[i].get_hp();
				if (_hp == 0) { torol = i; break; }
			}
			if (torol > -1) { hajok_vec.erase(hajok_vec.begin() + torol); }
		}

		Sleep(100);
	}

	return;
}

void animacio_ctrl(void)
{
	int _animaciok_szama = pAnimController->GetNumAnimationSets();
	
	int _max_tracks = pAnimController->GetMaxNumTracks();

	for (int i = 0; i < _max_tracks; i++)
	{
		pAnimController->SetTrackEnable(i, FALSE);
	}
	
	if (_animaciok_szama > 0) { pAnimController->GetAnimationSet(_animaciok_szama - 1, &pAnimSet); }

	pAnimController->SetTrackAnimationSet(0, pAnimSet);
	pAnimController->SetTrackEnable(0, TRUE);
	
	D3DXTRACK_DESC td;
	pAnimController->GetTrackDesc(0, &td);
	if (td.Position > pAnimSet->GetPeriod()) { pAnimController->ResetTime(); }

	return;
}

void futasi_ido_loop(void)
{
	double _start = time(&timer);

	while (!game_over)
	{
		futasido = time(&timer) - _start;
		Sleep(250);
	}

	return;
}

void DrawFrame(IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pd3dDevice, pMeshContainer, pFrame);

		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pd3dDevice, pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pd3dDevice, pFrame->pFrameFirstChild);
	}

	return;
}

void DrawMeshContainer(IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	Container* pMeshContainer = (Container*)pMeshContainerBase;
	Frame* pFrame = (Frame*)pFrameBase;
	UINT iMaterial;
	UINT NumBlend;
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;

	UINT iMatrixIndex;
	UINT iPaletteEntry;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps(&d3dCaps);

	// first check for skinning
	if (pMeshContainer->pSkinInfo != NULL)
	{
		AttribIdPrev = UNUSED32;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

		// Draw using default vtx processing of the device (typically HW)
		for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
		{
			NumBlend = 0;
			for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
			{
				if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
				{
					NumBlend = i;
				}
			}

			if (d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1)
			{
				// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
				for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
					if (iMatrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
						pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
						pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
					}
				}

				pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

				// lookup the material used for this subset of faces
				if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
				{
					pd3dDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
					pd3dDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
					AttribIdPrev = pBoneComb[iAttrib].AttribId;
				}

				// draw the subset now that the correct material and matrices are loaded
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			}
		}

		// If necessary, draw parts that HW could not handle using SW
		if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
		{
			AttribIdPrev = UNUSED32;
			pd3dDevice->SetSoftwareVertexProcessing(TRUE);
			for (iAttrib = pMeshContainer->iAttributeSW; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				NumBlend = 0;
				for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
				{
					if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
					{
						NumBlend = i;
					}
				}

				if (d3dCaps.MaxVertexBlendMatrices < NumBlend + 1)
				{
					// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
					for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
					{
						iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
						if (iMatrixIndex != UINT_MAX)
						{
							D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
							pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
						}
					}

					pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

					// lookup the material used for this subset of faces
					if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
					{
						pd3dDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
						pd3dDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
						AttribIdPrev = pBoneComb[iAttrib].AttribId;
					}

					// draw the subset now that the correct material and matrices are loaded
					pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
				}
			}
			pd3dDevice->SetSoftwareVertexProcessing(FALSE);
		}

		pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
	} else {
		pd3dDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			pd3dDevice->SetMaterial(&pMeshContainer->pMaterials[iMaterial].MatD3D);
			pd3dDevice->SetTexture(0, pMeshContainer->ppTextures[iMaterial]);
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
		}
	}

	return;
}

void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	Frame* pFrame = (Frame*)pFrameBase;

	if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	} else {
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}

	return;
}

// BUGOK

// save, load, settings hiányzik
// scroll és kamera forgatás FPS függõ
// nagy csillagból csak 20 renderelhetõ
// csoportos pick 5-10 pixelenkénti ray-el, adatkiíró felújítás
// box-box ütközés hibás
// útkeresés nincs
// mozgás vonal, cél mesh, forgatás nem kell a static cuccoknak
// kijelöléskor celUWV átállítás kell (csoportosnál átlag)
// camera pont üztközés nincs
// fény csillagokhoz rendelés
// D3DXVec3Subtract távmérésre