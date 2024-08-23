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
#include "Rabbit.h"
#include "InteractiveObject.h"	
#include "LoadingBar.h"
#include "Mother.h"
#include "NPCManager.h"
#include"UIBurn.h"
#include "Match.h"
#include "Harvest.h"
#include "UISleep.h"
#include "BedRoll.h"
#include "UIStruggleIntro.h"
#include "Flare.h"	
#include "UIPlayerState.h"
#include "Wolf.h"
#include "Particle_Rect.h"
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
	srand(time(NULL));
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

	m_pUImanager = CUImanager::Create(m_pDevice, m_pContext);	


	CPickUpSelector::SELECTOR_DESC pSelectDesc={};
	pSelectDesc.PlayerInventory = m_pInventory;
	pSelectDesc.UImanager = m_pUImanager;
	m_pPickUpSelector = CPickUpSelector::Create(m_pDevice,m_pContext, &pSelectDesc);
	if (nullptr == m_pPickUpSelector)
		return E_FAIL;

	CUIInventory::INVENTORY_DESC pInventoryDesc={};
	pInventoryDesc.PlayerInventory = m_pInventory;
	pInventoryDesc.UImanager = m_pUImanager;
	m_pUIInventory = CUIInventory::Create(m_pDevice,m_pContext,&pInventoryDesc);
	if (nullptr == m_pUIInventory)
		return E_FAIL;

	CLoadingBar::LOADINGBAR_DESC LoadingBarDesc={};
	LoadingBarDesc.pCurrentLoadingBar = &m_fCurrentLoadingBar;
	LoadingBarDesc.pUImanager = m_pUImanager;
	m_pLoadingBar = CLoadingBar::Create(m_pDevice, m_pContext, &LoadingBarDesc);
	if (nullptr == m_pLoadingBar)
		return E_FAIL;

	m_fSensor = 0.02f;
	CNPCManager::NPC_DESC NPCDesc={};
	NPCDesc.pUImanager = m_pUImanager;
	NPCDesc.pPlayerInventory = m_pInventory;
	m_pNPCManager = CNPCManager::Create(m_pDevice, m_pContext, &NPCDesc);
	if(nullptr== m_pNPCManager)
		return E_FAIL;

	CUIBurn::UIBURN_DESC BurnDesc = {};
	BurnDesc.pUImanager = m_pUImanager;
	BurnDesc.pPlayerInventory = m_pInventory;
	m_pUIBurn = CUIBurn::Create(m_pDevice, m_pContext, &BurnDesc);
	if (nullptr == m_pUIBurn)
		return E_FAIL;

	CHarvest::UIHARVEST_DESC HarvestDesc = {};
	HarvestDesc.pUImanager = m_pUImanager;
	HarvestDesc.pPlayerInventory = m_pInventory;
	m_pUIHarvest = CHarvest::Create(m_pDevice, m_pContext, &HarvestDesc);
	if (nullptr == m_pUIHarvest)
		return E_FAIL;


	CUISleep::UISLEEP_DESC SleepDesc = {};
	SleepDesc.pUImanager = m_pUImanager;
	m_pUISleep = CUISleep::Create(m_pDevice, m_pContext, &SleepDesc);
	if (nullptr == m_pUISleep)
		return E_FAIL;

	CUIStruggleIntro::STRUGGLEINTRO_DESC StruggleDesc = {};
	StruggleDesc.pUImanager = m_pUImanager;
	StruggleDesc.pPlayerInventory = m_pInventory;
	StruggleDesc.pCurrentBar = &m_fCurrentLoadingBar;
	m_pUIStruggle = CUIStruggleIntro::Create(m_pDevice, m_pContext, &StruggleDesc);
	if (nullptr == m_pUIStruggle)
		return E_FAIL;	


	CUIPlayerState::UISTATE_DESC StateDesc = {};
	StateDesc.pNPCManager = m_pNPCManager;
	m_pUIState = CUIPlayerState::Create(m_pDevice, m_pContext, &StateDesc);
	if (nullptr == m_pUIState)
		return E_FAIL;




	if (FAILED(Add_PartObjects()))
		return E_FAIL;

	//m_pInventory->Add_BedRoll();	
	//m_pUImanager->Add_InvenIcon(m_pInventory->Get_LastIndex(), m_pInventory->Get_vecItemData().back());	

	m_pInventory->Add_Knife();
	m_pUImanager->Add_InvenIcon(m_pInventory->Get_LastIndex(), m_pInventory->Get_vecItemData().back());

	m_pGameInstance->Play_BGM(TEXT("OutSnow.wav"),   0.8f);
	

	return S_OK;
}

