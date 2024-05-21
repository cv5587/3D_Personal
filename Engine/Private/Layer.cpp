#include "..\Public\Layer.h"
#include "GameObject.h"
#include "UIBase.h"
#include "Item.h"
CLayer::CLayer()
{

}

CComponent* CLayer::Get_Component(const wstring& strComponentTag, _uint iIndex)
{

	if (iIndex >= m_GameObjects.size())
		return nullptr;

	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return (*iter)->Get_Component(strComponentTag);
}

HRESULT CLayer::Initialize()
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;
	const _float4x4* pTransMatrix=dynamic_cast<CTransform*>(pGameObject->Get_Transform())->Get_WorldFloat4x4();

	m_ObPosition.push_back(pTransMatrix);
	m_GameObjects.push_back(pGameObject);

	return S_OK;
}


HRESULT CLayer::Delete_GameObject( CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return S_OK;

	list<CGameObject*>::iterator iter = m_GameObjects.begin();
	//auto& iter = m_GameObjects.begin();
	for (iter ; iter != m_GameObjects.end();)
	{
		/*삭제할 옵젝이 있다*/
		if (pGameObject == *iter)
		{
			Safe_Release(pGameObject);
			Safe_Release(*iter);
			iter = m_GameObjects.erase(iter);
			break;
		}
		else
			iter++;
	}
	return S_OK;
}

CGameObject* CLayer::Find_GameObject(CGameObject* pGameObject)
{

	if (nullptr == pGameObject)
		return nullptr;

	list<CGameObject*>::iterator iter = m_GameObjects.begin();
	for (iter; iter == m_GameObjects.end(); iter++)
	{
		/*옵젝 반환*/
		if (pGameObject == *iter)
			return *iter;
	}
	/*옵젝 없으면 실패여*/
	return nullptr;

}

CGameObject* CLayer::Find_GameObject(const _int& ID)
{
	if (-1 == ID)
		return nullptr;

	for (auto& pGameObject : m_GameObjects)
		if (pGameObject->Compare_ID(ID))
			return pGameObject;
	//list<CGameObject*>::iterator iter = m_GameObjects.begin();
	//for (iter; iter == m_GameObjects.end(); iter++)
	//{
	//	if((*iter)->Compare_ID(ID))
	//		return (*iter);
	//}
	/*옵젝 없으면 실패여*/
	return nullptr;
}

CGameObject* CLayer::Find_GameObject(const _uint& index)
{
	if (m_GameObjects.size() < index)
		return nullptr;

	_uint iCheckIndex = { 0 };
	for (auto& pGameObject : m_GameObjects)
	{
		if (iCheckIndex == index)
			return pGameObject;

		iCheckIndex++;
	}

	return nullptr;//이럼 리스트 관리 못한거라 망한거임 
}

void CLayer::Priority_Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Priority_Tick(fTimeDelta);
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Tick(fTimeDelta);
}

void CLayer::Late_Tick(_float fTimeDelta)
{

	for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); )
	{
		if ((*iter)->Get_Live())
		{
			(*iter)->Late_Tick(fTimeDelta);
			++iter;
		}
		else
		{
			Safe_Release(*iter);	
			iter = m_GameObjects.erase(iter);
		}
	}

}

CUIBase* CLayer::Find_UI(const _int& UIID)
{

	for (auto& pGameObject : m_GameObjects)
		if (dynamic_cast<CUIBase*>(pGameObject)->Compare_UIID(UIID))
			return dynamic_cast<CUIBase*>(pGameObject);	

	return nullptr;
}

_bool CLayer::Intersect( CCollider* pTargetCollider)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject->Intersect(pTargetCollider))
			return true;

	return false;
}

CGameObject* CLayer::IntersectRay(_vector* pRayArray, _float* fDist)
{
	for (auto& pGameObject : m_GameObjects)
		if (pGameObject->IntersectRay(pRayArray, fDist))
			return pGameObject;

	return nullptr;
}

void CLayer::Render_UI()
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Choice_Render();
}

void CLayer::Render_PickUI(_uint ObjIndex, _uint IconID)
{
	_uint index = 0;
	for(auto& pGameObject: m_GameObjects)
	{
		if (ObjIndex == index)
		{
			pGameObject->UI_Render(IconID);
		}
		index++;
	}
}


HRESULT CLayer::Save_Data(ofstream* fout)
{


	if (!fout->fail())
	{
		_uint iObjectSize = m_GameObjects.size();
		fout->write((char*)&iObjectSize, sizeof(_uint));

		for (auto& pGameObject : m_GameObjects)
			pGameObject->Save_Data(fout);
	}
	else
		return E_FAIL;

	return S_OK;
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	//for (auto& pPos : m_ObPosition)
	//	Safe_Delete(pPos);

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}