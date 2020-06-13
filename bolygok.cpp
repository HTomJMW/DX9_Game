#include "bolygok.h"

Bolygok::Bolygok(string _nev, string _tipus, bool _lakhato, int _nepesseg, string _birodalom, const vector<D3DMATERIAL9> &_mtrls_mesh, const vector<IDirect3DTexture9*> &_textures_mesh, const LPD3DXMESH &_mesh, D3DXVECTOR3 _pos_1, D3DXVECTOR3 _pos_2, float _r)
{
	_kivalaszt = FALSE;

	this->_nev = _nev;
	this->_tipus = _tipus;
	this->_lakhato = _lakhato;
	this->_nepesseg = _nepesseg;
	this->_birodalom = _birodalom;
	this->_mtrls_mesh = _mtrls_mesh;
	this->_textures_mesh = _textures_mesh;
	this->_mesh = _mesh;
	this->_pos_1 = _pos_1;
	this->_pos_2 = _pos_2;
	this->_r = _r;

	_bounding_sphere = calc_bounding_sphere(_mesh);
}

Bolygok::~Bolygok()
{

}

string Bolygok::get_nev() const { return _nev; }

void Bolygok::set_nev(string _nev) { this->_nev = _nev; }

string Bolygok::get_tipus() const { return _tipus; }

void Bolygok::set_tipus(string _tipus) { this->_tipus = _tipus; }

bool Bolygok::get_lakhato() const { return _lakhato; }

void Bolygok::set_lakhato(bool _lakhato) { this->_lakhato = _lakhato; }

int Bolygok::get_nepesseg() const { return _nepesseg; }

void Bolygok::set_nepesseg(int _nepesseg) { this->_nepesseg = _nepesseg; }

string Bolygok::get_birodalom() const { return _birodalom; }

void Bolygok::set_birodalom(string _birodalom) { this->_birodalom = _birodalom; }

LPD3DXMESH Bolygok::get_mesh() const { return _mesh; }

D3DXVECTOR3 Bolygok::get_pos() const { return D3DXVECTOR3(_transform._41, _transform._42, _transform._43); }

D3DXMATRIX Bolygok::get_transform() const { return _transform; }

bool Bolygok::get_pick() const { return _kivalaszt; }

void Bolygok::set_pick(bool _kivalaszt) { this->_kivalaszt = _kivalaszt; }

Bsphere Bolygok::get_bounding_sphere() const { return _bounding_sphere; }

Bsphere Bolygok::calc_bounding_sphere(ID3DXMesh* _mesh)
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

void Bolygok::render(void)
{
	D3DXMatrixTranslation(&_matTranslate1, _pos_1.x, _pos_1.y, _pos_1.z);
	D3DXMatrixRotationY(&_matRotateY, ksz * _r);
	D3DXMatrixTranslation(&_matTranslate2, _pos_2.x, _pos_2.y, _pos_2.z);
	d3ddev->SetTransform(D3DTS_WORLD, &(_matTranslate1 * _matRotateY * _matTranslate2));
	d3ddev->GetTransform(D3DTS_WORLD, &_transform);
	
	for (size_t i = 0; i < _mtrls_mesh.size(); i++)
	{
		d3ddev->SetMaterial(&_mtrls_mesh[i]);
		d3ddev->SetTexture(0, _textures_mesh[i]);
		_mesh->DrawSubset(i);
	}

	return;
}