void CPlayer::Priority_Tick(_float fTimeDelta)
{
	
	m_pGameInstance->Rotate_Light(fTimeDelta);
	
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
	case Client::EQUIP_MATCH:
		m_PartObjects[PART_MATCH]->Priority_Tick(fTimeDelta);
		m_PartObjects[PART_MATCHBOX]->Priority_Tick(fTimeDelta);
		break;
	case Client::EQUIP_KNIFE:
		m_PartObjects[PART_KNIFE]->Priority_Tick(fTimeDelta);
		break;
	case Client::EQUIP_FLARE:
		m_PartObjects[PART_FLARE]->Priority_Tick(fTimeDelta);
		break;
	case Client::EQUIP_PIPE:
		m_PartObjects[PART_PIPE]->Priority_Tick(fTimeDelta);
		break;
	}
}

void CPlayer::Tick(_float fTimeDelta)
{
	m_fSoundFXTime += fTimeDelta;

	if (m_fSoundFXTime > m_fEndTime)
	{
		if(7>=m_fEndTime)
			m_pGameInstance->Play_Sound(TEXT("Crow.wav"), CH_FX, 1.f);
		else if(14>=m_fBurnTime)
			m_pGameInstance->Play_Sound(TEXT("Bird.wav"), CH_FX, 1.f);
		else
			m_pGameInstance->Play_Sound(TEXT("Duck.wav"), CH_FX, 1.f);
			

		m_fSoundFXTime = 0.f;
		m_fEndTime = rand()%20;
	}

	if (PLAYER_INVENTORY != m_eState &&
		PLAYER_ENTER != m_eState &&
		PLAYER_PICKUP != m_eState &&
		PLAYER_QUEST != m_eState &&
		PLAYER_BURN != m_eState &&
		PLAYER_BUILD != m_eState &&
		PLAYER_HARVEST != m_eState &&
		PLAYER_SLEEP != m_eState)
	m_pUIState->Tick(fTimeDelta);

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
	case Client::EQUIP_MATCH:
		m_PartObjects[PART_MATCH]->Tick(fTimeDelta);
		m_PartObjects[PART_MATCHBOX]->Tick(fTimeDelta);
		break;
	case Client::EQUIP_KNIFE:
		m_PartObjects[PART_KNIFE]->Tick(fTimeDelta);
		break;
	case Client::EQUIP_FLARE:
		m_PartObjects[PART_FLARE]->Tick(fTimeDelta);
		break;
	case Client::EQUIP_PIPE:
		m_PartObjects[PART_PIPE]->Tick(fTimeDelta);
		break;
	}
}

void CPlayer::Late_Tick(_float fTimeDelta)
{

	if (PLAYER_INVENTORY != m_eState &&
		PLAYER_ENTER != m_eState &&
		PLAYER_PICKUP != m_eState &&
		PLAYER_QUEST != m_eState &&
		PLAYER_BURN != m_eState &&
		PLAYER_BUILD != m_eState &&
		PLAYER_HARVEST != m_eState &&
		PLAYER_SLEEP != m_eState)
		m_pUIState->Late_Tick(fTimeDelta);


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
	case Client::EQUIP_MATCH:
		m_PartObjects[PART_MATCH]->Late_Tick(fTimeDelta);
		m_PartObjects[PART_MATCHBOX]->Late_Tick(fTimeDelta);
		break;
	case Client::EQUIP_KNIFE:
		m_PartObjects[PART_KNIFE]->Late_Tick(fTimeDelta);
		break;
	case Client::EQUIP_FLARE:
		m_PartObjects[PART_FLARE]->Late_Tick(fTimeDelta);
		break;
	case Client::EQUIP_PIPE:
		m_PartObjects[PART_PIPE]->Late_Tick(fTimeDelta);
		break;
	}
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
	m_pGameInstance->Add_DebugComponent(m_pNavigationCom);
