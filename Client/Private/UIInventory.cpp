#include "UIInventory.h"

#include "GameInstance.h"
#include "Item.h"
#include "UImanager.h"
#include "Player.h"

CUIInventory::CUIInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance{CGameInstance::GetInstance()},
	m_pDevice{pDevice},
	m_pContext{pContext}
{
	Safe_AddRef( m_pGameInstance );
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CUIInventory::Initialize(void* pArg)
{
	INVENTORY_DESC* pDesc = static_cast<INVENTORY_DESC*>(pArg);

	m_pPlayerInventory = static_cast<CInventory*>(pDesc->PlayerInventory);
	Safe_AddRef(m_pPlayerInventory);

	m_pUImanager = static_cast<CUImanager*>(pDesc->UImanager);
	Safe_AddRef(m_pUImanager);

	return S_OK;
}

HRESULT CUIInventory::Tick(_float fTimeDelta)
{
	m_pUImanager->Set_Weight(m_pPlayerInventory->Get_TotalWeight());
	Cloth_Update();
	switch (m_CurrentScene)
	{
	case Client::CUIInventory::SCENE_STATE:
		break;
	case Client::CUIInventory::SCENE_EQUIP:
		break;
	case Client::CUIInventory::SCENE_BAG:
		break;
	case Client::CUIInventory::SCENE_BLUEPRINT:
	{
		m_pUImanager->BP_Update(fTimeDelta);

	}
		break;
	case Client::CUIInventory::SCENE_END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CUIInventory::Late_Tick(_float fTimeDelta)
{
	m_pUImanager->Render_UI(CUImanager::Layer_InvenBack);
	m_pUImanager->Render_UI(CUImanager::Layer_Toggle);
	m_pUImanager->Render_UI(CUImanager::Layer_NameText);
	m_pUImanager->Render_UI(CUImanager::Layer_InvenButton);
	switch (m_CurrentScene)
	{

	case Client::CUIInventory::SCENE_STATE:
		m_pUImanager->Render_UI(CUImanager::Layer_StateBody);
		m_pUImanager->Render_Cloth();
		m_pUImanager->Render_UI(CUImanager::Layer_StateInfoback);
		m_pUImanager->Render_UI(CUImanager::Layer_StateInfo);
		m_pUImanager->Render_UI(CUImanager::Layer_StateInfoText);
		m_pUImanager->Render_UI(CUImanager::Layer_Stateback);

		break;
	case Client::CUIInventory::SCENE_EQUIP:
		m_pUImanager->Render_UI(CUImanager::Layer_StateBody);
		m_pUImanager->Render_Cloth();
		m_pUImanager->Render_UI(CUImanager::Layer_ClothCase);
		m_pUImanager->Render_UI(CUImanager::Layer_ClothText);
		m_pUImanager->Render_UI(CUImanager::Layer_ClothToggle);

		break;
	case Client::CUIInventory::SCENE_BAG:
		m_pUImanager->Render_UI(CUImanager::Layer_InvenCase);
		m_pUImanager->Render_UI(CUImanager::Layer_SortBase);
		m_pUImanager->Render_UI(CUImanager::Layer_SortIcon);
		m_pUImanager->Render_UI(CUImanager::Layer_InvenText);

		m_pUImanager->Render_UI(CUImanager::Layer_Item);
		break;
	case Client::CUIInventory::SCENE_BLUEPRINT:
		m_pUImanager->Render_UI(CUImanager::Layer_BluePrint);
		break;
	case Client::CUIInventory::SCENE_END:
		break;
	default:
		break;
	}

	return S_OK;
}


//토글전용
void CUIInventory::Set_CurrentScene(_uint iSceneIndex)
{
	m_CurrentScene = (SCENE)iSceneIndex;
	m_pUImanager->Scene_Change(iSceneIndex);
	Set_SortMode(SORT_ALL);
}

void CUIInventory::Set_SortMode(_uint iSortIndex)
{
	m_pUImanager->Set_SortMode(iSortIndex);
	m_pUImanager->Sort_Reset();	
	vector<class CItemData*> Inventory =m_pPlayerInventory->Get_vecItemData();

	if (SORT_ALL == iSortIndex)
	{
		_uint uIconIndex = { 0 };
		for (size_t i = 0; i < Inventory.size(); i++)
		{
			m_pUImanager->Sort_InvenIcon(uIconIndex, Inventory[i]);
			++uIconIndex;
			
		}
	}
	else
	{
		_uint uIconIndex = { 0 };
		for (size_t i = 0; i < Inventory.size(); i++)
		{
			if (Inventory[i]->Compare_Type(iSortIndex-1))
			{
				m_pUImanager->Sort_InvenIcon(uIconIndex, Inventory[i]);
				++uIconIndex;
			}
		}
	}
	
}

void CUIInventory::Refresh()
{
	m_pUImanager->Sort_Reset();	
	vector<class CItemData*> Inventory = m_pPlayerInventory->Get_vecItemData();

	_uint iSortIndex = m_pUImanager->Get_SortMode();

	if (SORT_ALL == iSortIndex)
	{
		_uint uIconIndex = { 0 };
		for (size_t i = 0; i < Inventory.size(); i++)
		{
			m_pUImanager->Sort_InvenIcon(uIconIndex, Inventory[i]);
			++uIconIndex;

		}
	}
	else
	{
		_uint uIconIndex = { 0 };
		for (size_t i = 0; i < Inventory.size(); i++)
		{
			if (Inventory[i]->Compare_Type(iSortIndex - 1))
			{
				m_pUImanager->Sort_InvenIcon(uIconIndex, Inventory[i]);
				++uIconIndex;
			}
		}
	}
}

void CUIInventory::Pick_UIToggle()
{

	_uint UIID = m_pGameInstance->Picking_UIIDScreen();


	if (UIID < SCENE_END)
	{
		_uint SceneIndex = m_pUImanager->Pick_UISceneToggle(UIID);

		if (-1 != SceneIndex && SCENE_END > SceneIndex)
			Set_CurrentScene(SceneIndex);
	}
	else if(UIID< SCENE_END+ SORT_END)
	{
		_uint SortIndex = m_pUImanager->Pick_UISortToggle(UIID);

		if (-1 != SortIndex && SORT_END > SortIndex)
			Set_SortMode(SortIndex);
	}
	else if(UIID < SCENE_END + SORT_END+ INVENBUTTON_END)
	{
			dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")))
				->Set_State(PLAYER_IDLE);
	}
	else if (UIID < SCENE_END + SORT_END + INVENBUTTON_END + 20)
	{
		m_pUImanager->Pick_InvenItem(UIID);
	}
	else if (UIID < SCENE_END + SORT_END + INVENBUTTON_END + 20+16)
	{
		m_pUImanager->Cloth_Reset();
		m_pUImanager->Pick_ClothItem(UIID);//옷창 선택
	}

	if (m_pUImanager->Pick_InvenInfoButton())
		Refresh();

	m_pUImanager->Pick_InvenClothButton();

	m_pUImanager->Pick_InvenBPButton();

}

void CUIInventory::Cloth_Update()
{
	vector<class CItemData*> Inventory = m_pPlayerInventory->Get_vecItemData();
	
	for (auto& iter : Inventory)
	{
		if(iter->Compare_Type(CItem::ITEM_CLOTH))
		{
			m_pUImanager->Cloth_Update(iter);
			if (iter->Compare_Name(TEXT("Sweater")))
			{
				if (iter->Get_isEquip())
				{
					dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")))->Set_Cloth(1);
				}
				else
				{
					dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")))->Set_Cloth(0);

				}
			}
		}
	}

}

CUIInventory* CUIInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg)
{
	CUIInventory* pInstance = new CUIInventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CUIInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIInventory::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pPlayerInventory);
	Safe_Release(m_pUImanager);
}
