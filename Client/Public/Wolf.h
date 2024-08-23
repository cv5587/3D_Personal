#pragma once
#include "Monster.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CNavigation;
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)
class CWolf final:
    public CMonster
{
public:
	typedef struct WOLF_DESC : public CMonster::MONSTER_DESC
	{
		_bool isItem;
		_float4x4* pPlayerMatrix;
	}WOLF_DESC;

	enum WOLF {
		WOLF_RUN, WOLF_HOWL, WOLF_IDLELEFT, WOLF_IDLERIGHT,
		WOLF_IDLE, WOLF_SNIFF, WOLF_DEATH, WOLF_CORPSE, WOLF_CHASE, 
		WOLF_ATTACK, WOLF_STRUGGLEWEAPON, WOLF_STRUGGLEEND, WOLF_ESCAPE, WOLF_END
	};

protected:
	CWolf(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWolf(const CWolf& rhs);
	virtual ~CWolf() = default;



public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _bool IntersectRay(_vector* pRayArray, _float* fDist) override;

private:
	_float tt = { 0.f };
	_bool m_bStruggle = { false };
	void Wolf_Action(_float fTimeDelta);
	void WolfAnimControl(_float fTimeDelta);
	void Reset_Timer();
public:
	void Set_State(_uint iState) {
		if (m_iState != WOLF_CORPSE)
			m_iState = (WOLF)iState;
	}

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();
	virtual _bool Detected()override;

public:
	static CWolf* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END