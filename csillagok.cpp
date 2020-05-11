#include "csillagok.h"

Csillagok::Csillagok(string _nev, string _tipus, int _bounding, IDirect3DTexture9 *_texture, LPD3DXMESH _mesh, D3DXVECTOR3 _pos, float _r)
{
	_kivalaszt = FALSE;

	this->_nev = _nev;
	this->_tipus = _tipus;
	this->_bounding = _bounding;
	this->_texture = _texture;
	this->_mesh = _mesh;
	this->_pos = _pos;
	this->_r = _r;

	_bounding_sphere = calc_bounding_sphere(_mesh);
}

Csillagok::~Csillagok()
{

}

string Csillagok::get_nev() const { return _nev; }

void Csillagok::set_nev(string _nev) { this->_nev = _nev; }

string Csillagok::get_tipus() const { return _tipus; }

void Csillagok::set_tipus(string _tipus) { this->_tipus = _tipus; }

int Csillagok::get_bounding() const { return _bounding; }

LPD3DXMESH Csillagok::get_mesh() const { return _mesh; }

D3DXVECTOR3 Csillagok::get_pos() const { return _pos; } // { return D3DXVECTOR3(_transform._41, _transform._42, _transform._43); }

D3DXMATRIX Csillagok::get_transform() const { return _transform; }

bool Csillagok::get_pick() const { return _kivalaszt; }

void Csillagok::set_pick(bool _kivalaszt) { this->_kivalaszt = _kivalaszt; }

Bsphere Csillagok::get_bounding_sphere() const { return _bounding_sphere; }

Bsphere Csillagok::calc_bounding_sphere(ID3DXMesh* _mesh)
{
	D3DXVECTOR3 _center;
	_center.x = 0.0f;
	_center.y = 0.0f;
	_center.z = 0.0f;

	float _radius = 0.0f;

	BYTE* v = 0;
	_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&v);

	D3DXComputeBoundingSphere((D3DXVECTOR3*)v, _mesh->GetNumVertices(), D3DXGetFVFVertexSize(_mesh->GetFVF()), &_center, &_radius);

	_mesh->UnlockVertexBuffer();

	Bsphere _result;
	_result._center = _center;
	_result._radius = _radius;

	return _result;
}

void Csillagok::render(void)
{
	d3ddev->SetTexture(0, _texture);
	D3DXMatrixTranslation(&_matTranslate, _pos.x, _pos.y, _pos.z);
	D3DXMatrixRotationY(&_matRotateY, ksz * _r);
	d3ddev->SetTransform(D3DTS_WORLD, &(_matRotateY * _matTranslate));
	d3ddev->GetTransform(D3DTS_WORLD, &_transform);
	_mesh->DrawSubset(0);

	return;
}