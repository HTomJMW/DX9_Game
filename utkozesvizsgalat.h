#pragma once

#include <vector>
#include <d3dx9.h>

extern float tavmeres(D3DXVECTOR3, D3DXVECTOR3);

class Utkozesvizsgalat
{
	public:
		Utkozesvizsgalat();
		~Utkozesvizsgalat();

		bool utkozes_box_point(D3DXVECTOR3 &p, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3);
		bool utkozes_sphere_point(D3DXVECTOR3 &p, D3DXVECTOR3, float, D3DXVECTOR3);
		bool utkozes_box_box(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3);
		bool utkozes_sphere_sphere(D3DXVECTOR3, float, D3DXVECTOR3, D3DXVECTOR3, float, D3DXVECTOR3);
		bool utkozes_box_sphere(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3, float, D3DXVECTOR3);
};