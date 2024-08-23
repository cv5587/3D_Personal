#include "State_Burnout.h"
#include "Player.h"
CState_Burnout::CState_Burnout()
	:CState{}
{
}

void CState_Burnout::Enter(CPlayer* Player)
{
	m_pGameInstance->Set_LightSwitch(2, false);
	Player->Set_Lit(false);
	m_pGameInstance->StopSound(MATCH);
	m_pGameInstance->Play_Sound(TEXT("FireBurnout.wav"), MATCH, 0.6f);
}

void CState_Burnout::Update(CPlayer* Player, _float fTimeDelta)
{
	if (Player->isAnimFinished())
	{

		if (CON_NORMAL == Player->isCondition())
		{
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
		}
		else
		{
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
		}

		Player->Set_UnEquip();
		Player->Reset_BurnTime();
		Player->End_Change();
	}
	_long		MouseMoveX = { 0 };
	if (MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		Player->Player_Turn(fTimeDelta, MouseMoveX);
	}

	_long		MouseMoveY = { 0 };
	if (MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		Player->Cam_Turn(fTimeDelta, MouseMoveY);
	}

	Player->Mouse_Fix();
}

void CState_Burnout::Exit(CPlayer* Player)
{

}

CState* CState_Burnout::Create()
{

	CState_Burnout* pInstance = new CState_Burnout();
	return pInstance;
}

void CState_Burnout::Free()
{
	__super::Free();
}
