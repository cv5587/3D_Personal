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

	m_fTotalWeight += Desc.fWeight;

	return S_OK;
}

HRESULT CInventory::Add_Rabbit()
{

	CItem::ITEM_DESC Desc{};
	Desc.Durability = 100.f;
	Desc.fRotationPerSec = XMConvertToRadians(120.f);
	Desc.fSpeedPerSec = 2.f;
	Desc.fWeight = 2.48f;
	Desc.iQuantity = 1;
	Desc.ItemInfo = TEXT("방치된 토끼 시체. 채집하여 고기, 가죽 또는 \n내장을 사용할 수 있습니다.");
	Desc.ItemName = TEXT("Rabbit");
	Desc.ItemType[0] = 3;
	Desc.ItemType[1] = 6;
	Desc.ItemUIName = TEXT("토끼 시체");
	Desc.ModelTag = TEXT("Prototype_Component_Model_Rabbit");
	Desc.ProtoTypeTag = TEXT("Prototype_GameObject_Monster");

	CItemData* pItemData = CItemData::Create(&Desc);

	m_Inventory.emplace_back(pItemData);

	m_fTotalWeight += Desc.fWeight;

	return S_OK;
}

HRESULT CInventory::Drop_Item(wstring ItemName, void* pDesc)
{
	auto& iterater = m_Inventory.begin();
	for (auto iter : m_Inventory)
	{
		if (iter->Compare_Name(ItemName))
		{
			//아이템 겹치는 거면 갯수증가
			//어차피 묶음 개념이 없음
			iter->Drop_Item(pDesc);
			Safe_Release(iter);
			m_Inventory.erase(iterater);
			return S_OK;
		}
		iterater++;
	}
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
