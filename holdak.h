#pragma once

#include <d3dx9.h>
#include <windows.h>
#include <string>
#include <vector>
#include <tchar.h>
#include <strsafe.h>
#include "struct.h"

using namespace std;

extern LPDIRECT3DDEVICE9 d3ddev;
extern float ksz;

class Holdak
{
	public:
		Holdak(string _nev, string _tipus, bool _lakhato, int _nepesseg, string _birodalom, const vector<D3DMATERIAL9> &_mtrls_mesh, const vector<IDirect3DTexture9*> &_textures_mesh, const LPD3DXMESH &_mesh, D3DXVECTOR3 _pos_1, D3DXVECTOR3 _pos_2, D3DXVECTOR3 _pos_3, float _r);
		~Holdak();

		string get_nev() const;
		void set_nev(string _nev);
		string get_tipus() const;
		void set_tipus(string _tipus);
		bool get_lakhato() const;
		void set_lakhato(bool _lakhato);
		int get_nepesseg() const;
		void set_nepesseg(int _nepesseg);
		string get_birodalom() const;
		void set_birodalom(string _birodalom);
		LPD3DXMESH get_mesh() const;
		D3DXVECTOR3 get_pos() const;
		D3DXMATRIX get_transform() const;
		bool get_pick() const;
		void set_pick(bool _kivalaszt);

		Bsphere calc_bounding_sphere(ID3DXMesh*);
		Bsphere get_bounding_sphere() const;

		void render(void);

		void cleanup(void);

	private:
		string _nev;
		string _tipus;
		bool _lakhato;
		int _nepesseg;
		string _birodalom;
		bool _kivalaszt;

		D3DXMATRIX _matRotateY;
		D3DXMATRIX _matScale;
		D3DXMATRIX _matTranslate1;
		D3DXMATRIX _matTranslate2;
		D3DXMATRIX _matTranslate3;
		D3DXMATRIX _transform;

		D3DXVECTOR3 _pos_1;
		D3DXVECTOR3 _pos_2;
		D3DXVECTOR3 _pos_3;
		float _r; // forgási együttható

		vector<D3DMATERIAL9> _mtrls_mesh;
		vector<IDirect3DTexture9*> _textures_mesh;
		LPD3DXMESH _mesh;

		Bsphere _bounding_sphere;
};