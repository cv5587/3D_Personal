#include "stdafx.h"
#include "..\Public\Body_Player.h"

#include "GameInstance.h"
#include "Player.h"
#include "StateMachine.h"

CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CBody_Player::CBody_Player(const CBody_Player& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CBody_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	//m_pModelCom->Set_FirstAnimationIndex(CModel::ANIMATION_DESC(332, true));

	return S_OK;
}

void CBody_Player::Priority_Tick(_float fTimeDelta)
{
}

void CBody_Player::Tick(_float fTimeDelta)
{

	Set_AnimationState();

	m_pModelCom->Play_Animation(fTimeDelta);

	if(*m_pAnimFinished==false)
		if (m_pModelCom->Get_AnimFinished())
			*m_pAnimFinished = true;

	
	

}

void CBody_Player::Late_Tick(_float fTimeDelta)
{

	(*m_pAnimFinished) = m_pModelCom->Get_AnimFinished();

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBody_Player::Render()
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

void CBody_Player::Set_AnimationState()
{
	CModel::ANIMATION_DESC		AnimDesc{ 0, true };
	switch (*m_pEquip)
	{
	case EQUIP_NONE:
		switch (*m_pState)
		{
		case Client::PLAYER_IDLE:
			AnimDesc.iAnimIndex = 115;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_JOG:
			AnimDesc.iAnimIndex = 374;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_RUN:
			AnimDesc.iAnimIndex = 375;
			AnimDesc.isLoop = true;
			break;
		}
		break;
	case EQUIP_STONE:
		switch (*m_pState)
		{
		case Client::PLAYER_IDLE:
			AnimDesc.iAnimIndex = 332;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_IDLE_EXHAUSTED:
			AnimDesc.iAnimIndex = 330;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_JOG:
			AnimDesc.iAnimIndex = 333;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_RUN:
			AnimDesc.iAnimIndex = 336;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_WALK_S:
			AnimDesc.iAnimIndex = 341;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_WALK_E:
			AnimDesc.iAnimIndex = 344;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_WALK_FB:
			AnimDesc.iAnimIndex = 339;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_SLOW_WALK_FB:
			AnimDesc.iAnimIndex = 340;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_WALK_L:
			AnimDesc.iAnimIndex = 342;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_WALK_R:
			AnimDesc.iAnimIndex = 343;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_WALK_LE:
			AnimDesc.iAnimIndex = 345;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_WALK_RE:
			AnimDesc.iAnimIndex = 346;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_AIM_S:
			AnimDesc.iAnimIndex = 331;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_AIM:
			AnimDesc.iAnimIndex = 328;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_AIM_E:
			AnimDesc.iAnimIndex = 327;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_THROW:
			AnimDesc.iAnimIndex = 337;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_EQUIP:
			AnimDesc.iAnimIndex = 329;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_UNEQUIP:
			AnimDesc.iAnimIndex = 338;
			AnimDesc.isLoop = false;
			break;
		default:
			break;
		}
		break;
	case EQUIP_PIPE:
		break;
	default:
		break;
	}
	
	*m_pAnimFinished = AnimDesc.isLoop;

	m_pModelCom->Set_AnimationIndex(AnimDesc);
}

HRESULT CBody_Player::Add_Components()
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

HRESULT CBody_Player::Bind_ShaderResources()
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


CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Player* pInstance = new CBody_Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBody_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
	CBody_Player* pInstance = new CBody_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBody_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Player::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
