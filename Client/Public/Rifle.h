#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CRifle : public CPartObject
{
public:
	typedef struct RIFLE_DESC : public CPartObject::PARTOBJ_DESC
	{
		const _float4x4* pCombinedTransformationMatrix;
		const _uint* pNpcState;
	}RIFLE_DESC;

private:
	CRifle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRifle(const CRifle& rhs);
	virtual ~CRifle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	const _float4x4* m_pSocketMatrix = { nullptr };
	const _uint* m_pState = { nullptr };
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CRifle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END