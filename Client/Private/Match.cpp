#include "Match.h"
#include "GameInstance.h"
#include "Player.h"
#include "Explosion.h"
CMatch::CMatch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CMatch::CMatch(const CMatch& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CMatch::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMatch::Initialize(void* pArg)
{

	MATCH_DESC* pDesc = (MATCH_DESC*)pArg;

	m_pSocketMatrix = pDesc->pCombinedTransformationMatrix;
	m_pLit = pDesc->pLit;
	m_pBurnTime = pDesc->pBurnTime;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	LIGHT_DESC			LightDesc = {};
	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(20.f, 5.f, 20.f, 1.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(1.f, 0.3f, 0.f, 0.3f);
	LightDesc.vAmbient = _float4(0.4f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.bSwitch = false;
	m_pGameInstance->Add_Light(LightDesc);
	//m_pGameInstance->Set_LightSwitch(2, false);


	CExplosion::EXPLOSION_DESC ExplosionDesc{};

	ExplosionDesc.pParentsMatrix = &m_WorldMatrix;
	XMStoreFloat4x4(&ExplosionDesc.fControlMatrix, XMMatrixScaling(0.08f, 0.08f, 0.08f)*XMMatrixTranslation(0.007f, 0.04f, 0.0f));
	m_pEffect = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Explosion"),&ExplosionDesc);

	return S_OK;
}

void CMatch::Priority_Tick(_float fTimeDelta)
{
}

void CMatch::Tick(_float fTimeDelta)
{
	if (true == *m_pLit)
	{

		(*m_pBurnTime) += fTimeDelta;
		m_pEffect->Tick(fTimeDelta);
		
	}
	else
	{
		dynamic_cast<CExplosion*>(m_pEffect)->Reset();
	}

	Set_AnimationState();
	m_pModelCom->Play_Animation(fTimeDelta);

	//if (*m_pAnimFinished == false)
	//	if (m_pModelCom->Get_AnimFinished())
	//		*m_pAnimFinished = true;
}

void CMatch::Late_Tick(_float fTimeDelta)
{


	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

	_vector vLightPos = XMLoadFloat4x4(&m_WorldMatrix).r[3];

	m_pGameInstance->Set_LightPosition(2, vLightPos);

	if (true == *m_pLit)
	{
		//CTransform* pTransform = dynamic_cast<CTransform*>(m_pEffect->Get_Transform());
		//_float Y =XMVectorGetY(vLightPos) + 3.f;
		//XMVectorSetY(vLightPos, Y);
		//pTransform->Set_State(CTransform::STATE_POSITION, vLightPos);
		m_pEffect->Late_Tick(fTimeDelta);
	}
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMatch::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;



	//_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	//for (_uint i = 0; i < iNumMeshes; i++)
	//{
	//if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
	//		return E_FAIL;

	//	m_pShaderCom->Begin(0);

	//	m_pModelCom->Render(i);
	//}
	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", 0, aiTextureType_DIFFUSE)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pModelCom->Render(0);

	return S_OK;
}

void CMatch::Set_AnimationState()
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

	}

	m_pModelCom->Set_AnimationIndex(AnimDesc);

}

HRESULT CMatch::Add_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMatch::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CMatch* CMatch::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMatch* pInstance = new CMatch(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMatch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMatch::Clone(void* pArg)
{
	CMatch* pInstance = new CMatch(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMatch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMatch::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pEffect);
}
