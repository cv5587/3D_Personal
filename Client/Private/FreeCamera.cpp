#include "stdafx.h"
#include "FreeCamera.h"

#include "GameInstance.h"

CFreeCamera::CFreeCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera{pDevice,pContext}
{
}

CFreeCamera::CFreeCamera(const CFreeCamera& rhs)
    :CCamera{rhs}
{
}

HRESULT CFreeCamera::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CFreeCamera::Initialize(void* pArg)
{
	FREE_CAMERA_DESC* pDesc = (FREE_CAMERA_DESC*)pArg;

	m_fSensor = pDesc->fSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    return S_OK;
}

void CFreeCamera::Priority_Tick(_float fTimeDelta)
{
}

void CFreeCamera::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState_Once(DIK_Q) )
		m_bTab = !m_bTab;

	if (!m_bTab)
	{
	if (m_pGameInstance->Get_DIKeyState(DIK_LEFT) )
		m_pTransformCom->Go_Left(fTimeDelta);
	if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT) )
		m_pTransformCom->Go_Right(fTimeDelta);
	if (m_pGameInstance->Get_DIKeyState(DIK_UP) )
		m_pTransformCom->Go_Straight(fTimeDelta);
	if (m_pGameInstance->Get_DIKeyState(DIK_DOWN) )
		m_pTransformCom->Go_Backward(fTimeDelta);



		_long		MouseMove = { 0 };

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fSensor * MouseMove);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_fSensor * MouseMove);
		}

		Mouse_Fix();

	}


	__super::Tick(fTimeDelta);
}

void CFreeCamera::Late_Tick(_float fTimeDelta)
{
}

HRESULT CFreeCamera::Render()
{
    return S_OK;
}

void CFreeCamera::Mouse_Fix()
{

		POINT	pt{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

		ClientToScreen(g_hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
}

CFreeCamera* CFreeCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFreeCamera* pInstance = new CFreeCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CFreeCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFreeCamera::Clone(void* pArg)
{
	CFreeCamera* pInstance = new CFreeCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CFreeCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFreeCamera::Free()
{
	__super::Free();
}
