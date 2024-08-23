#include "Rabbit.h"
#include "GameInstance.h"
#include "Player.h"
#include "Trail_Effect.h"
CRabbit::CRabbit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster{ pDevice,pContext }
{
}

CRabbit::CRabbit(const CRabbit& rhs)
    :CMonster{rhs}
{
}

HRESULT CRabbit::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CRabbit::Initialize(void* pArg)
{
    CRabbit::RABBIT_DESC* pDesc = (CRabbit::RABBIT_DESC*)pArg;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;


    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));

   m_wUIName = TEXT("토끼");
    m_fDetectRange = 3.f;

    if (FAILED(Add_Components()))
        return E_FAIL;



    if (m_isItem)
    {
        m_bisStunned = true;
        m_iState = STATE_CORPSE;
        m_pNavigationCom->Set_OnNavigation(m_pTransformCom);
    }

    if (FAILED(Add_PartObject()))
        return E_FAIL;

    return S_OK;
}

void CRabbit::Priority_Tick(_float fTimeDelta)
{
    if(nullptr == m_pPlayerMatrix)
        m_pPlayerMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_WorldFloat4x4();  

}

void CRabbit::Tick(_float fTimeDelta)
{
    m_fCurrentTime += fTimeDelta;
    m_Detected = Detected();

    Rabbit_Action(fTimeDelta);
    RabbitAnimControl(fTimeDelta);  


    m_pNavigationCom->Set_OnNavigation(m_pTransformCom);

    m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());




    m_pModelCom->Play_Animation(fTimeDelta);
}

void CRabbit::Late_Tick(_float fTimeDelta)
{

    if (true == m_pGameInstance->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.f))
    {
        CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
        if (nullptr == pPlayer)
            return;



        if (!m_bisStunned)
        {
            //자기 콜라이더 들고 아이템 레이어 에 들어가기 아이템 레이어 안에
            if (m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_Item"), m_pColliderCom))
            {
                m_pGameInstance->Play_Sound(TEXT("RabbitDeath.wav"), CH_RABBIT, 1.f);
                
                m_iState = STATE_DEATH;
            }
            //플레이어 레이와 충돌(언제? 플레이어 상태가 fire, hipfire일때.)

        }

        m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
        m_pGameInstance->Add_DebugComponent(m_pColliderCom);
        m_pGameInstance->Add_DebugComponent(m_pNavigationCom);
#endif
    }

    pEyeEffect->Late_Tick(fTimeDelta);
}

HRESULT CRabbit::Render()
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

_bool CRabbit::IntersectRay(_vector* pRayArray, _float* fDist)
{
   return __super::IntersectRay(pRayArray, fDist);
}

void CRabbit::RabbitAnimControl(_float fTimeDelta)
{

    CModel::ANIMATION_DESC		AnimDesc{ 3, true };

    switch (m_iState)
    {
    case Client::CRabbit::STATE_WALK:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 1;
        break;
    case Client::CRabbit::STATE_RUN:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 2;
        break;
    case Client::CRabbit::STATE_GROOM:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 4;
        break;
    case Client::CRabbit::STATE_IDLELEFT:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 5;
        break;
    case Client::CRabbit::STATE_IDLERIGHT:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 6;
        break;
    case Client::CRabbit::STATE_IDLE:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 7;
        break;
    case Client::CRabbit::STATE_DEATH:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 3;
        break;
    case Client::CRabbit::STATE_CORPSE:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 0;
        break;
    case Client::CRabbit::STATE_HOPE:
        break;
    case Client::CRabbit::STATE_END:
        break;
    default:
        break;
    }

    m_pModelCom->Set_AnimationIndex(AnimDesc);

}

