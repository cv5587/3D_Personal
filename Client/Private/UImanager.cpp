#include "UImanager.h"
#include "GameInstance.h"
#include "Item.h"
#include "ItemData.h"


CUImanager::CUImanager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance{CGameInstance::GetInstance()},
	m_pDevice{pDevice},
	m_pContext{pContext}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CUImanager::Initialize()
{
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	m_ItemcasePos = { 0.665f,	0.4f };

	m_ItemcaseInterval = { 0.175f,	 0.32f };

	m_ItemToggle.resize(20);
	m_ClothToggle.resize(16);

	if (FAILED(Ready_RayColl()))
		return E_FAIL;

	if (FAILED(Ready_PickUpSelecter()))
		return E_FAIL;
	/*************************************************인벤토리********************************/
	if (FAILED(Ready_Inventory()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Cloth()))
		return E_FAIL;

	if (FAILED(Ready_LoadingBar()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUImanager::Item_SetInfo(CGameObject* pPickObject,_float fTotalWeight)
{
	//모든 정보를 가진 아이템
	CItem* pItem = static_cast<CItem*>(pPickObject);

	//ui 아이콘 type을 담아주자 한개면 한개만 뜨고 2개면 2개

	vector<_uint> vecType = pItem->Get_ItemType();


	for (size_t i = 0; i < vecType.size(); i++)
	{
		m_TypeUI[i]->Set_IconID((_uint)vecType[i]);
	}


	//각 uitext에 정보를 담아 줘야됨(TEXT)


	CUITEXT* pUIText;
	pUIText = dynamic_cast<CUITEXT*>(m_pGameInstance->
		FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Text], TEXT_NAME));

	pUIText->Set_Text(pItem->Get_TextUIName());

	pUIText = dynamic_cast<CUITEXT*>(m_pGameInstance->
		FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Text], TEXT_INFO));

	pUIText->Set_Text(pItem->Get_TextInfo());

	pUIText = dynamic_cast<CUITEXT*>(m_pGameInstance->
		FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Text], TEXT_WEIGHT));

	pUIText->Set_Text(pItem->Get_TextWeight());

	pUIText = dynamic_cast<CUITEXT*>(m_pGameInstance->
		FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Text], TEXT_DURABILITY));

	pUIText->Set_Text(pItem->Get_TextDurability());


	wstring wTotalWeight = to_wstring(fTotalWeight);
	wstring wPart = wTotalWeight.substr(0, 4);
	pUIText = dynamic_cast<CUITEXT*>(m_pGameInstance->
		FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Text], TEXT_TOTALWEIGHT));

	pUIText->Set_Text(wPart);

	return S_OK;
}

void CUImanager::Render_UI(_uint LayerIndex)
{

		m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_UISelector[LayerIndex]);

}

void CUImanager::Render_TypeUI(_uint LayerIndex)
{
	for(auto& ptypeUI : m_TypeUI)
		if(ptypeUI->Compare_ID())
			m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, ptypeUI);
}

void CUImanager::Render_Cloth()
{
	for (auto& pClothToggle : m_ClothToggle)
	{
		if(dynamic_cast<CUIClothToggle*>(pClothToggle)->Check_Equip())
		{
			_uint TYPE = dynamic_cast<CUIClothToggle*>(pClothToggle)->Get_Type();
			switch (TYPE)
			{
			case CUIClothToggle::TYPE_HAT:
					m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_UISelector[Layer_StateHat]);		
					break;
			case CUIClothToggle::TYPE_SHIRTS:
				m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_UISelector[Layer_StateSleeve]);
				break;
			case CUIClothToggle::TYPE_SHOES:
				m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_UISelector[Layer_StateShoes]);
				break;
			case CUIClothToggle::TYPE_OUTPANT:
				m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_UISelector[Layer_StatePant]);
				break;
			default:
				break;
			}
		}
	}
}

void CUImanager::Scene_Change( _uint iUIIndex)
{
	for (size_t i = 0; i < SCENE_END; i++)
	{
		if(i!=iUIIndex)
		{
			dynamic_cast<CUIBase*>(m_pGameInstance->
				FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Toggle], i))->Set_IconID(TOGGLE_OFF);
		}
		else
		{
			CGameObject* pGameObject = m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Toggle], i);
			dynamic_cast<CUIBase*>(pGameObject)->Set_IconID(TOGGLE_ON);
			
			dynamic_cast<CUITEXT*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_NameText]))
				->Set_Text(dynamic_cast<CUIToggleID*>(pGameObject)->Get_SceneText());

		}
	}
}

void CUImanager::Sort_IconChange(_uint iSortIndex)
{
	for (size_t i = 0; i < SORT_END; i++)
	{
		if (i != iSortIndex)
		{
			dynamic_cast<CUISortToggle*>(m_pGameInstance->
				FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_SortBase], i))->Set_Brightness(0.6f);
			dynamic_cast<CUISortToggle*>(m_pGameInstance->
				FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_SortIcon], i))->Set_Brightness(0.6f);
		}
		else
		{
			CGameObject* pGameObject = m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_SortBase], i);
			dynamic_cast<CUISortToggle*>(pGameObject)->Set_Brightness(1.f);			
			dynamic_cast<CUISortToggle*>(pGameObject)->Set_Brightness(1.f);
			 pGameObject = m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_InvenText], INVENTEXT_SORTMODE);
			
			wstring SortName = TEXT("");
			switch (i)
			{
			case Client::CUImanager::SORT_ALL:
				SortName = TEXT("모두");
				break;
			case Client::CUImanager::SORT_KINDLING:
				SortName = TEXT("불쏘시개");
				break;
			case Client::CUImanager::SORT_MEDIKIT:
				SortName = TEXT("구급 약품");
				break;
			case Client::CUImanager::SORT_CLOTH:
				SortName = TEXT("옷");
				break;
			case Client::CUImanager::SORT_FOOD:
				SortName = TEXT("음식");
				break;
			case Client::CUImanager::SORT_EQUIP:
				SortName = TEXT("장비");
				break;
			case Client::CUImanager::SORT_STUFF:
				SortName = TEXT("재료");
				break;
			default:
				break;
			}
			dynamic_cast<CUITEXT*>(pGameObject)->Set_Text(SortName);
		}
	}


}

