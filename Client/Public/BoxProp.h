#pragma once
#include "Client_Defines.h"
#include "InteractiveObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)
class CBoxProp final:
    public CInteractiveObject
{
public:
	typedef struct BOXPROP_DESC : public CInteractiveObject::INTERACTIVEOBJECT_DESC
	{

	}BOXPROP_DESC;

private:
	CBoxProp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoxProp(const CBoxProp& rhs);
	virtual ~CBoxProp() = default;

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


	HRESULT Make_Item();
private:
	//아이템 데이터 (들고 있게 만들기)
	vector<class CGameObject*> m_InItem;

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();


public:
	static CBoxProp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
