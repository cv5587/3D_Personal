#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instance abstract : public CVIBuffer
{
public:
	typedef struct INSTANCE_DESC
	{
		_uint			iNumInstance = { 0 };
		_float3			vOffsetPos;
		_float3			vPivotPos;
		_float3			vRange;
		_float2			vSize;
		_float			fScaleSize;
		_float2			vSpeed;
		_float2			vLifeTime;
		_bool			isLoop;
		_float4			vStartPos;
		const _float4x4* pParentMatrix;
	}INSTANCE_DESC;
protected:
	CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instance(const CVIBuffer_Instance& rhs);
	virtual ~CVIBuffer_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCE_DESC& InstanceDesc);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_Buffers() override;
	virtual HRESULT Render() override;

public:
	void Spread(_float fTimeDelta);
	void Drop(_float fTimeDelta);
	void Up(_float fTimeDelta);
	void Reset();
	void Scaling(_float fTimeDelta);

public:
	_bool Get_Live();
protected:
	ID3D11Buffer* m_pVBInstance = { nullptr };
	D3D11_BUFFER_DESC			m_InstanceBufferDesc = {};
	_uint						m_iNumInstance = { 0 };
	_uint						m_iInstanceStride = { 0 };
	_uint						m_iIndexCountPerInstance = { 0 };

	_float* m_pSpeeds = { nullptr };
	_float3* m_pOriginalPositions = { nullptr };
	INSTANCE_DESC				m_InstanceDesc = {};

protected:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END