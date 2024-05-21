#include "UIItemToggle.h"
#include "GameInstance.h"   
#include "Item.h"

#include "UIItemIcon.h"
#include "UITEXT.h"
#include "UIBack.h"
#include "UIButton.h"
#include "Player.h "

CUIItemToggle::CUIItemToggle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIBase(pDevice, pContext), m_fX(0.f), m_fY(0.f)
{
    ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
    ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIItemToggle::CUIItemToggle(const CUIItemToggle& rhs)
    :CUIBase(rhs),
    m_fX(rhs.m_fX), m_fY(rhs.m_fY),
    m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUIItemToggle::Initialize_Prototype()
{



    return S_OK;
}

HRESULT CUIItemToggle::Initialize(void* pArg)
{
    UI_ITEMTOGGLE_DESC* pDesc = static_cast<UI_ITEMTOGGLE_DESC*>(pArg);

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
    m_IconGroup.resize(5);
    m_TextGroup.resize(3);
    m_InfoGroup.resize(11);
    m_InfoButton.resize(3);

    if (FAILED(Add_PartUI()))
        return E_FAIL;

    return S_OK;
}

void CUIItemToggle::Priority_Tick(_float fTimeDelta)
{
}

void CUIItemToggle::Tick(_float fTimeDelta)
{
}

void CUIItemToggle::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUIItemToggle::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    //이건 쉐이더패스 몇번째꺼 쓸지에 대한 거임;;
    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    return S_OK;
}

void CUIItemToggle::Choice_Render()
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);

    if(m_inItem)
    {
        for (size_t i = 0; i < 3; i++)
        {
            m_IconGroup[i]->Choice_Render();
            m_TextGroup[i]->Choice_Render();
        }
        if (nullptr != m_bisEquip)
        {
            if (*m_bisEquip)
                m_IconGroup[4]->Choice_Render();
        }
    }

    if(m_isPick)
    {
        m_IconGroup[3]->Choice_Render();

        for (size_t i = 0; i < 8; i++)//7번째 까지가 기본 내용 뒤는 버튼 이름
        {
            m_InfoGroup[i]->Choice_Render();
        }

        for(auto& pType : m_ItemType)
        {
            switch (pType)
            {
            case CItem::ITEM_KINDLING:
                m_InfoButton[1]->Choice_Render();//액션
                m_InfoGroup[9]->Choice_Render();
                break;
            case CItem::ITEM_MEDKITS:
                m_InfoButton[0]->Choice_Render();//사용
                m_InfoGroup[8]->Choice_Render();
                break;
            case CItem::ITEM_CLOTH:
                m_InfoButton[1]->Choice_Render();//액션
                m_InfoGroup[9]->Choice_Render();
                break;
            case CItem::ITEM_FOOD:
                m_InfoButton[0]->Choice_Render();//사용
                m_InfoGroup[8]->Choice_Render();
                break;
            case CItem::ITEM_EQUIPMENT:
                m_InfoButton[0]->Choice_Render();//사용
                m_InfoGroup[8]->Choice_Render();
                break;
            case CItem::ITEM_STUFF:
                m_InfoButton[1]->Choice_Render();//액션
                m_InfoGroup[9]->Choice_Render();
                break;
            default:
                break;
            }
        }

        m_InfoButton[2]->Choice_Render();
        m_InfoGroup[10]->Choice_Render();
    }
}

HRESULT CUIItemToggle::Action()
{
    return S_OK;
}

