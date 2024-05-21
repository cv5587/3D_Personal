#include "..\Public\Navigation.h"

#include "Cell.h"
#include "Shader.h"
#include "GameInstance.h"
#include "VIBuffer_Cell.h"
#include "Transform.h"

_float4x4	CNavigation::m_WorldMatrix{};

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CNavigation::CNavigation(const CNavigation& rhs)
	: CComponent{ rhs }
	, m_Cells{ rhs.m_Cells }
#ifdef _DEBUG
	, m_pShader{ rhs.m_pShader }
#endif
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);
#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif
}

HRESULT CNavigation::Initialize_Prototype(const wstring& strNavigationDataFile)
{

	ifstream fin(strNavigationDataFile, ios::binary);

	if (fin.is_open())
	{
		if (!m_Cells.empty())
		{
			for (auto& pCell : m_Cells)
				Safe_Release(pCell);

			m_Cells.clear();
		}

		_int size = 0;
		fin.read(reinterpret_cast<char*>(&size), sizeof(_int));

		m_Cells.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			_float3 vPoints[3] = {};
			fin.read(reinterpret_cast<char*>(&vPoints), sizeof(_float3) * 3);
			_int vNeighbor[3] = {};
			fin.read(reinterpret_cast<char*>(&vNeighbor), sizeof(_int) * 3);

			CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, i, vNeighbor);

			m_Cells.emplace_back(pCell);
		}
	}
	fin.close();



	//if (FAILED(SetUp_Neighbors()))
	//	return E_FAIL;

#ifdef _DEBUG	
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CNavigation::Initialize_Prototype()
{
	
#ifdef _DEBUG	
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	
	if (nullptr == pArg)
		return S_OK;

	m_iCurrentCellIndex = static_cast<NAVIGATION_DESC*>(pArg)->iCurrentCellIndex;

	return S_OK;
}

void CNavigation::Update(const _float4x4* pWorldMatrix)
{
	m_WorldMatrix = *pWorldMatrix;
}

_bool CNavigation::isMove(_fvector vPosition, _float4* LineDir)
{
	//월드를 가져와서 로컬로 내려준다음 계산한다.
	_vector		vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));


	_int			iNeighborIndex = { -1 };

	/* 셀 안에서 움직였다. */
	if (true == m_Cells[m_iCurrentCellIndex]->isIn(vLocalPos, &iNeighborIndex, LineDir))
		return true;

	/* 셀 밖으로 움직였다. */
	else
	{
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, &iNeighborIndex, LineDir))
				{
					m_iCurrentCellIndex = iNeighborIndex;
					return true;
				}
			}

		}
		else
			return false;
	}
}

_bool CNavigation::isReflect(_fvector vPosition, _float4* LineDir)
{
	//월드를 가져와서 로컬로 내려준다음 계산한다.
	_vector		vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));


	_int			iNeighborIndex = { -1 };

	/* 셀 안에서 움직였다. */
	if (true == m_Cells[m_iCurrentCellIndex]->isIn(vLocalPos, &iNeighborIndex, LineDir))
		return true;

	/* 셀 밖으로 움직였다. */
	else
	{
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, &iNeighborIndex, LineDir))
				{
					m_iCurrentCellIndex = iNeighborIndex;
					return true;
				}
			}

		}
		else
			return false;
	}
}

HRESULT CNavigation::Make_Cell(const _float3* vPoint)
{
	CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoint, m_Cells.size());
	if (nullptr == pCell)
		return E_FAIL;

	m_Cells.emplace_back(pCell);


	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;

	return S_OK;
}

void CNavigation::Snap_Point(_vector vPoint, _float3* StorePoint, _float SnapReach)
{
	for (auto& iter : m_Cells)
	{
		if (iter->Check_Points(vPoint, StorePoint, SnapReach))
			return ;

	}
	
	XMStoreFloat3(StorePoint, vPoint);

}

void CNavigation::Delete_Index(_int CellIndex)
{
	m_Cells.erase(m_Cells.begin() + CellIndex);

	for (size_t i = 0; i < m_Cells.size(); i++)
	{
		m_Cells[i]->NeighborSort(CellIndex);
	}

	for (size_t i = CellIndex; i < m_Cells.size(); i++)
	{
		m_Cells[i]->Sorting(CellIndex);
	}
}

void CNavigation::Save_Data()
{

	ofstream fout;
	fout.open(TEXT("../Bin/bin/NavigationData.bin"), ios::out | ios::binary);
	if (!fout.fail())
	{
		_int size = m_Cells.size();
		fout.write(reinterpret_cast<char*>(&size), sizeof(_int));
		for (auto& pCell : m_Cells)
			pCell->Save_Data(&fout);
	}
	fout.close();
}

