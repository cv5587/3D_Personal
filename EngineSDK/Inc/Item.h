#pragma once
#include "GameObject.h"
#include "Navigation.h"
#include "Collider.h"


BEGIN(Engine)
class ENGINE_DLL CItem :
	public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC {
		_uint iQuantity;
		_uint ItemType[2];
		wstring ItemName;
		wstring ItemUIName;
		wstring ItemInfo;
		_float	Durability;
		_float	fWeight;
		_bool	bThrow;

		_bool*	isEquip;
		
	}ITEM_DESC;

	enum ITEMTYPE {
		ITEM_KINDLING,//
		ITEM_MEDKITS,//
		ITEM_CLOTH,//
		ITEM_FOOD,//
		ITEM_EQUIPMENT,//
		ITEM_STUFF,//
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
	virtual _bool Intersect(class CCollider* pTargetCollider) override;

public:
	wstring Get_ItemName() { return m_ItemName; }

	virtual void Make_Description(void* pArg) override;

	void* Get_Description(void* pDesc);

	HRESULT Save_Data(ofstream* fout);

public:
	wstring Get_TextName() {
		return m_ItemName;
	}
	wstring Get_TextUIName() {
		return m_ItemUIName;
	}
	wstring Get_TextInfo() {
		return m_ItemInfo;
	}
	wstring Get_TextWeight() {
		return m_ItemWeight;
	}
	wstring Get_TextDurability() {
		return m_ItemDurability;
	}
	vector<_uint> Get_ItemType() {
		return m_ItemType;
	}
	_float Get_Weight() {
		return m_fWeight;
	}
	_uint Get_Quantity() {
		return m_Quantity;
	}
	_float Get_Durability() {
		return m_Durability;
	}
protected:
	class CNavigation* m_pNavigationCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };
	vector<_uint> m_ItemType;
	_uint			 m_Quantity = { 1 };
	wstring		m_ItemName = { TEXT("") };

	//Ŭ��
	wstring		m_ItemUIName = { TEXT("") };//������ �ѱ� �̸�
	wstring		m_ItemInfo = { TEXT("") };//�����ۼ���
	wstring		m_ItemWeight = { TEXT("") };//�����۹���
	wstring		m_ItemDurability = { TEXT("") };//������ ������

	//�κ��丮 ����
	_bool		m_bThrow = { false };
	_float		m_Durability = { 0.f };
	_float		m_fWeight = { 0.f };
	//����
	_bool*		m_bisEquip = { nullptr };
public:
	virtual CGameObject* Clone(void* pArg) = 0;//������ Ŭ�󿡼� �����ϸ� ���ΰ� ���ؼ� ��������� �߻�
	virtual void Free() override;
};

END