#include "NPCManager.h"
#include "GameInstance.h"
#include"TalkManager.h"
#include "QuestManager.h"
#include "Inventory.h"
#include "Data_Manager.h"
CNPCManager::CNPCManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice{ pDevice }
	,m_pContext{ pContext }
	,m_pGameInstance {CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CNPCManager::Initialize(void* pArg)
{
	NPC_DESC* pDesc = static_cast<NPC_DESC*>(pArg);

	m_pUImanager = pDesc->pUImanager;
	Safe_AddRef(m_pUImanager);

	m_pPlayerInventory = pDesc->pPlayerInventory;
	Safe_AddRef(m_pPlayerInventory);

	m_pDatamanager = CData_Manager::Create(m_pDevice, m_pContext);



	m_pQuestManager = CQuestManager::Create();

	m_pTalkManager = CTalkManager::Create(m_pDevice, m_pContext);

	m_QuestpText =dynamic_cast<CUITEXT*>(m_pGameInstance->FindIndex_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_QuestBar"), 1));
	//Safe_AddRef(m_QuestpText);

	return S_OK;
}

HRESULT CNPCManager::Tick(_float fTimeDelta)
{
	switch (m_pQuestManager->Get_QuestID())
	{
	case Client::CQuestManager::QUEST1://다음 퀘스트는 왜부에서 불러주자.
	{

	}
	break;
	case Client::CQuestManager::QUEST2:
	{
		if (m_pPlayerInventory->Check_QuestItem(TEXT("AntiBioticsBottle")))
			m_pQuestManager->ProgressQuest();
	}
		break;
	case Client::CQuestManager::QUEST3:
	{
		if (m_pPlayerInventory->Check_QuestItem(TEXT("CookedRabbit")))
		{
			m_pQuestManager->ProgressQuest();
			m_pDatamanager->Quest_Load(LEVEL_GAMEPLAY);
		}
	}
		break;
	case Client::CQuestManager::QUEST4:
	{
		
	}
	break;
	case Client::CQuestManager::QUEST_END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CNPCManager::Late_Tick(_float fTimeDelta)
{
	//아이템 구해오면 아래 함수 실행
	//m_pQuestManager->ProgressQuest();

	m_pUImanager->Render_UI(CUImanager::Layer_QuestBar);
	return S_OK;
}

_bool CNPCManager::Talk(_int TalkIndex)
{

	_int iQuestTalkIndex =m_pQuestManager->Get_QuestID();

	wstring TalkData = m_pTalkManager->Get_Talk(TalkIndex+iQuestTalkIndex);
	if (0 == TalkData.length())
	{
		m_pQuestManager->Check_Quist();	
		
		return false;
	}
	else
	{
		m_QuestpText->Set_Text(TalkData);
		return true;
	}
}

wstring CNPCManager::Get_CurrentQuestName()
{
	return m_pQuestManager->Get_QuestName();
}


CNPCManager* CNPCManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CNPCManager* pInstance = new CNPCManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CNPCManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNPCManager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);

	Safe_Release(m_pDatamanager);
	Safe_Release(m_QuestpText);

	Safe_Release(m_pQuestManager);
	Safe_Release(m_pTalkManager);
	Safe_Release(m_pUImanager);
	Safe_Release(m_pPlayerInventory);
}
