#pragma once

#include <d3dx9.h>

// Ray
struct Ray
{
	D3DXVECTOR3 _origin;
	D3DXVECTOR3 _direction;
};

// vertex form�tum
struct CUSTOMVERTEX
{
	FLOAT X;
	FLOAT Y;
	FLOAT Z;
	D3DVECTOR NORMAL; // vertex norm�l
	FLOAT tu; // text�ra coordin�ta
	FLOAT tv; // text�ra coordin�ta
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

// shader vertex form�tum
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

// sz�rmaztatott frame strukt�ra
struct Frame : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};

// sz�rmaztatott kont�ner strukt�ra
struct Container : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTextures; // text�ra t�mb

	// SkinMesh inf�k
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