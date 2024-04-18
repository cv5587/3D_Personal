#include "stdafx.h"
#include "Gui.h"
#include "GameInstance.h"

#include "TerrainManager.h"
#include "EnvironmentObject.h"
#include "PipeLine.h"
#include "Data_Manager.h"	
#include "LandObject.h"

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
bool useWindow = false;

static const float identityMatrix[16] =
{ 1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f };

CGui::CGui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	,m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CGui::Initialize()
{
	m_pTerrainManager = CTerrainManager::Create();
	m_pData_Manager = CData_Manager::Create(m_pDevice,m_pContext,m_pTerrainManager);
	return S_OK;
}

CGui* CGui::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGui* pInstance = new CGui(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);
		MSG_BOX("ImGui Create Failed");
		return nullptr;
	}

	//ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	return pInstance;
}

//imgui 기능만 구현
HRESULT CGui::Update_UI(_float fTimeDelta)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuizmo::BeginFrame();

	bool bDemo = true;
	ImGui::ShowDemoWindow(&bDemo);

	static _int iTerrain[2] = { 0,0 };

	ImGui::Begin("BackGround");
	ImGui::SetWindowPos("BackGround", ImVec2(0, 0), ImGuiCond_Always);
	if (ImGui::TreeNode("MakeTerrain"))
	{
		ImGui::InputInt2("TerrainUV", iTerrain);
		ImGui::SliderInt2("TerrainUV", iTerrain, 3, 1025);

		if (ImGui::Button("Create")&& !m_bInputObject)
		{
			m_bInputObject = !m_bInputObject;
		}


		ImGui::NewLine();
		ImGui::TreePop();
	}


	
	const char* szObject = "EnvironmentObject";
	if (ImGui::TreeNode(szObject))
	{

		{
			const char* objects[] = { "CliffA","Charcoal","Stone"};
			static int object_current = 0;
			ImGui::ListBox("List", &object_current, objects, IM_ARRAYSIZE(objects));
			
		ImGui::NewLine();
			if (ImGui::Button("ObjectCreate") )
			{
				char szLayername[MAX_PATH] = "Layer_";
				strcat_s(szLayername, szObject);
				ZeroMemory(m_szLayerPath, sizeof(_tchar) * MAX_PATH);
				MultiByteToWideChar(CP_ACP, 0, szLayername, strlen(szLayername), m_szLayerPath, MAX_PATH);

				char szObjectname[MAX_PATH] = "Prototype_GameObject_";
				strcat_s(szObjectname, szObject);
				ZeroMemory(m_szRealFullPath, sizeof(_tchar) * MAX_PATH);
				MultiByteToWideChar(CP_ACP, 0, szObjectname, strlen(szObjectname), m_szRealFullPath, MAX_PATH);
				
				char szComponentname[MAX_PATH] = "Prototype_Component_Model_";
				strcat_s(szComponentname, objects[object_current]);	
				wstring wstr(szComponentname, szComponentname + strlen(szComponentname));
				m_ComponentTag = wstr;

				m_bMakeObject = true;
			}

			if (ImGui::Button("ObjectBinary"))
			{
				char szProtoname[MAX_PATH] = "Prototype_Component_Model_";
				strcat_s(szProtoname, objects[object_current]);

				wstring wstr(szProtoname, szProtoname + strlen(szProtoname));
				wstring filepath = TEXT("../Bin/bin/");
				wstring Ext = TEXT(".bin");
				filepath=filepath + wstr+Ext;
				m_ComponentTag = wstr;
				if (FAILED(m_pGameInstance->Save_Binary(LEVEL_GAMEPLAY, wstr, filepath)))
					return E_FAIL;
			}
		}

		ImGui::NewLine();
		ImGui::TreePop();
	}



	const char* szMoster = "Monster";
	if (ImGui::TreeNode(szMoster))
	{

		{
			const char* Monsters[] = { "Fiona","Rabbit","Player","Revolver"};
			static int Monster_current = 0;
			ImGui::ListBox("List", &Monster_current, Monsters, IM_ARRAYSIZE(Monsters));


			ImGui::NewLine();
			if (ImGui::Button("MonsterCreate"))
			{
				char szLayername[MAX_PATH] = "Layer_";
				strcat_s(szLayername, szMoster);
				ZeroMemory(m_szLayerPath, sizeof(_tchar) * MAX_PATH);
				MultiByteToWideChar(CP_ACP, 0, szLayername, strlen(szLayername), m_szLayerPath, MAX_PATH);

				char szMonstername[MAX_PATH] = "Prototype_GameObject_";
				strcat_s(szMonstername, szMoster);
				wcsset(m_szRealFullPath, L'\0');
				MultiByteToWideChar(CP_ACP, 0, szMonstername, strlen(szMonstername), m_szRealFullPath, MAX_PATH);

				char szComponentname[MAX_PATH] = "Prototype_Component_Model_";
				strcat_s(szComponentname, Monsters[Monster_current]);
				wstring wstr(szComponentname, szComponentname + strlen(szComponentname));
				m_ComponentTag = wstr;

				m_bMakeObject = true;
			}

			if (ImGui::Button("ObjectBinary"))
			{
				char szProtoname[MAX_PATH] = "Prototype_Component_Model_";
				strcat_s(szProtoname, Monsters[Monster_current]);

				wstring wstr(szProtoname, szProtoname + strlen(szProtoname));
				wstring filepath = TEXT("../Bin/bin/");
				wstring Ext = TEXT(".bin");
				filepath = filepath + wstr + Ext;
				m_ComponentTag = wstr;
				if (FAILED(m_pGameInstance->Save_Binary(LEVEL_GAMEPLAY, wstr, filepath)))
					return E_FAIL;
			}
		}

		ImGui::NewLine();

		ImGui::TreePop();
	}

	if (ImGui::Button("Delete_Object"))
	{
		if (nullptr != m_pPickObject)
		{
			m_pGameInstance->Delete_CloneObject(LEVEL_GAMEPLAY, m_pPickObject);
			m_pPickObject = nullptr;
		}
	}
	ImGui::End();

	ImGui::Begin("Control_GameData");

	if (ImGui::TreeNode("Level_Choice"))
	{
		const char* Levels[] = { "LEVEL_STATIC","LEVEL_LOADING","LEVEL_LOGO","LEVEL_GAMEPLAY"};
		static int Level_current = 0;
		ImGui::ListBox("List", &Level_current, Levels, IM_ARRAYSIZE(Levels));

		if (ImGui::Button("Save_Data"))
		{
			m_pGameInstance->Save_Level((_uint)Level_current);
		}
		if (ImGui::Button("Load_Data"))
		{
			//로드도 클라에서 하자
			m_pData_Manager->Load_Data((_uint)Level_current);
		}
		ImGui::TreePop();	
	}


	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(1030,0), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_Appearing);
	
	ImGui::Begin("Editor");	
	TestGuizmo( fTimeDelta);

	ImGui::End();
	

	if (0 == iTerrain[0] % 2)
		iTerrain[0]++;
	else if (0 == iTerrain[1] % 2)
		iTerrain[1]++;

	if (m_bInputObject)
	{
		Make_Terrain(iTerrain);
		//m_pTerrainManager->Clone_Terrain(iTerrain);
		
		m_bInputObject = !m_bInputObject;
	}

	if (m_bMakeObject &&(m_pGameInstance->Get_DIMouseState(DIM_LB) & 0x80)&&(m_pGameInstance->Get_DIKeyState(DIK_LCONTROL) & 0x80))	
	{
		
		CLandObject::LANDOBJ_DESC		LandObjDesc{};

		LandObjDesc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform")));
		LandObjDesc.pTerrainVIBuffer = dynamic_cast<CVIBuffer*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));

		_vector Pick_Point = Picking_HitScreen();

		LandObjDesc.ProtoTypeTag=m_szRealFullPath;
		LandObjDesc.ModelTag = m_ComponentTag;
		
		if(0<Pick_Point.m128_f32[0])
			{
				_matrix PrePosition = XMMatrixIdentity();
				PrePosition.r[3] = Pick_Point;
				XMStoreFloat4x4(&LandObjDesc.vPrePosition, PrePosition);
				if (FAILED(m_pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, m_szLayerPath, m_szRealFullPath, &LandObjDesc) ))
					return E_FAIL;
		
				m_bMakeObject = false;
			}
	}
	
	if ((m_pGameInstance->Get_DIMouseState(DIM_LB) & 0x80) && (m_pGameInstance->Get_DIKeyState(DIK_LALT) & 0x80))
	{
		m_pPickObject=m_pGameInstance->FindID_CloneObject(LEVEL_GAMEPLAY,  m_pGameInstance->Picking_IDScreen());

	}

	return S_OK;
}

