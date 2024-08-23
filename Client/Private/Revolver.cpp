#include "stdafx.h"
#include "..\Public\Revolver.h"

#include "GameInstance.h"
#include "Player.h"
#include "RevolverFire.h"
#include "Particle_Rect.h"

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
	m_pBulletMatrix = pDesc->pBulletMatrix;
	m_pBulletsLeft = pDesc->pBulletsLeft;
	m_pBodyAnimFinish = pDesc->pBodyAnimFinish;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;


	CRevolverFire::REVOLVERFIRE_DESC RevolverFireDesc{};

	RevolverFireDesc.pParentsMatrix = &m_WorldMatrix;
	RevolverFireDesc.pShot = &m_bShot;
	XMStoreFloat4x4(&RevolverFireDesc.fControlMatrix, XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(0.00f, 0.0f, -1.0f));
	m_pEffect = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_RevolverFire"), &RevolverFireDesc);




	return S_OK;
}

void CRevolver::Priority_Tick(_float fTimeDelta)
{

}

void CRevolver::Tick(_float fTimeDelta)
{
	

	if (*m_pBodyAnimFinish)
	{
		m_iCurrentReloadBullet = m_iTotalBullets - *m_pBulletsLeft;
	}
		
	//if (*m_pState == PLAYERSTATE::PLAYER_FIRE || *m_pState == PLAYERSTATE::PLAYER_HIPFIRE)
	//	if(false == m_bShot)
	//		m_bShot = true;

	if(m_bShot)
		m_pEffect->Tick(fTimeDelta);
	//0.6

	if (m_bSmog)
	{
		Add_Particle();
		m_fSmogTime += fTimeDelta;
	}

	if (m_fSmogTime > 3.f)
	{
		m_fSmogTime = 0.f;
		m_bSmog = false;
	}

	//m_pParticle->Tick(fTimeDelta);

	if(*m_pState==PLAYERSTATE::PLAYER_RELOAD)
		m_ReloadTime += fTimeDelta;

	Set_AnimationState();

	m_pModelCom->Play_Animation(fTimeDelta);

	if (*m_pAnimFinished == false)
		if (m_pModelCom->Get_AnimFinished())
			*m_pAnimFinished = true;

}

