#include "d3d.h"

D3D::D3D()
{
	player_name = NULL;
	fps_counter = NULL;
	esemenynaplo = NULL;
	pontos_ido = NULL;
	game_datum = NULL;
	adatok = NULL;

	for (int i = 0; i < (sizeof(textures) / sizeof(textures[0])); i++)
	{
		textures[i] = NULL;
	}

	sprite1 = NULL;
	position = D3DXVECTOR3(screen_width - 165.0f, 20.0f, 0.0f);

	for (int i = 0; i < (sizeof(flares) / sizeof(flares[0])); i++)
	{
		flares[i] = NULL;
	}

	menu_hatter_nagy = NULL;
	menu_hatter_nagy_pos = D3DXVECTOR3(screen_width / 2 - 300, 100.0f, 0.0f);
	menu_hatter = NULL;
	menu_hatter_pos = D3DXVECTOR3(screen_width / 2 - 200, 200.0f, 0.0f);
	gomb_continue = NULL;
	gomb_continue_pos = D3DXVECTOR3(screen_width / 2 - 128, 200.0f, 0.0f);
	gomb_save = NULL;
	gomb_save_pos = D3DXVECTOR3(screen_width / 2 - 128, 300.0f, 0.0f);
	gomb_load = NULL;
	gomb_load_pos = D3DXVECTOR3(screen_width / 2 - 128, 400.0f, 0.0f);
	gomb_settings = NULL;
	gomb_settings_pos = D3DXVECTOR3(screen_width / 2 - 128, 500.0f, 0.0f);
	gomb_exit = NULL;
	gomb_exit_pos = D3DXVECTOR3(screen_width / 2 - 128, 600.0f, 0.0f);

	line = NULL;

	for (int i = 0; i < (sizeof(vonalak) / sizeof(vonalak[0])); i++)
	{
		vonalak[i] = NULL;
	}
}

D3D::~D3D()
{

}

