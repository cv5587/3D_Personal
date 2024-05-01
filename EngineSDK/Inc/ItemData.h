#pragma once
#include "Base.h"

BEGIN(Engine)
class CItemData final:
    public CBase
{
private:
	CItemData();
	CItemData(const CItemData& rhs);
	virtual ~CItemData() = default;

public:
	HRESULT Initialize(void* pArg) ;
public:
	_bool Compare_Name(const wstring ItemName);
	void Add_Quantity() { m_iQuantity++; }
private:
	//아이템 타입은 여러개 가지고 있고 이걸 이용하자.
	vector<_uint> m_ItemType;
	_uint			 m_iQuantity = { 0 };
	wstring		m_ItemName = { TEXT("") };

	wstring m_ProtoTypeTag;//참조원형
	wstring m_ModelTag;//모델 태그

	_float					m_fSpeedPerSec = { 0.0f };
	_float					m_fRotationPerSec = { 0.0f };

public:
	static CItemData* Create(void* pArg);
	virtual void Free() override;
};

END