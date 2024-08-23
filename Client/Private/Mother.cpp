#include "Mother.h"
#include "GameInstance.h"
#include "Rifle.h"
#include "Player.h"

CMother::CMother(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{pDevice , pContext}
{
}

CMother::CMother(const CMother& rhs)
    :CGameObject{rhs}
{
}

HRESULT CMother::Initialize_Prototype()
{
    
    return S_OK;
}

HRESULT CMother::Initialize(void* pArg)
{
    NPC_DESC* pDesc = static_cast<NPC_DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));


    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Add_PartObjects()))
        return E_FAIL;

    return S_OK;
}

void CMother::Priority_Tick(_float fTimeDelta)
{
}

void CMother::Tick(_float fTimeDelta)
{
    
    m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

    AnimControl();


    m_pModelCom->Play_Animation(fTimeDelta);
}

void CMother::Late_Tick(_float fTimeDelta)
{
    if (true == m_pGameInstance->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f))
    {
        m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

        //#ifdef _DEBUG
        //    m_pGameInstance->Add_DebugComponent(m_pColliderCom);
        //#endif
    }

}

HRESULT CMother::Render()
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

_bool CMother::IntersectRay(_vector* pRayArray, _float* fDist)
{
    if (m_pColliderCom->IntersectRay(pRayArray, fDist))
    {
        return true;
    }

    return false;
}

HRESULT CMother::Action()
{
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
    pPlayer->Set_State(PLAYERSTATE::PLAYER_QUEST);

    return S_OK;
}

HRESULT CMother::Add_Components()
{
    /* For.Com_Collider */
    CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc={}; 

    ColliderDesc.eType = CCollider::TYPE_AABB;
    ColliderDesc.vExtents = _float3(0.3f, 1.f, 0.3f);//aabb 조절가능
    ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;


    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Mother"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMeshMonster"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CMother::Add_PartObjects()
{

    return S_OK;
}

HRESULT CMother::Bind_ShaderResources()
{
 
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CMother::AnimControl()
{
    CModel::ANIMATION_DESC		AnimDesc{ 1, true };

    switch (m_eState)
    {
    case 0 :
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 1;
        break;
    case 1:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 1;
        break;
    case 2:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 2;
        break;
    case 3:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 3;
        break;
    default:
        break;
    }

    m_pModelCom->Set_AnimationIndex(AnimDesc);
}

CMother* CMother::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMother* pInstance = new CMother(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CMother");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMother::Clone(void* pArg)
{
    CMother* pInstance = new CMother(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CMother");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CMother::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);

    Safe_Release(m_pModelCom);
    Safe_Release(m_pColliderCom);
}
