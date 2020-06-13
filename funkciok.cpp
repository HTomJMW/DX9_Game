#include "funkciok.h"

Funkciok::Funkciok()
{
	_torol = -1;
}

Funkciok::~Funkciok()
{

}

void Funkciok::random_csillagok(void)
{
	srand((unsigned int)time(NULL));

	// kis csillagokhoz
	int i = 0;
	while (i < 400/*rnd_meret*/)
	{
		rnd[i].x = rand() % 10000 - 5000;
		rnd[i].y = rand() % 10000 - 5000;
		rnd[i].z = rand() % 10000 - 5000;
		// gömb magnitúdón kívül levõ pontok
		if (sqrt((rnd[i].x * rnd[i].x) + (rnd[i].y * rnd[i].y) + (rnd[i].z * rnd[i].z)) >= 4000) { i++; }
	}

	// nagyobb csillagokhoz
	int j = 0;
	while (j < 20/*rnd_sprite_meret*/)
	{
		rnd_sprite[j].x = rand() % 10000 - 5000;
		rnd_sprite[j].y = rand() % 10000 - 5000;
		rnd_sprite[j].z = rand() % 10000 - 5000;

		if (sqrt((rnd_sprite[j].x * rnd_sprite[j].x) + (rnd_sprite[j].y * rnd_sprite[j].y) + (rnd_sprite[j].z * rnd_sprite[j].z)) >= 6000) { j++; }
	}

	return;
}

D3DXVECTOR3 Funkciok::get_pos_from_matrix(D3DXMATRIX world)
{
	return D3DXVECTOR3(world._41, world._42, world._43);
}

void Funkciok::uj_hajo(void)
{
	int _sz = rand() % 2000 + 1;
	float _xx = rand() % 50 + 1024;
	float _yy = rand() % 50 + 620;
	float _zz = rand() % 50 + 620;
	string _n = "NS_" + to_string(_sz);
	Hajok uj_hajo = { _n, "Ûrhajó", 2000, 500, 15.0f, 250, FALSE, "Ember", 0.0f, "Models/flagship_3.X", TheMesh.static_meshes[4], D3DXVECTOR3(_xx, _yy, _zz), D3DXVECTOR3(_xx, _yy, _zz) };
	hajok_vec.push_back(uj_hajo);

	return;
}

void Funkciok::hajo_torles()
{
	_torol = -1;
	for (size_t i = 0; i < hajok_vec.size(); i++)
	{
		hajok_vec[i].anim_pos_ell();
		if (hajok_vec[i].get_torol()) { _torol = i; break; }
	}
	if (_torol > -1)
	{
		hajok_vec[_torol].set_pick(FALSE);
		hajok_vec[_torol].cleanup();
		hajok_vec.erase(hajok_vec.begin() + _torol);
	}

	return;
}

string Funkciok::adatkiiro()
{
	string _str;

	for (size_t i = 0; i < csillagok_vec.size(); i++)
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
			string _lakhato;
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
			string _lakhato;
			if (holdak_vec[i].get_lakhato()) { _lakhato = "Igen"; } else { _lakhato = "Nem"; }
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
			string _pajzs;
			if (hajok_vec[i].get_pajzs()) { _pajzs = "Van"; } else { _pajzs = "Nincs"; }
			string _birodalom = hajok_vec[i].get_birodalom();
			_str = "Név: " + _nev + "\n" + "Típus: " + _tipus + "\n" + "HP: " + _hp + "\n" + "Energia: " + _energia + "\n" + "Legénység: " + _legenyseg + "\n" + "Pajzs: " + _pajzs + "\n" + "Birodalom: " + _birodalom;
			break;
		}
	}

	return _str;
}

int Funkciok::calc_fps()
{
	if (delta > 0)
	{
		return (1000 / delta);
	}
	else {
		return 1000;
	}
}

int Funkciok::delta_time()
{
	_kezdet = timeGetTime();
	delta = _kezdet - _vege;
	_vege = _kezdet;

	return delta; // 2 frame között eltelt idõ ms-ban
}

void Funkciok::update_cel(void)
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

string Funkciok::ora()
{
	int _ora = win_time.wHour;
	int _perc = win_time.wMinute;
	int _masodperc = win_time.wSecond;
	string _s_ora = "00";
	string _s_perc = "00";
	string _s_masodperc = "00";

	if (_ora < 10) { _s_ora = "0" + to_string(_ora); }
	else { _s_ora = to_string(_ora); }
	if (_perc < 10) { _s_perc = "0" + to_string(_perc); }
	else { _s_perc = to_string(_perc); }
	if (_masodperc < 10) { _s_masodperc = "0" + to_string(_masodperc); }
	else { _s_masodperc = to_string(_masodperc); }

	return (_s_ora + ":" + _s_perc + ":" + _s_masodperc);
}

void Funkciok::kering(void)
{
	ksz = ksz + 0.0002f * delta;
	if (ksz >= 360.0f) { ksz = 0.0f; }

	return;
}

float Funkciok::tavmeres(D3DXVECTOR3 start, D3DXVECTOR3 finish)
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

void Funkciok::datum_ingame(void)
{
	string _honap;
	string _nap;

	while (!game_over)
	{
		if (!game_pause && !game_start)
		{
			if (nap > 30) { nap = 1; honap++; }
			if (nap < 10) { _nap = "0" + to_string(nap); }
			else { _nap = to_string(nap); }
			if (honap > 12) { honap = 1; ev++; }
			if (honap < 10) { _honap = "0" + to_string(honap); }
			else { _honap = to_string(honap); }

			datum = to_string(ev) + ". " + _honap + ". " + _nap + ".";

			Sleep(1000);
			nap++;
		}
		else {
			Sleep(1000);
		}
	}

	return;
}