#pragma once

#include "Base.h"
#include <Layer.h>

/* 원형객체들을 보관한다. */
/* 특정 원형객체를 검색하여 복제하고, 복제한 객체(사본)를 레벨(동적할당)별로 그룹(Layer)지어 보관한다. */
/* 보관하고 있는 사본 객체들의 반복적인 Tick함수들을 호출해준다. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
public:
	typedef struct {
		_matrix PrePosition;
		wstring ClassTag;
		wstring ModelTag;
	}GAMEDATA_DESC;

private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex);

public:	
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg);
	//터레인 용
	HRESULT Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, CGameObject** pGameObject, void* pArg);
	HRESULT Delete_CloneObject(_uint iLevelIndex,  CGameObject* pGameObject);
	CGameObject* Find_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pGameObject);
	CGameObject* FindID_CloneObject(_uint iLevelIndex, const _int& ID);
	CGameObject* FindIndex_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, _uint iIndex);
	void Priority_Tick(_float fTimeDelta);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	void Clear(_uint iLevelIndex);
	vector< const _float4x4*>* Get_ObPos(_uint iLevelIndex, const wstring& strLayerTag);
	//파츠 오브젝트용
	class CGameObject* Clone_Object(const wstring& strPrototypeTag, void* pArg);



public:
	//UI렌더
	void Render_UI(_uint iLevelIndex, wstring LayerName);
public:
	//데이터 파싱
	HRESULT Save_Level(_uint iLevelIndex);

private:
	map<const wstring, class CGameObject*>				m_Prototypes;
	map<const wstring, class CLayer*>*					m_pLayers = { nullptr };
	_int											m_ObjectID = { 0 };
	_uint												m_iNumLevels = { 0 };

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);


public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END