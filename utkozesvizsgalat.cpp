#include "utkozesvizsgalat.h"

Utkozesvizsgalat::Utkozesvizsgalat()
{

}

Utkozesvizsgalat::~Utkozesvizsgalat()
{

}

bool Utkozesvizsgalat::utkozes_sphere_point(D3DXVECTOR3 &p, D3DXVECTOR3 _center, float _radius, D3DXVECTOR3 _pos)
{
	float _distance = TheFunctions.tavmeres(p, (_pos + _center));

	if (_distance <= _radius)
	{
		return TRUE;
	} else {
		return FALSE;
	}
}

bool Utkozesvizsgalat::utkozes_sphere_sphere(D3DXVECTOR3 _center_A, float _radius_A, D3DXVECTOR3 _pos_A, D3DXVECTOR3 _center_B, float _radius_B, D3DXVECTOR3 _pos_B)
{
	float _distance = TheFunctions.tavmeres((_pos_A + _center_A), (_pos_B + _center_B));

	if (_distance <= (_radius_A + _radius_B))
	{
		return TRUE;
	} else {
		return FALSE;
	}
}

void Utkozesvizsgalat::utkozesvizsgalat_loop(void)
{
	float _tav = -1.0f;
	bool _utkozes = FALSE;
	D3DXVECTOR3 _cam_pos;

	while (!game_over)
	{
		_utkozes = FALSE;
		_tav = -1.0f;
		TheCamera.getPosition(&_cam_pos);

		if (hajok_vec.size() > 0)
		{
			for (size_t i = 0; i < hajok_vec.size(); i++)
			{
				// hajó - kamera ütközés
				_tav = TheFunctions.tavmeres(_cam_pos, hajok_vec[i].get_pos());
				if (_tav > 0.0f && _tav < 16.0f)
				{
					_utkozes = utkozes_sphere_point(_cam_pos, hajok_vec[i].get_bounding_sphere()._center, hajok_vec[i].get_bounding_sphere()._radius, hajok_vec[i].get_pos());
					if (_utkozes)
					{
						TheCamera.walk(10.0f);
						TheCamera.strafe(10.0f);
					}
				}

				// hajó - hajó, csillag, bolygó, hold ütközés
				for (size_t j = 0; j < hajok_vec.size(); j++)
				{
					_tav = TheFunctions.tavmeres(hajok_vec[i].get_pos(), hajok_vec[j].get_pos());
					if (_tav > 0.0f && _tav < 16.0f)
					{
						_utkozes = utkozes_sphere_sphere(hajok_vec[i].get_bounding_sphere()._center, hajok_vec[i].get_bounding_sphere()._radius, hajok_vec[i].get_pos(), hajok_vec[j].get_bounding_sphere()._center, hajok_vec[j].get_bounding_sphere()._radius, hajok_vec[j].get_pos());
						if (_utkozes)
						{
							hajok_vec[i].set_hp(0);
							hajok_vec[i].set_pick(FALSE);
							hajok_vec[i].anim_run();
							hajok_vec[j].set_hp(0);
							hajok_vec[j].set_pick(FALSE);
							hajok_vec[j].anim_run();
						}
					}
				}
				for (size_t k = 0; k < csillagok_vec.size(); k++)
				{
					_tav = TheFunctions.tavmeres(hajok_vec[i].get_pos(), csillagok_vec[k].get_pos());
					if (_tav > 0.0f && _tav < 80.0f)
					{
						_utkozes = utkozes_sphere_sphere(hajok_vec[i].get_bounding_sphere()._center, hajok_vec[i].get_bounding_sphere()._radius, hajok_vec[i].get_pos(), csillagok_vec[k].get_bounding_sphere()._center, csillagok_vec[k].get_bounding_sphere()._radius, csillagok_vec[k].get_pos());
						if (_utkozes)
						{
							hajok_vec[i].set_hp(0);
							hajok_vec[i].set_pick(FALSE);
							hajok_vec[i].anim_run();
						}
					}
				}
				for (size_t l = 0; l < bolygok_vec.size(); l++)
				{
					_tav = TheFunctions.tavmeres(hajok_vec[i].get_pos(), bolygok_vec[l].get_pos());
					if (_tav > 0.0f && _tav < 40.0f)
					{
						_utkozes = utkozes_sphere_sphere(hajok_vec[i].get_bounding_sphere()._center, hajok_vec[i].get_bounding_sphere()._radius, hajok_vec[i].get_pos(), bolygok_vec[l].get_bounding_sphere()._center, bolygok_vec[l].get_bounding_sphere()._radius, bolygok_vec[l].get_pos());
						if (_utkozes)
						{
							hajok_vec[i].set_hp(0);
							hajok_vec[i].set_pick(FALSE);
							hajok_vec[i].anim_run();
						}
					}
				}
				for (size_t m = 0; m < holdak_vec.size(); m++)
				{
					_tav = TheFunctions.tavmeres(hajok_vec[i].get_pos(), holdak_vec[m].get_pos());
					if (_tav > 0.0f && _tav < 20.0f)
					{
						_utkozes = utkozes_sphere_sphere(hajok_vec[i].get_bounding_sphere()._center, hajok_vec[i].get_bounding_sphere()._radius, hajok_vec[i].get_pos(), holdak_vec[m].get_bounding_sphere()._center, holdak_vec[m].get_bounding_sphere()._radius, holdak_vec[m].get_pos());
						if (_utkozes)
						{
							hajok_vec[i].set_hp(0);
							hajok_vec[i].set_pick(FALSE);
							hajok_vec[i].anim_run();
						}
					}
				}
			}
		}

		// kamera ütközés égitesttel
		for (size_t n = 0; n < csillagok_vec.size(); n++)
		{
			_tav = TheFunctions.tavmeres(_cam_pos, csillagok_vec[n].get_pos());
			if (_tav < 80.0f)
			{
				_utkozes = utkozes_sphere_point(_cam_pos, csillagok_vec[n].get_bounding_sphere()._center, csillagok_vec[n].get_bounding_sphere()._radius, csillagok_vec[n].get_pos());
				if (_utkozes)
				{
					TheCamera.walk(10.0f);
					TheCamera.strafe(10.0f);
				}
			}
		}

		for (size_t o = 0; o < bolygok_vec.size(); o++)
		{
			_tav = TheFunctions.tavmeres(_cam_pos, bolygok_vec[o].get_pos());
			if (_tav < 40.0f)
			{
				_utkozes = utkozes_sphere_point(_cam_pos, bolygok_vec[o].get_bounding_sphere()._center, bolygok_vec[o].get_bounding_sphere()._radius, bolygok_vec[o].get_pos());
				if (_utkozes)
				{
					TheCamera.walk(10.0f);
					TheCamera.strafe(10.0f);
				}
			}
		}

		for (size_t p = 0; p < holdak_vec.size(); p++)
		{
			_tav = TheFunctions.tavmeres(_cam_pos, holdak_vec[p].get_pos());
			if (_tav < 20.0f)
			{
				_utkozes = utkozes_sphere_point(_cam_pos, holdak_vec[p].get_bounding_sphere()._center, holdak_vec[p].get_bounding_sphere()._radius, holdak_vec[p].get_pos());
				if (_utkozes)
				{
					TheCamera.walk(10.0f);
					TheCamera.strafe(10.0f);
				}
			}
		}

		Sleep(100);
	}

	return;
}