#pragma once

#include <d3dx9.h>
#include <vector>
#include <tchar.h>
#include <strsafe.h>
#include "hierarchy.h"
#include "diag.h"

using namespace std;

extern LPDIRECT3DDEVICE9 d3ddev;
extern Hierarchy TheAllocHierarchy;
extern Diag TheDiag;

class Mesh
{
	public:
		Mesh();
		~Mesh();

		LPD3DXMESH static_meshes[6];

		vector<D3DMATERIAL9> mtrls_mesh_0;
		vector<IDirect3DTexture9*> textures_mesh_0;

		vector<D3DMATERIAL9> mtrls_mesh_1;
		vector<IDirect3DTexture9*> textures_mesh_1;

		vector<D3DMATERIAL9> mtrls_mesh_2;
		vector<IDirect3DTexture9*> textures_mesh_2;

		vector<D3DMATERIAL9> mtrls_mesh_3;
		vector<IDirect3DTexture9*> textures_mesh_3;

		vector<D3DMATERIAL9> mtrls_mesh_4;
		vector<IDirect3DTexture9*> textures_mesh_4;

		vector<D3DMATERIAL9> mtrls_mesh_5;
		vector<IDirect3DTexture9*> textures_mesh_5;

		void init_static_meshes(void);
		void static_mesh_betoltes(string _fajlnev, int _elem, vector<D3DMATERIAL9> &mtrls_mesh_x, vector<IDirect3DTexture9*> &textures_mesh_x);

		void cleanup(void);

		private:
			string _fajlnev;
			int _elem;
};