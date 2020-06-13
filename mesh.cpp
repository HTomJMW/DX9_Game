#include "mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::init_static_meshes(void)
{
	static_mesh_betoltes("Models/gomb_nagy.X", 0, mtrls_mesh_0, textures_mesh_0);
	static_mesh_betoltes("Models/gomb_kozepes.X", 1, mtrls_mesh_1, textures_mesh_1);
	static_mesh_betoltes("Models/gomb_kicsi.X", 2, mtrls_mesh_2, textures_mesh_2);
	static_mesh_betoltes("Models/cel.X", 3, mtrls_mesh_3, textures_mesh_3);
	static_mesh_betoltes("Models/flagship_3.X", 4, mtrls_mesh_4, textures_mesh_4);
	static_mesh_betoltes("Models/muhold.X", 5, mtrls_mesh_5, textures_mesh_5);

	return;
}

void Mesh::static_mesh_betoltes(string _fajlnev, int _elem, vector<D3DMATERIAL9> &mtrls_mesh_x, vector<IDirect3DTexture9*> &textures_mesh_x)
{
	HRESULT _hr_mesh;
	HRESULT _hr_tex;
	vector<HRESULT> _hr_mesh_tex;

	ID3DXBuffer* adjBuffer = NULL;
	ID3DXBuffer* mtrlBuffer = NULL;
	DWORD numMtrls;

	_hr_mesh = D3DXLoadMeshFromX(_fajlnev.c_str(), D3DXMESH_MANAGED, d3ddev, &adjBuffer, &mtrlBuffer, 0, &numMtrls, &static_meshes[_elem]);

	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (UINT i = 0; i < numMtrls; i++)
		{
			mtrls_mesh_x.push_back(mtrls[i].MatD3D);

			mtrls_mesh_x[i].Ambient = mtrls_mesh_x[i].Diffuse;

			if (mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = NULL;

				LPSTR file_name = mtrls[i].pTextureFilename;

				string fn = file_name;
				string path = "Models/" + fn;
				LPSTR pPath = const_cast<char*>(path.c_str());

				_hr_tex = D3DXCreateTextureFromFile(d3ddev, pPath, &tex);
				textures_mesh_x.push_back(tex);
				_hr_mesh_tex.push_back(_hr_tex);
			}
		}
	}
	mtrlBuffer->Release();

	TheDiag.mesh_diaglista.push_back(_hr_mesh);
	TheDiag.mesh_tex_diaglista = _hr_mesh_tex;

	return;
}

void Mesh::cleanup(void)
{
	for (int i = 0; i < ((sizeof static_meshes) / (sizeof static_meshes[0])); i++)
	{
		static_meshes[i]->Release();
	}

	return;
}