//연산
HRESULT CGui::LateUpdate_UI()
{
	return S_OK;
}

HRESULT CGui::Render()
{
	ImGui::Render();

	return S_OK;
}


_vector CGui::Picking_HitScreen()
{
	return m_pGameInstance->Picking_HitScreen();
}


void CGui::EditTransform(_float* cameraView, _float* cameraProjection, _float* matrix)
{

	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	if (ImGui::IsKeyPressed(ImGuiKey_T))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(ImGuiKey_E))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
		mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation);
	ImGui::InputFloat3("Rt", matrixRotation);
	ImGui::InputFloat3("Sc", matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}
	ImGui::SameLine();
	

	ImGuiIO& io = ImGui::GetIO();
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;
	static ImGuiWindowFlags gizmoWindowFlags = 0;

	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	
	
	//ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
	//ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
	//_vector CamPosition=m_pGameInstance->Get_CamPosition();
	//_vector ObPosition = dynamic_cast<CTransform*>(m_pPickObject->Get_Transform())->Get_State(CTransform::STATE_POSITION);	
	//_vector CamDistance=XMVector3Length(ObPosition-CamPosition);
	//float camDistance = 8.f;
	//ImGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
	//m_pGameInstance->Set_Transform(CPipeLine::TS_CAMWORLD, XMMatrixInverse(nullptr,XMLoadFloat4x4((_float4x4*)cameraView)));
	if (useWindow)
	{
		ImGui::PopStyleColor(1);
	}
	
}