HRESULT CUImanager::Ready_RayColl()
{
	wstring LayerName = TEXT("Layer_RayColl");

	m_UISelector.push_back(LayerName);

	CUITEXT::UI_DESC UITextDesc{};

	//NAME
	UITextDesc.Font = TEXT("Font_Bold10");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX + m_fX * -0.025f, m_fY + m_fY * 0.25f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUImanager::Ready_PickUpSelecter()
{


	//KEY_ID
	wstring LayerName = TEXT("Layer_KeyIcon");

	m_UISelector.push_back(LayerName);

	CUIObject::UI_OBJECT_DESC UIDesc{};
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_KEYSelectorIcon");
	UIDesc.Icon_ID = (_uint)KEY_LEFTCLICK;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.2f, -m_fY * 0.5f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_KEYSelectorIcon");
	UIDesc.Icon_ID = (_uint)KEY_RIGHTCLICK;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * 0.2f, -m_fY * 0.5f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	//BASE_ID

	LayerName = TEXT("Layer_BaseIcon");

	m_UISelector.push_back(LayerName);

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_BASESelectorIcon");
	UIDesc.Icon_ID = (_uint)BASE_BAG;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.9f, -m_fY * 0.8f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_BASESelectorIcon");
	UIDesc.Icon_ID = (_uint)BASE_WEIGHT;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(50.f, 50.f, 1.f) * XMMatrixTranslation(m_fX * 0.4f, -m_fY * 0.04f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_BASESelectorIcon");
	UIDesc.Icon_ID = (_uint)BASE_DURABILITY;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(50.f, 50.f, 1.f) * XMMatrixTranslation(m_fX * 0.4f, m_fY * 0.04f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	//TYPE_ID(총 2개 보여주기 )
	LayerName = TEXT("Layer_Type");

	m_UISelector.push_back(LayerName);

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UIDesc.Icon_ID = (_uint)CItem::ITEMTYPE::ITEM_END;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(70.f, 70.f, 1.f) * XMMatrixTranslation(m_fX * 0.4f, m_fY * 0.5f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	m_TypeUI.push_back(dynamic_cast<CUIObject*>(m_pGameInstance->
		FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Type], TYPE_ONE)));

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UIDesc.Icon_ID = (_uint)CItem::ITEMTYPE::ITEM_END;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(70.f, 70.f, 1.f) * XMMatrixTranslation(m_fX * 0.4f + 50.f, m_fY * 0.5f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	m_TypeUI.push_back(dynamic_cast<CUIObject*>(m_pGameInstance->
		FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Type], TYPE_TWO)));

	//TEXT(총 8개 보여주기 )
	LayerName = TEXT("Layer_Text");

	m_UISelector.push_back(LayerName);

	CUITEXT::UI_DESC UITextDesc{};

	//NAME
	UITextDesc.Font = TEXT("Font_Bold18");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.38f, m_fY - m_fY * 0.42f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;


	//INFO
	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.38f, m_fY - m_fY * 0.32f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//WEIGHT
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.42f, m_fY + m_fY * 0.02f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//DURABILITY
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.42f, m_fY - m_fY * 0.06f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//LCLICK
	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("가져가기");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.25f, m_fY + m_fY * 0.6f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//RCLICK
	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("놔두기");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.15f, m_fY + m_fY * 0.6f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//BAGWEIGHT
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("/ 30.00 KG");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.78f, m_fY + m_fY * 0.78f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//TOTALWEIGHT
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.86f, m_fY + m_fY * 0.78f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUImanager::Ready_Inventory()
{

	wstring LayerName=TEXT("");
	//Inventory_ID
	LayerName = TEXT("Layer_InventroyUIBack");

	m_UISelector.push_back(LayerName);

	CUIColor::UI_COLOR_DESC UIBackColorDesc{};
	UIBackColorDesc.Icon_ID = (_uint)BACK_NOTEXTURE;
	XMStoreFloat4x4(&UIBackColorDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(g_iWinSizeX, g_iWinSizeY, 1.f) * XMMatrixTranslation(0, 0, 0.0f));
	UIBackColorDesc.vColor = _float4(0.f, 0.f, 0.f, 0.6f);

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIColor"), &UIBackColorDesc)))
		return E_FAIL;


	CUIBack::UI_BACK_DESC UIBackDesc{};
	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventoryback");
	UIBackDesc.Icon_ID = (_uint)BACK_FOCUS;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(g_iWinSizeX, g_iWinSizeY, 1.f) * XMMatrixTranslation(0, 0, 0.0f));
	UIBackDesc.vColor = _float4(0.f, 0.f, 0.f, 1.0f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexBack");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventoryback");
	UIBackDesc.Icon_ID = (_uint)BACK_CLOUD;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(760.f, 1080.f, 1.f) * XMMatrixTranslation(-m_fX * 0.2f, -m_fY * 0.3f, 0.f));
	UIBackDesc.vColor = _float4(0.29f, 0.41f, 0.33f, 0.3f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;


	LayerName = TEXT("Layer_InventroyUICase");

	m_UISelector.push_back(LayerName);

	CUIObject::UI_OBJECT_DESC UIDesc{};

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryCase");
	UIDesc.Icon_ID = (_uint)CASE_ONE;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(720.f, 720.f, 1.f) * XMMatrixTranslation(-m_fX * 0.24f, -m_fY * 0.37f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	LayerName = TEXT("Layer_InventroyUIToggle");

	m_UISelector.push_back(LayerName);

	CUIToggleID::UI_TOGGLE_DESC ToggleDesc{};

	ToggleDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryToggleState");
	ToggleDesc.Icon_ID = (_uint)TOGGLE_OFF;
	ToggleDesc.UIID = m_UIID++;
	ToggleDesc.UISceneIndex = SCENE_STATE;
	XMStoreFloat4x4(&ToggleDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(70.f, 70.f, 1.f) * XMMatrixTranslation(-m_fX * 0.2f, m_fY * 0.9, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIToggle"), &ToggleDesc)))
		return E_FAIL;

	ToggleDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryToggleEquip");
	ToggleDesc.Icon_ID = (_uint)TOGGLE_OFF;
	ToggleDesc.UIID = m_UIID++;
	ToggleDesc.UISceneIndex = SCENE_EQUIP;
	XMStoreFloat4x4(&ToggleDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(70.f, 70.f, 1.f) * XMMatrixTranslation(-m_fX * 0.1f, m_fY * 0.9, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIToggle"), &ToggleDesc)))
		return E_FAIL;

	ToggleDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryToggleBag");
	ToggleDesc.Icon_ID = (_uint)TOGGLE_OFF;
	ToggleDesc.UIID = m_UIID++;
	ToggleDesc.UISceneIndex = SCENE_INVEN;
	XMStoreFloat4x4(&ToggleDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(70.f, 70.f, 1.f) * XMMatrixTranslation(-m_fX * 0.f, m_fY * 0.9, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIToggle"), &ToggleDesc)))
		return E_FAIL;

	ToggleDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryToggleBlueprint");
	ToggleDesc.Icon_ID = (_uint)TOGGLE_OFF;
	ToggleDesc.UIID = m_UIID++;
	ToggleDesc.UISceneIndex = SCENE_BLUEPRINT;
	XMStoreFloat4x4(&ToggleDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(70.f, 70.f, 1.f) * XMMatrixTranslation(m_fX * 0.1f, m_fY * 0.9, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIToggle"), &ToggleDesc)))
		return E_FAIL;

	LayerName = TEXT("Layer_InventroyUISortBase");

	m_UISelector.push_back(LayerName);

	CUISortToggle::UI_SORT_DESC UISortDesc{};
	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_InventorySelect");
	UISortDesc.Icon_ID = (_uint)SELECT_BASE;
	UISortDesc.UIID = -1;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, m_fY * 0.45, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_InventorySelect");
	UISortDesc.Icon_ID = (_uint)SELECT_BASE;
	UISortDesc.UIID = -1;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, m_fY * 0.275f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_InventorySelect");
	UISortDesc.Icon_ID = (_uint)SELECT_BASE;
	UISortDesc.UIID = -1;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, m_fY * 0.1f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_InventorySelect");
	UISortDesc.Icon_ID = (_uint)SELECT_BASE;
	UISortDesc.UIID = -1;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, -m_fY * 0.075f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_InventorySelect");
	UISortDesc.Icon_ID = (_uint)SELECT_BASE;
	UISortDesc.UIID = -1;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, -m_fY * 0.25f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_InventorySelect");
	UISortDesc.Icon_ID = (_uint)SELECT_BASE;
	UISortDesc.UIID = -1;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, -m_fY * 0.425f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_InventorySelect");
	UISortDesc.Icon_ID = (_uint)SELECT_BASE;
	UISortDesc.UIID = -1;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, -m_fY * 0.6f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	LayerName = TEXT("Layer_InventroyUISortIcon");

	m_UISelector.push_back(LayerName);

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_BASESelectorIcon");
	UISortDesc.Icon_ID = (_uint)BASE_BAG;
	UISortDesc.UIID = m_UIID++;
	UISortDesc.UISortIndex = SORT_ALL;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, m_fY * 0.45, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UISortDesc.Icon_ID = (_uint)CItem::ITEM_KINDLING;
	UISortDesc.UIID = m_UIID++;
	UISortDesc.UISortIndex = SORT_KINDLING;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, m_fY * 0.275f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UISortDesc.Icon_ID = (_uint)CItem::ITEM_MEDKITS;
	UISortDesc.UIID = m_UIID++;
	UISortDesc.UISortIndex = SORT_MEDIKIT;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, m_fY * 0.1f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UISortDesc.Icon_ID = (_uint)CItem::ITEM_CLOTH;
	UISortDesc.UIID = m_UIID++;
	UISortDesc.UISortIndex = SORT_CLOTH;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, -m_fY * 0.075f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UISortDesc.Icon_ID = (_uint)CItem::ITEM_FOOD;
	UISortDesc.UIID = m_UIID++;
	UISortDesc.UISortIndex = SORT_FOOD;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, -m_fY * 0.25f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UISortDesc.Icon_ID = (_uint)CItem::ITEM_EQUIPMENT;
	UISortDesc.UIID = m_UIID++;
	UISortDesc.UISortIndex = SORT_EQUIP;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, -m_fY * 0.425f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	UISortDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UISortDesc.Icon_ID = (_uint)CItem::ITEM_STUFF;
	UISortDesc.UIID = m_UIID++;
	UISortDesc.UISortIndex = SORT_STUFF;
	UISortDesc.UIToggleBrightness = 0.5f;
	XMStoreFloat4x4(&UISortDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.8f, -m_fY * 0.6f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UISortToggle"), &UISortDesc)))
		return E_FAIL;

	//TEXT
	LayerName = TEXT("Layer_NameText");

	m_UISelector.push_back(LayerName);	

	CUITEXT::UI_DESC UITextDesc{};	

	//씬이름
	UITextDesc.Font = TEXT("Font_Bold18");
	UITextDesc.TextTag = TEXT("백팩");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.95f, m_fY - m_fY * 0.95f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;


	LayerName = TEXT("Layer_InventoryText");

	m_UISelector.push_back(LayerName);
	//총 무게
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("0.00");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.95f, m_fY - m_fY * 0.8f };
	UITextDesc.Color = { 0.8f, 0.8f, 0.8f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//가방 무게
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("/ 30.00 KG");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.88f, m_fY - m_fY * 0.8f };
	UITextDesc.Color = { 0.8f, 0.8f, 0.8f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//뒤로
	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("뒤로");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.87f, m_fY + m_fY * 0.875f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;



	//정렬 상태
	UITextDesc.Font = TEXT("Font_Bold18");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.73f, m_fY - m_fY * 0.65f };
	UITextDesc.Color = { 0.8f, 0.8f, 0.8f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;


	LayerName = TEXT("Layer_InvenButton");

	m_UISelector.push_back(LayerName);

	CUIButton::UI_BUTTON_DESC ButtonDesc{};

	ButtonDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
	ButtonDesc.Icon_ID = (_uint)INVENBUTTON_BACK;
	ButtonDesc.UIID = m_UIID++;
	ButtonDesc.UIToggleBrightness = 2.f;
	XMStoreFloat4x4(&ButtonDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(140.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.9f, -m_fY * 0.9f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIButton"), &ButtonDesc)))
		return E_FAIL;



	if (FAILED(Ready_Itemcase()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUImanager::Ready_Itemcase()
{

	wstring LayerName = TEXT("Layer_InventoryItem");

	m_UISelector.push_back(LayerName);

	CUIItemIcon::UI_ITEMICON_DESC ItemIconDesc{};

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t t = 0; t < 5; t++)
		{
			ItemIconDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryItemCase");
			ItemIconDesc.UIID = m_UIID++;//13++
			ItemIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
			XMStoreFloat4x4(&ItemIconDesc.vPrePosition,
				XMMatrixIdentity() * XMMatrixScaling(100.f, 100.f, 1.f) * XMMatrixTranslation(-m_fX * (m_ItemcasePos.x - m_ItemcaseInterval.x * t), m_fY * (m_ItemcasePos.y - m_ItemcaseInterval.y * i), 0.f));
			
			if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_InventoryItem"), TEXT("Prototype_GameObject_UIItemToggle"), &m_ItemToggle[i * 5 + t], &ItemIconDesc)))
				return E_FAIL;
		}
	}
	
	

	return S_OK;
}

HRESULT CUImanager::Ready_State()
{
	wstring LayerName = TEXT("Layer_StateBody");
	m_UISelector.push_back(LayerName);
	//몸
	CUIObject::UI_OBJECT_DESC UIDesc{};
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_PaperDoll_Fullbody");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(260.f*0.8f, 600.f * 0.8f, 1.f) * XMMatrixTranslation(- m_fX*0.4f, m_fY*+0.f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	// 옷들 아이콘 배치 해놓고 텍스처만 돌려끼자.
	 LayerName = TEXT("Layer_StateShoes");
	m_UISelector.push_back(LayerName);

	CUIStateCloth::UI_STATECLOTH_DESC UIClothDesc{};

	UIClothDesc.TextureTag = TEXT("Prototype_Component_Texture_WillBoots");
	UIClothDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIClothDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(260.f * 0.8f, 600.f * 0.8f, 1.f) * XMMatrixTranslation(-m_fX * 0.4f, m_fY * +0.f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIStateCloth"), &UIClothDesc)))
		return E_FAIL;

	LayerName = TEXT("Layer_StatePant");
	m_UISelector.push_back(LayerName);

	UIClothDesc.TextureTag = TEXT("Prototype_Component_Texture_Cargo_pants");
	UIClothDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIClothDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(260.f * 0.8f, 600.f * 0.8f, 1.f) * XMMatrixTranslation(-m_fX * 0.4f, m_fY * +0.f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIStateCloth"), &UIClothDesc)))
		return E_FAIL;

	LayerName = TEXT("Layer_StateSleeve");
	m_UISelector.push_back(LayerName);

	UIClothDesc.TextureTag = TEXT("Prototype_Component_Texture_WillSweater");
	UIClothDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIClothDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(260.f * 0.8f, 600.f * 0.8f, 1.f) * XMMatrixTranslation(-m_fX * 0.4f, m_fY *+0.f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIStateCloth"), &UIClothDesc)))
		return E_FAIL;

	LayerName = TEXT("Layer_StateHat");
	m_UISelector.push_back(LayerName);

	UIClothDesc.TextureTag = TEXT("Prototype_Component_Texture_Toque_Wool");
	UIClothDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIClothDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(260.f * 0.8f, 600.f * 0.8f, 1.f) * XMMatrixTranslation(-m_fX * 0.4f, m_fY * +0.f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIStateCloth"), &UIClothDesc)))
		return E_FAIL;

	LayerName = TEXT("Layer_StateInfoback");
	m_UISelector.push_back(LayerName);

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_State_Iconback");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(90.f, 90.f, 1.f) * XMMatrixTranslation(-m_fX * 0.65f, m_fY * 0.4f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_State_Iconback");
	UIDesc.Icon_ID = 1;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(90.f, 90.f, 1.f) * XMMatrixTranslation(-m_fX * 0.65f, m_fY * 0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_State_Iconback");
	UIDesc.Icon_ID = 2;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(90.f, 90.f, 1.f) * XMMatrixTranslation(-m_fX * 0.65f, m_fY * 0.0f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_State_Iconback");
	UIDesc.Icon_ID = 3;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(90.f, 90.f, 1.f) * XMMatrixTranslation(-m_fX * 0.65f, m_fY * -0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;
	
	LayerName = TEXT("Layer_StateInfo");
	m_UISelector.push_back(LayerName);

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_State_Icon");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(90.f, 90.f, 1.f) * XMMatrixTranslation(-m_fX * 0.65f, m_fY * 0.4f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_State_Icon");
	UIDesc.Icon_ID = 1;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(90.f, 90.f, 1.f) * XMMatrixTranslation(-m_fX * 0.65f, m_fY * 0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_State_Icon");
	UIDesc.Icon_ID = 2;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(90.f, 90.f, 1.f) * XMMatrixTranslation(-m_fX * 0.65f, m_fY * 0.0f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_State_Icon");
	UIDesc.Icon_ID = 3;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(90.f, 90.f, 1.f) * XMMatrixTranslation(-m_fX * 0.65f, m_fY * -0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	LayerName = TEXT("Layer_StateInfoText");
	m_UISelector.push_back(LayerName);

	CUITEXT::UI_DESC UITextDesc{};
	//추후 캐릭터 상태 만들어지면 상태 100,75,50,25, 마다 각자 상태만들어주기
	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("따뜻함");
	UITextDesc.TextPosition = { m_fX-m_fX * 0.85f, m_fY + m_fY * -0.42f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("휴식함");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.85f, m_fY + m_fY * -0.22f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("갈증 없음");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.85f, m_fY + m_fY * -0.02f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("배부름");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.85f, m_fY + m_fY * +0.18f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("칼로리");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.7f, m_fY + m_fY * +0.33f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;
	//캐릭터 칼로리 만들어지면 구현//칼로리 수치
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("1989");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.69f, m_fY + m_fY * +0.4f };
	UITextDesc.Color = { 1.0f, 1.0f, 1.0f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;


	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("기온");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.9f, m_fY + m_fY * +0.7f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 0.8f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT("체감온도 :");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.9f, m_fY + m_fY * +0.8f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 0.8f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT("-20 C");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.7f, m_fY + m_fY * +0.7f };
	UITextDesc.Color = { 0.8f, 0.f, 0.f, 0.9f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold18");
	UITextDesc.TextTag = TEXT("-20 C");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.7f, m_fY + m_fY * +0.8f };
	UITextDesc.Color = { 0.8f, 0.f, 0.f, 0.9f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//뒤로
	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("뒤로");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.87f, m_fY + m_fY * 0.875f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	LayerName = TEXT("Layer_Stateback");
	m_UISelector.push_back(LayerName);

	CUIBack::UI_BACK_DESC UIBackDesc{};
	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryDetailCase");
	UIBackDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.65f, m_fY * -0.4f, 0.f));
	UIBackDesc.vColor = _float4(0.3f, 0.34f, 0.33f, 0.25f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryDetailCase");
	UIBackDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(300.f, 160.f, 1.f) * XMMatrixTranslation(-m_fX * 0.75f, m_fY * -0.75f, 0.f));
	UIBackDesc.vColor = _float4(0.3f, 0.34f, 0.33f, 0.25f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUImanager::Ready_Cloth()
{

	wstring LayerName = TEXT("");
	//옷창 케이스
	LayerName = TEXT("Layer_ClothCase");

	m_UISelector.push_back(LayerName);

	CUIObject::UI_OBJECT_DESC UIDesc{};

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InvenCloth_Left");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(512.f * 0.6f, 1024.f * 0.6f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * 0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InvenCloth_Right");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(512.f * 0.6f, 1024.f * 0.6f, 1.f) * XMMatrixTranslation(m_fX * -0.1f, m_fY * 0.1f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	CUIBack::UI_BACK_DESC UIBackDesc{};
	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryDetailCase");
	UIBackDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(300.f, 160.f, 1.f) * XMMatrixTranslation(-m_fX * 0.75f, m_fY * -0.7f, 0.f));
	UIBackDesc.vColor = _float4(0.3f, 0.34f, 0.33f, 0.25f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	/* 이거 토글에 넣기
	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryDetailCase");
	UIBackDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(450.f, 100.f, 1.f) * XMMatrixTranslation(m_fX * 0.53f, m_fY * -0.5f, 0.f));
	UIBackDesc.vColor = _float4(0.3f, 0.34f, 0.33f, 0.25f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;*/


	//옷창 케이스 글씨

	LayerName = TEXT("Layer_ClothText");
	m_UISelector.push_back(LayerName);

	CUITEXT::UI_DESC UITextDesc{};

	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("머리");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.735f, m_fY + m_fY * -0.72f };
	UITextDesc.Color = { 0.3f, 0.3f, 0.3f, 0.3f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("몸통");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.735f, m_fY + m_fY * -0.45f };
	UITextDesc.Color = { 0.3f, 0.3f, 0.3f, 0.3f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("액세서리");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.16f, m_fY + m_fY * -0.72f };
	UITextDesc.Color = { 0.3f, 0.3f, 0.3f, 0.3f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("다리");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.135f, m_fY + m_fY * -0.45f };
	UITextDesc.Color = { 0.3f, 0.3f, 0.3f, 0.3f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//뒤로
	UITextDesc.Font = TEXT("Font_Normal14");	
	UITextDesc.TextTag = TEXT("뒤로");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.87f, m_fY + m_fY * 0.875f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;


	LayerName = TEXT("Layer_ClothToggle");

	m_UISelector.push_back(LayerName);

	CUIClothToggle::UI_CLOTHTOGGLE_DESC ClothIconDesc{};

	//Prototype_Component_Texture_InventoryItemCase
		/********************************************************************************************************************/
	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_hat");
	ClothIconDesc.UIID = m_UIID++;//33++
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	ClothIconDesc.Type = CUIClothToggle::TYPE_HAT;
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f , 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.76f, m_fY * 0.58f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[0], &ClothIconDesc)))
		return E_FAIL;


	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_hat");
	ClothIconDesc.UIID = m_UIID++;
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.64f, m_fY * 0.58f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[1], &ClothIconDesc)))
		return E_FAIL;


	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_neck");
	ClothIconDesc.UIID = m_UIID++;//33++
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	ClothIconDesc.Type = CUIClothToggle::TYPE_ACC;
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.16f, m_fY * 0.58f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[2], &ClothIconDesc)))
		return E_FAIL;


	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_neck");
	ClothIconDesc.UIID = m_UIID++;
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.04f, m_fY * 0.58f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[3], &ClothIconDesc)))
		return E_FAIL;
	/********************************************************************************************************************/
	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_jacket");
	ClothIconDesc.UIID = m_UIID++;//33++
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	ClothIconDesc.Type = CUIClothToggle::TYPE_JACKET;
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.76f, m_fY * 0.3f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[4], &ClothIconDesc)))
		return E_FAIL;


	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_jacket");
	ClothIconDesc.UIID = m_UIID++;
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.64f, m_fY * 0.3f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[5], &ClothIconDesc)))
		return E_FAIL;


	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_Outpant");
	ClothIconDesc.UIID = m_UIID++;//33++
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	ClothIconDesc.Type = CUIClothToggle::TYPE_OUTPANT;
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.16f, m_fY * 0.3f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[6], &ClothIconDesc)))
		return E_FAIL;


	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_Outpant");
	ClothIconDesc.UIID = m_UIID++;
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.04f, m_fY * 0.3f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[7], &ClothIconDesc)))
		return E_FAIL;
	/********************************************************************************************************************/
	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_shirts");
	ClothIconDesc.UIID = m_UIID++;//33++
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	ClothIconDesc.Type = CUIClothToggle::TYPE_SHIRTS;
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.76f, m_fY * 0.08f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[8], &ClothIconDesc)))
		return E_FAIL;


	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_shirts");
	ClothIconDesc.UIID = m_UIID++;
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.64f, m_fY * 0.08f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[9], &ClothIconDesc)))
		return E_FAIL;


	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_Inpant");
	ClothIconDesc.UIID = m_UIID++;//33++
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	ClothIconDesc.Type = CUIClothToggle::TYPE_INPANT;
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.16f, m_fY * 0.08f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[10], &ClothIconDesc)))
		return E_FAIL;


	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_Inpant");
	ClothIconDesc.UIID = m_UIID++;
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.04f, m_fY * 0.08f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[11], &ClothIconDesc)))
		return E_FAIL;
	/********************************************************************************************************************/
	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_glove");
	ClothIconDesc.UIID = m_UIID++;//33++
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	ClothIconDesc.Type = CUIClothToggle::TYPE_GLOVE;
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * -0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[12], &ClothIconDesc)))
		return E_FAIL;

	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_socks");
	ClothIconDesc.UIID = m_UIID++;//33++
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	ClothIconDesc.Type = CUIClothToggle::TYPE_SOCKS;
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.16f, m_fY * -0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[13], &ClothIconDesc)))
		return E_FAIL;


	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_socks");
	ClothIconDesc.UIID = m_UIID++;
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.04f, m_fY * -0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[14], &ClothIconDesc)))
		return E_FAIL;

	/********************************************************************************************************************/
	ClothIconDesc.TextureTag = TEXT("Prototype_Component_Texture_Clothcase_shoes");
	ClothIconDesc.UIID = m_UIID++;
	ClothIconDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	ClothIconDesc.Type = CUIClothToggle::TYPE_SHOES;
	XMStoreFloat4x4(&ClothIconDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.1f, m_fY * -0.42f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIClothToggle"), &m_ClothToggle[15], &ClothIconDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUImanager::Ready_LoadingBar()
{
	wstring LayerName = TEXT("Layer_LoadingBar");

	m_UISelector.push_back(LayerName);	

	CUIObject::UI_OBJECT_DESC UIDesc{};
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_LoadingBar_Back");
	UIDesc.Icon_ID =0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(200.f, 200.f, 1.f) * XMMatrixTranslation(m_fX * 0.0f, m_fY * 0.0f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	CUILoadingBar::UI_LOADINGBAR_DESC LoadingDesc{};

	LoadingDesc.TextureTag = TEXT("Prototype_Component_Texture_LoadingBar_Current");
	LoadingDesc.Icon_ID = 0;
	XMStoreFloat4x4(&LoadingDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(200.f, 200.f, 1.f)* XMMatrixTranslation(m_fX * 0.0f, m_fY * 0.0f, 0.f));
	
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UILoadingBar"), &LoadingDesc)))
		return E_FAIL;


	return S_OK;
}

void CUImanager::Set_SortMode(_uint SortIndex)
{
	 m_SortMode = (SORT)SortIndex; 
	 Sort_IconChange(SortIndex);
}

_uint CUImanager::Pick_UISceneToggle(_uint UIID)
{
	CUIToggleID* pUIToggle = dynamic_cast<CUIToggleID*>(m_pGameInstance->
		FindUIID_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Toggle],
			UIID));
	if (nullptr != pUIToggle)
		return pUIToggle->Get_SceneIndex();
	else
		return -1;
}

