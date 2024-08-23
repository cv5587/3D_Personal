#include "GEAR.h"

#include "GameInstance.h"
CGEAR::CGEAR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem{pDevice,pContext}
{
}

CGEAR::CGEAR(const CGEAR& rhs)
	:CItem{rhs}
{
}

HRESULT CGEAR::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CGEAR::Initialize(void* pArg)
{

	GEARITEM_DESC* pDesc = static_cast<GEARITEM_DESC*>(pArg);
	if (FAILED(Check_Model(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_bThrow = pDesc->bThrow;
	m_vDirection = pDesc->vDirection;

	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CGEAR::Priority_Tick(_float fTimeDelta)
{
}

void CGEAR::Tick(_float fTimeDelta)
{
	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	if (m_bThrow)
	{

		m_pTransformCom->Throw(&m_vDirection, fTimeDelta);
		if (m_pNavigationCom->Compare_Height(m_pTransformCom))
		{
			m_bThrow = false;
			m_iCellIndex = -1;
			m_pNavigationCom->Reset_CellIndex();
		}
	}
	else
	{
	//	m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	//	m_pNavigationCom->Set_OnNavigation(m_pTransformCom);
	}

	//�̰� �׺񺸴� �������� �׳� ���� �ٰ� �̵��� �������� �Լ� �������ٰ�
}

void CGEAR::Late_Tick(_float fTimeDelta)
{
	if (true == m_pGameInstance->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f))
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
		m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
	}
}

HRESULT CGEAR::Render()
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

//#ifdef _DEBUG
//	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
//#endif

	return S_OK;
}

_bool CGEAR::Intersect(CCollider* pTargetCollider)
{

	if (false==m_bThrow)
		return false;
	else
	{

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
}

_bool CGEAR::IntersectRay(_vector* pRayArray, _float* fDist)
{
	if (m_pColliderCom->IntersectRay(pRayArray, fDist))
		return true;


	return false;
}

_bool CGEAR::RayCollInfo(_vector* pRayArray, CGameObject** pGameObject)
{
	_float fDist = 0.f;
	if (m_pColliderCom->IntersectRay(pRayArray, &fDist))
	{
		if (3 > fDist)
		{
			*pGameObject =this;
			return true;
		}
	}

	return false;
}

HRESULT CGEAR::Check_Model(void* pArg)
{
	CItem::ITEM_DESC* itemDesc = static_cast<CItem::ITEM_DESC*>(pArg);
	wstring modeltag = itemDesc->ModelTag;
	if (modeltag == TEXT("Prototype_Component_Model_Stone"))
	{

		itemDesc->ItemName = TEXT("Stone");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_EQUIPMENT;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 30.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);
		
		m_ItemUIName = TEXT("���� ��");
		m_ItemInfo = TEXT("���� ������ ���ֱ⿡ ����� ������ ũ��. \n ��¼�� �� �� ū �������� ���� �� ���� ����\n �� ����.");
		m_ItemWeight = TEXT("0.15 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.15f;
	}
	else 	if (modeltag == TEXT("Prototype_Component_Model_AntiBioticsBottle"))
	{
		itemDesc->ItemName = TEXT("AntiBioticsBottle");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_MEDKITS;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 30.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("�׻���");
		m_ItemInfo = TEXT("�׻��� ���Դϴ�. ������ ������ ������ ��\n ���� �� �ֽ��ϴ�.");
		m_ItemWeight = TEXT("0.6 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.6f;
	}
	else 	if (modeltag == TEXT("Prototype_Component_Model_BeefJerky"))
	{

		itemDesc->ItemName = TEXT("BeefJerky");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_FOOD;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 30.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("�Ұ�� ����");
		m_ItemInfo = TEXT("�Ұ�⸦ ���� ��������ϴ�.\n");
		m_ItemWeight = TEXT("0.1 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.1f;
	}
	else 	if (modeltag == TEXT("Prototype_Component_Model_RawRabbit"))
	{

		itemDesc->ItemName = TEXT("RawRabbit");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_FOOD;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 30.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("�� �䳢���");
		m_ItemInfo = TEXT("�䳢�� ������ ���� ����Դϴ�.\n");
		m_ItemWeight = TEXT("0.05 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.05f;
	}
	else 	if (modeltag == TEXT("Prototype_Component_Model_CookedRabbit"))
	{

		itemDesc->ItemName = TEXT("CookedRabbit");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_FOOD;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 30.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("���� �䳢���");
		m_ItemInfo = TEXT("�䳢�� ������ ���� ��⸦ ��������� �Դϴ�.\n");
		m_ItemWeight = TEXT("0.05 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.05f;
	}
	else 	if (modeltag == TEXT("Prototype_Component_Model_Rut"))
	{

		itemDesc->ItemName = TEXT("Rut");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_FOOD;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 30.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT(" ����");
		m_ItemInfo = TEXT("�䳢�� ������ ���� ���� �Դϴ�..\n");
		m_ItemWeight = TEXT("0.05 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.05f;
	}
	else 	if (modeltag == TEXT("Prototype_Component_Model_Pelt"))
	{

		itemDesc->ItemName = TEXT("Pelt");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_STUFF;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 30.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT(" ����");
		m_ItemInfo = TEXT("�䳢�� ������ ���� ���� �Դϴ�..\n");
		m_ItemWeight = TEXT("0.05 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.05f;
		}
	else 	if (modeltag == TEXT("Prototype_Component_Model_Matchbox"))
	{

		itemDesc->ItemName = TEXT("Matchbox");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_KINDLING;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 10;
		itemDesc->fSpeedPerSec = 30.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT(" ����");
		m_ItemInfo = TEXT("�ƹ����� ������ ������ ���� �����Դϴ�.");
		m_ItemWeight = TEXT("0.10 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.10f;
		}
	else 	if (modeltag == TEXT("Prototype_Component_Model_Stick"))
	{

		itemDesc->ItemName = TEXT("Stick");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_STUFF;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 3;
		itemDesc->fSpeedPerSec = 30.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT(" ���� ����");
		m_ItemInfo = TEXT("���� ���������� �ҿ� Ÿ���� ���� ���� �ʽ��ϴ�.");
		m_ItemWeight = TEXT("0.10 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.10f;
		}
	else 	if (modeltag == TEXT("Prototype_Component_Model_Hardwood"))
	{

		itemDesc->ItemName = TEXT("Hardwood");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_STUFF;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 30.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT(" ������ ����");
		m_ItemInfo = TEXT("�߸��� �ƴմϴ�. �߸� �������� �� ž�ϴ�.");
		m_ItemWeight = TEXT("1.00 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 1.00f;
		}
	else 	if (modeltag == TEXT("Prototype_Component_Model_Flare"))
	{

		itemDesc->ItemName = TEXT("Flare");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_EQUIPMENT;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 2.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("��ȣź");
		m_ItemInfo = TEXT("��ȣź �Դϴ�. Ű�� ���� �����ϴ�.");
		m_ItemWeight = TEXT("1.00 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 1.00f;
		}
	//���� = �ҽ�ð� , ���
	
	return S_OK;
}


HRESULT CGEAR::Add_Components()
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
	CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc={};

	ColliderDesc.eType = CCollider::TYPE_AABB;
	ColliderDesc.vExtents = _float3(0.2f, 0.2f, 0.2f);//aabb ��������
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y*0.5f, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIGATION_DESC	NavigationDesc={};

	NavigationDesc.iCurrentCellIndex = 0;	

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGEAR::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;


	return S_OK;
}

CGEAR* CGEAR::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGEAR* pInstance = new CGEAR(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : GEARItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGEAR::Clone(void* pArg)
{
	CGEAR* pInstance = new CGEAR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : GEARItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGEAR::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
