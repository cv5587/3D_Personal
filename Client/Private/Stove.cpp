#include "Stove.h"
#include "GameInstance.h"
#include "Meat.h"
#include "Item.h"
#include "Explosion.h"
CStove::CStove(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CInteractiveObject{ pDevice,pContext }
{
}

CStove::CStove(const CStove& rhs)
    :CInteractiveObject(rhs)
{
}

HRESULT CStove::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStove::Initialize(void* pArg)
{
    STOVE_DESC* pDesc = static_cast<STOVE_DESC*>(pArg);


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));
    m_ItemUIName = TEXT("스토브");
    m_InItem.resize(2);

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_fMeatPos[0] = _float3{ 0.3f ,0.85f, 0.f };
    m_fMeatPos[1] = _float3{ -0.3f ,0.85f, 0.f };

   // if (FAILED(Add_Part()))
  //      return E_FAIL;

    //3번임
    LIGHT_DESC			LightDesc = {};
    ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
    LightDesc.eType = LIGHT_DESC::TYPE_POINT;
    LightDesc.vPosition = _float4(20.f, 5.f, 20.f, 1.f);
    LightDesc.fRange = 10.f;
    LightDesc.vDiffuse = _float4(1.f, 0.3f, 0.f, 0.3f);
    LightDesc.vAmbient = _float4(0.4f, 0.1f, 0.1f, 1.f);
    LightDesc.vSpecular = LightDesc.vDiffuse;
    LightDesc.bSwitch = false;
    m_pGameInstance->Add_Light(LightDesc);


    CExplosion::EXPLOSION_DESC ExplosionDesc{};

    ExplosionDesc.pParentsMatrix = m_pTransformCom->Get_WorldFloat4x4();
    XMStoreFloat4x4(&ExplosionDesc.fControlMatrix, XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixTranslation(0.0f, 0.3f, 0.1f));
    m_pEffect = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Explosion"), &ExplosionDesc);


    return S_OK;
}

void CStove::Priority_Tick(_float fTimeDelta)
{
}

void CStove::Tick(_float fTimeDelta)
{
    if (m_bFire)
    {

        m_pEffect->Tick(fTimeDelta);
        m_pGameInstance->Set_LightSwitch(3, true);
    }
    else
    {
        m_pGameInstance->Set_LightSwitch(3, false);
    }

    for (_int i = 0; i < m_InItem.size(); i++)
    {
        if (nullptr != m_InItem[i])
        {
            m_fCookTime[i] += fTimeDelta;
        }
    }

    for (_int i = 0; i < 2; i++)
    {
        
        if (10.f < m_fCookTime[i])
        {
            m_InItem[i]->Set_Live(false);
           
            CItem::ITEM_DESC itemDesc{};
            itemDesc.ItemName = TEXT("CookedRabbit");
            itemDesc.ItemType[0] = (_uint)CItem::ITEMTYPE::ITEM_FOOD;
            itemDesc.ItemType[1] = (_uint)CItem::ITEMTYPE::ITEM_END;
            itemDesc.iQuantity = 1;
            itemDesc.fSpeedPerSec = 30.f;
            itemDesc.fRotationPerSec = XMConvertToRadians(120.f);

            itemDesc.ProtoTypeTag= TEXT("Prototype_GameObject_GEAR");
            itemDesc.ModelTag= TEXT("Prototype_Component_Model_CookedRabbit");
            XMStoreFloat4x4(&itemDesc.vPrePosition, dynamic_cast<CTransform*>((m_InItem[i])->Get_Transform())->Get_WorldMatrix());

            m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), itemDesc.ProtoTypeTag, &itemDesc);
            m_pGameInstance->StopSound(CH_COOK);
            m_fCookTime[i] = 0.f;
            m_InItem[i] = nullptr;
        }
    }

    m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CStove::Late_Tick(_float fTimeDelta)
{
    if (m_bFire)
    {
        m_pEffect->Late_Tick(fTimeDelta);
        _vector vLightPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
        m_pGameInstance->Play_Sound(TEXT("FireLoop.wav"), CH_STOVE, 1.0f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

        m_pGameInstance->Set_LightPosition(3, vLightPos);
    }
    else
    {
        m_pGameInstance->StopSound(CH_STOVE);
    }


    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
    m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CStove::Render()
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

_bool CStove::IntersectRay(_vector* pRayArray, _float* fDist)
{
    if (m_pColliderCom->IntersectRay(pRayArray, fDist))
    {
        return true;
    }

    return false;
}

HRESULT CStove::Action()
{

    CGameObject* pPlayer = m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"));

    dynamic_cast<CPlayer*>(pPlayer)->Set_State(PLAYERSTATE::PLAYER_BURN);   


    //아이템 인벤토리에 넣어주기

    return S_OK;
}

_bool CStove::RayCollInfo(_vector* pRayArray, CGameObject** pGameObject)
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

HRESULT  CStove::Cooking(void* pDesc)
{
    CItem::ITEM_DESC* pItemDesc = static_cast<CItem::ITEM_DESC*>(pDesc);
    _vector pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);   
    XMStoreFloat4x4(&pItemDesc->vPrePosition, XMMatrixIdentity() *
        XMMatrixTranslation(m_fMeatPos[0].x, m_fMeatPos[0].y, m_fMeatPos[0].z)*m_pTransformCom->Get_WorldMatrix());

    if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), pItemDesc->ProtoTypeTag,&m_InItem[0], pItemDesc)))
        return E_FAIL;

    Safe_Release(m_InItem[0]);

    return S_OK;
}

HRESULT CStove::Add_Components()
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
    CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};

    ColliderDesc.eType = CCollider::TYPE_AABB;
    ColliderDesc.vExtents = _float3(0.3f, 0.3f, 0.3f);//aabb 조절가능
    ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
   //ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y * 0.5f, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;


    return S_OK;
}

HRESULT CStove::Add_Part()
{
    ///*생고기*/

    CMeat::MEAT_DESC			MeatDesc = {};
    MeatDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    //WeaponDesc.pState = &m_iState;
    MeatDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Weapon");
    MeatDesc.ModelTag = TEXT("Prototype_Component_Model_RawRabbit");
    XMStoreFloat4x4(&MeatDesc.vPrePosition, XMMatrixIdentity());
    //몸의 모델컴을 가져옴
  
    //뼈정보를 넣어줘서 제작
    CGameObject* pMeat = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Meat"), &MeatDesc);
    if (nullptr == pMeat)
        return E_FAIL;
    m_InItem.emplace_back(pMeat);


    ///*구운고기*/

    MeatDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    //WeaponDesc.pState = &m_iState;
    MeatDesc.ProtoTypeTag = TEXT("Prototype_GameObject_Weapon");
    MeatDesc.ModelTag = TEXT("Prototype_Component_Model_RawRabbit");
    XMStoreFloat4x4(&MeatDesc.vPrePosition, XMMatrixIdentity());
    //몸의 모델컴을 가져옴

    //뼈정보를 넣어줘서 제작
    pMeat = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Meat"), &MeatDesc);
    if (nullptr == pMeat)
        return E_FAIL;
    m_InItem.emplace_back(pMeat);


    return S_OK;
}

HRESULT CStove::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ))))
        return E_FAIL;


    return S_OK;
}

CStove* CStove::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CStove* pInstance = new CStove(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CStove");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CStove::Clone(void* pArg)
{
    CStove* pInstance = new CStove(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CStove");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CStove::Free()
{
    __super::Free();

    for (auto& pGameObject : m_InItem)
        Safe_Release(pGameObject);

    Safe_Release(m_pEffect);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
