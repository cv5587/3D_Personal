#include "QuestManager.h"
#include "QuestData.h"
CQuestManager::CQuestManager()
{
}

HRESULT CQuestManager::Initialize()
{
	CQuestData::QUEST_DESC QuestDesc={};
	QuestDesc.wQuestName = TEXT("���Ͻ� ���� ã��");
	m_Quest.emplace(make_pair(QUEST1,CQuestData::Create(&QuestDesc)));
	QuestDesc.wQuestName = TEXT("�׻��� �����ֱ�");
	m_Quest.emplace(make_pair(QUEST2,CQuestData::Create(&QuestDesc)));
	QuestDesc.wQuestName = TEXT("�丮�� �䳢��� �����ֱ�");
	m_Quest.emplace(make_pair(QUEST3, CQuestData::Create(&QuestDesc)));
	QuestDesc.wQuestName = TEXT("������ȣ ������");
	m_Quest.emplace(make_pair(QUEST4, CQuestData::Create(&QuestDesc)));


	return S_OK;
}

void CQuestManager::Priority_Tick(_float fTimeDelta)
{
}

void CQuestManager::Tick(_float fTimeDelta)
{
}

void CQuestManager::Late_Tick(_float fTimeDelta)
{
}

CQuestManager* CQuestManager::Create()
{
	CQuestManager* pInstance = new CQuestManager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CQuestManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuestManager::Free()
{
	for (auto& pQuestData : m_Quest)
		Safe_Release(pQuestData.second);

	m_Quest.clear();
}

void CQuestManager::Check_Quist()
{

	switch (m_iQuestID)
	{
	case Client::CQuestManager::QUEST1://���� ����Ʈ�� �ܺο��� �ҷ�����.
	{
		NextQuest();	
	}
		break;
	case Client::CQuestManager::QUEST2:
	{
		if (10 == m_iQuestCheckIndex)
		{
			NextQuest();
		}
	}
		break;
	case Client::CQuestManager::QUEST3:
	{
		if (10 == m_iQuestCheckIndex)
		{
			NextQuest();
		}
	}
		break;
	case Client::CQuestManager::QUEST4:
	{
		if (10 == m_iQuestCheckIndex)
		{
			NextQuest();
		}
	}
	break;

	case Client::CQuestManager::QUEST_END:
		break;
	default:
		break;
	}


}

wstring CQuestManager::Get_QuestName()
{
	auto iter = m_Quest.find(m_iQuestID);

	return (iter->second)->Get_QuestName();
}
