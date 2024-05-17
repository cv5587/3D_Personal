#include "stdafx.h"
#include "UIBack.h"	

#include "GameInstance.h"
#include "Item.h"

CUIBack::CUIBack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBase(pDevice, pContext), m_fX(0.f), m_fY(0.f), m_fSizeX(0), m_fSizeY(0)
{
	ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));
	ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
	ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIBack::CUIBack(const CUIBack& rhs)
	:CUIBase(rhs),
	m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_fSizeX(rhs.m_fSizeX), m_fSizeY(rhs.m_fSizeY),
	m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUIBack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIBack::Initialize(void* pArg)
{
	UI_BACK_DESC* pDesc = static_cast<UI_BACK_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_vColor = pDesc->vColor;
	m_ShaderTag = pDesc->ShaderTag;
	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));
	//��,�������
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CUIBack::Priority_Tick(_float fTimeDelta)
{
}

void CUIBack::Tick(_float fTimeDelta)
{
}

void CUIBack::Late_Tick(_float fTimeDelta)
{

}

HRESULT CUIBack::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	//�̰� ���̴��н� ���°�� ������ ���� ����;;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

//��ġ,ũ��, ������ID  ��Ʈ�� �ϸ��
//���̾ �־�ΰ� �����׷쿡 �� �־��ָ� ��.
void CUIBack::Choice_Render()
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

//ui��ġ���� �ϳ��� ��ü�� ������ �����ؾߵ�.
//�� ��ġ�� �������� iD�� �ٲ��ִ��������� ����
void CUIBack::UI_Render(_uint IconID)
{
	m_iIconID = IconID;
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUIBack::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, m_ShaderTag));
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, m_TextureTag));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CUIBack::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
		return E_FAIL;

	//������ ������ �������� �����ؼ� ���� (�̰� ���° �ؽ�ó�� ���������� ���� ����)
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iIconID)))
		return E_FAIL;

	return S_OK;
}

CUIBack* CUIBack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIBack* pInstance = new CUIBack(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIBack::Clone(void* pArg)
{
	CUIBack* pInstance = new CUIBack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIBack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIBack::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
