#include "..\Public\Cell.h"
#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3 * pPoints, _int iIndex)
{
	m_iIndex = iIndex;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CCell::Initialize_Data(const _float3* pPoints, _int iIndex, _int* Neighbor)
{
	m_iIndex = iIndex;
	
	memcpy(m_iNeighborIndices, Neighbor, sizeof(_int) * LINE_END);
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

	return S_OK;
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;
}

_bool CCell::isIn(_fvector vPoint, _int* pNeighborIndex, _float4* LineDir)
{

	_vector		vSour, vDest;
	for (size_t i = 0; i < LINE_END; i++)
	{
		vSour = vPoint - XMLoadFloat3(&m_vPoints[i]);

		_vector		vLine = XMLoadFloat3(&m_vPoints[(i + 1) % POINT_END]) - XMLoadFloat3(&m_vPoints[i]);
		//2차원 외적
		vDest = XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f);
		_vector Nordir=XMVector3Normalize(vDest);	
		_float a = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vSour), XMVector3Normalize(vDest)));

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vSour), XMVector3Normalize(vDest))))
		{
			if(nullptr!= LineDir)
				XMStoreFloat4(LineDir, XMVector3Normalize(vDest));//충돌선의 노멀벡터 저장

			*pNeighborIndex = m_iNeighborIndices[i];
			return false;
		}
	}

	return true;
}

_bool CCell::Check_Points(_vector vPoint, _float3* StorePoint, _float SnapReach)
{
	_vector Length = { SnapReach,SnapReach,SnapReach,SnapReach };

	if( XMVector3NearEqual(XMLoadFloat3(&m_vPoints[POINT_A]), vPoint, Length))
	{
		XMStoreFloat3(StorePoint, XMLoadFloat3(&m_vPoints[POINT_A]));
		return true;
	}

	if ( XMVector3NearEqual(XMLoadFloat3(&m_vPoints[POINT_B]), vPoint, Length))
	{
		XMStoreFloat3(StorePoint, XMLoadFloat3(&m_vPoints[POINT_B]));
		return true;
	}

	if (XMVector3NearEqual(XMLoadFloat3(&m_vPoints[POINT_C]), vPoint, Length))
	{
		XMStoreFloat3(StorePoint, XMLoadFloat3(&m_vPoints[POINT_C]));
		return true;
	}

	return false;
}

void CCell::Save_Data(ofstream* pfout)
{
	pfout->write(reinterpret_cast<char*>(&m_vPoints), sizeof(_float3) * POINT_END);
	pfout->write(reinterpret_cast<char*>(&m_iNeighborIndices), sizeof(_int) * LINE_END);
}

void CCell::Sorting(_int DeleteCellIndex)
{
	m_iIndex -= 1;
	for (size_t i = 0; i < LINE_END; i++)
	{
		if (-1 != m_iNeighborIndices[i]&& m_iNeighborIndices[i] >= DeleteCellIndex)
			m_iNeighborIndices[i] -= 1;
	}
	
}
//가운데 지우면 양옆에 있던 이웃들 삭제 ,
//다음 인덱스 전부 감소
//이웃 값도 전부 감소.
void CCell::NeighborSort(_int DeleteCellIndex)
{
	for (size_t i = 0; i < LINE_END; i++)
	{
		if (DeleteCellIndex == m_iNeighborIndices[i])
			m_iNeighborIndices[i] = -1;

	}
}


#ifdef _DEBUG
HRESULT CCell::Render()
{
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	return S_OK;
}
#endif

_float CCell::Set_OnCell(_float3 vLocalPos)
{
	return m_pVIBuffer->Compute_Height(vLocalPos);
}

CCell * CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3 * pPoints, _int iIndex)
{
	CCell*		pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
		return nullptr;
	}

	return pInstance;
}

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex, _int* Neighbor)
{
	CCell* pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Data(pPoints, iIndex, Neighbor)))
	{
		MSG_BOX("Failed to Created : CCell");
		return nullptr;
	}

	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif
}
