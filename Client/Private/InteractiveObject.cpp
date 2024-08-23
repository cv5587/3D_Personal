#include "InteractiveObject.h"
#include "GameInstance.h"
CInteractiveObject::CInteractiveObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem{ pDevice,pContext }
{
}

CInteractiveObject::CInteractiveObject(const CGameObject& rhs)
	:CItem{ rhs }
{
}

HRESULT CInteractiveObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInteractiveObject::Initialize(void* pArg)
{
	INTERACTIVEOBJECT_DESC* pDesc = static_cast<INTERACTIVEOBJECT_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CInteractiveObject::Priority_Tick(_float fTimeDelta)
{
}

void CInteractiveObject::Tick(_float fTimeDelta)
{
}

void CInteractiveObject::Late_Tick(_float fTimeDelta)
{
}

HRESULT CInteractiveObject::Render()
{


	return S_OK;
}

_bool CInteractiveObject::IntersectRay(_vector* pRayArray, _float* fDist)
{
	return true;
}

void CInteractiveObject::Free()
{
	__super::Free();


}
