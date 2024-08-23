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
#include "Portal.h"
#include "Mother.h"	
#include "Rifle.h"
#include "BoxProp.h"
#include"Stove.h"
#include "UIActiveButton.h"
#include "Match.h"
#include "UIStoveItem.h"
#include "Rabbit.h"
#include "Wolf.h"
#include "BedRoll.h"
#include "UILoadingStruggle.h"
#include "Particle_Rect.h"
#include "Particle_Point.h"
#include "Explosion.h"
#include "RevolverFire.h"
#include "Sky.h"
#include "Flare.h"
#include "UIBluePrintToggle.h"
#include"UIBlood.h"
#include "Trail_Effect.h"

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
	/* Prototype_Component_Texture_Explosion */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Explosion"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
		return E_FAIL;

	/* Prototype_Component_Texture_Fire */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Fire"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/FireEffect/FireEffect%d.dds"), 256))))
		return E_FAIL;

	/* Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Sky/Sky_%d.dds"), 3))))
		return E_FAIL;

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
	/**************************************스토브 기본 아이콘들*******************************************/
		/* For.Prototype_Component_Texture_Stove_Button*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Stove_Button"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Stove/Stove_Button.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stove_Duration*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Stove_Duration"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Stove/Stove_Duration.dds")))))
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
	
			/* For.Prototype_Component_Texture_ico_GearItem_Knife */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Knife"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_Knife.dds")))))
		return E_FAIL;

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

	/* For.Prototype_Component_Texture_ico_GearItem_BeefJerky*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_BeefJerky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_BeefJerky.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_CookedRabbit*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_CookedRabbit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_CookedRabbit.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_RawRabbit*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_RawRabbit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_RawRabbit.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_Pelt*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Pelt"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_Pelt.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_Rut*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Rut"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_Rut.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_AntiBioticsBottle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_AntiBioticsBottle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_AntiBioticsBottle.dds")))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_ico_GearItem_Matchbox*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Matchbox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_Matchbox.dds")))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_ico_GearItem_Hardwood*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Hardwood"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_Hardwood.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_BedRoll*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_BedRoll"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_BedRoll.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ico_GearItem_Flare*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ico_GearItem_Flare"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Item/ico_GearItem_Flare.dds")))))
		return E_FAIL;
	

	/**************************************로딩바*******************************************/

	/* For.Prototype_Component_Texture_LoadingBar_Back*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_LoadingBar_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/LoadingBar/LoadingBar_Back.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_LoadingBar_Current*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_LoadingBar_Current"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/LoadingBar/LoadingBar_Current.dds")))))
		return E_FAIL;
	/**************************************퀘스트 대화상자*******************************************/

	/* For.Prototype_Component_Texture_QuestBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_QuestBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/QuestBar/QuestBar.dds")))))
		return E_FAIL;

	/**************************************물림 액션 게이지바*******************************************/

/* For.Prototype_Component_Texture_Struggle_Back*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Struggle_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Struggle/Struggle_Back.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Struggle_Bar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Struggle_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Struggle/Struggle_Bar.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Struggle_Hand*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Struggle_Hand"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Struggle/Struggle_Hand.dds")))))
		return E_FAIL;

		/********************************************눈 텍스처***********************************************************/
	/*For. Ground_Snow*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow_Ground_B"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Terrain/Ground_Snow/TRN_Snow_Ground_B.dds")))))
		return E_FAIL;

	/********************************************모래 시계***********************************************************/
/*For. Prototype_Component_Texture_Time*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Time"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Time/Time.dds")))))
		return E_FAIL;

	/********************************************파티클 텍스처***********************************************************/
	/* Prototype_Component_Texture_SnowFlake */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SnowFlake"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/FX_SnowFlakeA.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Dust */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Dust"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/FX_DustA.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Breath */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Breath"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/FX_MenuParticleA.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Fog */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Fog"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/Fog.dds"), 1))))
		return E_FAIL;

	
		/* Prototype_Component_Texture_FlareSmoke */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FlareSmoke"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/FX_FlareSmoke.dds"), 1))))
			return E_FAIL;

	/* Prototype_Component_Texture_Radial */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Radial"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/FX_RadialGradient.dds"), 1))))
		return E_FAIL;


	/* Prototype_Component_Texture_Ver */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Ver"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/FX_VerGradient.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_FootStep */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FootStep"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/FootStep.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Blood */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Blood"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/Blood.dds"), 1))))
		return E_FAIL;


	/* Prototype_Component_Texture_Trail */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Trail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/078_trc_n.dds"), 1))))
		return E_FAIL;
	

	///* Prototype_Component_Texture_Fire */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Fire"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Particle/FX_FireSprite.dds"), 1))))
	//	return E_FAIL;

	/**************************************퀘스트 대화상자*******************************************/