void CRevolver::Late_Tick(_float fTimeDelta)
{
	//(*m_pBodyAnimFinish) = m_pModelCom->Get_AnimFinished();

	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));


	_matrix		BulletMatrix = XMLoadFloat4x4(m_pBulletMatrix);

	BulletMatrix.r[0] = XMVector3Normalize(BulletMatrix.r[0]);
	BulletMatrix.r[1] = XMVector3Normalize(BulletMatrix.r[1]);
	BulletMatrix.r[2] = XMVector3Normalize(BulletMatrix.r[2]);

	XMStoreFloat4x4(&m_BulletWorldMatrix, m_pTransformCom->Get_WorldMatrix() * BulletMatrix * XMLoadFloat4x4(m_pParentMatrix));

	if (m_bShot)
		m_pEffect->Late_Tick(fTimeDelta);

	//m_pParticle->Late_Tick(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CRevolver::Render()
{

	_uint Index = { 0 };
	if (0 == m_iCurrentReloadBullet)
	{
		Index = m_iCurrentReloadBullet;
	}
	else
	{
		Index = m_iCurrentReloadBullet - 1;
	}



	if (*m_pState == PLAYERSTATE::PLAYER_RELOAD)
	{
		
		if (*m_pBodyAnimFinish)
		{
			m_ReloadTime = 0.f;
			m_iReloadBullet++;
		}

		for (_uint i = 0; i < m_iTotalBullets; i++)
		{
			if (i == (0))
			{
				if (m_ReloadTime > 0.6f)
				{
					if (FAILED(Bind_ShaderResources()))
						return E_FAIL;

					if (FAILED(Normal_Render((i + 5) % (6))))
						return E_FAIL;
					if (FAILED(Normal_Render((i + 5) % (6) + (6))))
						return E_FAIL;
				}
				else
				{
					//리로드 시에
					if (FAILED(Bind_BulletShaderResources()))
						return E_FAIL;

					if (FAILED(Bullet_Render((i + 5) % (6))))
						return E_FAIL;
					if (FAILED(Bullet_Render((i + 5) % (6) + (6))))
						return E_FAIL;
				}

				
			}
			else if (0 <i&&i<= Index)
			{

			}
			else
			{
				if (FAILED(Bind_ShaderResources()))
					return E_FAIL;

				if (FAILED(Normal_Render((i + 5) % (6))))
					return E_FAIL;
				if (FAILED(Normal_Render((i + 5) % (6) + (6))))
					return E_FAIL;
			}

		}

	}
	else
	{

		m_iReloadBullet = 0;
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;
		for (_uint i = 0; i < m_iTotalBullets; i++)
		{
			if ( i < m_iCurrentReloadBullet)
			{
				int a = 11;
			}
			else
			{

				if (FAILED(Normal_Render((i + 5) % (6))))
					return E_FAIL;
				if (FAILED(Normal_Render((i + 5) % (6) + (6))))
					return E_FAIL;
			}

		}




	}
	

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	for (_uint i = 12; i < 19; i++)//총 몸체
	{
		if (FAILED(Normal_Render(i)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CRevolver::Normal_Render(_uint BulletIndex)
{
	m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", BulletIndex);

	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", BulletIndex, aiTextureType_DIFFUSE)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pModelCom->Render(BulletIndex);
	return S_OK;
}

HRESULT CRevolver::Bullet_Render(_uint BulletIndex)
{
	m_pModelCom->Bind_BulletBoneMatrices(m_pShaderCom, "g_BoneMatrices", BulletIndex);

	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", BulletIndex, aiTextureType_DIFFUSE)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pModelCom->Render(BulletIndex);

	return S_OK;
}

_bool CRevolver::Reload_Time()
{
	_bool is = m_pModelCom->Check_AinmTime(0.015);
	return m_pModelCom->Check_AinmTime(0.015);
}

void CRevolver::Set_AnimationState()
{
	CModel::ANIMATION_DESC		AnimDesc{ 0, true };
	_uint Index = { 0 };
	if (0 <= m_iCurrentReloadBullet - 1)
		Index = m_iCurrentReloadBullet - 1;

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
	case Client::PLAYER_RELOAD_S://3.4.5.6.7.8.
	{

		AnimDesc.iAnimIndex = 3;
		AnimDesc.isLoop = false;
		break;
	}
	case Client::PLAYER_RELOAD:
		AnimDesc.iAnimIndex = 2 ;
		AnimDesc.isLoop = false;
		break;
	
	case Client::PLAYER_RELOAD_E://9,10,11,12,13,14
		AnimDesc.iAnimIndex = 9;
		AnimDesc.isLoop = false;
		break;
	}
	//2 루프 실린더 돌리기
	//3 부터 오픈 


	//if (*m_pState == PLAYER_RELOAD && !*m_pBodyAnimFinish && *m_pBulletsLeft != 6)
	//{
	//	//*m_pAnimFinished = AnimDesc.isLoop;
	//	m_pModelCom->Set_RepeatAnimationIndex(AnimDesc);
	//}
	//else
	//{


		*m_pAnimFinished = AnimDesc.isLoop;
		m_pModelCom->Set_AnimationIndex(AnimDesc);


}

void CRevolver::Reset_Anim()
{
	m_pModelCom->Set_RepeatAnimationIndex();	

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

HRESULT CRevolver::Bind_BulletShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_BulletWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_ID("g_ID", m_iRenderID)))
		return E_FAIL;

	return S_OK;
}

void CRevolver::Add_Particle()
{
	CParticle_Rect::PARTICLE_DESC RectDesc{};
	XMStoreFloat4(&RectDesc.vStartPos, XMVectorSet(0.f, 0.1f, -0.37f, 1.f));
	RectDesc.BufferInstance.iNumInstance = 1;
	RectDesc.BufferInstance.vOffsetPos = _float3(0.0f, -10.f, 0.0f);
	RectDesc.BufferInstance.vPivotPos = _float3(0.0f, 0.0f, 0.0f);
	RectDesc.BufferInstance.vRange = _float3(0.f, 0.f, 0.f);
	RectDesc.BufferInstance.vSize = _float2(0.05f, 0.08f);
	RectDesc.BufferInstance.vSpeed = _float2(0.1f, 0.2f);
	RectDesc.BufferInstance.vLifeTime = _float2(0.5f, 1.f);
	RectDesc.BufferInstance.fScaleSize = 1.f;
	RectDesc.BufferInstance.isLoop = false;
	RectDesc.iTYPE = CParticle_Rect::TYPE_TRAIL;
	RectDesc.pParentMatrix = &m_WorldMatrix;

	m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Paticle"), TEXT("Prototype_GameObject_Particle_Rect"), &RectDesc);


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

	Safe_Release(m_pEffect);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
