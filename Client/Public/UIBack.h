#pragma once

#include "Client_Defines.h"
#include "UIBase.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

class CUIBack final :
	public CUIBase
{
public:
	typedef struct UI_BACK_DESC :public CUIBase::UI_BASE_DESC {
		_float4 vColor;
		wstring ShaderTag;
	}UI_BACK_DESC;

private:
	CUIBack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIBack(const CUIBack& rhs);
	virtual ~CUIBack() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;
	virtual void UI_Render(_uint IconID) override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float							m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	_float4						m_vColor;
	wstring						m_ShaderTag = { TEXT("") };
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUIBack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

