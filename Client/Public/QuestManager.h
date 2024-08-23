#pragma once
#include "Client_Defines.h"
#include "Base.h"
BEGIN(Client)
class CQuestManager :
    public CBase
{
public:
	enum QUESTID{
		QUEST1=100,
		QUEST2=200,
		QUEST3 = 300,
		QUEST4 = 400,
		QUEST_END};
private:
    CQuestManager();
    virtual ~CQuestManager() = default;

public:
	 HRESULT Initialize();
	 void Priority_Tick(_float fTimeDelta);
	 void Tick(_float fTimeDelta);
	 void Late_Tick(_float fTimeDelta);

	 void NextQuest()
	 {
		 m_iQuestID += 100;
		 m_iQuestCheckIndex= 0;
	 }
	 void ProgressQuest()
	 {
		 m_iQuestCheckIndex += 10;
	 }

	 _int Get_QuestID() {
		 return m_iQuestID+ m_iQuestCheckIndex;
	 }
	 void Check_Quist();
	 wstring Get_QuestName();
private:
	map<_int , class CQuestData*> m_Quest; 
	_int m_iQuestID = { 100 };
	_int m_iQuestCheckIndex = { 0 };
public:
	static CQuestManager* Create();
	virtual void Free() override;

};

END