#endif
}

HRESULT CPlayer::Render()
{

	return S_OK;
}

HRESULT CPlayer::Add_Components()
{

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc={};

	ColliderDesc.eType = CCollider::TYPE_AABB;
	ColliderDesc.vExtents = _float3(0.3f, 0.7f, 0.3f);//aabb 조절가능
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIGATION_DESC	NavigationDesc={};

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
	CBody_Player::BODY_DESC		BodyDesc={};
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;
	BodyDesc.pState = &m_eState;
	BodyDesc.pEquip = &m_eEquip;
	BodyDesc.pLit = &m_bLit;
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
	CPlayer_Camera::PLAYER_CAMERA_DESC		CameraDesc={};
	CameraDesc.pEyeBoneMatrix =
		dynamic_cast<CModel*>(m_PartObjects[PART_BODY]->Get_Component(TEXT("Com_Model")))->
		Get_BoneCombinedTransformationMatrix("Camera_Weapon_Offset");

	//Camera_Weapon_Offset == 조절 카메라 
	//Camera_Offset = 카메라 부모
	//camera			=카메라 박을 위치

	CameraDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	CameraDesc.pCamBone =
		dynamic_cast<CModel*>(m_PartObjects[PART_BODY]->Get_Component(TEXT("Com_Model")))->Get_CameraBone("camera");
	//Camera_Offset 이것도 던져줘서 부모한테 넣어주기
	CameraDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	CameraDesc.fFovy = XMConvertToRadians(45.f);
	CameraDesc.fNear = 0.01f;
	CameraDesc.fFar = 3000.f;
	CameraDesc.fSpeedPerSec = 50.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.f);
	XMStoreFloat4x4(&CameraDesc.vPrePosition, XMMatrixIdentity());
	CameraDesc.ProtoTypeTag = TEXT("Prototype_GameObject_PlayerCamera");
	CameraDesc.ModelTag = TEXT("");
	CameraDesc.pState = (_uint*)&m_eState;

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_PlayerCamera"), &CameraDesc)))
		return E_FAIL;
	m_pCamera = dynamic_cast<CPlayer_Camera*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), 0));

	///*돌*/
	///* 무기객체를 복제해온다. */
	CWeapon::WEAPON_DESC			WeaponDesc={};
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
	CRevolver::REVOLVER_DESC			RevolverDesc={};
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
	CPickRabbit::PICKRABBIT_DESC			PickRabbitDesc={};
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


	/*성냥*/
	///* 무기객체를 복제해온다. */
	CMatch::MATCH_DESC			MatchDesc = {};
	MatchDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	//MatchDesc.pState = &m_iState;
	MatchDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Match");
	MatchDesc.ModelTag = TEXT("Prototype_Component_Model_Match");
	XMStoreFloat4x4(&MatchDesc.vPrePosition, XMMatrixIdentity());
	MatchDesc.pState = &m_eState;
	MatchDesc.pEquip = &m_eEquip;
	MatchDesc.pLit = &m_bLit;
	MatchDesc.pBurnTime = &m_fBurnTime;
	MatchDesc.pAnimFinished = &m_bAnimFinished;
	//몸의 모델컴을 가져옴
	 pModelCom = dynamic_cast<CModel*>(pBody->Get_Component(TEXT("Com_Model")));
	if (nullptr == pModelCom)
		return E_FAIL;
	//무기 가 붙어 있을 뼈정보를 가져옴
	MatchDesc.pCombinedTransformationMatrix = pModelCom->Get_BoneCombinedTransformationMatrix("right_prop_point");
	if (nullptr == MatchDesc.pCombinedTransformationMatrix)
		return E_FAIL;
	//뼈정보를 넣어줘서 제작
	CGameObject* pMatch = m_pGameInstance->Clone_Object(MatchDesc.ProtoTypeTag, &MatchDesc);
	if (nullptr == pMatch)
		return E_FAIL;
	m_PartObjects.emplace_back(pMatch);




	///*성냥갑*/
	///* 무기객체를 복제해온다. */
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	//WeaponDesc.pState = &m_iState;
	WeaponDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Weapon");
	WeaponDesc.ModelTag = TEXT("Prototype_Component_Model_Matchbox");
	XMStoreFloat4x4(&WeaponDesc.vPrePosition, XMMatrixIdentity());
	WeaponDesc.pState = &m_eState;
	WeaponDesc.pEquip = &m_eEquip;
	WeaponDesc.pAnimFinished = &m_bAnimFinished;
	//몸의 모델컴을 가져옴
	pModelCom = dynamic_cast<CModel*>(pBody->Get_Component(TEXT("Com_Model")));
	if (nullptr == pModelCom)
		return E_FAIL;
	//무기 가 붙어 있을 뼈정보를 가져옴
	WeaponDesc.pCombinedTransformationMatrix = pModelCom->Get_BoneCombinedTransformationMatrix("left_prop_point");
	if (nullptr == WeaponDesc.pCombinedTransformationMatrix)
		return E_FAIL;
	//뼈정보를 넣어줘서 제작
	pWeapon = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Weapon"), &WeaponDesc);
	if (nullptr == pWeapon)
		return E_FAIL;
	m_PartObjects.emplace_back(pWeapon);

	///*나이프*/
