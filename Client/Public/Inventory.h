#pragma once
#include "Base.h"
#include "Item.h"

BEGIN(Engine)
class GameInstance;
END

BEGIN(Client)
class CInventory final:
    public CBase
{
private:
    CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CInventory(const CInventory& rhs);

public:
     HRESULT Initialize();

public:
    HRESULT Add_Item(CItem* pItem);

private:
   vector<class CItem*> m_Inventory ;  

    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
   class CGameInstance* m_pGameInstance = {nullptr   };

private:
    CItem* find_Item(CItem* pItem);
public:
    static CInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free()override;
};
END