void D3D::init_D3D(void)
{
	HRESULT _hr_szoveg[6];

	_hr_szoveg[0] = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &player_name);
	SetRect(&player_rect, 20, 20, 400, 60);
	_hr_szoveg[1] = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &fps_counter);
	SetRect(&fps_rect, 20, 60, 200, 120);
	_hr_szoveg[2] = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &esemenynaplo);
	SetRect(&esemeny_rect, (screen_width / 2) - 200, 60, (screen_width / 2) + 200, 120);
	_hr_szoveg[3] = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &pontos_ido);
	SetRect(&pontos_ido_rect, 20, 100, 200, 160);
	_hr_szoveg[4] = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &game_datum);
	SetRect(&game_datum_rect, 20, 140, 200, 200);
	_hr_szoveg[5] = D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "Times New Roman", &adatok);
	SetRect(&adatok_rect, screen_width - 220, 180, screen_width - 20, 480);

	for (int i = 0; i < (sizeof(_hr_szoveg) / sizeof(_hr_szoveg[0])); i++)
	{
		TheDiag.d3d_szoveg_diaglista.push_back(_hr_szoveg[i]);
	}

	HRESULT _hr_tex[25];

	_hr_tex[0] = D3DXCreateTextureFromFile(d3ddev, "Pictures/csillag.bmp", &textures[0]); // szabad
	_hr_tex[1] = D3DXCreateTextureFromFile(d3ddev, "Pictures/player.bmp", &textures[1]); // szabad
	_hr_tex[2] = D3DXCreateTextureFromFile(d3ddev, "Pictures/player_2.bmp", &textures[2]); // szabad
	_hr_tex[3] = D3DXCreateTextureFromFile(d3ddev, "Pictures/bolygo.png", &textures[3]); // szabad
	_hr_tex[4] = D3DXCreateTextureFromFile(d3ddev, "Pictures/hold.png", &textures[4]); // szabad
	_hr_tex[5] = D3DXCreateTextureFromFile(d3ddev, "Pictures/flotta.png", &textures[5]);
	_hr_tex[6] = D3DXCreateTextureFromFile(d3ddev, "Pictures/flare.png", &textures[6]);
	_hr_tex[7] = D3DXCreateTextureFromFile(d3ddev, "Pictures/menu_hatter.png", &textures[7]);
	_hr_tex[8] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_conti.png", &textures[8]);
	_hr_tex[9] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_save.png", &textures[9]);
	_hr_tex[10] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_load.png", &textures[10]);
	_hr_tex[11] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_set.png", &textures[11]);
	_hr_tex[12] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_exit.png", &textures[12]);
	_hr_tex[13] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_conti_a.png", &textures[13]);
	_hr_tex[14] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_save_a.png", &textures[14]);
	_hr_tex[15] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_load_a.png", &textures[15]);
	_hr_tex[16] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_set_a.png", &textures[16]);
	_hr_tex[17] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_exit_a.png", &textures[17]);
	_hr_tex[18] = D3DXCreateTextureFromFile(d3ddev, "Pictures/muhold.bmp", &textures[18]); // szabad
	_hr_tex[19] = D3DXCreateTextureFromFile(d3ddev, "Pictures/flare_1.png", &textures[19]);
	_hr_tex[20] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_start.png", &textures[20]);
	_hr_tex[21] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_start_a.png", &textures[21]);
	_hr_tex[22] = D3DXCreateTextureFromFile(d3ddev, "Pictures/gomb_save_b.png", &textures[22]);
	_hr_tex[23] = D3DXCreateTextureFromFile(d3ddev, "Pictures/egitest.png", &textures[23]);
	_hr_tex[24] = D3DXCreateTextureFromFile(d3ddev, "Pictures/menu_hatter_nagy.png", &textures[24]);

	for (int i = 0; i < (sizeof(_hr_tex) / sizeof(_hr_tex[0])); i++)
	{
		TheDiag.d3d_tex_diaglista.push_back(_hr_tex[i]);
	}

	HRESULT _hr_sprite[28];

	for (int i = 0; i < 20; i++)
	{
		_hr_sprite[i] = D3DXCreateSprite(d3ddev, &flares[i]);
	}

	_hr_sprite[20] = D3DXCreateSprite(d3ddev, &sprite1);

	_hr_sprite[21] = D3DXCreateSprite(d3ddev, &menu_hatter_nagy);
	SetRect(&menu_hatter_nagy_rect, 0, 0, 600, 750);
	_hr_sprite[22] = D3DXCreateSprite(d3ddev, &menu_hatter);
	SetRect(&menu_hatter_rect, 0, 0, 400, 550);
	_hr_sprite[23] = D3DXCreateSprite(d3ddev, &gomb_continue);
	SetRect(&gomb_continue_rect, 0, 0, 256, 100);
	_hr_sprite[24] = D3DXCreateSprite(d3ddev, &gomb_save);
	SetRect(&gomb_save_rect, 0, 0, 256, 100);
	_hr_sprite[25] = D3DXCreateSprite(d3ddev, &gomb_load);
	SetRect(&gomb_load_rect, 0, 0, 256, 100);
	_hr_sprite[26] = D3DXCreateSprite(d3ddev, &gomb_settings);
	SetRect(&gomb_settings_rect, 0, 0, 256, 100);
	_hr_sprite[27] = D3DXCreateSprite(d3ddev, &gomb_exit);
	SetRect(&gomb_exit_rect, 0, 0, 256, 100);

	for (int i = 0; i < (sizeof(_hr_sprite) / sizeof(_hr_sprite[0])); i++)
	{
		TheDiag.d3d_sprite_diaglista.push_back(_hr_sprite[i]);
	}

	HRESULT _hr_line[5];

	for (int i = 0; i < 4; i++)
	{
		_hr_line[i] = D3DXCreateLine(d3ddev, &vonalak[i]);
	}

	_hr_line[4] = D3DXCreateLine(d3ddev, &line);

	for (int i = 0; i < (sizeof(_hr_line) / sizeof(_hr_line[0])); i++)
	{
		TheDiag.line_diaglista.push_back(_hr_line[i]);
	}

	return;
}

void D3D::cleanup(void)
{
	player_name->Release();
	fps_counter->Release();
	esemenynaplo->Release();
	pontos_ido->Release();
	game_datum->Release();
	adatok->Release();

	sprite1->Release();
	menu_hatter_nagy->Release();
	menu_hatter->Release();
	gomb_continue->Release();
	gomb_save->Release();
	gomb_load->Release();
	gomb_settings->Release();
	gomb_exit->Release();

	for (int i = 0; i < (sizeof(textures) / sizeof(textures[0])); i++)
	{
		textures[i]->Release();
	}

	for (int i = 0; i < (sizeof(flares) / sizeof(flares[0])); i++)
	{
		flares[i]->Release();
	}

	for (int i = 0; i < (sizeof(vonalak) / sizeof(vonalak[0])); i++)
	{
		vonalak[i]->Release();
	}

	line->Release();

	return;
}