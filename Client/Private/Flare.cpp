#include "Flare.h"
#include "GameInstance.h"
#include "Player.h"
#include "Particle_Rect.h"
CFlare::CFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CFlare::CFlare(const CFlare& rhs)
    : CPartObject{ rhs }
{
}

HRESULT CFlare::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFlare::Initialize(void* pArg)
{

	FLARE_DESC* pDesc = static_cast<FLARE_DESC*>(pArg);

	m_pSocketMatrix = pDesc->pCombinedTransformationMatrix;
	m_pLit = pDesc->pLit;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;


	//4¹ø
	LIGHT_DESC			LightDesc = {};
	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(20.f, 5.f, 20.f, 1.f);
	LightDesc.fRange = 20.f;
	LightDesc.vDiffuse = _float4(1.0f, 0.f, 0.f, 1.0f);
	LightDesc.vAmbient = _float4(0.4f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.bSwitch = false;
	m_pGameInstance->Add_Light(LightDesc);
	//m_pGameInstance->Set_LightSwitch(2, false);


	CParticle_Rect::PARTICLE_DESC RectDesc{};
	XMStoreFloat4(&RectDesc.vStartPos, XMVectorSet(0.f, 0.2f, 0.f, 1.f));
	RectDesc.BufferInstance.iNumInstance = 300;
	RectDesc.BufferInstance.vOffsetPos = _float3(10.0f, -10.f, 0.0f);
	RectDesc.BufferInstance.vPivotPos = _float3(0.0f, 0.0f, 0.0f);
	RectDesc.BufferInstance.vRange = _float3(0.0f, 0.f, 0.0f);
	RectDesc.BufferInstance.vSize = _float2(0.01f, 0.03f);
	RectDesc.BufferInstance.vSpeed = _float2(0.0f, 0.01f);
	RectDesc.BufferInstance.vLifeTime = _float2(1.f, 2.0f);
	RectDesc.BufferInstance.fScaleSize = 1.f;
	RectDesc.BufferInstance.isLoop = true;
	RectDesc.iTYPE = CParticle_Rect::TYPE_FLARE;
	RectDesc.pParentMatrix = &m_WorldMatrix;
	RectDesc.BufferInstance.vStartPos = RectDesc.vStartPos;
	RectDesc.BufferInstance.pParentMatrix = &m_WorldMatrix;
	m_pEffect = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Particle_Rect"), &RectDesc);

	//ExplosionDesc.pParentsMatrix = &m_WorldMatrix;
	//XMStoreFloat4x4(&ExplosionDesc.fControlMatrix, XMMatrixScaling(0.08f, 0.08f, 0.08f) * XMMatrixTranslation(0.007f, 0.04f, 0.0f));

	return S_OK;
}

void CFlare::Priority_Tick(_float fTimeDelta)
{
}

void CFlare::Tick(_float fTimeDelta)
{

	if (*m_pLit)
	{
		m_fBurnTime += fTimeDelta;
		Add_Particle();
		m_pEffect->Tick(fTimeDelta);
		m_pGameInstance->Play_Sound(TEXT("FlareLoop.wav"), FLARE, 0.8f);
	}

	if (15.f < m_fBurnTime)
	{
		*m_pLit = false;
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
		pPlayer->Set_State(PLAYER_UNEQUIP);
		m_pGameInstance->Set_LightSwitch(4, false);
		m_pGameInstance->StopSound(FLARE);
	}

}

void CFlare::Late_Tick(_float fTimeDelta)
{
	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
	_vector vLightPos = XMLoadFloat4x4(&m_WorldMatrix).r[3];
	m_pGameInstance->Set_LightPosition(4, vLightPos);
	if (*m_pLit)
	{
		m_pEffect->Late_Tick(fTimeDelta);
	}
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CFlare::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;



	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CFlare::Set_AnimationState()
{
	CModel::ANIMATION_DESC		AnimDesc{ 0, true };

	switch (*m_pState)
	{
	case Client::PLAYER_IDLE:
		AnimDesc.iAnimIndex = 0;
		AnimDesc.isLoop = true;
		break;
	}

	m_pModelCom->Set_AnimationIndex(AnimDesc);
}

HRESULT CFlare::Add_Components()
{

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Flare"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFlare::Bind_ShaderResources()
{
	//if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
		return E_FAIL;


	return S_OK;
}

void CFlare::Add_Particle()
{

	CParticle_Rect::PARTICLE_DESC RectDesc{};
	XMStoreFloat4(&RectDesc.vStartPos, XMVectorSet(0.f, 0.2f, -0.37f, 1.f));
	RectDesc.BufferInstance.iNumInstance = 1;
	RectDesc.BufferInstance.vOffsetPos = _float3(0.0f, -10.f, 0.0f);
	RectDesc.BufferInstance.vPivotPos = _float3(0.0f, 0.0f, 0.0f);
	RectDesc.BufferInstance.vRange = _float3(0.1f, 0.1f, 0.1f);
	RectDesc.BufferInstance.vSize = _float2(0.8f, 1.3f);
	RectDesc.BufferInstance.vSpeed = _float2(0.1f, 0.2f);
	RectDesc.BufferInstance.vLifeTime = _float2(1.5f, 2.f);
	RectDesc.BufferInstance.fScaleSize = 1.f;
	RectDesc.BufferInstance.isLoop = false;
	RectDesc.iTYPE = CParticle_Rect::TYPE_FLARETRAIL;
	RectDesc.pParentMatrix = &m_WorldMatrix;

	m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Paticle"), TEXT("Prototype_GameObject_Particle_Rect"), &RectDesc);



}

CFlare* CFlare::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	CFlare* pInstance = new CFlare(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CFlare");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFlare::Clone(void* pArg)
{
	CFlare* pInstance = new CFlare(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CFlare");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlare::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
