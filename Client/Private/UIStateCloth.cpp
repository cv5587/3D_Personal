#include "UIStateCloth.h"
#include "GameInstance.h"   
#include "Item.h"

CUIStateCloth::CUIStateCloth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIBase{pDevice, pContext}
{
    ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIStateCloth::CUIStateCloth(const CUIStateCloth& rhs)
    :CUIBase{ rhs },
    m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUIStateCloth::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIStateCloth::Initialize(void* pArg)
{

    UI_STATECLOTH_DESC* pDesc = static_cast<UI_STATECLOTH_DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;



    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));
    //뷰,투영행렬
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

    if (FAILED(Add_Components()))
        return E_FAIL;

    //아이템아이콘,내구도 아이콘 , 무게 아이콘
   // m_IconGroup.resize(1);
    //이름, 설명 ,내구도, 무게
   // m_TextGroup.resize(4);
   



    return S_OK;
}

void CUIStateCloth::Priority_Tick(_float fTimeDelta)
{
}

void CUIStateCloth::Tick(_float fTimeDelta)
{
}

void CUIStateCloth::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUIStateCloth::Render()
{

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    return S_OK;
}

void CUIStateCloth::Choice_Render()
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

void CUIStateCloth::Set_PickItemInfo(_bool bItemPick)
{
}

HRESULT CUIStateCloth::Add_Components()
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


HRESULT CUIStateCloth::Bind_ShaderResources()
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

    return S_OK;
}

CUIStateCloth* CUIStateCloth::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUIStateCloth* pInstance = new CUIStateCloth(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUIStateCloth");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUIStateCloth::Clone(void* pArg)
{
    CUIStateCloth* pInstance = new CUIStateCloth(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUIStateCloth");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUIStateCloth::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
