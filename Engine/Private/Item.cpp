#include "Item.h"

CItem::CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice,pContext}
{
}

CItem::CItem(const CGameObject& rhs)
	:CGameObject{ rhs }
{
}

HRESULT CItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	ITEM_DESC* pDesc = static_cast<ITEM_DESC*>(pArg);
	m_ItemName = pDesc->ItemName;
	m_Quantity = pDesc->iQuantity;

	m_ItemType.emplace_back((ITEMTYPE)pDesc->ItemType[0]);
	m_ItemType.emplace_back((ITEMTYPE)pDesc->ItemType[1]);

	m_bThrow = pDesc->bThrow;

	m_bisEquip = false;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CItem::Priority_Tick(_float fTimeDelta)
{
}

void CItem::Tick(_float fTimeDelta)
{
}

void CItem::Late_Tick(_float fTimeDelta)
{
}

HRESULT CItem::Render()
{
	return S_OK;
}

_bool CItem::Intersect(CCollider* pTargetCollider)	
{
	if (!m_bThrow)
		return false;

	if (m_pColliderCom->Intersect(pTargetCollider))
	{
		m_pNavigationCom->Set_OnNavigation(m_pTransformCom);
		m_bThrow = false;
		m_iCellIndex = -1;
		m_pNavigationCom->Reset_CellIndex();
		return true;
	}
	else
	{
		return false;
	}
}

_bool CItem::IntersectRay(_vector* pRayArray)
{
	return true;
}

void CItem::Make_Description(void* pArg)
{
	ITEM_DESC* pDesc = static_cast<ITEM_DESC*>(pArg);

	pDesc->ItemType[0] = (_uint)m_ItemType[0];
	pDesc->ItemType[1] = (_uint)m_ItemType[1];

	pDesc->iQuantity = m_Quantity;

	pDesc->ItemName = m_ItemName;

	pDesc->ItemUIName = m_ItemUIName;

	pDesc->ItemInfo = m_ItemInfo;

	pDesc->fWeight = m_fWeight;

	pDesc->Durability = m_Durability;

	pDesc->isEquip = m_bisEquip;	
}

void* CItem::Get_Description(void* pDesc)
{
	

	Make_Description(pDesc);

	__super::Make_Description(pDesc);

	return pDesc;
}

HRESULT CItem::Save_Data(ofstream* fout)
{
	_tchar ProtoTag[MAX_PATH] = TEXT("");
	_tchar ModelTag[MAX_PATH] = TEXT("");

	wsprintf(ProtoTag, m_ProtoTypeTag.c_str());
	wsprintf(ModelTag, m_ModelTag.c_str());

	fout->write((char*)ProtoTag, sizeof(_tchar) * MAX_PATH);
	fout->write((char*)ModelTag, sizeof(_tchar) * MAX_PATH);

	_float4x4 pPosition{};
	XMStoreFloat4x4(&pPosition, m_pTransformCom->Get_WorldMatrix());
	fout->write((char*)&pPosition, sizeof(_float4x4));

	_tchar ItemName[MAX_PATH] = TEXT("");

	wsprintf(ItemName, m_ItemName.c_str());

	fout->write((char*)ItemName, sizeof(_tchar)*MAX_PATH);
	fout->write((char*)&m_Quantity, sizeof(_uint));	
	fout->write((char*)&m_ItemType[0], sizeof(_uint));
	fout->write((char*)&m_ItemType[1], sizeof(_uint));


	return S_OK;
}


void CItem::Free()
{
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	__super::Free();

}
