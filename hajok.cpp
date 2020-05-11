#include "hajok.h"

Hajok::Hajok(string _nev, string _tipus, int _hp, int _energia, float _sebesseg, int _legenyseg, bool _pajzs, string _birodalom, int _bounding, float _irany_y, vector<D3DMATERIAL9> _materials, vector<IDirect3DTexture9*> _textures, LPD3DXMESH _mesh, D3DXVECTOR3 _pos, D3DXVECTOR3 _cel)
{
	_kivalaszt = FALSE;

	_tavX = 0.0f;
	_tavY = 0.0f;
	_tavZ = 0.0f;
	_time_need_x = 0.0f;
	_time_need_y = 0.0f;
	_time_need_z = 0.0f;

	this->_nev = _nev;
	this->_tipus = _tipus;
	this->_hp = _hp;
	this->_energia = _energia;
	this->_sebesseg = _sebesseg;
	this->_legenyseg = _legenyseg;
	this->_pajzs = _pajzs;
	this->_birodalom = _birodalom;
	this->_bounding = _bounding;
	this->_irany_y = _irany_y;
	this->_materials = _materials;
	this->_textures = _textures;
	this->_mesh = _mesh;
	this->_pos = _pos;
	this->_cel = _cel;

	_bounding_box = calc_bounding_box(_mesh);
	kivalaszt_box();
}

Hajok::~Hajok()
{

}

string Hajok::get_nev() const { return _nev; }

void Hajok::set_nev(string _nev) { this->_nev = _nev; }

string Hajok::get_tipus() const { return _tipus; }

void Hajok::set_tipus(string _tipus) { this->_tipus = _tipus; }

int Hajok::get_hp() const { return _hp; }

void Hajok::set_hp(int _hp) { this->_hp = _hp; }

int Hajok::get_energia() const { return _energia; }

void Hajok::set_energia(int _energia) { this->_energia = _energia; }

float Hajok::get_sebesseg() const { return _sebesseg; }

void Hajok::set_sebesseg(float _sebesseg) { this->_sebesseg = _sebesseg; }

int Hajok::get_legenyseg() const { return _legenyseg; }

void Hajok::set_legenyseg(int _legenyseg) { this->_legenyseg = _legenyseg; }

bool Hajok::get_pajzs() const { return _pajzs; }

void Hajok::set_pajzs(bool _pajzs) { this->_pajzs = _pajzs; }

string Hajok::get_birodalom() const { return _birodalom; }

void Hajok::set_birodalom(string _birodalom) { this->_birodalom = _birodalom; }

int Hajok::get_bounding() const { return _bounding; }

float Hajok::get_irany_y() const { return _irany_y; }

void Hajok::set_irany_y(float _irany_y) { this->_irany_y = _irany_y; }

LPD3DXMESH Hajok::get_mesh() const { return _mesh; }

D3DXVECTOR3 Hajok::get_pos() const { return D3DXVECTOR3(_transform._41, _transform._42, _transform._43); }

D3DXMATRIX Hajok::get_transform() const { return _transform; }

void Hajok::set_pos(D3DXVECTOR3 _pos) { this->_pos = _pos; }

bool Hajok::get_pick() const { return _kivalaszt; }

void Hajok::set_pick(bool _kivalaszt) { this->_kivalaszt = _kivalaszt; }

D3DXVECTOR3 Hajok::get_start() const { return _start; }

void Hajok::set_start(D3DXVECTOR3 _start) { this->_start = _start; }

D3DXVECTOR3 Hajok::get_cel() const { return _cel; }

void Hajok::set_cel(D3DXVECTOR3 _cel) { this->_cel = _cel; }

Bbox Hajok::get_bounding_box() const { return _bounding_box; }

Bbox Hajok::get_forgatott_bbox() const { return _forgatott_bbox; }

Bbox Hajok::calc_bounding_box(ID3DXMesh* _mesh)
{
	D3DXVECTOR3 _min;
	_min.x = 0.0f;
	_min.y = 0.0f;
	_min.z = 0.0f;

	D3DXVECTOR3 _max;
	_max.x = 0.0f;
	_max.y = 0.0f;
	_max.z = 0.0f;

	BYTE* v = 0;
	_mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&v);

	D3DXComputeBoundingBox((D3DXVECTOR3*)v, _mesh->GetNumVertices(), D3DXGetFVFVertexSize(_mesh->GetFVF()), &_min, &_max);

	_mesh->UnlockVertexBuffer();

	Bbox _result;
	_result._min = _min;
	_result._max = _max;

	return _result;
}

