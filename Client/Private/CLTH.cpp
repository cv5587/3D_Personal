#include "CLTH.h"	
#include "GameInstance.h"

CCLTH::CCLTH(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem{ pDevice,pContext }
{
}

CCLTH::CCLTH(const CCLTH& rhs)
	:CItem{ rhs }
{
}

HRESULT CCLTH::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CCLTH::Initialize(void* pArg)
{

	CLTHITEM_DESC* pDesc = static_cast<CLTHITEM_DESC*>(pArg);

	if (FAILED(Check_Model(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CCLTH::Priority_Tick(_float fTimeDelta)
{
}

void CCLTH::Tick(_float fTimeDelta)
{
}

void CCLTH::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CCLTH::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


	return S_OK;
}

HRESULT CCLTH::Check_Model(void* pArg)
{
	CCLTH::CLTHITEM_DESC* itemDesc = static_cast<CCLTH::CLTHITEM_DESC*>(pArg);
	wstring modeltag=itemDesc->ModelTag;
	if (modeltag == TEXT("Prototype_Component_Model_ToqueWool"))
	{
		itemDesc->ItemName = TEXT("ToqueWool");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_CLOTH;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 2.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("���� ����");
		m_ItemInfo = TEXT("������ ��Ʈ �� �����Դϴ�. ���и�ŭ ���������� \n������ �ƹ��͵� ���� �� ���� �����ϴ�.");
		m_ItemWeight = TEXT("0.10 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.1f;
	}
	else if (modeltag == TEXT("Prototype_Component_Model_Cargopants"))
	{
		itemDesc->ItemName = TEXT("Cargopants");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_CLOTH;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 2.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("ī�� ����");
		m_ItemInfo = TEXT("�κ��� ������ �������Դϴ�. ��Ģ�� �ɸ�\n�� �����ϴ�.");
		m_ItemWeight = TEXT("0.75 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.75f;
	}
	else if (modeltag == TEXT("Prototype_Component_Model_Boots"))
	{
		itemDesc->ItemName = TEXT("Boots");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_CLOTH;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 2.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("�������� ����");
		m_ItemInfo = TEXT("����� �߶� ��� ��� �Ű� �ִ� �����Դϴ�.");
		m_ItemWeight = TEXT("1.00 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 1.0f;
	}
	else if (modeltag == TEXT("Prototype_Component_Model_Sweater"))
	{
		itemDesc->ItemName = TEXT("Sweater");
		itemDesc->ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_CLOTH;
		itemDesc->ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
		itemDesc->iQuantity = 1;
		itemDesc->fSpeedPerSec = 2.f;
		itemDesc->fRotationPerSec = XMConvertToRadians(120.f);

		m_ItemUIName = TEXT("�������� ������");
		m_ItemInfo = TEXT("�ܵ����� �Ծ ���������� �ܿ� ��Ʈ �ȿ�\n ���̾���ص� ���ƿ�.");
		m_ItemWeight = TEXT("0.25 KG");
		m_ItemDurability = TEXT("100%");

		m_Durability = 100.f;
		m_fWeight = 0.25f;
	}
	return S_OK;
}

HRESULT CCLTH::Add_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	//���� �ε��� ���̴��� �ٸ��� ��� ��� ���� ������.
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshID"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCLTH::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_ID("g_ID", m_iRenderID)))
		return E_FAIL;

	return S_OK;
}

CCLTH* CCLTH::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	CCLTH* pInstance = new CCLTH(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCLTH");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCLTH::Clone(void* pArg)
{
	CCLTH* pInstance = new CCLTH(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CCLTH");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCLTH::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
