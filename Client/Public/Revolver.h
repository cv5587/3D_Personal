#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CRevolver final : public CPartObject
{
public:
	typedef struct REVOLVER_DESC : public CPartObject::PARTOBJ_DESC
	{
		_uint* pBulletsLeft;
		_bool* pBodyAnimFinish;
		const _float4x4* pBulletMatrix;
		const _float4x4* pCombinedTransformationMatrix;
	}REVOLVER_DESC;
private:
	CRevolver(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRevolver(const CRevolver& rhs);
	virtual ~CRevolver() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT Normal_Render(_uint BulletIndex);
	HRESULT Bullet_Render(_uint BulletIndex);
private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
public:
	_bool Reload_Time();
	void Set_Shot() { m_bShot = true; }
private:
	_float4x4				m_BulletWorldMatrix;
	const _float4x4* m_pSocketMatrix = { nullptr };
	const _float4x4* m_pBulletMatrix = { nullptr };
	_uint* m_pBulletsLeft = { nullptr };
	_bool* m_pBodyAnimFinish = { nullptr };
	_uint m_iTotalBullets = { 6 };
	_uint m_iCurrentReloadBullet = { 0 };
	_uint m_iReloadBullet = { 0 };

	_float m_ReloadTime = { 0.f };

	class CGameObject* m_pEffect = { nullptr };

	_bool m_bShot = { false };
	_bool m_bSmog = { false };
	_float m_fSmogTime = { 0.f };
private:
	void Set_AnimationState();
public:
	void Set_Smog() { m_bSmog = true; }
	void Reset_Anim();
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_BulletShaderResources();
	void Add_Particle();
public:
	static CRevolver* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END