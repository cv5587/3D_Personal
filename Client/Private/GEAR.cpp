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

	//이거 네비보다 낮아지면 그냥 땅에 붙고 이동이 없어지는 함수 제작해줄것
	//m_pNavigationCom->Set_OnNavigation(m_pTransformCom);
}

void CGEAR::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CGEAR::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;



	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pColliderCom->Render();

#endif

	return S_OK;
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
		
		m_ItemUIName = TEXT("작은 돌");
		m_ItemInfo = TEXT("작은 동물을 겁주기에 충분할 정도로 크다. \n 어쩌면 좀 더 큰 동물에게 겁을 줄 수도 있을\n 것 같다.");
		m_ItemWeight = TEXT("0.15 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.15f;
	}
	//나무 = 불쏘시개 , 재료
	
	return S_OK;
}


HRESULT CGEAR::Add_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	//만약 부들이 쉐이더가 다르면 모두 양면 으로 해주자.
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshID"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};

	ColliderDesc.eType = CCollider::TYPE_AABB;
	ColliderDesc.vExtents = _float3(0.1f, 0.1f, 0.1f);//aabb 조절가능
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y*0.5f, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIGATION_DESC	NavigationDesc{};

	NavigationDesc.iCurrentCellIndex = m_iCellIndex;	

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
	if (FAILED(m_pShaderCom->Bind_ID("g_ID", m_iRenderID)))
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
