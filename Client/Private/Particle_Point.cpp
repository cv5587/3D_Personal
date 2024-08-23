#include "stdafx.h"
#include "..\Public\Particle_Point.h"

#include "GameInstance.h"

CParticle_Point::CParticle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CParticle_Point::CParticle_Point(const CParticle_Point& rhs)
	: CGameObject(rhs)
{
}

HRESULT CParticle_Point::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Point::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	PARTICLE_DESC* pDesc = static_cast<PARTICLE_DESC*>(pArg);

	m_pBufferInstance = &pDesc->BufferInstance;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&((PARTICLE_DESC*)pArg)->vStartPos));
	}

	return S_OK;
}

void CParticle_Point::Priority_Tick(_float fTimeDelta)
{

}

void CParticle_Point::Tick(_float fTimeDelta)
{
	m_pVIBufferCom->Drop(fTimeDelta);

}

void CParticle_Point::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CParticle_Point::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CParticle_Point::Add_Components()
{
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Instance_Point"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), m_pBufferInstance)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxInstance_Point"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Dust"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Point::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition_float4(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CParticle_Point* CParticle_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Point* pInstance = new CParticle_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CParticle_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Point::Clone(void* pArg)
{
	CParticle_Point* pInstance = new CParticle_Point(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CParticle_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Point::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

}
