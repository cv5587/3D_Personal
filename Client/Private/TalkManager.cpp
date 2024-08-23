#include "TalkManager.h"

CTalkManager::CTalkManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice{pDevice}
    ,m_pContext{pContext}
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}
HRESULT CTalkManager::Initialize()
{

    //QUESTID =100;
    //TALKID =0;
    wstring TalkData = TEXT("거기 누군가! 제발 잠시 도와줄수 있는가?");
    m_TalkData.emplace(100+0,TalkData);

    TalkData = TEXT("음식을 잘못 먹었는지 속이 너무 아픈데 약좀 구해줄수 있는가?");
    m_TalkData.emplace(100+1,TalkData);

    TalkData = TEXT("구해만 준다면 꼭 보답을 하겟네 한번만 도와주시게...");
    m_TalkData.emplace(100+2, TalkData);

    TalkData = TEXT("아직 못구했나?");
    m_TalkData.emplace(200 + 0, TalkData);

    TalkData = TEXT("빨리 구해주게 죽을거 같다네");
    m_TalkData.emplace(200 + 1, TalkData);

    TalkData = TEXT("고맙네 덕분에 살것 같구만");
    m_TalkData.emplace(210 + 0, TalkData);

    TalkData = TEXT("이건 보답일세 요긴하게 쓰게");
    m_TalkData.emplace(210 + 1, TalkData);

    TalkData = TEXT("몸이 괜찮으니 배가 고파서 그런데");
    m_TalkData.emplace(210 + 2, TalkData);

    TalkData = TEXT("토끼 고기도 구해주게나");
    m_TalkData.emplace(210 + 3, TalkData);



    TalkData = TEXT("뱃가죽이 등에 붙을거 같군");
    m_TalkData.emplace(300 + 0, TalkData);

    TalkData = TEXT("아직 멀었는가?");
    m_TalkData.emplace(300 + 1, TalkData);

    TalkData = TEXT("정말 고맙네");
    m_TalkData.emplace(310 + 0, TalkData);

    TalkData = TEXT("조금 질기긴하지만 먹을 만하군 ");
    m_TalkData.emplace(310 + 1, TalkData);


    //TalkData = TEXT("오! 정말 약을 구해 온겐가 정말 고맙구만...");
    //m_vecTalkData.push_back(TalkData);
    //TalkData = TEXT("덕분에 살거 같네.. 이건 약을 구해준 보답일세");
    //m_vecTalkData.push_back(TalkData);
    //총을 건내 준다.


    return S_OK;
}

void CTalkManager::Priority_Tick(_float fTimeDelta)
{
}

void CTalkManager::Tick(_float fTimeDelta)
{
}

void CTalkManager::Late_Tick(_float fTimeDelta)
{
}

wstring CTalkManager::Get_Talk(_int TalkIndex)
{
    if (m_TalkData .end() == m_TalkData.find(TalkIndex))
        return TEXT("");
    else
        return m_TalkData.find(TalkIndex)->second;  
}

CTalkManager* CTalkManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTalkManager* pInstance = new CTalkManager(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed To Created : CTalkManager");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CTalkManager::Free()
{
    m_TalkData.clear();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
