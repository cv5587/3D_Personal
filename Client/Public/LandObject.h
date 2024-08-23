#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CLandObject abstract : public CGameObject
{
public:
	typedef struct LANDOBJ_DESC : public CGameObject::GAMEOBJECT_DESC
	{

	}LANDOBJ_DESC;
protected:
	CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	/* 태우고자하는 객체의 월드위치. * 지형월드역*/
	/* 지형버퍼의 정점들의 로컬위치. */


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END