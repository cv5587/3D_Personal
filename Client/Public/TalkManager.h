#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CTalkManager final:
    public CBase
{
private:
	CTalkManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTalkManager() = default;

public:
	HRESULT Initialize();
	void Priority_Tick(_float fTimeDelta);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);

public:
	wstring Get_Talk(_int TalkIndex);
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	map <_int, wstring> m_TalkData;

public:
	static CTalkManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};
END
