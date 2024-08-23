#pragma once
#include "Monster.h"
BEGIN(Client)
class CRabbit final:
    public CMonster
{
public:
	typedef struct RABBIT_DESC :public CMonster::MONSTER_DESC {

	}RABBIT_DESC;

	enum STATE {
		STATE_WALK, STATE_RUN, STATE_GROOM, STATE_IDLELEFT, STATE_IDLERIGHT,
		STATE_IDLE, STATE_DEATH, STATE_CORPSE, STATE_HOPE, STATE_END
	};

protected:
	CRabbit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRabbit(const CRabbit& rhs);
	virtual ~CRabbit() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _bool IntersectRay(_vector* pRayArray, _float* fDist) override;

private:
	void RabbitAnimControl(_float fTimeDelta);
	void Rabbit_Action(_float fTimeDelta);
	void Reset_Timer();

public:
	void Set_State(_uint iState) {
		if (m_iState != STATE_CORPSE)
			m_iState = (STATE)iState;
	}


private:
	class CGameObject* pEyeEffect = { nullptr };
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Add_PartObject();
	virtual _bool Detected()override;

public:
	static CRabbit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END