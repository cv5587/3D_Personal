#include "stdafx.h"
#include "Level_GamePlay.h"
#include "GameInstance.h"

#include "LandObject.h"	

#include "TerrainManager.h"
#include "GEAR.h"	
#include "CLTH.h"
#include "Monster.h"
#include"Data_Manager.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CData_Manager* pDataManager)
	: CLevel{	pDevice, pContext}
	, m_pDataManager{ pDataManager }
{
	Safe_AddRef(m_pDataManager);
}

HRESULT CLevel_GamePlay::Initialize()
{

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(m_pDataManager->Load_Data(LEVEL_GAMEPLAY)))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이레벨임"));
#endif
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	m_pGameInstance->Add_Light(LightDesc);


	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(20.f, 5.f, 20.f, 1.f);
	LightDesc.fRange = 20.f;
	LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	m_pGameInstance->Add_Light(LightDesc);

	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(40.f, 5.f, 20.f, 1.f);
	LightDesc.fRange = 20.f;
	LightDesc.vDiffuse = _float4(0.0f, 1.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.4f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	m_pGameInstance->Add_Light(LightDesc);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring& strLayerTag)
{
	/*
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

HRESULT CLevel_GamePlay::Load_GameData()
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
		_tchar szItemName[MAX_PATH] = TEXT("");
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

					//m_pTerrainMgr->Clone_Terrain(iTerrain);
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
				else if (TEXT("Layer_Player") == wLayer)
				{
					_int CellIndex = { -1 };
					fin.read((char*)&CellIndex, sizeof(_int));

					CLandObject::LANDOBJ_DESC		LandObjDesc{};

					LandObjDesc.ProtoTypeTag = strPrototypeTag;
					LandObjDesc.ModelTag = strModelTag;
					LandObjDesc.vPrePosition = fWorldPosition;
					LandObjDesc.CellIndex = CellIndex;
					if (FAILED(m_pGameInstance->Add_CloneObject(iReadLevel, wLayer, strPrototypeTag, &LandObjDesc)))
						return E_FAIL;
				}
				else if (TEXT("Layer_Item") == wLayer)
				{
					if (TEXT("Prototype_GameObject_GEAR") == strPrototypeTag)
					{
						CGEAR::GEARITEM_DESC GEARItemDESC{};
						GEARItemDESC.ProtoTypeTag = strPrototypeTag;
						GEARItemDESC.ModelTag = strModelTag;
						GEARItemDESC.vPrePosition = fWorldPosition;

						_tchar ItemName[MAX_PATH] = TEXT("");
						fin.read((char*)ItemName, sizeof(_tchar) * MAX_PATH);
						wstring wItemName(ItemName);

						GEARItemDESC.ItemName = wItemName;
						fin.read((char*)&GEARItemDESC.iQuantity, sizeof(_uint));
						fin.read((char*)&GEARItemDESC.ItemType[0], sizeof(_uint));
						fin.read((char*)&GEARItemDESC.ItemType[1], sizeof(_uint));

						if (FAILED(m_pGameInstance->Add_CloneObject(iReadLevel, wLayer, strPrototypeTag, &GEARItemDESC)))
							return E_FAIL;
					}
					else if (TEXT("Prototype_GameObject_CLTH") == strPrototypeTag)
					{
						CCLTH::CLTHITEM_DESC CLTHItemDESC{};
						CLTHItemDESC.ProtoTypeTag = strPrototypeTag;
						CLTHItemDESC.ModelTag = strModelTag;
						CLTHItemDESC.vPrePosition = fWorldPosition;

						_tchar ItemName[MAX_PATH] = TEXT("");
						fin.read((char*)ItemName, sizeof(_tchar) * MAX_PATH);
						wstring wItemName(ItemName);

						CLTHItemDESC.ItemName = wItemName;
						fin.read((char*)&CLTHItemDESC.iQuantity, sizeof(_uint));
						fin.read((char*)&CLTHItemDESC.ItemType[0], sizeof(_uint));
						fin.read((char*)&CLTHItemDESC.ItemType[1], sizeof(_uint));

						if (FAILED(m_pGameInstance->Add_CloneObject(iReadLevel, wLayer, strPrototypeTag, &CLTHItemDESC)))
							return E_FAIL;
					}
				}
				else if (TEXT("Layer_Monster") == wLayer)
				{
					_int CellIndex = { -1 };
					fin.read((char*)&CellIndex, sizeof(_int));

					CMonster::MOSTER_DESC		MOSTER_DESC{};

					MOSTER_DESC.ProtoTypeTag = strPrototypeTag;
					MOSTER_DESC.ModelTag = strModelTag;
					MOSTER_DESC.vPrePosition = fWorldPosition;
					MOSTER_DESC.CellIndex = CellIndex;
					if (FAILED(m_pGameInstance->Add_CloneObject(iReadLevel, wLayer, strPrototypeTag, &MOSTER_DESC)))
						return E_FAIL;
				}
			}

		}
	}

	fin.close();


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

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CData_Manager* pDataManager)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext, pDataManager);

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
	Safe_Release(m_pDataManager);

}
