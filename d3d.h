#pragma once

#include <d3dx9.h>
#include "diag.h"

extern LPDIRECT3DDEVICE9 d3ddev;
extern Diag TheDiag;
extern int screen_width;
extern int screen_height;

class D3D
{
	public:
		D3D();
		~D3D();

		ID3DXFont *player_name;
		RECT player_rect;
		ID3DXFont *fps_counter;
		RECT fps_rect;
		ID3DXFont *esemenynaplo;
		RECT esemeny_rect;
		ID3DXFont *pontos_ido;
		RECT pontos_ido_rect;
		ID3DXFont *game_datum;
		RECT game_datum_rect;
		ID3DXFont *adatok;
		RECT adatok_rect;
		ID3DXFont *penz;
		RECT penz_rect;

		IDirect3DTexture9 *textures[25];

		ID3DXSprite *sprite1;
		D3DXVECTOR3 position;

		ID3DXSprite *flares[20/*rnd_sprite_meret*/];

		ID3DXSprite *menu_hatter_nagy;
		D3DXVECTOR3 menu_hatter_nagy_pos;
		RECT menu_hatter_nagy_rect;

		ID3DXSprite *menu_hatter;
		D3DXVECTOR3 menu_hatter_pos;
		RECT menu_hatter_rect;

		ID3DXSprite *gomb_continue;
		D3DXVECTOR3 gomb_continue_pos;
		RECT gomb_continue_rect;

		ID3DXSprite *gomb_save;
		D3DXVECTOR3 gomb_save_pos;
		RECT gomb_save_rect;

		ID3DXSprite *gomb_load;
		D3DXVECTOR3 gomb_load_pos;
		RECT gomb_load_rect;

		ID3DXSprite *gomb_settings;
		D3DXVECTOR3 gomb_settings_pos;
		RECT gomb_settings_rect;

		ID3DXSprite *gomb_exit;
		D3DXVECTOR3 gomb_exit_pos;
		RECT gomb_exit_rect;

		LPD3DXLINE line;

		LPD3DXLINE vonalak[4]; // kijelolõ vonalak

		D3DXVECTOR2 vonal_1[2];
		D3DXVECTOR2 vonal_2[2];
		D3DXVECTOR2 vonal_3[2];
		D3DXVECTOR2 vonal_4[2];

		void init_D3D(void);

		void cleanup(void);
};