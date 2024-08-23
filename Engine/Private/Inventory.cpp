#include "Inventory.h"
#include "ItemData.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Item.h"

CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent{pDevice,pContext}
{
}

CInventory::CInventory(const CInventory& rhs)
	:CComponent{rhs}
{
}

HRESULT CInventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CInventory::Add_Item(CItem* pItem)
{
	wstring wItemName = pItem->Get_ItemName();

	for (auto iter : m_Inventory)
		if(iter->Compare_Name(wItemName))
		{ 
			//������ ��ġ�� �Ÿ� ��������
			//������ ���� ������ ����
			iter->Add_Quantity();
			return S_OK;
		}

	CItem::ITEM_DESC Desc={};

	CItemData* pItemData = CItemData::Create(pItem->Get_Description(&Desc));

	m_Inventory.emplace_back(pItemData);

	m_fTotalWeight += Desc.fWeight * Desc.iQuantity;

	return S_OK;
}

HRESULT CInventory::Add_Rabbit()
{

	CItem::ITEM_DESC Desc={};
	Desc.Durability = 100.f;
	Desc.fRotationPerSec = XMConvertToRadians(120.f);
	Desc.fSpeedPerSec = 2.f;
	Desc.fWeight = 2.48f;
	Desc.iQuantity = 1;
	Desc.ItemInfo = TEXT("��ġ�� �䳢 ��ü. ä���Ͽ� ���, ���� �Ǵ� \n������ ����� �� �ֽ��ϴ�.");
	Desc.ItemName = TEXT("Rabbit");
	Desc.ItemType[0] = 5;
	Desc.ItemType[1] = 6;
	Desc.ItemUIName = TEXT("�䳢 ��ü");
	Desc.ModelTag = TEXT("Prototype_Component_Model_Rabbit");
	Desc.ProtoTypeTag = TEXT("Prototype_GameObject_Monster");

	CItemData* pItemData = CItemData::Create(&Desc);

	m_Inventory.emplace_back(pItemData);

	m_fTotalWeight += Desc.fWeight * Desc.iQuantity;

	return S_OK;
}

HRESULT CInventory::Add_RabbitMeat()
{
	for (auto iter : m_Inventory)
		if (iter->Compare_Name(TEXT("RawRabbit")))
		{
			//������ ��ġ�� �Ÿ� ��������
			//������ ���� ������ ����
			iter->Add_Quantity();
			return S_OK;
		}

	CItem::ITEM_DESC Desc = {};
	Desc.Durability = 100.f;
	Desc.fRotationPerSec = XMConvertToRadians(120.f);
	Desc.fSpeedPerSec = 2.f;
	Desc.fWeight = 0.05f;
	Desc.iQuantity = 1;
	Desc.ItemInfo = TEXT("�䳢�� ������ ���� ����Դϴ�.\n");
	Desc.ItemName = TEXT("RawRabbit");
	Desc.ItemType[0] = 3;
	Desc.ItemType[1] = 6;
	Desc.ItemUIName = TEXT("�� �䳢���");
	Desc.ModelTag = TEXT("Prototype_Component_Model_RawRabbit");
	Desc.ProtoTypeTag = TEXT("Prototype_GameObject_GEAR");

	CItemData* pItemData = CItemData::Create(&Desc);

	m_Inventory.emplace_back(pItemData);

	m_fTotalWeight += Desc.fWeight * Desc.iQuantity;
	return S_OK;
}

HRESULT CInventory::Add_RabbitRut()
{
	for (auto iter : m_Inventory)
		if (iter->Compare_Name(TEXT("Rut")))
		{
			//������ ��ġ�� �Ÿ� ��������
			//������ ���� ������ ����
			iter->Add_Quantity();
			return S_OK;
		}


	CItem::ITEM_DESC Desc = {};
	Desc.Durability = 100.f;
	Desc.fRotationPerSec = XMConvertToRadians(120.f);
	Desc.fSpeedPerSec = 2.f;
	Desc.fWeight = 0.05f;
	Desc.iQuantity = 1;
	Desc.ItemInfo = TEXT("�䳢�� ������ ���� ���� �Դϴ�..\n");
	Desc.ItemName = TEXT("Rut");
	Desc.ItemType[0] = 3;
	Desc.ItemType[1] = 6;
	Desc.ItemUIName = TEXT(" ����");
	Desc.ModelTag = TEXT("Prototype_Component_Model_Rut");
	Desc.ProtoTypeTag = TEXT("Prototype_GameObject_GEAR");

	CItemData* pItemData = CItemData::Create(&Desc);

	m_Inventory.emplace_back(pItemData);

	m_fTotalWeight += Desc.fWeight * Desc.iQuantity;

	return S_OK;
}

