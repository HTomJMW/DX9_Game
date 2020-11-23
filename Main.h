#pragma once

#include <d3dx9.h>
#include <string>

using namespace std;

// definíciók
#define WS_WINDOW (WS_POPUP)
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define PI 3.14159265
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p) = NULL; } }
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }

// globális változók
bool game_start = TRUE;
bool game_over = FALSE;
bool game_pause = FALSE;
bool game_settings = FALSE;
bool kijelolo_negyzet = FALSE;

//képernyõ felbontás és ablak pozíció
int screen_width = 1920;
int screen_height = 1080;
int window_pos_left = 0;
int window_pos_top = 0;

// szinek
D3DXCOLOR fekete = D3DCOLOR_XRGB(0, 0, 0);
D3DXCOLOR feher = D3DCOLOR_XRGB(255, 255, 255);
D3DXCOLOR sarga = D3DCOLOR_XRGB(255, 255, 0);
D3DXCOLOR szurke = D3DCOLOR_XRGB(128, 128, 128);

// materials
D3DMATERIAL9 material;
D3DMATERIAL9 material2;

// lights
D3DLIGHT9 light;

// fps
int frames = 0;
int fps = 0;

// eltelt idõ - 1 frame
int delta = -1;

// világ mérete
float map_width = 1280.0f;
float map_height = 1280.0f;
float map_depth = 1280.0f;

// start cam pos visszaugráshoz
D3DXVECTOR3 cam_pos = D3DXVECTOR3(640.0f, 800.0f, 128.0f);

// egér, kattintás pozíció
int moveXPos = screen_width / 2;
int moveYPos = screen_height / 2;
int clickxPos_lmb = screen_width / 2;
int clickyPos_lmb = screen_height / 2;
int clickxPos_rmb = screen_width / 2;
int clickyPos_rmb = screen_height / 2;
int scroll = 0;
float eger_fordulas = FALSE;
int w_click_X = 0;
int w_click_Y = 0;
int last_Xpos = 0;
int last_Ypos = 0;

// pick
float pU = 0.25f;
float pV = 0.25f;
D3DXVECTOR3 cel_UWV = D3DXVECTOR3(640.0f, 640.0f, 640.0f);

// send
D3DXVECTOR3 map_b_also = D3DXVECTOR3(0.0f, cel_UWV.y, 0.0f);
D3DXVECTOR3 map_b_felso = D3DXVECTOR3(0.0f, cel_UWV.y, 2560.0f);
D3DXVECTOR3 map_j_also = D3DXVECTOR3(2560.0f, cel_UWV.y, 0.0f);

// keringés, forgás
float ksz = 0.0f;

// ingame dátum
int ev = 2255;
int honap = 1;
int nap = 1;

// csillagok
float pointSize = 2.0f;

const int rnd_meret = 400;
D3DXVECTOR3 rnd[rnd_meret];

const int rnd_sprite_meret = 20;
D3DXVECTOR3 rnd_sprite[rnd_sprite_meret];

// sztringek
string author = "[H] Tom - 2020";
string p_nev = "Tom";
string p_birodalom = "Ember";
string ido = "00:00:00";
string datum = "2255. 01. 01.";
string penz = "1500 €";

int osszpenz = 1500;

// debug / settings
bool frissit_azonnal = TRUE; // 1000+ FPS
bool debug = TRUE;