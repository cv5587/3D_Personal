#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Instance_Point;
END

BEGIN(Client)

class CParticle_Point final : public CGameObject
{
public:
	typedef struct PARTICLE_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		_float4			vStartPos;
		CVIBuffer_Instance_Point::INSTANCE_DESC BufferInstance;
	}PARTICLE_DESC;
private:
	CParticle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Point(const CParticle_Point& rhs);
	virtual ~CParticle_Point() = default;

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
	CVIBuffer_Instance_Point* m_pVIBufferCom = { nullptr };
	CVIBuffer_Instance_Rect::INSTANCE_DESC* m_pBufferInstance;

private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CParticle_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END