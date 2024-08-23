#pragma once

#include "Client_Defines.h"
#include "UIBase.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUIToggleID final :
	public CUIBase
{
public:
	typedef struct UI_TOGGLE_DESC :public CUIBase::UI_BASE_DESC {
		_uint UISceneIndex;
	}UI_TOGGLE_DESC;

private:
	CUIToggleID(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIToggleID(const CUIToggleID& rhs);
	virtual ~CUIToggleID() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;

	_uint Get_SceneIndex() {
		return m_UISceneIndex;
	}
	wstring Get_SceneText() { return m_SceneText; }
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float							m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	_uint m_UISceneIndex = { 0 };
	wstring m_SceneText = { TEXT("") };
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUIToggleID* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
END
