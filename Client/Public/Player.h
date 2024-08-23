#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "Body_Player.h"
BEGIN(Engine)
class CCollider;
class CNavigation;
class CBone;	
class CInventory;
END


BEGIN(Client)

class CPlayer final : public CLandObject
{
public:
	enum PART { PART_BODY,  PART_STONE, PART_REOVLVER, PART_RABBIT
		, PART_MATCH, PART_MATCHBOX, PART_KNIFE, PART_FLARE, PART_PIPE, PART_END };
	
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Add_Components();
	HRESULT Add_PartObjects();


public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);

	void Go_LeftStraight(_float fTimeDelta);
	void Go_RightStraight(_float fTimeDelta);
	void Go_LeftBackward(_float fTimeDelta);
	void Go_RightBackward(_float fTimeDelta);

	void Set_DeltaValue(_float _fSpeedPerSec, _float _fRotationPerSec);

	void Set_State(PLAYERSTATE eNewState);
	void Set_Equip(PLAYEREQUIP eEquip) {
		m_eEquip = eEquip;
		m_bChangeEquip = true;
	}
	void Set_NoActionEquip(PLAYEREQUIP eEquip) {
		m_eEquip = eEquip;
	}
	void Set_UnEquip() {
		m_eEquip = PLAYEREQUIP::EQUIP_NONE;
	}
	void End_Change(){ m_bChangeEquip = false; }

	void Throw();
public:
	void Player_Turn(_float fTimeDelta,_long MouseMove);
	_bool Pick_up();
	_bool Pick_InteractiveObject();
	void Selector_In(class CGameObject* PickObject);
	void Pick_up_Update(_float fTimeDelta);

	void Add_Item();
	void Add_Rabbit();
	void Drop_Item();
	HRESULT Inventory_Drop(wstring ItemName);
	HRESULT Inventory_DropRabbit(wstring ItemName);
	HRESULT Release_Rabbit(wstring ItemName);

	void UIBurn_Update(_float fTimeDelta);
	void UIHarvest_Update(_float fTimeDelta);
	void UISleep_Update(_float fTimeDelta);
	void UIStruggle_Update(_float fTimeDelta);
	void Inventory_Update(_float fTimeDelta);

	
public:
	void Pick_UI();
	void Pick_UIBurn();
	void Pick_UIHarvest();
	void Pick_UISleep();
	void Pick_UIStruggle();
	void Loading_UI(_float fTimeDelta);
public:
	const PLAYERCONDITION isCondition() { return m_eCondition; }
	const PLAYEREQUIP			isEquip() { return m_eEquip; }

	_bool						isEquipChange() { return m_bChangeEquip; }
	_bool						isAnimFinished() { return m_bAnimFinished; }
	_bool						isRevolver_AnimFin() { return m_bRevolver_AnimFin; }
	_bool						isRabbit_AnimFin() { return m_bRabbit_AnimFin; }
	_bool						isRabbitCatch() { return m_bRabbitCatch; }
	_bool						isEnter() { 	return m_bEnter; }
	_bool						isLit() { 	return m_bLit; }
	_bool						isStruggle() { return m_bStruggle; }
	_bool						isStruggleItem() { return m_bStruggleItem; }
	void Set_Reload_Reset();

public://Quest
	void Quest_Update(_float fTimeDelta);
	_bool Talk(_int TalkIndex);
public:
	void Set_SceneSelect(_uint iSceneIndex);
	void Set_RabbitCatch(_bool isCatch) { m_bRabbitCatch = isCatch; }
	void Set_Enter(_bool isEnter) { 
			m_fCurrentLoadingBar = 0.f; 
			m_bEnter = isEnter; 
	}
	void CurrentLoadingBarReset() {
		m_fCurrentLoadingBar = 0.f;
	}
	void Set_Lit(_bool bLit) { m_bLit = bLit; }
	void Set_FlareLit(_bool bLit) { m_bFlareLit = bLit; }
	void Set_Struggle(_bool bStruggle) { m_bStruggle = bStruggle; }
	void Set_StruggleItem(_bool bStruggleItem) { m_bStruggleItem = bStruggleItem	;}
	void Reset_BurnTime() { m_fBurnTime = 0.f; }
	_bool Burn_Out();
	void Using_Item(wstring strName);
