#include "UIBurn.h"
#include "GameInstance.h"
#include "Item.h"
#include "UImanager.h"
#include "Player.h"
#include "Stove.h"
#include "UIActiveButton.h"
#include "UIStoveItem.h"
#include "UITEXT.h"

CUIBurn::CUIBurn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance{ CGameInstance::GetInstance() },
	m_pDevice{ pDevice },
	m_pContext{ pContext }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CUIBurn::Initialize(void* pArg)
{
	UIBURN_DESC* pDesc = static_cast<UIBURN_DESC*>(pArg);	

	m_pPlayerInventory = static_cast<CInventory*>(pDesc->pPlayerInventory);
	m_pUImanager = static_cast<CUImanager*>(pDesc->pUImanager);
	Safe_AddRef(m_pPlayerInventory);

	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;


	m_pButton.resize(8);
	m_pIcon.resize(3);
	m_pSelector.resize(3);
	m_pFood.resize(3);
	m_pFuelButton.resize(4);
	m_pFuelIcon.resize(1);

	if (FAILED(Make_UI()))
		return E_FAIL;

	if (FAILED(Make_Load()))
		return E_FAIL;

	if (FAILED(Make_SelectUI()))
		return E_FAIL;

	if (FAILED(Make_FuelUI()))
		return E_FAIL;

	if (FAILED(Make_CookedUI()))
		return E_FAIL;

	if (FAILED(Make_BoilUI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIBurn::Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));

	if (m_pStove->Get_isFire())//불킴
	{

		switch (m_iState)
		{
		case Client::CUIBurn::STOVE_FUEL:
		{
			CItem::ITEM_DESC ItemDesc{};
			if (m_pPlayerInventory->Check_StoveItem(TEXT("Stick"), &ItemDesc))
				dynamic_cast<CUIStoveItem*>(m_pFuelIcon[0])->Set_ItemUIInfo(&ItemDesc);
			else
				dynamic_cast<CUIStoveItem*>(m_pFuelIcon[0])->Set_ItemUIInfo(nullptr);

		}
		break;

		default:
			break;
		}

		m_Time -= fTimeDelta * 0.1f;

		wstring Time = to_wstring((_int)m_Time);
		
		dynamic_cast<CUITEXT*>(m_pTimeUI)->Set_Text(Time + TEXT("M"));
		 
		if (0.f >= m_Time)
		{
			m_pStove->Set_Fire(false);
			m_iState = STOVE_BURN;
		}
	}
	else//끔
	{

		switch (m_iState)
		{
			case Client::CUIBurn::STOVE_BURN:
			{

			}
			break;
		
			case Client::CUIBurn::STOVE_LOAD:
			{
				pPlayer->Add_EnterTime(fTimeDelta);	
				if (pPlayer->isLoadFinished())
				{
					m_pPlayerInventory->Using_Item(TEXT("Matchbox"));
					m_pPlayerInventory->Using_Item(TEXT("Stick"));
					m_pPlayerInventory->Using_Item(TEXT("Hardwood"));
					m_iState = STOVE_SELECT;
					pPlayer->CurrentLoadingBarReset();
					m_pStove->Set_Fire(true);
				}
			}
			break;
		}

		CItem::ITEM_DESC ItemDesc{};
		if (m_pPlayerInventory->Check_StoveItem(TEXT("Matchbox"), &ItemDesc))
			dynamic_cast<CUIStoveItem*>(m_pIcon[0])->Set_ItemUIInfo(&ItemDesc);
		if (m_pPlayerInventory->Check_StoveItem(TEXT("Stick"), &ItemDesc))
			dynamic_cast<CUIStoveItem*>(m_pIcon[1])->Set_ItemUIInfo(&ItemDesc);
		if (m_pPlayerInventory->Check_StoveItem(TEXT("Hardwood"), &ItemDesc))
			dynamic_cast<CUIStoveItem*>(m_pIcon[2])->Set_ItemUIInfo(&ItemDesc);
	}

	return S_OK;
}

