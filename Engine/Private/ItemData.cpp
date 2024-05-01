#include "ItemData.h"
#include "Item.h"	
#include"GameObject.h"

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

	m_ProtoTypeTag = pDesc->ProtoTypeTag;
	m_ModelTag = pDesc->ModelTag;

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;

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

void CItemData::Free()
{
}
