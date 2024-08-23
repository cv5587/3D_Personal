#pragma once
#include "Client_Defines.h"
#include "UIBase.h"
#include "UITEXT.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUIActiveButton final:
    public CUIBase
{
public:
    typedef struct ACTIVEUI_DESC : public CUIBase::UI_BASE_DESC {
		_float4 vColor;
		_bool bhasText;
		CUITEXT::UI_DESC* TEXTDesc;
    }ACTIVEUI_DESC;

protected:
    CUIActiveButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUIActiveButton(const CUIActiveButton& rhs);
    virtual ~CUIActiveButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render()override;
private:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	class CGameObject* m_pText = { nullptr };

	_bool						m_bhasText = { false };
	CUITEXT::UI_DESC* m_pTextDesc={nullptr};	
	_float4x4						 m_ViewMatrix, m_ProjMatrix;
	_float2						m_fSize;
	_float3						m_fPosition;
	_float4						m_vColor ;
private:
	HRESULT Add_Components();
	HRESULT Add_Text();
	HRESULT Bind_ShaderResources();
public:
	virtual _bool IntersectUI()override;

public:
	static CUIActiveButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;//생성은 클라에서 복사하면 세부값 정해서 만들기위해 추상
	virtual void Free() override;
};
END
