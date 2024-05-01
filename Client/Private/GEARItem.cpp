#include "GEARItem.h"

#include "GameInstance.h"
CGEARItem::CGEARItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem{pDevice,pContext}
{
}

CGEARItem::CGEARItem(const CGEARItem& rhs)
	:CItem{rhs}
{
}

HRESULT CGEARItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGEARItem::Initialize(void* pArg)
{

	GAMEOBJECT_DESC* pDesc = (GAMEOBJECT_DESC*)pArg;

	//이동과 회전은 밖에서 넘겨주게 만들어주자
	pDesc->fSpeedPerSec = 2.f;
	pDesc->fRotationPerSec = XMConvertToRadians(120.0f);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CGEARItem::Priority_Tick(_float fTimeDelta)
{
}

void CGEARItem::Tick(_float fTimeDelta)
{
}

void CGEARItem::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CGEARItem::Render()
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

HRESULT CGEARItem::Add_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	//만약 부들이 쉐이더가 다르면 모두 양면 으로 해주자.
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshID"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGEARItem::Bind_ShaderResources()
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

CGEARItem* CGEARItem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGEARItem* pInstance = new CGEARItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : GEARItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGEARItem::Clone(void* pArg)
{
	CGEARItem* pInstance = new CGEARItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : GEARItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGEARItem::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
