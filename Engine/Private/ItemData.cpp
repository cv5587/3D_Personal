#include "ItemData.h"
#include "Item.h"	



CItemData::CItemData()
{
}

CItemData::CItemData(const CItemData& rhs)
{
}

HRESULT CItemData::Initialize(void* pArg)
{
	CItem::ITEM_DESC* pDesc = static_cast<CItem::ITEM_DESC*>(pArg);

	m_ItemType.emplace_back(pDesc->ItemType[0]);
	m_ItemType.emplace_back(pDesc->ItemType[1]);

	m_iQuantity = pDesc->iQuantity;
	m_ItemName = pDesc->ItemName;
	m_ItemUIName = pDesc->ItemUIName;
	m_ItemInfo = pDesc->ItemInfo;

	m_ProtoTypeTag = pDesc->ProtoTypeTag;
	m_ModelTag = pDesc->ModelTag;

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;

	m_fWeight = pDesc->fWeight;
	m_Durability = pDesc->Durability;
	m_isEquip = pDesc->isEquip;
	return S_OK;
}

CItemData* CItemData::Create(void* pArg)
{
	CItemData* pInstance = new CItemData();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CItemData");
		return nullptr;
	}

	return pInstance;
}

_bool CItemData::Compare_Name(const wstring ItemName)
{
	if (m_ItemName == ItemName)
		return true;
	else
		return false;
}

_bool CItemData::QuestItemFinish()
{
	if (1<m_iQuantity )
	{
		m_iQuantity--;
		return false;
	}
	else if (1 == m_iQuantity)
	{
		return true;
	}

}

_bool CItemData::Compare_Type(_uint TypeIndex)
{
	if (m_ItemType[0] == TypeIndex || m_ItemType[1] == TypeIndex)
		return true;
	else
		return false;
}

void CItemData::Drop_Item(void* pDesc)
{
	CItem::ITEM_DESC* pItemDesc = static_cast<CItem::ITEM_DESC*>(pDesc);

	pItemDesc->ProtoTypeTag = m_ProtoTypeTag;
	pItemDesc->ModelTag = m_ModelTag;
	pItemDesc->fSpeedPerSec = m_fSpeedPerSec;
	pItemDesc->fRotationPerSec = m_fRotationPerSec;
	pItemDesc->Durability = m_Durability;
	pItemDesc->fWeight = m_fWeight;
	pItemDesc->iQuantity = m_iQuantity;
	pItemDesc->ItemName = m_ItemName;
	pItemDesc->ItemType[0] = m_ItemInfo[0];
	pItemDesc->ItemType[1] = m_ItemInfo[1];
	pItemDesc->ItemUIName = m_ItemUIName;
	
}


void CItemData::Make_ItemDataDesc(void* pDesc)
{
	CItem::ITEM_DESC* pItemDesc = static_cast<CItem::ITEM_DESC*>(pDesc);

	pItemDesc->ItemType[0] = m_ItemType[0];
	pItemDesc->ItemType[1] = m_ItemType[1];

	pItemDesc->ItemName = m_ItemName;
	pItemDesc->ItemUIName = m_ItemUIName;
	pItemDesc->ItemInfo = m_ItemInfo;
	pItemDesc->iQuantity = m_iQuantity;
	pItemDesc->Durability = m_Durability;
	pItemDesc->fWeight = m_fWeight;
	pItemDesc->isEquip = &m_isEquip;
	pItemDesc->CellIndex = m_iCellIndex;
}

_bool CItemData::Using()
{
	if (1 < m_iQuantity)
	{
		m_iQuantity-=1;
		return false;
	}
	else
	{
		return true;
	}

}

void CItemData::Free()
{
	m_ItemType.clear();
}
