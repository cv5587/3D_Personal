#pragma once
#include "Base.h"

BEGIN(Client)
class CImGuiManager :
    public CBase
{
	DECLARE_SINGLETON(CImGuiManager)
private:
	CImGuiManager();
	virtual ~CImGuiManager() = default;
public:
	HRESULT Initialize(ID3D11Device* pDevice,  ID3D11DeviceContext* pContext);
	HRESULT Tick(_float fTimeDelta);
	HRESULT Late_Tick();
	HRESULT Render();

public:
	void Make_Terrain(void* pArg);

private:
	class ID3D11Device* m_pDevice = { nullptr };
	class ID3D11DeviceContext* m_pContext = { nullptr };
	class CGui* m_pImGui = { nullptr };

public:
	virtual void Free() override;
};
END