_uint CUImanager::Pick_UISortToggle(_uint UIID)
{
	CUISortToggle* pUISortToggle = dynamic_cast<CUISortToggle*>(m_pGameInstance->
		FindUIID_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_SortIcon],
			UIID));

	if (nullptr != pUISortToggle)
		return pUISortToggle->Get_SortIndex();

}

void CUImanager::Pick_InvenItem(_uint UIID)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->
		FindUIID_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Item], UIID));

	_int ItemToggleIndex = -1;

	for( auto& iter : m_ItemToggle)
	{

		if(iter==pGameObject)
		{
			dynamic_cast<CUIItemToggle*>(iter)->Set_PickItem(true);
			m_PickItemIndex = ItemToggleIndex;
		}
		else
			dynamic_cast<CUIItemToggle*>(iter)->Set_PickItem(false);

		ItemToggleIndex++;
	}

}

void CUImanager::Pick_ClothItem(_uint UIID)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->
		FindUIID_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_ClothToggle], UIID));

	_int ClothToggleIndex = -1;

	for (auto& iter : m_ClothToggle)
	{

		if (iter == pGameObject)
		{
			dynamic_cast<CUIClothToggle*>(iter)->Set_PickItem(true);
			m_PickClothIndex = ClothToggleIndex;
			break;
		}
		else
			dynamic_cast<CUIClothToggle*>(iter)->Set_PickItem(false);

		ClothToggleIndex++;
	}
}

