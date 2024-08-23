#include "Wolf.h"

#include "GameInstance.h"
#include "Player.h"
#include "Particle_Rect.h"
CWolf::CWolf(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster{ pDevice,pContext }
{
}

CWolf::CWolf(const CWolf& rhs)
    :CMonster{rhs}
{
}

HRESULT CWolf::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWolf::Initialize(void* pArg)
{
    CWolf::WOLF_DESC* pDesc = (CWolf::WOLF_DESC*)pArg;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));


      m_wUIName = TEXT("늑대");
      m_fDetectRange = 5.f;
    

    if (FAILED(Add_Components()))
        return E_FAIL;

    if (m_isItem)
    {
        m_bisStunned = true;
        m_iState = WOLF_CORPSE;
        m_pNavigationCom->Set_OnNavigation(m_pTransformCom);
    }



    return S_OK;
}

void CWolf::Priority_Tick(_float fTimeDelta)
{
    if (nullptr == m_pPlayerMatrix)
        m_pPlayerMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_WorldFloat4x4();

}

void CWolf::Tick(_float fTimeDelta)
{
    m_fCurrentTime += fTimeDelta;
    tt += fTimeDelta;
    if(tt>5.f)
        m_Detected = Detected();

    if (!m_bStruggle)
        Wolf_Action(fTimeDelta);
    else
    {
        m_iState = WOLF_STRUGGLEWEAPON;         
        if (m_pModelCom->Get_AnimFinished())
        {
            _vector vWolfPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
            CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
            CTransform* pTransform = dynamic_cast<CTransform*>(pPlayer->Get_Transform());
            pTransform->LookAt(vWolfPos);
            m_iState = WOLF_STRUGGLEEND;
            m_bStruggle = false;
        }
    }
    WolfAnimControl(fTimeDelta);


    m_pNavigationCom->Set_OnNavigation(m_pTransformCom);

    m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());




    m_pModelCom->Play_Animation(fTimeDelta);
}

void CWolf::Late_Tick(_float fTimeDelta)
{

    if (true == m_pGameInstance->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 10.f))
    {

        if (!m_bisStunned)
        {
            //자기 콜라이더 들고 아이템 레이어 에 들어가기 아이템 레이어 안에
            if (m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_Item"), m_pColliderCom))
            {
                //이건 도망만
                m_iState = WOLF_DEATH;
            }
            //플레이어 레이와 충돌(언제? 플레이어 상태가 fire, hipfire일때.)

        }

        m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
        m_pGameInstance->Add_DebugComponent(m_pColliderCom);
        m_pGameInstance->Add_DebugComponent(m_pNavigationCom);
#endif
    }
}

HRESULT CWolf::Render()
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

_bool CWolf::IntersectRay(_vector* pRayArray, _float* fDist)
{
    return __super::IntersectRay(pRayArray, fDist);
}

