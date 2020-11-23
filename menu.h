#pragma once

#include <d3dx9.h>
#include <windows.h>
#include <fstream>
#include "d3d.h"

extern int clickxPos_lmb;
extern int clickyPos_lmb;
extern int moveXPos;
extern int moveYPos;
extern bool game_start;
extern bool game_over;
extern bool game_pause;
extern bool game_settings;
extern D3D TheD3D;
extern D3DXCOLOR feher;

class Menu
{
	public:
		Menu();
		~Menu();

		bool cursor_start;
		bool cursor_conti;
		bool cursor_save;
		bool cursor_load;
		bool cursor_set;
		bool cursor_exit;

		void menu_fgv(int, int);
		void menu_loop(int, int);
		void save(void);
		void load(void);
		void settings_fgv(int, int);

		void menu_render(void);
};