_bool CUImanager::Pick_InvenInfoButton()
{
	
	for (auto& pToggle : m_ItemToggle)
	{
		 if (dynamic_cast <CUIItemToggle*>(pToggle)->Intersect())
		 {
			 return true;
		 }
	}
	return false;
}

_bool CUImanager::Pick_InvenClothButton()
{
	for (auto& pToggle : m_ClothToggle)
	{
		if (dynamic_cast <CUIClothToggle*>(pToggle)->Intersect())
		{
			return true;
		}
	}
	return false;
}

void CUImanager::Set_Weight(_float fTotalWeight)
{
	wstring wTotalWeight = to_wstring(fTotalWeight);
	wstring wPart = wTotalWeight.substr(0, 4);

	dynamic_cast<CUITEXT*>(m_pGameInstance->
		FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_InvenText], INVENTEXT_TOTALWEIGHT))->Set_Text(wPart);
}

HRESULT CUImanager::Cloth_Update(CItemData* pItemData)
{
	CUIClothToggle::UI_CLOTHTOGGLE_DESC ClothDataDesc{};
	Add_ClothIcon(pItemData, &ClothDataDesc);	
	if (pItemData->Compare_Name(TEXT("ToqueWool")))
	{
		ClothDataDesc.Type = CUIClothToggle::TYPE::TYPE_HAT;	
		dynamic_cast<CUIClothToggle*>(m_ClothToggle[0])->Set_ItemUIInfo(&ClothDataDesc);
	}
	else	if (pItemData->Compare_Name(TEXT("Boots")))
	{
		ClothDataDesc.Type = CUIClothToggle::TYPE::TYPE_SHOES;
		dynamic_cast<CUIClothToggle*>(m_ClothToggle[15])->Set_ItemUIInfo(&ClothDataDesc);
	}
	else	if (pItemData->Compare_Name(TEXT("Sweater")))
	{
		ClothDataDesc.Type = CUIClothToggle::TYPE::TYPE_SHIRTS;
		dynamic_cast<CUIClothToggle*>(m_ClothToggle[8])->Set_ItemUIInfo(&ClothDataDesc);
	}
	else	if (pItemData->Compare_Name(TEXT("Cargopants")))
	{
		ClothDataDesc.Type = CUIClothToggle::TYPE::TYPE_OUTPANT;
		dynamic_cast<CUIClothToggle*>(m_ClothToggle[6])->Set_ItemUIInfo(&ClothDataDesc);
	}

	return S_OK;	
}

