#include "stdafx.h"
#include "PickRabbit.h"

#include "GameInstance.h"
#include "Player.h"

CPickRabbit::CPickRabbit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CPickRabbit::CPickRabbit(const CPickRabbit& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CPickRabbit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPickRabbit::Initialize(void* pArg)
{
	PICKRABBIT_DESC* pDesc = static_cast<PICKRABBIT_DESC*>(pArg);

	m_pSocketMatrix = pDesc->pCombinedTransformationMatrix;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pModelCom->Set_FirstAnimationIndex(CModel::ANIMATION_DESC(0, true));



	return S_OK;
}

void CPickRabbit::Priority_Tick(_float fTimeDelta)
{
}

void CPickRabbit::Tick(_float fTimeDelta)
{

	Set_AnimationState();

	m_pModelCom->Play_Animation(fTimeDelta);

}

void CPickRabbit::Late_Tick(_float fTimeDelta)
{
	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPickRabbit::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;



	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


	return S_OK;
}

void CPickRabbit::Set_AnimationState()
{
	CModel::ANIMATION_DESC		AnimDesc{ 0, true };

	switch (*m_pState)
	{
	case Client::PLAYER_IDLE:
		AnimDesc.iAnimIndex = 1;
		AnimDesc.isLoop = true;
		break;
	case Client::PLAYER_EQUIP:
		AnimDesc.iAnimIndex = 0;
		AnimDesc.isLoop = false;
		break;
	case Client::PLAYER_KILL:
		AnimDesc.iAnimIndex = 2;
		AnimDesc.isLoop = false;
		break;
	case Client::PLAYER_RELEASE:
		AnimDesc.iAnimIndex = 3;
		AnimDesc.isLoop = false;
		break;
	}

	m_pModelCom->Set_AnimationIndex(AnimDesc);
}

HRESULT CPickRabbit::Add_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PickRabbit"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPickRabbit::Bind_ShaderResources()
{
	//if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_ID("g_ID", m_iRenderID)))
		return E_FAIL;

	return S_OK;
}

CPickRabbit* CPickRabbit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPickRabbit* pInstance = new CPickRabbit(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPickRabbit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPickRabbit::Clone(void* pArg)
{
	CPickRabbit* pInstance = new CPickRabbit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPickRabbit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPickRabbit::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
