#pragma once

#include "Renderer.h"
#include "Component_Manager.h"
#include "PipeLine.h"



BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDevice, _Out_ ID3D11DeviceContext** ppContext);
	void Tick_Engine(_float fTimeDelta);
	HRESULT Draw(const _float4& vClearColor);
	void Clear_Resources(_uint iLevelIndex);
	void Clear_CloneData(_uint iLevelIndex);

public: /* For.Graphic_Device */	
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_HitScreenBuffer_View();	
	HRESULT Clear_IDScreenBuffer_View();
	HRESULT Clear_UIIDScreenBuffer_View();
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();
	_float Compute_ProjZ(const POINT& ptWindowPos, ID3D11Texture2D* pHitScreenTexture);
	_int	Compute_ID(const POINT& ptWindowPos, ID3D11Texture2D* pIDScreenTexture);
	_int	Compute_UIID(const POINT& ptWindowPos, ID3D11Texture2D* pIDScreenTexture);

	template<typename T>
	HRESULT Clear_Texture(TextureType eTextureType, T* data);

public: /* For.Input_Device */
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte		Get_DIKeyState_Once(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_byte		Get_DIMouseState_Once(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

public: /* For.Timer_Manager */
	_float		Get_TimeDelta(const _tchar* pTimerTag);
	void		Update_TimeDelta(const _tchar* pTimerTag);
	HRESULT		Ready_Timer(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	//기본생성
	HRESULT Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	//터레인 변경생성
	HRESULT Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag,  CGameObject** pGameObject, void* pArg = nullptr);
	HRESULT Delete_CloneObject(_uint iLevelIndex,  CGameObject* pGameObject);
	CGameObject* Find_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pGameObject);
	CGameObject* FindID_CloneObject(_uint iLevelIndex, const _int& ID);
	CGameObject* FindIndex_CloneObject(_uint iLevelIndex , const wstring& strLayerTag,_uint iIndex =0 );
	vector< const _float4x4*>* Get_ObPos(_uint iLevelIndex, const wstring& strLayerTag);
	//파츠오브젝트용
	class CGameObject* Clone_Object(const wstring& strPrototypeTag, void* pArg);
	//데이터 파싱
	HRESULT Save_Level(_uint iLevelIndex);
	class CComponent* Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComponentTag, _uint iIndex = 0);
	//레이어 렌더용
	HRESULT Render_UI(_uint iLevelIndex, wstring LayerName);
	//UI 용
	class CUIBase* FindUIID_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, _int UIID);
	_bool Intersect(_uint iLevelIndex, const wstring& strLayerTag, class CCollider* pTargetCollider);


public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);
	HRESULT Save_Binary(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring FilePath);

public: /* For.Renderer */
	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);

public: /* For.PipeLine */
	const _float4x4* Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState);
	_matrix Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_float4x4_Inverse(CPipeLine::TRANSFORMSTATE eState);
	_matrix Get_Transform_Matrix_Inverse(CPipeLine::TRANSFORMSTATE eState);
	const _float4* Get_CamPosition_float4();
	_vector Get_CamPosition();
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix);

public:/*For.Calculator*/
	_vector Picking_on_Terrain(HWND hWnd, _matrix TerrainWorldMatrixInverse, _matrix mViewMatrixInverse, _matrix mProjMatrixInverse, _float4* pVtxPos, _int* pTerrainUV, _float* pWinSize);
	_vector Picking_HitScreen();
	_int Picking_IDScreen();
	_int Picking_UIIDScreen();
	_bool Compare_Float4(_float4 f1, _float4 f2);
	_vector Picking_UI(_fmatrix ProjM);
	void World_MouseRay(_vector* RayArray);

public: /* For.Font_Manager */
	HRESULT Add_Font(const wstring& strFontTag, const wstring& strFontFilePath);
	HRESULT Render_Font(const wstring& strFontTag, const wstring& strText, const _float2& vPosition, _fvector vColor);

private:
	class CGraphic_Device*				m_pGraphic_Device = { nullptr };
	class CInput_Device*					m_pInput_Device = { nullptr };
	class CTimer_Manager*				m_pTimer_Manager = { nullptr };
	class CLevel_Manager*				m_pLevel_Manager = { nullptr };
	class CObject_Manager*				m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CRenderer*							m_pRenderer = { nullptr };
	class CPipeLine*							m_pPipeLine = { nullptr };
	class CCalculator*						m_pCalculator = { nullptr };
	class CFont_Manager*					m_pFont_Manager = { nullptr };
public:	
	static void Release_Engine();
	virtual void Free() override;
};

END

template<typename T>
inline HRESULT CGameInstance::Clear_Texture(TextureType eTextureType, T* data)
{
	return m_pGraphic_Device->Clear_Texture(eTextureType, data);
}