#pragma once

#include "Component.h"

/* 객체의 월드 변환 행렬을 보관한다. */
/* 월드 변환 상태를 제어하기위한 기능. */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}TRANSFORM_DESC;

public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
	enum ROTATE{ROT_X,ROT_Y,ROT_Z,ROT_END};
private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_float3 Get_Scaled() {
		return _float3(XMVector3Length(Get_State(STATE_RIGHT)).m128_f32[0],
								XMVector3Length(Get_State(STATE_UP)).m128_f32[0],
								XMVector3Length(Get_State(STATE_LOOK)).m128_f32[0]);
	}
	_float4x4* Get_WorldFloat4x4() {
		return &m_WorldMatrix;
	}
	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}
	_matrix Get_WorldMatrix_Inverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}
	_float Get_SpeedValue() {
		return m_fSpeedPerSec;
	}
	_float Get_RotateValue() {
		return m_fRotationPerSec;
	}
public:
	void Set_State(STATE eState, _fvector vState);
	void Set_State_Matrix(_fmatrix mState);
	void Set_DeltaValue(_float _fSpeedPerSec, _float _fRotationPerSec) {
		m_fSpeedPerSec = _fSpeedPerSec;
		m_fRotationPerSec = _fRotationPerSec;
	}
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Scaling(_float fScaleX, _float fScaleY, _float fScaleZ);
	void Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ);

	HRESULT Go_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT Go_Backward(_float fTimeDelta, class  CNavigation* pNavigation = nullptr);
	HRESULT Go_Left(_float fTimeDelta, class  CNavigation* pNavigation = nullptr);
	HRESULT Go_Right(_float fTimeDelta, class  CNavigation* pNavigation = nullptr);

	HRESULT Go_LeftStraight(_float fTimeDelta, class  CNavigation* pNavigation = nullptr);
	HRESULT Go_RightStraight(_float fTimeDelta, class  CNavigation* pNavigation = nullptr);
	HRESULT Go_LeftBackward(_float fTimeDelta, class  CNavigation* pNavigation = nullptr);
	HRESULT Go_RightBackward(_float fTimeDelta, class  CNavigation* pNavigation = nullptr);

	HRESULT LookAt(_fvector vTargetPosition);
	HRESULT LookAt_For_LandObject(_fvector vTargetPosition);
	HRESULT Turn(_fvector vAxis, _float fTimeDelta);
	HRESULT Rotation(_fvector vAxis, _float fRadian);
	HRESULT Rotaion_Reset();

	HRESULT Rotate(_uint _rot, _float fTimeDelta);

	HRESULT Rotate_Radian(_uint _rot, _float fRadian);

	HRESULT Throw(_float4* fDir, _float fTimeDelta);

	HRESULT Patrol(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT Escape(_fmatrix mPlayer , _float fTimeDelta, class  CNavigation* pNavigation = nullptr);
public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);

public:
	void Make_Description(void* pArg);
private:
	/* row major */
	_float4x4				m_WorldMatrix;
	_float					m_fSpeedPerSec = { 0.0f };
	_float					m_fRotationPerSec = { 0.0f };

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END