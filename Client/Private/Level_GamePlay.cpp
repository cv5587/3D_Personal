#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"

#include "FreeCamera.h"
#include "LandObject.h"	
#include "ImGuiManager.h"


CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Environment(TEXT("Layer_EnvironmentObject"))))
		return E_FAIL;

	if (FAILED(Ready_LandObjects()))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이레벨임"));
#endif
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring& strLayerTag)
{
	CFreeCamera::FREE_CAMERA_DESC		CameraDesc{};

	CameraDesc.fSensor = 0.05f;
	CameraDesc.vEye = _float4(0.0f, 30.f, -25.f, 1.f);
	CameraDesc.vAt = _float4(0.0f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 3000.f;
	CameraDesc.fSpeedPerSec = 50.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.f);
	XMStoreFloat4x4(&CameraDesc.vPrePosition, XMMatrixIdentity());
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_FreeCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	_int sTerrainUV[2] = { 51,53 };
	CImGuiManager::GetInstance()->Make_Terrain(sTerrainUV);

	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Terrain"))))
	//	return E_FAIL;
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Environment(const wstring& strLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_CliffA"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_LandObjects()
{
	CLandObject::LANDOBJ_DESC		LandObjDesc{};

	LandObjDesc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform")));
	LandObjDesc.pTerrainVIBuffer = dynamic_cast<CVIBuffer*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));


	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), &LandObjDesc)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"), &LandObjDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const wstring& strLayerTag, CLandObject::LANDOBJ_DESC* pLandObjDesc)
{
	pLandObjDesc->ModelTag = TEXT("Prototype_Component_Model_Rabbit");
	pLandObjDesc->ProtoTypeTag = TEXT("Prototype_GameObject_Monster");
	XMStoreFloat4x4(&pLandObjDesc->vPrePosition,XMMatrixIdentity());

		if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Monster"), pLandObjDesc)))
			return E_FAIL;

		pLandObjDesc->ModelTag = TEXT("Prototype_Component_Model_Revolver");
		pLandObjDesc->ProtoTypeTag = TEXT("Prototype_GameObject_Monster");
		XMStoreFloat4x4(&pLandObjDesc->vPrePosition, XMMatrixIdentity());

		if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Monster"), pLandObjDesc)))
			return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring& strLayerTag, CLandObject::LANDOBJ_DESC* pLandObjDesc)
{
	pLandObjDesc->ModelTag = TEXT("Prototype_Component_Model_Player");
	pLandObjDesc->ProtoTypeTag = TEXT("Prototype_GameObject_Player");
	
	_matrix PreMatrix = XMMatrixIdentity() * XMMatrixTranslation(20.f, 0.f, 20.f);	
	XMStoreFloat4x4(&pLandObjDesc->vPrePosition, PreMatrix);

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_Player"), pLandObjDesc)))
		return E_FAIL;


	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();


}
