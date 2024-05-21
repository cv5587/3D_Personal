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


	//모든 정보를 가진 아이템
	CItem* pItem = static_cast<CItem*>(pPickObject);

	//ui 아이콘 type을 담아주자 한개면 한개만 뜨고 2개면 2개

	vector<_uint> vecType = pItem->Get_ItemType();

	CTransform* pTransform = dynamic_cast<CTransform*>(m_PickObject->Get_Transform());

	_matrix ItemMatrix = pTransform->Get_WorldMatrix();

	XMStoreFloat4x4(&m_PickObjectOriginMatrix, ItemMatrix);

	const _float4x4* pCamMatrix = m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW);//카메라 위치행렬

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
//추후에 아이템을 틱 드로리는걸 원근투형으로 그리지말고 직교 투영으로 그리게 하자.(3d도 가능하다)
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
	//아이템 주우면 ui 생성 하는 형식으로 가자 (미리 생성해서 바꾸지 말고.)
	m_pUImanager->Add_InvenIcon(m_pPlayerInventory->Get_LastIndex(),dynamic_cast<CItem*>(m_PickObject));
	
	m_pGameInstance->Delete_CloneObject(LEVEL_GAMEPLAY, m_PickObject);
	//여기서 아이템 ui 생성 하고 하나의 레이어 모아둔다. 
	//인벤토리에서는 항상 모아둔 레이어만 출력하게 만든다. 선택하면 설명창은 하나의 화면으로 돌려 쓰자.


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
//결국 리스트는 주소를 가지고 있어야도미;처음 제작 할때 주소를 가지고 와서 보관하고 사용하자.
