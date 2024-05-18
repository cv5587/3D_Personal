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
	m_pBulletMatrix = pDesc->pBulletMatrix;
	m_pBulletsLeft = pDesc->pBulletsLeft;
	m_pBodyAnimFinish = pDesc->pBodyAnimFinish;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;


	return S_OK;
}

void CRevolver::Priority_Tick(_float fTimeDelta)
{

}

void CRevolver::Tick(_float fTimeDelta)
{
	m_iCurrentReloadBullet = m_iTotalBullets - *m_pBulletsLeft;
	
	//0.6



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


	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CRevolver::Render()
{




	if (*m_pState == PLAYERSTATE::PLAYER_RELOAD)
	{
		_uint Index = { 0 };
		if (0 == m_iCurrentReloadBullet)
		{
			Index = m_iCurrentReloadBullet;
		}
		else
		{
			Index = m_iCurrentReloadBullet-1;
		}


		for (_uint i = Index; i < m_iTotalBullets; i++)//ÃÑ ¸öÃ¼
		{
			if (i == (Index))
			{
				//¸®·Îµå ½Ã¿¡
				if (FAILED(Bind_BulletShaderResources()))
					return E_FAIL;

				if (FAILED(Bullet_Render((i + 5) % (6))))
					return E_FAIL;
				if (FAILED(Bullet_Render((i + 5) % (6) + (6))))
					return E_FAIL;
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
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;
		for (_uint i = m_iCurrentReloadBullet; i < m_iTotalBullets; i++)//ÃÑ ¸öÃ¼
		{
			if (FAILED(Normal_Render((i + 5) % (6))))
				return E_FAIL;
			if (FAILED(Normal_Render((i + 5) % (6) + (6))))
				return E_FAIL;

		}
	}
	

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	for (_uint i = 12; i < 19; i++)//ÃÑ ¸öÃ¼
	{
		if (FAILED(Normal_Render(i)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CRevolver::Normal_Render(_uint BulletIndex)
{
	m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", BulletIndex);

	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", BulletIndex, aiTextureType_DIFFUSE)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pModelCom->Render(BulletIndex);
	return S_OK;
}

HRESULT CRevolver::Bullet_Render(_uint BulletIndex)
{
	m_pModelCom->Bind_BulletBoneMatrices(m_pShaderCom, "g_BoneMatrices", BulletIndex);

	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", BulletIndex, aiTextureType_DIFFUSE)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pModelCom->Render(BulletIndex);

	return S_OK;
}

_bool CRevolver::Reload_Time()
{
	return m_pModelCom->Check_AinmTime(1.0);
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
	case Client::PLAYER_RELOAD_S://3.4.5.6.7.8.
	{
		_uint Index = { 0 };
		if (0 <= m_iCurrentReloadBullet - 1)
			Index = m_iCurrentReloadBullet - 1;

		AnimDesc.iAnimIndex = 3 + (Index);
		AnimDesc.isLoop = false;
		break;
	}
	case Client::PLAYER_RELOAD:
		AnimDesc.iAnimIndex = 2 ;
		AnimDesc.isLoop = false;
		break;
	
	case Client::PLAYER_RELOAD_LOOP:
		AnimDesc.iAnimIndex = 2;
		AnimDesc.isLoop = true;
		break;
	case Client::PLAYER_RELOAD_E://9,10,11,12,13,14
		AnimDesc.iAnimIndex = 9;
		AnimDesc.isLoop = false;
		break;
	}
	//2 ·çÇÁ ½Ç¸°´õ µ¹¸®±â
	//3 ºÎÅÍ ¿ÀÇÂ 


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
