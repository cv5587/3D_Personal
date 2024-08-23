#pragma once
#include "Client_Defines.h"
#include "InteractiveObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)
class CBedRoll :
    public CInteractiveObject
{
public:
	typedef struct BEDROLL_DESC : public CInteractiveObject::INTERACTIVEOBJECT_DESC
	{
		_uint iShaderPass;
	}BEDROLL_DESC;

private:
	CBedRoll(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBedRoll(const CBedRoll& rhs);
	virtual ~CBedRoll() = default;

private:
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _bool IntersectRay(_vector* pRayArray, _float* fDist) override;
	virtual HRESULT Action() override;
	virtual _bool RayCollInfo(_vector* pRayArray, CGameObject** pGameObject)override;

private:
	_uint m_iShaderPass= { 0};
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBedRoll* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END