void CWolf::Wolf_Action(_float fTimeDelta)
{
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
    if (nullptr == pPlayer)
        return;

    if (!m_Detected)
    {
        switch (m_iState)
        {
        case Client::CWolf::WOLF_RUN:
        {
            m_pTransformCom->Patrol(fTimeDelta, m_pNavigationCom);
            if (m_fCurrentTime > m_fPatternTime)
            {
                Reset_Timer();
            }
        }
            break;
        case Client::CWolf::WOLF_HOWL:
        {
            m_pGameInstance->Play_Sound(TEXT("WolfHowl.wav"), CH_WOLF, 1.0f);
            if (m_pModelCom->Get_AnimFinished())
            {
                m_pGameInstance->StopSound(CH_WOLF);
                Reset_Timer();
            }
        }
            break;
        case Client::CWolf::WOLF_IDLELEFT:
        {
            if (m_pModelCom->Get_AnimFinished())
            {
                Reset_Timer();
            }
        }
            break;
        case Client::CWolf::WOLF_IDLERIGHT:
        {
            if (m_pModelCom->Get_AnimFinished())
            {
                Reset_Timer();
            }
        }
            break;
        case Client::CWolf::WOLF_IDLE:
        {
            if (m_fCurrentTime > m_fPatternTime)
            {
                Reset_Timer();
            }
        }
           break;
        case Client::CWolf::WOLF_SNIFF:
        {
            if (m_pModelCom->Get_AnimFinished())
            {
                Reset_Timer();
            }
        }
            break;
        case Client::CWolf::WOLF_DEATH:
        {
            m_pGameInstance->StopSound(CH_WOLF);
            m_pGameInstance->Play_Sound(TEXT("WolfDeath.wav"), CH_WOLF, 1.f);
            if (m_pModelCom->Get_AnimFinished())
            {
                m_iState = WOLF_CORPSE;
            }
        }
            break;
        case Client::CWolf::WOLF_ESCAPE:
        {
            m_iState = WOLF_IDLE;
            break;
        }
        case Client::CWolf::WOLF_CORPSE:
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
        case Client::CWolf::WOLF_HOWL:     
            m_iState = WOLF_CHASE;
        break;
        case Client::CWolf::WOLF_IDLELEFT:
            m_iState = WOLF_CHASE;
            break;
        case Client::CWolf::WOLF_IDLERIGHT:
            m_iState = WOLF_CHASE;
            break;
        case Client::CWolf::WOLF_IDLE:
            m_iState = WOLF_CHASE;
            break;
        case Client::CWolf::WOLF_SNIFF:
            m_iState = WOLF_CHASE;
            break;
        case Client::CWolf::WOLF_RUN:
            m_iState = WOLF_CHASE;
            break;
        case Client::CWolf::WOLF_CHASE:
        {
            m_pTransformCom->Chase(XMLoadFloat4x4(m_pPlayerMatrix), fTimeDelta, m_pNavigationCom);
            _float flength = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
            if (flength < 1.f&&false== m_bStruggle)
            {
                m_bStruggle = true;
                CTransform* pTransform = dynamic_cast<CTransform*>(pPlayer->Get_Transform());
                _vector vWolfPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
                _vector vWolfLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
                _vector vPlayerPosition = vWolfPos + vWolfLook * 2.65f;
                pTransform->Set_State(CTransform::STATE_POSITION, vPlayerPosition);
                pTransform->LookBack(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

                pPlayer->Set_Struggle(true);
            }
            
        }
        break;
        case Client::CWolf::WOLF_ATTACK:
        {
            if (m_pModelCom->Get_AnimFinished())
            {
                m_iState = WOLF_CHASE;
            }
        }
        break;
        case Client::CWolf::WOLF_STRUGGLEWEAPON:
            if (m_pModelCom->Get_AnimFinished())
            {

                m_iState = WOLF_STRUGGLEEND;
                m_bStruggle = false;
            }
            break;
        case Client::CWolf::WOLF_STRUGGLEEND:
            if (m_pModelCom->Get_AnimFinished())
            {

                m_iState = WOLF_ESCAPE;
            }
            break;
        case Client::CWolf::WOLF_ESCAPE:
        {
            m_pTransformCom->Escape(XMLoadFloat4x4(m_pPlayerMatrix), fTimeDelta, m_pNavigationCom);
            break;
        }
        case Client::CWolf::WOLF_DEATH:
        {
            m_pGameInstance->StopSound(CH_WOLF);
            m_pGameInstance->Play_Sound(TEXT("WolfDeath.wav"), CH_WOLF, 1.0f);

            if (m_pModelCom->Get_AnimFinished())
            {
                m_iState = WOLF_CORPSE;
            }
        }
        break;
        case Client::CWolf::WOLF_CORPSE:
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

void CWolf::WolfAnimControl(_float fTimeDelta)
{
    CModel::ANIMATION_DESC		AnimDesc{ 3, true };

    switch (m_iState)
    {
    case Client::CWolf::WOLF_RUN:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 15;
        break;
    case Client::CWolf::WOLF_CHASE:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 15;
        break;
    case Client::CWolf::WOLF_HOWL:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 4;
        break;
    case Client::CWolf::WOLF_IDLELEFT:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 12;
        break;
    case Client::CWolf::WOLF_IDLERIGHT:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 11;
        break;
    case Client::CWolf::WOLF_IDLE:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 35;
        break;
    case Client::CWolf::WOLF_SNIFF:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 13;
        break;
    case Client::CWolf::WOLF_DEATH:
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 1;
        break;
    case Client::CWolf::WOLF_CORPSE:
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 2;
        break;
    case Client::CWolf::WOLF_ATTACK://얼굴 물어 뜯기
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 3;
        break;
    case Client::CWolf::WOLF_STRUGGLEWEAPON://무기 맞기
        AnimDesc.isLoop = false;
        //AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 5;
        break;
    case Client::CWolf::WOLF_STRUGGLEEND://걍 끝내기
        AnimDesc.isLoop = false;
        AnimDesc.iAnimIndex = 0;
        break;
    case Client::CWolf::WOLF_ESCAPE://ㅌㅌ
        AnimDesc.isLoop = true;
        AnimDesc.iAnimIndex = 15;
        break;
    default:
        break;
    }

    m_pModelCom->Set_AnimationIndex(AnimDesc);

}

void CWolf::Reset_Timer()
{
    m_RandomNumber = mt19937_64(m_RandomDevice());

    uniform_int_distribution<unsigned int> RandState(WOLF_RUN, WOLF_IDLE);
    uniform_real_distribution<float> RandPatternTime(3.f, 7.f);

    m_fPatternTime = RandPatternTime(m_RandomNumber);
    m_fCurrentTime = { 0.f };
    m_iState = RandState(m_RandomNumber);
}

HRESULT CWolf::Add_Components()
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
    ColliderDesc.vExtents = _float3(0.4f, 0.4f, 0.4f);//aabb 조절가능
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

HRESULT CWolf::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
        return E_FAIL;

    return S_OK;
}

_bool CWolf::Detected()
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

CWolf* CWolf::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

    CWolf* pInstance = new CWolf(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CWolf");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWolf::Clone(void* pArg)
{
    CWolf* pInstance = new CWolf(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CWolf");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWolf::Free()
{
    __super::Free();
}
