#include "..\Public\GameInstance.h"

#include "Graphic_Device.h"
#include "Input_Device.h"

#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Renderer.h"
#include "Calculator.h"
#include "Font_Manager.h"
#include "Light_Manager.h"
#include "Target_Manager.h"
#include "Frustum.h"
#include "SoundMgr.h"
IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}


HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const ENGINE_DESC & EngineDesc, ID3D11Device ** ppDevice, ID3D11DeviceContext ** ppContext)
{
	/* 엔진 프로젝트르 ㄹ클라이언트에서 사용하기 위해 필수적으로 해야할 여러 초기화작업을 수행한다. */

	/* 그래픽 디바이스를 초기화하낟. */
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	/* 인풋 디바이스를 초기화하낟. */
	m_pInput_Device = CInput_Device::Create(hInst, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	/* 사운드  디바이스를 초기화하낟. */
	m_pSoundMgr = CSoundMgr::Create();
	if (nullptr == m_pSoundMgr)
		return E_FAIL;

	/* 레벨 매니져의 준비를 하자. */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	/* 오브젝트, 컴포넌트 매니져를 사용하기 위한 준비를 한다.*/
	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	/*타겟 매니저 생성*/
	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* 렌더러 생성. */
	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	/* 파이프라인 생성. */
	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	/*Calculator 생성*/
	m_pCalculator = CCalculator::Create(*ppDevice, *ppContext, EngineDesc.hWnd, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY);
	if (nullptr == m_pCalculator)
		return E_FAIL;

	/*폰트 매니저 생성*/
	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	/*라이트 매니저 생성*/
	m_pLight_Manager = CLight_Manager::Create();
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pFrustum = CFrustum::Create();
	if (nullptr == m_pFrustum)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager)
		return;

	m_pInput_Device->Update_InputDev();

	m_pObject_Manager->Priority_Tick(fTimeDelta);

	m_pObject_Manager->Tick(fTimeDelta);	

	m_pPipeLine->Tick();

	m_pSoundMgr->Update_Listener();

	m_pFrustum->Update();

	m_pCalculator->Tick();

	m_pObject_Manager->Late_Tick(fTimeDelta);

	m_pLevel_Manager->Tick(fTimeDelta);
}

HRESULT CGameInstance::Draw(const _float4 & vClearColor)
{
	m_pRenderer->Draw();	

	return S_OK;
}



void CGameInstance::Clear_Resources(_uint iLevelIndex)
{
	m_pRenderer->Clear();

	m_pObject_Manager->Clear(iLevelIndex);

	m_pComponent_Manager->Clear(iLevelIndex);
}

void CGameInstance::Clear_CloneData(_uint iLevelIndex)
{
	m_pRenderer->Clear();

	m_pObject_Manager->Clear(iLevelIndex);
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

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIKeyState_Once(_ubyte byKeyID)
{
	 return m_pInput_Device->Get_DIKeyState_Once(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_byte CGameInstance::Get_DIMouseState_Once(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState_Once(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

_float CGameInstance::Get_TimeDelta(const _tchar * pTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);	
}

void CGameInstance::Update_TimeDelta(const _tchar * pTimerTag)
{
	m_pTimer_Manager->Update_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Ready_Timer(const _tchar * pTimerTag)
{
	return m_pTimer_Manager->Ready_Timer(pTimerTag);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_CloneObject(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_CloneObject(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag,  CGameObject** pGameObject ,void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_CloneObject(iLevelIndex, strLayerTag, strPrototypeTag, pGameObject, pArg );

}

HRESULT CGameInstance::Delete_CloneObject(_uint iLevelIndex,  CGameObject* pGameObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Delete_CloneObject(iLevelIndex,  pGameObject);
}

CGameObject* CGameInstance::Find_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pGameObject)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_CloneObject(iLevelIndex, strLayerTag, pGameObject);
}

CGameObject* CGameInstance::FindID_CloneObject(_uint iLevelIndex,const _int& ID)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->FindID_CloneObject(iLevelIndex, ID);
}

CGameObject* CGameInstance::FindIndex_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->FindIndex_CloneObject(iLevelIndex, strLayerTag, iIndex);
}

vector< const _float4x4*>* CGameInstance::Get_ObPos(_uint iLevelIndex, const wstring& strLayerTag)
{
	return m_pObject_Manager->Get_ObPos(iLevelIndex, strLayerTag);
}

CGameObject* CGameInstance::Clone_Object(const wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Clone_Object(strPrototypeTag, pArg);
}

HRESULT CGameInstance::Save_Level(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Save_Level(iLevelIndex);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);

}

HRESULT CGameInstance::Render_UI(_uint iLevelIndex, wstring LayerName)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	 m_pObject_Manager->Render_UI(iLevelIndex, LayerName);
	 return S_OK;
}

CUIBase* CGameInstance::FindUIID_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, _int UIID)
{
	return m_pObject_Manager->FindUIID_CloneObject(iLevelIndex, strLayerTag, UIID);
}

