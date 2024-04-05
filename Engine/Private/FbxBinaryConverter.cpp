#include "FbxBinaryConverter.h"

CFbxBinaryConverter::CFbxBinaryConverter()
{
}

HRESULT CFbxBinaryConverter::Initialize()
{
    return S_OK;
}

HRESULT CFbxBinaryConverter::Save_Binary(void* pArg)
{

    return S_OK;
}

HRESULT CFbxBinaryConverter::Load_Binary()
{
    return S_OK;
}

CFbxBinaryConverter* CFbxBinaryConverter::Create()
{
	CFbxBinaryConverter* pInstance = new CFbxBinaryConverter();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CFbxBinaryConverter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFbxBinaryConverter::Free()
{
}
