#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "Loader.h"
#include "BackGround.h"

#include "GameInstance.h"

#pragma region LEVEL_HEADER
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#pragma endregion

#include "Data_Manager.h"
#include "TerrainManager.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CData_Manager* pDataManager)
	: CLevel(pDevice, pContext)
	, m_pDataManager{ pDataManager }
{
	Safe_AddRef(m_pDataManager);
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	/* 로딩씬에 뭔가를 보여주려한다면 그 보여주기위한 객체들을 생성한다. (백그라운드 + 로딩바 + 폰트) */
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	/* eNextLevel을 위한 자원을 로드한다.(추가적으로 생성한 스레드) */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;



	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	if (true == m_pLoader->is_Finished())
	{
	//	if (GetKeyState(VK_RETURN) & 0x8000)
	//	{
			CLevel* pNewLevel = { nullptr };

			switch (m_eNextLevel)
			{
			case LEVEL_LOGO:
				pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext, m_pDataManager);
				break;
			case LEVEL_GAMEPLAY:
				pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext, m_pDataManager);
				break;
			}

			if (nullptr == pNewLevel)
				return;

 			if (FAILED(m_pGameInstance->Open_Level(m_eNextLevel, pNewLevel)))
				return;
//		}
	}


#ifdef _DEBUG
	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());
#endif
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	/*CBackGround::BACKGROUND_DESC		ObjectDesc={};

	ObjectDesc.fData = 10.f;
	ObjectDesc.iData = 5;
	ObjectDesc.fSpeedPerSec = 10.f;
	ObjectDesc.fRotationPerSec = XMConvertToRadians(90.0f);*/
	m_pGameInstance->Play_BGM(TEXT("Loading.wav"),  0.8f);

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_LOADING, strLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel, class CData_Manager* pDataManager)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext, pDataManager);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed To Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();
	Safe_Release(m_pDataManager);
	Safe_Release(m_pLoader);
}
