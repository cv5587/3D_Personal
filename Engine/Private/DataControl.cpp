#include "DataControl.h"

CDataControl::CDataControl()
{
}

HRESULT CDataControl::Initialize()
{
    return S_OK;
}

HRESULT CDataControl::Save_Data(void* pArg)
{

    return S_OK;
}

HRESULT CDataControl::Load_Data()
{
    return S_OK;
}

CDataControl* CDataControl::Create()
{
	CDataControl* pInstance = new CDataControl();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : DataControl");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDataControl::Free()
{
}
