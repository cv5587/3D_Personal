#include "stdafx.h"
#include "UIObject.h"

#include "GameInstance.h"
#include "Item.h"

CUIObject::CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBase(pDevice,pContext),m_fX(0.f),m_fY(0.f), m_fSizeX(0), m_fSizeY(0)
{
	ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));		
	ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
	ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIObject::CUIObject(const CUIObject& rhs)
	:CUIBase(rhs),
	m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_fSizeX(rhs.m_fSizeX), m_fSizeY(rhs.m_fSizeY),
	m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUIObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
	UI_OBJECT_DESC* pDesc = static_cast<UI_OBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	//m_fSizeX = 60.f;
	//m_fSizeY = 60.f;
	//m_fX = g_iWinSizeX >> 1;
	//m_fY = g_iWinSizeY >> 1;

	////이제 아이콘을 어떻게 따로 위치를 잡을지를 알아보고
	////아이콘을 어떻게 저장해놓고 언제 보여줄지 로직을 잡자
	//m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-m_fX*0.3f, -m_fY*0.4f, 0.f, 1.f));
	//
	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));
	//뷰,투영행렬
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	if (FAILED(Add_Components()))
		return E_FAIL;

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
	
}

HRESULT CUIObject::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	//이건 쉐이더패스 몇번째꺼 쓸지에 대한 거임;;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

//위치,크기, 아이콘ID  컨트롤 하면됨
//레이어에 넣어두고 렌더그룹에 안 넣어주면 됨.
void CUIObject::Choice_Render()
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

//ui위치마다 하나의 객체는 무조건 생성해야됨.
//각 위치의 아이콘의 iD만 바꿔주는형식으로 제작
void CUIObject::UI_Render(_uint IconID)
{
	m_iIconID = IconID;
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

_bool CUIObject::Compare_ID()
{
	
	if (CItem::ITEM_END == m_iIconID)
		return false;
	else
		return true;
}

HRESULT CUIObject::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex")));
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, m_TextureTag));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CUIObject::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//아이콘 여러개 넣을려고 생각해서 만듬 (이게 몇번째 텍스처에 접근할지에 대한 것임)
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iIconID)))
		return E_FAIL;

	return S_OK;
}

CUIObject* CUIObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIObject* pInstance = new CUIObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : UIObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIObject::Clone(void* pArg)
{
	CUIObject* pInstance = new CUIObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIObject");
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
