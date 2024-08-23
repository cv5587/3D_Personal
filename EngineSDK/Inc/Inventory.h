#pragma once

#include "Component.h"
#include "ItemData.h"
BEGIN(Engine)
class ENGINE_DLL CInventory final
    : public CComponent
{
protected:
    CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CInventory(const CInventory& rhs);
    virtual ~CInventory() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg) override;

public:
    HRESULT Add_Item(class CItem* pItem);   
    HRESULT Add_Rabbit();
    HRESULT Add_RabbitMeat();
    HRESULT Add_RabbitRut();
    HRESULT Add_RabbitPelt();
    HRESULT Add_BedRoll();
    HRESULT Add_Knife();
    HRESULT Drop_Item(wstring ItemName, void* pDesc);

    _bool Check_QuestItem(wstring ItemName);
    _bool Check_StoveItem(wstring ItemName, void* pDesc);

    _float Get_TotalWeight() {
        return m_fTotalWeight;
    }
    _uint Get_LastIndex()
    {
        return (_uint)m_Inventory.size() - 1;
    }
    vector<class CItemData*> Get_vecItemData()
    {
        return m_Inventory;
    }
    void Using_Item(wstring strName);
private:
    vector<class CItemData*> m_Inventory;
    _float                                  m_fTotalWeight = { 0.f };

private:
    class CItemData* find_Item(wstring ItemName);

public:
    static CInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);    
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free();
};
END