//인벤토리 레이어 안에 들어 가있는 아이템의 인덱스( 찾지말고 생성하자.)
HRESULT CUImanager::Add_InvenIcon(_uint Index ,CItem* pItem)
{

	CItem::ITEM_DESC pItemDesc{};
	pItem->Get_Description(&pItemDesc);
	
	CUIItemToggle::UI_ITEMTOGGLE_DESC ItemToggleDesc{};
	ItemToggleDesc.UIID = m_UIID++;
	ItemToggleDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0

	ItemToggleDesc.ItemType.push_back(pItemDesc.ItemType[0]);
	ItemToggleDesc.ItemType.push_back(pItemDesc.ItemType[1]);


	ItemToggleDesc.ItemName = pItemDesc.ItemName;
	ItemToggleDesc.ItemUIName = pItemDesc.ItemUIName;
	ItemToggleDesc.ItemInfo = pItemDesc.ItemInfo;
	ItemToggleDesc.iQuantity = pItemDesc.iQuantity;
	ItemToggleDesc.Durability = pItemDesc.Durability;
	ItemToggleDesc.fWeight = pItemDesc.fWeight;

	CUIItemToggle* pItemToggle = dynamic_cast<CUIItemToggle*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_InventoryItem"), Index));
	pItemToggle->Set_ItemUIInfo(&ItemToggleDesc);

	return S_OK;
	//m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_InventoryItem"),TEXT("Prototype_GameObject_UIItemToggle"), &ItemToggleDesc);
}

