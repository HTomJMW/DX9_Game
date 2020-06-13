#include "menu.h"

using namespace std;

Menu::Menu()
{
	cursor_start = FALSE;
	cursor_conti = FALSE;
	cursor_save = FALSE;
	cursor_load = FALSE;
	cursor_set = FALSE;
	cursor_exit = FALSE;
}

Menu::~Menu()
{

}

void Menu::menu_fgv(int screen_width, int screen_height)
{
	if (clickxPos_lmb > screen_width / 2 - 100 && clickxPos_lmb < screen_width / 2 + 100 && clickyPos_lmb > 220 && clickyPos_lmb < 300)
	{
		game_pause = FALSE;
		game_start = FALSE;
	}

	if (!game_start) {
		if (clickxPos_lmb > screen_width / 2 - 100 && clickxPos_lmb < screen_width / 2 + 100 && clickyPos_lmb > 320 && clickyPos_lmb < 400)
		{
			save();
			game_pause = FALSE;
			game_start = FALSE;
		}
	}

	if (clickxPos_lmb > screen_width / 2 - 100 && clickxPos_lmb < screen_width / 2 + 100 && clickyPos_lmb > 420 && clickyPos_lmb < 500)
	{
		load();
		game_pause = FALSE;
		game_start = FALSE;
	}

	if (clickxPos_lmb > screen_width / 2 - 100 && clickxPos_lmb < screen_width / 2 + 100 && clickyPos_lmb > 520 && clickyPos_lmb < 600)
	{
		// settings
	}

	if (clickxPos_lmb > screen_width / 2 - 100 && clickxPos_lmb < screen_width / 2 + 100 && clickyPos_lmb > 620 && clickyPos_lmb < 700)
	{
		game_over = TRUE; PostQuitMessage(0);
	}

	return;
}

void Menu::menu_loop(int screen_width, int screen_height)
{
	while ((game_pause || game_start) && !game_over)
	{
		if (moveXPos > screen_width / 2 - 100 && moveXPos < screen_width / 2 + 100)
		{
			if (game_start)
			{
				if (moveYPos > 220 && moveYPos < 300) { cursor_start = TRUE; } else { cursor_start = FALSE; }
			} else {
				if (moveYPos > 220 && moveYPos < 300) { cursor_conti = TRUE; } else { cursor_conti = FALSE; }
				if (moveYPos > 320 && moveYPos < 400) { cursor_save = TRUE; } else { cursor_save = FALSE; }
			}
			if (moveYPos > 420 && moveYPos < 500) { cursor_load = TRUE; } else { cursor_load = FALSE; }
			if (moveYPos > 520 && moveYPos < 600) { cursor_set = TRUE; } else { cursor_set = FALSE; }
			if (moveYPos > 620 && moveYPos < 700) { cursor_exit = TRUE; } else { cursor_exit = FALSE; }
		}
		else {
			cursor_start = FALSE;
			cursor_conti = FALSE;
			cursor_save = FALSE;
			cursor_load = FALSE;
			cursor_set = FALSE;
			cursor_exit = FALSE;
		}
		Sleep(100);
	}

	return;
}

void Menu::save(void)
{
	ofstream savegame;
	savegame.open("save.sav");

	/*savegame << ---- << endl;*/

	savegame.close();

	return;
}

void Menu::load(void)
{
	string adat;
	ifstream loadgame;
	loadgame.open("save.sav");
	if (loadgame.is_open())
	{
		/*getline(loadgame, adat);
		---- = stof(adat);*/

		loadgame.close();
	}

	return;
}

