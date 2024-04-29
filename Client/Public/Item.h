#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CItem :
    public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC {
		_uint iQuantity;
		_uint ItemType;
		wstring ItemName;
	}ITEM_DESC;

	enum ITEMTYPE {
		ITEM_KINDLING, 
		ITEM_MEDKITS, 
		ITEM_CLOTH, 
		ITEM_FOOD,
		ITEM_EQUIPMENT, 
		ITEM_STUFF,
		ITEM_END
	};

protected:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CGameObject& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool Type_Compare(CItem::ITEMTYPE Itemtype);

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

protected:
	ITEMTYPE m_ItemType = { ITEM_END };
	_uint			 m_Quantity = {1};
	wstring		m_ItemName = { TEXT("") };

public:
	static CItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END