#include "State_StruggleKnife.h"
#include "Player.h"
#include "UIBlood.h"
CState_StruggleKnife::CState_StruggleKnife()
	:CState{}
{
}

void CState_StruggleKnife::Enter(CPlayer* Player)
{
	m_pGameInstance->StopSound(CH_WOLF);
	m_pGameInstance->Play_Sound(TEXT("WolfKnife.wav"), CH_WOLF, 1.0f);
}

void CState_StruggleKnife::Update(CPlayer* Player, _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	m_fTwoTime += fTimeDelta;
	if (!m_bOnce)
	{
		if (m_fTime > 1.8f)
		{
			CUIBlood::UI_BLOOD_DESC UIBloodDesc = {};
			XMStoreFloat4x4(&UIBloodDesc.vPrePosition,
				XMMatrixIdentity() * XMMatrixScaling(g_iWinSizeX, g_iWinSizeY, 1.f) * XMMatrixTranslation(0, 0, 0.0f));

			if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Blood"), TEXT("Prototype_GameObject_UIBlood"), &UIBloodDesc)))
				return;

			m_bOnce = true;
		}

	}
	if (!m_bTwoOnce)
	{
		if (m_fTwoTime > 4.2f)
		{
			CUIBlood::UI_BLOOD_DESC UIBloodDesc = {};
			XMStoreFloat4x4(&UIBloodDesc.vPrePosition,
				XMMatrixIdentity() * XMMatrixScaling(g_iWinSizeX, g_iWinSizeY, 1.f) * XMMatrixTranslation(0, 0, 0.0f));

			if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Blood"), TEXT("Prototype_GameObject_UIBlood"), &UIBloodDesc)))
				return;

			m_bTwoOnce = true;
		}
	}

	if (true == Player->isAnimFinished())
	{
		Player->Set_FadeIn();	
		Player->Add_EnterTime(fTimeDelta);	
		if (Player->isFadeInFinished())
		{
			Player->Set_UnEquip();
			Player->Set_State(PLAYER_FADEOUT);
		}

	}
	_long		MouseMoveY = { 0 };
	if (MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		Player->Cam_Turn(fTimeDelta, MouseMoveY);
	}

	_long		MouseMoveX = { 0 };
	if (MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		Player->Player_Turn(fTimeDelta, MouseMoveX);
	}


}

void CState_StruggleKnife::Exit(CPlayer* Player)
{

	m_fTime = 0.f;
	m_fTwoTime = 0.f;
	m_bOnce = false;
	m_bTwoOnce = false;
	m_pGameInstance->StopSound(CH_WOLF);
	Player->Set_StruggleItem(false);
}

CState* CState_StruggleKnife::Create()
{
	CState_StruggleKnife* pInstance = new CState_StruggleKnife();
	return pInstance;
}

void CState_StruggleKnife::Free()
{
	__super::Free();
}
