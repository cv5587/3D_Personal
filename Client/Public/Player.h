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
	enum PART { PART_BODY,  PART_STONE, PART_REOVLVER, PART_RABBIT, PART_PIPE, PART_END };
	
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
	void Set_UnEquip() {
		m_eEquip = PLAYEREQUIP::EQUIP_NONE;
	}
	void End_Change(){ m_bChangeEquip = false; }

	void Throw();
public:
	void Player_Turn(_float fTimeDelta,_long MouseMove);
	_bool Pick_up();
	void Puck_up_Update(_float fTimeDelta);

	void Add_Item();
	void Add_Rabbit();
	void Drop_Item();
	HRESULT Inventory_Drop(wstring ItemName);
	HRESULT Inventory_DropRabbit(wstring ItemName);

	void Cam_Turn(_float fTimeDelta, _long MouseMove);
	
public:
	void Pick_UI();
public:
	void Inventory_Update(_float fTimeDelta);
	const PLAYERCONDITION isCondition() { return m_eCondition; }
	const PLAYEREQUIP			isEquip() { return m_eEquip; }

	const _bool						isEquipChange() { return m_bChangeEquip; }
	const _bool						isAnimFinished() { return m_bAnimFinished; }
	const _bool						isRevolver_AnimFin() { return m_bRevolver_AnimFin; }
	const _bool						isRabbit_AnimFin() { return m_bRabbit_AnimFin; }
	const _bool						isRabbitCatch() { return m_bRabbitCatch; }

	void Set_Reload_Reset();

public:
	void Set_SceneSelect(_uint iSceneIndex);
	void Set_RabbitCatch(_bool isCatch) { m_bRabbitCatch = isCatch; }
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
	
	_bool											m_bRevolver_AnimFin = { true };
	_bool											m_bRabbit_AnimFin = { false };
	//pickupselector 돌릴지 말지 확인용 
	_bool											m_bAcquire = {false};
	_bool											m_bRabbitCatch = { false };

	CGameObject*									m_pRabbit = {nullptr};

	class CNavigation* m_pNavigationCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };
	class CInventory* m_pInventory = { nullptr };
	class CPickUpSelector* m_pPickUpSelector = { nullptr };
	class CUIInventory* m_pUIInventory = { nullptr };
	class CUImanager* m_pUImanager = { nullptr };
private:
	class CBone* m_pCamBone = { nullptr };
	class CPlayer_Camera* m_pCamera = { nullptr };

public:
	void isFire() { m_iBulletsLeft = --m_iBulletsLeft; }
	void isReload() { m_iBulletsLeft = ++m_iBulletsLeft; }
	_uint Get_BullletLeft() { return m_iBulletsLeft; }
	const _bool isEmptyBullet() { return(0==m_iBulletsLeft ); }
private:
		_uint m_iBulletsLeft = { 6 };
public:
	void RayCollInfo(const wstring Objname, CGameObject* pRabbit);

public:
	void Mouse_Fix();
public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END