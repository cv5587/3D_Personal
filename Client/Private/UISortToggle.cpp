#include "UISortToggle.h"
#include "GameInstance.h"
#include "Item.h"
CUISortToggle::CUISortToggle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIBase(pDevice, pContext), m_fX(0.f), m_fY(0.f), m_fSizeX(0), m_fSizeY(0)
{
    ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUISortToggle::CUISortToggle(const CUISortToggle& rhs)
    :CUIBase(rhs),
    m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_fSizeX(rhs.m_fSizeX), m_fSizeY(rhs.m_fSizeY),
    m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUISortToggle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUISortToggle::Initialize(void* pArg)
{

    UI_SORT_DESC* pDesc = static_cast<UI_SORT_DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_UISortIndex = pDesc->UISortIndex;
    m_UIToggleBrightness = pDesc->UIToggleBrightness;
    //
    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));
    //뷰,투영행렬
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

    if (FAILED(Add_Components()))
        return E_FAIL;

    return S_OK;
}

void CUISortToggle::Priority_Tick(_float fTimeDelta)
{
}

void CUISortToggle::Tick(_float fTimeDelta)
{
}

void CUISortToggle::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUISortToggle::Render()
{

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    //이건 쉐이더패스 몇번째꺼 쓸지에 대한 거임;;
    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    return S_OK;
}

void CUISortToggle::Choice_Render()
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUISortToggle::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTexBrightnessToggleID")));
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, m_TextureTag));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CUISortToggle::Bind_ShaderResources()
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

    //uiid
    if (FAILED(m_pShaderCom->Bind_ID("g_ID", m_UIID)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Brightness", &m_UIToggleBrightness, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

CUISortToggle* CUISortToggle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUISortToggle* pInstance = new CUISortToggle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUISortToggle");
        Safe_Release(pInstance);
    }

    return pInstance;
}
CGameObject* CUISortToggle::Clone(void* pArg)
{
    CUISortToggle* pInstance = new CUISortToggle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUISortToggle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUISortToggle::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
