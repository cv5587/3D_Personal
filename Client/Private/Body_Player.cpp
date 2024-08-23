#include "stdafx.h"
#include "..\Public\Body_Player.h"

#include "GameInstance.h"
#include "Player.h"
#include "StateMachine.h"
#include "Particle_Rect.h"

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
	BODY_DESC* pDesc = (BODY_DESC*)pArg;
	m_pBulletsLeft = pDesc->pBulletsLeft;
	m_pLit = pDesc->pLit;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;


	

	return S_OK;
}

void CBody_Player::Priority_Tick(_float fTimeDelta)
{
}

void CBody_Player::Tick(_float fTimeDelta)
{
	m_fCurrentTime += fTimeDelta;
	Set_AnimationState();

	m_pModelCom->Play_Animation(fTimeDelta);

	if(*m_pAnimFinished==false)
		if (m_pModelCom->Get_AnimFinished())
			*m_pAnimFinished = true;

	if (m_fCurrentTime > 3.f)
	{
		m_fCurrentTime = 0.f;
		Add_Particle();
	}
	
	

}

void CBody_Player::Late_Tick(_float fTimeDelta)
{

	//(*m_pAnimFinished) = m_pModelCom->Get_AnimFinished();

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBody_Player::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	/*
		0 WeightedCube
		1 HMM_FP_Hand_Bandage
		2 Male_Arms
		3 Will_Sleeves
		4 Will_Hands
	*/
	switch (*m_pCloth)
	{
	case Client::CLOTH_NONE:
		Mesh_Render(2);
		break;
	case Client::CLOTH_SLEEVES:
		Mesh_Render(3);
		Mesh_Render(4);
		break;
	case Client::CLOTH_BANDAGE:
		Mesh_Render(1);
		Mesh_Render(3);
		Mesh_Render(4);
		break;
	case Client::CLOTH_END:
		break;
	default:
		break;
	}
	/*
	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}*/


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
		case Client::PLAYER_INVENTORY:
		case Client::PLAYER_PICKUP:
		case Client::PLAYER_ENTER:
		case Client::PLAYER_QUEST:
		case Client::PLAYER_HARVEST:
		case Client::PLAYER_BURN:
		case Client::PLAYER_BUILD:
		case Client::PLAYER_SLEEP:
		case Client::PLAYER_FADEOUT:
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
		case Client::PLAYER_STRUGGLE:	//intro
			AnimDesc.iAnimIndex = 354;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_STRUGGLEBARE:
			AnimDesc.iAnimIndex = 350;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_STRUGGLEKNIFE:
			AnimDesc.iAnimIndex = 355;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_GETUP:
			AnimDesc.iAnimIndex = 89;
			AnimDesc.isLoop = false;
			break;
		}
		break;
	case EQUIP_STONE:
		switch (*m_pState)
		{
		case Client::PLAYER_IDLE:
		case Client::PLAYER_INVENTORY:
		case Client::PLAYER_PICKUP:
		case Client::PLAYER_ENTER:
		case Client::PLAYER_QUEST:
		case Client::PLAYER_BURN:
		case Client::PLAYER_HARVEST:
		case Client::PLAYER_BUILD:
		case Client::PLAYER_SLEEP:
		case Client::PLAYER_FADEOUT:
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
		case Client::PLAYER_STRUGGLE:	//intro
			AnimDesc.iAnimIndex = 354;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_STRUGGLEBARE:
			AnimDesc.iAnimIndex = 350;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_STRUGGLEKNIFE:
			AnimDesc.iAnimIndex = 355;
			AnimDesc.isLoop = false;
			break;
		default:
			break;
		}
		break;
	case EQUIP_REVOLVER:
		switch (*m_pState)
		{
		case Client::PLAYER_IDLE:
		case Client::PLAYER_INVENTORY:
		case Client::PLAYER_PICKUP:
		case Client::PLAYER_ENTER:
		case Client::PLAYER_QUEST:
		case Client::PLAYER_BURN:
		case Client::PLAYER_HARVEST:
		case Client::PLAYER_BUILD:
		case Client::PLAYER_SLEEP:
		case Client::PLAYER_FADEOUT:
			AnimDesc.iAnimIndex = 302;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_IDLE_EXHAUSTED:
			AnimDesc.iAnimIndex = 302;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_JOG:
			AnimDesc.iAnimIndex = 303;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_RUN:
			AnimDesc.iAnimIndex = 307;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_EQUIP:
			AnimDesc.iAnimIndex = 298;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_UNEQUIP:
			AnimDesc.iAnimIndex = 309;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_AIM_S:
			AnimDesc.iAnimIndex = 301;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_AIM:
			AnimDesc.iAnimIndex = 296;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_AIM_E:
			AnimDesc.iAnimIndex = 297;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_WALK_L:
			AnimDesc.iAnimIndex = 310;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_WALK_R:
			AnimDesc.iAnimIndex = 311;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_FIRE:
			AnimDesc.iAnimIndex = 299;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_HIPFIRE:
			AnimDesc.iAnimIndex = 300;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_RELOAD_S:
			AnimDesc.iAnimIndex = 306;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_RELOAD://총알 갯수 받아서 횟수만큼 진행 예정
			AnimDesc.iAnimIndex = 305;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_RELOAD_E:
			AnimDesc.iAnimIndex = 304;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_STRUGGLE:	//intro
			AnimDesc.iAnimIndex = 354;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_STRUGGLEBARE:
			AnimDesc.iAnimIndex = 350;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_STRUGGLEKNIFE:
			AnimDesc.iAnimIndex = 355;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_END:
			break;
		default:
			break;
		}
		break;

	case EQUIP_RABBIT:	
		switch (*m_pState)
		{
		case Client::PLAYER_IDLE:
			AnimDesc.iAnimIndex = 86;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_EQUIP:
			AnimDesc.iAnimIndex = 85;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_KILL:
			AnimDesc.iAnimIndex = 87;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_RELEASE:	
			AnimDesc.iAnimIndex = 88;
			AnimDesc.isLoop = false;
			break;
		}
		break;

	case EQUIP_MATCH:
		switch (*m_pState)
		{
			case Client::PLAYER_IDLE:
			{
				if (true == (*m_pLit))
				{
					AnimDesc.iAnimIndex = 223;
				}
				else
				{
					AnimDesc.iAnimIndex = 239;
				}
				AnimDesc.isLoop = true;
				break;
			}
			case Client::PLAYER_EQUIP:
				AnimDesc.iAnimIndex = 217;
				AnimDesc.isLoop = false;
				break;
			case Client::PLAYER_BURNOUT:
				AnimDesc.iAnimIndex = 216;
				AnimDesc.isLoop = false;
				break;
			case Client::PLAYER_UNEQUIP:
				AnimDesc.iAnimIndex = 228;
				AnimDesc.isLoop = false;
				break;
			case Client::PLAYER_JOG:
			{
				if (true == (*m_pLit))
				{
					AnimDesc.iAnimIndex = 230;
				}
				else
				{
					AnimDesc.iAnimIndex = 224;
				}
				AnimDesc.isLoop = true;
				break;
			}
			case Client::PLAYER_MATCHPRE:
				AnimDesc.iAnimIndex = 220;
				AnimDesc.isLoop = false;
				break;
			case Client::PLAYER_MATSUCCESS:
				AnimDesc.iAnimIndex = 221;
				AnimDesc.isLoop = false;
				break;
			case Client::PLAYER_STRUGGLE:	//intro
				AnimDesc.iAnimIndex = 354;
				AnimDesc.isLoop = false;
				break;
			case Client::PLAYER_STRUGGLEBARE:
				AnimDesc.iAnimIndex = 350;
				AnimDesc.isLoop = false;
				break;
			case Client::PLAYER_STRUGGLEKNIFE:
				AnimDesc.iAnimIndex = 355;
				AnimDesc.isLoop = false;
				break;
		}
		break;
	case EQUIP_KNIFE:
	{
		switch (*m_pState)
		{

		case Client::PLAYER_FADEOUT:
			AnimDesc.iAnimIndex = 115;
			AnimDesc.isLoop = true;
			break;
		case Client::PLAYER_STRUGGLE:	//intro
			AnimDesc.iAnimIndex = 354;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_STRUGGLEBARE:
			AnimDesc.iAnimIndex = 350;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_STRUGGLEKNIFE:
			AnimDesc.iAnimIndex = 355;
			AnimDesc.isLoop = false;
			break;
		}
	}
		break;

	case EQUIP_FLARE:
		switch (*m_pState)
		{
		case Client::PLAYER_IDLE:
		case Client::PLAYER_INVENTORY:
		case Client::PLAYER_PICKUP:
		case Client::PLAYER_ENTER:
		case Client::PLAYER_QUEST:
		case Client::PLAYER_BURN:
		case Client::PLAYER_HARVEST:
		case Client::PLAYER_BUILD:
		case Client::PLAYER_SLEEP:
		case Client::PLAYER_FADEOUT:
		{
			AnimDesc.iAnimIndex = 144;
			AnimDesc.isLoop = true;
			break;
		}
		case Client::PLAYER_EQUIP:
			AnimDesc.iAnimIndex = 161;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_UNEQUIP:
			AnimDesc.iAnimIndex = 142;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_JOG:
		{
			AnimDesc.iAnimIndex = 145;
			AnimDesc.isLoop = true;
			break;
		}
		case Client::PLAYER_FLAREPRE:
			AnimDesc.iAnimIndex = 123;
			AnimDesc.isLoop = false;
			break;
		case Client::PLAYER_FLARESUCCESS:
			AnimDesc.iAnimIndex = 124;
			AnimDesc.isLoop = false;
			break;
		}
		break;
	case EQUIP_PIPE:
		break;
	default:
		break;
	}

	//if(*m_pState== PLAYER_RELOAD && !* m_pAnimFinished && *m_pBulletsLeft!=6)
	//{
	//	//*m_pAnimFinished = AnimDesc.isLoop;
	//	m_pModelCom->Set_RepeatAnimationIndex(AnimDesc);
	//}
	//else
	//{

		*m_pAnimFinished = AnimDesc.isLoop;
		if(354 == AnimDesc.iAnimIndex || 350 == AnimDesc.iAnimIndex || 355 == AnimDesc.iAnimIndex)
			m_pModelCom->Set_NoShiftAnimationIndex(AnimDesc);//이 애니메이션 끝난뒤에 보간애님 진행 안됨. 바로 시전.
		else
			m_pModelCom->Set_AnimationIndex(AnimDesc);

		
	
}

