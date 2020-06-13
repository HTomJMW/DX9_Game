#pragma once

#include <vector>
#include <d3dx9.h>
#include <algorithm>
#include "csillagok.h"
#include "bolygok.h"
#include "holdak.h"
#include "hajok.h"
#include "funkciok.h"
#include "camera.h"

extern bool game_over;
extern vector<Csillagok> csillagok_vec;
extern vector<Bolygok> bolygok_vec;
extern vector<Holdak> holdak_vec;
extern vector<Hajok> hajok_vec;
extern Funkciok TheFunctions;
extern Camera TheCamera;

class Utkozesvizsgalat
{
	public:
		Utkozesvizsgalat();
		~Utkozesvizsgalat();

		bool utkozes_sphere_point(D3DXVECTOR3 &p, D3DXVECTOR3, float, D3DXVECTOR3);
		bool utkozes_sphere_sphere(D3DXVECTOR3, float, D3DXVECTOR3, D3DXVECTOR3, float, D3DXVECTOR3);

		void utkozesvizsgalat_loop(void);
};