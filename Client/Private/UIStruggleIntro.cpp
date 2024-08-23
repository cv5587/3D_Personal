#include "UIStruggleIntro.h"
#include "GameInstance.h"
#include "UImanager.h"
#include "Player.h"
#include "UIActiveButton.h"
#include "Item.h"
#include "UILoadingStruggle.h"

CUIStruggleIntro::CUIStruggleIntro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance{ CGameInstance::GetInstance() },
	m_pDevice{ pDevice },
	m_pContext{ pContext }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CUIStruggleIntro::Initialize(void* pArg)
{
	STRUGGLEINTRO_DESC* pDesc = static_cast<STRUGGLEINTRO_DESC*>(pArg);

	m_pPlayerInventory = static_cast<CInventory*>(pDesc->pPlayerInventory);
	m_pUImanager = static_cast<CUImanager*>(pDesc->pUImanager);
	Safe_AddRef(m_pPlayerInventory);
	Safe_AddRef(m_pUImanager);

	m_pButton.resize(3);
	m_pLoading.resize(1);
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	if (FAILED(Make_UI()))
		return E_FAIL;

	if (FAILED(Make_UILoad()))
		return E_FAIL;

	dynamic_cast<CUILoadingStruggle*>(m_pLoading[0])->Set_CurrentBar(pDesc->pCurrentBar);

	return S_OK;
}

HRESULT CUIStruggleIntro::Tick(_float fTimeDelta)
{
	CItem::ITEM_DESC ItemDesc{};
	if (m_pPlayerInventory->Check_StoveItem(TEXT("Knife"), &ItemDesc))
		m_bisItem = true;
	else
		m_bisItem = false;

	return S_OK;
}

HRESULT CUIStruggleIntro::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[2]);

	if (m_bisItem)
	{
		m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[0]);//손,무기레이어
	}
	else
	{
		m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_wLayerName[1]);
	}
	return S_OK;
}

HRESULT CUIStruggleIntro::Pick_UIStruggle()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));

	for (_int i = 0; i < m_pButton.size(); i++)
	{
		if (m_pButton[i]->IntersectUI())
		{
			switch (i)
			{
				case 0://손
				{
					pPlayer->Set_StruggleItem(false);
					break;
				}
				case 1://나이프
				{
					pPlayer->Set_StruggleItem(true);
					break;
				}
				case 2://손
				{
					pPlayer->Set_StruggleItem(false);
					break;
				}
			}
		}
	}

	return S_OK;
}

HRESULT CUIStruggleIntro::Make_UI()
{
	wstring LayerName = TEXT("");

	LayerName = TEXT("Layer_StruggleItem");

	m_wLayerName.push_back(LayerName);



	CUIObject::UI_OBJECT_DESC UIDesc = {};
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryPickLight");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.3f, m_fY * -0.f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;


	CUIActiveButton::ACTIVEUI_DESC ActiveDesc = {};

	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Struggle_Hand");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * -0.3f, m_fY * -0.f, 0.f));
	ActiveDesc.bhasText = false;
	CUITEXT::UI_DESC TextDesc{};

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[0], &ActiveDesc)))
		return E_FAIL;


	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryPickLight");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * 0.3f, m_fY * -0.f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;


	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_ico_GearItem_Knife");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * 0.3f, m_fY * -0.f, 0.f));
	ActiveDesc.bhasText = false;
	TextDesc={};

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[1], &ActiveDesc)))
		return E_FAIL;



	LayerName = TEXT("Layer_StruggleBare");

	m_wLayerName.push_back(LayerName);

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_InventoryPickLight");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * 0.0f, m_fY * -0.f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;


	ActiveDesc.TextureTag = TEXT("Prototype_Component_Texture_Struggle_Hand");
	ActiveDesc.Icon_ID = 0;
	ActiveDesc.UIID = -1;
	ActiveDesc.vColor = _float4{ 1.f, 1.f, 1.f, 1.f };
	XMStoreFloat4x4(&ActiveDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(120.f, 120.f, 1.f) * XMMatrixTranslation(m_fX * 0.0f, m_fY * -0.f, 0.f));
	ActiveDesc.bhasText = false;
	TextDesc = {};

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIActiveButton"), &m_pButton[2], &ActiveDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIStruggleIntro::Make_UILoad()
{

	wstring LayerName = TEXT("");

	LayerName = TEXT("Layer_StruggleLoad");

	m_wLayerName.push_back(LayerName);



	CUIObject::UI_OBJECT_DESC UIDesc = {};
	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_Struggle_Back");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(256.f, 64.f, 1.f) * XMMatrixTranslation(m_fX * -0.0f, m_fY * -0.3f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
		return E_FAIL;

	UIDesc.TextureTag = TEXT("Prototype_Component_Texture_Struggle_Bar");
	UIDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(256.f, 64.f, 1.f) * XMMatrixTranslation(m_fX * -0.0f, m_fY * -0.3f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UILoadingStruggle"),&m_pLoading[0] ,& UIDesc)))
		return E_FAIL;
	
	

	return S_OK;
}

CUIStruggleIntro* CUIStruggleIntro::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CUIStruggleIntro* pInstance = new CUIStruggleIntro(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CUIStruggleIntro");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIStruggleIntro::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pPlayerInventory);
	Safe_Release(m_pUImanager);

	for (auto& iter : m_pButton)
		Safe_Release(iter);

	for (auto& iter : m_pLoading)
		Safe_Release(iter);
}
