#include "Inventory.h"
#include "GameInstance.h"
CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	,m_pGameInstance{CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pGameInstance);
}

CInventory::CInventory(const CInventory& rhs)
	: m_pDevice{ rhs.m_pDevice }
	, m_pContext{ rhs.m_pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);	
	Safe_AddRef(m_pGameInstance);
}

HRESULT CInventory::Initialize()
{
    return S_OK;
}

HRESULT CInventory::Add_Item(CItem* pItem)
{

	auto iter = find_Item(pItem);
	if (nullptr == iter)
	{


		if (nullptr == pItem)
			return E_FAIL;
		m_Inventory.emplace_back(pItem);
	}
	else
	{

	}
	return S_OK;
}


CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInventory* pInstance = new CInventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	for (auto& pItem : m_Inventory)
		Safe_Release(pItem);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

CItem* CInventory::find_Item(CItem* pItem)
{
	if (m_Inventory.empty())
		return nullptr;


}
