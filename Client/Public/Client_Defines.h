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
	//�ִϸ��̼� �б����
	enum PLAYERSTATE {
		PLAYER_IDLE,//�⺻
		PLAYER_IDLE_EXHAUSTED,//��ħ �⺻
		PLAYER_JOG,
		PLAYER_RUN,//�ٱ�
		PLAYER_EQUIP,//����
		PLAYER_UNEQUIP,//Ż��

		PLAYER_AIM_S,//���ӽ���
		PLAYER_AIM,//������ �����߿��� �����ϼ� ����
		PLAYER_AIM_E,//��������

		PLAYER_WALK_S,//�ȱ� ����-stone
		PLAYER_WALK_E,//�ȱ�����-stone
		PLAYER_WALK_FB,//�յ� �ȱ�
		PLAYER_WALK_L,//�·� �ȱ�
		PLAYER_WALK_R,//��� �ȱ�
		PLAYER_WALK_LE,//�·� �ȱ� ����-stone
		PLAYER_WALK_RE,//��� �ȱ� ����-stone


		PLAYER_THROW,//������
		PLAYER_ATTACK,//����(STIM(��,������,���������) ���� FPH_Gear_Stim_Inject)
		PLAYER_FIRE,//���� �ѽ��
		PLAYER_HIPFIRE,//�׳� �ѽ��
		PLAYER_RELOAD_S,//�̰� ���� ������ �Ǵµ� �ϴ� Ŭ������ ����� ����.
		PLAYER_RELOAD,//������ �����ϼ� ���� �ٵ� ����Ű,���콺 Ŭ�� �ϸ� ����
		PLAYER_ENTER,
		PLAYER_RELOAD_E,

		PLAYER_PICKUP,
		PLAYER_INVENTORY,

		PLAYER_KILL,//�䳢 �ֱ�
		PLAYER_RELEASE,//�䳢 �����

		PLAYER_QUEST,//����Ʈ ����
		PLAYER_BURN,//�����Ǳ�


		PLAYER_BURNOUT,//���� ����
		PLAYER_MATCHPRE,//�����Ǳ�
		PLAYER_MATSUCCESS,//�����Ǳ�

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
