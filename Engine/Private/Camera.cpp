#include "Camera.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{pDevice,pContext}
{
}

CCamera::CCamera(const CCamera& rhs)
    :CGameObject{rhs}
{
}

HRESULT CCamera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{

	CAMERA_DESC* pDesc = (CAMERA_DESC*)pArg;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* 카메라의 트랜스폼에 카메라의 초기 상태를 셋팅했다. */
	/* 카메라의 월드행렬의 역행렬 == 뷰 스페이스변환 행렬. */
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pDesc->vEye));
	m_pTransformCom->LookAt(XMLoadFloat4(&pDesc->vAt));

	m_fFovy = pDesc->fFovy;
	m_fAspect = pDesc->fAspect;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;

	return S_OK;
}

void CCamera::Priority_Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State_Matrix(m_pGameInstance->Get_Transform_Matrix(CPipeLine::TS_CAMWORLD));
}

void CCamera::Tick(_float fTimeDelta)
{
	m_pGameInstance->Set_Transform(CPipeLine::TS_CAMWORLD,m_pTransformCom->Get_WorldMatrix());
	m_pGameInstance->Set_Transform(CPipeLine::TS_VIEW, XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix()));
	m_pGameInstance->Set_Transform(CPipeLine::TS_PROJ, XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar));
}

void CCamera::Late_Tick(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
    return S_OK;
}

void CCamera::Free()
{
	__super::Free();
}
