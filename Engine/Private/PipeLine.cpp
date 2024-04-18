#include "PipeLine.h"

CPipeLine::CPipeLine()
{
}

const _float4x4* CPipeLine::Get_Transform_float4x4(TRANSFORMSTATE eState)
{
	return &m_TransformStateMatrix[eState];
}

_matrix CPipeLine::Get_Transform_Matrix(TRANSFORMSTATE eState)
{
	return XMLoadFloat4x4(&m_TransformStateMatrix[eState]);
}

const _float4x4* CPipeLine::Get_Transform_float4x4_Inverse(TRANSFORMSTATE eState)
{
	return &m_TransformStateMatrixInverse[eState];
}

_matrix CPipeLine::Get_Transform_Matrix_Inverse(TRANSFORMSTATE eState)
{
	return XMLoadFloat4x4(&m_TransformStateMatrixInverse[eState]);
}

const _float4* CPipeLine::Get_CamPosition_float4()
{
	return &m_vCamPosition;
}

_vector CPipeLine::Get_CamPosition()
{
	return XMLoadFloat4(&m_vCamPosition);
}

void CPipeLine::Set_CamWorld_Pointer(_float4x4* CamWorld)
{
	m_CamWorldMatrix = CamWorld;
}

HRESULT CPipeLine::Initialize()
{
	for (size_t i = 0; i < TS_END; i++)
	{
		XMStoreFloat4x4(&m_TransformStateMatrix[i], XMMatrixIdentity());
		m_TransformStateMatrixInverse[i] = m_TransformStateMatrix[i];
	}

	m_vCamPosition = _float4(0.f, 0.f, 0.f, 1.f);

	return S_OK;
}


void CPipeLine::Tick()
{
	for (size_t i = TS_CAMWORLD; i < TS_END; i++)
	{
		XMStoreFloat4x4(&m_TransformStateMatrixInverse[i], XMMatrixInverse(nullptr, Get_Transform_Matrix(TRANSFORMSTATE(i))));
	}

	memcpy(&m_vCamPosition, &m_TransformStateMatrixInverse[TS_VIEW].m[3], sizeof(_float4));
}

CPipeLine* CPipeLine::Create()
{
	CPipeLine* pInstance = new CPipeLine();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CPipeLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPipeLine::Free()
{
}
