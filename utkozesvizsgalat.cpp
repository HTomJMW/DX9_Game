#include "utkozesvizsgalat.h"

Utkozesvizsgalat::Utkozesvizsgalat()
{

}

Utkozesvizsgalat::~Utkozesvizsgalat()
{

}

bool Utkozesvizsgalat::utkozes_box_point(D3DXVECTOR3 &p, D3DXVECTOR3 _min, D3DXVECTOR3 _max, D3DXVECTOR3 _pos)
{
	if ((p.x >= _pos.x + _min.x && p.y >= _pos.y + _min.y && p.z >= _pos.z + _min.z) && (p.x <= _pos.x + _max.x && p.y <= _pos.y + _max.y && p.z <= _pos.z + _max.z))
	{
		return TRUE;
	} else {
		return FALSE;
	}
}

bool Utkozesvizsgalat::utkozes_sphere_point(D3DXVECTOR3 &p, D3DXVECTOR3 _center, float _radius, D3DXVECTOR3 _pos)
{
	float _distance = tavmeres(p, (_pos + _center));

	if (_distance <= _radius)
	{
		return TRUE;
	} else {
		return FALSE;
	}
}

bool Utkozesvizsgalat::utkozes_box_box(D3DXVECTOR3 _min_A, D3DXVECTOR3 _max_A, D3DXVECTOR3 _pos_A, D3DXVECTOR3 _min_B, D3DXVECTOR3 _max_B, D3DXVECTOR3 _pos_B)
{
	if ((_pos_A.x + _min_A.x <= _pos_B.x + _max_B.x && _pos_A.x + _max_A.x >= _pos_B.x + _min_B.x) &&
		(_pos_A.y + _min_A.y <= _pos_B.y + _max_B.y && _pos_A.y + _max_A.y >= _pos_B.y + _min_B.y) &&
		(_pos_A.z + _min_A.z <= _pos_B.z + _max_B.z && _pos_A.z + _max_A.z >= _pos_B.z + _min_B.z))
	{
		return TRUE;
	} else {
		return FALSE;
	}
}

bool Utkozesvizsgalat::utkozes_sphere_sphere(D3DXVECTOR3 _center_A, float _radius_A, D3DXVECTOR3 _pos_A, D3DXVECTOR3 _center_B, float _radius_B, D3DXVECTOR3 _pos_B)
{
	float _distance = tavmeres((_pos_A + _center_A), (_pos_B + _center_B));

	if (_distance <= (_radius_A + _radius_B))
	{
		return TRUE;
	} else {
		return FALSE;
	}
}

bool Utkozesvizsgalat::utkozes_box_sphere(D3DXVECTOR3 _box_min, D3DXVECTOR3 _box_max, D3DXVECTOR3 _box_pos, D3DXVECTOR3 _sphere_center, float _sphere_radius, D3DXVECTOR3 _sphere_pos)
{
	float _x = 0.0f;
	float _y = 0.0f;
	float _z = 0.0f;
	float _distance = 0.0f;

	_x = max((_box_pos.x + _box_min.x), min((_sphere_pos.x + _sphere_center.x), (_box_pos.x + _box_max.x)));
	_y = max((_box_pos.y + _box_min.y), min((_sphere_pos.y + _sphere_center.y), (_box_pos.y + _box_max.y)));
	_z = max((_box_pos.z + _box_min.z), min((_sphere_pos.z + _sphere_center.z), (_box_pos.z + _box_max.z)));

	_distance = tavmeres(D3DXVECTOR3(_x, _y, _z), (_sphere_pos + _sphere_center));

	if (_distance <= _sphere_radius)
	{
		return TRUE;
	} else {
		return FALSE;
	}
}