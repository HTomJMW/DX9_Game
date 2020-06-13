#pragma once

#include <d3dx9.h>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Diag
{
	public:
		Diag();
		~Diag();

		vector<HRESULT> d3d_szoveg_diaglista;
		vector<HRESULT> d3d_tex_diaglista;
		vector<HRESULT> d3d_sprite_diaglista;
		vector<HRESULT> mesh_diaglista;
		vector<HRESULT> mesh_tex_diaglista;
		vector<HRESULT> line_diaglista;
		vector<HRESULT> mesh_hierarchy_diaglista;

		void diag_mentes(void);
};