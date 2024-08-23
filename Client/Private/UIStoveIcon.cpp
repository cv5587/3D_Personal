#include "UIStoveIcon.h"
#include "GameInstance.h"   
#include "Item.h"

#include "UIItemIcon.h"
#include "UITEXT.h"
#include "UIBack.h"
#include "UIButton.h"
#include "Player.h "

CUIStoveIcon::CUIStoveIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIBase(pDevice, pContext), m_fX(0.f), m_fY(0.f)
{
    ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIStoveIcon::CUIStoveIcon(const CUIStoveIcon& rhs)
    :CUIBase(rhs),
    m_fX(rhs.m_fX), m_fY(rhs.m_fY),
    m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUIStoveIcon::Initialize_Prototype()
{



    return S_OK;
}

HRESULT CUIStoveIcon::Initialize(void* pArg)
{
    STOVEICON_DESC* pDesc = static_cast<STOVEICON_DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    m_fX = g_iWinSizeX >> 1;
    m_fY = g_iWinSizeY >> 1;


    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));
    //뷰,투영행렬
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_TextGroup.resize(3);



    if (FAILED(Add_PartUI()))
        return E_FAIL;

    return S_OK;
}

void CUIStoveIcon::Priority_Tick(_float fTimeDelta)
{
}

void CUIStoveIcon::Tick(_float fTimeDelta)
{
}

void CUIStoveIcon::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUIStoveIcon::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    //이건 쉐이더패스 몇번째꺼 쓸지에 대한 거임;;
    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    return S_OK;
}

void CUIStoveIcon::Choice_Render()
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);

    if(m_inItem)
    {
        for (size_t i = 0; i < 2; i++)
        {
            m_TextGroup[i]->Choice_Render();
        }
    }

}

HRESULT CUIStoveIcon::Set_ItemUIInfo(void* pArg)
{
    STOVEICON_DESC* pDesc = static_cast<STOVEICON_DESC*>(pArg);

    m_iQuantity = pDesc->iQuantity;//아이콘

    m_ItemUIName = pDesc->ItemUIName;//선택시 렌더용
   
    m_ItemName = pDesc->ItemName;//선택시 렌더용

    m_TextureTag = pDesc->TextureTag; //아이콘 텍스처 생성용

    wstring wItemFront = TEXT("Prototype_Component_Texture_ico_GearItem_");

    m_IconTextureTag = wItemFront+ m_ItemName;

    wstring wQuantity= to_wstring(m_iQuantity);


    dynamic_cast<CUITEXT*>(m_TextGroup[0])->Set_Text(m_ItemUIName);

    dynamic_cast<CUITEXT*>(m_TextGroup[1])->Set_Text(TEXT("1"));

    dynamic_cast<CUITEXT*>(m_TextGroup[2])->Set_Text(wQuantity);

     m_inItem = true;

    return S_OK;
}

HRESULT CUIStoveIcon::Reset()
{
    m_inItem = { false };
    return S_OK;
}

HRESULT CUIStoveIcon::Add_Components()
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


HRESULT CUIStoveIcon::Add_PartUI()
{


   //m_TextGroup******************************************************************************

    _vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    CUITEXT::UI_DESC UITextDesc={};

    //아이템 이름
    UITextDesc.Font = TEXT("Font_Bold8");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = { pos.m128_f32[0]+45.f, pos.m128_f32[1]};
    UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_StoveIconDetail"), TEXT("Prototype_GameObject_UIText"), &m_TextGroup[0], &UITextDesc)))
            return E_FAIL;

    //쓸 갯수 (1)
    UITextDesc.Font = TEXT("Font_Bold8");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = {  pos.m128_f32[0] - 15.f,  pos.m128_f32[1] - 35.f };
    UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_StoveIconDetail"), TEXT("Prototype_GameObject_UIText"), &m_TextGroup[1], &UITextDesc)))
        return E_FAIL;
    //들고있는 아이템 갯수
    UITextDesc.Font = TEXT("Font_Bold8");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = {  pos.m128_f32[0] + 36.f,  pos.m128_f32[1] - 35.f };
    UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_StoveIconDetail"), TEXT("Prototype_GameObject_UIText"), &m_TextGroup[2], &UITextDesc)))
        return E_FAIL;



    return S_OK;

}


HRESULT CUIStoveIcon::Bind_ShaderResources()
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

CUIStoveIcon* CUIStoveIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUIStoveIcon* pInstance = new CUIStoveIcon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUIStoveIcon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUIStoveIcon::Clone(void* pArg)
{
    CUIStoveIcon* pInstance = new CUIStoveIcon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUIStoveIcon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUIStoveIcon::Free()
{
    __super::Free();

    for (auto& pText : m_TextGroup)
        Safe_Release(pText);

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
