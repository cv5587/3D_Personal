#include "RevolverFire.h"
#include "GameInstance.h"
CRevolverFire::CRevolverFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBlendObject(pDevice, pContext)
{
}

CRevolverFire::CRevolverFire(const CRevolverFire& rhs)
	: CBlendObject(rhs)
{
}

HRESULT CRevolverFire::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRevolverFire::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	REVOLVERFIRE_DESC* pDesc = static_cast<CRevolverFire::REVOLVERFIRE_DESC*>(pArg);

	m_pParentsMatrix = pDesc->pParentsMatrix;
	m_fControlMatrix = pDesc->fControlMatrix;
	m_pShot = pDesc->pShot;
	if (FAILED(Add_Components()))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(rand() % 20, 3.f, rand() % 20, 1.f));

//	_vector Position = XMVector3TransformCoord(XMLoadFloat4(&m_fControlMatrix), XMLoadFloat4x4(m_pParentsMatrix));

//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Position);

	return S_OK;
}

void CRevolverFire::Priority_Tick(_float fTimeDelta)
{
}

void CRevolverFire::Tick(_float fTimeDelta)
{
	m_Time +=  fTimeDelta;

	if (m_Time > 0.05f)
	{
		*m_pShot = false;
		m_Time = 0.f;
	}
	//m_Time = 0.f;
}

void CRevolverFire::Late_Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&m_fControlMatrix) * XMLoadFloat4x4(m_pParentsMatrix));

	_vector cam = m_pGameInstance->Get_CamPosition();

	m_pTransformCom->LookBack(cam);
	//m_pTransformCom->LookAt(cam);

	Compute_ViewZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_BLEND, this);
}

HRESULT CRevolverFire::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CRevolverFire::Add_Components()
{
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_LoadingStruggle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Muzzle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CRevolverFire::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_CurrentBar", &m_Time, sizeof(_float))))
			return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

void CRevolverFire::Reset()
{
}

CRevolverFire* CRevolverFire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRevolverFire* pInstance = new CRevolverFire(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CRevolverFire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRevolverFire::Clone(void* pArg)
{
	CRevolverFire* pInstance = new CRevolverFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CRevolverFire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRevolverFire::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
