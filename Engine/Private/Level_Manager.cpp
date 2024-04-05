#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

CLevel_Manager::CLevel_Manager()
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLevel_Manager::Initialize()
{
	return S_OK;
}

void CLevel_Manager::Tick(_float fTimeDelta)
{
	m_pCurrentLevel->Tick(fTimeDelta);
}

HRESULT CLevel_Manager::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == pNewLevel)
		return E_FAIL;

	/* 기존레벨의 자원을 삭제한다. */
	if(nullptr != m_pCurrentLevel)
		m_pGameInstance->Clear_Resources(m_iLevelIndex);	

	Safe_Release(m_pCurrentLevel);		

	m_pCurrentLevel = pNewLevel;

	m_iLevelIndex = iLevelIndex;

	return S_OK;
}

CLevel_Manager * CLevel_Manager::Create()
{
	CLevel_Manager*		pInstance = new CLevel_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pCurrentLevel);
}

