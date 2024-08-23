#pragma once

#include "Client_Defines.h"
#include "UIBase.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUILoadingStruggle :
    public CUIBase
{
public:
	typedef struct UI_LOADINGSTRUGGLE_DESC :public CUIBase::UI_BASE_DESC {

	}UI_LOADINGSTRUGGLE_DESC;

private:
	CUILoadingStruggle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUILoadingStruggle(const CUILoadingStruggle& rhs);
	virtual ~CUILoadingStruggle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;

	void Set_CurrentBar(_float* pCurrentBar) {
		m_pCurrentLoadingBar = pCurrentBar;
	}
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float							m_fX, m_fY;
	_float4x4						m_ViewMatrix, m_ProjMatrix;

	_float* m_pCurrentLoadingBar = { nullptr };
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUILoadingStruggle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END