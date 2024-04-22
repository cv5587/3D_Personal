#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"
#include "PartObject.h"
#include "Weapon.h"
#include "Revolver.h"
#include "StateMachine.h"
#include "Player_Camera.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject{ pDevice, pContext }
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CLandObject{ rhs }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	CLandObject::LANDOBJ_DESC* pDesc = (CLandObject::LANDOBJ_DESC*)pArg;

	pDesc->fSpeedPerSec = 2.f;
	pDesc->fRotationPerSec = XMConvertToRadians(120.0f);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pStateMachine = CStateMachine::GetInstance();
	m_pStateMachine->Initialize();

	m_pStateMachine->Set_CurrentState(this, PLAYERSTATE::PLAYER_IDLE);
	//Set_Equip(PLAYEREQUIP::EQUIP_NONE);

	m_fSensor = 0.02f;

	if (FAILED(Add_PartObjects()))
		return E_FAIL;

	/* 플레이어의 Transform이란 녀석은 파츠가 될 바디와 웨폰의 부모 행렬정보를 가지는 컴포넌트가 될거다. */

	return S_OK;
}

void CPlayer::Priority_Tick(_float fTimeDelta)
{
	m_PartObjects[PART_BODY]->Priority_Tick(fTimeDelta);
	switch (m_eEquip)
	{
	case Client::EQUIP_STONE:
		m_PartObjects[PART_STONE]->Priority_Tick(fTimeDelta);
		break;
	case Client::EQUIP_REVOLVER:
		m_PartObjects[PART_REOVLVER]->Priority_Tick(fTimeDelta);
		break;
	case Client::EQUIP_PIPE:
		m_PartObjects[PART_PIPE]->Priority_Tick(fTimeDelta);
		break;
	}
}

void CPlayer::Tick(_float fTimeDelta)
{

	if (m_eEquip != PLAYEREQUIP::EQUIP_STONE)
	{
		if (m_pGameInstance->Get_DIKeyState_Once(DIK_U))
		{
			Set_Equip(PLAYEREQUIP::EQUIP_STONE);
			Set_State(PLAYERSTATE::PLAYER_EQUIP);
		}
	}
	 if (m_eEquip != PLAYEREQUIP::EQUIP_REVOLVER)
	{
		if (m_pGameInstance->Get_DIKeyState_Once(DIK_I))
		{
			Set_Equip(PLAYEREQUIP::EQUIP_REVOLVER);
			Set_State(PLAYERSTATE::PLAYER_EQUIP);
		}
	}

	if (m_pGameInstance->Get_DIKeyState_Once(DIK_P))
	{
		Set_State(PLAYERSTATE::PLAYER_UNEQUIP);
	}

	_long		MouseMove = { 0 };

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fSensor * MouseMove);
	}



	Mouse_Fix();

	m_pStateMachine->Update(this, fTimeDelta);
	

	if (FAILED(__super::SetUp_OnTerrain(m_pTransformCom)))
		return;


	m_PartObjects[PART_BODY]->Tick(fTimeDelta);
	switch (m_eEquip)
	{
	case Client::EQUIP_STONE:
		m_PartObjects[PART_STONE]->Tick(fTimeDelta);
		break;
	case Client::EQUIP_REVOLVER:
		m_PartObjects[PART_REOVLVER]->Tick(fTimeDelta);
		break;
	case Client::EQUIP_PIPE:
		m_PartObjects[PART_PIPE]->Tick(fTimeDelta);
		break;
	}

}

