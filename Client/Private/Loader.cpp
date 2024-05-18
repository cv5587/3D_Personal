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
#include "GEAR.h"
#include "CLTH.h"	
#include "UImanager.h"
#include "PickRabbit.h"
#include"UIClothToggle.h"


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

	/**************************************픽업셀렉터*******************************************/

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

	/**************************************인벤토리*******************************************/

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

	/**************************************인벤토리 토글 아이콘*******************************************/
		/* For.Prototype_Component_Texture_InventoryToggleState */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventoryToggleState"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Toggle/Toggle_State%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_InventoryToggleEquip */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventoryToggleEquip"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Toggle/Toggle_Equip%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_InventoryToggleBag */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventoryToggleBag"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Toggle/Toggle_Bag%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_InventoryToggleBlueprint */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventoryToggleBlueprint"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Toggle/Toggle_Blueprint%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_InventoryItemCase */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventoryItemCase"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Inven_ItemCase%d.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_InventoryPickLight */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventoryPickLight"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Inven_PickLight%d.dds"),1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_InventoryDetailCase*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InventoryDetailCase"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Inven_ItemDetailCase%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Inven_ClothEquip*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Inven_ClothEquip"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Inventory/Inven_ClothEquip.dds")))))
		return E_FAIL;
	/**************************************상태창*******************************************/

	///옷 + 몸
		/* For.Prototype_Component_Texture_PaperDoll_Fullbody*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_PaperDoll_Fullbody"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/State/PaperDoll_Fullbody.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Toque_Wool*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Toque_Wool"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/State/CLTH_HED_Toque_Wool.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cargo_pants*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Cargo_pants"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/State/CLTH_LEG_Cargo_pants.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_WillBoots*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_WillBoots"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/State/CLTH_TOR_WillBoots.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_WillSweater*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_WillSweater"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/State/CLTH_TOR_WillSweater.dds"), 1))))
		return E_FAIL;

	/**************************************상태창 기본 아이콘들*******************************************/

		/* For.Prototype_Component_Texture_State_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_State_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/State/State_Icon%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_State_Iconback*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_State_Iconback"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/State/State_Iconback%d.dds"), 4))))
		return E_FAIL;

	/**************************************인벤Cloth 옷 케이스*******************************************/

	/* For.Prototype_Component_Texture_InvenCloth_Left*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InvenCloth_Left"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/InvenCloth_Left.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_InvenCloth_Right*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_InvenCloth_Right"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/InvenCloth_Right.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clothcase_glove*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Clothcase_glove"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/ClothCase/Clothcase_glove.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clothcase_hat*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Clothcase_hat"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/ClothCase/Clothcase_hat.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clothcase_Inpant*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Clothcase_Inpant"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/ClothCase/Clothcase_Inpant.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clothcase_jacket*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Clothcase_jacket"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/ClothCase/Clothcase_jacket.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clothcase_neck*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Clothcase_neck"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/ClothCase/Clothcase_neck.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clothcase_Outpant*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Clothcase_Outpant"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/ClothCase/Clothcase_Outpant.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clothcase_shirts*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Clothcase_shirts"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/ClothCase/Clothcase_shirts.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clothcase_shoes*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Clothcase_shoes"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/ClothCase/Clothcase_shoes.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clothcase_socks*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Clothcase_socks"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/InvenCloth/ClothCase/Clothcase_socks.dds")))))
		return E_FAIL;
	/**************************************아이템 아이콘*******************************************/
	
		/* For.Prototype_Component_Texture_ico_GearItem_Stone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Stone"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_Stone.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_ToqueWool */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_ToqueWool"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_BasicWoolHat.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_CargoPants */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Cargopants"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_CargoPants.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_Stick */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Stick"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_Stick.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_WaterSupply */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_WaterSupply"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_WaterSupply.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_WillBoots*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Boots"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_WillBoots.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_WillSweater */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Sweater"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_WillSweater.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_Rabbit*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Rabbit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_Rabbit.dds")))))
		return E_FAIL;

		/********************************************눈 텍스처***********************************************************/
	/*For. Ground_Snow*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow_Ground_B"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Terrain/Ground_Snow/TRN_Snow_Ground_B.dds")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Terrain/Heightmap/heightmap_lakeregion.bmp")))))
		return E_FAIL;

	//절벽 바위
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
	
	//장비 토끼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PickRabbit"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PickRabbit.bin"))))
		return E_FAIL;

	//큰바위A
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RockBigA"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_RockBigA.bin"))))
		return E_FAIL;

	//큰바위B
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RockBigB"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_RockBigB.bin"))))
		return E_FAIL;

	//큰바위C
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RockBigC"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_RockBigC.bin"))))
		return E_FAIL;

	//큰바위D
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RockBigD"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_RockBigD.bin"))))
		return E_FAIL;


	//통나무 A~E
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

	//나무 A~N

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


	//***************** 옷 ************************************************
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ToqueWool"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_ToqueWool.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cargopants"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Cargopants.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boots"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Boots.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sweater"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Sweater.bin"))))
		return E_FAIL;

	//******************************************동굴
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cave"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Cave.bin"))))
		return E_FAIL;

	//******************************************농장집
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_FarmHouse"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_FarmHouse.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_FarmHouseBasementDoor"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_FarmHouseBasementDoor.bin"))))
		return E_FAIL;
	//******************************************쉐이더********************

	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로딩 중 입니다."));
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

	/* For.Prototype_Component_Shader_VtxAnimMeshMonster */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMeshMonster"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("충돌체 원형을 로딩 중 입니다."));

	/* For.Prototype_Component_Collider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		CCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("네비게이션 원형을 로딩 중 입니다."));
	/*For.Prototypr_Component_Navigation*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/bin/NavigationData.bin")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("인벤토리 원형을 로딩 중 입니다."));
	/*For.Prototypr_Component_Inventory*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Inventory"),
		CInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));

	/* For.Prototype_GameObject_UIObject*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIObject"), 
		CUIObject::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

	/* For.Prototype_GameObject_UIBack*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIBack"),
		CUIBack::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIColor*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIColor"),
		CUIColor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIText */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIText"),
		CUITEXT::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIToggle */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIToggle"),
		CUIToggleID::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UISortToggle */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UISortToggle"),
		CUISortToggle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIButton*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIButton"),
		CUIButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIItemIcon*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIItemIcon"),
		CUIItemIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIItemToggle*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIItemToggle"),
		CUIItemToggle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIClothToggle*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIClothToggle"),
		CUIClothToggle::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_UIStateCloth*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIStateCloth"),
		CUIStateCloth::Create(m_pDevice, m_pContext))))
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

	/* For.Prototype_GameObject_PickRabbit */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PickRabbit"),
		CPickRabbit::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_GEAR */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GEAR"),
		CGEAR::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_CLTH */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CLTH"),
		CCLTH::Create(m_pDevice, m_pContext))))
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
