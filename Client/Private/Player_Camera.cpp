#include "stdafx.h"
#include "Player_Camera.h"
#include "GameInstance.h"
#include "Bone.h"

CPlayer_Camera::CPlayer_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{pDevice,pContext}
{
}

CPlayer_Camera::CPlayer_Camera(const CPlayer_Camera& rhs)
    :CCamera{rhs}
{
}

HRESULT CPlayer_Camera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayer_Camera::Initialize(void* pArg)
{
	PLAYER_CAMERA_DESC* pDesc = (PLAYER_CAMERA_DESC*)pArg;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pParentMatrix = pDesc->pParentMatrix;

	m_pCamBone = pDesc->pCamBone;
	//카메라를 만들어서 본에 박아 주면 180도 돌아간 상태로 되있음 
	//그래서 또한번 180 도 돌려서 기본상태로 제작해줌
	XMStoreFloat4x4(&m_fixWorld, (XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, 0.01f, -0.3f)));

	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&m_fixWorld)*XMLoadFloat4x4(m_pCamBone->Get_CombinedTransformationMatrix()) * XMLoadFloat4x4(m_pParentMatrix));

	return S_OK;
}

void CPlayer_Camera::Priority_Tick(_float fTimeDelta)
{
}

void CPlayer_Camera::Tick(_float fTimeDelta)
{

	_long		MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y);
	if (0!=MouseMove )
	{
		_matrix  Socket = XMLoadFloat4x4(m_pCamBone->Get_ControlBoneMatrix());

		_vector		vRight = Socket.r[0];
		_vector		vUp = Socket.r[1];
		_vector		vLook = Socket.r[2];

		_matrix		RotationMatrix = XMMatrixRotationAxis(vRight, -0.01 * fTimeDelta* MouseMove);

		vRight = XMVector3TransformNormal(vRight, RotationMatrix);
		vUp = XMVector3TransformNormal(vUp, RotationMatrix);	
		vLook = XMVector3TransformNormal(vLook, RotationMatrix);	

		memcpy(&Socket.r[0], &vRight, sizeof(_vector));
		memcpy(&Socket.r[1], &vUp, sizeof(_vector));
		memcpy(&Socket.r[2], &vLook, sizeof(_vector));

		m_pCamBone->Set_TransformationMatrix(Socket);
	}
	
	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&m_fixWorld)*XMLoadFloat4x4(m_pCamBone->Get_CombinedTransformationMatrix()) * XMLoadFloat4x4(m_pParentMatrix));


	
    __super::Tick(fTimeDelta);

}

void CPlayer_Camera::Late_Tick(_float fTimeDelta)
{
}

HRESULT CPlayer_Camera::Render()
{
    return S_OK;
}


CPlayer_Camera* CPlayer_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Camera* pInstance = new CPlayer_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created :Player_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Camera::Clone(void* pArg)
{
	CPlayer_Camera* pInstance = new CPlayer_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Player_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Camera::Free()
{
	__super::Free();
}

