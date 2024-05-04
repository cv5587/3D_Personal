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
#include "GEARStone.h"
#include "UIObject.h"
#include "UITEXT.h"

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
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε� �� �Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε� �� �Դϴ�."));	


	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε� �� �Դϴ�."));	


	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlayLevel()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε� �� �Դϴ�."));

	/* For.Prototype_Component_Texture_BASESelectorIcon */ 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BASESelectorIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Selector/ICON_BASEInfo%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KEYSelectorIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_KEYSelectorIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Selector/ICON_KEYInfo%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TYPESelectorIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_TYPESelectorIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Selector/ICON_TYPEInfo%d.dds"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Inventoryback */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Inventoryback"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Inven_back%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Inventorybutton */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Inventorybutton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Inven_button%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Inventorycase */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventoryCase"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Inven_Case.dds"),1))))	
		return E_FAIL;

	/* For.Prototype_Component_Texture_InventoryDetail */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventoryDetail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Inven_Detail%d.dds"), 8))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_InventorySelect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventorySelect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Inven_Select.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_InventoryWeightbar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventoryWeightbar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Inven_Weightbar%d.dds"), 2))))
		return E_FAIL;

	/*******************************************************************************************************/
	
	/*For. Ground_Snow*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow_Ground_B"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Terrain/Ground_Snow/TRN_Snow_Ground_B.dds")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε� �� �Դϴ�."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Terrain/Heightmap/heightmap_lakeregion.bmp")))))
		return E_FAIL;

	//��
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CliffA"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_CliffA.bin"))))
		return E_FAIL;
	//�䳢
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rabbit"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Rabbit.bin"))))
		return E_FAIL;
	//�÷��̾�
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Player.bin"))))
		return E_FAIL;
	//������
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Revolver"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Revolver.bin"))))
		return E_FAIL;
	//��
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stone"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Stone.bin"))))
		return E_FAIL;
	

	//ū����A
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RockBigA"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_RockBigA.bin"))))
		return E_FAIL;

	//ū����B
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RockBigB"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_RockBigB.bin"))))
		return E_FAIL;

	//ū����C
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RockBigC"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_RockBigC.bin"))))
		return E_FAIL;

	//ū����D
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RockBigD"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_RockBigD.bin"))))
		return E_FAIL;


	//�볪�� A~E
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeLogA"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeLogA.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeLogB"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeLogB.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeLogC"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeLogC.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeLogD"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeLogD.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeLogE"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeLogE.bin"))))
		return E_FAIL;

	//���� A~N

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleA"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleA.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleB"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleB.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleC"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleC.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleD"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleD.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleE"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleE.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleF"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleF.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleG"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleG.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleH"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleH.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleI"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleI.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleJ"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleJ.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleK"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleK.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleL"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleL.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleM.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeSingleN"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeSingleN.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PinTreeRootA"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PinTreeRootA.bin"))))
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
	///*�䳢*/
	//PreTransformMatrix= XMMatrixRotationY(XMConvertToRadians(180.0f));
	//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rabbit"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, CAnimation::OBJ_MONSTER, "../Bin/Asset2D/Monster/Rabbit/WILDLIFE_Rabbit_fix.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	//	/*�÷��̾�*/
	//PreTransformMatrix = XMMatrixIdentity()* XMMatrixRotationY(XMConvertToRadians(180.0f));
	//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, CAnimation::OBJ_PLAYER, "../Bin/Asset2D/Player/NEW_FPHand_Rig_fix.fbx", PreTransformMatrix))))
	//	return E_FAIL;


	//����

		//	/*������*/
	//PreTransformMatrix=XMMatrixIdentity();	
	//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rifle"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Asset2D/Equipment/Rifle/rifle_rig.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	//			/*������*/
	//PreTransformMatrix=XMMatrixIdentity();
	//	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Revolver"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, CAnimation::OBJ_WEAPON,"../Bin/Asset2D/Equipment/Revolver/FPH_Revolver_44Mag_Rig.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	///*��*/
	//PreTransformMatrix =  XMMatrixIdentity();
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stone"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, CAnimation::OBJ_WEAPON, "../Bin/Asset2D/Equipment/Stone/GEAR_Rock.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε� �� �Դϴ�."));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMeshTree */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshTree"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMeshTree.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMeshID */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshID"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMeshID.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMeshTreeID */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshTreeID"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMeshTreeID.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�浹ü ������ �ε� �� �Դϴ�."));

	/* For.Prototype_Component_Collider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		CCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�׺���̼� ������ �ε� �� �Դϴ�."));
	/*For.Prototypr_Component_Navigation*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/bin/NavigationData.bin")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�κ��丮 ������ �ε� �� �Դϴ�."));
	/*For.Prototypr_Component_Inventory*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Inventory"),
		CInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("��ü������ �ε� �� �Դϴ�."));

	/* For.Prototype_GameObject_SelectorIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SelectorIcon"), 
		CUIObject::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

	/* For.Prototype_GameObject_UIText */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIText"),
		CUITEXT::Create(m_pDevice, m_pContext))))
		return E_FAIL;

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

	/* For.Prototype_GameObject_GEARStone */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GEARStone"),
		CGEARStone::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

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