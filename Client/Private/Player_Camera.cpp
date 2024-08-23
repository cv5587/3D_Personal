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

	m_pState = pDesc->pState;


	//ī�޶� ���� ���� �ھ� �ָ� 180�� ���ư� ���·� ������ 
	//�׷��� ���ѹ� 180 �� ������ �⺻���·� ��������
	//XMStoreFloat4x4(&m_fixWorld, (XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, 0.01f, -0.25f)));

	//m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&m_fixWorld)*XMLoadFloat4x4(m_pCamBone->Get_CombinedTransformationMatrix()) * XMLoadFloat4x4(m_pParentMatrix));

	return S_OK;
}

void CPlayer_Camera::Priority_Tick(_float fTimeDelta)
{
}

void CPlayer_Camera::Tick(_float fTimeDelta)
{
	//m_pCamBone camera ��
	//pEyeBoneMatrix Camera_Weapon_Offset ��

	//XMStoreFloat4x4(&m_fixWorld, ( XMMatrixTranslation(0.f, 0.0f, 0.05f)*XMLoadFloat4x4(m_pEyeBoneMatrix) ));


	if(*m_pState==PLAYER_STRUGGLE || *m_pState == PLAYER_STRUGGLEBARE || *m_pState == PLAYER_STRUGGLEKNIFE)
	{
		m_pTransformCom->Set_State_Matrix(
			XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, 0.0f, -0.1f) 
			 *XMLoadFloat4x4(m_pCamBone->Get_CombinedTransformationMatrix())
			//* XMLoadFloat4x4(m_pEyeBoneMatrix)
			* XMLoadFloat4x4(m_pParentMatrix));

	}
	else
	{
		m_pTransformCom->Set_State_Matrix(
			 XMLoadFloat4x4(m_pCamBone->Get_CombinedTransformationMatrix())
			* XMLoadFloat4x4(m_pEyeBoneMatrix)
			* XMLoadFloat4x4(m_pParentMatrix));

	}

	//m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&m_fixWorld)
	//	*XMLoadFloat4x4(m_pCamBone->Get_CombinedTransformationMatrix()) * XMLoadFloat4x4(m_pParentMatrix));	

    __super::Tick(fTimeDelta);
}

void CPlayer_Camera::Late_Tick(_float fTimeDelta)
{
}

HRESULT CPlayer_Camera::Render()
{
    return S_OK;
}

_vector CPlayer_Camera::Get_CamLook()
{

	return 	m_pTransformCom->Get_State(CTransform::STATE_LOOK);
}

void CPlayer_Camera::Set_CamMatrix(_float fTimeDelta)
{
	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&m_fixWorld) * XMLoadFloat4x4(m_pCamBone->Get_CombinedTransformationMatrix()) * XMLoadFloat4x4(m_pParentMatrix));

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

