#include "UImanager.h"
#include "GameInstance.h"
#include "UIObject.h"

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
	UIDesc.ProtoTypeTag = TEXT("Prototype_GameObject_SelectorIcon");
	UIDesc.Icon_ID = (_uint)KEY_LEFTCLICK;
	XMStoreFloat4x4(&UIDesc.vPrePosition, 
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(-m_fX * 0.3f, -m_fY * 0.4f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_SelectorIcon"), &UIDesc)))
		return E_FAIL;




	UIDesc.ProtoTypeTag = TEXT("Prototype_GameObject_SelectorIcon");
	UIDesc.Icon_ID = (_uint)KEY_RIGHTCLICK;
	XMStoreFloat4x4(&UIDesc.vPrePosition,
		XMMatrixIdentity() * XMMatrixScaling(60.f, 60.f, 1.f) * XMMatrixTranslation(m_fX * 0.3f, -m_fY * 0.4f, 0.f));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, LayerName, TEXT("Prototype_GameObject_SelectorIcon"), &UIDesc)))
		return E_FAIL;


	//BASE_ID
	return S_OK;
}

void CUImanager::Render_KeyUI(_uint LayerIndex)
{
	m_pGameInstance->Render_UI(LEVEL_GAMEPLAY, m_UISelector[Layer_KEY]);
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
