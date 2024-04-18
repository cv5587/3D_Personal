#pragma once

#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	class CComponent* Get_Component(const wstring& strComponentTag, _uint iIndex);


public:
	HRESULT Initialize();
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	HRESULT Delete_GameObject( class CGameObject* pGameObject);
	CGameObject* Find_GameObject(class CGameObject* pGameObject);
	CGameObject* Find_GameObject(const _int& ID);
	void Priority_Tick(_float fTimeDelta);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	vector< const _float4x4*>* Get_ObPos(){
		return &m_ObPosition;
	}

public:
	HRESULT Save_Data(ofstream* fout);	
private:
	list<class CGameObject* >		m_GameObjects;
	vector< const _float4x4*>		m_ObPosition;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END