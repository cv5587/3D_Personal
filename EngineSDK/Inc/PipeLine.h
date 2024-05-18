#pragma once
#include "Base.h"
BEGIN(Engine)
class CPipeLine final:
    public CBase
{
public:
    enum TRANSFORMSTATE{TS_CAMWORLD,TS_VIEW,TS_PROJ,TS_END};
private:
    CPipeLine();
    virtual ~CPipeLine() = default;

public:
	const _float4x4* Get_Transform_float4x4(TRANSFORMSTATE eState);
	_matrix Get_Transform_Matrix(TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_float4x4_Inverse(TRANSFORMSTATE eState);
	_matrix Get_Transform_Matrix_Inverse(TRANSFORMSTATE eState);
	const _float4* Get_CamPosition_float4();
	_vector Get_CamPosition();
	void Set_CamWorld_Pointer(_float4x4* CamWorld);
public:
	void Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformStateMatrix[eState], TransformMatrix);
	}
public:
	HRESULT Initialize();
	void Tick();

private:
	_float4x4				m_TransformStateMatrix[TS_END];
	_float4x4				m_TransformStateMatrixInverse[TS_END];
	_float4					m_vCamPosition;
	_float4x4*				m_CamWorldMatrix;
	

public:
	static CPipeLine* Create();
	virtual void Free() override;
};

END