_bool CGameInstance::Intersect(_uint iLevelIndex, const wstring& strLayerTag, CCollider* pTargetCollider)
{
	return m_pObject_Manager->Intersect(iLevelIndex, strLayerTag, pTargetCollider);
}

CGameObject* CGameInstance::IntersectRay(_uint iLevelIndex, const wstring& strLayerTag, _vector* pRayArray, _float* fDist)
{
	return m_pObject_Manager->IntersectRay(iLevelIndex, strLayerTag, pRayArray,fDist);
}

_bool CGameInstance::RayCollInfo(_uint iLevelIndex, const wstring& strLayerTag, _vector* pRayArray, CGameObject** pGameObject)
{
	return m_pObject_Manager->RayCollInfo(iLevelIndex,strLayerTag,pRayArray, pGameObject);
}


HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring & strPrototypeTag, CComponent * pPrototype)
{
	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring & strPrototypeTag, void * pArg)
{
	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Save_Binary(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring FilePath)
{
	return m_pComponent_Manager->Save_Binary(iLevelIndex, strPrototypeTag, FilePath);
}

HRESULT CGameInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);	
}
#ifdef _DEBUG
HRESULT CGameInstance::Add_DebugComponent(CComponent* pComponent)
{
	return m_pRenderer->Add_DebugComponent(pComponent);
}
#endif
const _float4x4* CGameInstance::Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_float4x4(eState);

}

_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

const _float4x4* CGameInstance::Get_Transform_float4x4_Inverse(CPipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_float4x4_Inverse(eState);
}

_matrix CGameInstance::Get_Transform_Matrix_Inverse(CPipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Matrix_Inverse(eState);
}

const _float4* CGameInstance::Get_CamPosition_float4()
{
	return m_pPipeLine->Get_CamPosition_float4();
}

_vector CGameInstance::Get_CamPosition()
{
	return m_pPipeLine->Get_CamPosition();
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}


_vector CGameInstance::Picking_on_Terrain(HWND hWnd, _matrix TerrainWorldMatrixInverse, _matrix mViewMatrixInverse, _matrix mProjMatrixInverse, _float4* pVtxPos, _int* pTerrainUV, _float* pWinSize)
{
	return m_pCalculator->Picking_on_Terrain(hWnd, TerrainWorldMatrixInverse, mViewMatrixInverse, mProjMatrixInverse, pVtxPos, pTerrainUV, pWinSize);
}

_vector CGameInstance::Picking_HitScreen()
{
	return m_pCalculator->Picking_HitScreen();
}

_int CGameInstance::Picking_IDScreen()
{
	return m_pCalculator->Picking_IDScreen();
}

_int CGameInstance::Picking_UIIDScreen()
{
	return m_pCalculator->Picking_UIIDScreen();
}

_bool CGameInstance::Compare_Float4(_float4 f1, _float4 f2)
{
	return m_pCalculator->Compare_Float4(f1,f2);
}

_vector CGameInstance::Picking_UI(_fmatrix ProjM)
{
	return m_pCalculator->Picking_UI(ProjM);
}

void CGameInstance::World_MouseRay(_vector* RayArray)
{
	m_pCalculator->World_MouseRay(RayArray);
}

