#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(_uint iIndex) const;
	HRESULT Set_Switch(_uint iIndex, _bool bSwitch);
	HRESULT Set_Position(_uint iIndex, _fvector vPosition);
	HRESULT Set_Range(_uint iIndex, _float fRange);

public:
	HRESULT Initialize();
	HRESULT Rotate(_float fTimeDelta);
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	
private:
	list<class CLight*>			m_Lights;

public:
	static CLight_Manager* Create();
	virtual void Free() override;
};

END