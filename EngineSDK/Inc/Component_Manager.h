#pragma once

#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "Model.h"
#include "Calculator.h"

/* 컴포넌트의 원형들을 레벨별로 보관한다. */

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent* pPrototype);
	HRESULT Save_Binary(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring FilePath);
	CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg);
	void Clear(_uint iLevelIndex);

private:
	map<const wstring, CComponent*>*			m_pPrototypes = { nullptr };
	typedef map<const wstring, CComponent*>		PROTOTYPES;

	_uint									m_iNumLevels = { 0 };

private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag);

public:
	static CComponent_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END