#include "stdafx.h"
#include "Monster.h"

#include "GameInstance.h"
#include "Player.h"
const _uint CMonster::iState[CMonster::STATE_END] = {
    0x00000001, /* 0001 */
    0x00000002, /* 0010 */
    0x00000004, /* 0100 */
    0x00000008, /* 1000 */
    0x00000010, /* 0001 0000 */
    0x00000020, /* 0010 0000 */
    0x00000040, /* 0100 0000 */
    0x00000080, /* 1000 0000 */
};

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
    CMonster::MOSTER_DESC* pDesc = (CMonster::MOSTER_DESC*)pArg;

    pDesc->fSpeedPerSec = 5.f;
    pDesc->fRotationPerSec = XMConvertToRadians(120.0f);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;


    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

    if (TEXT("Prototype_Component_Model_Rabbit") == m_ModelTag)
    {
        m_wUIName = TEXT("토끼");
        m_fDetectRange = 3.f;
    }

    m_isItem = pDesc->isItem;


    if (FAILED(Add_Components()))
        return E_FAIL;

    if (m_isItem)
    {
        m_bisStunned = true;
        m_iState = STATE_CORPSE;
        m_pNavigationCom->Set_OnNavigation(m_pTransformCom);
    }

    return S_OK;
}

void CMonster::Priority_Tick(_float fTimeDelta)
{
    m_pPlayerMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_WorldFloat4x4();

}

void CMonster::Tick(_float fTimeDelta)
{

   m_fCurrentTime += fTimeDelta;


   m_Detected = Detected();



   if (!m_Detected)
   {
       switch (m_iState)
       {

       case Client::CMonster::STATE_WALK:
       {
           m_pTransformCom->Set_DeltaValue(2.f, XMConvertToRadians(120.0f));
           m_pTransformCom->Patrol(fTimeDelta, m_pNavigationCom);
           if (m_fCurrentTime > m_fPatternTime)
           {
               Reset_Timer();
           }
       }
       break;

       case Client::CMonster::STATE_RUN:
       {
           m_pTransformCom->Set_DeltaValue(5.f, XMConvertToRadians(120.0f));
           m_pTransformCom->Patrol(fTimeDelta, m_pNavigationCom);
           if (m_fCurrentTime > m_fPatternTime)
           {
               Reset_Timer();
           }
       }
       break;

       case Client::CMonster::STATE_GROOM:
       {
           if (m_pModelCom->Get_AnimFinished())
           {
               Reset_Timer();
           }
       }
       break;
       case Client::CMonster::STATE_IDLELEFT:
       {
           if (m_pModelCom->Get_AnimFinished())
           {
               Reset_Timer();
           }
       }
       break;
       case Client::CMonster::STATE_IDLERIGHT:
       {
           if (m_pModelCom->Get_AnimFinished())
           {
               Reset_Timer();
           }
       }
       break;
       case Client::CMonster::STATE_IDLE:
       {
           if (m_fCurrentTime > m_fPatternTime)
           {
               Reset_Timer();
           }
       }
       break;

       case Client::CMonster::STATE_DEATH:
       {
           if (m_pModelCom->Get_AnimFinished())
           {
               m_iState = STATE_CORPSE;
           }
       }
       break;

       case Client::CMonster::STATE_CORPSE:
       {
           m_bisStunned = true;
       }
       break;
       default:
           break;
       }
   }
   else
   {
       switch (m_iState)
       {

       case Client::CMonster::STATE_RUN:
       {
           if (!m_bEscapeRotate)
           {
               m_pTransformCom->Escape(XMLoadFloat4x4(m_pPlayerMatrix), fTimeDelta, m_pNavigationCom);
               m_bEscapeRotate = true;
           }

           m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
       }
       break;

       case Client::CMonster::STATE_GROOM:
       {
           m_iState = STATE_RUN;

       }
       break;
       case Client::CMonster::STATE_IDLELEFT:
       {
           m_iState = STATE_RUN;
       }
       break;
       case Client::CMonster::STATE_IDLERIGHT:
       {
           m_iState = STATE_RUN;
       }
       break;
       case Client::CMonster::STATE_IDLE:
       {
           m_iState = STATE_RUN;
       }
       break;
       case Client::CMonster::STATE_DEATH:
       {
           if (m_pModelCom->Get_AnimFinished())
           {
               m_iState = STATE_CORPSE;
           }
       }
       break;

       case Client::CMonster::STATE_CORPSE:
       {
           m_bisStunned = true;
       }
       break;
       default:
           break;
       }
   }


    

    
    m_pNavigationCom->Set_OnNavigation(m_pTransformCom);

    m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

    AnimControl();


    m_pModelCom->Play_Animation(fTimeDelta);
}

