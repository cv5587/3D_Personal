#pragma once

#include "Client_Defines.h"
#include "BlendObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Instance_Rect;
END

BEGIN(Client)

class CParticle_Rect final : public CBlendObject
{
public :
	enum TYPE { TYPE_SNOW , TYPE_BREATH ,  TYPE_FIRE , TYPE_TRAIL, TYPE_FLARETRAIL, TYPE_FLARE,TYPE_FOOT, TYPE_END};
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		const _float4x4* pParentMatrix;
		CVIBuffer_Instance_Rect::INSTANCE_DESC BufferInstance;
		_float4			vStartPos;
		const _float4x4*		pBreathPos;
		TYPE				iTYPE;
	}PARTICLE_DESC;
private:
	CParticle_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Rect(const CParticle_Rect& rhs);
	virtual ~CParticle_Rect() = default;

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
	CVIBuffer_Instance_Rect* m_pVIBufferCom = { nullptr };
	CVIBuffer_Instance_Rect::INSTANCE_DESC* m_pBufferInstance;
	_uint m_iType = { TYPE_END };
	const _float4x4* m_pParentMatrix = { nullptr };
	const _float4x4* m_pBreathPos = { nullptr };
	_float4x4						 m_ViewMatrix, m_ProjMatrix;

	_float4 m_vStartPos;
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CParticle_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END