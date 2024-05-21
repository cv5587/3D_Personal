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
	SELECTOR_DESC* pDesc = static_cast<SELECTOR_DESC*>(pArg);

	m_pPlayerInventory = static_cast<CInventory*>(pDesc->PlayerInventory);
	Safe_AddRef(m_pPlayerInventory);

	m_pUImanager = static_cast<CUImanager*>(pDesc->UImanager);
	Safe_AddRef(m_pUImanager);

	return S_OK;
}

HRESULT CPickUpSelector::Pick_up(CGameObject* pPickObject, _bool* Acquire)
{
	m_pAcquire = Acquire;

	m_PickObject = pPickObject;


	//��� ������ ���� ������
	CItem* pItem = static_cast<CItem*>(pPickObject);

	//ui ������ type�� ������� �Ѱ��� �Ѱ��� �߰� 2���� 2��

	vector<_uint> vecType = pItem->Get_ItemType();

	CTransform* pTransform = dynamic_cast<CTransform*>(m_PickObject->Get_Transform());

	_matrix ItemMatrix = pTransform->Get_WorldMatrix();

	XMStoreFloat4x4(&m_PickObjectOriginMatrix, ItemMatrix);

	const _float4x4* pCamMatrix = m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW);//ī�޶� ��ġ���

	_vector vCamPos = XMLoadFloat4x4(m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW)).r[3];

	_vector vLook = XMLoadFloat4x4(m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW)).r[2];

	_vector Length	=	XMVector3Normalize(vLook);

	_vector Position{};
	

	if(CItem::ITEM_CLOTH==vecType[0] || CItem::ITEM_CLOTH == vecType[1])
	{
		Position = vCamPos + Length * 0.3f;
		pTransform->Rotaion_Reset();
		pTransform->Set_State_Matrix(XMMatrixIdentity()* XMMatrixScaling(0.4f, 0.4f, 0.4f)*
			XMMatrixTranslation(XMVectorGetX(Position), XMVectorGetY(Position), XMVectorGetZ(Position)));
		

	}
	else 
	{
		Position = vCamPos + Length * 0.3f;
		pTransform->Set_State(CTransform::STATE_POSITION, Position);
	}



	return S_OK;
}
//���Ŀ� �������� ƽ ��θ��°� ������������ �׸������� ���� �������� �׸��� ����.(3d�� �����ϴ�)
HRESULT CPickUpSelector::Tick( _float fTimeDelta)
{
	if (nullptr == m_PickObject)
		return E_FAIL;

	CTransform* pTransform = dynamic_cast<CTransform*>(m_PickObject->Get_Transform());

	if(!m_UIInfoSet)
	{
		m_pUImanager->Item_SetInfo(m_PickObject, m_pPlayerInventory->Get_TotalWeight());
		m_UIInfoSet = true;
		pTransform->Rotaion_Reset();
	}

	

	_long		MouseMove = { 0 };
	
	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fSensor * MouseMove);
	}
	
	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		pTransform->Turn(pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_fSensor * -MouseMove);
	}

	m_pUImanager->Render_UI(CUImanager::Layer_KEY);
	m_pUImanager->Render_UI(CUImanager::Layer_Base);
	m_pUImanager->Render_TypeUI(CUImanager::Layer_Type);
	m_pUImanager->Render_UI(CUImanager::Layer_Text);

	return S_OK;
}

HRESULT CPickUpSelector::Late_Tick(_float fTimeDelta)
{

	return S_OK;
}

HRESULT CPickUpSelector::Render()
{
	return S_OK;
}

void CPickUpSelector::Add_Item()
{
	m_pPlayerInventory->Add_Item(dynamic_cast<CItem*>(m_PickObject));
	//������ �ֿ�� ui ���� �ϴ� �������� ���� (�̸� �����ؼ� �ٲ��� ����.)
	m_pUImanager->Add_InvenIcon(m_pPlayerInventory->Get_LastIndex(),dynamic_cast<CItem*>(m_PickObject));
	
	m_pGameInstance->Delete_CloneObject(LEVEL_GAMEPLAY, m_PickObject);
	//���⼭ ������ ui ���� �ϰ� �ϳ��� ���̾� ��Ƶд�. 
	//�κ��丮������ �׻� ��Ƶ� ���̾ ����ϰ� �����. �����ϸ� ����â�� �ϳ��� ȭ������ ���� ����.


	m_PickObject = nullptr;

	*m_pAcquire = false;

	m_UIInfoSet = false;
}

void CPickUpSelector::Drop_Item()
{
	CTransform* pTransform = dynamic_cast<CTransform*>(m_PickObject->Get_Transform());

	pTransform->Set_State_Matrix(XMLoadFloat4x4(&m_PickObjectOriginMatrix));

	m_PickObject = nullptr;

	*m_pAcquire = false;

	m_UIInfoSet = false;
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
	Safe_Release(m_PickObject);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
//�ᱹ ����Ʈ�� �ּҸ� ������ �־�ߵ���;ó�� ���� �Ҷ� �ּҸ� ������ �ͼ� �����ϰ� �������.
