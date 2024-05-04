#pragma once

#include "Component.h"

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

    _float Get_TotalWeight() {
        return m_fTotalWeight;
    }

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