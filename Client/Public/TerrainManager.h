#pragma once

#include"Client_Defines.h"
#include "Base.h"



BEGIN(Client)
class CTerrainManager :
    public CBase
{
private :
    CTerrainManager();
    virtual ~CTerrainManager() = default;

public:
    HRESULT Initialize();
    HRESULT Clone_Terrain(void* pArg);

public:
    class CComponent* Get_Terrain_Component(const wstring& ComTag);

public:
    void Terrain_Release();
private:
    class CGameInstance* m_pGameInstance = { nullptr };
    class CGameObject* m_pTerrain = { nullptr };

public:
   static CTerrainManager* Create();
    virtual void Free() ;
};
END