HRESULT CInventory::Add_RabbitPelt()
{
	for (auto iter : m_Inventory)
		if (iter->Compare_Name(TEXT("Pelt")))
		{
			//������ ��ġ�� �Ÿ� ��������
			//������ ���� ������ ����
			iter->Add_Quantity();
			return S_OK;
		}

	CItem::ITEM_DESC Desc = {};
	Desc.Durability = 100.f;
	Desc.fRotationPerSec = XMConvertToRadians(120.f);
	Desc.fSpeedPerSec = 2.f;
	Desc.fWeight = 0.05f;
	Desc.iQuantity = 1;
	Desc.ItemInfo = TEXT("�䳢�� ������ ���� ���� �Դϴ�..\n");
	Desc.ItemName = TEXT("Pelt");
	Desc.ItemType[0] = 5;
	Desc.ItemType[1] = 6;
	Desc.ItemUIName = TEXT(" ����");
	Desc.ModelTag = TEXT("Prototype_Component_Model_Pelt");
	Desc.ProtoTypeTag = TEXT("Prototype_GameObject_GEAR");

	CItemData* pItemData = CItemData::Create(&Desc);

	m_Inventory.emplace_back(pItemData);

	m_fTotalWeight += Desc.fWeight* Desc.iQuantity;

	return S_OK;
}

HRESULT CInventory::Add_BedRoll()
{
	CItem::ITEM_DESC Desc = {};
	Desc.Durability = 100.f;
	Desc.fRotationPerSec = XMConvertToRadians(120.f);
	Desc.fSpeedPerSec = 2.f;
	Desc.fWeight = 1.5f;
	Desc.iQuantity = 1;
	Desc.ItemInfo = TEXT("�����з� ä�� ħ���̴�.");
	Desc.ItemName = TEXT("BedRoll");
	Desc.ItemType[0] = 4;
	Desc.ItemType[1] = 6;
	Desc.ItemUIName = TEXT("ħ��");
	Desc.ModelTag = TEXT("Prototype_Component_Model_BedRoll");
	Desc.ProtoTypeTag = TEXT("Prototype_GameObject_BedRoll");

	CItemData* pItemData = CItemData::Create(&Desc);

	m_Inventory.emplace_back(pItemData);

	m_fTotalWeight += Desc.fWeight * Desc.iQuantity;

	return S_OK;
}

HRESULT CInventory::Add_Knife()
{
	CItem::ITEM_DESC Desc = {};
	Desc.Durability = 100.f;
	Desc.fRotationPerSec = XMConvertToRadians(120.f);
	Desc.fSpeedPerSec = 2.f;
	Desc.fWeight = 0.50f;
	Desc.iQuantity = 1;
	Desc.ItemInfo = TEXT("���� �����̿� �����η��� ��ƿ ���� �� ���� �������Դϴ�.");
	Desc.ItemName = TEXT("Knife");
	Desc.ItemType[0] = 4;
	Desc.ItemType[1] = 6;
	Desc.ItemUIName = TEXT("��ɿ� ������");
	Desc.ModelTag = TEXT("Prototype_Component_Model_Knife");
	Desc.ProtoTypeTag = TEXT("Prototype_GameObject_GEAR");

	CItemData* pItemData = CItemData::Create(&Desc);

	m_Inventory.emplace_back(pItemData);

	m_fTotalWeight += Desc.fWeight * Desc.iQuantity;

	return S_OK;
}

HRESULT CInventory::Drop_Item(wstring ItemName, void* pDesc)
{
	auto& iterater = m_Inventory.begin();
	for (auto iter : m_Inventory)
	{
		if (iter->Compare_Name(ItemName))
		{
			//������ ��ġ�� �Ÿ� ��������
			//������ ���� ������ ����
			iter->Drop_Item(pDesc);
			if(iter->Using())
			{
				Safe_Release(iter);
				m_Inventory.erase(iterater);
			}
			return S_OK;
		}
		iterater++;
	}
	return S_OK;
}

_bool CInventory::Check_QuestItem(wstring ItemName)
{
	auto& iterater = m_Inventory.begin();
	for (auto iter : m_Inventory)
	{
		if (iter->Compare_Name(ItemName))
		{
			//������ ��ġ�� �Ÿ� ��������
			//������ ���� ������ ����
			if(iter->QuestItemFinish())
			{
				Safe_Release(iter);
				m_Inventory.erase(iterater);
			}
			return true;
		}
		iterater++;
	}
	return false;

}

_bool CInventory::Check_StoveItem(wstring ItemName, void* pDesc)
{
	auto& iterater = m_Inventory.begin();
	for (auto iter : m_Inventory)
	{
		if (iter->Compare_Name(ItemName))
		{
			//������ ��ġ�� �Ÿ� ��������
			//������ ���� ������ ����
			iter->Drop_Item(pDesc);	
			return true;
		}
		iterater++;
	}
	return false;

}


void CInventory::Using_Item(wstring strName)
{
	for (auto iter = m_Inventory.begin(); iter != m_Inventory.end();)
	{
		if ((*iter)->Compare_Name(strName))
		{
			if ((*iter)->Using())
			{
				Safe_Release(*iter);
				iter = m_Inventory.erase(iter);
			}
			return;
		}
		else
			++iter;
	}
}

CItemData* CInventory::find_Item(wstring ItemName)
{
	for (auto& pItem : m_Inventory)
	{
		if (pItem->Compare_Name(ItemName))
			return pItem;
	}


	return nullptr;
}

CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInventory* pInstance = new CInventory(pDevice,pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInventory");
		return nullptr;
	}

	return pInstance;
}

CComponent* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CInventory");
		return nullptr;
	}

	return pInstance;
}

void CInventory::Free()
{
	__super::Free();

	for (auto& pItemData : m_Inventory)
		Safe_Release(pItemData);

	m_Inventory.clear();
}
