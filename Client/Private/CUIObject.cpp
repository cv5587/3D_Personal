#include "stdafx.h"
#include "CUIObject.h"

#include "GameInstance.h"

CUIObject::CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext),m_fX(0.f),m_fY(0.f), m_fSizeX(0), m_fSizeY(0)
{
	ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));		
	ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
	ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIObject::CUIObject(const CUIObject& rhs)
	:CGameObject(rhs), m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_fSizeX(rhs.m_fSizeX), m_fSizeY(rhs.m_fSizeY),
	m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUIObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = 225.f;
	m_fSizeY = 225.f;
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_WorldMatrix._11 = m_fSizeX;
	m_WorldMatrix._22 = m_fSizeY;

	m_WorldMatrix._41 = m_fX - g_iWinSizeX * 0.5f;
	m_WorldMatrix._42 = -m_fY + g_iWinSizeY * 0.5f;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	return S_OK;
}

void CUIObject::Priority_Tick(_float fTimeDelta)
{
}

void CUIObject::Tick(_float fTimeDelta)
{
}

void CUIObject::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUIObject::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUIObject::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex")));
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_LOADING, TEXT("Prototype_Component_Texture_Icon")));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CUIObject::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CUIObject* CUIObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIObject* pInstance = new CUIObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIObject::Clone(void* pArg)
{
	CUIObject* pInstance = new CUIObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIObject::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
