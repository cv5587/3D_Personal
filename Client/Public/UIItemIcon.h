#pragma once

#include "Client_Defines.h"
#include "UIBase.h"
#include "Item.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CItemData;
END

BEGIN(Client)
class CUIItemIcon :
	public CUIBase
{
public:
	typedef struct :public CUIBase::UI_BASE_DESC {

	}UI_ITEMICON_DESC;

private:
	CUIItemIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIItemIcon(const CUIItemIcon& rhs);
	virtual ~CUIItemIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;
	virtual HRESULT Action() override;

	HRESULT Set_Texture(wstring TextureTag);

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float							m_fX, m_fY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

private:
	vector<_uint> m_ItemType;
	wstring		m_ItemUIName = { TEXT("") };
	wstring		m_ItemName = { TEXT("") };
	wstring		m_ItemInfo = { TEXT("") };
	_uint			 m_iQuantity = { 0 };
	_float		m_Durability = { 0.f };
	_float		m_fWeight = { 0.f };


	//한글이름, 설명 ,갯수 ,내구도 , 무게, 아이콘 텍스처 태그
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUIItemIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END