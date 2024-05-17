#pragma once

#include "Client_Defines.h"
#include "UIBase.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
END

class CUIColor final :
	public CUIBase
{
public:
	typedef struct :public CUIBase::UI_BASE_DESC {
		_float4 vColor;
	}UI_COLOR_DESC;

private:
	CUIColor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIColor(const CUIColor& rhs);
	virtual ~CUIColor() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;


private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float							m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	_float4						m_vColor;
	wstring						m_ShaderTag = { TEXT("") };

private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUIColor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

