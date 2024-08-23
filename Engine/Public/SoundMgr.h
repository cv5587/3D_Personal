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
	void Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume);	//true 면 무시후 출력 , false 면 안하기
	void Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _bool bPlay);
	void Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _fvector vPos);	//거리에 따른 음량 조절

	void Play_BGM(wstring pSoundKey, _float fVolume);
	void StopSound(CHANNELID eID);




private:
	void LoadSoundFile();
private:
	class CGameInstance* m_pGameInstance = { nullptr };

	// 사운드 리소스 정보를 갖는 객체 
	map<wstring, FMOD::Sound*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD::Channel* m_pChannelArr[MAXCHANNEL];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체
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