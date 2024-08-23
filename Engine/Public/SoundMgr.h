#pragma once
#include "Base.h"
#include "Engine_Defines.h"
BEGIN(Engine)
class ENGINE_DLL CSoundMgr :
    public CBase
{
public:

private:
	CSoundMgr(void);
	virtual ~CSoundMgr(void) = default;


public:
	HRESULT Ready_Sound();
	void Update_Listener();


public:
	void Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume);	//true �� ������ ��� , false �� ���ϱ�
	void Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _bool bPlay);
	void Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _fvector vPos);	//�Ÿ��� ���� ���� ����

	void Play_BGM(wstring pSoundKey, _float fVolume);
	void StopSound(CHANNELID eID);




private:
	void LoadSoundFile();
private:
	class CGameInstance* m_pGameInstance = { nullptr };

	// ���� ���ҽ� ������ ���� ��ü 
	map<wstring, FMOD::Sound*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD::Channel* m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü
	//FMOD_SYSTEM* m_pSystem;
	FMOD::System* m_pSystem;
	//CPlayer* m_pPlayer;
	_int		 m_iSoundIdx;
	_float4x4		m_PlayerMatrix;
	//array <float, MAXCHANNEL> m_tInitFreqs;
	_bool		m_bReverse = false;

public:
	static CSoundMgr* Create();
	virtual void	Free(void);
};

END