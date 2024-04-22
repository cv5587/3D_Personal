#include "stdafx.h"
#include "..\Public\Revolver.h"

#include "GameInstance.h"
#include "Player.h"

CRevolver::CRevolver(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CRevolver::CRevolver(const CRevolver& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CRevolver::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRevolver::Initialize(void* pArg)
{

	REVOLVER_DESC* pDesc = (REVOLVER_DESC*)pArg;

	m_pSocketMatrix = pDesc->pCombinedTransformationMatrix;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (*m_pAnimFinished == false)
		if (m_pModelCom->Get_AnimFinished())
			*m_pAnimFinished = true;


	return S_OK;
}

void CRevolver::Priority_Tick(_float fTimeDelta)
{

}

void CRevolver::Tick(_float fTimeDelta)
{
	Set_AnimationState();

	m_pModelCom->Play_Animation(fTimeDelta);


}

void CRevolver::Late_Tick(_float fTimeDelta)
{
	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CRevolver::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;



	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


	return S_OK;
}

void CRevolver::Set_AnimationState()
{
	CModel::ANIMATION_DESC		AnimDesc{ 0, true };
	
	switch (*m_pState)
	{
	case Client::PLAYER_IDLE:
		AnimDesc.iAnimIndex = 0;
		AnimDesc.isLoop = true;
		break;		
	case Client::PLAYER_FIRE:
		AnimDesc.iAnimIndex = 1;
		AnimDesc.isLoop = false;
		break;
	case Client::PLAYER_HIPFIRE:
		AnimDesc.iAnimIndex = 1;
		AnimDesc.isLoop = false;
		break;			
	case Client::PLAYER_RELOAD_S:
		AnimDesc.iAnimIndex = 3;
		AnimDesc.isLoop = false;
		break;
	case Client::PLAYER_RELOAD://2.4.5.6.7.8.
		AnimDesc.iAnimIndex = 2;
		AnimDesc.isLoop = false;
		break;
	case Client::PLAYER_RELOAD_E://9,10,11,12,13,14
		AnimDesc.iAnimIndex = 9;
		AnimDesc.isLoop = false;
		break;
	}

	m_pModelCom->Set_AnimationIndex(AnimDesc);

}

HRESULT CRevolver::Add_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Revolver"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CRevolver::Bind_ShaderResources()
{
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

CRevolver* CRevolver::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRevolver* pInstance = new CRevolver(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CRevolver");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRevolver::Clone(void* pArg)
{
	CRevolver* pInstance = new CRevolver(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CRevolver");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRevolver::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