public:
	void Set_FadeIn();
	HRESULT Set_Portal(_int iGoalCellIndex, _float4 vGoalPosition);
	void Cam_Turn(_float fTimeDelta, _long MouseMove);
private:
	vector<class CGameObject*>		m_PartObjects;
	PLAYERSTATE								m_eState = { PLAYERSTATE::PLAYER_IDLE };
	PLAYEREQUIP								m_eEquip = { PLAYEREQUIP::EQUIP_NONE };
	PLAYERCLOTH								m_eCloth = { PLAYERCLOTH::CLOTH_NONE };
	_bool											m_bChangeEquip = { false };
	PLAYERCONDITION						m_eCondition = { PLAYERCONDITION::CON_NORMAL };
	class CStateMachine*					m_pStateMachine = { nullptr };
	_bool											m_bAnimFinished = { true };
	_float											m_fSensor = { 0.0f };
	
	
	_bool											m_bStruggleItem = { false };
	_bool											m_bStruggle = { false };
	_bool											m_bRevolver_AnimFin = { true };
	_bool											m_bRabbit_AnimFin = { false };
	//pickupselector 돌릴지 말지 확인용 
	_bool											m_bAcquire = {false};
	_bool											m_bRabbitCatch = { false };
	_bool											m_bEnter = { false };
	_bool											m_bLit = { false };
	_bool											m_bFlareLit = { false };
	_float											m_fBurnTime = { 0.f };
	CGameObject*								m_pRabbit = {nullptr};

	class CNavigation* m_pNavigationCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };
	class CInventory* m_pInventory = { nullptr };
	class CPickUpSelector* m_pPickUpSelector = { nullptr };
	class CUIInventory* m_pUIInventory = { nullptr };
	class CUImanager* m_pUImanager = { nullptr };
	class CLoadingBar* m_pLoadingBar = { nullptr };
	class CNPCManager* m_pNPCManager = { nullptr };
	class CUIBurn* m_pUIBurn = { nullptr };
	class CHarvest* m_pUIHarvest = { nullptr };
	class CUISleep* m_pUISleep = { nullptr };
	class CUIStruggleIntro* m_pUIStruggle = { nullptr };
	class CUIPlayerState* m_pUIState = { nullptr };

	class CGameObject* m_pBuildObject = { nullptr };
private:
	class CBone* m_pCamBone = { nullptr };
	class CPlayer_Camera* m_pCamera = { nullptr };
	public:
		void Set_Cloth(_uint Index) { m_eCloth = (PLAYERCLOTH)Index; }

public:
	void Add_EnterTime(_float fTimeDelta);

	//아이템 설치 필요함수
public:
	void Set_BuildObject(class CGameObject* pBuildItem) {
		m_pBuildObject = pBuildItem;
	}
	void Move_BuildObject(_float fTimeDelta);
	void BuildObject();

	void Make_FootStep();
	public:
		_float* Get_LoadBar() { return &m_fCurrentLoadingBar; }
	//로딩바 함수
private:
	_float m_fMaxLoadingBar = { 10.f };
	_float m_fCurrentLoadingBar = { 0.f };
public:
		_bool isLoadFinished() {
			return m_fCurrentLoadingBar >= m_fMaxLoadingBar;
		}
		_bool isFadeInFinished() {
			return m_fCurrentLoadingBar >= 5.f;
		}
public:
	void RayFire();
	void isFire() { m_iBulletsLeft = --m_iBulletsLeft; }
	void isReload();
	_uint Get_BullletLeft() { return m_iBulletsLeft; }
	const _bool isEmptyBullet() { return(0==m_iBulletsLeft ); }
private:
		_uint m_iBulletsLeft = { 6 };
		_float m_fSoundFXTime = { 0.f };
		_float m_fEndTime = { 3.f };
public:
	void RayCollInfo();

public:
	void Mouse_Fix();
public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END