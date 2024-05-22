#include "UIButton.h"
#include "GameInstance.h"
#include "Item.h"
CUIButton::CUIButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIBase(pDevice, pContext)
{

    ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIButton::CUIButton(const CUIButton& rhs)
    :CUIBase(rhs),m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUIButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIButton::Initialize(void* pArg)
{

    UI_BUTTON_DESC* pDesc = static_cast<UI_BUTTON_DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_UIToggleBrightness = pDesc->UIToggleBrightness;
    m_bhasCollider= pDesc->bisCollider;
    //
    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));
    //뷰,투영행렬
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));


    _vector Scale = XMLoadFloat3(&m_pTransformCom->Get_Scaled());//x,y 크기   

    m_fSize.x = abs(XMVectorGetX(Scale));
    m_fSize.y = abs(XMVectorGetY(Scale));  

    XMStoreFloat3(&m_fPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));


    if (FAILED(Add_Components()))
        return E_FAIL;

    return S_OK;
}

void CUIButton::Priority_Tick(_float fTimeDelta)
{
}

void CUIButton::Tick(_float fTimeDelta)
{

}

void CUIButton::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUIButton::Render()
{

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    //이건 쉐이더패스 몇번째꺼 쓸지에 대한 거임;;
    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();



    return S_OK;
}

void CUIButton::Choice_Render()
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUIButton::Action()
{

    return S_OK;
}

_bool CUIButton::Intersect_Point()
{
    if (m_bhasCollider)
    {
        _vector vWorldMouse = m_pGameInstance->Picking_UI(XMLoadFloat4x4(&m_ProjMatrix));

        _float2 pt{};
        pt.x = XMVectorGetX(vWorldMouse);
        pt.y = XMVectorGetY(vWorldMouse);

        if (pt.x >= m_fPosition.x - m_fSize.x / 2 && pt.x < m_fPosition.x + m_fSize.x / 2
            && pt.y >= m_fPosition.y - m_fSize.y / 2 && pt.y < m_fPosition.y + m_fSize.x / 2)
            return true;
    }
    

    return false;
}

HRESULT CUIButton::Add_Components()
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

HRESULT CUIButton::Bind_ShaderResources()
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

    //uiid
    if (FAILED(m_pShaderCom->Bind_ID("g_ID", m_UIID)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Brightness", &m_UIToggleBrightness, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

CUIButton* CUIButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUIButton* pInstance = new CUIButton(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUIButton");
        Safe_Release(pInstance);
    }

    return pInstance;
}
CGameObject* CUIButton::Clone(void* pArg)
{
    CUIButton* pInstance = new CUIButton(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUIButton");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CUIButton::Free()
{
    __super::Free();


    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
