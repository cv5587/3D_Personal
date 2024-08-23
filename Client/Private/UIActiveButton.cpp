#include "UIActiveButton.h"
#include "GameInstance.h"

CUIActiveButton::CUIActiveButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIBase{pDevice,pContext}
{
    ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIActiveButton::CUIActiveButton(const CUIActiveButton& rhs)
    :CUIBase( rhs ), m_ViewMatrix{ rhs.m_ViewMatrix }, m_ProjMatrix{ rhs.m_ProjMatrix }
{
}

HRESULT CUIActiveButton::Initialize_Prototype()
{
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

    return S_OK;
}

HRESULT CUIActiveButton::Initialize(void* pArg)
{
    ACTIVEUI_DESC* pDesc = static_cast<ACTIVEUI_DESC*>(pArg);

    m_vColor = pDesc->vColor;



    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

    _vector Scale = XMLoadFloat3(&m_pTransformCom->Get_Scaled());

    m_fSize.x = abs(XMVectorGetX(Scale));
    m_fSize.y = abs(XMVectorGetY(Scale));

    XMStoreFloat3(&m_fPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));


    if (FAILED(Add_Components()))
        return E_FAIL;

    m_bhasText = pDesc->bhasText;
    if (m_bhasText)
    {
        m_pTextDesc = pDesc->TEXTDesc;
        if (FAILED(Add_Text()))
            return E_FAIL;
    }

    return S_OK;
}

void CUIActiveButton::Priority_Tick(_float fTimeDelta)
{
}

void CUIActiveButton::Tick(_float fTimeDelta)
{
}

void CUIActiveButton::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUIActiveButton::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();



    return S_OK;
}

void CUIActiveButton::Choice_Render()
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
   if(m_bhasText)
    {
        m_pText->Choice_Render();
    }
}

HRESULT CUIActiveButton::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
    if (nullptr == m_pVIBufferCom)
        return E_FAIL;

    m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTexColor")));
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, m_TextureTag));
    if (nullptr == m_pTextureCom)
        return E_FAIL;

    return S_OK;
}

HRESULT CUIActiveButton::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    //아이콘 여러개 넣을려고 생각해서 만듬 (이게 몇번째 텍스처에 접근할지에 대한 것임)
    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iIconID)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
        return E_FAIL;


    return S_OK;
}

_bool CUIActiveButton::IntersectUI()
{
    _vector vWorldMouse = m_pGameInstance->Picking_UI(XMLoadFloat4x4(&m_ProjMatrix));

    _float2 pt{};
    pt.x = XMVectorGetX(vWorldMouse);
    pt.y = XMVectorGetY(vWorldMouse);

    if (pt.x >= m_fPosition.x - m_fSize.x / 2 && pt.x < m_fPosition.x + m_fSize.x / 2
        && pt.y >= m_fPosition.y - m_fSize.y / 2 && pt.y < m_fPosition.y + m_fSize.x / 2)
        return true;


    return false;
}


CUIActiveButton* CUIActiveButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUIActiveButton* pInstance = new CUIActiveButton(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUIActiveButton");
        Safe_Release(pInstance);
    }

    return pInstance;
}
HRESULT CUIActiveButton::Add_Text()
{

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ActiveText"), TEXT("Prototype_GameObject_UIText"),&m_pText, m_pTextDesc)))
        return E_FAIL;

    return S_OK;
}
CGameObject* CUIActiveButton::Clone(void* pArg)
{
    CUIActiveButton* pInstance = new CUIActiveButton(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUIActiveButton");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUIActiveButton::Free()
{
    __super::Free();


    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pText);
}
