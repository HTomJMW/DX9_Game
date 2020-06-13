#include "hierarchy.h"

Hierarchy::Hierarchy()
{
	UseSoftwareVP = FALSE;
}

Hierarchy::~Hierarchy()
{

}

HRESULT Hierarchy::AllocateName(LPCSTR Name, LPSTR* pNewName)
{
	UINT hossz;
	hossz = (UINT)strlen(Name) + 1;
	*pNewName = new CHAR[hossz];
	memcpy(*pNewName, Name, hossz * sizeof(CHAR));

	return S_OK;
}

HRESULT Hierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	Frame* pFrame;

	*ppNewFrame = NULL;

	pFrame = new Frame;

	AllocateName(Name, &pFrame->Name);

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;

	pFrame = NULL;
	
	return S_OK;
}

HRESULT Hierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);

	return S_OK;
}

HRESULT Hierarchy::CreateMeshContainer(LPCSTR Name, const D3DXMESHDATA *pMeshData, const D3DXMATERIAL *pMaterials, const D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, const DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	Container *pMeshContainer = NULL;
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;
	LPD3DXMESH pMesh = NULL;

	*ppNewMeshContainer = NULL;

	pMesh = pMeshData->pMesh;

	pMeshContainer = new Container;

	memset(pMeshContainer, 0, sizeof(Container));

	AllocateName(Name, &pMeshContainer->Name);

	pMesh->GetDevice(&pd3dDevice);
	UINT num_faces = pMesh->GetNumFaces();

	pMeshContainer->MeshData.pMesh = pMesh;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	pMesh->AddRef();

	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[num_faces * 3];

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * num_faces * 3);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

	for (UINT i = 0; i < NumMaterials; i++)
	{
		LPSTR file_name = pMaterials[i].pTextureFilename;

		string fn = file_name;
		string path = "Models/" + fn;
		LPSTR pPath = const_cast<char*>(path.c_str());

		D3DXCreateTextureFromFile(pd3dDevice, pPath, &pMeshContainer->ppTextures[i]);

		// elfelejteni a fájlnevet betöltés után
		pMeshContainer->pMaterials[i].pTextureFilename = NULL;
	}

	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		UINT c_bones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[c_bones];

		for (UINT i = 0; i < c_bones; i++)
		{
			pMeshContainer->pBoneOffsetMatrices[i] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;
	
	return D3D_OK;
}

HRESULT Hierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	Container* pMeshContainer = (Container*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

	// release all the allocated textures
	if (pMeshContainer->ppTextures != NULL)
	{
		for (UINT i = 0; i < pMeshContainer->NumMaterials; i++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[i]);
		}
	}

	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_RELEASE(pMeshContainer->pOrigMesh);
	SAFE_DELETE(pMeshContainer);

	return S_OK;
}