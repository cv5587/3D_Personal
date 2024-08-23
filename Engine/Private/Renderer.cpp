#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "GameInstance.h"

#include "Shader.h"
#include "BlendObject.h"
#include "VIBuffer_Rect.h"
_uint		g_iSizeX = 8192;
_uint		g_iSizeY = 4608;
CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize()
{
	D3D11_VIEWPORT			ViewportDesc={};
	_uint					iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	/* Target_Diffuse */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.0f, 0.f, 1.f, 0.f))))
		return E_FAIL;

	/* Target_Normal */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_Depth */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/* Target_LightDepth */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_LightDepth"), g_iSizeX, g_iSizeY, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* Target_Shade */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_Specular */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Decal */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Decal"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

// 이거 렌더링과 아무상관 없어서 오브젝트 그릴때 1번에 render ID 넣고 ui그릴때 2번에 uiid 넣게 제작 해서 보관하고 필요할때
	//이걸 꺼내쓰는식으로 만들기 디바이스 만들 필요 X
	/* Target_ID(renderID,UIID) 1번만 쓸거임*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_ObjectID"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(-1.f, -1.f, -1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_UIID"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(-1.f, -1.f, -1.f, 1.f))))
		return E_FAIL;


#pragma region MRT_Bloom

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Effect"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_X"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_X"), TEXT("Target_Blur_X"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_Y"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_Y"), TEXT("Target_Blur_Y"))))
		return E_FAIL;

#pragma endregion

#pragma region MRT_ShadowResult

	/* Target_Result */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_ShadowResult"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_ShadowResult"), TEXT("Target_ShadowResult"))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Result

	/* Target_Result */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Result"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Result"), TEXT("Target_Result"))))
		return E_FAIL;
#pragma endregion


	/* MRT_GameObjects */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_ObjectID"))))
		return E_FAIL;

	/* MRT_Decal */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Decal"), TEXT("Target_Decal"))))
		return E_FAIL;

	/* MRT_ShadowObject */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_ShadowObjects"), TEXT("Target_LightDepth"))))
		return E_FAIL;

	/* MRT_LightAcc */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;





	/* MRT_ID */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_ID"), TEXT("Target_UIID"))))
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	/* 화면을 꽉 채워주기 위한 월드변환행렬. */
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));


	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;



	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = g_iSizeX;
	TextureDesc.Height = g_iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pLightDepthStencilView)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

#ifdef _DEBUG
	if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_Diffuse"), 50.f, 50.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_Normal"), 50.f, 150.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_Depth"), 50.f, 250.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_LightDepth"), ViewportDesc.Width - 150.0f, 150.0f, 300.f, 300.f)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_ObjectID"), 50.f, 350.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_Shade"), 150.f, 50.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_Specular"), 150.f, 150.f, 100.f, 100.f)))
		return E_FAIL;

//	if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_Result"), 1180.f, 100.f, 200.f, 200.f)))
//		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_Effect"), 50.f, 350.f, 100.f, 100.f)))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_Blur_X"), 500.f, 500.f, 200.f, 200.f)))
	//	return E_FAIL;

//	if (FAILED(m_pGameInstance->Ready_RTDebug(TEXT("Target_Blur_Y"), 700.f, 500.f, 200.f, 200.f)))
	//	return E_FAIL;
#endif

	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	m_RenderGroup[eRenderGroup].push_back(pRenderObject);

 	Safe_AddRef(pRenderObject);

	return S_OK;
}

void CRenderer::Draw()
{
	Render_Priority();
	Render_ShadowObjects();
	Render_NonBlend();
	Render_LightAcc();
	Render_DeferredResult();

	Render_NonLight();
	Render_Bloom();//그림자
	Render_FinalBlend();//그림자

	Render_Blend();
	Render_UI();

#ifdef _DEBUG
//	Render_Debug();
#endif
}

void CRenderer::Clear()
{
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderGroup[i])
			Safe_Release(pGameObject);

		m_RenderGroup[i].clear();
	}
}

#ifdef _DEBUG

HRESULT CRenderer::Add_DebugComponent(CComponent* pComponent)
{
	m_DebugComponents.push_back(pComponent);

	Safe_AddRef(pComponent);

	return S_OK;
}
#endif 

void CRenderer::Render_Priority()
{
	m_pGameInstance->Begin_MRT(TEXT("MRT_Result"),true);

	for (auto& pGameObject : m_RenderGroup[RENDER_PRIORITY])
	{
		if(nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_PRIORITY].clear();		

	m_pGameInstance->End_MRT();
}

void CRenderer::Render_ShadowObjects()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_ShadowObjects"),true, m_pLightDepthStencilView)))
		return;

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)g_iSizeX;
	ViewPortDesc.Height = (_float)g_iSizeY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);


	for (auto& pGameObject : m_RenderGroup[RENDER_SHADOWOBJ])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_LightDepth();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_SHADOWOBJ].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return;

	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)1280.0f;
	ViewPortDesc.Height = (_float)720.0f;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);
}

void  CRenderer::Render_NonBlend()
{
	/* Diffuse + Normal */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"),true)))
		return;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONBLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_NONBLEND].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return;
}

void CRenderer::Render_Decal()
{	/* Diffuse + Normal */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Decal"), true)))
		return;

	for (auto& pGameObject : m_RenderGroup[RENDER_DECAL])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_DECAL].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return;
}

