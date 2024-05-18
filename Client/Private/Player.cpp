#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"
#include "PartObject.h"
#include "Weapon.h"
#include "Revolver.h"
#include "StateMachine.h"
#include "Player_Camera.h"
#include "PickUpSelector.h"
#include "UIInventory.h"
#include "Bone.h"
#include "UImanager.h"
#include "GEAR.h"
#include "PickRabbit.h"
#include "Monster.h"

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
	Safe_AddRef(m_pStateMachine);
	m_pStateMachine->Initialize();

	m_pStateMachine->Set_CurrentState(this, PLAYERSTATE::PLAYER_IDLE);

	m_pUImanager = CUImanager::Create(m_pDevice, m_pContext);	


	CPickUpSelector::SELECTOR_DESC pSelectDesc{};
	pSelectDesc.PlayerInventory = m_pInventory;
	pSelectDesc.UImanager = m_pUImanager;
	m_pPickUpSelector = CPickUpSelector::Create(m_pDevice,m_pContext, &pSelectDesc);

	CUIInventory::INVENTORY_DESC pInventoryDesc{};
	pInventoryDesc.PlayerInventory = m_pInventory;
	pInventoryDesc.UImanager = m_pUImanager;
	m_pUIInventory = CUIInventory::Create(m_pDevice,m_pContext,&pInventoryDesc);

	m_fSensor = 0.02f;

	if (FAILED(Add_PartObjects()))
		return E_FAIL;

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
	case Client::EQUIP_RABBIT:
		m_PartObjects[PART_RABBIT]->Priority_Tick(fTimeDelta);
		break;
	case Client::EQUIP_PIPE:
		m_PartObjects[PART_PIPE]->Priority_Tick(fTimeDelta);
		break;
	}
}

