#include "SoundMgr.h"
#include "GameInstance.h"
#include "PipeLine.h"

CSoundMgr::CSoundMgr(void)
	:m_pGameInstance{CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CSoundMgr::Ready_Sound()
{
	FMOD_RESULT result;
	unsigned int      version;
	result = FMOD::System_Create(&m_pSystem);
	if (FMOD_OK != result)
		return E_FAIL;

	result = m_pSystem->getVersion(&version);
	if (result != FMOD_OK) 
		return E_FAIL;

	result = m_pSystem->init(512, FMOD_INIT_NORMAL, nullptr);
	if (FMOD_OK != result)
		return E_FAIL;

	LoadSoundFile();

	return S_OK;


	//// 사운드를 담당하는 대표객체를 생성하는 함수
	//FMOD_System_Create(&m_pSystem,64);
	//
	//// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	//FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
	////FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, NULL);

	//if(nullptr==m_pSystem)
	//	return E_FAIL;

	//LoadSoundFile();
}

void CSoundMgr::Update_Listener()
{
	XMStoreFloat4x4(&m_PlayerMatrix, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::TRANSFORMSTATE::TS_VIEW));	
	//_vec3 vUp = { 0.f, 1.f, 0.f };
	_matrix PlayerMatrix = XMLoadFloat4x4(&m_PlayerMatrix);


	FMOD_VECTOR position = { PlayerMatrix.r[3].m128_f32[0], PlayerMatrix.r[3].m128_f32[1], PlayerMatrix.r[3].m128_f32[2] };
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward = { PlayerMatrix.r[2].m128_f32[0], PlayerMatrix.r[2].m128_f32[1], PlayerMatrix.r[2].m128_f32[2] };
	FMOD_VECTOR up = { PlayerMatrix.r[1].m128_f32[0], PlayerMatrix.r[1].m128_f32[1], PlayerMatrix.r[1].m128_f32[2] };

	//FMOD_Channel_Set3DAttributes(m_pChannelArr[BACKGROUND], &position, &velocity);
	m_pChannelArr[BACKGROUND]->set3DAttributes(&position, &velocity);
	//FMOD_System_Set3DListenerAttributes(m_pSystem, 0, &position, &velocity, &forward, &up);
	m_pSystem->set3DListenerAttributes(0, &position, &velocity, &forward, &up);

	//FMOD_System_Update(m_pSystem);
	m_pSystem->update();
}

void CSoundMgr::Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume)
{
	_bool bplay = false;
	m_pChannelArr[eID]->isPlaying(&bplay);
	if (!bplay)
	{
		FMOD_RESULT result;

		map<wstring, FMOD::Sound*>::iterator iter;

		iter = find_if(m_mapSound.begin(), m_mapSound.end(),
			[&](auto& iter)->bool
			{
				return pSoundKey == iter.first;
			});

		if (iter == m_mapSound.end())
			return;

		//끝나구 출력 만들기.
		//_bool isPlay = false;
		//m_pChannelArr[eID]->isPlaying(&isPlay);
		//if (!isPlay)
		//{
		//}
		result = m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[eID]);
		if (FMOD_OK != result)
			return;

		result = m_pChannelArr[eID]->setVolume(fVolume);
		if (FMOD_OK != result)
			return;

		result = m_pSystem->update();
		if (FMOD_OK != result)
			return;
	}
	
		

	
}

void CSoundMgr::Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _bool bPlay)
{
	if (!bPlay)
	{
		map<wstring, FMOD::Sound*>::iterator iter;

		iter = find_if(m_mapSound.begin(), m_mapSound.end(),
			[&](auto& iter)->bool
			{
				return pSoundKey == iter.first;
			});

		if (iter == m_mapSound.end())
			return;

		_bool play = false;

		if (m_pChannelArr[eID]->isPlaying(&play))
		{
			m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[eID]);
			m_pChannelArr[eID]->setVolume(fVolume);
		}
		m_pSystem->update();

	}
}

void CSoundMgr::Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _fvector vPos)
{
	
	
	if (10.f < XMVectorGetX(XMVector3Length(vPos - XMLoadFloat4x4(&m_PlayerMatrix).r[3]))) 
		return;	//거리 10 이상일 때 그냥 함수 건너뜀

	map<wstring, FMOD::Sound*>::iterator iter;	

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),	
		[&](auto& iter)->bool
		{
			return pSoundKey == iter.first;
		});

	if (iter == m_mapSound.end())
		return;

	//_vec3 vPos = pGameObject->Get_TransformCom_Pointer()->Get_Pos();
	FMOD_VECTOR position = { XMVectorGetX(vPos), XMVectorGetY(vPos), XMVectorGetZ(vPos) };
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
	m_pChannelArr[eID]->set3DAttributes(&position, &velocity);

	FMOD_BOOL play = FALSE;
	_bool bplay = false;

	if(m_pChannelArr[eID]->isPlaying(&bplay))
	{
		m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[eID]);
		m_pChannelArr[eID]->set3DMinMaxDistance(1.f, 5.f);
	}
	/*
	if (FMOD_Channel_IsPlaying(m_pChannelArr[m_iSoundIdx], &play))
	{
		FMOD_Sound_Set3DMinMaxDistance(iter->second, 1.0f, 5.0f);
	}*/


	m_pChannelArr[eID]->setVolume(fVolume);

	m_pSystem->update();

}


void CSoundMgr::Play_BGM(wstring pSoundKey, _float fVolume)
{
	map<wstring, FMOD::Sound*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return pSoundKey == iter.first;
		});

	if (iter == m_mapSound.end())
		return;

	m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[BACKGROUND]);
	//FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);


	m_pChannelArr[BACKGROUND]->setMode(FMOD_LOOP_NORMAL);
	//FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);

	m_pChannelArr[BACKGROUND]->setVolume(fVolume);
	//FMOD_Channel_SetVolume(m_pChannelArr[BGM], fVolume);

	m_pSystem->update();
	//FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	m_pChannelArr[eID]->stop();
	//FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::LoadSoundFile()
{

	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddata_t fd;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	long long handle = _findfirst("../../Sound/*.wav", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../../Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../Sound/Success.wav"
		strcat_s(szFullPath, fd.name);

		FMOD::Sound* pSound = nullptr;

		FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_3D, nullptr, &pSound);
		//FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_3D | FMOD_SOFTWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			wstring pSoundKey(fd.name, fd.name + iLength - 1);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnext(handle, &fd);
	}


	m_pSystem->update();

	_findclose(handle);
}

CSoundMgr* CSoundMgr::Create()
{
	CSoundMgr* pInstance = new CSoundMgr();

	if (FAILED(pInstance->Ready_Sound()))
	{
		MSG_BOX("Failed to Created : CSoundMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSoundMgr::Free(void)
{
	for (auto& Mypair : m_mapSound)
	{
		//FMOD_Sound_Release(Mypair.second);
		Mypair.second->release();
	}
	m_mapSound.clear();
	
	for (size_t i = 0; i < MAXCHANNEL; i++)
	{
		if (m_pChannelArr[i] != nullptr)
		{
			// 채널을 중지합니다.
			m_pChannelArr[i]->stop();
			m_pChannelArr[i] = nullptr; // 포인터를 nullptr로 초기화합니다.
		}
	}

	if (m_pSystem)
	{
		m_pSystem->release();
		m_pSystem = nullptr;
	}

	Safe_Release(m_pGameInstance);
	//FMOD_System_Release(m_pSystem);
	//FMOD_System_Close(m_pSystem);
}
