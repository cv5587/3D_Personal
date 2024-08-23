#pragma once
#include "Client_Defines.h"
#include "InteractiveObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)
class CStove :
    public CInteractiveObject
{
	enum PART{PART_RAWRABBIT , PART_COOKEDRABBIT , PART_END};
public:
	typedef struct STOVE_DESC : public CInteractiveObject::INTERACTIVEOBJECT_DESC
	{

	}STOVE_DESC;

private:
	CStove(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStove(const CStove& rhs);
	virtual ~CStove() = default;

private:
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };

	CGameObject* m_pEffect = { nullptr };
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


public:
	HRESULT Cooking(void* pDesc);
	void Set_Fire(_bool bFire) { m_bFire = bFire; }
	_bool Get_isFire() {
		return m_bFire;
	}
private:
	vector<class CGameObject*> m_InItem;//요리중인 아이템 체크
	_float3 m_fMeatPos[2] = {};
	_float m_fCookTime[2] = {0.f ,};
	_bool m_bFire = { false };

public:
	HRESULT Add_Components();
	HRESULT Add_Part();
	HRESULT Bind_ShaderResources();


public:
	static CStove* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END