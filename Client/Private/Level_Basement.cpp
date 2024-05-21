#include "Level_Basement.h"

CLevel_Basement::CLevel_Basement(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Basement::Initialize()
{
    return E_NOTIMPL;
}

void CLevel_Basement::Tick(_float fTimeDelta)
{
#ifdef _DEBUG
    SetWindowText(g_hWnd, TEXT("지하실레벨"));
#endif
}

CLevel_Basement* CLevel_Basement::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Basement* pInstance = new CLevel_Basement(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_Basement");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CLevel_Basement::Free()
{
	__super::Free();
}
