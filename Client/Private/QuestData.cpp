#include "QuestData.h"

CQuestData::CQuestData()
{
}

HRESULT CQuestData::Initialize(void* pArg)
{
	QUEST_DESC* pDesc = static_cast<QUEST_DESC*>(pArg);
	m_wQuestName = pDesc->wQuestName;

	return S_OK;
}

void CQuestData::Priority_Tick(_float fTimeDelta)
{
}

void CQuestData::Tick(_float fTimeDelta)
{
}

void CQuestData::Late_Tick(_float fTimeDelta)
{
}

CQuestData* CQuestData::Create(void* pArg)
{
	CQuestData* pInstance = new CQuestData();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CQuestData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuestData::Free()
{
}