void CPlayer::Tick(_float fTimeDelta)
{


	m_pStateMachine->Update(this, fTimeDelta);

	m_pNavigationCom->Set_OnNavigation(m_pTransformCom);

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	m_PartObjects[PART_BODY]->Tick(fTimeDelta);
	switch (m_eEquip)
	{
	case Client::EQUIP_STONE:
		m_PartObjects[PART_STONE]->Tick(fTimeDelta);
		break;
	case Client::EQUIP_REVOLVER:
		m_PartObjects[PART_REOVLVER]->Tick(fTimeDelta);
		break;
	case Client::EQUIP_RABBIT:
		m_PartObjects[PART_RABBIT]->Tick(fTimeDelta);
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
	case Client::EQUIP_RABBIT:
		m_PartObjects[PART_RABBIT]->Late_Tick(fTimeDelta);
		break;
	case Client::EQUIP_PIPE:
		m_PartObjects[PART_PIPE]->Late_Tick(fTimeDelta);
		break;
	}
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPlayer::Render()
{

#ifdef _DEBUG
	m_pColliderCom->Render();
	m_pNavigationCom->Render();
#endif

	return S_OK;
}

HRESULT CPlayer::Add_Components()
{

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};

	ColliderDesc.eType = CCollider::TYPE_AABB;
	ColliderDesc.vExtents = _float3(0.3f, 0.7f, 0.3f);//aabb 조절가능
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIGATION_DESC	NavigationDesc{};

	NavigationDesc.iCurrentCellIndex = m_iCellIndex;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Inventory"),
		TEXT("Com_Inventory"), reinterpret_cast<CComponent**>(&m_pInventory))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Add_PartObjects()
{


	/* 바디객체를 복제해온다. */
	CBody_Player::BODY_DESC		BodyDesc{};
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;
	BodyDesc.pState = &m_eState;
	BodyDesc.pEquip = &m_eEquip;
	BodyDesc.pBulletsLeft = &m_iBulletsLeft;
	BodyDesc.pCloth = &m_eCloth;
	BodyDesc.pAnimFinished = &m_bAnimFinished;

	CGameObject* pBody = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Body_Player"), &BodyDesc);
	if (nullptr == pBody)
		return E_FAIL;
	m_PartObjects.emplace_back(pBody);

	//카메라 본 제어를 위함
	m_pCamBone =
		dynamic_cast<CModel*>(m_PartObjects[PART_BODY]->Get_Component(TEXT("Com_Model")))->Get_CameraBone("Camera_Weapon_Offset");

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
	m_pCamera = dynamic_cast<CPlayer_Camera*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), 0));

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
	RevolverDesc.pBulletsLeft = &m_iBulletsLeft;
	RevolverDesc.pAnimFinished = &m_bRevolver_AnimFin;
	RevolverDesc.pBodyAnimFinish = &m_bAnimFinished;
	//몸의 모델컴을 가져옴
	pModelCom = dynamic_cast<CModel*>(pBody->Get_Component(TEXT("Com_Model")));
	if (nullptr == pModelCom)
		return E_FAIL;

	//불렛 장전할때 뼈 정보(shoulder_right_prop_point)
	RevolverDesc.pBulletMatrix = pModelCom->Get_BoneCombinedTransformationMatrix("shoulder_right_prop_point");
	if (nullptr == RevolverDesc.pBulletMatrix)
		return E_FAIL;
	//무기 가 붙어 있을 뼈정보를 가져옴   right_prop_point
	RevolverDesc.pCombinedTransformationMatrix = pModelCom->Get_BoneCombinedTransformationMatrix("shoulder_prop_point");
	if (nullptr == RevolverDesc.pCombinedTransformationMatrix)
		return E_FAIL;
	//뼈정보를 넣어줘서 제작
	CGameObject* pRevolver = m_pGameInstance->Clone_Object(RevolverDesc.ProtoTypeTag, &RevolverDesc);
	if (nullptr == pRevolver)
		return E_FAIL;
	m_PartObjects.emplace_back(pRevolver);


	/*토끼*/
	///* 무기객체를 복제해온다. */
	CPickRabbit::PICKRABBIT_DESC			PickRabbitDesc{};
	PickRabbitDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	//WeaponDesc.pState = &m_iState;
	PickRabbitDesc.ProtoTypeTag = TEXT("Prototype_GameObject_PickRabbit");
	PickRabbitDesc.ModelTag = TEXT("Prototype_Component_Model_PickRabbit");
	XMStoreFloat4x4(&PickRabbitDesc.vPrePosition, XMMatrixIdentity());
	PickRabbitDesc.pState = &m_eState;
	PickRabbitDesc.pEquip = &m_eEquip;
	PickRabbitDesc.pAnimFinished = &m_bRabbit_AnimFin;
	//몸의 모델컴을 가져옴
	pModelCom = dynamic_cast<CModel*>(pBody->Get_Component(TEXT("Com_Model")));
	if (nullptr == pModelCom)
		return E_FAIL;
	//무기 가 붙어 있을 뼈정보를 가져옴
	PickRabbitDesc.pCombinedTransformationMatrix = pModelCom->Get_BoneCombinedTransformationMatrix("right_prop_point");
	if (nullptr == PickRabbitDesc.pCombinedTransformationMatrix)
		return E_FAIL;
	//뼈정보를 넣어줘서 제작
	CGameObject* pPickRabbit = m_pGameInstance->Clone_Object(PickRabbitDesc.ProtoTypeTag, &PickRabbitDesc);
	if (nullptr == pPickRabbit)
		return E_FAIL;
	m_PartObjects.emplace_back(pPickRabbit);

	return S_OK;
}

void CPlayer::Go_Straight(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
}

void CPlayer::Go_Backward(_float fTimeDelta)
{
	m_pTransformCom->Go_Backward(fTimeDelta, m_pNavigationCom);
}

void CPlayer::Go_Left(_float fTimeDelta)
{
	m_pTransformCom->Go_Left(fTimeDelta, m_pNavigationCom);
}

void CPlayer::Go_Right(_float fTimeDelta)
{
	m_pTransformCom->Go_Right(fTimeDelta, m_pNavigationCom);
}