void CRabbit::Rabbit_Action(_float fTimeDelta)
{
    if (!m_Detected)
    {
        switch (m_iState)
        {

        case Client::CRabbit::STATE_WALK:
        {
            m_pTransformCom->Set_DeltaValue(2.f, XMConvertToRadians(120.0f));
            m_pTransformCom->Patrol(fTimeDelta, m_pNavigationCom);
            if (m_fCurrentTime > m_fPatternTime)
            {
                Reset_Timer();
            }
        }
        break;

        case Client::CRabbit::STATE_RUN:
        {
            m_pTransformCom->Set_DeltaValue(5.f, XMConvertToRadians(120.0f));
            m_pTransformCom->Patrol(fTimeDelta, m_pNavigationCom);
            if (m_fCurrentTime > m_fPatternTime)
            {
                Reset_Timer();
            }
        }
        break;

        case Client::CRabbit::STATE_GROOM:
        {
            if (m_pModelCom->Get_AnimFinished())
            {
                Reset_Timer();
            }
        }
        break;
        case Client::CRabbit::STATE_IDLELEFT:
        {
            if (m_pModelCom->Get_AnimFinished())
            {
                Reset_Timer();
            }
        }
        break;
        case Client::CRabbit::STATE_IDLERIGHT:
        {
            if (m_pModelCom->Get_AnimFinished())
            {
                Reset_Timer();
            }
        }
        break;
        case Client::CRabbit::STATE_IDLE:
        {
            if (m_fCurrentTime > m_fPatternTime)
            {
                Reset_Timer();
            }
        }
        break;

        case Client::CRabbit::STATE_DEATH:
        {
            if (m_pModelCom->Get_AnimFinished())
            {
                m_iState = STATE_CORPSE;
            }
        }
        break;

        case Client::CRabbit::STATE_CORPSE:
        {
            m_bisStunned = true;
            m_isItem = true;
        }
        break;
        default:
            break;
        }
    }
    else
    {
        _vector vTargetPos, vScale, vRotation;
        XMMatrixDecompose(&vScale, &vRotation, &vTargetPos, XMLoadFloat4x4(m_pPlayerMatrix));
        switch (m_iState)
        {
        case Client::CRabbit::STATE_WALK:
        {
            m_iState = STATE_RUN;
        }
        break;

        case Client::CRabbit::STATE_RUN:
        {
            if (!m_bEscapeRotate)
            {
                m_pTransformCom->LookBack(vTargetPos);  
                m_bEscapeRotate = true;
            }

            m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
        }
        break;

        case Client::CRabbit::STATE_GROOM:
        {
            m_iState = STATE_RUN;

        }
        break;
        case Client::CRabbit::STATE_IDLELEFT:
        {
            m_iState = STATE_RUN;
        }
        break;
        case Client::CRabbit::STATE_IDLERIGHT:
        {
            m_iState = STATE_RUN;
        }
        break;
        case Client::CRabbit::STATE_IDLE:
        {
            m_iState = STATE_RUN;
        }
        break;
        case Client::CRabbit::STATE_DEATH:
        {
            if (m_pModelCom->Get_AnimFinished())
            {
                m_iState = STATE_CORPSE;
            }
        }
        break;

        case Client::CRabbit::STATE_CORPSE:
        {
            m_bisStunned = true;
            m_isItem = true;
        }
        break;
        default:
            break;
        }
    }

}

void CRabbit::Reset_Timer()
{
    m_RandomNumber = mt19937_64(m_RandomDevice());

    uniform_int_distribution<unsigned int> RandState(STATE_WALK, STATE_IDLE);
    uniform_real_distribution<float> RandPatternTime(3.f, 7.f);

    m_fPatternTime = RandPatternTime(m_RandomNumber);
    m_fCurrentTime = { 0.f };
    m_iState = RandState(m_RandomNumber);
}

HRESULT CRabbit::Add_Components()
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
    CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc = {};

    ColliderDesc.eType = CCollider::TYPE_AABB;
    ColliderDesc.vExtents = _float3(0.3f, 0.3f, 0.3f);//aabb 조절가능
    ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y * 0.5f, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    /* For.Com_Navigation */
    CNavigation::NAVIGATION_DESC	NavigationDesc = {};

    NavigationDesc.iCurrentCellIndex = m_iCellIndex;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CRabbit::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
        return E_FAIL;

    return S_OK;
}

HRESULT CRabbit::Add_PartObject()
{
    CTrail_Effect::TRAILEFFECT_DESC TrailDesc{};
    //TrailDesc.ModelTag = TEXT("Prototype_Component_Texture_Trail");

    TrailDesc.pParentsMatrix =m_pTransformCom->Get_WorldFloat4x4();

    //m_pCamBone =
    //    dynamic_cast<CModel*>(m_PartObjects[PART_BODY]->Get_Component(TEXT("Com_Model")))->Get_CameraBone("Camera_Weapon_Offset");
    TrailDesc.fControlMatrix = dynamic_cast<CModel*>(m_pModelCom)->Get_BoneCombinedTransformationMatrix("rabbit:mesh_eyes");

   // XMStoreFloat4x4(&TrailDesc.fControlMatrix, XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(0.00f, 0.0f, -1.0f));// eye 본 넣어주기
    pEyeEffect = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_TrailEffect"), &TrailDesc);
    
    return S_OK ;
}

_bool CRabbit::Detected()
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

    return false;
}

CRabbit* CRabbit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CRabbit* pInstance = new CRabbit(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CRabbit");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CRabbit::Clone(void* pArg)
{
    CRabbit* pInstance = new CRabbit(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CRabbit");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CRabbit::Free()
{
    __super::Free();
    Safe_Release(pEyeEffect);

}