void CGui::TestGuizmo(_float fTimeDelta)
{

	_float* cameraView = (_float*)m_pGameInstance->Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE::TS_VIEW);
	_float* cameraProjection = (_float*)m_pGameInstance->Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE::TS_PROJ);
	if(nullptr!=m_pPickObject)
	{
		_float* objectWorld = (_float*)dynamic_cast<CTransform*>(m_pPickObject->Get_Transform())->Get_WorldFloat4x4();
		EditTransform(cameraView, cameraProjection, objectWorld);
		ImGuizmo::IsUsing();
	}

}

_matrix CGui::CameraStateChange(_fmatrix CamWorld, _cmatrix ObjWorld,_float fTimeDelta)
{
	_matrix CamW = CamWorld;
	_float3 fScaled = { XMVector3Length(CamWorld.r[0]).m128_f32[0],
		XMVector3Length(CamWorld.r[1]).m128_f32[0],
		XMVector3Length(CamWorld.r[2]).m128_f32[0]};

	_vector vLook = XMVector4Normalize(ObjWorld.r[3] - CamWorld.r[3])*fScaled.z;

	_vector vRight= XMVector4Normalize(XMVector3Cross(vLook, XMVectorSet(0.f, 1.f, 0.f, 0.f)))*fScaled.x;
	
	_vector vUp = XMVector4Normalize(XMVector3Cross(vRight, vLook))*fScaled.y;
	if (vUp.m128_f32[1] < 0)
		vUp *= -1.f;

	_vector a, b;
	_matrix ObPosition = XMMatrixIdentity();
	XMMatrixDecompose(&a,&b, &ObPosition.r[3],ObjWorld);
	_matrix ObPositionInverse=XMMatrixInverse(nullptr, ObPosition);
	CamW.r[0] = vRight;
	CamW.r[1] = vUp;
	CamW.r[2] = vLook;

	CamW*= ObPositionInverse;
	
	_long		MouseMove = { 0 };
	_float		fSensor = 0.05f;
	_float		fRotationPerSec = XMConvertToRadians(90.f);
	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f,1.f,0.f,0.f), fRotationPerSec * fTimeDelta);
		CamW *= RotationMatrix;
	}

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		_matrix		RotationMatrix = XMMatrixRotationAxis(CamW.r[0], fRotationPerSec * fTimeDelta);
		CamW *= RotationMatrix;	
	}
	CamW *= ObPosition;

	return XMMatrixInverse(nullptr, CamW);
}

void CGui::Make_Terrain(void* pArg)
{
	m_pTerrainManager->Clone_Terrain(pArg);
}

void CGui::Free()
{
	Safe_Release(m_pData_Manager);
	Safe_Release(m_pTerrainManager);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
