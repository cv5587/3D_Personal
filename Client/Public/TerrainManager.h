#pragma once
#include "Base.h"
BEGIN(Client)
class CTerrainManager :
    public CBase
{
public:
    CTerrainManager();
    virtual ~CTerrainManager() = default;

public:
    HRESULT Initialize();
    HRESULT Clone_Terrain(void* pArg);

    _matrix Get_Terrain_WorldMatrix();
    _float4* Get_Terrain_VtxPos();
    _int* Get_Terrain_UV();

private:
    class CGameInstance* m_pGameInstance = { nullptr };
    class CGameObject* m_pTerrain = { nullptr };

public:
   static CTerrainManager* Create();
    virtual void Free() ;
};
END
