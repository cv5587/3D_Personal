#pragma once
#include "Client_Defines.h"
#include "UIBase.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUIItemToggle :
	public CUIBase
{
public:
	typedef struct :public CUIBase::UI_BASE_DESC {
		vector<_uint> ItemType;
		_uint			iQuantity = { 0 };
		wstring		ItemUIName = { TEXT("") };
		wstring		ItemName = { TEXT("") };
		wstring		ItemInfo = { TEXT("") };
		wstring		IconTextureTag = { TEXT("") };
		_float		Durability = { 0.f };
		_float		fWeight = { 0.f };
		_bool* isEquip;
	}UI_ITEMTOGGLE_DESC;

private:
	CUIItemToggle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIItemToggle(const CUIItemToggle& rhs);
	virtual ~CUIItemToggle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;
	virtual HRESULT Action() override;
	HRESULT Set_ItemUIInfo(void* pArg);

	void Set_PickItem(_bool bPick);

	_bool Intersect( );
	HRESULT Reset();
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float							m_fX, m_fY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	_bool							m_inItem = {false};	
	_bool							m_isPick = { false };
	vector<class CGameObject*> m_IconGroup;
	vector <class CGameObject*> m_TextGroup;

	vector <class CGameObject*> m_InfoGroup;
	vector <class CGameObject*> m_InfoButton;

private:
	vector<_uint> m_ItemType;
	wstring		m_ItemUIName = { TEXT("") };
	wstring		m_ItemName = { TEXT("") };
	wstring		m_ItemInfo = { TEXT("") };
	wstring		m_IconTextureTag = { TEXT("") };
	_uint			 m_iQuantity = { 0 };
	_float		m_Durability = { 0.f };
	_float		m_fWeight = { 0.f };
	_bool*									m_bisEquip = { nullptr };
private:
	HRESULT Add_Components();
	HRESULT Add_PartUI();
	HRESULT Bind_ShaderResources();

public:
	static CUIItemToggle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END