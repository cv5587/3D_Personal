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

public: /* For.Graphic_Device */	
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

public: /* For.Input_Device */
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

public: /* For.Timer_Manager */
	_float		Get_TimeDelta(const _tchar* pTimerTag);
	void		Update_TimeDelta(const _tchar* pTimerTag);
	HRESULT		Ready_Timer(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);

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

private:
	class CGraphic_Device*				m_pGraphic_Device = { nullptr };
	class CInput_Device*					m_pInput_Device = { nullptr };
	class CTimer_Manager*				m_pTimer_Manager = { nullptr };
	class CLevel_Manager*				m_pLevel_Manager = { nullptr };
	class CObject_Manager*				m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CRenderer*							m_pRenderer = { nullptr };
	class CPipeLine*							m_pPipeLine = { nullptr };

public:	
	static void Release_Engine();
	virtual void Free() override;
};

END