void CNavigation::Load_Data()
{
	ifstream fin(TEXT("../Bin/bin/NavigationData.bin"), ios::binary);

	if (fin.is_open())
	{
		if(!m_Cells.empty())
		{
			for (auto& pCell : m_Cells)
				Safe_Release(pCell);

			m_Cells.clear();
		}

		_int size = 0;
		fin.read(reinterpret_cast<char*>(&size), sizeof(_int));

		m_Cells.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			_float3 vPoints[3] = {};
			fin.read(reinterpret_cast<char*>(&vPoints), sizeof(_float3) * 3);
			_int vNeighbor[3] = {};
			fin.read(reinterpret_cast<char*>(&vNeighbor), sizeof(_int) * 3);

			CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, i, vNeighbor);

			m_Cells.emplace_back(pCell);
		}
	}
	fin.close();


}

void CNavigation::Undo_Cell()
{
	_int iCellIndex = m_Cells.size() - 1;
	m_Cells.pop_back();
	for (size_t i = 0; i < m_Cells.size(); i++)
	{
		m_Cells[i]->NeighborSort(iCellIndex);
	}
}
//틱으로 월드 받아오고나서 할것
HRESULT CNavigation::Set_OnNavigation(CTransform* pTransform)
{
	_vector vWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);
	
	_float3 vLocalPos;	
	XMStoreFloat3(&vLocalPos, XMVector3TransformCoord(vWorldPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix))));	

	if (-1 == m_iCurrentCellIndex)
		return S_OK;

	vLocalPos.y=m_Cells[m_iCurrentCellIndex]->Set_OnCell(vLocalPos);

	pTransform->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat3(&vLocalPos), XMLoadFloat4x4(&m_WorldMatrix)));

	return S_OK;
}

_bool CNavigation::Compare_Height(CTransform* pTransform)
{
	_vector vWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vLocalPos;//돌의 로컬
	XMStoreFloat3(&vLocalPos, XMVector3TransformCoord(vWorldPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix))));

	if (-1 == m_iCurrentCellIndex)
		return S_OK;

	_float3 vCellPos{};
	vCellPos.y = m_Cells[m_iCurrentCellIndex]->Set_OnCell(vLocalPos);//셀의 높이

	if (vLocalPos.y > vCellPos.y)
		return false;
	else
	{
		vLocalPos.y = vCellPos.y;

		pTransform->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat3(&vLocalPos), XMLoadFloat4x4(&m_WorldMatrix)));
		return true;
	}

}
//TODO ::0513 구현  준비하셈
_bool CNavigation::Cell_Reflect(_fvector* vDirection, _fvector vPosition)
{

	//월드를 가져와서 로컬로 내려준다음 계산한다.
	_vector		vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));


	_int			iNeighborIndex = { -1 };

	/* 셀 안에서 움직였다. */
	if (true == m_Cells[m_iCurrentCellIndex]->isIn(vLocalPos, &iNeighborIndex))
		return true;

	/* 셀 밖으로 움직였다. */
	else
	{
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, &iNeighborIndex))
				{
					m_iCurrentCellIndex = iNeighborIndex;
					return true;
				}
			}

		}
		else
			return false;
	}
}

#ifdef _DEBUG

HRESULT CNavigation::Render()
{
	if (m_Cells.size() != 0)
	{
		m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);

		m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_VIEW));
		m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::TS_PROJ));

		if (-1 == m_iCurrentCellIndex)
		{

			_float4x4	Matrix = m_WorldMatrix;

			Matrix._42 += 0.1f;

			m_pShader->Bind_Matrix("g_WorldMatrix", &Matrix);

			if (FAILED(m_pShader->Bind_RawValue("g_vColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4))))
				return E_FAIL;

			m_pShader->Begin(0);

			for (auto& pCell : m_Cells)
				pCell->Render();

			return S_OK;
		}

		else
		{
			_float4x4	Matrix = m_WorldMatrix;

			Matrix._42 += 0.15f;

			m_pShader->Bind_Matrix("g_WorldMatrix", &Matrix);

			if (FAILED(m_pShader->Bind_RawValue("g_vColor", &_float4(1.f, 0.0f, 0.f, 1.f), sizeof(_float4))))
				return E_FAIL;

			m_pShader->Begin(0);

			m_Cells[m_iCurrentCellIndex]->Render();

			return S_OK;
		}
	}
	




}

#endif

HRESULT CNavigation::SetUp_Neighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
				pSourCell->SetUp_Neighbor(CCell::LINE_AB, pDestCell);

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
				pSourCell->SetUp_Neighbor(CCell::LINE_BC, pDestCell);

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
				pSourCell->SetUp_Neighbor(CCell::LINE_CA, pDestCell);
		}
	}
	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationDataFile)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strNavigationDataFile)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		return nullptr;
	}

	return pInstance;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNavigation");
		return nullptr;
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		return nullptr;
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif
}