HRESULT CUIItemToggle::Set_ItemUIInfo(void* pArg)
{
    UI_ITEMTOGGLE_DESC* pDesc = static_cast<UI_ITEMTOGGLE_DESC*>(pArg);

    m_iQuantity = pDesc->iQuantity;//아이콘

    m_ItemType = pDesc->ItemType;//정렬 참고용

    m_ItemName = pDesc->ItemName;  

    m_ItemUIName = pDesc->ItemUIName;//선택시 렌더용
    m_ItemInfo = pDesc->ItemInfo;       //선택시 렌더용

    m_Durability = pDesc->Durability;//텍스트용
    m_fWeight = pDesc->fWeight;//텍스트용
    m_TextureTag = pDesc->TextureTag; //아이콘 텍스처 생성용

    m_bisEquip = pDesc->isEquip;

    wstring wItemFront = TEXT("Prototype_Component_Texture_ico_GearItem_");

    m_IconTextureTag = wItemFront+ m_ItemName;


    //여기서 아이콘,무게,내구도 아이콘 생성하고 , text 내구도 , 무게 , 갯수 생성(총 6개)
     //세부정보 (인포 아이콘,내구도 아이콘 , 무게 아이콘,이름,설명,내구도,무게) 순서 중요
    dynamic_cast<CUIItemIcon*>(m_IconGroup[0])->Set_Texture(m_IconTextureTag);  

    wstring wQuantity= to_wstring(m_iQuantity);

    dynamic_cast<CUITEXT*>(m_TextGroup[0])->Set_Text(wQuantity);

    wstring wTotalWeight = to_wstring(m_fWeight);
    wstring wPart = wTotalWeight.substr(0, 4);

    dynamic_cast<CUITEXT*>(m_TextGroup[1])->Set_Text(wPart + TEXT("KG"));


    wstring wTotalDurability = to_wstring(m_Durability);
     wstring wDurabilityPart = wTotalDurability.substr(0, 3);

    dynamic_cast<CUITEXT*>(m_TextGroup[2])->Set_Text(wDurabilityPart +TEXT("%"));

    //세부정보 
    //아이콘
    dynamic_cast<CUIItemIcon*>(m_InfoGroup[0])->Set_Texture(m_IconTextureTag);
    //내구도
   // dynamic_cast<CUIItemIcon*>(m_InfoGroup[1])->Set_Texture(m_IconTextureTag);
    //무게
   // dynamic_cast<CUIItemIcon*>(m_InfoGroup[2])->Set_Texture(m_IconTextureTag);  
    //디테일 배경
   // dynamic_cast<CUIItemIcon*>(m_InfoGroup[3])->Set_Texture(m_IconTextureTag);
    //이름
    dynamic_cast<CUITEXT*>(m_InfoGroup[4])->Set_Text(m_ItemUIName);
    //설명
    dynamic_cast<CUITEXT*>(m_InfoGroup[5])->Set_Text(m_ItemInfo);
    //내구도
    dynamic_cast<CUITEXT*>(m_InfoGroup[6])->Set_Text(wDurabilityPart + TEXT("%"));
    //무게
    dynamic_cast<CUITEXT*>(m_InfoGroup[7])->Set_Text(wPart + TEXT("KG"));

    m_inItem = true;

    //작업 할거임
    return S_OK;
}

void CUIItemToggle::Set_PickItem(_bool bPick)
{
    if (m_inItem)
        m_isPick = bPick;
}

_bool  CUIItemToggle::Intersect( )
{
    
    if (m_isPick)
    {
        for (_uint i = 0; i < m_InfoButton.size(); i++)
        {
            if (dynamic_cast<CUIButton*>(m_InfoButton[i])->Intersect_Point())
            {
                CPlayer* pPlayer= dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
                switch (i)
                {
                case 0 :
                    if (TEXT("Stone") == m_ItemName)
                    {
                       pPlayer->Set_Equip(PLAYEREQUIP::EQUIP_STONE);    
                       pPlayer->Set_State(PLAYERSTATE::PLAYER_EQUIP);
                    break;
                    }
                    else if (TEXT("Rabbit") == m_ItemName)
                    {

                       // pPlayer->Set_State(PLAYERSTATE::PLAYER_EQUIP);
                        break;
                    }


                case 1:
                    if (TEXT("Stone") == m_ItemName)
                    {
                        break;
                    }

                case 2:
                    if (TEXT("Stone") == m_ItemName)
                    {
                        pPlayer->Inventory_Drop(m_ItemName);
                        break;
                    }
                    else if (TEXT("Rabbit") == m_ItemName)
                    {

                        pPlayer->Inventory_DropRabbit(m_ItemName);
                        break;
                    }

                default:
                    break;
                }

                return true;
            }
        }
    }

    return false;
}

