#pragma once
#include "Base.h"
BEGIN(Engine)
class CChannel final:
    public CBase
{
private:    
    CChannel();
    virtual ~CChannel() = default;
public:
    HRESULT Initialize(const aiNodeAnim* pAIChannel, const vector<class CBone*>& Bones);
    HRESULT Initialize(ifstream* fin);
    void Update_TransformationMatrix(_double CurrentPosition, const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex);

    HRESULT Save_Channel( ofstream* fout);
private:
    //채널 이름
    _char       m_szName[MAX_PATH] = " ";
    //영향받는 본의 인덱스
    _uint       m_iBoneIndex = { 0 };
    //키프레임 갯수
    _uint				m_iNumKeyFrames = { 0 };
    //각 키프레임별 영향 값(scale,rotation(쿼터니언),position,time)
    vector<KEYFRAME>	m_KeyFrames;
public:
    static CChannel* Create(const aiNodeAnim* pAIChannel, const vector<class CBone*>& Bones);
    static CChannel* Create(ifstream* fin);
    virtual void Free() override;
};

END