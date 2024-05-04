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
		_float	fWeight;
	}ITEM_DESC;

	enum ITEMTYPE {
		ITEM_STUFF,//
		ITEM_MEDKITS,//
		ITEM_KINDLING,//
		ITEM_CLOTH,//
		ITEM_FOOD,//
		ITEM_EQUIPMENT,//
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

public:
	wstring Get_TextUIName() {
		return m_ItemUIName	;
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
	vector<ITEMTYPE> Get_ItemType() {
		return m_ItemType;
	}
protected:
	vector<ITEMTYPE> m_ItemType;
	_uint			 m_Quantity = { 1 };
	wstring		m_ItemName = { TEXT("") };
	//Ŭ��
	wstring		m_ItemUIName = { TEXT("") };
	wstring		m_ItemInfo = { TEXT("") };//�����ۼ���
	wstring		m_ItemWeight = { TEXT("") };//�����۹���
	wstring		m_ItemDurability= { TEXT("") };//������ ������
	//�κ��丮 ����
	_float		m_fWeight = { 0.f };
public:
	virtual CGameObject* Clone(void* pArg) = 0;//������ Ŭ�󿡼� �����ϸ� ���ΰ� ���ؼ� ��������� �߻�
	virtual void Free() override;
};

END