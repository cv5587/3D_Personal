#include "ImGuiManager.h"

#include "Gui.h"

IMPLEMENT_SINGLETON(CImGuiManager)

CImGuiManager::CImGuiManager()
{
}

HRESULT CImGuiManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	m_pImGui = CGui::Create(m_pDevice, m_pContext);

	return S_OK;
}

HRESULT CImGuiManager::Tick(_float fTimeDelta)
{
	m_pImGui->Update_UI(fTimeDelta);
	return S_OK;
}

HRESULT CImGuiManager::Late_Tick()
{
	m_pImGui->LateUpdate_UI();
	return S_OK;
}

HRESULT CImGuiManager::Render()
{
	m_pImGui->Render();
	return S_OK;
}

void CImGuiManager::Make_Terrain(void* pArg)
{
	m_pImGui->Make_Terrain(pArg);
}

void CImGuiManager::Free()
{
	Safe_Release(m_pImGui);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
