#include "stdafx.h"
#include "..\Public\Particle_Rect.h"

#include "GameInstance.h"

CParticle_Rect::CParticle_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBlendObject(pDevice, pContext)
{
}

CParticle_Rect::CParticle_Rect(const CParticle_Rect& rhs)
	: CBlendObject(rhs)
{
}

HRESULT CParticle_Rect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Rect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	PARTICLE_DESC* pDesc = static_cast<PARTICLE_DESC*>(pArg);

	m_pBufferInstance = &pDesc->BufferInstance;
	m_iType = pDesc->iTYPE;
	m_pParentMatrix = pDesc->pParentMatrix;
	m_vStartPos = pDesc->vStartPos;
	if (FAILED(Add_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		switch (m_iType)
		{
		case Client::CParticle_Rect::TYPE_SNOW:
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&((PARTICLE_DESC*)pArg)->vStartPos));
			break;
		case Client::CParticle_Rect::TYPE_BREATH:
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&((PARTICLE_DESC*)pArg)->vStartPos));
		}
			break;
		case Client::CParticle_Rect::TYPE_FIRE:
			break;
		case Client::CParticle_Rect::TYPE_TRAIL:
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat4(&m_vStartPos), XMLoadFloat4x4(m_pParentMatrix)));
		}
			break;
		case Client::CParticle_Rect::TYPE_FLARETRAIL:
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat4(&m_vStartPos), XMLoadFloat4x4(m_pParentMatrix)));
		}
		break;
		case Client::CParticle_Rect::TYPE_FOOT:
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat4(&m_vStartPos), XMLoadFloat4x4(m_pParentMatrix)));
			m_pTransformCom->LookUp();
		}
		break;
		case Client::CParticle_Rect::TYPE_FLARE:
		{
			//m_pTransformCom->Set_State_Matrix(XMMatrixRotationZ(XMConvertToRadians(-45.f)));

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat4(&m_vStartPos), XMLoadFloat4x4(m_pParentMatrix)));
		}
		break;
		case Client::CParticle_Rect::TYPE_END:
			break;
		default:
			break;
		}
	}

	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	return S_OK;
}

void CParticle_Rect::Priority_Tick(_float fTimeDelta)
{

}

void CParticle_Rect::Tick(_float fTimeDelta)
{

	if(m_isLived)
	{
		switch (m_iType)
		{
		case Client::CParticle_Rect::TYPE_SNOW:
			m_pVIBufferCom->Drop(fTimeDelta);
			break;
		case Client::CParticle_Rect::TYPE_BREATH:
		{
			m_pVIBufferCom->Spread(fTimeDelta);
			m_pVIBufferCom->Scaling(fTimeDelta);
		}
			break;
		case Client::CParticle_Rect::TYPE_FIRE:
			break;
		case Client::CParticle_Rect::TYPE_TRAIL:
		{
			
			m_pVIBufferCom->Spread(fTimeDelta);
			m_pTransformCom->LookBack(m_pGameInstance->Get_CamPosition());
		}
			break;
		case Client::CParticle_Rect::TYPE_FLARETRAIL:
		{
			m_pVIBufferCom->Spread(fTimeDelta);
			m_pTransformCom->LookBack(m_pGameInstance->Get_CamPosition());
		}
			break;
		case Client::CParticle_Rect::TYPE_FOOT:
		{
		//	m_pTransformCom->LookUp();
			//m_pTransformCom->Rotation(XMVectorSet(1.f , 0.f , 0.f ,0.f),XMConvertToRadians(90.f));
		}
		break;
		case Client::CParticle_Rect::TYPE_FLARE:
		{
		//	m_pVIBufferCom->Up(fTimeDelta);
		//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat4(&m_vStartPos), XMLoadFloat4x4(m_pParentMatrix)));
		////	m_pVIBufferCom->Scaling(fTimeDelta*-1.f);
		//	m_pTransformCom->LookBack(m_pGameInstance->Get_CamPosition());

		}
		break;
		case Client::CParticle_Rect::TYPE_END:
			break;
		default:
			break;
		}
	}


}

