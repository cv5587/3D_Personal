#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* ȭ�鿡 �ε� ���� �����ش� + ���� ������ ���� ���ҽ��� �غ��ϳ�. */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CData_Manager* pDataManager);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVEL eNextLevel);
	virtual void Tick(_float fTimeDelta) override;

private:
	LEVEL				m_eNextLevel = { LEVEL_END };

	class CLoader* m_pLoader = { nullptr };
	class CData_Manager* m_pDataManager = { nullptr };
	
private:
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);

public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel, class CData_Manager* pDataManager);
	virtual void Free() override;
};

END