#include "Meat.h"

#include "GameInstance.h"
CMeat::CMeat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CMeat::CMeat(const CMeat& rhs)
    : CPartObject{ rhs }
{
}

HRESULT CMeat::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMeat::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    //강제 대입
    XMStoreFloat4x4(m_pSocketMatrix, XMMatrixIdentity() * XMMatrixTranslation(0.f, 1.f, 0.f));
    return S_OK;
}

void CMeat::Priority_Tick(_float fTimeDelta)
{
}

void CMeat::Tick(_float fTimeDelta)
{
}

void CMeat::Late_Tick(_float fTimeDelta)
{
    _matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
    SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
    SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

    XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));


    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMeat::Render()
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

HRESULT CMeat::Add_Components()
{
    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ModelTag,
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshID"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMeat::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
        return E_FAIL;


    return S_OK;
}

CMeat* CMeat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMeat* pInstance = new CMeat(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CMeat");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMeat::Clone(void* pArg)
{
    CMeat* pInstance = new CMeat(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CMeat");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMeat::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
