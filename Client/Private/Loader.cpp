#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Terrain.h"
#include "FreeCamera.h"
#include "Monster.h"
#include "EnvironmentObject.h"
#include "Player.h"
#include "Body_Player.h"
#include "Weapon.h"
#include "Player_Camera.h"
#include "Revolver.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED)))
		return E_FAIL;

	CLoader* pLoader = (CLoader*)pArg;

	if (FAILED(pLoader->Loading()))
		return 1;

	CoUninitialize();

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT			hr{};

	switch (m_eNextLevel)
	{
	case LEVEL_LOGO:
		hr = Loading_For_LogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlayLevel();
		break;
	}

	LeaveCriticalSection(&m_Critical_Section);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_LogoLevel()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));


	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));	


	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로딩 중 입니다."));	


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlayLevel()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	/*For. Ground_Snow*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow_Ground_B"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Terrain/Ground_Snow/TRN_Snow_Ground_B.dds")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Terrain/Heightmap/heightmap_ravine.bmp")))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Fiona.bin"))))
	//	return E_FAIL;
	//돌
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CliffA"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_CliffA.bin"))))
		return E_FAIL;
	//토끼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rabbit"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Rabbit.bin"))))
		return E_FAIL;
	//플레이어
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Player.bin"))))
		return E_FAIL;
	//리볼버
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Revolver"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Revolver.bin"))))
		return E_FAIL;
	//돌
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stone"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Stone.bin"))))
		return E_FAIL;
	
	_matrix		PreTransformMatrix;
	///* For.Prototype_Component_Model_Fiona */
	//PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", PreTransformMatrix))))
	//	return E_FAIL;
	// /*ForkLift*/
	//PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM,CAnimation::OBJ_ENVIRONMENT, "../Bin/Resources/Models/ForkLift/ForkLift.fbx", PreTransformMatrix))))
	//	return E_FAIL;
	///*CliffA*/
	//PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CliffA"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, CAnimation::OBJ_ENVIRONMENT, "../Bin/Asset2D/EnvironmentObject/CliffA/Cliff_Climbable_5m_v3.fbx", PreTransformMatrix))))
	//	return E_FAIL;
	///*토끼*/
	//PreTransformMatrix= XMMatrixRotationY(XMConvertToRadians(180.0f));
	//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rabbit"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, CAnimation::OBJ_MONSTER, "../Bin/Asset2D/Monster/Rabbit/WILDLIFE_Rabbit_fix.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	//	/*플레이어*/
	//PreTransformMatrix = XMMatrixIdentity()* XMMatrixRotationY(XMConvertToRadians(180.0f));
	//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, CAnimation::OBJ_PLAYER, "../Bin/Asset2D/Player/NEW_FPHand_Rig_fix.fbx", PreTransformMatrix))))
	//	return E_FAIL;


	//장비류

		//	/*라이플*/
	//PreTransformMatrix=XMMatrixIdentity();	
	//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rifle"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Asset2D/Equipment/Rifle/rifle_rig.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	//			/*리볼버*/
	//PreTransformMatrix=XMMatrixIdentity();
	//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Revolver"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, CAnimation::OBJ_WEAPON,"../Bin/Asset2D/Equipment/Revolver/FPH_Revolver_44Mag_Rig.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	///*돌*/
	//PreTransformMatrix =  XMMatrixIdentity();
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stone"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, CAnimation::OBJ_WEAPON, "../Bin/Asset2D/Equipment/Stone/GEAR_Rock.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로딩 중 입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMeshID */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshID"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMeshID.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FreeCamera*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FreeCamera"),
		CFreeCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_PlayerCamera*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerCamera"),
		CPlayer_Camera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cliff */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EnvironmentObject"),
		CEnvironmentObject::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Body_Player"),
		CBody_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon"),
		CWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Revolver */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Revolver"),
		CRevolver::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevel)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed To Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
