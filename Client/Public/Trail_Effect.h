#pragma once
#include "Client_Defines.h"
#include "BlendObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Trail;
END


BEGIN (Client)
class CTrail_Effect final:
    public CBlendObject
{
public:
	typedef struct TRAILEFFECT_DESC :public CBlendObject::GAMEOBJECT_DESC
	{
		_float4x4* pParentsMatrix;
		const _float4x4* fControlMatrix;
	}TRAILEFFECT_DESC;


private:
	CTrail_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrail_Effect(const CTrail_Effect& rhs);
	virtual ~CTrail_Effect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Trail* m_pVIBufferCom = { nullptr };

private:
	_float4x4* m_pParentsMatrix = { nullptr };
	const _float4x4* m_fControlMatrix = { nullptr };
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTrail_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END