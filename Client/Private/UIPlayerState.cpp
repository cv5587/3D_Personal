#include "UIPlayerState.h"
#include "GameInstance.h"
#include "NPCManager.h"
#include "UIBack.h"
CUIPlayerState::CUIPlayerState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance{ CGameInstance::GetInstance() },
	m_pDevice{ pDevice },
	m_pContext{ pContext }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CUIPlayerState::Initialize(void* pArg)
{
	UISTATE_DESC* pDesc = static_cast<UISTATE_DESC*>(pArg);

	m_pNPCManager = static_cast<CNPCManager*>(pDesc->pNPCManager);
	Safe_AddRef(m_pNPCManager);


	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;
	m_pText.resize(1);

	if (FAILED(Make_UI()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUIPlayerState::Tick(_float fTimeDelta)
{
	wstring Name =m_pNPCManager->Get_CurrentQuestName();
	dynamic_cast<CUITEXT*>(m_pText[0])->Set_Text(Name);
	return S_OK;
}

HRESULT CUIPlayerState::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_UISelector[0]);

	return S_OK;
}

HRESULT CUIPlayerState::Make_UI()
{
	wstring LayerName = TEXT("Layer_QuestStateINFO");

	m_UISelector.push_back(LayerName);


	CUIBack::UI_BACK_DESC UIBackDesc = {};
	UIBackDesc.TextureTag = TEXT("Prototype_Component_Texture_QuestName");
	UIBackDesc.Icon_ID = 0;
	XMStoreFloat4x4(&UIBackDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(300.f, 100.f, 1.f) * XMMatrixTranslation(m_fX * 0.85f, m_fY * 0.6f, 0.f));
	UIBackDesc.vColor = _float4(0.f, 0.f, 0.f, 1.0f);
	UIBackDesc.ShaderTag = TEXT("Prototype_Component_Shader_VtxPosTexBack");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIBack"), &UIBackDesc)))
		return E_FAIL;

	//CUIObject::UI_OBJECT_DESC UIDesc = {};
	//UIDesc.TextureTag = TEXT("Prototype_Component_Texture_QuestName");
	//UIDesc.Icon_ID = 0;
	//XMStoreFloat4x4(&UIDesc.vPrePosition,
	//	XMMatrixIdentity() * XMMatrixScaling(300.f, 100.f, 1.f) * XMMatrixTranslation(m_fX * 0.85f, m_fY * 0.6f, 0.f));

	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIObject"), &UIDesc)))
	//	return E_FAIL;

	CUITEXT::UI_DESC UITextDesc = {};

	//NAME
	UITextDesc.Font = TEXT("Font_Bold14");
	UITextDesc.TextTag = TEXT("");
	UITextDesc.TextPosition = { m_fX * 0.8f, m_fY * 0.6f };
	UITextDesc.Color = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"),&m_pText[0], & UITextDesc)))
		return E_FAIL;

	UITextDesc.Font = TEXT("Font_Bold10");
	UITextDesc.TextTag = TEXT("¸ñÇ¥");
	UITextDesc.TextPosition = { m_fX * 0.645f, m_fY * 0.655f };
	UITextDesc.Color = { 0.8f, 0.8f, 0.f, 1.f };
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_UIText"),  &UITextDesc)))
		return E_FAIL;

	return S_OK;
}

CUIPlayerState* CUIPlayerState::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CUIPlayerState* pInstance = new CUIPlayerState(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created :CUIPlayerState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPlayerState::Free()
{
	for (auto iter : m_pText)
		Safe_Release(iter);


	Safe_Release(m_pNPCManager);


	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