HRESULT CUImanager::Add_InvenIcon(_uint Index, CItemData* pItemData)
{
	CItem::ITEM_DESC pItemDesc{};
	pItemData->Make_ItemDataDesc(&pItemDesc);

	CUIItemToggle::UI_ITEMTOGGLE_DESC ItemToggleDesc{};
	ItemToggleDesc.UIID = m_UIID++;
	ItemToggleDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0

	ItemToggleDesc.ItemType.push_back(pItemDesc.ItemType[0]);
	ItemToggleDesc.ItemType.push_back(pItemDesc.ItemType[1]);


	ItemToggleDesc.ItemName = pItemDesc.ItemName;
	ItemToggleDesc.ItemUIName = pItemDesc.ItemUIName;
	ItemToggleDesc.ItemInfo = pItemDesc.ItemInfo;
	ItemToggleDesc.iQuantity = pItemDesc.iQuantity;
	ItemToggleDesc.Durability = pItemDesc.Durability;
	ItemToggleDesc.fWeight = pItemDesc.fWeight;
	ItemToggleDesc.isEquip = pItemDesc.isEquip;

	CUIItemToggle* pItemToggle = dynamic_cast<CUIItemToggle*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_InventoryItem"), Index));
	pItemToggle->Set_ItemUIInfo(&ItemToggleDesc);

	return S_OK;

}

