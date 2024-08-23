#pragma once
#include "Client_Defines.h"
#include "UIBase.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUIBlood :
    public CUIBase
{
public:
	typedef struct UI_BLOOD_DESC :public CUIBase::UI_BASE_DESC {


	}UI_BLOOD_DESC;

private:
	CUIBlood(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIBlood(const CUIBlood& rhs);
	virtual ~CUIBlood() = default;

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
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };


	_float							m_fX, m_fY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	_float4						m_vColor = {0.4f, 0.f, 0.f, 1.f};
	_float						m_fTime = { 0.f };
	_float						m_fMaxTime = { 1.5f };
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();
public:
	static CUIBlood* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END