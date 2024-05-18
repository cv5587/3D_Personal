#include "UIBase.h"

CUIBase::CUIBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice,pContext }
{
}

CUIBase::CUIBase(const CGameObject& rhs)
    :CGameObject{ rhs }
{
}

HRESULT CUIBase::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIBase::Initialize(void* pArg)
{

    UI_BASE_DESC* pUIDesc = static_cast<UI_BASE_DESC*>(pArg);

    if (nullptr != pArg)//겜오브젝트 전체가 들고있을 데이터 
    {
        m_iIconID = pUIDesc->Icon_ID;
        m_TextureTag = pUIDesc->TextureTag;
        m_UIID = pUIDesc->UIID;
    }


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CUIBase::Priority_Tick(_float fTimeDelta)
{
}

void CUIBase::Tick(_float fTimeDelta)
{
}

void CUIBase::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUIBase::Render()
{
    return S_OK;
}

HRESULT CUIBase::Action()
{
    return S_OK;
}

_bool CUIBase::Compare_UIID(const int& UIID)
{
    if (UIID == m_UIID)
        return true;
    else
        return false;
}

void CUIBase::Free()
{
    __super::Free();
}
