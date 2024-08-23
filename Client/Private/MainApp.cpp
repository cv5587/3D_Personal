#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "Data_Manager.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "UIObject.h"

#include "Shader.h"



CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{

	ENGINE_DESC			EngineDesc={};

	EngineDesc.hWnd = g_hWnd;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.isWindowed = true;

	/* 엔진 초기화과정을 거친다. ( 그래픽디바이스 초기화과정 + 레벨매니져를 사용할 준비를 한다. ) */
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst,LEVEL_END, EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;	

	if (FAILED(Ready_Font()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_DataManager()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_GAMEPLAY)))
		return E_FAIL;
	

	return S_OK;
}

void CMainApp::Tick(float fTimeDelta)
{
#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;

#endif
	m_pGameInstance->Tick_Engine(fTimeDelta);



}

HRESULT CMainApp::Render()
{
#ifdef _DEBUG
	//++m_iNumRender;

	//if (m_fTimeAcc >= 1.f)
	//{
	//	wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumRender);

	//	m_fTimeAcc = 0.f;
	//	m_iNumRender = 0;
	//}
#endif

	/* 그린다. */
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	if (FAILED(m_pGameInstance->Draw(_float4(0.f, 0.f, 1.f, 1.f))))
		return E_FAIL;
	
#ifdef _DEBUG
	//m_pGameInstance->Render_Font(TEXT("Font_Normal14"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
#endif

	if (FAILED(m_pGameInstance->Present()))
		return E_FAIL;


	return S_OK;
}


HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID, m_pDataManager))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Icon0"), CUIObject::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{	

	/* Prototype_Component_Texture_Muzzle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Muzzle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/MuzzleFlash02.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), 
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTexBack */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTexBack"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTexBack.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTexColor*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTexColor"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTexColor.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosColor*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosColor"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosColor.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTexToggleID */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTexToggleID"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTexToggleID.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTexBrightnessToggleID */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTexBrightnessToggleID"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTexBrightnessToggleID.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_LoadingBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_LoadingBar"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_LoadingBar.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_LoadingStruggle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_LoadingStruggle"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_LoadingStruggle.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Loading */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	/* Prototype_Component_Texture_LoadLevel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadLevel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/LoadLevel/region_ToC_PleasantValley.dds"), 1))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Font()
{
	// MakeSpriteFont "넥슨lv1고딕 Bold" /FontSize:8 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 Bold8.spritefont
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Bold18"), TEXT("../Bin/Asset2D/Fonts/Bold18.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Bold14"), TEXT("../Bin/Asset2D/Fonts/Bold14.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Bold12"), TEXT("../Bin/Asset2D/Fonts/Bold12.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Bold10"), TEXT("../Bin/Asset2D/Fonts/Bold10.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Bold8"), TEXT("../Bin/Asset2D/Fonts/Bold8.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Normal14"), TEXT("../Bin/Asset2D/Fonts/Normal14.spritefont"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_DataManager()
{

	m_pDataManager = CData_Manager::Create(m_pDevice,m_pContext);

	return S_OK;
}




CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{

	Safe_Release(m_pDataManager);
	Safe_Release(m_pDataManager);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	/* 레퍼런스 카운트를 0으로만든다. */
	Safe_Release(m_pGameInstance);
	CGameInstance::Release_Engine();
}
