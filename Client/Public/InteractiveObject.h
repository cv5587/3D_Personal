#pragma once
#include "Client_Defines.h"
#include "Item.h"
#include "Player.h"
BEGIN(Client)
class CInteractiveObject abstract :
    public CItem
{
public:
	typedef struct INTERACTIVEOBJECT_DESC : public CItem::ITEM_DESC
	{

	}INTERACTIVEOBJECT_DESC;
protected:
	CInteractiveObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInteractiveObject(const CGameObject& rhs);
	virtual ~CInteractiveObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _bool IntersectRay(_vector* pRayArray, _float* fDist) override;
	
	virtual HRESULT Action() = 0;


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END