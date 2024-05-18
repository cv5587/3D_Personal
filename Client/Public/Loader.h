#pragma once

#include "Client_Defines.h"
#include "Base.h"
BEGIN(Engine)
class CGameInstance;
END
BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual ~CLoader() = default;

public:
	const _tchar* Get_LoadingText() const {
		return m_szLoadingText;
	}

	_bool is_Finished() const {
		return m_isFinished;
	}

public:	
	HRESULT Initialize(LEVEL eNextLevel);
	HRESULT Loading();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };	

	HANDLE					m_hThread = { 0 };
	CRITICAL_SECTION		m_Critical_Section = {};
	LEVEL					m_eNextLevel = { LEVEL_END };
	_tchar					m_szLoadingText[MAX_PATH] = { TEXT("") };
	_bool					m_isFinished = { false };
	CGameInstance* m_pGameInstance = { nullptr };

private:
	HRESULT Loading_For_LogoLevel();
	HRESULT Loading_For_GamePlayLevel();
	//클라에서  로드 클래스 만들자
public:
	static CLoader* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevel);
	virtual void Free() override;



};

END