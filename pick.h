#pragma once

#include <d3dx9.h>
#include <vector>
#include <algorithm>
#include "struct.h"
#include "camera.h"
#include "csillagok.h"
#include "bolygok.h"
#include "holdak.h"
#include "hajok.h"
#include "funkciok.h"

using namespace std;

extern LPDIRECT3DDEVICE9 d3ddev;
extern int clickxPos_lmb;
extern int clickyPos_lmb;
extern int clickxPos_rmb;
extern int clickyPos_rmb;
extern vector<Csillagok> csillagok_vec;
extern vector<Bolygok> bolygok_vec;
extern vector<Holdak> holdak_vec;
extern vector<Hajok> hajok_vec;
extern D3DXVECTOR3 cel_UWV;
extern Camera TheCamera;
extern float pU;
extern float pV;
extern D3DXVECTOR3 map_b_also;
extern D3DXVECTOR3 map_b_felso;
extern D3DXVECTOR3 map_j_also;
extern Funkciok TheFunctions;

class Pick
{
	public:
		Pick();
		~Pick();

		Ray CalcPickingRay(LPDIRECT3DDEVICE9 Device, int, int);
		void TransformRay(Ray* ray, D3DXMATRIX* T);
		bool pick(ID3DXMesh*, D3DXMATRIX &);
		void tav_fuggo_pick(void);
		void send(void);
};