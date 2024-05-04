#include "PickUpSelector.h"
#include "GameInstance.h"
#include "Item.h"
#include "UImanager.h"


CPickUpSelector::CPickUpSelector(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pGameInstance{CGameInstance::GetInstance()},
	m_pDevice{ pDevice },
	m_pContext{pContext}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CPickUpSelector::Initialize(void* pArg)
{
	m_pPlayerInventory = static_cast<CInventory*>(pArg);	
	Safe_AddRef(m_pPlayerInventory);
	
	m_pUImanager = CUImanager::Create(m_pDevice,m_pContext);

	return S_OK;
}

HRESULT CPickUpSelector::Pick_up(CGameObject* pPickObject, _bool* Acquire)
{
	m_pAcquire = Acquire;

	m_PickObject = pPickObject;

	CTransform* pTransform = dynamic_cast<CTransform*>(m_PickObject->Get_Transform());

	_matrix ItemMatrix = pTransform->Get_WorldMatrix();

	XMStoreFloat4x4(&m_PickObjectOriginMatrix, ItemMatrix);

	const _float4x4* pCamMatrix = m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW);//ī�޶� ��ġ���

	_vector vCamPos = XMLoadFloat4x4(m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW)).r[3];

	_vector vLook = XMLoadFloat4x4(m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW)).r[2];

	_vector Length	=	XMVector3Normalize(vLook);

	_vector Position = vCamPos + Length*0.3f;

	pTransform->Set_State(CTransform::STATE_POSITION, Position);

	return S_OK;
}
//pickupselector �� �������� ������ �κ����� ������ ������ �����ϴ� Ŭ������.
//��� �����ܰ� 
HRESULT CPickUpSelector::Tick( _float fTimeDelta)
{
	if (nullptr == m_PickObject)
		return E_FAIL;

	if(!m_UIInfoSet)
	{
		m_pUImanager->Item_SetInfo(m_PickObject, m_pPlayerInventory->Get_TotalWeight());
		m_UIInfoSet = true;
	}

	CTransform* pTransform = dynamic_cast<CTransform*>(m_PickObject->Get_Transform());

	_long		MouseMove = { 0 };
	
	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fSensor * MouseMove);
	}
	
	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		pTransform->Turn(pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_fSensor * MouseMove);
	}

	//�������� �ݱ�,������ ���ý� ��� �ൿ���� ¥����.
	if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
	{
		m_pPlayerInventory->Add_Item(dynamic_cast<CItem*>(m_PickObject));
		m_pGameInstance->Delete_CloneObject(LEVEL_GAMEPLAY,m_PickObject);
		*m_pAcquire = false;
		m_UIInfoSet = false;
		return S_OK;
	}

	if (m_pGameInstance->Get_DIMouseState_Once(DIM_RB))
	{
		CTransform* pTransform = dynamic_cast<CTransform*>(m_PickObject->Get_Transform());
		pTransform->Set_State_Matrix(XMLoadFloat4x4(&m_PickObjectOriginMatrix));
		m_PickObject = nullptr;
		*m_pAcquire = false;
		m_UIInfoSet = false;
		return S_OK;
	}

	m_pUImanager->Render_UI(CUImanager::Layer_KEY);
	m_pUImanager->Render_UI(CUImanager::Layer_Base);
	m_pUImanager->Render_TypeUI(CUImanager::Layer_Type);
	m_pUImanager->Render_UI(CUImanager::Layer_Text);

	return S_OK;
}

HRESULT CPickUpSelector::Late_Tick(_float fTimeDelta)
{


	//������ ������ߵ�.����?
	//UI�����ܰ� �� ��������
	return S_OK;
}

HRESULT CPickUpSelector::Render()
{
	return S_OK;
}


CPickUpSelector* CPickUpSelector::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CPickUpSelector* pInstance = new CPickUpSelector(pDevice, pContext);	

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created :CPickUpSelector");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPickUpSelector::Free()
{
	Safe_Release(m_pPlayerInventory);
	Safe_Release(m_pUImanager);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
//�ᱹ ����Ʈ�� �ּҸ� ������ �־�ߵ���;ó�� ���� �Ҷ� �ּҸ� ������ �ͼ� �����ϰ� �������.
