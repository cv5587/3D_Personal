#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CModel;
class CShader;
class CBone;
END


BEGIN(Client)
class CMother final:
    public CGameObject
{//몸체는 바디오브젝트 , 파트는 라이플public:
public:
	typedef struct NPC_DESC : public CGameObject::GAMEOBJECT_DESC
	{

	}NPC_DESC;
public:
	 enum NPCSTATE {NPC_IDLE , NPCSTATE_END};
private:
	CMother(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMother(const CMother& rhs);
	virtual ~CMother() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _bool IntersectRay(_vector* pRayArray, _float* fDist) override;
	HRESULT Action() ;

public:
	HRESULT Add_Components();
	HRESULT Add_PartObjects();
	HRESULT Bind_ShaderResources();
private:
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };
	_uint								m_eState = { 0 };


	void AnimControl();
public:
	static CMother* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END