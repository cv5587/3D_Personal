#include "stdafx.h"
#include "..\Public\LandObject.h"

#include "GameInstance.h"

CLandObject::CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CLandObject::CLandObject(const CLandObject& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CLandObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	LANDOBJ_DESC* pLandObjDesc = (LANDOBJ_DESC*)pArg;

	m_pTerrainTransform = pLandObjDesc->pTerrainTransform;
	m_pTerrainVIBuffer = pLandObjDesc->pTerrainVIBuffer;

	Safe_AddRef(m_pTerrainTransform);
	Safe_AddRef(m_pTerrainVIBuffer);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLandObject::Priority_Tick(_float fTimeDelta)
{
}

void CLandObject::Tick(_float fTimeDelta)
{
}

void CLandObject::Late_Tick(_float fTimeDelta)
{
}

HRESULT CLandObject::Render()
{
	return S_OK;
}


HRESULT CLandObject::SetUp_OnTerrain(CTransform* pTransform)
{
	if (nullptr == m_pTerrainTransform)
		return S_OK;

	_vector			vWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);
	_float3			vLocalPos;

	XMStoreFloat3(&vLocalPos, XMVector3TransformCoord(vWorldPos, m_pTerrainTransform->Get_WorldMatrix_Inverse()));

	vLocalPos.y = m_pTerrainVIBuffer->Compute_Height(vLocalPos);

	pTransform->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat3(&vLocalPos), m_pTerrainTransform->Get_WorldMatrix()));

	return S_OK;
}

void CLandObject::Free()
{
	__super::Free();

	Safe_Release(m_pTerrainTransform);
	Safe_Release(m_pTerrainVIBuffer);
}