void Menu::menu_render()
{
	TheD3D.menu_hatter_nagy->Begin(D3DXSPRITE_ALPHABLEND);
	TheD3D.menu_hatter_nagy->Draw(TheD3D.textures[24], &TheD3D.menu_hatter_nagy_rect, NULL, &TheD3D.menu_hatter_nagy_pos, feher);
	TheD3D.menu_hatter_nagy->End();

	TheD3D.menu_hatter->Begin(D3DXSPRITE_ALPHABLEND);
	TheD3D.menu_hatter->Draw(TheD3D.textures[7], &TheD3D.menu_hatter_rect, NULL, &TheD3D.menu_hatter_pos, feher);
	TheD3D.menu_hatter->End();

	if (game_start)
	{
		TheD3D.gomb_continue->Begin(D3DXSPRITE_ALPHABLEND);
		if (cursor_start) {
			TheD3D.gomb_continue->Draw(TheD3D.textures[21], &TheD3D.gomb_continue_rect, NULL, &TheD3D.gomb_continue_pos, feher);
		} else {
			TheD3D.gomb_continue->Draw(TheD3D.textures[20], &TheD3D.gomb_continue_rect, NULL, &TheD3D.gomb_continue_pos, feher);
		}
		TheD3D.gomb_continue->End();

		TheD3D.gomb_save->Begin(D3DXSPRITE_ALPHABLEND);
		if (cursor_save || !cursor_save)
		{
			TheD3D.gomb_save->Draw(TheD3D.textures[22], &TheD3D.gomb_save_rect, NULL, &TheD3D.gomb_save_pos, feher);
		}
		TheD3D.gomb_save->End();
	} else {
		TheD3D.gomb_continue->Begin(D3DXSPRITE_ALPHABLEND);
		if (cursor_conti) {
			TheD3D.gomb_continue->Draw(TheD3D.textures[13], &TheD3D.gomb_continue_rect, NULL, &TheD3D.gomb_continue_pos, feher);
		} else {
			TheD3D.gomb_continue->Draw(TheD3D.textures[8], &TheD3D.gomb_continue_rect, NULL, &TheD3D.gomb_continue_pos, feher);
		}
		TheD3D.gomb_continue->End();

		TheD3D.gomb_save->Begin(D3DXSPRITE_ALPHABLEND);
		if (cursor_save)
		{
			TheD3D.gomb_save->Draw(TheD3D.textures[14], &TheD3D.gomb_save_rect, NULL, &TheD3D.gomb_save_pos, feher);
		} else {
			TheD3D.gomb_save->Draw(TheD3D.textures[9], &TheD3D.gomb_save_rect, NULL, &TheD3D.gomb_save_pos, feher);
		}
		TheD3D.gomb_save->End();
	}

	TheD3D.gomb_load->Begin(D3DXSPRITE_ALPHABLEND);
	if (cursor_load)
	{
		TheD3D.gomb_load->Draw(TheD3D.textures[15], &TheD3D.gomb_load_rect, NULL, &TheD3D.gomb_load_pos, feher);
	} else {
		TheD3D.gomb_load->Draw(TheD3D.textures[10], &TheD3D.gomb_load_rect, NULL, &TheD3D.gomb_load_pos, feher);
	}
	TheD3D.gomb_load->End();

	TheD3D.gomb_settings->Begin(D3DXSPRITE_ALPHABLEND);
	if (cursor_set)
	{
		TheD3D.gomb_settings->Draw(TheD3D.textures[16], &TheD3D.gomb_settings_rect, NULL, &TheD3D.gomb_settings_pos, feher);
	} else {
		TheD3D.gomb_settings->Draw(TheD3D.textures[11], &TheD3D.gomb_settings_rect, NULL, &TheD3D.gomb_settings_pos, feher);
	}
	TheD3D.gomb_settings->End();

	TheD3D.gomb_exit->Begin(D3DXSPRITE_ALPHABLEND);
	if (cursor_exit)
	{
		TheD3D.gomb_exit->Draw(TheD3D.textures[17], &TheD3D.gomb_exit_rect, NULL, &TheD3D.gomb_exit_pos, feher);
	} else {
		TheD3D.gomb_exit->Draw(TheD3D.textures[12], &TheD3D.gomb_exit_rect, NULL, &TheD3D.gomb_exit_pos, feher);
	}
	TheD3D.gomb_exit->End();

	return;
}