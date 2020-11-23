#pragma once

#include <d3dx9.h>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include "mesh.h"
#include "csillagok.h"
#include "bolygok.h"
#include "holdak.h"
#include "hajok.h"

using namespace std;

extern int delta;
extern D3DXVECTOR3 rnd[400/*rnd_meret*/];
extern D3DXVECTOR3 rnd_sprite[20/*rnd_sprite_meret*/];
extern Mesh TheMesh;
extern vector<Csillagok> csillagok_vec;
extern vector<Bolygok> bolygok_vec;
extern vector<Holdak> holdak_vec;
extern vector<Hajok> hajok_vec;
extern D3DXVECTOR3 map_b_also;
extern D3DXVECTOR3 map_b_felso;
extern D3DXVECTOR3 map_j_also;
extern D3DXVECTOR3 cel_UWV;
extern float map_width;
extern float map_depth;
extern float pU;
extern float pV;
extern SYSTEMTIME win_time;
extern float ksz;
extern bool game_over;
extern bool game_pause;
extern bool game_start;
extern int ev;
extern int honap;
extern int nap;
extern string datum;
extern string penz;
extern int osszpenz;

class Funkciok
{
	public:
		Funkciok();
		~Funkciok();

		void random_csillagok(void);
		D3DXVECTOR3 get_pos_from_matrix(D3DXMATRIX world);
		void uj_hajo(void);
		void hajo_torles(void);
		string adatkiiro();
		int calc_fps();
		int delta_time();
		void update_cel(void);
		string ora();
		void kering(void);
		float tavmeres(D3DXVECTOR3, D3DXVECTOR3);
		void datum_ingame(void);
		void ado(void);
		int nepesseg(void);

	private:
		int _torol;
		int _kezdet;
		int _vege;
		int _nepesseg;
};