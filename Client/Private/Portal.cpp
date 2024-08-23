#include "Portal.h"
#include "GameInstance.h"
CPortal::CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CInteractiveObject{ pDevice,pContext }
{
}

CPortal::CPortal(const CPortal& rhs)
    :CInteractiveObject( rhs )
{
}

HRESULT CPortal::Initialize_Prototype()
{
    
    return S_OK;
}

HRESULT CPortal::Initialize(void* pArg)
{
    CPortal::PORTAL_DESC* pDesc = static_cast<PORTAL_DESC*>(pArg);
    m_ItemUIName = TEXT("문");
    m_iGoalCellIndex = pDesc->iGoalCellIndex;
    m_vGoalPosition = pDesc->vGoalPosition;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

    if (FAILED(Add_Components()))
        return E_FAIL;

    return S_OK;
}

void CPortal::Priority_Tick(_float fTimeDelta)
{
}

void CPortal::Tick(_float fTimeDelta)
{
    m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CPortal::Late_Tick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
    m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

}

HRESULT CPortal::Render()
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

_bool CPortal::IntersectRay(_vector* pRayArray, _float* fDist)
{
    if (m_pColliderCom->IntersectRay(pRayArray, fDist))
    {
        return true;
    }
    
    return false;
}

HRESULT CPortal::Action()
{
    _bool Switch = !(m_pGameInstance->Get_LightDesc(0)->bSwitch);
    m_pGameInstance->Set_LightSwitch(0, Switch);
    m_pGameInstance->Set_LightSwitch(1, !Switch);

    CGameObject* pPlayer = m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY,TEXT("Layer_Player"));
    
    if (FAILED(dynamic_cast<CPlayer*>(pPlayer)->Set_Portal(m_iGoalCellIndex, m_vGoalPosition)))
        return E_FAIL;



    return S_OK;
}

_bool CPortal::RayCollInfo(_vector* pRayArray, CGameObject** pGameObject)
{
    _float fDist = 0.f;
    if (m_pColliderCom->IntersectRay(pRayArray, &fDist))
    {
        if (3 > fDist)
        {
            *pGameObject = this;
            return true;
        }
    }

    return false;
}

HRESULT CPortal::Add_Components()
{
    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ModelTag,
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Collider */
    CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc={};

    ColliderDesc.eType = CCollider::TYPE_AABB;
    ColliderDesc.vExtents = _float3(1.0f, 1.0f, 1.0f);//aabb 조절가능
    ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y * 0.5f, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;


    return S_OK;
}

HRESULT CPortal::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
        return E_FAIL;


    return S_OK;
}

CPortal* CPortal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPortal* pInstance = new CPortal(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CPortal");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPortal::Clone(void* pArg)
{
    CPortal* pInstance = new CPortal(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CPortal");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPortal::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
