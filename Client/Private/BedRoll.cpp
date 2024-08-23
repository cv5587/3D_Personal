#include "BedRoll.h"
#include "GameInstance.h"
CBedRoll::CBedRoll(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CInteractiveObject{ pDevice,pContext }
{
}

CBedRoll::CBedRoll(const CBedRoll& rhs)
	:CInteractiveObject(rhs)
{
}

HRESULT CBedRoll::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBedRoll::Initialize(void* pArg)
{

	BEDROLL_DESC* pDesc = static_cast<BEDROLL_DESC*>(pArg);
	m_ItemUIName = TEXT("침낭");
	m_iShaderPass = pDesc->iShaderPass;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

	if (FAILED(Add_Components()))
		return E_FAIL;


	return S_OK;
}

void CBedRoll::Priority_Tick(_float fTimeDelta)
{
}

void CBedRoll::Tick(_float fTimeDelta)
{
	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

}

void CBedRoll::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CBedRoll::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iShaderPass);

		m_pModelCom->Render(i);
	}



	return S_OK;
}

_bool CBedRoll::IntersectRay(_vector* pRayArray, _float* fDist)
{
	if (m_pColliderCom->IntersectRay(pRayArray, fDist))
	{
		return true;
	}

	return false;
}

HRESULT CBedRoll::Action()
{


	CGameObject* pPlayer = m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"));

	dynamic_cast<CPlayer*>(pPlayer)->Set_State(PLAYERSTATE::PLAYER_SLEEP); //이거 PLAYER_SLEEP;으로


	//아이템 인벤토리에 넣어주기

	return S_OK;
}

_bool CBedRoll::RayCollInfo(_vector* pRayArray, CGameObject** pGameObject)
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

HRESULT CBedRoll::Add_Components()
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
	ColliderDesc.vExtents = _float3(0.5f, 0.2f, 0.5f);//aabb 조절가능
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y*0.5f, 0.f);
	//ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y * 0.5f, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBedRoll::Bind_ShaderResources()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

CBedRoll* CBedRoll::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBedRoll* pInstance = new CBedRoll(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBedRoll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBedRoll::Clone(void* pArg)
{
	CBedRoll* pInstance = new CBedRoll(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBedRoll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBedRoll::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
