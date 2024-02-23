#include "..\Public\GameInstance.h"

#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}


HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC & EngineDesc, ID3D11Device ** ppDevice, ID3D11DeviceContext ** ppContext)
{
	/* 엔진 프로젝트르 ㄹ클라이언트에서 사용하기 위해 필수적으로 해야할 여러 초기화작업을 수행한다. */

	/* 그래픽 디바이스를 초기화하낟. */
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 인풋 디바이스를 초기화하낟. */

	/* 사운드  디바이스를 초기화하낟. */

	/* 오브젝트, 컴포넌트 매니져를 사용하기 위한 준비를 한다.*/

	return S_OK;


}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	return m_pGraphic_Device->Present();
}


void CGameInstance::Free()
{
	Safe_Release(m_pGraphic_Device);
}