HRESULT CUImanager::Add_ClothIcon(CItemData* pItemData, void* ClothDesc)
{
	CItem::ITEM_DESC pItemDesc{};
	pItemData->Make_ItemDataDesc(&pItemDesc);

	CUIClothToggle::UI_CLOTHTOGGLE_DESC*  pDesc=static_cast<CUIClothToggle::UI_CLOTHTOGGLE_DESC*>(ClothDesc);	

	pDesc->Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0

	pDesc->ItemName = pItemDesc.ItemName;
	pDesc->ItemUIName = pItemDesc.ItemUIName;
	pDesc->ItemInfo = pItemDesc.ItemInfo;
	pDesc->iQuantity = pItemDesc.iQuantity;
	pDesc->Durability = pItemDesc.Durability;
	pDesc->fWeight = pItemDesc.fWeight;
	pDesc->isEquip = pItemDesc.isEquip;
	return S_OK;
}

HRESULT CUImanager::Sort_Reset()
{
	for (auto& iter : m_ItemToggle)
		dynamic_cast<CUIItemToggle*>(iter)->Reset();	

	dynamic_cast<CUIItemToggle*>(m_ItemToggle[0])->Set_PickItem(true);
	return S_OK;
}

HRESULT CUImanager::Cloth_Reset()
{
	for (auto& iter : m_ClothToggle)
		dynamic_cast<CUIClothToggle*>(iter)->Reset();	
	return S_OK;
}

