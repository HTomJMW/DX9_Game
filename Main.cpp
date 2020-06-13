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
#include "Main.h"
#include "d3d.h"
#include "hangok.h"
#include "camera.h"
#include "menu.h"
#include "struct.h"
#include "mesh.h"
#include "pick.h"
#include "funkciok.h"
#include "csillagok.h"
#include "bolygok.h"
#include "holdak.h"
#include "hajok.h"
#include "utkozesvizsgalat.h"
#include "hierarchy.h"
#include "diag.h"

// névterek
using namespace std;

// függvénykönyvtárak linkelése
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "Strmiids.lib")

// global deklarációk
// d3d
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;

// vbuffer
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;

// cursor
D3DXIMAGE_INFO cursor_info; // képfájl infók
IDirect3DSurface9 *cursor = NULL;

// idõ
SYSTEMTIME win_time;

// objektumok
D3D TheD3D;
Hangok TheSound;
Camera TheCamera(Camera::LANDOBJECT);
Menu TheMenu;
Mesh TheMesh;
Pick ThePick;
Utkozesvizsgalat TheCollision;
Funkciok TheFunctions;
Hierarchy TheAllocHierarchy;
Diag TheDiag;

// obj vectorok
vector<Csillagok> csillagok_vec;
vector<Bolygok> bolygok_vec;
vector<Holdak> holdak_vec;
vector<Hajok> hajok_vec;

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

