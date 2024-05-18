#pragma once

#include "Client_Defines.h"
#include "UIBase.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUISortToggle final :
	public CUIBase
{
public:
	typedef struct :public CUIBase::UI_BASE_DESC {
		_uint UISortIndex;
		_float UIToggleBrightness;//ÀÏ´Ü µÖº¸ÀÚ

	}UI_SORT_DESC;

private:
	CUISortToggle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUISortToggle(const CUISortToggle& rhs);
	virtual ~CUISortToggle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;

	_uint Get_SortIndex() {
		return m_UISortIndex;
	}
	void Set_Brightness(_float value) {
		m_UIToggleBrightness = value;
	}
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float							m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	_uint m_UISortIndex = { 0 };
	_float m_UIToggleBrightness = { 0.f };
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUISortToggle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
END