HRESULT CUImanager::Sort_InvenIcon(_uint Index, CItemData* pItemData)
{
	CItem::ITEM_DESC ItemDesc{};
	pItemData->Make_ItemDataDesc(&ItemDesc);

	CUIItemToggle::UI_ITEMTOGGLE_DESC ItemToggleDesc{};
	//ItemToggleDesc.UIID = m_UIID++;
	//ItemToggleDesc.Icon_ID = 0;

	ItemToggleDesc.ItemType.push_back(ItemDesc.ItemType[0]);
	ItemToggleDesc.ItemType.push_back(ItemDesc.ItemType[1]);


	ItemToggleDesc.ItemName = ItemDesc.ItemName;
	ItemToggleDesc.ItemUIName = ItemDesc.ItemUIName;
	ItemToggleDesc.ItemInfo = ItemDesc.ItemInfo;
	ItemToggleDesc.iQuantity = ItemDesc.iQuantity;
	ItemToggleDesc.Durability = ItemDesc.Durability;
	ItemToggleDesc.fWeight = ItemDesc.fWeight;
	ItemToggleDesc.isEquip = ItemDesc.isEquip;
	CUIItemToggle* pItemToggle = dynamic_cast<CUIItemToggle*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_InventoryItem"), Index));
	pItemToggle->Set_ItemUIInfo(&ItemToggleDesc);

	return S_OK;
}

HRESULT CUImanager::RayColl_SetInfo(const wstring ObjName)
{
	CUITEXT* pUIText;
	pUIText = dynamic_cast<CUITEXT*>(m_pGameInstance->
		FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_RayColl]));

	pUIText->Set_Text(ObjName);	

	return S_OK;
}

CUImanager* CUImanager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUImanager* pInstance = new CUImanager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : UImanger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUImanager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	//for (auto iter : m_UISelector)
	//	Safe_Release(iter);

	for (auto& pType : m_TypeUI)
		Safe_Release(pType);

	for (auto& pItemToggle : m_ItemToggle)
		Safe_Release(pItemToggle);

	for (auto& pClothToggle : m_ClothToggle)
		Safe_Release(pClothToggle);

	m_UISelector.clear();	
}
