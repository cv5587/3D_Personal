#pragma once
#include "Client_Defines.h"
#include "InteractiveObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END
BEGIN(Client)
class CPortal final:
    public CInteractiveObject
{
public:
	typedef struct PORTAL_DESC : public CInteractiveObject::INTERACTIVEOBJECT_DESC
	{
		_int  iGoalCellIndex;
		_float4 vGoalPosition;
	}PORTAL_DESC;

private:
	CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPortal(const CPortal& rhs);
	virtual ~CPortal() = default;

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
	virtual _bool RayCollInfo(_vector* pRayArray, CGameObject** pGameObject) override;

private:
	_int m_iGoalCellIndex = { -1 };
	_float4 m_vGoalPosition = { };
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();


public:
	static CPortal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