///* 무기객체를 복제해온다. */
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	//WeaponDesc.pState = &m_iState;
	WeaponDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Weapon");
	WeaponDesc.ModelTag = TEXT("Prototype_Component_Model_Knife");
	XMStoreFloat4x4(&WeaponDesc.vPrePosition, XMMatrixIdentity());
	WeaponDesc.pState = &m_eState;
	WeaponDesc.pEquip = &m_eEquip;
	WeaponDesc.pAnimFinished = &m_bAnimFinished;
	//몸의 모델컴을 가져옴
	pModelCom = dynamic_cast<CModel*>(pBody->Get_Component(TEXT("Com_Model")));
	if (nullptr == pModelCom)
		return E_FAIL;
	//무기 가 붙어 있을 뼈정보를 가져옴
	WeaponDesc.pCombinedTransformationMatrix = pModelCom->Get_BoneCombinedTransformationMatrix("right_prop_point");
	if (nullptr == WeaponDesc.pCombinedTransformationMatrix)
		return E_FAIL;
	//뼈정보를 넣어줘서 제작
	pWeapon = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Weapon"), &WeaponDesc);
	if (nullptr == pWeapon)
		return E_FAIL;
	m_PartObjects.emplace_back(pWeapon);

	///*플레어*/
///* 무기객체를 복제해온다. */
	CFlare::FLARE_DESC			FlareDesc = {};
	FlareDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	//WeaponDesc.pState = &m_iState;
	FlareDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Flare");
	FlareDesc.ModelTag = TEXT("Prototype_Component_Model_Flare");
	XMStoreFloat4x4(&FlareDesc.vPrePosition, XMMatrixIdentity());
	FlareDesc.pState = &m_eState;
	FlareDesc.pEquip = &m_eEquip;
	FlareDesc.pAnimFinished = &m_bAnimFinished;
	FlareDesc.pLit = &m_bFlareLit;
	//몸의 모델컴을 가져옴
	pModelCom = dynamic_cast<CModel*>(pBody->Get_Component(TEXT("Com_Model")));
	if (nullptr == pModelCom)
		return E_FAIL;
	//무기 가 붙어 있을 뼈정보를 가져옴
	FlareDesc.pCombinedTransformationMatrix = pModelCom->Get_BoneCombinedTransformationMatrix("right_prop_point");
	if (nullptr == FlareDesc.pCombinedTransformationMatrix)
		return E_FAIL;
	//뼈정보를 넣어줘서 제작
	pWeapon = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Flare"), &FlareDesc);
	if (nullptr == pWeapon)
		return E_FAIL;
	m_PartObjects.emplace_back(pWeapon);



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
	//	m_pInventory->Using_Item(TEXT("Stone"));
		CGEAR::GEARITEM_DESC GEARItemDesc={};
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
	_vector vMouseRay[2] = {};
	_float fDist = 0.f;
	m_pGameInstance->World_MouseRay(vMouseRay);
	CGameObject* pPickObject = m_pGameInstance->IntersectRay(LEVEL_GAMEPLAY, TEXT("Layer_Item"), vMouseRay, &fDist);
	if (nullptr != pPickObject)
{
	m_fCurrentLoadingBar = 0.f;
	Selector_In(pPickObject);

	return true;
}


	m_pGameInstance->World_MouseRay(vMouseRay);
	pPickObject= m_pGameInstance->IntersectRay(LEVEL_GAMEPLAY , TEXT("Layer_Monster"), vMouseRay, &fDist);

	if(nullptr !=pPickObject)
	{
		if (TEXT("Prototype_Component_Model_Rabbit") == pPickObject->Get_ModelTag())
		{
			m_fCurrentLoadingBar = 0.f;


			//로직 다시
			if (dynamic_cast<CMonster*>(pPickObject)->Get_isItem())
			{
				m_bRabbitCatch = true;
				pPickObject->Set_Live(false);
				return true;
			}
		}
	}
	//여기부터 상호작용 오브젝트 레이피킹


	pPickObject = m_pGameInstance->IntersectRay(LEVEL_GAMEPLAY, TEXT("Layer_Npc"), vMouseRay, &fDist);

	if (nullptr != pPickObject)
	{
		dynamic_cast<CMother*>(pPickObject)->Action();
		return false;
	}
	return false;
}

