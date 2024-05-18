#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_Basement :
    public CLevel
{
private:
	CLevel_Basement(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Basement() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

public:
	static CLevel_Basement* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};


END