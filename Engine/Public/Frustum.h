#pragma once

#include "Base.h"

/* ���������̽� ���� ����ü�� �����ϳ�. */
/* �� �����Ӹ��� ����ü�� ������� ��ȯ���ѳ����۾��� �����Ѵ�. */

BEGIN(Engine)

class CFrustum final : public CBase
{
private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT Initialize();
	void Update();

	void Transform_ToLocalSpace(_fmatrix WorldMatrixInv);
	_bool isIn_WorldFrustum(_fvector vPosition, _float fRange = 0.f);
	_bool isIn_LocalFrustum(_fvector vPosition, _float fRange);

private:
	_float3					m_vPoints[8] = {};
	_float4					m_vWorldPoints[8] = {};
	class CGameInstance* m_pGameInstance = { nullptr };

	_float4					m_vWorldPlanes[6] = {};
	_float4					m_vLocalPlanes[6] = {};

private:

	void Make_Planes(const _float4* vPoints, _float4* pPlanes);


public:
	static CFrustum* Create();
	virtual void Free() override;
};

END