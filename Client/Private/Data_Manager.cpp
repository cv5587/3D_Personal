#include "Data_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "TerrainManager.h"
#include "LandObject.h"	
#include "FreeCamera.h"
#include "Level_Loading.h"

#include "CLTH.h"
#include "GEAR.h"
#include "Monster.h"
#include "Portal.h"
CData_Manager::CData_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance{CGameInstance::GetInstance()}
	, m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pGameInstance);	
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

HRESULT CData_Manager::Load_Data(_uint iLevelIndex)
{

	char FileRoute[MAX_PATH] = "../Bin/bin/Save_Data/";
	char FilePath[MAX_PATH] = "";
	char FileDat[MAX_PATH] = ".dat";

	m_CurrentLevel = iLevelIndex;

	switch (iLevelIndex)
	{
	case 0:
		strcpy_s(FilePath, "LEVEL_STATIC_GAMEDATA");
		break;
	case 1:
		strcpy_s(FilePath, "LEVEL_LOADING_GAMEDATA");
		break;
	case 2:
		strcpy_s(FilePath, "LEVEL_LOGO_GAMEDATA");
		break;
	case 3:
		strcpy_s(FilePath, "LEVEL_GAMEPLAY_GAMEDATA");
		break;

	default:
		return E_FAIL;
	}

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
				else if (TEXT("Layer_InteractiveObject") == wLayer)
				{
					CPortal::PORTAL_DESC PortalDesc{};
					PortalDesc.ProtoTypeTag = strPrototypeTag;
					PortalDesc.ModelTag = strModelTag;
					PortalDesc.vPrePosition = fWorldPosition;

					fin.read((char*)&PortalDesc.iGoalCellIndex, sizeof(_int));
					fin.read((char*)&PortalDesc.vGoalPosition, sizeof(_float4));

					if (FAILED(m_pGameInstance->Add_CloneObject(iReadLevel, wLayer, strPrototypeTag, &PortalDesc)))
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

HRESULT CData_Manager::Initialize()
{
	m_pTerrainMgr = CTerrainManager::Create();

	return S_OK;
}

CData_Manager* CData_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CData_Manager* pInstance = new CData_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);
		MSG_BOX("Data_Manager Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CData_Manager::Free()
{

	Safe_Release(m_pTerrainMgr);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