void CMonster::Late_Tick(_float fTimeDelta)
{
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
    if (nullptr == pPlayer)
        return;


   if(!m_bisStunned)
   {
        //자기 콜라이더 들고 아이템 레이어 에 들어가기 아이템 레이어 안에
        if (m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_Item"), m_pColliderCom))
        {
            m_iState = STATE_DEATH;
        }
        //플레이어 레이와 충돌(언제? 플레이어 상태가 fire, hipfire일때.)

    }
   
   /*
   else
   {
       _vector vMouseRay[2] = {};
       _float fDist = 0.f;

       m_pGameInstance->World_MouseRay(vMouseRay);


       if (m_pColliderCom->IntersectRay(vMouseRay, &fDist))
       {
           if (2.5f > fDist)
           {
               pPlayer->RayCollInfo(m_wUIName, this);
           }
       }
       else
       {
           pPlayer->Set_RabbitCatch(false);
       }

   }*/

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

#ifdef _DEBUG
    m_pColliderCom->Render();
    m_pNavigationCom->Render();
#endif


    return S_OK;
}

_bool CMonster::IntersectRay(_vector* pRayArray, _float* fDist)
{

    if (m_bisStunned)
    {
        if (m_pColliderCom->IntersectRay(pRayArray, fDist))
            return true;
    }

    return false;
}

void CMonster::AnimControl()
{
    CModel::ANIMATION_DESC		AnimDesc{ 3, true };

    switch (m_iState)
    {
    case Client::CMonster::STATE_WALK:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 1;
        break;
    case Client::CMonster::STATE_RUN:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 2;
        break;
    case Client::CMonster::STATE_GROOM:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 4;
        break;
    case Client::CMonster::STATE_IDLELEFT:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 5;
        break;
    case Client::CMonster::STATE_IDLERIGHT:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 6;
        break;
    case Client::CMonster::STATE_IDLE:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 7;
        break;
    case Client::CMonster::STATE_DEATH:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 3;
        break;
    case Client::CMonster::STATE_CORPSE:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 0;
        break;
    case Client::CMonster::STATE_HOPE:
        break;
    case Client::CMonster::STATE_END:
        break;
    default:
        break;
    }

    m_pModelCom->Set_AnimationIndex(AnimDesc);

}

void CMonster::Reset_Timer()
{

    m_RandomNumber = mt19937_64(m_RandomDevice());

    uniform_int_distribution<unsigned int> RandState(STATE_WALK, STATE_IDLE);
    uniform_real_distribution<float> RandPatternTime(3.f, 7.f);

    m_fPatternTime = RandPatternTime(m_RandomNumber);
    m_fCurrentTime = { 0.f };
    m_iState = RandState(m_RandomNumber);
}

HRESULT CMonster::Add_Components()
{
    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_ModelTag,
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMeshMonster"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Collider */
    CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};

    ColliderDesc.eType = CCollider::TYPE_AABB;
    ColliderDesc.vExtents = _float3(0.3f, 0.3f, 0.3f);//aabb 조절가능
    ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y*0.5f, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    /* For.Com_Navigation */
    CNavigation::NAVIGATION_DESC	NavigationDesc{};

    NavigationDesc.iCurrentCellIndex = m_iCellIndex;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
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


    return S_OK;
}

_bool CMonster::Detected()
{
    _matrix Playermatrix = XMLoadFloat4x4(m_pPlayerMatrix);

    _vector Position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
        
    _vector TargetPos, Scale, Rotation;
    if (XMMatrixDecompose(&Scale, &Rotation, &TargetPos, Playermatrix))
    {
        _float Length = XMVectorGetX(XMVector3Length(TargetPos - Position));
        if (m_fDetectRange > Length)
        {
            return true;
        }
        else
        {
            m_bEscapeRotate = false;
            return false;
        }
    }
    else
    {
        m_bEscapeRotate = false;
        return false;
    }

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
