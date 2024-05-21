#include "..\Public\Object_Manager.h"
#include "GameObject.h"
#include "UIBase.h"
#include "Layer.h"

CObject_Manager::CObject_Manager()
{
}

CComponent* CObject_Manager::Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(strComponentTag, iIndex);
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
//몬스터는 일로 생성하지 말것
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

	pCloneObject->Set_ID(m_ObjectID++);
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

	pCloneObject->Set_ID(m_ObjectID++);
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



HRESULT CObject_Manager::Delete_CloneObject(_uint iLevelIndex,  CGameObject* pGameObject)
{
	for (auto& pLayer : m_pLayers[iLevelIndex])
	{
		pLayer.second->Delete_GameObject(pGameObject);
	}

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

CGameObject* CObject_Manager::FindID_CloneObject(_uint iLevelIndex, const _int& ID)
{
	for(auto& pLayer : m_pLayers[iLevelIndex])
	{
		CGameObject* pGameObject= pLayer.second->Find_GameObject(ID);
		if (nullptr != pGameObject)
			return pGameObject;
	}
	return nullptr;
}

CGameObject* CObject_Manager::FindIndex_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, _uint iIndex)
{

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;
	else
		return pLayer->Find_GameObject(iIndex);
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

CGameObject* CObject_Manager::Clone_Object(const wstring& strPrototypeTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject* pCloneObject = pPrototype->Clone(pArg);
	if (nullptr == pCloneObject)
		return nullptr;
	pCloneObject->Set_ID(m_ObjectID++);

	return pCloneObject;
}

_bool CObject_Manager::Intersect(_uint iLevelIndex, const wstring& strLayerTag, CCollider* pTargetCollider)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;
	else
		return pLayer->Intersect(pTargetCollider);
}

CGameObject* CObject_Manager::IntersectRay(_uint iLevelIndex, const wstring& strLayerTag, _vector* pRayArray, _float* fDist)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;
	else
		return pLayer->IntersectRay(pRayArray, fDist);
}

CUIBase* CObject_Manager::FindUIID_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, _int UIID)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);	

	if (nullptr == pLayer)
		return nullptr;
	else
		return pLayer->Find_UI(UIID);
}

void CObject_Manager::Render_UI(_uint iLevelIndex, wstring LayerName)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, LayerName);

	pLayer->Render_UI();
}

HRESULT CObject_Manager::Save_Level(_uint iLevelIndex)
{
	char FileRoute[MAX_PATH] = "../Bin/bin/Save_Data/";
	char FilePath[MAX_PATH] = "";

	switch (iLevelIndex)
	{
		//클라 에서 이중작업해줘야됨
	case 0 :
		strcpy_s(FilePath, "LEVEL_STATIC_GAMEDATA");
		break;
	case 1:
		strcpy_s(FilePath, "LEVEL_LOADING_GAMEDATA");
		break;
	case 2:
		strcpy_s(FilePath, "LEVEL_LOGO_GAMEDATA");
		break;
	case 3:
		strcpy_s(FilePath, "LEVEL_GAMEPLAY_GAMEDATA");
		break;
	default:
		return E_FAIL;
	}

	strcat_s(FileRoute, FilePath);

	_tchar Layer[MAX_PATH] = TEXT("");



	ofstream fout;
	fout.open(FileRoute, ios::out | ios::binary);

	if (!fout.fail())
	{
		fout.write((char*)&iLevelIndex,  sizeof(_uint));

		//레벨에 레이어 갯수를 내보냄
		_uint Layersize = m_pLayers[iLevelIndex].size();

		fout.write((char*)&Layersize, sizeof(_uint));

		for (auto& pLayer : m_pLayers[iLevelIndex])
		{
			ZeroMemory(Layer, sizeof(_tchar) * MAX_PATH);
			wsprintf(Layer, pLayer.first.c_str());
			wstring wLayer(Layer);

			//카메라랑 플레이어도 가져 가게 조절
			if (TEXT("Layer_Camera") != wLayer )
			{
				fout.write((char*)Layer, sizeof(_tchar) * MAX_PATH);
				if (FAILED(pLayer.second->Save_Data(&fout)))	
					return E_FAIL;
			}
		}
	}

	fout.close();

	return S_OK;
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
