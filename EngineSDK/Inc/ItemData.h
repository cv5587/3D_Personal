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
	//������ Ÿ���� ������ ������ �ְ� �̰� �̿�����.
	vector<_uint> m_ItemType;
	_uint			 m_iQuantity = { 0 };
	wstring		m_ItemName = { TEXT("") };

	wstring m_ProtoTypeTag;//��������
	wstring m_ModelTag;//�� �±�

	_float					m_fSpeedPerSec = { 0.0f };
	_float					m_fRotationPerSec = { 0.0f };

public:
	static CItemData* Create(void* pArg);
	virtual void Free() override;
};

END