void CPlayer::Late_Tick(_float fTimeDelta)
{

	m_PartObjects[PART_BODY]->Late_Tick(fTimeDelta);
	switch (m_eEquip)
	{
	case Client::EQUIP_STONE:
		m_PartObjects[PART_STONE]->Late_Tick(fTimeDelta);
		break;
	case Client::EQUIP_REVOLVER:
		m_PartObjects[PART_REOVLVER]->Late_Tick(fTimeDelta);
		break;
	case Client::EQUIP_PIPE:
		m_PartObjects[PART_PIPE]->Late_Tick(fTimeDelta);
		break;
	}
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPlayer::Render()
{

	return S_OK;
}

HRESULT CPlayer::Add_Components()
{

	return S_OK;
}

HRESULT CPlayer::Add_PartObjects()
{


	/* 바디객체를 복제해온다. */
	CPartObject::PARTOBJ_DESC		BodyDesc{};
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;
	BodyDesc.pState = &m_eState;
	BodyDesc.pEquip = &m_eEquip;
	BodyDesc.pAnimFinished = &m_bAnimFinished;

	CGameObject* pBody = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Body_Player"), &BodyDesc);
	if (nullptr == pBody)
		return E_FAIL;
	m_PartObjects.emplace_back(pBody);


	/*카메라 생성이지 파트 오브젝트는 아님*/
	CPlayer_Camera::PLAYER_CAMERA_DESC		CameraDesc{};
	CameraDesc.pEyeBoneMatrix =
		dynamic_cast<CModel*>(m_PartObjects[PART_BODY]->Get_Component(TEXT("Com_Model")))->Get_ControlBoneMatrix("Camera_Weapon_Offset");

	CameraDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	CameraDesc.pCamBone =
		dynamic_cast<CModel*>(m_PartObjects[PART_BODY]->Get_Component(TEXT("Com_Model")))->Get_CameraBone("Camera_Weapon_Offset");
	CameraDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	CameraDesc.fFovy = XMConvertToRadians(70.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 3000.f;
	CameraDesc.fSpeedPerSec = 50.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.f);
	XMStoreFloat4x4(&CameraDesc.vPrePosition, XMMatrixIdentity());
	CameraDesc.ProtoTypeTag = TEXT("Prototype_GameObject_PlayerCamera");
	CameraDesc.ModelTag = TEXT("");

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_PlayerCamera"), &CameraDesc)))
		return E_FAIL;


	///*돌*/
	///* 무기객체를 복제해온다. */
	CWeapon::WEAPON_DESC			WeaponDesc{};
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	//WeaponDesc.pState = &m_iState;
	WeaponDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Weapon");
	WeaponDesc.ModelTag = TEXT("Prototype_Component_Model_Stone");
	XMStoreFloat4x4(&WeaponDesc.vPrePosition, XMMatrixIdentity());	
	WeaponDesc.pState = &m_eState;
	WeaponDesc.pEquip = &m_eEquip;
	WeaponDesc.pAnimFinished = &m_bAnimFinished;
	//몸의 모델컴을 가져옴
	CModel* pModelCom = dynamic_cast<CModel*>(pBody->Get_Component(TEXT("Com_Model")));
	if (nullptr == pModelCom)
		return E_FAIL;
	//무기 가 붙어 있을 뼈정보를 가져옴
	WeaponDesc.pCombinedTransformationMatrix = pModelCom->Get_BoneCombinedTransformationMatrix("right_prop_point");
	if (nullptr == WeaponDesc.pCombinedTransformationMatrix)
		return E_FAIL;
	//뼈정보를 넣어줘서 제작
	CGameObject* pWeapon = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Weapon"), &WeaponDesc);
	if (nullptr == pWeapon)
		return E_FAIL;
	m_PartObjects.emplace_back(pWeapon);

	/*리볼버*/
	///* 무기객체를 복제해온다. */
	CRevolver::REVOLVER_DESC			RevolverDesc{};
	RevolverDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	//WeaponDesc.pState = &m_iState;
	RevolverDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Revolver");
	RevolverDesc.ModelTag = TEXT("Prototype_Component_Model_Revolver");
	XMStoreFloat4x4(&RevolverDesc.vPrePosition, XMMatrixIdentity());
	RevolverDesc.pState = &m_eState;
	RevolverDesc.pEquip = &m_eEquip;
	RevolverDesc.pAnimFinished = &m_bRevolver_AnimFin;
	//몸의 모델컴을 가져옴
	pModelCom = dynamic_cast<CModel*>(pBody->Get_Component(TEXT("Com_Model")));
	if (nullptr == pModelCom)
		return E_FAIL;
	//무기 가 붙어 있을 뼈정보를 가져옴
	RevolverDesc.pCombinedTransformationMatrix = pModelCom->Get_BoneCombinedTransformationMatrix("right_prop_point");
	if (nullptr == RevolverDesc.pCombinedTransformationMatrix)
		return E_FAIL;
	//뼈정보를 넣어줘서 제작
	CGameObject* pRevolver = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Revolver"), &RevolverDesc);
	if (nullptr == pRevolver)
		return E_FAIL;
	m_PartObjects.emplace_back(pRevolver);

	return S_OK;
}

void CPlayer::Go_Straight(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);
}

void CPlayer::Go_Backward(_float fTimeDelta)
{
	m_pTransformCom->Go_Backward(fTimeDelta);
}

void CPlayer::Go_Left(_float fTimeDelta)
{
	m_pTransformCom->Go_Left(fTimeDelta);
}

void CPlayer::Go_Right(_float fTimeDelta)
{
	m_pTransformCom->Go_Right(fTimeDelta);
}

void CPlayer::Go_LeftStraight(_float fTimeDelta)
{
	m_pTransformCom->Go_LeftStraight(fTimeDelta);
}

void CPlayer::Go_RightStraight(_float fTimeDelta)
{
	m_pTransformCom->Go_RightStraight(fTimeDelta);
}

void CPlayer::Go_LeftBackward(_float fTimeDelta)
{
	m_pTransformCom->Go_LeftBackward(fTimeDelta);
}

void CPlayer::Go_RightBackward(_float fTimeDelta)
{
	m_pTransformCom->Go_RightBackward(fTimeDelta);
}

void CPlayer::Set_DeltaValue(_float _fSpeedPerSec, _float _fRotationPerSec)
{
	m_pTransformCom->Set_DeltaValue(_fSpeedPerSec, _fRotationPerSec);
}

void CPlayer::Set_State(PLAYERSTATE eNewState)
{
	m_eState = eNewState;
	m_pStateMachine->Set_CurrentState(this, eNewState);
}

void CPlayer::Mouse_Fix()
{
	POINT	pt{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}
CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	for (auto& pPartObject : m_PartObjects)
		Safe_Release(pPartObject);

	m_PartObjects.clear();

	Safe_Release(m_pStateMachine);
	CStateMachine::DestroyInstance();

}
