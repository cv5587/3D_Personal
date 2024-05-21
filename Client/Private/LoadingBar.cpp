#include "LoadingBar.h"
#include "GameInstance.h"
#include "UImanager.h"

CLoadingBar::CLoadingBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pGameInstance{ CGameInstance::GetInstance() },
    m_pDevice{ pDevice },
    m_pContext{ pContext }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CLoadingBar::Initialize(void* pArg)
{
    LOADINGBAR_DESC* pDesc = static_cast<LOADINGBAR_DESC*>(pArg);

    m_pUImanager = pDesc->pUImanager;
    Safe_AddRef(m_pUImanager);

    m_pCurrentLoadingBar = pDesc->pCurrentLoadingBar;

    dynamic_cast<CUILoadingBar*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_LoadingBar"), 1))->Set_CurrentBar(m_pCurrentLoadingBar);


    return S_OK;


}

HRESULT CLoadingBar::Tick(_float fTimeDelta)
{



    m_pUImanager->Render_UI(CUImanager::Layer_LoadingBar);  
    return S_OK;
}



HRESULT CLoadingBar::Render()
{
    return S_OK;
}

CLoadingBar* CLoadingBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
    CLoadingBar* pInstance = new CLoadingBar(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Created :CLoadingBar");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLoadingBar::Free()
{
    Safe_Release(m_pUImanager);

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