_bool CPlayer::Pick_InteractiveObject()
{
	_vector vMouseRay[2] = {};
	_float fDist = 0.f;
	m_pGameInstance->World_MouseRay(vMouseRay);
	CGameObject* pPickObject = m_pGameInstance->IntersectRay(LEVEL_GAMEPLAY, TEXT("Layer_InteractiveObject"), vMouseRay, &fDist);

	if (nullptr != pPickObject)
	{

		if (TEXT("Prototype_GameObject_BedRoll") == pPickObject->Get_Prototype())
		{
			dynamic_cast<CInteractiveObject*>(pPickObject)->Action();
		}
		else if(TEXT("Prototype_GameObject_Stove")!= pPickObject->Get_Prototype())
		{
			
	
			m_bEnter = true;
	
			if (m_fCurrentLoadingBar >= m_fMaxLoadingBar)
			{
				dynamic_cast<CInteractiveObject*>(pPickObject)->Action();
				m_bEnter = false;
			}
	
			return true;
	
		}
		else
		{
			m_pUIBurn->Set_Stove(pPickObject);
			dynamic_cast<CInteractiveObject*>(pPickObject)->Action();
			
			return false;
		}
	}
	return false;
}

void CPlayer::Selector_In(CGameObject* PickObject)
{
	m_bAcquire = true;
	m_pPickUpSelector->Pick_up(PickObject, &m_bAcquire);
}

