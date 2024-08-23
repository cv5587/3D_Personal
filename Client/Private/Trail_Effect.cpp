#include "Trail_Effect.h"
#include "GameInstance.h"
CTrail_Effect::CTrail_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBlendObject(pDevice, pContext)
{
}

CTrail_Effect::CTrail_Effect(const CTrail_Effect& rhs)
	: CBlendObject(rhs)
{
}

HRESULT CTrail_Effect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrail_Effect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	TRAILEFFECT_DESC* pDesc = static_cast<CTrail_Effect::TRAILEFFECT_DESC*>(pArg);

	m_pParentsMatrix = pDesc->pParentsMatrix;
	m_fControlMatrix = pDesc->fControlMatrix;

	if (FAILED(Add_Components()))
		return E_FAIL;


	return S_OK;
}

void CTrail_Effect::Priority_Tick(_float fTimeDelta)
{
}

void CTrail_Effect::Tick(_float fTimeDelta)
{

}

void CTrail_Effect::Late_Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(m_fControlMatrix) * XMLoadFloat4x4(m_pParentsMatrix));


	m_pVIBufferCom->Tick(fTimeDelta, m_pTransformCom->Get_WorldMatrix());
	//m_pVIBufferCom->Render();
	Compute_ViewZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_BLEND, this);
}

HRESULT CTrail_Effect::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(3);

	//m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTrail_Effect::Add_Components()
{
	CVIBuffer_Trail::TRAIL_DESC pDesc{};

	pDesc.ivtxMaxNum = 10;
	pDesc.vInitPosA = _float3( -0.05f ,0.f, 0.f);
	pDesc.vInitPosB = _float3(0.05f, 0.f, 0.f);
	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Trail"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom),&pDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FlareSmoke"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrail_Effect::Bind_ShaderResources()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

CTrail_Effect* CTrail_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrail_Effect* pInstance = new CTrail_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTrail_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrail_Effect::Clone(void* pArg)
{
	CTrail_Effect* pInstance = new CTrail_Effect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTrail_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail_Effect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
