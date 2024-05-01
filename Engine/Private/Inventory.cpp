#include "Inventory.h"
#include "ItemData.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Item.h"

CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent{pDevice,pContext}
{
}

CInventory::CInventory(const CInventory& rhs)
	:CComponent{rhs}
{
}

HRESULT CInventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CInventory::Add_Item(CItem* pItem)
{
	wstring wItemName = pItem->Get_ItemName();

	for (auto iter : m_Inventory)
		if(iter->Compare_Name(wItemName))
		{ 
			//아이템 겹치는 거면 갯수증가
			//어차피 묶음 개념이 없음
			iter->Add_Quantity();
			return S_OK;
		}

	CItem::ITEM_DESC Desc{};

	CItemData* pItemData = CItemData::Create(pItem->Get_Description(&Desc));

	m_Inventory.emplace_back(pItemData);

	return S_OK;
}



CItemData* CInventory::find_Item(wstring ItemName)
{
	for (auto& pItem : m_Inventory)
	{
		if (pItem->Compare_Name(ItemName));
		return pItem;
	}


	return nullptr;
}

CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInventory* pInstance = new CInventory(pDevice,pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInventory");
		return nullptr;
	}

	return pInstance;
}

CComponent* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CInventory");
		return nullptr;
	}

	return pInstance;
}

void CInventory::Free()
{
	__super::Free();

	for (auto& pItemData : m_Inventory)
		Safe_Release(pItemData);

	m_Inventory.clear();
}
