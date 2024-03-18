#include "stdafx.h"
#include "FreeCamera.h"

CFreeCamera::CFreeCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{pDevice,pContext}
{
}

CFreeCamera::CFreeCamera(const CFreeCamera& rhs)
    :CCamera{rhs}
{
}

HRESULT CFreeCamera::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CFreeCamera::Initialize(void* pArg)
{
	FREE_CAMERA_DESC* pDesc = (FREE_CAMERA_DESC*)pArg;

	m_fSensor = pDesc->fSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    return S_OK;
}

void CFreeCamera::Priority_Tick(_float fTimeDelta)
{
}

void CFreeCamera::Tick(_float fTimeDelta)
{
}

void CFreeCamera::Late_Tick(_float fTimeDelta)
{
}

HRESULT CFreeCamera::Render()
{
    return S_OK;
}

CFreeCamera* CFreeCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFreeCamera* pInstance = new CFreeCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CFreeCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFreeCamera::Clone(void* pArg)
{
	CFreeCamera* pInstance = new CFreeCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CFreeCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFreeCamera::Free()
{
	__super::Free();
}