HRESULT CUIBurn::Late_Tick(_float fTimeDelta)
{

	if (m_pStove->Get_isFire())//킴
	{
		switch (m_iState)
		{
		case Client::CUIBurn::STOVE_SELECT:
			m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[2]);
			break;
		case Client::CUIBurn::STOVE_FUEL:
			m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[3]);
				if (dynamic_cast<CUIStoveItem*>(m_pFuelIcon[0])->isItem())
					m_pFuelIcon[0]->Choice_Render();

			break;
		case Client::CUIBurn::STOVE_COOK:
			m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[4]);
			break;
		case Client::CUIBurn::STOVE_BOIL:
			m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[5]);
			break;
		case Client::CUIBurn::STOVE_END:
			break;
		default:
			break;
		}
		
			m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[1]);

	}
	else//끔
	{
		switch (m_iState)
		{
		case Client::CUIBurn::STOVE_BURN:
		{
			m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[0]);
			for (auto& iter : m_pIcon)
				if (dynamic_cast<CUIStoveItem*>(iter)->isItem())
					iter->Choice_Render();
		}
			break;
		case Client::CUIBurn::STOVE_LOAD:
		{
			m_pUImanager->Render_UI(CUImanager::Layer_LoadingBar);
		}
			break;
		
		}


			m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[1]);

		
	}

		

	return S_OK;
}

void CUIBurn::Set_Stove(CGameObject* PickObject)
{
	m_pStove = dynamic_cast<CStove*>(PickObject);

}

HRESULT CUIBurn::Pick_UIBurn()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
	if (m_pStove->Get_isFire())//킴
	{
		switch (m_iState)
		{
		case Client::CUIBurn::STOVE_SELECT:
		{
			for (_int i = 0; i < m_pSelector.size(); i++)
			{
				if (m_pSelector[i]->IntersectUI())
				{
					switch (i)
					{
					case 0://연료추가
					{
						m_iState = STOVE_FUEL;
						return S_OK;
						break;
					}
					case 1://요리하기
					{
						m_iState = STOVE_COOK;
						return S_OK;
						break;
					}
					case 2://물 끓이기
						m_iState = STOVE_BOIL;
						break;
					default:
						break;
					}
				}
			}
		}
			break;
		case Client::CUIBurn::STOVE_FUEL:
		{
			for (_int i = 0; i < m_pFuelButton.size(); i++)
			{
				if (m_pFuelButton[i]->IntersectUI())
				{
					switch (i)
					{
					case 0 :
					{
						m_iState = STOVE_SELECT;
						return S_OK;
					}
						break;
					case 1:
					{
						CItem::ITEM_DESC ItemDesc = {};
						//m_pPlayerInventory->Drop_Item(TEXT("Stick"), &ItemDesc);
						m_Time += 15.f;
						m_pPlayerInventory->Using_Item(TEXT("Stick"));
						m_pGameInstance->StopSound(CH_FX);
						m_pGameInstance->Play_Sound(TEXT("FireFuel.wav"), CH_FX, 1.f);
					}
					default:
						break;
					}
				}
			}
		}
			break;
		case Client::CUIBurn::STOVE_COOK:
		{
			for (_int i = 0; i < m_pSelector.size(); i++)
			{
				if (m_pFood[i]->IntersectUI())
				{
					switch (i)
					{
					case 0://아이콘임 건들 ㄴ
					{
						m_iState = STOVE_FUEL;
						return S_OK;
						break;
					}
					case 1://요리하기
					{
						CItem::ITEM_DESC ItemDesc = {};
						m_pGameInstance->Play_Sound(TEXT("Cooked.wav"), CH_COOK, 1.0f);

						m_pPlayerInventory->Drop_Item(TEXT("RawRabbit"), &ItemDesc);
						if (FAILED(m_pStove->Cooking(&ItemDesc)))
							return E_FAIL;

						return S_OK;
						break;
					}
					case 2://뒤로가기
						m_iState = STOVE_SELECT;	
						break;
					default:
						break;
					}
				}
			}
		}
			break;
		case Client::CUIBurn::STOVE_BOIL:
			break;
		case Client::CUIBurn::STOVE_END:
			break;
		default:
			break;
		}
	
	}
	else//끔
	{
		switch (m_iState)
		{
		case Client::CUIBurn::STOVE_BURN:
		{
			for (_int i = 0; i < m_pButton.size(); i++)
			{
				if (m_pButton[i]->IntersectUI())
				{
					switch (i)
					{
					case 0://뒤로가기
					{
						pPlayer->Set_State(PLAYERSTATE::PLAYER_IDLE);
						return S_OK;
						break;
					}
					case 1://불피우기
					{
						for (auto& iter : m_pIcon)
							if (!dynamic_cast<CUIStoveItem*>(iter)->isItem())
								return S_OK;

						m_iState = STOVE_LOAD;
						m_Time += 30.f;
						//	m_pStove->Set_Fire();

							//플레이어 로딩 게이지 실행 완료시 idle //로딩 게이지 추후구현
						return S_OK;
						break;
					}
					default:
						break;
					}
				}
			}
		}
		break;
		case Client::CUIBurn::STOVE_LOAD:
		{
			m_pUImanager->Render_UI(CUImanager::Layer_LoadingBar);

			

		}
		break;
		}
	}
	

	return S_OK;
}