void CParticle_Rect::Late_Tick(_float fTimeDelta)
{
	Compute_ViewZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));


	switch (m_iType)
	{
	case Client::CParticle_Rect::TYPE_SNOW:
		
		break;
	case Client::CParticle_Rect::TYPE_BREATH:
		if (!m_pVIBufferCom->Get_Live())
			m_isLived = false;
		break;
	case Client::CParticle_Rect::TYPE_FIRE:
		break;
	case Client::CParticle_Rect::TYPE_END:
		break;
	case Client::CParticle_Rect::TYPE_TRAIL:
		if (!m_pVIBufferCom->Get_Live())
			m_isLived = false;
		break;
	case Client::CParticle_Rect::TYPE_FLARETRAIL:
		if (!m_pVIBufferCom->Get_Live())
			m_isLived = false;
		break;
	case Client::CParticle_Rect::TYPE_FOOT:
		if (!m_pVIBufferCom->Get_Live())
			m_isLived = false;
		break;
	case Client::CParticle_Rect::TYPE_FLARE:
	{
		m_pVIBufferCom->Up(fTimeDelta);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat4(&m_vStartPos), XMLoadFloat4x4(m_pParentMatrix)));
		m_pTransformCom->LookBack(m_pGameInstance->Get_CamPosition());

	}
		break;
	default:
		break;
	}

	
	//if (!m_pVIBufferCom->Get_Live())
	//	m_isLived = false;
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_BLEND, this);
}

HRESULT CParticle_Rect::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	switch (m_iType)
	{
	case Client::CParticle_Rect::TYPE_SNOW:
		m_pShaderCom->Begin(1);
		break;
	case Client::CParticle_Rect::TYPE_BREATH:
		m_pShaderCom->Begin(1);
		break;
	case Client::CParticle_Rect::TYPE_FIRE:
		m_pShaderCom->Begin(1);
		break;
	case Client::CParticle_Rect::TYPE_END:
		break;
	case Client::CParticle_Rect::TYPE_TRAIL:
		m_pShaderCom->Begin(1);
		break;
	case Client::CParticle_Rect::TYPE_FLARETRAIL:
		m_pShaderCom->Begin(1);
		break;
	case Client::CParticle_Rect::TYPE_FOOT:
		m_pShaderCom->Begin(3);
		break;
	case Client::CParticle_Rect::TYPE_FLARE:
		m_pShaderCom->Begin(2);
		break;
	default:
		break;
	}


	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CParticle_Rect::Add_Components()
{
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Instance_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), m_pBufferInstance)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxInstance_Rect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;



	switch (m_iType)
	{
	case Client::CParticle_Rect::TYPE_SNOW:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Dust"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_BREATH:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Breath"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_FIRE:
		break;
	case Client::CParticle_Rect::TYPE_TRAIL:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Fog"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_FLARETRAIL:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FlareSmoke"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_FLARE:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Ver"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_FOOT:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FootStep"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CParticle_Rect::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	switch (m_iType)
	{
	case Client::CParticle_Rect::TYPE_SNOW:
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_BREATH:
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_FIRE:
		break;
	case Client::CParticle_Rect::TYPE_TRAIL:
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_FLARE:
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_FOOT:
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
			return E_FAIL;
		break;

	case Client::CParticle_Rect::TYPE_FLARETRAIL:
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
			return E_FAIL;
		break;
	case Client::CParticle_Rect::TYPE_END:
		break;
	default:
		break;
	}

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;



	return S_OK;
}

CParticle_Rect* CParticle_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Rect* pInstance = new CParticle_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CParticle_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Rect::Clone(void* pArg)
{
	CParticle_Rect* pInstance = new CParticle_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CParticle_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Rect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);	

}
