#pragma once

#include <d3dx9.h>
#include <windows.h>
#include <string>
#include "struct.h"

using namespace std;

extern LPDIRECT3DDEVICE9 d3ddev;
extern float ksz;

class Csillagok
{
	public:
		Csillagok(string _nev, string _tipus, int _bounding, IDirect3DTexture9 *_texture, LPD3DXMESH _mesh, D3DXVECTOR3 _pos, float _r);
		~Csillagok();

		string get_nev() const;
		void set_nev(string _nev);
		string get_tipus() const;
		void set_tipus(string _tipus);
		int get_bounding() const;
		LPD3DXMESH get_mesh() const;
		D3DXVECTOR3 get_pos() const;
		D3DXMATRIX get_transform() const;
		bool get_pick() const;
		void set_pick(bool _kivalaszt);

		Bsphere calc_bounding_sphere(ID3DXMesh*);
		Bsphere get_bounding_sphere() const;

		void render(void);

	private:
		string _nev;
		string _tipus;
		int _bounding; // bounding box: 0, bounding sphere: 1
		bool _kivalaszt;

		D3DXMATRIX _matRotateY;
		D3DXMATRIX _matScale;
		D3DXMATRIX _matTranslate;
		D3DXMATRIX _transform;

		D3DXVECTOR3 _pos;
		float _r; // forgási együttható

		IDirect3DTexture9 *_texture;
		LPD3DXMESH _mesh;

		Bsphere _bounding_sphere;
};