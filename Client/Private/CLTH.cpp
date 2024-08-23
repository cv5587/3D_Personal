#include "CLTH.h"	
#include "GameInstance.h"

CCLTH::CCLTH(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem{ pDevice,pContext }
{
}

CCLTH::CCLTH(const CCLTH& rhs)
	:CItem{ rhs }
{
}

HRESULT CCLTH::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CCLTH::Initialize(void* pArg)
{

	CLTHITEM_DESC* pDesc = static_cast<CLTHITEM_DESC*>(pArg);

	if (FAILED(Check_Model(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CCLTH::Priority_Tick(_float fTimeDelta)
{
}

void CCLTH::Tick(_float fTimeDelta)
{
	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CCLTH::Late_Tick(_float fTimeDelta)
{
	if (true == m_pGameInstance->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f))
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
		m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
	}
}

HRESULT CCLTH::Render()
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

HRESULT CCLTH::Check_Model(void* pArg)
{
	CCLTH::CLTHITEM_DESC* itemDesc = static_cast<CCLTH::CLTHITEM_DESC*>(pArg);
	wstring modeltag=itemDesc->ModelTag;
	if (modeltag == TEXT("Prototype_Component_Model_ToqueWool"))
	{
		itemDesc->ItemName = TEXT("ToqueWool");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_CLOTH;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 2.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("���� ����");
		m_ItemInfo = TEXT("������ ��Ʈ �� �����Դϴ�. ���и�ŭ ���������� \n������ �ƹ��͵� ���� �� ���� �����ϴ�.");
		m_ItemWeight = TEXT("0.10 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.1f;
	}
	else if (modeltag == TEXT("Prototype_Component_Model_Cargopants"))
	{
		itemDesc->ItemName = TEXT("Cargopants");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_CLOTH;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 2.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("ī�� ����");
		m_ItemInfo = TEXT("�κ��� ������ �������Դϴ�. ��Ģ�� �ɸ�\n�� �����ϴ�.");
		m_ItemWeight = TEXT("0.75 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.75f;
	}
	else if (modeltag == TEXT("Prototype_Component_Model_Boots"))
	{
		itemDesc->ItemName = TEXT("Boots");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_CLOTH;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 2.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("�������� ����");
		m_ItemInfo = TEXT("����� �߶� ��� ��� �Ű� �ִ� �����Դϴ�.");
		m_ItemWeight = TEXT("1.00 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 1.0f;
	}
	else if (modeltag == TEXT("Prototype_Component_Model_Sweater"))
	{
		itemDesc->ItemName = TEXT("Sweater");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_CLOTH;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 2.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("�������� ������");
		m_ItemInfo = TEXT("�ܵ����� �Ծ ���������� �ܿ� ��Ʈ �ȿ�\n ���̾���ص� ���ƿ�.");
		m_ItemWeight = TEXT("0.25 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.25f;
	}
	return S_OK;
}

_bool CCLTH::Intersect(CCollider* pTargetCollider)
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

_bool CCLTH::IntersectRay(_vector* pRayArray, _float* fDist)
{
	if (m_pColliderCom->IntersectRay(pRayArray, fDist))
		return true;


	return false;
}

_bool CCLTH::RayCollInfo(_vector* pRayArray, CGameObject** pGameObject)
{
	_float fDist = 0.f;
	if (m_pColliderCom->IntersectRay(pRayArray, &fDist))
	{
		if (3.f > fDist)
		{
			*pGameObject = this;
			return true;
		}
	}

	return false;
}

HRESULT CCLTH::Add_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	//���� �ε��� ���̴��� �ٸ��� ��� ��� ���� ������.
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc = {};

	ColliderDesc.eType = CCollider::TYPE_AABB;
	ColliderDesc.vExtents = _float3(0.3f, 0.3f, 0.3f);//aabb ��������
	ColliderDesc.vCenter = _float3(0.f,0.f, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCLTH::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CCLTH* CCLTH::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	CCLTH* pInstance = new CCLTH(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCLTH");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCLTH::Clone(void* pArg)
{
	CCLTH* pInstance = new CCLTH(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CCLTH");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCLTH::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
}