// prototípusok
void initD3D(HWND hWnd);
void render_frame(void);
void cleanD3D(void);
void init_graphics(void);
void init_shader(void);
void init_light(void);
void init_materials(void);
void kijelolo(void);

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

	// init objecktumok
	Csillagok csillag_0 = { "GAX-A001", "Fõsorozat", TheMesh.mtrls_mesh_0, TheMesh.textures_mesh_0, TheMesh.static_meshes[0], D3DXVECTOR3(640.0f, 640.0f, 640.0f), 1.5f };
	Bolygok bolygo_0 = { "G-01", "Kõzet", TRUE, 5000, "Ember", TheMesh.mtrls_mesh_1, TheMesh.textures_mesh_1, TheMesh.static_meshes[1], D3DXVECTOR3(300.0f, 0.0f, 0.0f), D3DXVECTOR3(640.0f, 640.0f, 640.0f), 0.3f };
	Bolygok bolygo_1 = { "G-02", "Kõzet", TRUE, 1000, "Ember", TheMesh.mtrls_mesh_2, TheMesh.textures_mesh_2, TheMesh.static_meshes[2], D3DXVECTOR3(500.0f, 0.0f, 0.0f), D3DXVECTOR3(640.0f, 640.0f, 640.0f), 0.2f };
	Hajok hajo_0 = { "Flagship", "Ûrhajó", 2000, 500, 15.0f, 250, FALSE, "Ember", 0.0f, "Models/flagship_3.X", TheMesh.static_meshes[4], D3DXVECTOR3(1024.0f, 640.0f, 640.0f), D3DXVECTOR3(1024.0f, 640.0f, 640.0f) };
	Hajok hajo_1 = { "Mh-1", "Mûhold", 500, 100, 0.0f, 0, FALSE, "Ember", 270.0f, "Models/muhold.X",  TheMesh.static_meshes[4], D3DXVECTOR3(640.0f, 720.0f, 500.0f), D3DXVECTOR3(640.0f, 720.0f, 500.0f) };

	// vectorhoz adás
	csillagok_vec.push_back(csillag_0);
	bolygok_vec.push_back(bolygo_0);
	bolygok_vec.push_back(bolygo_1);
	hajok_vec.push_back(hajo_0);
	hajok_vec.push_back(hajo_1);
	
	TheDiag.diag_mentes(); // diaglog készítés init után

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

		TheFunctions.delta_time();
		frames++;
		if (!game_start && !game_pause)
		{
			TheCamera.camera_move(cel_UWV);
			if (kijelolo_negyzet) { kijelolo(); }
			TheFunctions.update_cel();
			TheFunctions.kering();
		}
		TheFunctions.hajo_torles();
		render_frame();
		GetLocalTime(&win_time);
		if (frames % 30 == 0) { fps = TheFunctions.calc_fps(); ido = TheFunctions.ora(); frames = 0; }
	}

	TheDiag.diag_mentes(); // diaglog felülírása normál kilépéskor

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
				ThePick.tav_fuggo_pick();
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

			ThePick.send();
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
						TheSound.start();
					} break;
				case '2':
					{
						TheSound.stop();
					} break;
				case '3':
					{
						TheFunctions.uj_hajo();
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

	init_graphics(); // call the function to initialize the shapes
	init_materials();
	init_light(); // call the light
	init_shader();
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

	TheD3D.init_D3D();
	TheSound.init_sound();
	TheMesh.init_static_meshes(); // init static mesh tömb és azok mtrl, texture vektorjai

	HRESULT hr0 = D3DXCreateEffectFromFile(d3ddev, "shader.fx", 0, 0, 0, 0, &effect_ptr, NULL);
	tech_handle = effect_ptr->GetTechniqueByName("Tech");
	WVP_handle = effect_ptr->GetParameterByName(0, "WorldViewProjection");
	color_handle = effect_ptr->GetParameterByName(0, "MaterialColor");
	texture_handle = effect_ptr->GetParameterByName(0, "Texture_A");
	d3ddev->CreateVertexDeclaration(vertexElements, &decl_ptr);

	// cursor
	HRESULT hr1 = D3DXGetImageInfoFromFile("Pictures/cursor.png", &cursor_info);
	d3ddev->CreateOffscreenPlainSurface(cursor_info.Width, cursor_info.Height, cursor_info.Format, D3DPOOL_DEFAULT, &cursor, NULL);
	HRESULT hr2 = D3DXLoadSurfaceFromFile(cursor, NULL, NULL, "Pictures/cursor.png", NULL, D3DX_FILTER_NONE, feher, NULL);
	d3ddev->SetCursorProperties(0, 0, cursor);
	d3ddev->SetCursorPosition(screen_width / 2, screen_height / 3, D3DCURSOR_IMMEDIATE_UPDATE);

	// szálak
	thread szal_1(&Funkciok::datum_ingame, ref(TheFunctions)); // start dátum szál
	szal_1.detach();

	// ütkozésvizsgálat
	thread szal_3(&Utkozesvizsgalat::utkozesvizsgalat_loop, ref(TheCollision));
	szal_3.detach();

	TheFunctions.random_csillagok();
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
		effect_ptr->SetTexture(texture_handle, TheD3D.textures[0]);
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
		d3ddev->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

		// mesh-ek
		for (size_t i = 0; i < csillagok_vec.size(); i++)
		{
			csillagok_vec[i].render();
		}

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
			hajok_vec[i].UpdateAnimTime();
			hajok_vec[i].UpdateFrameMatrices();
			hajok_vec[i].render();
			if (hajok_vec[i].get_pick())
			{
				hajok_vec[i].mozgas_vonal();
				hajok_vec[i].mv_render(V, matProjection);
				hajok_vec[i].cel_render(TheMesh.static_meshes[3]);
			}
		}

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
		TheD3D.player_name->DrawTextA(NULL, ("Player: " + p_nev).c_str(), -1, &TheD3D.player_rect, DT_LEFT, feher);
		TheD3D.fps_counter->DrawTextA(NULL, (to_string(fps) + " FPS").c_str(), -1, &TheD3D.fps_rect, DT_LEFT, feher);
		TheD3D.esemenynaplo->DrawTextA(NULL, "", -1, &TheD3D.esemeny_rect, DT_CENTER, feher);
		TheD3D.pontos_ido->DrawTextA(NULL, ido.c_str(), -1, &TheD3D.pontos_ido_rect, DT_LEFT, feher);
		TheD3D.game_datum->DrawTextA(NULL, datum.c_str(), -1, &TheD3D.game_datum_rect, DT_LEFT, feher);
		TheD3D.adatok->DrawTextA(NULL, (TheFunctions.adatkiiro()).c_str(), -1, &TheD3D.adatok_rect, DT_LEFT, feher);

		// sprites
		TheD3D.sprite1->Begin(D3DXSPRITE_ALPHABLEND);
		for (size_t i = 0; i < csillagok_vec.size(); i++)
		{
			if (csillagok_vec[i].get_pick())
			{
				TheD3D.sprite1->Draw(TheD3D.textures[23], NULL, NULL, &TheD3D.position, feher);
				break;
			}
		}

		for (size_t i = 0; i < bolygok_vec.size(); i++)
		{
			if (bolygok_vec[i].get_pick())
			{
				TheD3D.sprite1->Draw(TheD3D.textures[23], NULL, NULL, &TheD3D.position, feher);
				break;
			}
		}

		for (size_t i = 0; i < holdak_vec.size(); i++)
		{
			if (holdak_vec[i].get_pick())
			{
				TheD3D.sprite1->Draw(TheD3D.textures[23], NULL, NULL, &TheD3D.position, feher);
				break;
			}
		}

		for (size_t i = 0; i < hajok_vec.size(); i++)
		{
			if (hajok_vec[i].get_pick())
			{
				TheD3D.sprite1->Draw(TheD3D.textures[5], NULL, NULL, &TheD3D.position, feher);
				break;
			}
		}
		TheD3D.sprite1->End();

		// flares
		D3DXMATRIX wrld;
		D3DXMATRIX vw;

		d3ddev->GetTransform(D3DTS_VIEW, &vw);
		D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);

		for (int i = 0; i < rnd_sprite_meret; i++)
		{
			D3DXMatrixTranslation(&matTranslate, rnd_sprite[i].x, rnd_sprite[i].y, rnd_sprite[i].z);
			d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matTranslate));

			d3ddev->GetTransform(D3DTS_WORLD, &wrld);
			TheD3D.flares[i]->SetWorldViewLH(&wrld, &vw);

			TheD3D.flares[i]->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE | D3DXSPRITE_BILLBOARD);
			TheD3D.flares[i]->Draw(TheD3D.textures[6], NULL, NULL, NULL, feher);
			TheD3D.flares[i]->End();
		}

		// kijelölõ
		if (kijelolo_negyzet)
		{
			for (int i = 0; i < 4; i++)
			{
				TheD3D.vonalak[i]->SetWidth(1.0f);
				TheD3D.vonalak[i]->SetAntialias(TRUE);
				TheD3D.vonalak[i]->Begin();
				if (i == 0) { TheD3D.vonalak[i]->Draw(TheD3D.vonal_1, 2, feher); }
				if (i == 1) { TheD3D.vonalak[i]->Draw(TheD3D.vonal_2, 2, feher); }
				if (i == 2) { TheD3D.vonalak[i]->Draw(TheD3D.vonal_3, 2, feher); }
				if (i == 3) { TheD3D.vonalak[i]->Draw(TheD3D.vonal_4, 2, feher); }
				TheD3D.vonalak[i]->End();
			}
		}

	} else {
		TheMenu.menu_render();// menü
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
	effect_ptr->Release();
	decl_ptr->Release();
	shader_vpuffer_ptr->Release();
	TheD3D.cleanup();
	TheSound.cleanup();
	TheMesh.cleanup();
	for (size_t i = 0; i < hajok_vec.size(); i++)
	{
		hajok_vec[i].cleanup();
	}
}

// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		//pont csillagokhoz
		{ 0.0f, 0.0f, 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f }, // 0
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

void kijelolo(void)
{
	TheD3D.vonal_1[0] = D3DXVECTOR2(clickxPos_lmb, moveYPos); // balos
	TheD3D.vonal_1[1] = D3DXVECTOR2(clickxPos_lmb, clickyPos_lmb);

	TheD3D.vonal_2[0] = D3DXVECTOR2(moveXPos, moveYPos); // jobbos
	TheD3D.vonal_2[1] = D3DXVECTOR2(moveXPos, clickyPos_lmb);

	TheD3D.vonal_3[0] = D3DXVECTOR2(clickxPos_lmb, clickyPos_lmb); // felsõ
	TheD3D.vonal_3[1] = D3DXVECTOR2(moveXPos, clickyPos_lmb);

	TheD3D.vonal_4[0] = D3DXVECTOR2(clickxPos_lmb, moveYPos); // alsó
	TheD3D.vonal_4[1] = D3DXVECTOR2(moveXPos, moveYPos);

	return;
}

// BUGOK

// save, load, settings hiányzik
// scroll és kamera forgatás FPS függõ
// nagy csillagból csak 20 renderelhetõ
// csoportos pick hiányzik
// csoportos pickhez, adatkiíró illesz
// útkeresés nincs
// kijelöléskor celUWV átállítás kell (csoportosnál átlag)
// kijelöléskor magasság koordináta render kell
// fény csillagokhoz rendelés
// D3DXVec3Subtract távmérésre?
// pont csillagok material?
// hajok kivalaszt box - diaglog createbox
// hierarchy - mesh container - diag
// externek eltávolítása
// kijelölve vonal rajzol célhoz
// bolygó pálya rajzol