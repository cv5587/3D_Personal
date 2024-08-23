#include "UILoadingStruggle.h"
#include "GameInstance.h"
CUILoadingStruggle::CUILoadingStruggle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBase(pDevice, pContext), m_fX(0.f), m_fY(0.f)
{
	ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
	ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUILoadingStruggle::CUILoadingStruggle(const CUILoadingStruggle& rhs)
	:CUIBase(rhs), m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUILoadingStruggle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUILoadingStruggle::Initialize(void* pArg)
{
	UI_LOADINGSTRUGGLE_DESC* pDesc = static_cast<UI_LOADINGSTRUGGLE_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CUILoadingStruggle::Priority_Tick(_float fTimeDelta)
{
}

void CUILoadingStruggle::Tick(_float fTimeDelta)
{
}

void CUILoadingStruggle::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUILoadingStruggle::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	//이건 쉐이더패스 몇번째꺼 쓸지에 대한 거임;;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUILoadingStruggle::Choice_Render()
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUILoadingStruggle::Add_Components()
{

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_LoadingStruggle")));
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, m_TextureTag));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CUILoadingStruggle::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_CurrentBar", m_pCurrentLoadingBar, sizeof(_float))))
		return E_FAIL;
	//아이콘 여러개 넣을려고 생각해서 만듬 (이게 몇번째 텍스처에 접근할지에 대한 것임)
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iIconID)))
		return E_FAIL;

	return S_OK;
}

CUILoadingStruggle* CUILoadingStruggle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUILoadingStruggle* pInstance = new CUILoadingStruggle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUILoadingStruggle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUILoadingStruggle::Clone(void* pArg)
{
	CUILoadingStruggle* pInstance = new CUILoadingStruggle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUILoadingStruggle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUILoadingStruggle::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
