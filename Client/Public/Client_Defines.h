#pragma once

#include <process.h>

namespace Client
{
#define SPEED_EX_WALK 1.f
#define SPEED_WALK 2.f

#define SPEED_EX_JOG 2.f
#define SPEED_JOG 3.f

#define SPEED_EX_RUN 3.f
#define SPEED_RUN 5.f

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_BASEMENT,  LEVEL_END };
	//애니메이션 분기기준
	enum PLAYERSTATE {
		PLAYER_IDLE,//기본
		PLAYER_IDLE_EXHAUSTED,//지침 기본
		PLAYER_JOG,
		PLAYER_RUN,//뛰기
		PLAYER_EQUIP,//장착
		PLAYER_UNEQUIP,//탈착

		PLAYER_AIM_S,//에임시작
		PLAYER_AIM,//에임중 에임중에는 움직일수 없음
		PLAYER_AIM_E,//에임종료

		PLAYER_WALK_S,//걷기 시작-stone
		PLAYER_WALK_E,//걷기종료-stone
		PLAYER_WALK_FB,//앞뒤 걷기
		PLAYER_WALK_L,//좌로 걷기
		PLAYER_WALK_R,//우로 걷기
		PLAYER_WALK_LE,//좌로 걷기 종료-stone
		PLAYER_WALK_RE,//우로 걷기 종료-stone


		PLAYER_THROW,//던지기
		PLAYER_ATTACK,//공격(STIM(검,몽둥이,파이프등등) 기준 FPH_Gear_Stim_Inject)
		PLAYER_FIRE,//조준 총쏘기
		PLAYER_HIPFIRE,//그냥 총쏘기
		PLAYER_RELOAD_S,//이건 조금 만져야 되는데 일단 클래스로 만들어 놓자.
		PLAYER_RELOAD,//장전중 움직일수 잇음 근데 장전키,마우스 클릭 하면 끊김
		PLAYER_ENTER,
		PLAYER_RELOAD_E,

		PLAYER_PICKUP,
		PLAYER_INVENTORY,

		PLAYER_KILL,//토끼 주김
		PLAYER_RELEASE,//토끼 살려줌

		PLAYER_QUEST,//퀘스트 진입
		PLAYER_BURN,//불지피기


		PLAYER_BURNOUT,//성냥 상태
		PLAYER_MATCHPRE,//불지피기
		PLAYER_MATSUCCESS,//불지피기

		PLAYER_HARVEST,
		PLAYER_BUILD,
		PLAYER_SLEEP,
		PLAYER_GETUP,

		PLAYER_STRUGGLE,
		PLAYER_STRUGGLEBARE,
		PLAYER_STRUGGLEKNIFE,
		PLAYER_FADEOUT,

		PLAYER_FLAREPRE,
		PLAYER_FLARESUCCESS,

		PLAYER_END
	};
	
	enum PLAYEREQUIP {
		EQUIP_NONE,
		EQUIP_STONE,
		EQUIP_REVOLVER,
		EQUIP_RABBIT,
		EQUIP_MATCH,
		EQUIP_KNIFE,
		EQUIP_FLARE,
		EQUIP_PIPE,
		EQUIP_END
	};
	enum PLAYERCLOTH {
		CLOTH_NONE ,
		CLOTH_SLEEVES,
		CLOTH_BANDAGE,
		CLOTH_END
	};
	enum PLAYERCONDITION {
		CON_NORMAL,
		CON_EXHAUSTED,
		CON_END
	};

	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;
}

extern HINSTANCE g_hInst;
extern HWND		g_hWnd;

using namespace Client;
