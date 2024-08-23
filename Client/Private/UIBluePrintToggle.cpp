#include "UIBluePrintToggle.h"
#include "GameInstance.h"   
#include "Item.h"
#include "Inventory.h"

#include "UIItemIcon.h"
#include "UITEXT.h"
#include "UIBack.h"
#include "UIButton.h"
#include "UIActiveButton.h"
#include "UIObject.h"
#include "Player.h"
#include"UImanager.h"

CUIBluePrintToggle::CUIBluePrintToggle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIBase(pDevice, pContext), m_fX(0.f), m_fY(0.f)
{
    ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIBluePrintToggle::CUIBluePrintToggle(const CUIBluePrintToggle& rhs)
    :CUIBase(rhs),
    m_fX(rhs.m_fX), m_fY(rhs.m_fY),
    m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUIBluePrintToggle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIBluePrintToggle::Initialize(void* pArg)
{
    UI_BLUEPRINTTOGGLE_DESC* pDesc = static_cast<UI_BLUEPRINTTOGGLE_DESC*>(pArg);

    m_vColor = pDesc->vColor;
    m_ShaderTag = pDesc->ShaderTag;
    m_ItemUIName = pDesc->UINameTag;
    m_ItemName = pDesc->NameTag;
    m_fUsingTime = pDesc->fUsingTime;
    m_FirstIngredient = pDesc->FirstIngredient;
    m_SecondIngredient = pDesc->SecondIngredient;
    m_bhasCollider = pDesc->bIsCollider;




    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_TextGroup.resize(3);
    m_InfoButton.resize(1);

    m_fX = g_iWinSizeX >> 1;
    m_fY = g_iWinSizeY >> 1;


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




    if (FAILED(Add_PartUI()))
        return E_FAIL;

    return S_OK;
}

void CUIBluePrintToggle::Priority_Tick(_float fTimeDelta)
{
}

void CUIBluePrintToggle::Tick(_float fTimeDelta)
{

    if (STATE_LOAD==m_eState)
    {
        CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
        CInventory* pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Inventory")));

        pPlayer->Add_EnterTime(fTimeDelta);
        if (pPlayer->isLoadFinished())
        {
            pInventory->Using_Item(m_FirstIngredient);
            pInventory->Using_Item(m_SecondIngredient);
            pInventory->Add_BedRoll();
            pPlayer->CurrentLoadingBarReset();
            m_eState = STATE_NORMAL;
        }
    }
    Set_CheckInven();
}

void CUIBluePrintToggle::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUIBluePrintToggle::Render()
{

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    return S_OK;
}

void CUIBluePrintToggle::Choice_Render()
{
    m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[0]);

    if(m_isPick)
        m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[1]);
    
    if(STATE_LOAD==m_eState)
        m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, TEXT("Layer_LoadingBar"));

    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

void CUIBluePrintToggle::Set_PickItem(_bool bPick)
{
    m_isPick = bPick;
}

_bool CUIBluePrintToggle::Intersect()
{
    if (!m_isPick)
    {
        if (Intersect_Point())
        {
            m_isPick = true;
            dynamic_cast<CUITEXT*>(m_TextGroup[2])->Set_Color(_float4{ 1.f, 1.f, 1.f, 1.f });
            return true;
        }
        else
        {
            dynamic_cast<CUITEXT*>(m_TextGroup[2])->Set_Color(_float4{ 0.8f, 0.8f, 0.8f, 1.f });
        }
    }
    else if (m_isPick)
    {
        for (_uint i = 0; i < m_InfoButton.size(); i++)
        {
            if (dynamic_cast<CUIActiveButton*>(m_InfoButton[i])->IntersectUI())
            {
              
                switch (i)
                {
                case 0:
                {
                    if (Make_CheckInven())
                        m_eState = STATE_LOAD;
                }
                    break;

                default:
                    break;
                }

                return true;
            }
        }
    }

    return false;
}

void CUIBluePrintToggle::Set_CheckInven()
{
    CInventory* pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Inventory")));


    
    CItem::ITEM_DESC ItemDesc{};
    pInventory->Check_StoveItem(m_FirstIngredient, &ItemDesc);
    dynamic_cast<CUITEXT*>(m_TextGroup[0])->Set_Text(to_wstring(m_iFirstEA) + TEXT(" / ") + to_wstring(ItemDesc.iQuantity) + TEXT("  가죽"));

    pInventory->Check_StoveItem(m_SecondIngredient, &ItemDesc);
    dynamic_cast<CUITEXT*>(m_TextGroup[1])->Set_Text(to_wstring(m_iSecondEA) + TEXT(" / ") + to_wstring(ItemDesc.iQuantity) + TEXT("  내장"));
}

_bool CUIBluePrintToggle::Make_CheckInven()
{
    CInventory* pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Inventory")));

    CItem::ITEM_DESC ItemDesc{};
    pInventory->Check_StoveItem(m_FirstIngredient, &ItemDesc);
    if (ItemDesc.iQuantity < m_iFirstEA)
        return false;

    pInventory->Check_StoveItem(m_SecondIngredient, &ItemDesc);
    if (ItemDesc.iQuantity < m_iSecondEA)
        return false;

    return true;
}

HRESULT CUIBluePrintToggle::Add_Components()
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

