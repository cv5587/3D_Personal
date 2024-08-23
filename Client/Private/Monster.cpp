#include "stdafx.h"
#include "Monster.h"

#include "GameInstance.h"
#include "Player.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CLandObject{pDevice,pContext}
{
}

CMonster::CMonster(const CMonster& rhs)
    :CLandObject{rhs}
{
}

HRESULT CMonster::Initialize_Prototype()
{
    
    return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
    CMonster::MONSTER_DESC* pDesc = (CMonster::MONSTER_DESC*)pArg;

    pDesc->fSpeedPerSec = 5.f;
    pDesc->fRotationPerSec = XMConvertToRadians(120.0f);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_isItem = pDesc->isItem;

    //m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

    //if (TEXT("Prototype_Component_Model_Rabbit") == m_ModelTag)
    //{
    //    m_wUIName = TEXT("Åä³¢");
    //    m_fDetectRange = 3.f;
    //}
    //else if (TEXT("Prototype_Component_Model_Wolf") == m_ModelTag)
    //{
    //    m_wUIName = TEXT("´Á´ë");
    //    m_fDetectRange = 5.f;
    //}

    //m_isItem = pDesc->isItem;




    //if (m_isItem)
    //{
    //    m_bisStunned = true;
    //    m_iState = STATE_CORPSE;
    //    m_pNavigationCom->Set_OnNavigation(m_pTransformCom);
    //}

    return S_OK;
}

void CMonster::Priority_Tick(_float fTimeDelta)
{

}

void CMonster::Tick(_float fTimeDelta)
{
  
}

void CMonster::Late_Tick(_float fTimeDelta)
{

}

HRESULT CMonster::Render()
{
    return S_OK;
}

_bool CMonster::IntersectRay(_vector* pRayArray, _float* fDist)
{
    if (m_pColliderCom->IntersectRay(pRayArray, fDist))
           return true;
    

    return false;
}



HRESULT CMonster::Add_Components()
{
   
    return S_OK;
}

HRESULT CMonster::Bind_ShaderResources()
{
   


    return S_OK;
}

_bool CMonster::Detected()
{
    return true;

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

    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
