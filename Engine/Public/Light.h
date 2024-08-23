#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	const LIGHT_DESC* Get_LightDesc() const {
		return &m_LightDesc;
	}
	void Set_LightDesc(_vector vDir) {
		XMStoreFloat4(&m_LightDesc.vDirection, vDir);
	}
	_bool Get_Switch() { return m_bSwitch; }
	void Set_Switch(_bool bSwtich) {
		m_bSwitch = bSwtich;
		m_LightDesc.bSwitch = bSwtich;
	}
public:
	HRESULT Initialize(const LIGHT_DESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

private:
	LIGHT_DESC				m_LightDesc={};
	_bool						m_bSwitch = { false };

public:
	static CLight* Create(const LIGHT_DESC& LightDesc);
	virtual void Free() override;
};

END