HRESULT CUIBluePrintToggle::Add_PartUI()
{

    wstring LayerName = TEXT("");
    //inStove
    LayerName = TEXT("Layer_UIBlueprintToggle");

    m_wLayerName.push_back(LayerName);  

    CUITEXT::UI_DESC UITextDesc = {};
    _vector Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    
    //뒤로
    UITextDesc.Font = TEXT("Font_Bold14");
    UITextDesc.TextTag = m_ItemUIName;
    UITextDesc.TextPosition = { XMVectorGetX(Pos),XMVectorGetY(Pos) };
    UITextDesc.Color = { 0.8f, 0.8f, 0.8f, 1.f };
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
        return E_FAIL;

    LayerName = TEXT("Layer_DetailBlueprintToggle");

    m_wLayerName.push_back(LayerName);

    UITextDesc.Font = TEXT("Font_Bold18");
    UITextDesc.TextTag = m_ItemUIName;
    UITextDesc.TextPosition = { m_fX*0.0f, m_fY*0.65f};
    UITextDesc.Color = { 1.0f, 1.0f, 1.0f, 1.f };
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"),&m_TextGroup[2], & UITextDesc)))
        return E_FAIL;



    CUIObject::UI_OBJECT_DESC UIDesc = {};
    wstring FrontText = TEXT("Prototype_Component_Texture_ico_GearItem_");
    UIDesc.TextureTag = FrontText + m_ItemName;
    UIDesc.Icon_ID = 0;
    XMStoreFloat4x4(&UIDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(300.f, 300.f, 1.f) * XMMatrixTranslation(m_fX * 0.0f, m_fY * 0.0f, 0.f));

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
        return E_FAIL;

    UITextDesc.Font = TEXT("Font_Bold18");
    UITextDesc.TextTag = TEXT("재료");
    UITextDesc.TextPosition = { m_fX * 0.6f, m_fY * 0.65f };
    UITextDesc.Color = { 1.0f, 1.0f, 1.0f, 1.f };
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
        return E_FAIL;


    UIDesc.TextureTag = TEXT("Prototype_Component_Texture_Time");
    UIDesc.Icon_ID = 0;
    XMStoreFloat4x4(&UIDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(40.f, 40.f, 1.f) * XMMatrixTranslation(m_fX * +0.45f, m_fY * 0.0f, 0.f));

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
        return E_FAIL;

    wstring TimeText = to_wstring((_int)m_fUsingTime);
    UITextDesc.Font = TEXT("Font_Bold14");
    UITextDesc.TextTag = TimeText+TEXT("분");
    UITextDesc.TextPosition = { m_fX * 0.8f, m_fY * 0.f };
    UITextDesc.Color = { 1.0f, 1.0f, 1.0f, 1.f };
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
        return E_FAIL;

    UIDesc.TextureTag = FrontText + m_FirstIngredient;
    UIDesc.Icon_ID = 0;
    XMStoreFloat4x4(&UIDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * 0.45f, m_fY * 0.2f, 0.f));

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
        return E_FAIL;




    UITextDesc.Font = TEXT("Font_Bold14");
    UITextDesc.TextTag = to_wstring(m_iFirstEA) + TEXT("  가죽");
    UITextDesc.TextPosition = { m_fX * 0.8f, m_fY * 0.2f };
    UITextDesc.Color = { 1.0f, 1.0f, 1.0f, 1.f };
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"),&m_TextGroup[0], & UITextDesc)))
        return E_FAIL;


    UITextDesc.Font = TEXT("Font_Bold14");
    UITextDesc.TextTag = to_wstring(m_iSecondEA) + TEXT("  내장");
    UITextDesc.TextPosition = { m_fX * 0.8f, m_fY * 0.4f };
    UITextDesc.Color = { 1.0f, 1.0f, 1.0f, 1.f };
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &m_TextGroup[1], &UITextDesc)))
        return E_FAIL;

    UIDesc.TextureTag = FrontText + m_SecondIngredient;
    UIDesc.Icon_ID = 0;
    XMStoreFloat4x4(&UIDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * 0.45f, m_fY * 0.4f, 0.f));

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
        return E_FAIL;

    CUIActiveButton::ACTIVEUI_DESC ActiveDesc = {};

    ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
    ActiveDesc.Icon_ID = 2;
    ActiveDesc.UIID = -1;
    ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
    XMStoreFloat4x4(&ActiveDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(240.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * 0.65f, m_fY * -0.6f, 0.f));
    ActiveDesc.bhasText = true;
    CUITEXT::UI_DESC TextDesc{};
    TextDesc.TextTag = TEXT("제작하기");
    TextDesc.Font = TEXT("Font_Bold18");
    TextDesc.TextPosition = _float2(m_fX * 0.63f, m_fY * -0.6f);
    TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
    ActiveDesc.TEXTDesc = &TextDesc;

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_InfoButton[0], &ActiveDesc)))
        return E_FAIL;


    return S_OK;
}

_bool CUIBluePrintToggle::Intersect_Point()
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

HRESULT CUIBluePrintToggle::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
        return E_FAIL;
    //아이콘 여러개 넣을려고 생각해서 만듬 (이게 몇번째 텍스처에 접근할지에 대한 것임)
    //if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iIconID)))
    //    return E_FAIL;



    return S_OK;
}

CUIBluePrintToggle* CUIBluePrintToggle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

    CUIBluePrintToggle* pInstance = new CUIBluePrintToggle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUIBluePrintToggle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUIBluePrintToggle::Clone(void* pArg)
{
    CUIBluePrintToggle* pInstance = new CUIBluePrintToggle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUIBluePrintToggle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUIBluePrintToggle::Free()
{
    for (auto& iter : m_TextGroup)
        Safe_Release(iter);

    for (auto& iter : m_InfoButton)
        Safe_Release(iter);

    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
