#include "stdafx.h"
#include "..\Public\Explosion.h"

#include "GameInstance.h"

CExplosion::CExplosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBlendObject(pDevice, pContext)
{
}

CExplosion::CExplosion(const CExplosion& rhs)
	: CBlendObject(rhs)
{
}

HRESULT CExplosion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CExplosion::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	EXPLOSION_DESC* pDesc = static_cast<CExplosion::EXPLOSION_DESC*>(pArg);

	m_pParentsMatrix = pDesc->pParentsMatrix;
	m_fControlMatrix = pDesc->fControlMatrix;

	if (FAILED(Add_Components()))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(rand() % 20, 3.f, rand() % 20, 1.f));
	
//	_vector Position = XMVector3TransformCoord(XMLoadFloat4(&m_fControlMatrix), XMLoadFloat4x4(m_pParentsMatrix));

//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Position);

	return S_OK;
}

void CExplosion::Priority_Tick(_float fTimeDelta)
{

}

void CExplosion::Tick(_float fTimeDelta)
{


	m_fFrame += 60.f * fTimeDelta;

	if (m_fFrame >= 160.f)
		m_fFrame = 120.f;


}

void CExplosion::Late_Tick(_float fTimeDelta)
{

	//_vector Position = XMVector3TransformCoord();

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, Position);

	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&m_fControlMatrix)* XMLoadFloat4x4(m_pParentsMatrix));

	_vector cam = m_pGameInstance->Get_CamPosition();

	m_pTransformCom->LookBack(cam);	

	Compute_ViewZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_BLEND, this);
}

HRESULT CExplosion::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CExplosion::Add_Components()
{
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Fire"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CExplosion::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;
	
//	if (FAILED(m_pShaderCom->Bind_RawValue("g_Index", &m_fFrame, sizeof(_uint))))
//		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

void CExplosion::Reset()
{
	m_fFrame = 0.f;
}

CExplosion* CExplosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CExplosion* pInstance = new CExplosion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CExplosion::Clone(void* pArg)
{
	CExplosion* pInstance = new CExplosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExplosion::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
