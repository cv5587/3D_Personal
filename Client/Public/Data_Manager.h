#pragma once
#include "Client_Defines.h"
#include "Base.h"
BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CData_Manager :
    public CBase
{
private:
	CData_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CData_Manager() = default;

	
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };

	class CTerrainManager* m_pTerrainMgr = { nullptr };
	_uint m_CurrentLevel = { 3 };
public:
	HRESULT Load_Data(_uint iLevelIndex);
public:
	HRESULT Initialize();
	static CData_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	virtual void Free() override;
};

END