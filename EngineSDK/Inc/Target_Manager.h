#pragma once

#include "Base.h"

/*
1. 렌더타겟들을 모아서 저장한다.
2. 렌더타겟들을 장치에 바인딩한다. 다시 원상태로 복구한다.(백버퍼를 다시 장치에 바인딩한다.)
*/

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
private:
	CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);
	HRESULT Begin_MRT(const wstring& strMRTTag, _bool isClear, ID3D11DepthStencilView* pDSView = nullptr);
	HRESULT Begin_UIMRT(const wstring& strMRTTag);
	HRESULT End_MRT();
	HRESULT Bind_RenderTargetSRV(const wstring& strTargetTag, class CShader* pShader, const _char* pConstantName);
	HRESULT Copy_Resource(const wstring& strTargetTag, ID3D11Texture2D* pDesc);

	class CRenderTarget* Find_RenderTarget(const wstring& strTargetTag);
	list<class CRenderTarget*>* Find_MRT(const wstring& strMRTTag);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	ID3D11RenderTargetView* m_pBackBufferView = { nullptr };
	ID3D11DepthStencilView* m_pDepthStencilView = { nullptr };

	map<const wstring, class CRenderTarget*>			m_RenderTargets;
	map<const wstring, list<class CRenderTarget*>>		m_MRTs;

public:
	static CTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END