void CRenderer::Render_LightAcc()
{
	/* Shade */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc"),true)))
		return;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return;



	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW))))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_PROJ))))
		return;

	if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition_float4(), sizeof(_float4))))
		return;




	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return;
	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return;


	m_pGameInstance->Render_Lights(m_pShader, m_pVIBuffer);

	if (FAILED(m_pGameInstance->End_MRT()))
		return;
}

void CRenderer::Render_DeferredResult()
{
	m_pGameInstance->Begin_MRT(TEXT("MRT_Result"), false);

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_VIEW))))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::TS_PROJ))))
		return;

	_float4x4		ViewMatrix, ProjMatrix;

	/* 광원 기준의 뷰 변환행렬. */
	const LIGHT_DESC* pDesc = m_pGameInstance->Get_LightDesc(0);

	if (nullptr != pDesc)
	{
		if (pDesc->bSwitch)
			if ((pDesc->vPosition).y > 30.f)
			XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMLoadFloat4(&pDesc->vPosition), XMVectorSet(800.f, 0.f, 380.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));

	}
	else
		XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(800.f, 10.f, 400.f, 1.f), XMVectorSet(800.f, 0.f, 380.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));

	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), (_float)g_iSizeX / g_iSizeY, 0.1f, 1000.f));

	if (FAILED(m_pShader->Bind_Matrix("g_LightViewMatrix", &ViewMatrix)))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_LightProjMatrix", &ProjMatrix)))
		return;



	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return;
	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return;
	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
		return;
	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_LightDepth"), m_pShader, "g_LightDepthTexture")))//그림즈+최종
		return;
	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return;

	if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition_float4(), sizeof(_float4))))	
		return;

	_float fFogStart = 5.f;
	_float fFogEnd = 60.f;
	_float4 fFogColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pShader->Bind_RawValue("g_FogStart", &fFogStart, sizeof(_float));
	m_pShader->Bind_RawValue("g_FogEnd", &fFogEnd, sizeof(_float));
	m_pShader->Bind_RawValue("g_FogColor", &fFogColor, sizeof(_float4));


	m_pShader->Begin(3);

	m_pVIBuffer->Bind_Buffers();

	m_pVIBuffer->Render();

	m_pGameInstance->End_MRT();

}

void CRenderer::Render_Bloom()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return ;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return ;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return ;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_X"),true)))
		return ;

	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Effect"), m_pShader, "g_EffectTexture")))//그림자를 주기위해
		return ;

	m_pVIBuffer->Bind_Buffers();

	m_pShader->Begin(4);

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return ;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_Y"),true)))
		return ;

	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Blur_X"), m_pShader, "g_EffectTexture")))//그림자X 블러를 Y로 넣어줌
		return ;

	m_pVIBuffer->Bind_Buffers();

	m_pShader->Begin(5);

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return ;

}

void CRenderer::Render_FinalBlend()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return ;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return ;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return ;


	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Blur_Y"), m_pShader, "g_BlurTexture")))//최종블러텍스처
		return ;

	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Effect"), m_pShader, "g_EffectTexture")))//그림자 텍스처 원본
		return ;

	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Result"), m_pShader, "g_ResultTexture")))//원본 최종
		return ;

	m_pVIBuffer->Bind_Buffers();

	m_pShader->Begin(6);

	m_pVIBuffer->Render();


	return ;
}

void  CRenderer::Render_NonLight()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Effect"),true)))
		return ;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_NONLIGHT].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return ;
}

void  CRenderer::Render_Blend()
{
	m_RenderGroup[RENDER_BLEND].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	{
		return dynamic_cast<CBlendObject*>(pSour)->Get_CamDistance() > dynamic_cast<CBlendObject*>(pDest)->Get_CamDistance();
	});

	for (auto& pGameObject : m_RenderGroup[RENDER_BLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_BLEND].clear();
}

void  CRenderer::Render_UI()
{
	/* Diffuse + Normal */
	if (FAILED(m_pGameInstance->Begin_UIMRT(TEXT("MRT_ID"))))
		return;


 	for (auto& pGameObject : m_RenderGroup[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_UI].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return;

}

#ifdef _DEBUG
void CRenderer::Render_Debug()
{
	for (auto& pDebugCom : m_DebugComponents)
	{
		pDebugCom->Render();
		Safe_Release(pDebugCom);
	}
	m_DebugComponents.clear();

	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pGameInstance->Render_RTDebug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_RTDebug(TEXT("MRT_ShadowObjects"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_RTDebug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_RTDebug(TEXT("MRT_Effect"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_RTDebug(TEXT("MRT_ID"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_RTDebug(TEXT("MRT_Blur_X"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_RTDebug(TEXT("MRT_Blur_Y"), m_pShader, m_pVIBuffer);
}
#endif


CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRenderer*		pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
	Safe_Release(m_pGameInstance);

#ifdef _DEBUG
	for (auto& pDebugCom : m_DebugComponents)
	{
		Safe_Release(pDebugCom);
	}
	m_DebugComponents.clear();
#endif

	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderGroup[i])
			Safe_Release(pGameObject);

		m_RenderGroup[i].clear();
	}


	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pLightDepthStencilView);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
