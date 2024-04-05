#pragma once

#include "Base.h"

BEGIN(Engine)

/* 1. ID3D11Device, ID3D11DeviceContext라는 객체를 우선 생성한다. */
/* 2. IDXGISwapChain를 생성한다.(백버퍼(ID3D11Texture2D)도 같이 생성한거야.) */
/* 3. 백버퍼뷰를 생성한다. */
/* 4. 깊이버퍼 텍스쳐를 생성하고 깊이버퍼 뷰를 생성한다. */

class CGraphic_Device final : public CBase
{	
public:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:

	/* 그래픽 디바이스의 초기화. */
	HRESULT Initialize(const ENGINE_DESC& EngineDesc, 
		_Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);

	/* 백버퍼를 지운다. */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);

	/* HitScreen을 지운다. */
	HRESULT Clear_HitScreenBuffer_View();

	/* HitScreen을 지운다. */
	HRESULT Clear_IDScreenBuffer_View();

	template<typename T>
	HRESULT Clear_Texture(TextureType eTextureType, T* data);

	/* 깊이버퍼 + 스텐실버퍼를 지운다. */
	HRESULT Clear_DepthStencil_View();
	
	/* 후면 버퍼를 전면버퍼로 교체한다.(백버퍼를 화면에 직접 보여준다.) */
	HRESULT Present();

public:
	_float Compute_ProjZ(const POINT& ptWindowPos, ID3D11Texture2D* pHitScreenTexture);
	_int	Compute_ID(const POINT& ptWindowPos, ID3D11Texture2D* pIDScreenTexture);
private:	
	// IDirect3DDevice9* == LPDIRECT3DDEVICE9 == ID3D11Device + ID3D11DeviceContext 

	/* 메모리 할당. (정점버퍼, 인덱스버퍼, 텍스쳐로드) 컴객체의 생성과 관련된 역할 */
	ID3D11Device*				m_pDevice = { nullptr };
		
	/* 기능실행.(바인딩작업, 정점버퍼를 SetStreamSource(), SetIndices(), SetTransform(), SetTexture() */
	/* 그린다. */
	ID3D11DeviceContext*		m_pDeviceContext = { nullptr };

	/* 후면버퍼와 전면버퍼를 교체해가면서 화면에 보여주는 역할 */
	IDXGISwapChain*				m_pSwapChain = { nullptr };

	ID3D11Texture2D* m_pHitScreenTexture = { nullptr };

	ID3D11Texture2D* m_pIDScreenTexture = { nullptr };



	/* IDirect3DTexture9 */
	/* ID3D11Texture2D : 텍스쳐를 표현하는 사전객체 타입이다.
	why? 용도에 맞는 실질적으로 사용하기 위한 텍스쳐객체를 만들어내기위해.  */	

	/* ID3D11ShaderResourceView : 셰이더에 전달될 수 있는 텍스처 타입. */	
	/* ID3D11RenderTargetView : 렌더타겟용으로 사용될 수 있는 텍스처 타입. */
	/* ID3D11DepthStencilView : 깊이스텐실 버퍼로서 사용될 수 있는 타입.  */
	ID3D11RenderTargetView*		m_pBackBufferRTV = { nullptr };
	ID3D11RenderTargetView*		m_pHitScreenRTV = { nullptr };
	ID3D11RenderTargetView*		m_pIDScreenRTV = { nullptr };
	ID3D11DepthStencilView*		m_pDepthStencilView = { nullptr };
	D3D11_TEXTURE2D_DESC		m_HitTextureDesc{};
	D3D11_TEXTURE2D_DESC		m_IDTextureDesc{};
private:
	/* 스왑체인에게 필수적으로 필요한 데이터는 백버퍼가 필요하여 백버퍼를 생성하기위한 정보를 던져준다. */
	/* 스왑체인을 만들었다 == 백버퍼(텍스쳐)가 생성된다. */
	HRESULT Ready_SwapChain(HWND hWnd, _bool isWindowed, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_HitScreenRenderTargetView(_uint iWinCX, _uint iWinCY);
	HRESULT Ready_IDScreenRenderTargetView(_uint iWinCX, _uint iWinCY);
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

public:
	static CGraphic_Device* Create(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDevice,
		_Out_ ID3D11DeviceContext** ppDeviceContextOut);
	virtual void Free() override;
};

END

//템플릿 정의는 항상 inline으로 해줘야함.
template<typename T>
HRESULT CGraphic_Device::Clear_Texture(TextureType eTextureType, T* data)
{
	D3D11_TEXTURE2D_DESC* pTextureDesc = { nullptr };
	ID3D11Texture2D* pTexture = { nullptr };

	if (eTextureType == TextureType::HitPosTexture)
	{
		pTextureDesc = &m_HitTextureDesc;
		pTexture = m_pHitScreenTexture;
	}
	else if(eTextureType == TextureType::HitIDTexture)
	{
		pTextureDesc = &m_IDTextureDesc;
		pTexture = m_pIDScreenTexture;
	}


	switch ((*pTextureDesc).Usage)
	{
	case D3D11_USAGE_DEFAULT:
	{
		//윈도우 초기화 과정 ( 범위 지정 부분)
		D3D11_BOX box;
		box.left = 0;
		box.right = (*pTextureDesc).Width;
		box.top = 0;
		box.bottom = (*pTextureDesc).Height;
		box.front = 0;
		box.back = 1;
		//constexpr 메모리 할당을 하지 않고 바로 결과식을 내기위한 함수==컴파일 타이밍에 결과값을 알수 있따.
		if (eTextureType == TextureType::HitPosTexture && constexpr(std::is_same<T, float>::value))
		{
			float* pData = new float[(*pTextureDesc).Width * (*pTextureDesc).Height];
			float val = *reinterpret_cast<float*>(data);
			for (_uint i = 0; i < (*pTextureDesc).Width * (*pTextureDesc).Height; i++)
			{
				pData[i] = val;
			}

			m_pDeviceContext->UpdateSubresource(pTexture, 0, &box, pData, (*pTextureDesc).Width * sizeof(float), 0);

			delete[] pData;
		}
		else if (eTextureType == TextureType::HitIDTexture && constexpr(std::is_same<T, int>::value))	
		{
			int* pData = new int[(*pTextureDesc).Width * (*pTextureDesc).Height];
			int val = *reinterpret_cast<int*>(data);
			for (_uint i = 0; i < (*pTextureDesc).Width * (*pTextureDesc).Height; i++)
			{
				pData[i] = val;
			}

			m_pDeviceContext->UpdateSubresource(pTexture, 0, &box, pData, (*pTextureDesc).Width * sizeof(int), 0);

			delete[] pData;
		}
		else
		{
			MSG_BOX("T Type Is Not Same As Texture Data Type");
			return E_FAIL;
		}
	}
	break;
	default:
		MSG_BOX("textureDesc Usage Is Not Default Usage");
		return E_FAIL;
	}

	return S_OK;
}
