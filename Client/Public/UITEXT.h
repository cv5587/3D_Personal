#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
END

class CUITEXT final :
    public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC {
		wstring TextTag;
		wstring Font;
		_float2	TextPosition;
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
	void Set_Text(wstring Text)
	{
		m_TextTag = Text;
	}

private:

	_float							m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	wstring						m_TextTag = { TEXT("") };
	wstring						m_Font = { TEXT("") };
	_float2						m_TextPosition = { 0.f, 0.f };
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUITEXT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

