#pragma once

#include <d3dx9.h>
#include <tchar.h>
#include <dshow.h> // StringCchCat
#include "struct.h"

#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }

class Hierarchy : public ID3DXAllocateHierarchy
{
	public:
		Hierarchy();
		~Hierarchy();

		bool UseSoftwareVP;

		HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName);

		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
		STDMETHOD(CreateMeshContainer)(THIS_
			LPCSTR Name,
			const D3DXMESHDATA *pMeshData,
			const D3DXMATERIAL *pMaterials,
			const D3DXEFFECTINSTANCE *pEffectInstances,
			DWORD NumMaterials,
			const DWORD *pAdjacency,
			LPD3DXSKININFO pSkinInfo,
			LPD3DXMESHCONTAINER *ppNewMeshContainer);
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};