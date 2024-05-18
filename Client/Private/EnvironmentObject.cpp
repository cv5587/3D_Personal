#include "EnvironmentObject.h"

#include "GameInstance.h"

CEnvironmentObject::CEnvironmentObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{pDevice,pContext}
{
}

CEnvironmentObject::CEnvironmentObject(const CGameObject& rhs)
    :CGameObject{rhs}
{
}

HRESULT CEnvironmentObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEnvironmentObject::Initialize(void* pArg)
{
    GAMEOBJECT_DESC* pDesc = (GAMEOBJECT_DESC*)pArg;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

    if (FAILED(Add_Components()))
        return E_FAIL;

    return S_OK;
}

void CEnvironmentObject::Priority_Tick(_float fTimeDelta)
{
}

void CEnvironmentObject::Tick(_float fTimeDelta)
{

}

void CEnvironmentObject::Late_Tick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CEnvironmentObject::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;



    _uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }


    return S_OK;
}

HRESULT CEnvironmentObject::Add_Components()
{
    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ModelTag,
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;



    /* For.Com_Shader *///나무 검사
    wstring CompareName = TEXT("Pin");
    if (m_ModelTag.substr(26, 3) == CompareName)
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshTree"),
            TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
            return E_FAIL;
    }
    else
    {
        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
            TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CEnvironmentObject::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
        return E_FAIL;


    return S_OK;
}

CEnvironmentObject* CEnvironmentObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEnvironmentObject* pInstance = new CEnvironmentObject(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CEnvironmentObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CEnvironmentObject::Clone(void* pArg)
{
    CEnvironmentObject* pInstance = new CEnvironmentObject(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CEnvironmentObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEnvironmentObject::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
