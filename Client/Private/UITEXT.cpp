#include "UITEXT.h"
#include "GameInstance.h"

CUITEXT::CUITEXT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBase(pDevice, pContext), m_fX(0.f), m_fY(0.f), m_fSizeX(0), m_fSizeY(0)
{
	ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));
	ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
	ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUITEXT::CUITEXT(const CUITEXT& rhs)
	:CUIBase(rhs),
	m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_fSizeX(rhs.m_fSizeX), m_fSizeY(rhs.m_fSizeY),
	m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)

{
}

HRESULT CUITEXT::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUITEXT::Initialize(void* pArg)
{

	UI_DESC* pDesc = static_cast<UI_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_TextTag = pDesc->TextTag;
	m_TextPosition = pDesc->TextPosition;
	m_Font = pDesc->Font;
	m_Color = pDesc->Color;
	//m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));
	//ºä,Åõ¿µÇà·Ä
	//XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	//XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CUITEXT::Priority_Tick(_float fTimeDelta)
{
}

void CUITEXT::Tick(_float fTimeDelta)
{
}

void CUITEXT::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUITEXT::Render()
{
	

	m_pGameInstance->Render_Font(m_Font, m_TextTag, m_TextPosition, XMLoadFloat4(&m_Color));
	return S_OK;
}

void CUITEXT::Choice_Render()
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUITEXT::Add_Components()
{

	return S_OK;
}

HRESULT CUITEXT::Bind_ShaderResources()
{

	return S_OK;
}

CUITEXT* CUITEXT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUITEXT* pInstance = new CUITEXT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUITEXT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUITEXT::Clone(void* pArg)
{
	CUITEXT* pInstance = new CUITEXT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUITEXT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUITEXT::Free()
{
	__super::Free();

	//Safe_Release(m_pVIBufferCom);
	//Safe_Release(m_pShaderCom);
}
