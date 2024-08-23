#include "Harvest.h"
#include "UIActiveButton.h"
#include "GameInstance.h"
#include "Item.h"
#include "UImanager.h"
#include "Player.h"
#include"UITEXT.h"

CHarvest::CHarvest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance{ CGameInstance::GetInstance() },
	m_pDevice{ pDevice },
	m_pContext{ pContext }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CHarvest::Initialize(void* pArg)
{
	UIHARVEST_DESC* pDesc = static_cast<UIHARVEST_DESC*>(pArg);

	m_pPlayerInventory = static_cast<CInventory*>(pDesc->pPlayerInventory);
	m_pUImanager = static_cast<CUImanager*>(pDesc->pUImanager);
	Safe_AddRef(m_pPlayerInventory);
	Safe_AddRef(m_pUImanager);

	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;


	m_pButton.resize(8);
	m_pInfo.resize(5);
	m_pNumText.resize(3);

	m_pText.resize(2);
	
	if (FAILED(Make_UI()))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CHarvest::Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));

	switch (m_iState)
	{
	case Client::CHarvest::HARVEST_SELECT:

		break;
	case Client::CHarvest::HARVEST_LOAD:
	{
		pPlayer->Add_EnterTime(fTimeDelta);
		if (pPlayer->isLoadFinished())
		{

			for (_uint j = 0; j < m_iNumMeat; j++)
			{
				m_pPlayerInventory->Add_RabbitMeat();
				m_pUImanager->Add_InvenIcon(m_pPlayerInventory->Get_LastIndex(), m_pPlayerInventory->Get_vecItemData().back());
			}

			for (_uint j = 0; j < m_iNumRut; j++)
			{
				m_pPlayerInventory->Add_RabbitRut();
				m_pUImanager->Add_InvenIcon(m_pPlayerInventory->Get_LastIndex(), m_pPlayerInventory->Get_vecItemData().back());
			}

			for (_uint j = 0; j < m_iNumPelt; j++)
			{
				m_pPlayerInventory->Add_RabbitPelt();
				m_pUImanager->Add_InvenIcon(m_pPlayerInventory->Get_LastIndex(), m_pPlayerInventory->Get_vecItemData().back());
			}
			m_pPlayerInventory->Using_Item(TEXT("Rabbit"));	
			pPlayer->CurrentLoadingBarReset();
			pPlayer->Set_State(PLAYER_IDLE);
		}
	}
		break;
	case Client::CHarvest::HARVEST_END:
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CHarvest::Late_Tick(_float fTimeDelta)
{
		m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[0]);
	switch (m_iState)
	{
	case Client::CHarvest::HARVEST_SELECT:

		break;
	case Client::CHarvest::HARVEST_LOAD:
		m_pUImanager->Render_UI(CUImanager::Layer_LoadingBar);
		break;
	case Client::CHarvest::HARVEST_END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CHarvest::Pick_UIHarvest()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
	for (_int i = 0; i < m_pButton.size(); i++)
	{
		if (m_pButton[i]->IntersectUI())
		{
			switch (i)
			{
			case 0://뒤로가기
			{
				pPlayer->Set_State(PLAYER_IDLE);
				
				return S_OK;
				break;
			}
			case 1://수확하기
			{
				m_iState = HARVEST_LOAD;

				return S_OK;
				break;
			}
			case 2:
				if (3 > m_iNumMeat)
				{
					m_iNumMeat++;
					dynamic_cast<CUITEXT*>(m_pNumText[0])->Set_Text(to_wstring(m_iNumMeat));
				}
				break;
			case 3:
				if (0 < m_iNumMeat)
				{
					m_iNumMeat--;
					dynamic_cast<CUITEXT*>(m_pNumText[0])->Set_Text(to_wstring(m_iNumMeat));
				}
				break;
			case 4:
				if (3 > m_iNumRut)
				{
					m_iNumRut++;
					dynamic_cast<CUITEXT*>(m_pNumText[1])->Set_Text(to_wstring(m_iNumRut));
				}
				break;
			case 5:
				if (0 < m_iNumRut)
				{
					m_iNumRut--;
					dynamic_cast<CUITEXT*>(m_pNumText[1])->Set_Text(to_wstring(m_iNumRut));
				}
				break;
			case 6:
				if (3 > m_iNumPelt)
				{
					m_iNumPelt++;
					dynamic_cast<CUITEXT*>(m_pNumText[2])->Set_Text(to_wstring(m_iNumPelt));
				}
				break;
			case 7:
				if (0 < m_iNumPelt)
				{
					m_iNumPelt--;
					dynamic_cast<CUITEXT*>(m_pNumText[2])->Set_Text(to_wstring(m_iNumPelt));
				}
				break;

			default:
				break;
			}
		}
	}
	m_iTime = m_iNumMeat * 30 + m_iNumRut * 20 + m_iNumPelt * 30;
	
	dynamic_cast<CUITEXT*>(m_pText[1])->Set_Text(to_wstring(m_iTime)+TEXT("M"));
	return S_OK;
}

HRESULT CHarvest::Make_UI()
{

	wstring LayerName = TEXT("");
	//inStove
	LayerName = TEXT("Layer_Harvest");

	m_wLayerName.push_back(LayerName);

	CUIColor::UI_COLOR_DESC UIBackColorDesc = {};
	UIBackColorDesc.Icon_ID = (_uint)2;
	XMStoreFloat4x4(&UIBackColorDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(g_iWinSizeX, g_iWinSizeY, 1.f) * XMMatrixTranslation(0, 0, 0.0f));
	UIBackColorDesc.vColor = _float4(0.f, 0.f, 0.f, 0.6f);

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIColor"), &UIBackColorDesc)))
		return E_FAIL;

	CUIBack::UI_BACK_DESC UIBackDesc = {};

	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventoryback");
	UIBackDesc.Icon_ID = (_uint)0;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(g_iWinSizeX, g_iWinSizeY, 1.f) * XMMatrixTranslation(0, 0, 0.0f));
	UIBackDesc.vColor = _float4(0.f, 0.f, 0.f, 1.0f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexBack");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventoryback");
	UIBackDesc.Icon_ID = (_uint)1;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(760.f, 1080.f, 1.f) * XMMatrixTranslation(m_fX * -0.2f, m_fY * 0.3f, 0.f));
	UIBackDesc.vColor = _float4(0.29f, 0.41f, 0.33f, 0.3f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	CUIObject::UI_OBJECT_DESC UIDesc = {};
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryPickLight");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * 0.4f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_ico_GearItem_RawRabbit");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(80.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * 0.4f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryPickLight");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * 0.1f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_ico_GearItem_Rut");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(80.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * 0.1f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryPickLight");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * -0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_ico_GearItem_Pelt");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(80.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * -0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	CUIActiveButton::ACTIVEUI_DESC ActiveDesc = {};

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
	ActiveDesc.Icon_ID = 2;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(140.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.8f, m_fY * -0.85f, 0.f));
	ActiveDesc.bhasText = true;
	CUITEXT::UI_DESC TextDesc{};
	TextDesc.TextTag = TEXT("뒤로가기");
	TextDesc.Font = TEXT("Font_Bold12");
	TextDesc.TextPosition = _float2(m_fX * 0.8f, m_fY * -0.85f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[0], &ActiveDesc)))
		return E_FAIL;


	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
	ActiveDesc.Icon_ID = 2;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(140.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.55f, m_fY * -0.85f, 0.f));
	ActiveDesc.bhasText = true;

	TextDesc.TextTag = TEXT("수확하기");
	TextDesc.Font = TEXT("Font_Bold12");
	TextDesc.TextPosition = _float2(m_fX * 0.55f, m_fY * -0.85f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[1], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Stove_Button");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.6f, m_fY * 0.4f, 0.f));
	ActiveDesc.bhasText = false;
	TextDesc = {};
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[2], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Stove_Button");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(-60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.8f, m_fY * 0.4f, 0.f));
	ActiveDesc.bhasText = false;
	TextDesc = {};
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[3], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Stove_Button");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.6f, m_fY * 0.1f, 0.f));
	ActiveDesc.bhasText = false;
	TextDesc = {};
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[4], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Stove_Button");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(-60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.8f, m_fY * 0.1f, 0.f));
	ActiveDesc.bhasText = false;
	TextDesc = {};
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[5], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Stove_Button");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.6f, m_fY * -0.2f, 0.f));
	ActiveDesc.bhasText = false;
	TextDesc = {};
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[6], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Stove_Button");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(-60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.8f, m_fY * -0.2f, 0.f));
	ActiveDesc.bhasText = false;
	TextDesc = {};
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[7], &ActiveDesc)))
		return E_FAIL;

	CUITEXT::UI_DESC UITextDesc = {};
	//NAME
	UITextDesc.Font = TEXT("Font_Bold18");
	UITextDesc.TextTag = TEXT("수확하기");
	UITextDesc.TextPosition = { m_fX * -0.75f, m_fY * 0.85f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("고기");
	UITextDesc.TextPosition = { m_fX * -0.5f, m_fY * 0.5f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("가죽");
	UITextDesc.TextPosition = { m_fX * -0.5f, m_fY * 0.2f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("내장");
	UITextDesc.TextPosition = { m_fX * -0.5f, m_fY * -0.1f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UIBackColorDesc.Icon_ID = (_uint)2;
	XMStoreFloat4x4(&UIBackColorDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(300.f, 200.f, 1.f) * XMMatrixTranslation(m_fX * 0.6f, m_fY * 0.6f, 0.0f));
	UIBackColorDesc.vColor = _float4(0.f, 0.f, 0.f, 0.6f);

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIColor"), &UIBackColorDesc)))
		return E_FAIL;

	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_Stove_Duration");
	UIBackDesc.Icon_ID = (_uint)0;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(180.f, 180.f, 1.f) * XMMatrixTranslation(m_fX * 0.6f, m_fY * 0.6f, 0.f));
	UIBackDesc.vColor = _float4(0.5f, 0.5f, 0.5f, 0.3f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold18");
	UITextDesc.TextTag = TEXT("토끼 시체");
	UITextDesc.TextPosition = { m_fX * 0.6f, m_fY * 0.7f };
	UITextDesc.Color = { 0.7f, 0.7f, 0.7f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"),&m_pText[0] ,& UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT("수확 시간");
	UITextDesc.TextPosition = { m_fX * 0.6f, m_fY * 0.6f };
	UITextDesc.Color = { 0.7f, 0.7f, 0.7f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT("0M");
	UITextDesc.TextPosition = { m_fX * 0.6f, m_fY * 0.5f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"),&m_pText[1], & UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT("0");
	UITextDesc.TextPosition = { m_fX * -0.5f, m_fY * 0.4f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &m_pNumText[0], &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT(" 0");
	UITextDesc.TextPosition = { m_fX * -0.5f, m_fY * 0.1f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &m_pNumText[1], &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT(" 0");
	UITextDesc.TextPosition = { m_fX * -0.5f, m_fY * -0.2f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &m_pNumText[2], &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT("/ 3");
	UITextDesc.TextPosition = { m_fX * -0.4f, m_fY * 0.4f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"),  &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT("/ 3");
	UITextDesc.TextPosition = { m_fX * -0.4f, m_fY * 0.1f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"),  &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT("/ 3");
	UITextDesc.TextPosition = { m_fX * -0.4f, m_fY * -0.2f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	return S_OK;
}


CHarvest* CHarvest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CHarvest* pInstance = new CHarvest(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CHarvest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHarvest::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pPlayerInventory);
	Safe_Release(m_pUImanager);

	for (auto& pGameObject : m_pButton)
		Safe_Release(pGameObject);

	m_pButton.clear();

	for (auto& pGameObject : m_pNumText)
		Safe_Release(pGameObject);

	m_pNumText.clear();

	for (auto& pGameObject : m_pInfo)
		Safe_Release(pGameObject);

	m_pInfo.clear();

	for (auto& pGameObject : m_pText)
		Safe_Release(pGameObject);

	m_pText.clear();
}
