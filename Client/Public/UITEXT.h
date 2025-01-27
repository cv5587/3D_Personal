#pragma once

#include "Client_Defines.h"
#include "UIBase.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
END

class CUITEXT final :
    public CUIBase
{
public:
	typedef struct UI_DESC :public CUIBase::UI_BASE_DESC {
		wstring TextTag;
		wstring Font;
		_float2	TextPosition;
		_float4 Color;
	}UI_DESC;

private:
	CUITEXT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUITEXT(const CUITEXT& rhs);
	virtual ~CUITEXT() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;


public:
	void Set_Text(wstring Text);
	void Switch_Text(wstring Text);
	wstring* Get_pText() {
		return &m_TextTag;
	}
	void Set_Color(_float4 vColor) { m_Color = vColor; }
private:

	_float							m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	wstring						m_TextTag = { TEXT("") };
	wstring						m_Font = { TEXT("") };
	_float2						m_TextPosition = { 0.f, 0.f };
	_float2						m_ResultPosition = { 0.f, 0.f };
	_float4						m_Color = {1.f, 1.f, 1.f, 1.f};	

	_bool						m_bFixPosition = { false };
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUITEXT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

