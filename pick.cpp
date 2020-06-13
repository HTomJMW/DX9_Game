#include "pick.h"

Pick::Pick()
{

}

Pick::~Pick()
{

}

Ray Pick::CalcPickingRay(LPDIRECT3DDEVICE9 Device, int clickxPos, int clickyPos)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	Device->GetViewport(&vp);

	D3DXMATRIX proj;
	Device->GetTransform(D3DTS_PROJECTION, &proj);

	px = (((2.0f * clickxPos) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f * clickyPos) / vp.Height) + 1.0f) / proj(1, 1);

	Ray ray;
	ray._origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction = D3DXVECTOR3(px, py, 1.0f);

	return ray;
}

void Pick::TransformRay(Ray* ray, D3DXMATRIX* invertViewMatrix)
{
	// transform the ray's origin, w = 1.
	D3DXVec3TransformCoord(
		&ray->_origin,
		&ray->_origin,
		invertViewMatrix);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(
		&ray->_direction,
		&ray->_direction,
		invertViewMatrix);

	// normalize the direction
	D3DXVec3Normalize(&ray->_direction, &ray->_direction);

	return;
}

bool Pick::pick(ID3DXMesh *mesh, D3DXMATRIX &mesh_world)
{
	// Calculate the picking ray
	Ray ray = CalcPickingRay(d3ddev, clickxPos_lmb, clickyPos_lmb);

	// transform the ray from view space to world space
	// get view matrix
	D3DXMATRIX view;
	d3ddev->GetTransform(D3DTS_VIEW, &view);

	// inverse it
	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);

	// apply on the ray
	TransformRay(&ray, &viewInverse);

	// átváltás a worldspace-re, a mesh a worldspace-ben van
	/*D3DXMATRIX world;
	d3ddev->GetTransform(D3DTS_WORLD, &world);*/

	D3DXMATRIX worldInverse;
	D3DXMatrixInverse(&worldInverse, 0, /*&world*/&mesh_world);

	TransformRay(&ray, &worldInverse);

	// collision detection
	BOOL _hit = FALSE;

	D3DXIntersect(mesh, &ray._origin, &ray._direction, &_hit, NULL, NULL, NULL, NULL, NULL, NULL);

	if (_hit)
	{
		return TRUE;
	}
	else { return FALSE; }
}

void Pick::tav_fuggo_pick(void)
{
	D3DXVECTOR3 _cam_pos;
	TheCamera.getPosition(&_cam_pos);

	D3DXMATRIX _tr;

	int _cs_szam = -1; // legközelebbi csillag száma
	float _cs_tav = 1000.0f; // legközelebbi csillag távolsága

	int _b_szam = -1;
	float _b_tav = 1000.0f;

	int _h_szam = -1;
	float _h_tav = 1000.0f;

	int _hajo_szam = -1;
	float _hajo_tav = 1000.0f;

	for (size_t i = 0; i < csillagok_vec.size(); i++)
	{
		csillagok_vec[i].set_pick(FALSE);
		_tr = csillagok_vec[i].get_transform();
		if (pick(csillagok_vec[i].get_mesh(), _tr))
		{
			float _tav = TheFunctions.tavmeres(_cam_pos, csillagok_vec[i].get_pos());
			if (_tav < _cs_tav) { _cs_tav = _tav; _cs_szam = i; }
		}
	}

	for (size_t i = 0; i < bolygok_vec.size(); i++)
	{
		bolygok_vec[i].set_pick(FALSE);
		_tr = bolygok_vec[i].get_transform();
		if (pick(bolygok_vec[i].get_mesh(), _tr))
		{
			float _tav = TheFunctions.tavmeres(_cam_pos, bolygok_vec[i].get_pos());
			if (_tav < _b_tav) { _b_tav = _tav; _b_szam = i; }
		}
	}

	for (size_t i = 0; i < holdak_vec.size(); i++)
	{
		holdak_vec[i].set_pick(FALSE);
		_tr = holdak_vec[i].get_transform();
		if (pick(holdak_vec[i].get_mesh(), _tr))
		{
			float _tav = TheFunctions.tavmeres(_cam_pos, holdak_vec[i].get_pos());
			if (_tav < _h_tav) { _h_tav = _tav; _h_szam = i; }
		}
	}

	for (size_t i = 0; i < hajok_vec.size(); i++)
	{
		hajok_vec[i].set_pick(FALSE);
		_tr = hajok_vec[i].get_transform();
		if (pick(hajok_vec[i].get_mesh(), _tr))
		{
			float _tav = TheFunctions.tavmeres(_cam_pos, hajok_vec[i].get_pos());
			if (_tav < _hajo_tav) { _hajo_tav = _tav; _hajo_szam = i; }
		}
	}

	int _obj_tomb[4] = { _cs_szam, _b_szam, _h_szam, _hajo_szam };
	float _tav_tomb[4] = { _cs_tav, _b_tav, _h_tav, _hajo_tav };

	// legkissebb távolság elemének száma a tömbben
	int _closest = distance(_tav_tomb, min_element(_tav_tomb, _tav_tomb + 4));

	if (_closest == 0 && _cs_szam > -1) { csillagok_vec[_cs_szam].set_pick(TRUE); }
	if (_closest == 1 && _b_szam > -1) { bolygok_vec[_b_szam].set_pick(TRUE); }
	if (_closest == 2 && _h_szam > -1) { holdak_vec[_h_szam].set_pick(TRUE); }
	if (_closest == 3 && _hajo_szam > -1) { hajok_vec[_hajo_szam].set_pick(TRUE); }

	return;
}

void Pick::send(void)
{
	// Calculate the picking ray
	Ray ray = CalcPickingRay(d3ddev, clickxPos_rmb, clickyPos_rmb);

	// transform the ray from view space to world space
	// get view matrix
	D3DXMATRIX view;
	d3ddev->GetTransform(D3DTS_VIEW, &view);

	// inverse it
	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);

	// apply on the ray
	TransformRay(&ray, &viewInverse);

	// get mapclick positions with ray
	D3DXIntersectTri(&map_b_also, &map_b_felso, &map_j_also, &ray._origin, &ray._direction, &pU, &pV, NULL);

	//update UWV
	TheFunctions.update_cel();

	for (size_t i = 0; i < hajok_vec.size(); i++)
	{
		if (hajok_vec[i].get_pick())
		{
			D3DXVECTOR3 _pos = hajok_vec[i].get_pos();

			hajok_vec[i].set_start(_pos);
			hajok_vec[i].set_cel(cel_UWV);

			float _irany = hajok_vec[i].get_irany_y();
			float _uj_irany = hajok_vec[i].iranyszog_y(_irany, _pos, cel_UWV);

			hajok_vec[i].set_irany_y(_uj_irany);

			break;
		}
	}

	return;
}