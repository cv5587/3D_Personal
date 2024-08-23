#pragma once
#include "Client_Defines.h"
#include "UIBase.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUIBluePrintToggle :
	public CUIBase
{
public:
	enum STATE{STATE_NORMAL , STATE_LOAD , STATE_END	};
public:
	typedef struct UI_BLUEPRINTTOGGLE_DESC :public CUIBase::UI_BASE_DESC {

		_float4 vColor;
		wstring ShaderTag;
		wstring UINameTag;
		wstring NameTag;
		_float	fUsingTime;
		wstring FirstIngredient;
		wstring SecondIngredient;
		_bool bIsCollider;

	}UI_BLUEPRINTTOGGLE_DESC;

private:
	CUIBluePrintToggle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIBluePrintToggle(const CUIBluePrintToggle& rhs);
	virtual ~CUIBluePrintToggle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Choice_Render() override;

	void Set_PickItem(_bool bPick);

	_bool Intersect();

	void Set_CheckInven();
	_bool Make_CheckInven();
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float4 m_vColor = {};
	wstring m_ShaderTag = { TEXT("") };

	_float							m_fX, m_fY;
	_float4x4						m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	_bool							m_isPick = { false };

	vector <class CGameObject*> m_TextGroup;//갯수 나타내기
	vector <class CGameObject*> m_InfoButton;


	vector<wstring> m_wLayerName;
private:
	//내가 제작할 아이템 
	wstring		m_ItemUIName = { TEXT("") };
	wstring		m_ItemName = { TEXT("") };
	_bool						m_bhasCollider = { false };

	//필요 재료
	wstring m_FirstIngredient = { TEXT("") };
	wstring m_SecondIngredient = { TEXT("") };
	_float		m_fUsingTime = { 0.f };

	_uint m_iFirstEA = { 1 };
	_uint m_iSecondEA = { 1 };

	_float3						m_fPosition;
	_float2						m_fSize;

	STATE m_eState = { STATE_NORMAL };

private:
	HRESULT Add_Components();
	HRESULT Add_PartUI();
	_bool Intersect_Point();
	HRESULT Bind_ShaderResources();

public:
	static CUIBluePrintToggle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END