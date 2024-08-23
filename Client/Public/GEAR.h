#pragma once
#include "Client_Defines.h"
#include "Item.h"

BEGIN(Engine)
class CCollider;
class CNavigation;
class CShader;
class CModel;
class CCollider;
END

BEGIN(Client)
class CGEAR final :
    public CItem
{
public:
	typedef struct GEARITEM_DESC : public CItem::ITEM_DESC {


		_float4 vDirection;
	}GEARITEM_DESC;

protected:
	CGEAR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGEAR(const CGEAR& rhs);
	virtual ~CGEAR() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _bool Intersect(CCollider* pTargetCollider)override;
	virtual _bool IntersectRay(_vector* pRayArray, _float* fDist) override;
	virtual _bool RayCollInfo(_vector* pRayArray, CGameObject** pGameObject) override;
	HRESULT Check_Model(void* pArg);
protected:
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };


public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

private:

	_float4 m_vDirection = {};


public:
	static CGEAR* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END