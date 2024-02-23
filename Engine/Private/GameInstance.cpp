#include "..\Public\GameInstance.h"

#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}


HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC & EngineDesc, ID3D11Device ** ppDevice, ID3D11DeviceContext ** ppContext)
{
	/* ���� ������Ʈ�� ��Ŭ���̾�Ʈ���� ����ϱ� ���� �ʼ������� �ؾ��� ���� �ʱ�ȭ�۾��� �����Ѵ�. */

	/* �׷��� ����̽��� �ʱ�ȭ�ϳ�. */
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* ��ǲ ����̽��� �ʱ�ȭ�ϳ�. */

	/* ����  ����̽��� �ʱ�ȭ�ϳ�. */

	/* ������Ʈ, ������Ʈ �Ŵ����� ����ϱ� ���� �غ� �Ѵ�.*/

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
