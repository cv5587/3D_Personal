#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	enum ANIMTYPE { OBJ_PLAYER, OBJ_MONSTER, OBJ_WEAPON, OBJ_ENVIRONMENT, OBJ_END };
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	_bool Get_Finished() const {
		return m_isFinished;

	}

public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones, ANIMTYPE eAnimType);
	HRESULT Initialize(ifstream* fin);
	void Update_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _bool isLoop);
	void Update_StruggleTransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _bool isLoop);
	_bool Shift_Animation_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones);
	void Reset();
	void Shift_Reset();

	HRESULT Save_Animation(ofstream* fout);
	_bool Compare_CurrentTime(_double isTime);
private:
	_char			m_szName[MAX_PATH] = "";
	_double			m_Duration = { 0.0 };
	_double			m_TickPerSecond = { 0.0 };
	_double			m_CurrentPosition = { 0.0 };
	_bool			m_isFinished = { false };

	_uint					m_iNumChannels = { 0 };
	
	//각 채널들의 현재 위치(currentposition) 의 왼쪽값을 채널의 갯수만큼 순서대로 모아둠
	vector<_uint>			m_CurrentKeyFrameIndices;
	//위는 아래 각 채널이 가진 현재 위치의 왼쪽 키값임
	vector<class CChannel*>	m_Channels;
	vector<KEYFRAME> m_LastKeys;
	
	_double		m_ShiftDuration = { 0.15 };
	_double		m_ShiftCurrentPosition = { 0.0 };
	_bool			m_First_Shift = { true };
public:
	static CAnimation* Create(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones, ANIMTYPE eObjectType);
	static CAnimation* Create(ifstream* fin);
	CAnimation* Clone();
	virtual void Free() override;
};

END