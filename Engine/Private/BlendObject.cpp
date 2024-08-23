#include "..\Public\BlendObject.h"
#include "GameInstance.h"

CBlendObject::CBlendObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CBlendObject::CBlendObject(const CBlendObject& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CBlendObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlendObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBlendObject::Priority_Tick(_float fTimeDelta)
{
}

void CBlendObject::Tick(_float fTimeDelta)
{
}

void CBlendObject::Late_Tick(_float fTimeDelta)
{
}

HRESULT CBlendObject::Render()
{
	return S_OK;
}

void CBlendObject::Compute_ViewZ(_fvector vWorldPos)
{
	m_fViewZ = XMVectorGetX(XMVector3Length(vWorldPos - m_pGameInstance->Get_CamPosition()));
}

void CBlendObject::Free()
{
	__super::Free();

}
