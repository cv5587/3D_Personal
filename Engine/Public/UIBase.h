#pragma once
#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CUIBase :
    public CGameObject
{
public:
	typedef struct UI_BASE_DESC : public CGameObject::GAMEOBJECT_DESC {
		wstring TextureTag;
		_uint Icon_ID;
		_int UIID;
		_float2 LowerBound;
		_float2 UpperBound;
	}UI_BASE_DESC;
protected:
	CUIBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIBase(const CGameObject& rhs);
	virtual ~CUIBase() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _bool IntersectUI()override;
	virtual HRESULT Action()override;
public:
	void Set_IconID(_uint ID) { m_iIconID = ID; }

	virtual _bool Compare_UIID(const int& UIID);
protected :
	wstring						m_TextureTag = { TEXT("") };
	_uint							m_iIconID = { 0 };
	_int							m_UIID = { -1 };

public:
	virtual CGameObject* Clone(void* pArg) = 0;//생성은 클라에서 복사하면 세부값 정해서 만들기위해 추상
	virtual void Free() override;
};

END