void CPlayer::Pick_up_Update(_float fTimeDelta)
{
	m_pPickUpSelector->Tick(fTimeDelta);
	m_pPickUpSelector->Late_Tick(fTimeDelta);
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
	CItem::ITEM_DESC ItemDesc={};
	m_pInventory->Drop_Item(ItemName, &ItemDesc);
	_vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat4x4(&ItemDesc.vPrePosition, XMMatrixIdentity() *
		XMMatrixTranslation(XMVectorGetX(pos), XMVectorGetY(pos), XMVectorGetZ(pos)));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), ItemDesc.ProtoTypeTag, &ItemDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Inventory_DropRabbit(wstring ItemName)
{
	CItem::ITEM_DESC ItemDesc={};
	m_pInventory->Drop_Item(ItemName, &ItemDesc);

	CMonster::MONSTER_DESC MonsterDesc={};

	MonsterDesc.fRotationPerSec = ItemDesc.fRotationPerSec;
	MonsterDesc.fSpeedPerSec = ItemDesc.fSpeedPerSec;
	MonsterDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Rabbit");
	MonsterDesc.ModelTag = TEXT("Prototype_Component_Model_Rabbit");
	MonsterDesc.pPlayerMatrix = m_pTransformCom->Get_WorldFloat4x4();
	MonsterDesc.isItem = true;
	MonsterDesc.CellIndex = *m_pNavigationCom->Get_CurrentCell();
	_vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat4x4(&MonsterDesc.vPrePosition, XMMatrixIdentity() *XMMatrixTranslation(XMVectorGetX(pos), 0.f, XMVectorGetZ(pos)));
	
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), MonsterDesc.ProtoTypeTag, &MonsterDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Release_Rabbit(wstring ItemName)
{
	CRabbit::RABBIT_DESC MonsterDesc={};

	MonsterDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Rabbit");
	MonsterDesc.ModelTag = TEXT("Prototype_Component_Model_Rabbit");
	MonsterDesc.CellIndex = *m_pNavigationCom->Get_CurrentCell();
	_vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	MonsterDesc.isItem = false;
	XMStoreFloat4x4(&MonsterDesc.vPrePosition, XMMatrixIdentity() * XMMatrixTranslation(XMVectorGetX(pos), XMVectorGetY(pos), XMVectorGetZ(pos)));

	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), MonsterDesc.ProtoTypeTag, &MonsterDesc)))
		return E_FAIL;
	m_bRabbitCatch = false;
	m_pRabbit = nullptr;
	return S_OK;
}


void CPlayer::UIBurn_Update(_float fTimeDelta)
{
	m_pUIBurn->Tick(fTimeDelta);
	m_pUIBurn->Late_Tick(fTimeDelta);
}

void CPlayer::UIHarvest_Update(_float fTimeDelta)
{
	m_pUIHarvest->Tick(fTimeDelta);
	m_pUIHarvest->Late_Tick(fTimeDelta);
}

void CPlayer::UISleep_Update(_float fTimeDelta)
{
	m_pUISleep->Tick(fTimeDelta);
	m_pUISleep->Late_Tick(fTimeDelta);
}

void CPlayer::UIStruggle_Update(_float fTimeDelta)
{
	m_pUIStruggle->Tick(fTimeDelta);
	m_pUIStruggle->Late_Tick(fTimeDelta);
}

void CPlayer::Cam_Turn(_float fTimeDelta, _long MouseMove)
{
	_matrix  Socket = XMLoadFloat4x4(m_pCamBone->Get_ControlBoneMatrix());

	_vector		vRight = Socket.r[0];
	_vector		vUp = Socket.r[1];
	_vector		vLook = Socket.r[2]; 

	_matrix		RotationMatrix = XMMatrixRotationAxis(vRight, -0.02f * fTimeDelta * MouseMove);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	memcpy(&Socket.r[0], &vRight, sizeof(_vector));
	memcpy(&Socket.r[1], &vUp, sizeof(_vector));
	memcpy(&Socket.r[2], &vLook, sizeof(_vector));

	m_pCamBone->Set_TransformationMatrix(Socket);

	//이거하면 카메라 2번 움직는 꼴
	m_pCamera->Set_CamMatrix(fTimeDelta);	
}

void CPlayer::Pick_UI()
{
	m_pUIInventory->Pick_UIToggle();
}

