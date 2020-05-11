#pragma once

#include <d3dx9.h>
#include <windows.h>
#include <string>
#include <vector>
#include "struct.h"
#include "csillagok.h"
#include "bolygok.h"
#include "holdak.h"

using namespace std;

#define PI 3.14159265

extern LPDIRECT3DDEVICE9 d3ddev;
extern LPD3DXLINE line;
extern int delta;
extern float ksz;

extern float tavmeres(D3DXVECTOR3, D3DXVECTOR3);

class Hajok
{
	public:
		Hajok(string _nev, string _tipus, int _hp, int _energia, float _sebesseg, int _legenyseg, bool _pajzs, string _birodalom, int _bounding, float _irany_y, vector<D3DMATERIAL9> _materials, vector<IDirect3DTexture9*> _textures, LPD3DXMESH _mesh, D3DXVECTOR3 _pos, D3DXVECTOR3 _cel);
		~Hajok();

		string get_nev() const;
		void set_nev(string _nev);
		string get_tipus() const;
		void set_tipus(string _tipus);
		int get_hp() const;
		void set_hp(int _hp);
		int get_energia() const;
		void set_energia(int _energia);
		float get_sebesseg() const;
		void set_sebesseg(float _sebesseg);
		int get_legenyseg() const;
		void set_legenyseg(int _legenyseg);
		bool get_pajzs() const;
		void set_pajzs(bool _pajzs);
		string get_birodalom() const;
		void set_birodalom(string _birodalom);
		int get_bounding() const;
		float get_irany_y() const;
		void set_irany_y(float _irany_y);
		LPD3DXMESH get_mesh() const;
		D3DXVECTOR3 get_pos() const;
		void set_pos(D3DXVECTOR3 _pos);
		D3DXMATRIX get_transform() const;
		bool get_pick() const;
		void set_pick(bool _kivalaszt);
		D3DXVECTOR3 get_start() const;
		void set_start(D3DXVECTOR3 _start);
		D3DXVECTOR3 get_cel() const;
		void set_cel(D3DXVECTOR3 _cel);

		Bbox calc_bounding_box(ID3DXMesh*);
		Bbox get_bounding_box() const;
		Bbox get_forgatott_bbox() const;

		void kivalaszt_box(void);

		void mozgas(void);
		void mozgas_vonal(void);
		void mv_render(D3DXMATRIX, D3DXMATRIX);
		void cel_render(LPD3DXMESH);
		float iranyszog_y(float, D3DXVECTOR3, D3DXVECTOR3);
		void render(void);
		void bbox_forgat(void);

	private:
		string _nev;
		string _tipus;
		int _hp;
		int _energia;
		float _sebesseg;
		int _legenyseg;
		bool _pajzs;
		string _birodalom;
		int _bounding; // bounding box: 0, bounding sphere: 1
		float _irany_y;
		bool _kivalaszt;

		D3DXMATRIX _matRotateX;
		D3DXMATRIX _matRotateY;
		D3DXMATRIX _matRotateZ;
		D3DXMATRIX _matScale;
		D3DXMATRIX _matTranslate;
		D3DXMATRIX _transform;
		D3DXMATRIX _boundingRotateY; // bounding forgatáshoz

		D3DXVECTOR3 _pos;

		vector<D3DMATERIAL9> _materials;
		vector<IDirect3DTexture9*> _textures;
		LPD3DXMESH _mesh;

		float _tavX;
		float _tavY;
		float _tavZ;
		float _time_need_x;
		float _time_need_y;
		float _time_need_z;
		D3DXVECTOR3 _start;
		D3DXVECTOR3 _cel;

		D3DXVECTOR3 _vertexList[2]; // mozgás vonal

		Bbox _bounding_box;
		Bbox _forgatott_bbox;
		LPD3DXMESH _boxmesh;
};