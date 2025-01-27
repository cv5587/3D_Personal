#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CNavigation;
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)
class CMonster :
    public CLandObject
{
public:
	typedef struct MONSTER_DESC : public CLandObject::LANDOBJ_DESC
	{
		_bool isItem;
		_float4x4* pPlayerMatrix;
	}MONSTER_DESC;


//	static const _uint iState[STATE_END];
protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _bool IntersectRay(_vector* pRayArray, _float* fDist) override;

	virtual _bool Get_isItem() {
		return m_isItem;
	}
	virtual _bool Get_isStuned() {
		return m_bisStunned;
	}

protected:
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };
	class CNavigation* m_pNavigationCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };

protected:
	_float4x4*					m_pPlayerMatrix = {};
	_uint							m_iState = { 0 };
	_float						m_fPatternTime = { 0.f };
	_float						m_fCurrentTime = { 0.f };
	_float						m_fDetectRange = { 0.f };
	_bool						m_Detected = { false };
	_bool						m_bisStunned = { false };
	_bool						m_isItem = { false };
	wstring						m_wUIName = { TEXT("") };
	_bool						m_bEscapeRotate = { false };
protected:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();
	virtual _bool Detected();

public:
	static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END