/* For.Prototype_Component_Texture_Sleep*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sleep"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Sleep/Sleep_Icon.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_QuestName*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_QuestName"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/QuestBar/QuestName.dds")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Asset2D/Textures/Terrain/Heightmap/heightmap_lakeregion.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Trail */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Trail"),
		CVIBuffer_Trail::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//절벽 바위
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CliffA"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_CliffA.bin"))))
		return E_FAIL;
	//토끼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rabbit"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Rabbit.bin"))))
		return E_FAIL;
	//늑대
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wolf"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Wolf.bin"))))
		return E_FAIL;
	//플레이어
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Player.bin"))))
		return E_FAIL;
	//리볼버
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Revolver"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Revolver.bin"))))
		return E_FAIL;
	//라이플
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rifle"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Rifle.bin"))))
		return E_FAIL;
	//돌
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stone"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Stone.bin"))))
		return E_FAIL;
	
	//장비 토끼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PickRabbit"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_PickRabbit.bin"))))
		return E_FAIL;
	
	//칼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Knife"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Knife.bin"))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_FarmHouseBasement"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_FarmHouseBasement.bin"))))
		return E_FAIL;

	//	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//항생제
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_AntiBioticsBottle"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_AntiBioticsBottle.bin"))))
		return E_FAIL;

	//육포
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BeefJerky"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_BeefJerky.bin"))))
		return E_FAIL;

	//토끼 생고기 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RawRabbit"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_RawRabbit.bin"))))
		return E_FAIL;

	//토끼 구운고기 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CookedRabbit"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_CookedRabbit.bin"))))
		return E_FAIL;

	//내장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rut"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Rut.bin"))))
		return E_FAIL;

	//가죽
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Pelt"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Pelt.bin"))))
		return E_FAIL;
	//****************************************npc
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Mother"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Mother.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MotherChair"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_MotherChair.bin"))))
		return E_FAIL;
	//--------------------------------------------------stove
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stove"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_StoveMain.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_StoveDoor"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_StoveDoor.bin"))))
		return E_FAIL;
	//--------------------------------------------------메디킷
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MedKit"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_MedKit.bin"))))
		return E_FAIL;
	//--------------------------------------------------러그
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rug"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Rug.bin"))))
		return E_FAIL;
	//--------------------------------------------------테이블
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Table"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Table.bin"))))
		return E_FAIL;
	//--------------------------------------------------공구상자
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ToolBox"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_ToolBox.bin"))))
		return E_FAIL;


	//--------------------------------------------------성냥갑
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Matchbox"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Matchbox.bin"))))
		return E_FAIL;
	//--------------------------------------------------성냥
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Match"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Match.bin"))))
		return E_FAIL;

	//--------------------------------------------------나무막대
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stick"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Stick.bin"))))
		return E_FAIL;


	//--------------------------------------------------장작
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Hardwood"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Hardwood.bin"))))
		return E_FAIL;

	//-------------------------------------------------- 침낭
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BedRoll"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_BedRoll.bin"))))
		return E_FAIL;

	//-------------------------------------------------- 지평선
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SkyHorizon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_SkyHorizon.bin"))))
		return E_FAIL;

	//--------------------------------------------------플레어
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Flare"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_Flare.bin"))))
		return E_FAIL;


	//-------------------------------------------------- 풀
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_GrassA"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_GrassA.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_GrassB"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/bin/Prototype_Component_Model_GrassB.bin"))))
		return E_FAIL;

	//-------------------------------------------------- 파티클
	/* For.Prototype_Component_VIBuffer_Instance_Rect */

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Instance_Rect"),
		CVIBuffer_Instance_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//CVIBuffer_Instance::INSTANCE_DESC InstanceDesc{};
	///* For.Prototype_Component_VIBuffer_Instance_Point*/
	//ZeroMemory(&InstanceDesc, sizeof InstanceDesc);
	//InstanceDesc.iNumInstance = 5000;
	//InstanceDesc.vOffsetPos = _float3(0.0f, 0.f, 0.0f);
	//InstanceDesc.vPivotPos = _float3(0.0f, 50.f, 0.0f);
	//InstanceDesc.vRange = _float3(500.0f, 0.5f, 500.0f);
	//InstanceDesc.vSize = _float2(0.001f, 10.0f);
	//InstanceDesc.vSpeed = _float2(1.f, 7.f);
	//InstanceDesc.vLifeTime = _float2(6.f, 10.f);
	//InstanceDesc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Instance_Point"),
		CVIBuffer_Instance_Point::Create(m_pDevice, m_pContext))))
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

	/* For.Prototype_Component_Shader_VtxMeshID */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshID"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMeshID.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMeshMonster */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMeshMonster"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxInstance_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxInstance_Rect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstance_Rect.hlsl"), VTXINSTANCE_RECT::Elements, VTXINSTANCE_RECT::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxInstance_Point */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxInstance_Point"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstance_Point.hlsl"), VTXINSTANCE_POINT::Elements, VTXINSTANCE_POINT::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
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


	/* For.Prototype_GameObject_UILoadingBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UILoadingBar"),
		CUILoadingBar::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

	/* For.Prototype_GameObject_UIActiveButton*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIActiveButton"),
		CUIActiveButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIStoveItem*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIStoveItem"),
		CUIStoveItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UILoadingStruggle*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UILoadingStruggle"),
		CUILoadingStruggle::Create(m_pDevice, m_pContext))))
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

	/* For.Prototype_GameObject_Portal*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mother*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mother"),
		CMother::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rifle*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rifle"),
		CRifle::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_BoxProp*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BoxProp"),
		CBoxProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Stove*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stove"),
		CStove::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_Match*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Match"),
		CMatch::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rabbit*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rabbit"),
		CRabbit::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wolf*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wolf"),
		CWolf::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BedRoll*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BedRoll"),
		CBedRoll::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Rect"),
		CParticle_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Point */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Point"),
		CParticle_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Explosion */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Explosion"),
		CExplosion::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_RevolverFire */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RevolverFire"),
		CRevolverFire::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Flare */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Flare"),
		CFlare::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIBPToggle */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIBPToggle"),
		CUIBluePrintToggle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIBlood */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIBlood"),
		CUIBlood::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TrailEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TrailEffect"),
		CTrail_Effect::Create(m_pDevice, m_pContext))))
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
