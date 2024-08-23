#include "UIStoveItem.h"
#include "GameInstance.h"
#include"UIItemIcon.h"
#include "UITEXT.h"

CUIStoveItem::CUIStoveItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIBase{ pDevice,pContext }
{
    ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIStoveItem::CUIStoveItem(const CUIStoveItem& rhs)
    :CUIBase(rhs), m_ViewMatrix{ rhs.m_ViewMatrix }, m_ProjMatrix{ rhs.m_ProjMatrix }
{
}

HRESULT CUIStoveItem::Initialize_Prototype()
{
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

    return S_OK;
}

HRESULT CUIStoveItem::Initialize(void* pArg)
{
    STOVEITEMUI_DESC* pDesc = static_cast<STOVEITEMUI_DESC*>(pArg);



    m_fX = g_iWinSizeX >> 1;
    m_fY = g_iWinSizeY >> 1;


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

    _vector Scale = XMLoadFloat3(&m_pTransformCom->Get_Scaled());

    m_fSize.x = abs(XMVectorGetX(Scale));
    m_fSize.y = abs(XMVectorGetY(Scale));

    XMStoreFloat3(&m_fPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

    m_pText.resize(3);

    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Add_PartUI()))
        return E_FAIL;

    return S_OK;
}

void CUIStoveItem::Priority_Tick(_float fTimeDelta)
{
}

void CUIStoveItem::Tick(_float fTimeDelta)
{
}

void CUIStoveItem::Late_Tick(_float fTimeDelta)
{

}

HRESULT CUIStoveItem::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();



    return S_OK;
}

void CUIStoveItem::Choice_Render()
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);

    if (m_InItem)
    {
        m_pIcon->Choice_Render();
        for (auto& pText : m_pText)
            pText->Choice_Render();
    }

}

HRESULT CUIStoveItem::Set_ItemUIInfo(void* pArg)
{
    if (nullptr != pArg)
    {
        CItem::ITEM_DESC* pDesc = static_cast<CItem::ITEM_DESC*>(pArg);

        m_iQuantity = pDesc->iQuantity;//아이콘
        m_ItemName = pDesc->ItemName;
        m_ItemUIName = pDesc->ItemUIName;//선택시 렌더용
        wstring wItemFront = TEXT("Prototype_Component_Texture_ico_GearItem_");

        m_IconTextureTag = wItemFront + m_ItemName;
    


        dynamic_cast<CUIItemIcon*>(m_pIcon)->Set_Texture(m_IconTextureTag);

        dynamic_cast<CUITEXT*>(m_pText[0])->Set_Text(m_ItemUIName);

        wstring wQuantity = to_wstring(m_iQuantity);
        dynamic_cast<CUITEXT*>(m_pText[2])->Set_Text(wQuantity);

        m_InItem = true;
    }
    else
    {
        m_IconTextureTag = TEXT("Prototype_Component_Texture_InventoryItemCase");
        m_InItem = false;
    }

    return S_OK;
}

HRESULT CUIStoveItem::Add_Components()
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

HRESULT CUIStoveItem::Bind_ShaderResources()
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

_bool CUIStoveItem::IntersectUI()
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


CUIStoveItem* CUIStoveItem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUIStoveItem* pInstance = new CUIStoveItem(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUIStoveItem");
        Safe_Release(pInstance);
    }

    return pInstance;
}
HRESULT CUIStoveItem::Add_PartUI()
{

    CUIItemIcon::UI_ITEMICON_DESC UIIconDesc = {};
    UIIconDesc.TextureTag = TEXT("");
    UIIconDesc.Icon_ID = 0;
    UIIconDesc.UIID = -1;
    _vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    XMStoreFloat4x4(&UIIconDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(80.f, 80.f, 1.f) * XMMatrixTranslation(pos.m128_f32[0], pos.m128_f32[1], 0.f));
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_StoveUIDetail"), TEXT("Prototype_GameObject_UIItemIcon"), &m_pIcon, &UIIconDesc)))
        return E_FAIL;

    CUITEXT::UI_DESC UITextDesc = {};
    //이름
    UITextDesc.Font = TEXT("Font_Bold12");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = { pos.m128_f32[0] + 120.f, pos.m128_f32[1] };
    UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_StoveUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_pText[0], &UITextDesc)))
        return E_FAIL;
    //필요갯수
    UITextDesc.Font = TEXT("Font_Bold12");
    UITextDesc.TextTag = TEXT("1 /");
    UITextDesc.TextPosition = { pos.m128_f32[0]+120.f, pos.m128_f32[1]-18.f };
    UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_StoveUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_pText[1], &UITextDesc)))
        return E_FAIL;
    //총
    UITextDesc.Font = TEXT("Font_Bold12");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = { pos.m128_f32[0]+150.f, pos.m128_f32[1]-18.f };
    UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_StoveUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_pText[2], &UITextDesc)))
        return E_FAIL;
    return S_OK;
}
CGameObject* CUIStoveItem::Clone(void* pArg)
{
    CUIStoveItem* pInstance = new CUIStoveItem(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUIStoveItem");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUIStoveItem::Free()
{
    __super::Free();


    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pIcon);

    for(auto& iter :m_pText)
     Safe_Release(iter);
}
