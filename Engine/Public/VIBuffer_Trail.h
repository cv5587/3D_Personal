#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
public:
	typedef struct TRAIL_DESC  {
		_uint ivtxMaxNum;
		_float3 vInitPosA;
		_float3 vInitPosB;
	};
	typedef struct tTrailInfo {
		_float3 vPos[2];
	}TRAIL_INFO;

private:
	CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Trail(const CVIBuffer_Trail& rhs);
	virtual ~CVIBuffer_Trail() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;
public:
	HRESULT Tick(_float fTimeDelta , _fmatrix WorldMatrix);

private:
	_float m_fCurrentTime = { 0.f };
	_float m_fTimeAcc = { 0.02f };
	_uint m_iMaxTrail = { 0 };
	_uint m_iCurrentTrail = { 0 };

	_uint m_iCurrentIndices = { 0 };

	_float3 m_vInitPosA;
	_float3 m_vInitPosB;
private:
	list<TRAIL_INFO> m_TrailInfos;

public:

public:
	static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END