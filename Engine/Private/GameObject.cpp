#include "..\Public\GameObject.h"
#include "GameInstance.h"

const _tchar* CGameObject::m_pTransformTag = TEXT("Com_Transfrom");

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pDevice{ rhs.m_pDevice }
	, m_pContext{ rhs.m_pContext }
	, m_pGameInstance{ rhs.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)//겜오브젝트 전체가 들고있을 데이터 
	{
		GAMEOBJECT_DESC* pDesc = (GAMEOBJECT_DESC*)pArg;
		m_ComponentTag = pDesc->ComponentTag;
		m_iRenderID = pDesc->ObjectID;
		_float4 fPickPoint = pDesc->vPrePosition;
		_vector vPosition = XMLoadFloat4(&fPickPoint);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}

	m_Components.emplace(m_pTransformTag, m_pTransformCom);
	Safe_AddRef(m_pTransformCom);	
	return S_OK;
}

void CGameObject::Priority_Tick(_float fTimeDelta)
{
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::Late_Tick(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CComponent* CGameObject::Get_Transform()
{
	auto& iter = m_Components.find(m_pTransformTag);
	return iter->second;
}

CComponent* CGameObject::Get_Component(const wstring& strComponentTag)
{
	auto& iter = m_Components.find(strComponentTag);
	return iter->second;
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	if (m_Components.end() != m_Components.find(strComponentTag))
		return E_FAIL;

	CComponent* pComponent = m_pGameInstance->Clone_Component(iPrototypeLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

void CGameObject::Free()
{
	for (auto& iter : m_Components)
		Safe_Release(iter.second);
	m_Components.clear();
	
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
