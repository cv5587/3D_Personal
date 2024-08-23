#pragma once
#include "Client_Defines.h"
#include "UIBase.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUIStoveIcon :
	public CUIBase
{
public:
	typedef struct STOVEICON_DESC :public CUIBase::UI_BASE_DESC {
		_uint			iQuantity = { 0 };
		wstring		ItemUIName = { TEXT("") };
		wstring		ItemName = { TEXT("") };
		wstring		IconTextureTag = { TEXT("") };
		_bool		bInItem;
	}STOVEICON_DESC;

private:
	CUIStoveIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIStoveIcon(const CUIStoveIcon& rhs);
	virtual ~CUIStoveIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;

	HRESULT Set_ItemUIInfo(void* pArg);

	HRESULT Reset();
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float							m_fX, m_fY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	_bool							m_inItem = {false};	
	vector <class CGameObject*> m_TextGroup;

private:
	wstring		m_ItemUIName = { TEXT("") };
	wstring		m_ItemName = { TEXT("") };
	wstring		m_IconTextureTag = { TEXT("") };
	_uint			 m_iQuantity = { 0 };

private:
	HRESULT Add_Components();
	HRESULT Add_PartUI();
	HRESULT Bind_ShaderResources();

public:
	static CUIStoveIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END