void CPlayer::Pick_UIBurn()
{
	m_pUIBurn->Pick_UIBurn();
}

void CPlayer::Pick_UIHarvest()
{
	m_pUIHarvest->Pick_UIHarvest();
}

void CPlayer::Pick_UISleep()
{
	m_pUISleep->Pick_UISleep();
}

void CPlayer::Pick_UIStruggle()
{
	m_pUIStruggle->Pick_UIStruggle();
}

void CPlayer::Loading_UI(_float fTimeDelta)
{
	m_pLoadingBar->Tick(fTimeDelta);
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

void CPlayer::Quest_Update(_float fTimeDelta)
{
	m_pNPCManager->Tick(fTimeDelta);
	m_pNPCManager->Late_Tick(fTimeDelta);
}

_bool CPlayer::Talk(_int TalkIndex)
{
	return m_pNPCManager->Talk(TalkIndex);
}

void CPlayer::Set_SceneSelect(_uint iSceneIndex)
{
	m_pUIInventory->Set_CurrentScene(iSceneIndex);
}

_bool CPlayer::Burn_Out()
{
	if (true == m_bLit)
		if (m_fBurnTime > 5.f)
		{
			Set_State(PLAYERSTATE::PLAYER_BURNOUT);
			return true;
		}

	return false;
}

void CPlayer::Using_Item(wstring strName)
{
	m_pInventory->Using_Item(strName);
}

void CPlayer::Set_FadeIn()
{
	m_pUImanager->Render_UI(CUImanager::Layer_FadeIn);
}

HRESULT CPlayer::Set_Portal(_int iGoalCellIndex, _float4 vGoalPosition)
{
	m_pNavigationCom->Set_CellIndex(iGoalCellIndex);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vGoalPosition));
	if (FAILED(m_pNavigationCom->Set_OnNavigation(m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}


void CPlayer::Add_EnterTime(_float fTimeDelta)
{
	m_fCurrentLoadingBar += 5.f * fTimeDelta;
}

void CPlayer::Move_BuildObject(_float fTimeDelta)
{


	_float4		vPickPos = {};	
	if (m_pGameInstance->Get_PickPos(&vPickPos))	
		dynamic_cast<CTransform*>(m_pBuildObject->Get_Transform())->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPickPos));

	m_pBuildObject->Tick(fTimeDelta);
	m_pBuildObject->Late_Tick(fTimeDelta);
}

void CPlayer::BuildObject()
{
	CBedRoll::BEDROLL_DESC Desc{};

	Desc.ProtoTypeTag = m_pBuildObject->Get_Prototype();
	Desc.ModelTag = m_pBuildObject->Get_ModelTag();
	XMStoreFloat4x4(&Desc.vPrePosition, dynamic_cast<CTransform*>(m_pBuildObject->Get_Transform())->Get_WorldMatrix());
	Desc.CellIndex = m_pNavigationCom->Pick_CellIndex(dynamic_cast<CTransform*>(m_pBuildObject->Get_Transform())->Get_State(CTransform::STATE_POSITION));
	Desc.iShaderPass = 0;
	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_InteractiveObject"), Desc.ProtoTypeTag, &Desc)))
		return;

	m_pInventory->Using_Item(TEXT("BedRoll"));
	
	Safe_Release(m_pBuildObject);

}

