#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CItemData final :
	public CBase
{
private:
	CItemData();
	CItemData(const CItemData& rhs);
	virtual ~CItemData() = default;

public:
	HRESULT Initialize(void* pArg);
public:
	_bool Compare_Name(const wstring ItemName);
	void Add_Quantity() { m_iQuantity++; }
	
	_bool QuestItemFinish();
	_bool Compare_Type(_uint TypeIndex);

	void Drop_Item(void* pDesc);
	void Make_ItemDataDesc(void* pDesc);
	_bool	Using();
private:
	//아이템 타입은 여러개 가지고 있고 이걸 이용하자.
	vector<_uint> m_ItemType;
	_uint			 m_iQuantity = { 0 };
	wstring		m_ItemName = { TEXT("") };
	wstring		m_ItemUIName = { TEXT("") };
	wstring		m_ItemInfo = { TEXT("") };

	wstring m_ProtoTypeTag;//참조원형
	wstring m_ModelTag;//모델 태그

	_float					m_fSpeedPerSec = { 0.0f };
	_float					m_fRotationPerSec = { 0.0f };

	_float		m_Durability = { 0.f };
	_float		m_fWeight = { 0.f };
	_bool		m_isEquip = { false };
	_int			m_iCellIndex = { -1 };
public:
	_bool Get_isEquip() { return m_isEquip; }
public:
	static CItemData* Create(void* pArg);
	virtual void Free() override;
};

END