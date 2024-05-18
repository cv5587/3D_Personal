#pragma once

#include "Client_Defines.h"
#include "UIBase.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUIButton :
    public CUIBase
{
public:
	typedef struct :public CUIBase::UI_BASE_DESC {
		_float UIToggleBrightness;
		_bool bisCollider = { false };
	}UI_BUTTON_DESC;

private:
	CUIButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIButton(const CUIButton& rhs);
	virtual ~CUIButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;
	virtual HRESULT Action() override;

	void Set_Brightness(_float value) {
		m_UIToggleBrightness = value;
	}

	// TODO :: 콜라이더 생성 AABB 충돌 제작 마우스 레이와 
private:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float4x4						 m_ViewMatrix, m_ProjMatrix;


	_float						 m_UIToggleBrightness = { 0.f };
	_bool						m_bhasCollider = { false };

	_float2						m_fSize;
	_float3						m_fPosition;

public:
	_bool Intersect_Point();
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUIButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END