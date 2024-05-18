#pragma once
#include "Client_Defines.h"
#include "Item.h"
BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CCLTH final :
	public CItem
{
public:
	typedef struct : public CItem::ITEM_DESC {


	}CLTHITEM_DESC;

protected:
	CCLTH(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCLTH(const CCLTH& rhs);
	virtual ~CCLTH() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Check_Model(void* pArg);

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();


public:
	static CCLTH* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END