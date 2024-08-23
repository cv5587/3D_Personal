#include "UIBlood.h"
#include "GameInstance.h"   
CUIBlood::CUIBlood(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIBase(pDevice, pContext), m_fX(0.f), m_fY(0.f)
{
    ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIBlood::CUIBlood(const CUIBlood& rhs)
    :CUIBase(rhs),
    m_fX(rhs.m_fX), m_fY(rhs.m_fY),
    m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUIBlood::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIBlood::Initialize(void* pArg)
{

    UI_BLOOD_DESC* pDesc = static_cast<UI_BLOOD_DESC*>(pArg);


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    m_fX = g_iWinSizeX >> 1;
    m_fY = g_iWinSizeY >> 1;


    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));
    //뷰,투영행렬
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));


    _vector Scale = XMLoadFloat3(&m_pTransformCom->Get_Scaled());//x,y 크기   

    if (FAILED(Add_Components()))
        return E_FAIL;

    return S_OK;
}

void CUIBlood::Priority_Tick(_float fTimeDelta)
{
}

void CUIBlood::Tick(_float fTimeDelta)
{
    m_fTime += 2.f*fTimeDelta;
    if (m_fTime > m_fMaxTime)
        m_isLived = false;
}

void CUIBlood::Late_Tick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);

}

HRESULT CUIBlood::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(1);

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUIBlood::Add_Components()
{

    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
    if (nullptr == m_pVIBufferCom)
        return E_FAIL;

    m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTexBack")));
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Blood")));
    if (nullptr == m_pTextureCom)
        return E_FAIL;

    return S_OK;
}

HRESULT CUIBlood::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fCurrentLoad", &m_fTime, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fMaxLoad", &m_fMaxTime, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
        return E_FAIL;
    //아이콘 여러개 넣을려고 생각해서 만듬 (이게 몇번째 텍스처에 접근할지에 대한 것임)
    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
        return E_FAIL;



    return S_OK;
}

CUIBlood* CUIBlood::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUIBlood* pInstance = new CUIBlood(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUIBlood");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUIBlood::Clone(void* pArg)
{
    CUIBlood* pInstance = new CUIBlood(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUIBlood");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUIBlood::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
