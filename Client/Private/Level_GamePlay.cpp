#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"

#include "FreeCamera.h"
#include "LandObject.h"	

#include "TerrainManager.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_pTerrainMgr = CTerrainManager::Create();



	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Environment(TEXT("Layer_EnvironmentObject"))))
	//	return E_FAIL;
	if (FAILED(Load_GameData(TEXT("LEVEL_GAMEPLAY_GAMEDATA"))))
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
{/*
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
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring& strLayerTag)
{
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

	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"), &LandObjDesc)))
	//	return E_FAIL;

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

HRESULT CLevel_GamePlay::Load_GameData(const wstring& strLayerTag)
{

	char FileRoute[MAX_PATH] = "../Bin/bin/Save_Data/";
	char FilePath[MAX_PATH] = "LEVEL_GAMEPLAY_GAMEDATA";
	char FileDat[MAX_PATH] = ".dat";

	strcat_s(FileRoute, FilePath);
	strcat_s(FileRoute, FileDat);



	ifstream fin;
	fin.open(FileRoute, ios::out | ios::binary);

	if (fin.is_open())
	{
		_uint iReadLevel = 0;
		fin.read((char*)&iReadLevel, sizeof(_uint));

		_tchar Layer[MAX_PATH] = TEXT("");
		_tchar szProtoTag[MAX_PATH] = TEXT("");
		_tchar szModelTag[MAX_PATH] = TEXT("");
		_float4x4 fWorldPosition{};

		_uint Layersize = 0;
		fin.read((char*)&Layersize, sizeof(_uint));
		for (_uint i = 0; i < Layersize; i++)
		{

			fin.read((char*)&Layer, sizeof(_tchar) * MAX_PATH);


			_uint iObjectSize = 0;
			fin.read((char*)&iObjectSize, sizeof(_uint));

			for (_uint i = 0; i < iObjectSize; i++)
			{
				fin.read((char*)szProtoTag, sizeof(_tchar) * MAX_PATH);
				fin.read((char*)szModelTag, sizeof(_tchar) * MAX_PATH);
				fin.read((char*)&fWorldPosition, sizeof(_float4x4));


				wstring wLayer(Layer);
				wstring strPrototypeTag(szProtoTag);
				wstring strModelTag(szModelTag);

				if (TEXT("Layer_BackGround") == wLayer)
				{
					_int iTerrain[2] = { 0,0 };
					fin.read((char*)iTerrain, sizeof(_int) * 2);

					m_pTerrainMgr->Clone_Terrain(iTerrain);
				}
				else if (TEXT("Layer_EnvironmentObject") == wLayer)
				{
					CGameObject::GAMEOBJECT_DESC pDesc{};
					pDesc.ProtoTypeTag = strPrototypeTag;
					pDesc.ModelTag = strModelTag;
					pDesc.vPrePosition = fWorldPosition;

					if (FAILED(m_pGameInstance->Add_CloneObject(iReadLevel, wLayer, strPrototypeTag, &pDesc)))
						return E_FAIL;
				}
				else//몬스터 ,플레이어 설정 (몬스터는 터레인만 추가, 플레이어는 터레인,파츠(고정값이므로 작업필요 ㄴ))
				{
					CLandObject::LANDOBJ_DESC		LandObjDesc{};

					LandObjDesc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform")));
					LandObjDesc.pTerrainVIBuffer = dynamic_cast<CVIBuffer*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));
					LandObjDesc.ProtoTypeTag = strPrototypeTag;
					LandObjDesc.ModelTag = strModelTag;
					LandObjDesc.vPrePosition = fWorldPosition;

					if (FAILED(m_pGameInstance->Add_CloneObject(iReadLevel, wLayer, strPrototypeTag, &LandObjDesc)))
						return E_FAIL;
				}
			}

		}
	}

	fin.close();

	////툴에서는 카메라를 제작해주지만 인게임으로 넘어가서는 플레이어 생성시 카메라 자동생성으로 제작하기.
	/*CFreeCamera::FREE_CAMERA_DESC		CameraDesc{};

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
	CameraDesc.ProtoTypeTag = TEXT("Prototype_GameObject_FreeCamera");
	CameraDesc.ModelTag = TEXT("");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_FreeCamera"), &CameraDesc)))
		return E_FAIL;*/


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring& strLayerTag, CLandObject::LANDOBJ_DESC* pLandObjDesc)
{
	pLandObjDesc->ModelTag = TEXT("Prototype_Component_Model_Player");
	pLandObjDesc->ProtoTypeTag = TEXT("Prototype_GameObject_Player");
	
	_matrix PreMatrix = XMMatrixIdentity(); /**XMMatrixTranslation(20.f, 0.f, 20.f);*/
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
	Safe_Release(m_pTerrainMgr);

}
