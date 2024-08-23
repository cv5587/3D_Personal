#include "BoxProp.h"
#include "GameInstance.h"
#include "ItemData.h"
#include "Item.h"
CBoxProp::CBoxProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CInteractiveObject{ pDevice,pContext }
{
}

CBoxProp::CBoxProp(const CBoxProp& rhs)
	:CInteractiveObject(rhs)
{
}

HRESULT CBoxProp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoxProp::Initialize(void* pArg)
{
	CBoxProp::BOXPROP_DESC* pDesc = static_cast<BOXPROP_DESC*>(pArg);

	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

	if (FAILED(Add_Components()))
		return E_FAIL;

	Make_Item();
	return S_OK;
}

void CBoxProp::Priority_Tick(_float fTimeDelta)
{
}

void CBoxProp::Tick(_float fTimeDelta)
{
	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CBoxProp::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

}

HRESULT CBoxProp::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}



	return S_OK;
}

_bool CBoxProp::IntersectRay(_vector* pRayArray, _float* fDist)
{
	if (m_pColliderCom->IntersectRay(pRayArray, fDist))
	{
		return true;
	}

	return false;
}

HRESULT CBoxProp::Action()
{

		CGameObject* pPlayer = m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"));

		dynamic_cast<CPlayer*>(pPlayer)->Selector_In(m_InItem[0]);
		dynamic_cast<CPlayer*>(pPlayer)->Set_State(PLAYERSTATE::PLAYER_PICKUP);
		m_pGameInstance->Play_Sound(TEXT("OpenToolKit.wav"), OPEN, 1.0f);

//아이템 인벤토리에 넣어주기

	return S_OK;
}

_bool CBoxProp::RayCollInfo(_vector* pRayArray, CGameObject** pGameObject)
{
	_float fDist = 0.f;
	if (m_pColliderCom->IntersectRay(pRayArray, &fDist))
	{
		if (3 > fDist)
		{
			*pGameObject = this;
			return true;
		}
	}

	return false;
}

HRESULT CBoxProp::Make_Item()
{
	CItem::ITEM_DESC ItemDesc={};

	if (m_ModelTag == TEXT("Prototype_Component_Model_ToolBox"))
	{
		m_ItemUIName = TEXT("공구 상자");
		ItemDesc.ItemName = TEXT("BeefJerky");
		ItemDesc.ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_FOOD;
		ItemDesc.ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		ItemDesc.iQuantity = 1;
		ItemDesc.fSpeedPerSec = 0.f;
		ItemDesc.fRotationPerSec = XMConvertToRadians(0.f);

		ItemDesc.ProtoTypeTag = TEXT("Prototype_GameObject_GEAR");;
		ItemDesc.ModelTag = TEXT("Prototype_Component_Model_BeefJerky");
		XMStoreFloat4x4(&(ItemDesc.vPrePosition), XMMatrixIdentity());
		
		CGameObject* pItem = m_pGameInstance->Clone_Object(ItemDesc.ProtoTypeTag, &ItemDesc);
		m_InItem.push_back(pItem);	
	}
	else if (m_ModelTag == TEXT("Prototype_Component_Model_MedKit"))
	{
		m_ItemUIName = TEXT("구급 상자");
		ItemDesc.ItemName = TEXT("AntiBiotics");
		ItemDesc.ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_MEDKITS;
		ItemDesc.ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		ItemDesc.iQuantity = 1;
		ItemDesc.fSpeedPerSec = 0.f;
		ItemDesc.fRotationPerSec = XMConvertToRadians(0.f);


		ItemDesc.ProtoTypeTag = TEXT("Prototype_GameObject_GEAR");;
		ItemDesc.ModelTag = TEXT("Prototype_Component_Model_AntiBioticsBottle");
		XMStoreFloat4x4(&(ItemDesc.vPrePosition), XMMatrixIdentity());

		CGameObject* pItem = m_pGameInstance->Clone_Object(ItemDesc.ProtoTypeTag, &ItemDesc);
		m_InItem.push_back(pItem);

	}
	return S_OK;
}

HRESULT CBoxProp::Add_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};

	ColliderDesc.eType = CCollider::TYPE_AABB;
	ColliderDesc.vExtents = _float3(0.2f, 0.2f, 0.2f);//aabb 조절가능
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y * 0.5f, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBoxProp::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CBoxProp* CBoxProp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoxProp* pInstance = new CBoxProp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoxProp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoxProp::Clone(void* pArg)
{
	CBoxProp* pInstance = new CBoxProp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoxProp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoxProp::Free()
{
	__super::Free();

	for (auto& pGameObject : m_InItem)
		Safe_Release(pGameObject);

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
