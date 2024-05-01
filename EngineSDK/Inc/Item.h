#pragma once
#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CItem :
    public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC {
		_uint iQuantity;
		_uint ItemType[2];
		wstring ItemName;
	}ITEM_DESC;

	enum ITEMTYPE {
		ITEM_KINDLING,
		ITEM_MEDKITS,
		ITEM_CLOTH,
		ITEM_FOOD,
		ITEM_EQUIPMENT,
		ITEM_STUFF,
		ITEM_END
	};

protected:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CGameObject& rhs);
	virtual ~CItem() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	wstring Get_ItemName() { return m_ItemName; }

	virtual void Make_Description(void* pArg) override;

	void* Get_Description(void* pDesc) ;

	HRESULT Save_Data(ofstream* fout);
protected:
	vector<ITEMTYPE> m_ItemType;
	_uint			 m_Quantity = { 1 };
	wstring		m_ItemName = { TEXT("") };

public:
	virtual CGameObject* Clone(void* pArg) = 0;//생성은 클라에서 복사하면 세부값 정해서 만들기위해 추상
	virtual void Free() override;
};

END