HRESULT CGameInstance::Tick()
{
	return m_pCalculator->Tick();
}

_bool CGameInstance::Get_PickPos(_float4* pPickPos)
{
	return m_pCalculator->Get_PickPos(pPickPos);
}

HRESULT CGameInstance::Add_Font(const wstring& strFontTag, const wstring& strFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, strFontFilePath);
}

HRESULT CGameInstance::Render_Font(const wstring& strFontTag, const wstring& strText, const _float2& vPosition, _fvector vColor)
{
	return m_pFont_Manager->Render_Font(strFontTag, strText, vPosition, vColor);
}

const LIGHT_DESC* CGameInstance::Get_LightDesc(_uint iIndex) const
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(LightDesc);
}

HRESULT CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Render(pShader, pVIBuffer);
}

HRESULT CGameInstance::Rotate_Light(_float fTimeDelta)
{
	return m_pLight_Manager->Rotate(fTimeDelta);
}

HRESULT CGameInstance::Set_LightSwitch(_uint iIndex, _bool bSwitch)
{
	return m_pLight_Manager->Set_Switch(iIndex, bSwitch);
}

HRESULT CGameInstance::Set_LightPosition(_uint iIndex, _fvector vPosition)
{
	return m_pLight_Manager->Set_Position(iIndex, vPosition);
}

HRESULT CGameInstance::Set_Range(_uint iIndex, _float fRange)
{
	return m_pLight_Manager->Set_Range(iIndex, fRange);
}

HRESULT CGameInstance::Add_RenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iSizeX, iSizeY, ePixelFormat, vClearColor);
}

HRESULT CGameInstance::Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const wstring& strMRTTag, _bool isClear, ID3D11DepthStencilView* pDSView)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag, isClear, pDSView);
}

HRESULT CGameInstance::Begin_UIMRT(const wstring& strMRTTag)
{
	return m_pTarget_Manager->Begin_UIMRT(strMRTTag);
}

HRESULT CGameInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}

HRESULT CGameInstance::Bind_RenderTargetSRV(const wstring& strTargetTag, CShader* pShader, const _char* pConstantName)
{
	return m_pTarget_Manager->Bind_RenderTargetSRV(strTargetTag, pShader, pConstantName);
}

HRESULT CGameInstance::Copy_Resource(const wstring& strTargetTag, ID3D11Texture2D* pDesc)
{
	return m_pTarget_Manager->Copy_Resource(strTargetTag, pDesc);
}

void CGameInstance::Transform_ToLocalSpace(_fmatrix WorldMatrixInv)
{
	return m_pFrustum->Transform_ToLocalSpace(WorldMatrixInv);
}

_bool CGameInstance::isIn_WorldFrustum(_fvector vPosition, _float fRange)
{
	return m_pFrustum->isIn_WorldFrustum(vPosition, fRange);
}

_bool CGameInstance::isIn_LocalFrustum(_fvector vPosition, _float fRange)
{
	return m_pFrustum->isIn_LocalFrustum(vPosition, fRange);
}

void CGameInstance::Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume )
{
	m_pSoundMgr->Play_Sound(pSoundKey, eID, fVolume);
}

void CGameInstance::Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _fvector vPos)
{
	m_pSoundMgr->Play_Sound(pSoundKey, eID, fVolume, vPos);
}

void CGameInstance::StopSound(CHANNELID eID)
{
	m_pSoundMgr->StopSound(eID);
}

void CGameInstance::Play_BGM(wstring pSoundKey, _float fVolume)
{
	m_pSoundMgr->Play_BGM(pSoundKey, fVolume);
}


#ifdef _DEBUG
HRESULT CGameInstance::Ready_RTDebug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}

HRESULT CGameInstance::Render_RTDebug(const wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_Debug(strMRTTag, pShader, pVIBuffer);
}
#endif

void CGameInstance::Release_Engine()
{	
	CGameInstance::GetInstance()->Free();

	DestroyInstance();	
}

void CGameInstance::Free()
{
	Safe_Release(m_pSoundMgr);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pCalculator);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}