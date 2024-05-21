#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"


#include "Level_Loading.h"
#include "Data_Manager.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CData_Manager* pDataManager)
	: CLevel(pDevice, pContext)
	,m_pDataManager{pDataManager}
{
	Safe_AddRef(m_pDataManager);
}

HRESULT CLevel_Logo::Initialize()
{
	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY, m_pDataManager))))
			return;
	}

#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고레벨임"));
#endif
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CData_Manager* pDataManager)
{
	CLevel_Logo*		pInstance = new CLevel_Logo(pDevice, pContext, pDataManager);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
	Safe_Release(m_pDataManager);

}
