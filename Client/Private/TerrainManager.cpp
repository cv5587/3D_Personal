#include "TerrainManager.h"
#include "Terrain.h"
#include "GameInstance.h"


CTerrainManager::CTerrainManager()	
	:m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CTerrainManager::Initialize()
{
	return S_OK;
	
}

HRESULT CTerrainManager::Clone_Terrain(void* pArg)	
{
	_int* sTerrainUV=(_int*)pArg;	
	CTerrain::TERRAIN_DESC pDesc{};
	pDesc.TerrainUV[0] = sTerrainUV[0];
	pDesc.TerrainUV[1] = sTerrainUV[1];
	XMStoreFloat4x4(&pDesc.vPrePosition, XMMatrixIdentity());
	pDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Terrain");	
	pDesc.ModelTag = TEXT("Prototype_Component_VIBuffer_Terrain");
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 
		TEXT("Prototype_GameObject_Terrain"), &m_pTerrain, &pDesc)))
		return E_FAIL;
	
	return S_OK;
}

CComponent* CTerrainManager::Get_Terrain_Component(const wstring& ComTag)
{
	return nullptr;
}

void CTerrainManager::Terrain_Release()
{
	Safe_Release(m_pTerrain);
}



CTerrainManager* CTerrainManager::Create()
{
	CTerrainManager* pInstance = new CTerrainManager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CTerrainManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrainManager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pTerrain);
}