void CPlayer::Go_LeftStraight(_float fTimeDelta)
{
	m_pTransformCom->Go_LeftStraight(fTimeDelta, m_pNavigationCom);
}

void CPlayer::Go_RightStraight(_float fTimeDelta)
{
	m_pTransformCom->Go_RightStraight(fTimeDelta, m_pNavigationCom);
}

void CPlayer::Go_LeftBackward(_float fTimeDelta)
{
	m_pTransformCom->Go_LeftBackward(fTimeDelta, m_pNavigationCom);
}

void CPlayer::Go_RightBackward(_float fTimeDelta)
{
	m_pTransformCom->Go_RightBackward(fTimeDelta, m_pNavigationCom);
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

void CPlayer::Throw()
{
	//투사체 레이어 생성해서 몬스터와 투사체 레이어 비교 하는 식으로 하자.
	switch (m_eEquip)
	{
	case Client::EQUIP_STONE:
	{
		CGEAR::GEARITEM_DESC GEARItemDesc{};
		m_pInventory->Drop_Item(TEXT("Stone"), &GEARItemDesc);
		_vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		//몸의 모델컴을 가져옴
		CModel* pModelCom = dynamic_cast<CModel*>(m_PartObjects[PART_BODY]->Get_Component(TEXT("Com_Model")));


		GEARItemDesc.bThrow = true;
		XMStoreFloat4	(& GEARItemDesc.vDirection, m_pCamera->Get_CamLook());
		//무기 가 붙어 있을 뼈정보를 가져옴 
		//TODO :: 뼈정보를 가져오는게 지금 잘못 됬음 로컬기준 뼈가져오는 기분
		//좀더 알아보고 월드로 바로 소환 되게 준비하자
		XMStoreFloat4x4(&GEARItemDesc.vPrePosition, XMLoadFloat4x4(pModelCom->Get_BoneCombinedTransformationMatrix("right_prop_point"))
			* m_pTransformCom->Get_WorldMatrix());



		if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), GEARItemDesc.ProtoTypeTag, &GEARItemDesc)))
			return ;
	}
		break;
	default:
		break;
	}
}

void CPlayer::Player_Turn(_float fTimeDelta, _long MouseMove)
{
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fSensor * MouseMove);
}

_bool CPlayer::Pick_up()
{
	//id 가 있는 객체 == 아이템 같은것을 이걸로
	CGameObject* pPickObject = m_pGameInstance->FindID_CloneObject(LEVEL_GAMEPLAY, m_pGameInstance->Picking_IDScreen());
	if (nullptr != pPickObject)
	{
		m_bAcquire = true;
		m_pPickUpSelector->Pick_up(pPickObject, &m_bAcquire);
		return true;
	}

	if(m_bRabbitCatch)//여기서 콜리전 레이 픽업을 사용하자. 몬스터는 이걸로 사용
	{
		if (dynamic_cast<CMonster*>(m_pRabbit)->Get_isItem())
		{
			Add_Rabbit();
			m_pRabbit->Set_Live(false);
			return false;
		}
		else
		{
			m_pRabbit->Set_Live(false);
			return true;
		}
	}
}

void CPlayer::Puck_up_Update(_float fTimeDelta)
{
	m_pPickUpSelector->Tick(fTimeDelta);
}

void CPlayer::Add_Item()
{
	m_pPickUpSelector->Add_Item();
}

void CPlayer::Add_Rabbit()
{
	m_pInventory->Add_Rabbit();
	m_pUImanager->Add_InvenIcon(m_pInventory->Get_LastIndex(), m_pInventory->Get_vecItemData().back());
	m_bRabbitCatch = false;
	m_pRabbit = nullptr;
}

void CPlayer::Drop_Item()
{
	m_pPickUpSelector->Drop_Item();
}

HRESULT CPlayer::Inventory_Drop(wstring ItemName)
{
	CItem::ITEM_DESC ItemDesc{};
	m_pInventory->Drop_Item(ItemName, &ItemDesc);
	_vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat4x4(&ItemDesc.vPrePosition, XMMatrixIdentity() *
		XMMatrixTranslation(XMVectorGetX(pos), XMVectorGetY(pos), XMVectorGetZ(pos)));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), ItemDesc.ProtoTypeTag, &ItemDesc)))
		return E_FAIL;


}

