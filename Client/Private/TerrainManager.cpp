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
	pDesc.vPrePosition = _float4{ 0.f,0.f,0.f,1.f };
	pDesc.ObjectID = -3;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 
		TEXT("Prototype_GameObject_Terrain"), &m_pTerrain, &pDesc)))
		return E_FAIL;
	
	return S_OK;
}

_matrix CTerrainManager::Get_Terrain_WorldMatrix()
{
	
	return dynamic_cast<CTransform*>(m_pTerrain->Get_Transform())->Get_WorldMatrix();
}

_float4* CTerrainManager::Get_Terrain_VtxPos()
{
	return 	dynamic_cast<CVIBuffer_Terrain*>(m_pTerrain->Get_Component(TEXT("Com_VIBuffer")))->Get_Terrain_Vtx();
}

_int* CTerrainManager::Get_Terrain_UV()
{
	return 	dynamic_cast<CVIBuffer_Terrain*>(m_pTerrain->Get_Component(TEXT("Com_VIBuffer")))->Get_Terrain_UV();
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
