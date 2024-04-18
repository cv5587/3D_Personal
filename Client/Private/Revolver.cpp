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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pModelCom->Set_FirstAnimationIndex(CModel::ANIMATION_DESC(166, true));



	return S_OK;
}

void CRevolver::Priority_Tick(_float fTimeDelta)
{
}

void CRevolver::Tick(_float fTimeDelta)
{
	CModel::ANIMATION_DESC		AnimDesc{ 0, true };

	//if (*m_pState & CPlayer::iState[CPlayer::STATE_IDLE])
	//{
	//	AnimDesc.isLoop = true;
	//	AnimDesc.iAnimIndex = 166;
	//}
	//else if (*m_pState & CPlayer::iState[CPlayer::STATE_WALK])
	//{
	//	AnimDesc.isLoop = true;
	//	AnimDesc.iAnimIndex = 165;
	//}
	//TODO::040920 애니메이션 번호 체크해서 돌려보기 ->이후 보간 ㄱ
	m_pModelCom->Set_AnimationIndex(AnimDesc);

	m_pModelCom->Play_Animation(fTimeDelta);

	//m_pModelCom->Get_AnimFinished();
}

void CRevolver::Late_Tick(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

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

HRESULT CRevolver::Add_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
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
