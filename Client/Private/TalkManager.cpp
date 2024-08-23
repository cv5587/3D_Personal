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
    wstring TalkData = TEXT("�ű� ������! ���� ��� �����ټ� �ִ°�?");
    m_TalkData.emplace(100+0,TalkData);

    TalkData = TEXT("������ �߸� �Ծ����� ���� �ʹ� ���µ� ���� �����ټ� �ִ°�?");
    m_TalkData.emplace(100+1,TalkData);

    TalkData = TEXT("���ظ� �شٸ� �� ������ �ϰٳ� �ѹ��� �����ֽð�...");
    m_TalkData.emplace(100+2, TalkData);

    TalkData = TEXT("���� �����߳�?");
    m_TalkData.emplace(200 + 0, TalkData);

    TalkData = TEXT("���� �����ְ� ������ ���ٳ�");
    m_TalkData.emplace(200 + 1, TalkData);

    TalkData = TEXT("���� ���п� ��� ������");
    m_TalkData.emplace(210 + 0, TalkData);

    TalkData = TEXT("�̰� �����ϼ� ����ϰ� ����");
    m_TalkData.emplace(210 + 1, TalkData);

    TalkData = TEXT("���� �������� �谡 ���ļ� �׷���");
    m_TalkData.emplace(210 + 2, TalkData);

    TalkData = TEXT("�䳢 ��⵵ �����ְԳ�");
    m_TalkData.emplace(210 + 3, TalkData);



    TalkData = TEXT("����� � ������ ����");
    m_TalkData.emplace(300 + 0, TalkData);

    TalkData = TEXT("���� �־��°�?");
    m_TalkData.emplace(300 + 1, TalkData);

    TalkData = TEXT("���� ����");
    m_TalkData.emplace(310 + 0, TalkData);

    TalkData = TEXT("���� ����������� ���� ���ϱ� ");
    m_TalkData.emplace(310 + 1, TalkData);


    //TalkData = TEXT("��! ���� ���� ���� �°հ� ���� ������...");
    //m_vecTalkData.push_back(TalkData);
    //TalkData = TEXT("���п� ��� ����.. �̰� ���� ������ �����ϼ�");
    //m_vecTalkData.push_back(TalkData);
    //���� �ǳ� �ش�.


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
