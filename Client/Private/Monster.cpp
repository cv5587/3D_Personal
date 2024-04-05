#include "stdafx.h"
#include "Monster.h"

#include "GameInstance.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{pDevice,pContext}
{
}

CMonster::CMonster(const CMonster& rhs)
    :CGameObject{rhs}
{
}

HRESULT CMonster::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;


    m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(m_AnimationIdx, true));
    m_vecCheckAnim.push_back(351);
    m_vecCheckAnim.push_back(383);
    m_vecCheckAnim.push_back(297);
    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(rand() % 20, 3.f, rand() % 20, 1.f));

    return S_OK;
}

void CMonster::Priority_Tick(_float fTimeDelta)
{
}

void CMonster::Tick(_float fTimeDelta)
{
    if (m_pGameInstance->Get_DIKeyState(DIK_E))
    {
            m_AnimationIdx++;

    if (m_AnimationIdx >= 10)
        m_AnimationIdx = 0;

    m_pModelCom->Set_AnimationIndex(CModel::ANIMATION_DESC(m_AnimationIdx, true));
    }
    m_pModelCom->Play_Animation(fTimeDelta);
}

void CMonster::Late_Tick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster::Render()
{

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;



    _uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);  

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }


    return S_OK;
}

HRESULT CMonster::Add_Components()
{
    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ComponentTag,
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMonster::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_ID("g_ID", m_iRenderID)))
        return E_FAIL;

    return S_OK;
}

CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster* pInstance = new CMonster(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CMonster");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
    CMonster* pInstance = new CMonster(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CMonster");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
