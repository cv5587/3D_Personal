#include "..\Public\Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_pLayers = new map<const wstring, class CLayer*>[iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr != Find_Prototype(strPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{

	/* 내가 복제해야할 우언형을 검색하낟. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);



	CGameObject* pCloneObject = pPrototype->Clone(pArg);
	if (nullptr == pCloneObject)
		return E_FAIL;

	/* 아직 추가할려고하는 레이어가 없었따?!! */
	/* 레이어를 생성해서 추가하믄되겄다. */
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		pLayer->Add_GameObject(pCloneObject);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	/* 내가 추가할려가하ㅡㄴㄴ 레이어가 있었다!!  */
	else
		pLayer->Add_GameObject(pCloneObject);

	return S_OK;
}

HRESULT CObject_Manager::Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, CGameObject** pGameObject, void* pArg)
{
	/* 내가 복제해야할 우언형을 검색하낟. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);



	CGameObject* pCloneObject = pPrototype->Clone(pArg);
	if (nullptr == pCloneObject)
		return E_FAIL;







	/* 아직 추가할려고하는 레이어가 없었따?!! */
	/* 레이어를 생성해서 추가하믄되겄다. */
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		pLayer->Add_GameObject(pCloneObject);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	/* 내가 추가할려가하ㅡㄴㄴ 레이어가 있었다!!  */
	else
		pLayer->Add_GameObject(pCloneObject);


	//주소공유를 위한 얕은 복사 객체
	//더블 포인터로 터레인객체를 가져오면 nullptr 이 아니기 때문에  
	// 터레인에 대해서만 delete시킨다.
	if (nullptr != *pGameObject)
	{
		pLayer->Delete_GameObject(*pGameObject);
		*pGameObject = nullptr;
	}
	*pGameObject = pCloneObject;
	Safe_AddRef(pCloneObject);

	return S_OK;
}



HRESULT CObject_Manager::Delete_CloneObject(_uint iLevelIndex, const wstring& strLayerTag,  CGameObject* pGameObject)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

/*삭제레이어가 없으면 */
	if (nullptr == pLayer)
	{
		return E_FAIL;
	}
	/* 삭제할레이어가 있따.*/
	else
		if (FAILED(pLayer->Delete_GameObject(pGameObject)))
			return E_FAIL;

	return S_OK;
}

CGameObject* CObject_Manager::Find_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pGameObject)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	/*삭제레이어가 없으면 */
	if (nullptr == pLayer)
		return nullptr;
	else
		return pLayer->Find_GameObject(pGameObject);
}

CGameObject* CObject_Manager::Find_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const _int& ID)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	/*삭제레이어가 없으면 */
	if (nullptr == pLayer)
		return nullptr;
	else
		return pLayer->Find_GameObject(ID);
}

void CObject_Manager::Priority_Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Priority_Tick(fTimeDelta);
	}
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Tick(fTimeDelta);
	}
}

void CObject_Manager::Late_Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Late_Tick(fTimeDelta);
		}
	}	
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();
}

vector< const _float4x4*>* CObject_Manager::Get_ObPos(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second->Get_ObPos();
}

CGameObject * CObject_Manager::Find_Prototype(const wstring & strPrototypeTag)
{
	auto	iter = m_Prototypes.find(strPrototypeTag);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;	
}


CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const wstring & strLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CObject_Manager * CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager*		pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed To Created : CObject_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();
}
