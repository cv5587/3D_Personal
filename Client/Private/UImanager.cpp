#include "UImanager.h"
#include "GameInstance.h"
#include "Item.h"
#include "UIObject.h"
#include "UITEXT.h"

CUImanager::CUImanager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance{CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CUImanager::Initialize()
{
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	//KEY_ID
	wstring LayerName = TEXT("Layer_KeyIcon");

	m_UISelector.push_back(LayerName);

	CUIObject::UI_DESC UIDesc {};
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_KEYSelectorIcon");
	UIDesc.Icon_ID = (_uint)KEY_LEFTCLICK;
	XMStoreFloat4x4(&UIDesc.vPrePosition, 
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.2f, -m_fY * 0.5f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_SelectorIcon"), &UIDesc)))
		return E_FAIL;




	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_KEYSelectorIcon");
	UIDesc.Icon_ID = (_uint)KEY_RIGHTCLICK;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * 0.2f, -m_fY * 0.5f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_SelectorIcon"), &UIDesc)))
		return E_FAIL;

	//BASE_ID

	LayerName = TEXT("Layer_BaseIcon");
	
	m_UISelector.push_back(LayerName);

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_BASESelectorIcon");
	UIDesc.Icon_ID = (_uint)BASE_BAG;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.9f, -m_fY * 0.8f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_SelectorIcon"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_BASESelectorIcon");
	UIDesc.Icon_ID = (_uint)BASE_WEIGHT;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(50.f, 50.f, 1.f) * XMMatrixTranslation(m_fX * 0.4f, -m_fY * 0.04f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_SelectorIcon"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_BASESelectorIcon");
	UIDesc.Icon_ID = (_uint)BASE_DURABILITY;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(50.f, 50.f, 1.f) * XMMatrixTranslation(m_fX * 0.4f, m_fY * 0.04f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_SelectorIcon"), &UIDesc)))
		return E_FAIL;

	//TYPE_ID(총 2개 보여주기 )
	LayerName = TEXT("Layer_Type");

	m_UISelector.push_back(LayerName);

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UIDesc.Icon_ID = (_uint)CItem::ITEMTYPE::ITEM_END;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(70.f, 70.f, 1.f) * XMMatrixTranslation(m_fX * 0.4f, m_fY * 0.5f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_SelectorIcon"), &UIDesc)))
		return E_FAIL;

	m_TypeUI.push_back(dynamic_cast<CUIObject*>(m_pGameInstance->
		FindIndex_CloneObject(LEVEL_GAMEPLAY, m_UISelector[Layer_Type], TYPE_ONE)));

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UIDesc.Icon_ID = (_uint)CItem::ITEMTYPE::ITEM_END;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(70.f, 70.f, 1.f) * XMMatrixTranslation(m_fX * 0.4f+50.f, m_fY * 0.5f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_SelectorIcon"), &UIDesc)))
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
	UITextDesc.TextPosition = { m_fX+ m_fX * 0.38f, m_fY-m_fY * 0.42f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;


	//INFO
	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.38f, m_fY - m_fY * 0.32f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;
	
	//WEIGHT
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.42f, m_fY + m_fY * 0.02f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//DURABILITY
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.42f, m_fY - m_fY * 0.06f };
	

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//LCLICK
	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("가져가기");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.25f, m_fY + m_fY * 0.6f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//RCLICK
	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("놔두기");
	UITextDesc.TextPosition = { m_fX + m_fX * 0.15f, m_fY + m_fY * 0.6f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//BAGWEIGHT
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("/ 30.00 KG");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.78f, m_fY + m_fY * 0.78f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//TOTALWEIGHT
	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX - m_fX * 0.86f, m_fY + m_fY * 0.78f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUImanager::Item_SetInfo(CGameObject* pPickObject,_float fTotalWeight)
{
	//모든 정보를 가진 아이템
	CItem* pItem = static_cast<CItem*>(pPickObject);

	//ui 아이콘 type을 담아주자 한개면 한개만 뜨고 2개면 2개

	vector<CItem::ITEMTYPE> vecType = pItem->Get_ItemType();


	for (size_t i = 0; i < vecType.size(); i++)
	{
		m_TypeUI[i]->Set_ID((_uint)vecType[i]);
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

	//for (auto iter : m_UISelector)
	//	Safe_Release(iter);

	m_UISelector.clear();	
}