HRESULT CUIBurn::Make_UI()
{
	wstring LayerName = TEXT("");
	//inStove
	LayerName = TEXT("Layer_Burn");

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

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryPickLight");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * 0.1f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryPickLight");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * -0.2f, 0.f));

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

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"),&m_pButton[0], &ActiveDesc)))
		return E_FAIL;


	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
	ActiveDesc.Icon_ID = 2;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(140.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.55f, m_fY * -0.85f, 0.f));
	ActiveDesc.bhasText = true;

	TextDesc.TextTag = TEXT("불피우기");
	TextDesc.Font = TEXT("Font_Bold12");
	TextDesc.TextPosition = _float2(m_fX * 0.55f, m_fY * -0.85f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"),&m_pButton[1], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Stove_Button");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f , 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.6f, m_fY * 0.4f, 0.f));
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


	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(100.f, 100.f, 1.f) * XMMatrixTranslation(m_fX * -0.9f, m_fY * 0.85f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	//아이콘
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(100.f, 100.f, 1.f) * XMMatrixTranslation(m_fX * -0.9f, m_fY * 0.85f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	//NAME
	UITextDesc.Font = TEXT("Font_Bold18");
	UITextDesc.TextTag = TEXT("불 켜기");
	UITextDesc.TextPosition = { m_fX * -0.75f, m_fY * 0.85f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))	
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("불붙이기");
	UITextDesc.TextPosition = { m_fX * -0.5f, m_fY * 0.5f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("불쏘시개");
	UITextDesc.TextPosition = { m_fX * -0.5f, m_fY * 0.2f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("연료");
	UITextDesc.TextPosition = { m_fX * -0.5f, m_fY * -0.1f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//설명 배경


	return S_OK;
}

HRESULT CUIBurn::Make_Load()
{
	wstring LayerName = TEXT("");
	//inStove
	LayerName = TEXT("Layer_Load");

	m_wLayerName.push_back(LayerName);

	CUIColor::UI_COLOR_DESC UIBackColorDesc = {};
	CUIBack::UI_BACK_DESC UIBackDesc = {};
	CUITEXT::UI_DESC UITextDesc = {};

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

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT("지속 시간");
	UITextDesc.TextPosition = { m_fX * 0.6f, m_fY * 0.7f };
	UITextDesc.Color = { 0.7f, 0.7f, 0.7f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT(" 30 M");
	UITextDesc.TextPosition = { m_fX * 0.6f, m_fY * 0.5f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 0.6f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &m_pTimeUI, &UITextDesc)))
		return E_FAIL;


	CUIStoveItem::STOVEITEMUI_DESC StoveItemDesc = {};

	StoveItemDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryItemCase");
	StoveItemDesc.UIID = -1;//13++
	StoveItemDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&StoveItemDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(100.f, 100.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * 0.4f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_StoveItemcase"), TEXT("Prototype_GameObject_UIStoveItem"), &m_pIcon[0], &StoveItemDesc)))
		return E_FAIL;

	StoveItemDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryItemCase");
	StoveItemDesc.UIID = -1;//13++
	StoveItemDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&StoveItemDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * 0.1f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_StoveItemcase"), TEXT("Prototype_GameObject_UIStoveItem"), &m_pIcon[1], &StoveItemDesc)))
		return E_FAIL;

	StoveItemDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryItemCase");
	StoveItemDesc.UIID = -1;//13++
	StoveItemDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&StoveItemDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * -0.2f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_StoveItemcase"), TEXT("Prototype_GameObject_UIStoveItem"), &m_pIcon[2], &StoveItemDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIBurn::Make_SelectUI()
{
	wstring LayerName = TEXT("");
	//inStove
	LayerName = TEXT("Layer_select");

	m_wLayerName.push_back(LayerName);
	

	CUIColor::UI_COLOR_DESC UIBackColorDesc = {};
	UIBackColorDesc.Icon_ID = (_uint)2;
	XMStoreFloat4x4(&UIBackColorDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(700.f, 300.f, 1.f) * XMMatrixTranslation(0, m_fY*-0.5f, 0.0f));
	UIBackColorDesc.vColor = _float4(0.f, 0.f, 0.f, 0.6f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIColor"), &UIBackColorDesc)))
		return E_FAIL;


	CUIActiveButton::ACTIVEUI_DESC ActiveDesc = {};

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.3f, m_fY * -0.5f, 0.f));
	ActiveDesc.bhasText = true;
	CUITEXT::UI_DESC TextDesc{};
	TextDesc.TextTag = TEXT("연료 추가");
	TextDesc.Font = TEXT("Font_Bold18");
	TextDesc.TextPosition = _float2(m_fX * -0.3f, m_fY * -0.75f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pSelector[0], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	ActiveDesc.Icon_ID = 4;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.f, m_fY * -0.5f, 0.f));
	ActiveDesc.bhasText = true;

	TextDesc.TextTag = TEXT("요리하기");
	TextDesc.Font = TEXT("Font_Bold18");
	TextDesc.TextPosition = _float2(m_fX * -0.f, m_fY * -0.75f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pSelector[1], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_TYPESelectorIcon");
	ActiveDesc.Icon_ID = 5;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * 0.3f, m_fY * -0.5f, 0.f));
	ActiveDesc.bhasText = true;

	TextDesc.TextTag = TEXT("물 끓이기");
	TextDesc.Font = TEXT("Font_Bold18");
	TextDesc.TextPosition = _float2(m_fX * 0.3f, m_fY * -0.75f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pSelector[2], &ActiveDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIBurn::Make_FuelUI()
{
	wstring LayerName = TEXT("");
	//inStove
	LayerName = TEXT("Layer_Fuel");

	m_wLayerName.push_back(LayerName);


	//CUIColor::UI_COLOR_DESC UIBackColorDesc = {};
	//UIBackColorDesc.Icon_ID = (_uint)2;
	//XMStoreFloat4x4(&UIBackColorDesc.vPrePosition,
	//	XMMatrixIdentity() * XMMatrixScaling(500.f, 300.f, 1.f) * XMMatrixTranslation(m_fX * 0.f, m_fY * -0.5f, 0.0f));
	//UIBackColorDesc.vColor = _float4(0.f, 0.f, 0.f, 0.6f);
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIColor"), &UIBackColorDesc)))
	//	return E_FAIL;


	CUIBack::UI_BACK_DESC UIBackDesc = {};

	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventoryback");
	UIBackDesc.Icon_ID = (_uint)0;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(g_iWinSizeX, g_iWinSizeY, 1.f) * XMMatrixTranslation(0, 0, 0.0f));
	UIBackDesc.vColor = _float4(0.f, 0.f, 0.f, 1.0f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexBack");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	CUIObject::UI_OBJECT_DESC UIDesc = {};
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryPickLight");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * 0.4f, 0.f));

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

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pFuelButton[0], &ActiveDesc)))
		return E_FAIL;


	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
	ActiveDesc.Icon_ID = 2;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(140.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.55f, m_fY * -0.85f, 0.f));
	ActiveDesc.bhasText = true;

	TextDesc.TextTag = TEXT("연료추가");
	TextDesc.Font = TEXT("Font_Bold12");
	TextDesc.TextPosition = _float2(m_fX * 0.55f, m_fY * -0.85f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pFuelButton[1], &ActiveDesc)))
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

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pFuelButton[2], &ActiveDesc)))
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

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pFuelButton[3], &ActiveDesc)))
		return E_FAIL;


	CUIStoveItem::STOVEITEMUI_DESC StoveItemDesc = {};

	StoveItemDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryItemCase");
	StoveItemDesc.UIID = -1;//13++
	StoveItemDesc.Icon_ID = 0;//텍스처는 한장만 쓸거임 (아이콘 텍스처) 무조건 0
	XMStoreFloat4x4(&StoveItemDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(100.f, 100.f, 1.f) * XMMatrixTranslation(m_fX * -0.7f, m_fY * 0.4f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIStoveItem"), &m_pFuelIcon[0], &StoveItemDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUIBurn::Make_CookedUI()
{
	wstring LayerName = TEXT("");
	//inStove
	LayerName = TEXT("Layer_cooked");

	m_wLayerName.push_back(LayerName);


	CUIColor::UI_COLOR_DESC UIBackColorDesc = {};
	UIBackColorDesc.Icon_ID = (_uint)2;
	XMStoreFloat4x4(&UIBackColorDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(500.f, 300.f, 1.f) * XMMatrixTranslation(m_fX*0.f, m_fY * -0.5f, 0.0f));
	UIBackColorDesc.vColor = _float4(0.f, 0.f, 0.f, 0.6f);
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIColor"), &UIBackColorDesc)))
		return E_FAIL;


	CUIObject::UI_OBJECT_DESC UIDesc = {};
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryPickLight");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * 0.f, m_fY * -0.5f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;



	CUIActiveButton::ACTIVEUI_DESC ActiveDesc = {};

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_ico_GearItem_RawRabbit");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(80.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.f, m_fY * -0.5f, 0.f));
	ActiveDesc.bhasText = true;
	CUITEXT::UI_DESC TextDesc{};
	TextDesc.TextTag = TEXT("토끼 고기");
	TextDesc.Font = TEXT("Font_Bold18");
	TextDesc.TextPosition = _float2(m_fX * -0.f, m_fY * -0.65f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pFood[0], &ActiveDesc)))
		return E_FAIL;


	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
	ActiveDesc.Icon_ID = 2;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(140.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.55f, m_fY * -0.85f, 0.f));
	ActiveDesc.bhasText = true;

	TextDesc.TextTag = TEXT("요리하기");
	TextDesc.Font = TEXT("Font_Bold12");
	TextDesc.TextPosition = _float2(m_fX * 0.55f, m_fY * -0.85f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pFood[1], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
	ActiveDesc.Icon_ID = 2;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(140.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.8f, m_fY * -0.85f, 0.f));
	ActiveDesc.bhasText = true;

	TextDesc.TextTag = TEXT("뒤로가기");
	TextDesc.Font = TEXT("Font_Bold12");
	TextDesc.TextPosition = _float2(m_fX * 0.8f, m_fY * -0.85f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pFood[2], &ActiveDesc)))
		return E_FAIL;




	return S_OK;
}

HRESULT CUIBurn::Make_BoilUI()
{
	wstring LayerName = TEXT("");
	//inStove
	LayerName = TEXT("Layer_Boil");

	m_wLayerName.push_back(LayerName);

	return S_OK;
}



CUIBurn* CUIBurn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CUIBurn* pInstance = new CUIBurn(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CUIBurn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIBurn::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pPlayerInventory);
	Safe_Release(m_pUImanager);

	Safe_Release(m_pName);
	Safe_Release(m_pTimeUI);


	for (auto& pGameObject : m_pFood)
		Safe_Release(pGameObject);

	m_pFood.clear();

	for (auto& pGameObject : m_pButton)
		Safe_Release(pGameObject);

	m_pButton.clear();

	for (auto& pGameObject : m_pIcon)
		Safe_Release(pGameObject);

	m_pIcon.clear();

	for (auto& pGameObject : m_pSelector)
		Safe_Release(pGameObject);

	m_pSelector.clear();


	for (auto& pGameObject : m_pFuelIcon)
		Safe_Release(pGameObject);

	m_pFuelIcon.clear();



	for (auto& pGameObject : m_pFuelButton)
		Safe_Release(pGameObject);

	m_pFuelButton.clear();




}
