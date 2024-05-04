#include "GEARStone.h"

#include "GameInstance.h"
CGEARStone::CGEARStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem{pDevice,pContext}
{
}

CGEARStone::CGEARStone(const CGEARStone& rhs)
	:CItem{rhs}
{
}

HRESULT CGEARStone::Initialize_Prototype()
{
	m_ItemUIName = TEXT("작은 돌");
	m_ItemInfo = TEXT("작은 동물을 겁주기에 충분할 정도로 크다. \n 어쩌면 좀 더 큰 동물에게 겁을 줄 수도 있을\n 것 같다.");
	m_ItemWeight = TEXT("0.15 KG");
	m_ItemDurability = TEXT("100%");

	m_fWeight = 0.15f;

	return S_OK;
}

HRESULT CGEARStone::Initialize(void* pArg)
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

void CGEARStone::Priority_Tick(_float fTimeDelta)
{
}

void CGEARStone::Tick(_float fTimeDelta)
{
}

void CGEARStone::Late_Tick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CGEARStone::Render()
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

HRESULT CGEARStone::Add_Components()
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

HRESULT CGEARStone::Bind_ShaderResources()
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

CGEARStone* CGEARStone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGEARStone* pInstance = new CGEARStone(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : GEARItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGEARStone::Clone(void* pArg)
{
	CGEARStone* pInstance = new CGEARStone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : GEARItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGEARStone::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
