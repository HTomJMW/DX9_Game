#include "diag.h"

Diag::Diag()
{

}

Diag::~Diag()
{

}

void Diag::diag_mentes(void)
{
	string _info;

	ofstream diaglog;
	diaglog.open("diag.log");

	diaglog << "DiagLog DX9 Game - [H] Tom" << endl << endl;

	for (size_t i = 0; i < d3d_szoveg_diaglista.size(); i++)
	{
		if (d3d_szoveg_diaglista[i] == D3D_OK || d3d_szoveg_diaglista[i] == S_OK) { _info = "OK"; } else { _info = "HIBA"; }
		diaglog << "D3D_szoveg hr" << i << ": " << _info << endl;
	}

	diaglog << endl;

	for (size_t i = 0; i < d3d_tex_diaglista.size(); i++)
	{
		if (d3d_tex_diaglista[i] == D3D_OK || d3d_tex_diaglista[i] == S_OK) { _info = "OK"; } else { _info = "HIBA"; }
		diaglog << "D3D_tex hr" << i << ": " << _info << endl;
	}

	diaglog << endl;

	for (size_t i = 0; i < d3d_sprite_diaglista.size(); i++)
	{
		if (d3d_sprite_diaglista[i] == D3D_OK || d3d_sprite_diaglista[i] == S_OK) { _info = "OK"; } else { _info = "HIBA"; }
		diaglog << "D3D_sprite hr" << i << ": " << _info << endl;
	}

	diaglog << endl;

	for (size_t i = 0; i < mesh_diaglista.size(); i++)
	{
		if (mesh_diaglista[i] == D3D_OK || mesh_diaglista[i] == S_OK) { _info = "OK"; } else { _info = "HIBA"; }
		diaglog << "Mesh hr" << i << ": " << _info << " " << endl;

		for (size_t j = 0; j < mesh_tex_diaglista.size(); j++)
		{
			if (mesh_tex_diaglista[i] == D3D_OK || mesh_tex_diaglista[i] == S_OK) { _info = "OK"; } else { _info = "HIBA"; }
			diaglog << "     " << "Mesh" << i << "_tex hr" << j << ": " << _info << endl;
		}

		diaglog << endl << endl;
	}

	for (size_t i = 0; i < line_diaglista.size(); i++)
	{
		if (line_diaglista[i] == D3D_OK || line_diaglista[i] == S_OK) { _info = "OK"; } else { _info = "HIBA"; }
		diaglog << "D3D_line hr" << i << ": " << _info << endl;
	}

	diaglog << endl;

	for (size_t i = 0; i < mesh_hierarchy_diaglista.size(); i++)
	{
		if (mesh_hierarchy_diaglista[i] == D3D_OK || mesh_hierarchy_diaglista[i] == S_OK) { _info = "OK"; } else { _info = "HIBA"; }
		diaglog << "Hajok_mesh_hierarchy hr" << i << ": " << _info << endl;
	}

	diaglog.close();

	return;
}