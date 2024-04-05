#pragma once

#include "Base.h"

/* 현재 할당되어있느 ㄴ레벨을 들고있는다. */
/* 들고있는 레벨의 반복적인 갱신을 수행한다. */
/* 레벨의 교체 작업을 수행한다. */
/* 레벨 교체 시, 기존 레벨용 자원드릉 클리어한다. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);

public:
	/* 새로운 레벨로 교체한다. */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);

private:
	class CLevel*			m_pCurrentLevel = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

	_uint					m_iLevelIndex = { 0 };

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END