HRESULT CUIItemToggle::Reset()
{
    m_inItem = { false };
    m_isPick = { false };
    return S_OK;
}

HRESULT CUIItemToggle::Add_Components()
{
    m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
    if (nullptr == m_pVIBufferCom)
        return E_FAIL;

    m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTexToggleID")));
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, m_TextureTag));
    if (nullptr == m_pTextureCom)
        return E_FAIL;

    return S_OK;

}


HRESULT CUIItemToggle::Add_PartUI()
{

    //m_IconGroup******************************************************************************
    //기본 인벤토리 아이콘
    CUIItemIcon::UI_ITEMICON_DESC UIIconDesc{};
    UIIconDesc.TextureTag = TEXT("");
    UIIconDesc.Icon_ID = 0;
    UIIconDesc.UIID = -1;
    _vector pos= m_pTransformCom->Get_State(CTransform::STATE_POSITION);    
    
    XMStoreFloat4x4(&UIIconDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(pos.m128_f32[0], pos.m128_f32[1], 0.f));    
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIItemIcon"), &m_IconGroup[0], &UIIconDesc)))
           return E_FAIL;

    //장착중아이콘
    UIIconDesc.TextureTag = TEXT("");
    UIIconDesc.Icon_ID = 0;
    UIIconDesc.UIID = -1;
     pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    XMStoreFloat4x4(&UIIconDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(100.f, 100.f, 1.f) * XMMatrixTranslation(pos.m128_f32[0], pos.m128_f32[1], 0.f));
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIItemIcon"), &m_IconGroup[4], &UIIconDesc)))
        return E_FAIL;
    dynamic_cast<CUIItemIcon*>(m_IconGroup[4])->Set_Texture(TEXT("Prototype_Component_Texture_Inven_ClothEquip"));

    //weight
    UIIconDesc.TextureTag = TEXT("");
    UIIconDesc.Icon_ID = 1;
    UIIconDesc.UIID = -1;
    XMStoreFloat4x4(&UIIconDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(25.f, 25.f, 1.f) * XMMatrixTranslation(pos.m128_f32[0]-47.f, pos.m128_f32[1]-40.f, 0.f));
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIItemIcon"), &m_IconGroup[1], &UIIconDesc)))
        return E_FAIL;
    dynamic_cast<CUIItemIcon*>(m_IconGroup[1])->Set_Texture(TEXT("Prototype_Component_Texture_BASESelectorIcon"));

    //durability
    UIIconDesc.TextureTag = TEXT("");
    UIIconDesc.Icon_ID = 2;
    UIIconDesc.UIID = -1;
    XMStoreFloat4x4(&UIIconDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(25.f, 25.f, 1.f) * XMMatrixTranslation(pos.m128_f32[0]+10.f, pos.m128_f32[1]-40.f, 0.f));
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIItemIcon"), &m_IconGroup[2], &UIIconDesc)))
        return E_FAIL;
    dynamic_cast<CUIItemIcon*>(m_IconGroup[2])->Set_Texture(TEXT("Prototype_Component_Texture_BASESelectorIcon"));

    //PickLight
    UIIconDesc.TextureTag = TEXT("");
    UIIconDesc.Icon_ID = 0;
    UIIconDesc.UIID = -1;
    XMStoreFloat4x4(&UIIconDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(160.f, 160.f, 1.f) * XMMatrixTranslation(pos.m128_f32[0], pos.m128_f32[1] , 0.f));
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIItemIcon"), &m_IconGroup[3], &UIIconDesc)))
        return E_FAIL;
    dynamic_cast<CUIItemIcon*>(m_IconGroup[3])->Set_Texture(TEXT("Prototype_Component_Texture_InventoryPickLight"));


    //m_TextGroup******************************************************************************

    CUITEXT::UI_DESC UITextDesc{};

    //NAME
    UITextDesc.Font = TEXT("Font_Bold8");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = { m_fX+pos.m128_f32[0]-45.f, m_fY-pos.m128_f32[1] -45.f};
    UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_TextGroup[0], &UITextDesc)))
            return E_FAIL;
    //weight
    UITextDesc.Font = TEXT("Font_Bold8");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = { m_fX + pos.m128_f32[0] - 38.f, m_fY - pos.m128_f32[1] + 35.f };
    UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_TextGroup[1], &UITextDesc)))
        return E_FAIL;
    //durability
    UITextDesc.Font = TEXT("Font_Bold8");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = { m_fX + pos.m128_f32[0] + 16.f, m_fY - pos.m128_f32[1] + 35.f };
    UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_TextGroup[2], &UITextDesc)))
        return E_FAIL;




    //m_InfoGroup******************************************************************************

    //InfoIcon
    UIIconDesc.TextureTag = TEXT("");
    UIIconDesc.Icon_ID = 0;
    UIIconDesc.UIID = -1;
    XMStoreFloat4x4(&UIIconDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(250.f, 250.f, 1.f) * XMMatrixTranslation(m_fX * 0.55f, m_fY * 0.f, 0.f));
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIItemIcon"), &m_InfoGroup[0], &UIIconDesc)))
        return E_FAIL;

    CUIBack::UI_BACK_DESC UIBackDesc{};

    //Durability
    UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_BASESelectorIcon");
    UIBackDesc.Icon_ID = 2;
    XMStoreFloat4x4(&UIBackDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * 0.4f, -m_fY * 0.55f, 0.f));
    UIBackDesc.vColor = _float4(1.0f, 1.0f, 0.9f,1.0f);
    UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIBack"), &m_InfoGroup[1], &UIBackDesc)))
        return E_FAIL;

    //Weight
    UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_BASESelectorIcon");
    UIBackDesc.Icon_ID =1;
    XMStoreFloat4x4(&UIBackDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * 0.58f, -m_fY * 0.55f, 0.f));
    UIBackDesc.vColor = _float4(1.0f, 1.0f, 0.9f, 1.0f);
    UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIBack"), &m_InfoGroup[2], &UIBackDesc)))
        return E_FAIL;



    //DetailCase
   
    UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryDetailCase");
    UIBackDesc.Icon_ID = 0;
    XMStoreFloat4x4(&UIBackDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(450.f, 100.f, 1.f) * XMMatrixTranslation(m_fX * 0.53f, -m_fY * 0.5f, 0.f));
    UIBackDesc.vColor = _float4(0.3f, 0.34f, 0.33f, 0.5f);
    UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"),  TEXT("Prototype_GameObject_UIBack"), &m_InfoGroup[3], &UIBackDesc)))
        return E_FAIL;


    //이름
    UITextDesc.Font = TEXT("Font_Bold18");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = { m_fX +m_fX*0.5f, m_fY -m_fY*0.7f };
    UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_InfoGroup[4], &UITextDesc)))
        return E_FAIL;
    //설명
    UITextDesc.Font = TEXT("Font_Normal14");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = { m_fX + m_fX * 0.25f, m_fY - m_fY * 0.6f };
    UITextDesc.Color = { 0.8f, 0.8f, 0.8f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_InfoGroup[5], &UITextDesc)))
        return E_FAIL;
    //내구도
    UITextDesc.Font = TEXT("Font_Bold12");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = { m_fX + m_fX * 0.425f, m_fY - m_fY * -0.525f };
    UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_InfoGroup[6], &UITextDesc)))
        return E_FAIL;
    //무게
    UITextDesc.Font = TEXT("Font_Bold12");
    UITextDesc.TextTag = TEXT("");
    UITextDesc.TextPosition = { m_fX + m_fX * 0.625f, m_fY - m_fY * -0.525f };
    UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_InfoGroup[7], &UITextDesc)))
        return E_FAIL;

    //***************************************************버튼 글

        //사용하기
    UITextDesc.Font = TEXT("Font_Normal14");
    UITextDesc.TextTag = TEXT("사용하기");
    UITextDesc.TextPosition = { m_fX + m_fX * 0.175f, m_fY + m_fY * 0.675f };
    UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_InfoGroup[8], &UITextDesc)))
        return E_FAIL;

    //액션
    UITextDesc.Font = TEXT("Font_Normal14");
    UITextDesc.TextTag = TEXT("액션");
    UITextDesc.TextPosition = { m_fX + m_fX * 0.47f, m_fY + m_fY * 0.675f };
    UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_InfoGroup[9], &UITextDesc)))
        return E_FAIL;

    //내려놓기
    UITextDesc.Font = TEXT("Font_Normal14");
    UITextDesc.TextTag = TEXT("내려놓기");
    UITextDesc.TextPosition = { m_fX + m_fX * 0.69f, m_fY + m_fY * 0.675f };
    UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_ToggleUIDetail"), TEXT("Prototype_GameObject_UIText"), &m_InfoGroup[10], &UITextDesc)))
        return E_FAIL;

    ////**********************버튼

    // use , INVENBUTTON_ACTION , INVENBUTTON_DROP  1 2 
    wstring LayerName = TEXT("Layer_InfoButton");

    CUIButton::UI_BUTTON_DESC ButtonDesc{};
    //사용
    ButtonDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
    ButtonDesc.Icon_ID = (_uint)1;
    ButtonDesc.UIID = -1;
    ButtonDesc.UIToggleBrightness = 2.f;
    ButtonDesc.bisCollider = true;
    XMStoreFloat4x4(&ButtonDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(160.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.25f, -m_fY * 0.7f, 0.f));

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIButton"),&m_InfoButton[0], & ButtonDesc)))
        return E_FAIL;
    //액션
    ButtonDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
    ButtonDesc.Icon_ID = (_uint)1;
    ButtonDesc.UIID = -1;
    ButtonDesc.UIToggleBrightness = 2.f;
    ButtonDesc.bisCollider = true;
    XMStoreFloat4x4(&ButtonDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(160.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.5f, -m_fY * 0.7f, 0.f));

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIButton"), &m_InfoButton[1], &ButtonDesc)))
        return E_FAIL;
    //버리기
    ButtonDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
    ButtonDesc.Icon_ID = (_uint)2;
    ButtonDesc.UIID = -1;
    ButtonDesc.UIToggleBrightness = 2.f;
    ButtonDesc.bisCollider = true;
    XMStoreFloat4x4(&ButtonDesc.vPrePosition,
        XMMatrixIdentity() * XMMatrixScaling(160, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.75f, -m_fY * 0.7f, 0.f));

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIButton"), &m_InfoButton[2], &ButtonDesc)))
        return E_FAIL;


    return S_OK;

}


HRESULT CUIItemToggle::Bind_ShaderResources()
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


    return S_OK;

}


CUIItemToggle* CUIItemToggle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUIItemToggle* pInstance = new CUIItemToggle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUIItemToggle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUIItemToggle::Clone(void* pArg)
{
    CUIItemToggle* pInstance = new CUIItemToggle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUIItemToggle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUIItemToggle::Free()
{
    __super::Free();

    for (auto& pIcon : m_IconGroup)
        Safe_Release(pIcon);

    for (auto& pText : m_TextGroup)
        Safe_Release(pText);

    for (auto& pInfo : m_InfoGroup)
        Safe_Release(pInfo);

       for (auto& pButton : m_InfoButton)
            Safe_Release(pButton);

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
