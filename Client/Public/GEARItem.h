#pragma once
#include "Client_Defines.h"
#include "Item.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CGEARItem final :
    public CItem
{
public:
	typedef struct : public CItem::ITEM_DESC {


	}GEARITEM_DESC;

protected:
	CGEARItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGEARItem(const CGEARItem& rhs);
	virtual ~CGEARItem() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();


public:
	static CGEARItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END