#include "UISleep.h"
#include "GameInstance.h"
#include "UIActiveButton.h"
#include "Player.h"
CUISleep::CUISleep(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance{ CGameInstance::GetInstance() },
	m_pDevice{ pDevice },
	m_pContext{ pContext }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CUISleep::Initialize(void* pArg)
{
	UISLEEP_DESC* pDesc = static_cast<UISLEEP_DESC*>(pArg);

	m_pUImanager = static_cast<CUImanager*>(pDesc->pUImanager);

	Safe_AddRef(m_pUImanager);

	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;
	m_pButton.resize(3);
	m_pText.resize(1);

	if (FAILED(Make_UI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUISleep::Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
	switch (m_iState)
	{
	case SLEEP_SELECT:

		break;
	case SLEEP_LOAD:
	{
		pPlayer->Add_EnterTime(fTimeDelta);
		if (pPlayer->isLoadFinished())
		{
			pPlayer->CurrentLoadingBarReset();
			m_iState = SLEEP_SELECT;
			m_iTime = 1;
			dynamic_cast<CUITEXT*>(m_pText[0])->Set_Text(to_wstring(m_iTime));
			pPlayer->Set_State(PLAYER_IDLE);	
		}
	}
	break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CUISleep::Late_Tick(_float fTimeDelta)
{
	
	m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[0]);

	switch (m_iState)
	{
	case SLEEP_SELECT:  
		break;
	case SLEEP_LOAD:	
		m_pUImanager->Render_UI(CUImanager::Layer_FadeIn);		

		m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[1]);
		break;
	}
	return S_OK;
}

HRESULT CUISleep::Pick_UISleep()
{

	for (_int i = 0; i < m_pButton.size(); i++)
	{
		if (m_pButton[i]->IntersectUI())
		{
			switch (i)
			{
			case 0://--
			{
				if (1 < m_iTime)
				{
					m_iTime--;
					dynamic_cast<CUITEXT*>(m_pText[0])->Set_Text(to_wstring(m_iTime));	
				}
				break;
			}
			case 1://++
			{
				if (8 > m_iTime)
				{
					m_iTime++;
					dynamic_cast<CUITEXT*>(m_pText[0])->Set_Text(to_wstring(m_iTime));	
				}
				break;
			}
			case 2://자기
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
				pPlayer->CurrentLoadingBarReset();
				m_iState = SLEEP_LOAD;
			}
				break;
			default:
				break;
			}
		}
	}
	
	return S_OK;
}

HRESULT CUISleep::Make_UI()
{

	wstring LayerName = TEXT("");
	//inStove
	LayerName = TEXT("Layer_Sleep");

	m_wLayerName.push_back(LayerName);


	CUIColor::UI_COLOR_DESC UIBackColorDesc = {};
	UIBackColorDesc.Icon_ID = (_uint)2;
	XMStoreFloat4x4(&UIBackColorDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(500.f, 400.f, 1.f) * XMMatrixTranslation(0, 0, 0.0f));
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
		XMMatrixIdentity() * XMMatrixScaling(500.f, 400.f, 1.f) * XMMatrixTranslation(m_fX * 0.0f, m_fY * 0.0f, 0.f));
	UIBackDesc.vColor = _float4(0.29f, 0.41f, 0.33f, 0.3f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	//시간배경
	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryDetailCase");
	UIBackDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(100.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.f, m_fY * 0.0f, 0.f));
	UIBackDesc.vColor = _float4(0.3f, 0.34f, 0.33f, 0.5f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	//온도 배경
	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryDetailCase");
	UIBackDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(350.f, 100.f, 1.f) * XMMatrixTranslation(m_fX * 0.f, m_fY * -0.4f, 0.f));
	UIBackDesc.vColor = _float4(0.3f, 0.34f, 0.33f, 0.5f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexColor");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	CUITEXT::UI_DESC UITextDesc = {};
	//이름
	UITextDesc.Font = TEXT("Font_Bold18");
	UITextDesc.TextTag = TEXT("꿀잠 자기");
	UITextDesc.TextPosition = { m_fX * 0.f, m_fY * 0.5f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;
	//설명
	UITextDesc.Font = TEXT("Font_Normal14");
	UITextDesc.TextTag = TEXT("잠을 자면 피로를 줄이고 시간을 보낼수 있다.\n 건강할 경우에는 체력도 회복한다");
	UITextDesc.TextPosition = { m_fX * 0.4f,  m_fY * 0.3f };
	UITextDesc.Color = { 0.8f, 0.8f, 0.8f, 1.f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//체감온도
	UITextDesc.Font = TEXT("Font_Bold10");
	UITextDesc.TextTag = TEXT("체감 온도");
	UITextDesc.TextPosition = { m_fX * -0.15f,  m_fY * -0.4f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//침대 온기 보너스
	UITextDesc.Font = TEXT("Font_Bold10");
	UITextDesc.TextTag = TEXT("온기 보너스");
	UITextDesc.TextPosition = { m_fX * 0.05f,  m_fY * -0.4f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//도씨
	UITextDesc.Font = TEXT("Font_Bold10");
	UITextDesc.TextTag = TEXT("C");
	UITextDesc.TextPosition = { m_fX * -0.05f,  m_fY * -0.4f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;
	//도씨
	UITextDesc.Font = TEXT("Font_Bold10");
	UITextDesc.TextTag = TEXT("C");
	UITextDesc.TextPosition = { m_fX * 0.15f,  m_fY * -0.4f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//체감온도 온도
	UITextDesc.Font = TEXT("Font_Bold10");
	UITextDesc.TextTag = TEXT("-10");
	UITextDesc.TextPosition = { m_fX * -0.075f,  m_fY * -0.4f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//온기 보너스 온도
	UITextDesc.Font = TEXT("Font_Bold10");
	UITextDesc.TextTag = TEXT("+5");
	UITextDesc.TextPosition = { m_fX * 0.125f,  m_fY * -0.4f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;

	//시간
	UITextDesc.Font = TEXT("Font_Bold18");
	UITextDesc.TextTag = TEXT("1");
	UITextDesc.TextPosition = { m_fX * 0.0f,  m_fY * 0.0f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"),&m_pText[0], & UITextDesc)))
		return E_FAIL;

	//--
	CUIActiveButton::ACTIVEUI_DESC ActiveDesc = {};
	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Stove_Button");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(-60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * -0.2f, m_fY * 0.0f, 0.f));
	ActiveDesc.bhasText = false;
	CUITEXT::UI_DESC TextDesc{};	
	ActiveDesc.TEXTDesc = &TextDesc;	

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[0], &ActiveDesc)))
		return E_FAIL;
	//++

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Stove_Button");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * 0.2f, m_fY * 0.0f, 0.f));
	ActiveDesc.bhasText = false;
	 TextDesc={};
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[1], &ActiveDesc)))
		return E_FAIL;

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Inventorybutton");
	ActiveDesc.Icon_ID = 2;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(140.f, 80.f, 1.f) * XMMatrixTranslation(m_fX * 0.0f, m_fY * -0.7f, 0.f));
	ActiveDesc.bhasText = true;

	TextDesc.TextTag = TEXT("잠 자기");
	TextDesc.Font = TEXT("Font_Bold12");
	TextDesc.TextPosition = _float2(m_fX * 0.f, m_fY * -0.7f);
	TextDesc.Color = _float4{ 1.f, 1.f, 1.f, 1.f };
	ActiveDesc.TEXTDesc = &TextDesc;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[2], &ActiveDesc)))
		return E_FAIL;

	LayerName = TEXT("Layer_SleepLoad");

	m_wLayerName.push_back(LayerName);



	CUIObject::UI_OBJECT_DESC UIDesc = {};
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_Sleep");
	UIDesc.Icon_ID = (_uint)0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * 0.f, m_fY * 0.f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;


	UITextDesc.Font = TEXT("Font_Bold12");
	UITextDesc.TextTag = TEXT("잠 자는중");
	UITextDesc.TextPosition = { m_fX * 0.0f, m_fY * -0.2f };
	UITextDesc.Color = { 0.6f, 0.6f, 0.6f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"), &UITextDesc)))
		return E_FAIL;



	return S_OK;
}

CUISleep* CUISleep::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CUISleep* pInstance = new CUISleep(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CUISleep");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUISleep::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pUImanager);

	for (auto iter : m_pButton)
		Safe_Release(iter);

	for (auto iter : m_pText)
		Safe_Release(iter);

}