HRESULT CBody_Player::Mesh_Render(_uint MeshIndex)
{
	m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", MeshIndex);

	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", MeshIndex, aiTextureType_DIFFUSE)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pModelCom->Render(MeshIndex);

	return S_OK;
}

void CBody_Player::Reset_Anim()
{
	m_pModelCom->Set_RepeatAnimationIndex();
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

HRESULT CBody_Player::Add_Particle()
{

	CParticle_Rect::PARTICLE_DESC RectDesc{};

	//_vector Pos = XMLoadFloat4x4(m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW)).r[3];
	//_vector Look = XMLoadFloat4x4(m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW)).r[2];
	//_vector Length = XMVector3Normalize(Look);
	//_vector Position =Pos + Length * 0.3f;

	//XMStoreFloat4(&RectDesc.vStartPos, Position);
	//RectDesc.BufferInstance.iNumInstance = 3;
	//RectDesc.BufferInstance.vOffsetPos = _float3(0.0f, -10.f, 0.0f);
	//RectDesc.BufferInstance.vPivotPos = _float3(0.0f, -2.0f, 0.0f);
	//RectDesc.BufferInstance.vRange = _float3(0.1f, 0.1f, 0.1f);
	//RectDesc.BufferInstance.vSize = _float2(0.5f, 0.8f);
	//RectDesc.BufferInstance.vSpeed = _float2(0.5f, 1.2f);
	//RectDesc.BufferInstance.vLifeTime = _float2(1.f, 3.f);
	//RectDesc.BufferInstance.fScaleSize = 2.0f;
	//RectDesc.BufferInstance.isLoop = false;
	//RectDesc.iTYPE = CParticle_Rect::TYPE_BREATH;
	//if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Breath"), TEXT("Prototype_GameObject_Particle_Rect"), &RectDesc)))
	//	return E_FAIL;
	_float m_fX = g_iWinSizeX >> 1;
	_float m_fY = g_iWinSizeY >> 1;

	XMStoreFloat4(&RectDesc.vStartPos, XMVectorSet(m_fX * 0.f, m_fY * -1.f, 0.f, 1.f));
	RectDesc.BufferInstance.iNumInstance = 3;
	RectDesc.BufferInstance.vOffsetPos = _float3(0.0f, -500.f, 0.0f);
	RectDesc.BufferInstance.vPivotPos = _float3(0.0f, 0.0f, 0.0f);
	RectDesc.BufferInstance.vRange = _float3(500.f, 50.f, 0.f);
	RectDesc.BufferInstance.vSize = _float2(1024.f, 1024.f);
	RectDesc.BufferInstance.vSpeed = _float2(100.f, 150.f);
	RectDesc.BufferInstance.vLifeTime = _float2(1.f, 2.f);
	RectDesc.BufferInstance.fScaleSize = 100.0f;
	RectDesc.BufferInstance.isLoop = false;
	RectDesc.iTYPE = CParticle_Rect::TYPE_BREATH;
//	if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Breath"), TEXT("Prototype_GameObject_Particle_Rect"), &RectDesc)))
//		return E_FAIL;

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
	//if (FAILED(m_pShaderCom->Bind_ID("g_ID", m_iRenderID)))
	//	return E_FAIL;

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
