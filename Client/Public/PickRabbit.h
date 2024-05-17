#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPickRabbit final : public CPartObject
{
public:
		typedef struct : public CPartObject::PARTOBJ_DESC
		{
			const _float4x4* pCombinedTransformationMatrix;
		}PICKRABBIT_DESC;

private:
	CPickRabbit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPickRabbit(const CPickRabbit& rhs);
	virtual ~CPickRabbit() = default;

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
	void Set_AnimationState();
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CPickRabbit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END