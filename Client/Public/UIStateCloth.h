#pragma once
#include "Client_Defines.h"
#include "UIBase.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUIStateCloth final :
    public CUIBase
{
public:
	typedef struct UI_STATECLOTH_DESC :public CUIBase::UI_BASE_DESC {

	}UI_STATECLOTH_DESC;

private:
	CUIStateCloth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIStateCloth(const CUIStateCloth& rhs);
	virtual ~CUIStateCloth() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;

public:

	void Set_PickItemInfo(_bool bItemPick);
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;


	_bool m_isClothData = {false};
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUIStateCloth* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END