void CPlayer::Make_FootStep()
{
	CParticle_Rect::PARTICLE_DESC RectDesc{};
	XMStoreFloat4(&RectDesc.vStartPos, XMVectorSet(0.f, 0.1f, 0.f, 1.f));
	RectDesc.BufferInstance.iNumInstance = 1;
	RectDesc.BufferInstance.vOffsetPos = _float3(0.0f, 0.f, 0.0f);
	RectDesc.BufferInstance.vPivotPos = _float3(0.0f, 0.0f, 0.0f);
	RectDesc.BufferInstance.vRange = _float3(0.0f, 0.f, 0.0f);
	RectDesc.BufferInstance.vSize = _float2(1.f, 1.f);
	RectDesc.BufferInstance.vSpeed = _float2(0.0f, 0.00f);
	RectDesc.BufferInstance.vLifeTime = _float2(2.f, 2.0f);
	RectDesc.BufferInstance.fScaleSize = 1.f;
	RectDesc.BufferInstance.isLoop = false;
	RectDesc.iTYPE = CParticle_Rect::TYPE_FOOT;

	_float4x4 position{};
	XMStoreFloat4x4(&position, m_pTransformCom->Get_WorldMatrix());

	RectDesc.pParentMatrix = &position;	
	RectDesc.BufferInstance.vStartPos = RectDesc.vStartPos;
	RectDesc.BufferInstance.pParentMatrix = &position;

//	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_FootStep"),
//		TEXT("Prototype_GameObject_Particle_Rect"), &RectDesc)))
//		return;
	 
}


void CPlayer::RayFire()
{
	dynamic_cast<CRevolver*>(m_PartObjects[PART_REOVLVER])->Set_Shot();
	dynamic_cast<CRevolver*>(m_PartObjects[PART_REOVLVER])->Set_Smog();
	_vector vMouseRay[2] = {};
	_float fDist = 0.f;
	m_pGameInstance->World_MouseRay(vMouseRay);
	CGameObject* pPickObject = m_pGameInstance->IntersectRay(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), vMouseRay, &fDist);

	if (nullptr != pPickObject)
	{
		if (TEXT("Prototype_Component_Model_Rabbit") == pPickObject->Get_ModelTag())
		{
			m_fCurrentLoadingBar = 0.f;

			if (!dynamic_cast<CRabbit*>(pPickObject)->Get_isItem())
			{
				dynamic_cast<CRabbit*>(pPickObject)->Set_State(6);	
			}
		}
		if (TEXT("Prototype_Component_Model_Wolf") == pPickObject->Get_ModelTag())
		{
			m_fCurrentLoadingBar = 0.f;

			dynamic_cast<CWolf*>(pPickObject)->Set_State(6);
			
		}
	}
}

void CPlayer::isReload()
{
//	if (dynamic_cast<CRevolver*>(m_PartObjects[EQUIP_REVOLVER])->Reload_Time())
			m_iBulletsLeft = ++m_iBulletsLeft;
}

void CPlayer::RayCollInfo()
{
	_vector pRay[2];
	CGameObject* pGameObject = nullptr;

	m_pGameInstance->World_MouseRay(pRay);
	if(m_pGameInstance->RayCollInfo(LEVEL_GAMEPLAY, TEXT("Layer_Item"), pRay, &pGameObject))
	{
		m_pUImanager->RayColl_SetInfo(dynamic_cast<CItem*>(pGameObject)->Get_TextUIName());
		m_pUImanager->Render_UI(CUImanager::Layer_RayColl);
	}

	if (m_pGameInstance->RayCollInfo(LEVEL_GAMEPLAY, TEXT("Layer_InteractiveObject"), pRay, &pGameObject))
	{
		m_pUImanager->RayColl_SetInfo(dynamic_cast<CItem*>(pGameObject)->Get_TextUIName());
		m_pUImanager->Render_UI(CUImanager::Layer_RayColl);
	}

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

	Safe_Release(m_pUIState);

	Safe_Release(m_pNavigationCom);

	Safe_Release(m_pColliderCom);

	Safe_Release(m_pStateMachine);

	Safe_Release(m_pPickUpSelector);

	Safe_Release(m_pUIInventory);

	Safe_Release(m_pUImanager);

	Safe_Release(m_pInventory);

	Safe_Release(m_pLoadingBar);

	Safe_Release(m_pNPCManager);

	Safe_Release(m_pUIBurn);

	Safe_Release(m_pUIHarvest);

	Safe_Release(m_pUISleep);

	Safe_Release(m_pUIStruggle);

	//CStateMachine::DestroyInstance();

}