HRESULT CPlayer::Inventory_DropRabbit(wstring ItemName)
{
	CItem::ITEM_DESC ItemDesc{};
	m_pInventory->Drop_Item(ItemName, &ItemDesc);

	CMonster::MOSTER_DESC MonsterDesc{};

	MonsterDesc.fRotationPerSec = ItemDesc.fRotationPerSec;
	MonsterDesc.fSpeedPerSec = ItemDesc.fSpeedPerSec;
	MonsterDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Monster");
	MonsterDesc.ModelTag = TEXT("Prototype_Component_Model_Rabbit");
	MonsterDesc.pPlayerMatrix = m_pTransformCom->Get_WorldFloat4x4();
	MonsterDesc.isItem = true;
	MonsterDesc.CellIndex = m_iCellIndex;
	_vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat4x4(&MonsterDesc.vPrePosition, XMMatrixIdentity() *XMMatrixTranslation(XMVectorGetX(pos), 0.f, XMVectorGetZ(pos)));
	
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), MonsterDesc.ProtoTypeTag, &MonsterDesc)))
		return E_FAIL;

	return S_OK;
}


void CPlayer::Cam_Turn(_float fTimeDelta, _long MouseMove)
{
	_matrix  Socket = XMLoadFloat4x4(m_pCamBone->Get_ControlBoneMatrix());

	_vector		vRight = Socket.r[0];
	_vector		vUp = Socket.r[1];
	_vector		vLook = Socket.r[2];

	_matrix		RotationMatrix = XMMatrixRotationAxis(vRight, -0.02 * fTimeDelta * MouseMove);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	memcpy(&Socket.r[0], &vRight, sizeof(_vector));
	memcpy(&Socket.r[1], &vUp, sizeof(_vector));
	memcpy(&Socket.r[2], &vLook, sizeof(_vector));

	m_pCamBone->Set_TransformationMatrix(Socket);

	//이거하면 카메라 2번 움직는 꼴
	//m_pCamera->Set_CamMatrix();
}

void CPlayer::Pick_UI()
{
	m_pUIInventory->Pick_UIToggle();
}

void CPlayer::Inventory_Update(_float fTimeDelta)
{
	m_pUIInventory->Tick(fTimeDelta);
	m_pUIInventory->Late_Tick(fTimeDelta);	
}

void CPlayer::Set_Reload_Reset()
{
	
	m_bRevolver_AnimFin = false;
	m_bAnimFinished = false;

	dynamic_cast<CBody_Player*>(m_PartObjects[PART_BODY])->Reset_Anim();
	dynamic_cast<CRevolver*>(m_PartObjects[PART_REOVLVER])->Reset_Anim();
	
}

void CPlayer::Set_SceneSelect(_uint iSceneIndex)
{
	m_pUIInventory->Set_CurrentScene(iSceneIndex);
}


void CPlayer::isReload()
{
	if (dynamic_cast<CRevolver*>(m_PartObjects[EQUIP_REVOLVER])->Reload_Time())
			m_iBulletsLeft = m_iBulletsLeft++;
}

void CPlayer::RayCollInfo(const wstring Objname, CGameObject* pRabbit)
{
	m_pRabbit = pRabbit;
	m_pUImanager->RayColl_SetInfo(Objname);
	m_pUImanager->Render_UI(CUImanager::Layer_RayColl);	
	m_bRabbitCatch = true;
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

	Safe_Release(m_pNavigationCom);

	Safe_Release(m_pColliderCom);

	Safe_Release(m_pStateMachine);

	Safe_Release(m_pPickUpSelector);

	Safe_Release(m_pUIInventory);

	Safe_Release(m_pUImanager);

	Safe_Release(m_pInventory);

	CStateMachine::DestroyInstance();

}
