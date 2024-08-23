#include "State_Equip.h"
#include "Player.h"

CState_Equip::CState_Equip()
	:CState{}
{
}

void CState_Equip::Enter(CPlayer* Player)
{


}

void CState_Equip::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_W))
	{
		Player->Go_Straight(fTimeDelta);
	}
	else 	if (m_pGameInstance->Get_DIKeyState(DIK_S))
	{
		Player->Go_Backward(fTimeDelta);
	}
	else 	if (m_pGameInstance->Get_DIKeyState(DIK_A))
	{
		Player->Go_Left(fTimeDelta);
	}
	else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
	{
		Player->Go_Right(fTimeDelta);
	}

	//_uint Equip = Player->isEquip();
	//_bool ChangeFinish= false;
	//switch (Equip)	
	//{
	//case PLAYEREQUIP::EQUIP_STONE:
	//	ChangeFinish=Player->isAnimFinished();
	//	break;
	//case PLAYEREQUIP::EQUIP_REVOLVER:
	//	ChangeFinish = Player->isRevolver_AnimFin();
	//	break;
	//case PLAYEREQUIP::EQUIP_RABBIT:
	//	ChangeFinish = Player->isRabbit_AnimFin();
	//	break;
	//default:
	//	break;
	//}

	if(Player->isAnimFinished())
	{
		if (EQUIP_FLARE == Player->isEquip())
		{
			Player->Set_State(PLAYERSTATE::PLAYER_FLAREPRE);
			return;
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_W))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_S))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		}
		else 
		{
			if (CON_NORMAL == Player->isCondition())
			{
				Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
			}
			else
			{
				Player->Set_State(PLAYERSTATE::PLAYER_IDLE_EXHAUSTED);
			}
		}
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

void CState_Equip::Exit(CPlayer* Player)
{
	
}

CState* CState_Equip::Create()
{
	CState_Equip* pInstance = new CState_Equip();
	return pInstance;
}

void CState_Equip::Free()
{
	__super::Free();
}
