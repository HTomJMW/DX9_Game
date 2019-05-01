#pragma once

// define the screen resolution, position
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_POS_LEFT 100
#define WINDOW_POS_TOP 100
#define WS_WINDOW (WS_POPUP)
#define PI 3.14159

// public variables
bool game_over = FALSE;
bool game_pause = FALSE;
bool stop_thread = FALSE;

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

// declaration of gametime
int evszam = 2860;
int honap = 1;
int nap = 1;

// global declaration of money
int penz = 7000;
int ai_penz = 7000;

// declaration of flotta informations
int hajok_szama = 1;
int legenyseg_szama = 40;
int nmy_hajok_szama = 1;
int nmy_legenyseg_szama = 40;

// global declaration of flotta1pos (central of map)
float flotta1xPos = (map_width / 2);
float flotta1yPos = (map_height / 2);
float nmy_flottaxPos = (map_width / 3);
float nmy_flottayPos = (map_height / 3);
bool flotta_kivalaszt = TRUE;
bool nmy_flotta_kivalaszt = FALSE;
bool rmb = FALSE;

// global declaration for FPS
int kezdet = 0;
int vege = 0;
int the_fps = 0;
int frames = 0;

// for system owners
bool cap_player_done = FALSE;
bool cap_ai_done = FALSE;
int cap_rendszer_player = -1;
int cap_rendszer_ai = -1;
int system_count = 0;

// declaration of random star position arrays
float starsX[30];
float starsY[30];

// for radar
float pointSize = 3.0f;

// for pick
float pU = 0;
float pV = 0;
bool csillag_kivalaszt = FALSE;
int csillag_index;
int planets[30];