void Hajok::mozgas(void)
{
	// szükséges idõ tengelyenként (X és Z)
	if (_start.x > _cel.x) { _tavX = _start.x - _cel.x; }
	if (_start.x < _cel.x) { _tavX = _cel.x - _start.x; }
	if (_start.x == _cel.x) { _tavX = 0.0f; }
	_time_need_x = (_tavX / _sebesseg) * 1000; // ms

	if (_start.y > _cel.y) { _tavY = _start.y - _cel.y; }
	if (_start.y < _cel.y) { _tavY = _cel.y - _start.z; }
	if (_start.y == _cel.y) { _tavY = 0.0f; }
	_time_need_y = (_tavY / _sebesseg) * 1000; // ms

	if (_start.z > _cel.z) { _tavZ = _start.z - _cel.z; }
	if (_start.z < _cel.z) { _tavZ = _cel.z - _start.z; }
	if (_start.z == _cel.z) { _tavZ = 0.0f; }
	_time_need_z = (_tavZ / _sebesseg) * 1000; // ms

	// mozgatás
	if (_time_need_x > 0 && _time_need_x >= _time_need_z)
	{
		if (_pos.x < _cel.x) { _pos.x = _pos.x + (_tavX / (_time_need_x / delta)); }
		if (_pos.x > _cel.x) { _pos.x = _pos.x - (_tavX / (_time_need_x / delta)); }
		if (_pos.z < _cel.z) { _pos.z = _pos.z + (_tavZ / (_time_need_x / delta)); }
		if (_pos.z > _cel.z) { _pos.z = _pos.z - (_tavZ / (_time_need_x / delta)); }
	}

	if (_time_need_z > 0 && _time_need_x < _time_need_z)
	{
		if (_pos.x < _cel.x) { _pos.x = _pos.x + (_tavX / (_time_need_z / delta)); }
		if (_pos.x > _cel.x) { _pos.x = _pos.x - (_tavX / (_time_need_z / delta)); }
		if (_pos.z < _cel.z) { _pos.z = _pos.z + (_tavZ / (_time_need_z / delta)); }
		if (_pos.z > _cel.z) { _pos.z = _pos.z - (_tavZ / (_time_need_z / delta)); }
	}

	if (_time_need_y > 0 && _pos.y < _cel.y) { _pos.y = _pos.y + (_tavY / (_time_need_y / delta)); }
	if (_time_need_y > 0 && _pos.y > _cel.y) { _pos.y = _pos.y - (_tavY / (_time_need_y / delta)); }

	// korrekció számítási eltérés miatt
	if (_pos.x + 0.004 > _cel.x && _pos.x - 0.004 < _cel.x) { _pos.x = _cel.x; }
	if (_pos.y + 0.004 > _cel.y && _pos.y - 0.004 < _cel.y) { _pos.y = _cel.y; }
	if (_pos.z + 0.004 > _cel.z && _pos.z - 0.004 < _cel.z) { _pos.z = _cel.z; }

	return;
}

float Hajok::iranyszog_y(float _irany_y, D3DXVECTOR3 _pos, D3DXVECTOR3 _cel)
{
	float _uj_szog = get_irany_y();
	
	float _sx = _pos.x;
	float _sz = _pos.z;
	float _cx = _cel.x;
	float _cz = _cel.z; 

	if (_cz - _sz != 0 && _cx - _sx != 0 && _sebesseg > 0)
	{
		_uj_szog = atan2(_cz - _sz, _cx - _sx) * -180 / PI;
		if (_uj_szog < 0) { _uj_szog = 360.0f + _uj_szog; }
	}

	return _uj_szog;
}

void Hajok::mozgas_vonal(void)
{
	_vertexList[0] = _pos;
	_vertexList[1] = _cel;

	return;
}

void Hajok::mv_render(D3DXMATRIX V, D3DXMATRIX matProjection)
{
	if (_sebesseg > 0)
	{
		D3DXMATRIX tempFinal = V * matProjection;
		line->SetWidth(1.5f);
		line->SetAntialias(TRUE);
		line->Begin();
		line->DrawTransform(_vertexList, 2, &tempFinal, D3DCOLOR_XRGB(120, 120, 120));
		line->End();
	}

	return;
}

void Hajok::cel_render(LPD3DXMESH _mesh)
{
	if (_sebesseg > 0)
	{
		d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
		D3DXMatrixTranslation(&_matTranslate, _cel.x, _cel.y, _cel.z);
		D3DXMatrixRotationY(&_matRotateY, D3DXToRadian(45.0f));
		d3ddev->SetTransform(D3DTS_WORLD, &(_matRotateY * _matTranslate));
		d3ddev->SetTexture(0, NULL);
		_mesh->DrawSubset(0);
		d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	return;
}

void Hajok::kivalaszt_box(void)
{
	D3DXCreateBox(d3ddev, _bounding_box._max.x - _bounding_box._min.x, _bounding_box._max.y - _bounding_box._min.y, _bounding_box._max.z - _bounding_box._min.z, &_boxmesh, NULL);

	return;
}

void Hajok::render(void)
{
	D3DXMatrixTranslation(&_matTranslate, _pos.x, _pos.y, _pos.z);
	D3DXMatrixRotationY(&_matRotateY, D3DXToRadian(_irany_y));
	d3ddev->SetTransform(D3DTS_WORLD, &(_matRotateY * _matTranslate));
	d3ddev->GetTransform(D3DTS_WORLD, &_transform);

	for (size_t i = 0; i < _materials.size(); i++)
	{
		d3ddev->SetMaterial(&_materials[i]);
		d3ddev->SetTexture(0, _textures[i]);
		_mesh->DrawSubset(i);
	}

	if (_kivalaszt)
	{
		d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
		d3ddev->SetTexture(0, NULL);
		_boxmesh->DrawSubset(0);
		d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	return;
}

void Hajok::bbox_forgat(void)
{
	// forgatás mátrix y-on, irány szöggel
	D3DXMatrixRotationY(&_boundingRotateY, D3DXToRadian(_irany_y));

	// bbox _max forgatása
	D3DXVec3TransformCoord(&_forgatott_bbox._max, &_bounding_box._max, &_boundingRotateY);
	// bbox _min forgatása
	D3DXVec3TransformCoord(&_forgatott_bbox._min, &_bounding_box._min, &_boundingRotateY);

	return;
}