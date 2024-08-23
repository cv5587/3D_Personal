#include "stdafx.h"
#include "UIColor.h"	

#include "GameInstance.h"
#include "Item.h"
#include "Player.h"

CUIColor::CUIColor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBase(pDevice, pContext), m_fX(0.f), m_fY(0.f), m_fSizeX(0), m_fSizeY(0)
{
	ZeroMemory(&m_WorldMatrix, sizeof(_float4x4));
	ZeroMemory(&m_ViewMatrix, sizeof(_float4x4));
	ZeroMemory(&m_ProjMatrix, sizeof(_float4x4));
}

CUIColor::CUIColor(const CUIColor& rhs)
	:CUIBase(rhs),
	m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_fSizeX(rhs.m_fSizeX), m_fSizeY(rhs.m_fSizeY),
	m_WorldMatrix(rhs.m_WorldMatrix), m_ViewMatrix(rhs.m_ViewMatrix), m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUIColor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIColor::Initialize(void* pArg)
{
	UI_COLOR_DESC* pDesc = static_cast<UI_COLOR_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_vColor = pDesc->vColor;
	m_iShaderPass = pDesc->iShaderPass;
	m_pCurrentLoad = pDesc->pCurrentLoad;
	m_pTransformCom->Set_State_Matrix(XMLoadFloat4x4(&pDesc->vPrePosition));
	//뷰,투영행렬
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CUIColor::Priority_Tick(_float fTimeDelta)
{
	if(nullptr == m_pCurrentLoad)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player")));
		m_pCurrentLoad=pPlayer->Get_LoadBar();
	}

}

void CUIColor::Tick(_float fTimeDelta)
{
	//if (1 == m_iShaderPass)
	//{
	//	if (3 < *m_pCurrentLoad&& 7> *m_pCurrentLoad)
	//		XMStoreFloat4(&m_vColor, _vector{ 0.f, 0.f, 0.f, 1.f });
	//}
}

void CUIColor::Late_Tick(_float fTimeDelta)
{

}

HRESULT CUIColor::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	//이건 쉐이더패스 몇번째꺼 쓸지에 대한 거임;;
	m_pShaderCom->Begin(m_iShaderPass);

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

//위치,크기, 아이콘ID  컨트롤 하면됨
//레이어에 넣어두고 렌더그룹에 안 넣어주면 됨.
void CUIColor::Choice_Render()
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}


HRESULT CUIColor::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosColor")));
	if (nullptr == m_pShaderCom)
		return E_FAIL;


	return S_OK;
}

HRESULT CUIColor::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
		return E_FAIL;

	if(1==m_iShaderPass)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fCurrentLoad", m_pCurrentLoad, sizeof(_float))))
			return E_FAIL;
	}

	return S_OK;
}

CUIColor* CUIColor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIColor* pInstance = new CUIColor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIColor::Clone(void* pArg)
{
	CUIColor* pInstance = new CUIColor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIColor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIColor::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
