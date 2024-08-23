#pragma once
#include "Client_Defines.h"
#include "UIBase.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUIStoveItem final :
	public CUIBase
{
public:
	typedef struct STOVEITEMUI_DESC : public CUIBase::UI_BASE_DESC {
		_uint			iQuantity = { 0 };
		wstring		ItemUIName = { TEXT("") };
		wstring		ItemName = { TEXT("") };
	}STOVEITEMUI_DESC;

protected:
	CUIStoveItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIStoveItem(const CUIStoveItem& rhs);
	virtual ~CUIStoveItem() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render()override;

	HRESULT Set_ItemUIInfo(void* pArg);
	_bool isItem() { return m_InItem;}
private:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	class CGameObject* m_pIcon = { nullptr };
	vector<class CGameObject*> m_pText ;

	wstring		m_ItemUIName = { TEXT("") };
	wstring		m_ItemName = { TEXT("") };
	_uint			 m_iQuantity = { 0 };
	wstring		m_IconTextureTag = { TEXT("") };

	_bool						m_InItem = { false };

	//CUITEXT::UI_DESC* m_pTextDesc = { nullptr };
	_float4x4						 m_ViewMatrix, m_ProjMatrix;
	_float2						m_fSize;
	_float3						m_fPosition;
	_float4						m_vColor;

	_float m_fX, m_fY;
private:
	HRESULT Add_Components();
	HRESULT Add_PartUI();	
	HRESULT Bind_ShaderResources();
public:
	virtual _bool IntersectUI()override;

public:
	static CUIStoveItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;//생성은 클라에서 복사하면 세부값 정해서 만들기위해 추상
	virtual void Free() override;
};
END
