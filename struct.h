#pragma once

#include <d3dx9.h>

// Ray
struct Ray
{
	D3DXVECTOR3 _origin;
	D3DXVECTOR3 _direction;
};

// vertex formátum
struct CUSTOMVERTEX
{
	FLOAT X;
	FLOAT Y;
	FLOAT Z;
	D3DVECTOR NORMAL; // vertex normál
	FLOAT tu; // textúra coordináta
	FLOAT tv; // textúra coordináta
};

// bounding box
struct Bbox
{
	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};

// bounding sphere
struct Bsphere
{
	D3DXVECTOR3 _center;
	float _radius;
};

// shader vertex formátum
struct SVertex
{
	D3DXVECTOR3 pos;
};

// .vaw audio
struct WaveHeaderType
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
};

// származtatott frame struktúra
struct Frame : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};

// származtatott konténer struktúra
struct Container : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTextures; // textúra tömb

	// SkinMesh infók
	LPD3DXMESH pOrigMesh;
	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD NumAttributeGroups;
	DWORD NumInfl;
	LPD3DXBUFFER pBoneCombinationBuf;
	D3DXMATRIX** ppBoneMatrixPtrs;
	D3DXMATRIX* pBoneOffsetMatrices;
	DWORD NumPaletteEntries;
	bool UseSoftwareVP;
	DWORD iAttributeSW;
};