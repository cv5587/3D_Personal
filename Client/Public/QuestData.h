#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CQuestData final :
	public CBase
{
public:
	typedef struct {
		wstring wQuestName;
	}QUEST_DESC;
private:
	CQuestData();
	virtual ~CQuestData() = default;

public:
	 HRESULT Initialize(void* pArg);
	 void Priority_Tick(_float fTimeDelta);
	 void Tick(_float fTimeDelta);
	 void Late_Tick(_float fTimeDelta);

	 wstring Get_QuestName() { return m_wQuestName;}
private:
	wstring m_wQuestName;

public:
	static CQuestData* Create(void* pArg);
	virtual void Free() override;
};

END