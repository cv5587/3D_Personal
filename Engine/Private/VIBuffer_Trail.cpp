#include "VIBuffer_Trail.h"

CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Trail::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{
	if(nullptr != pArg)
	{
		TRAIL_DESC* TrailDesc = static_cast<CVIBuffer_Trail::TRAIL_DESC*>(pArg);

		m_vInitPosA = TrailDesc->vInitPosA;
		m_vInitPosB = TrailDesc->vInitPosB;
		m_iMaxTrail = TrailDesc->ivtxMaxNum;
		
	}

	m_iNumVertices = 4 + (m_iMaxTrail - 1) * 2;

	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXPOSTEX);


#pragma region VERTEX_BUFFER 

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//¿Ã∞« πª±Ó?
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i].vPosition = _float3{ 0.f, 0.f, 0.f };
		pVertices[i].vTexcoord = _float2(-1.f, -1.f);
	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER 


	m_iIndexStride = 4;
	m_iNumIndices = m_iMaxTrail * 6; //m_iNumVertices * 2 - 2;
	//m_iCurNumIndices = m_iNumVertices * 2 - 2;



	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ulong);

	INDEX32* pIndices = new INDEX32[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(INDEX32) * m_iNumIndices);

	for (size_t iIndex = 0; iIndex < m_iNumIndices; iIndex+=2)
	{
		pIndices[iIndex]._0	= iIndex+3;
		pIndices[iIndex]._1	= iIndex+1;
		pIndices[iIndex]._2	= iIndex;


		pIndices[iIndex+1]._0	= iIndex+2;
		pIndices[iIndex+1]._1	= iIndex+3;
		pIndices[iIndex+1]._2	= iIndex;
	}

	//ZeroMemory(&m_InitialData, sizeof(D3D11_SUBRESOURCE_DATA));	
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

//	m_Trails.clear();
	//m_iCurNumIndices = 0;

	return S_OK;
}

HRESULT CVIBuffer_Trail::Render()
{

	ID3D11Buffer* pVertices[] = {
		m_pVB,
	};

	_uint pStrideVertices[] = {
		m_iVertexStride,
	};

	_uint pStartVertices[] = {
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertices, pStrideVertices, pStartVertices);
	m_pContext->IASetIndexBuffer(m_pIB, m_iIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	m_pContext->DrawIndexed(m_iCurrentIndices, 0, 0);

	return S_OK;
}





HRESULT CVIBuffer_Trail::Tick(_float fTimeDelta, _fmatrix WorldMatrix)
{
	m_fCurrentTime += fTimeDelta;

	if(m_fCurrentTime>m_fTimeAcc)
	{

		VTXPOSTEX* pVertex;
		_float3 vNewPos[2];

		D3D11_MAPPED_SUBRESOURCE SubResource{};
		m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		VTXPOSTEX* vResult = ((VTXPOSTEX*)SubResource.pData);

		XMStoreFloat3(&vNewPos[0], XMVector3TransformCoord(XMLoadFloat3(&m_vInitPosA), WorldMatrix));
		XMStoreFloat3(&vNewPos[1], XMVector3TransformCoord(XMLoadFloat3(&m_vInitPosB), WorldMatrix));

		if (m_iNumVertices <= m_TrailInfos.size() * 2)
			m_TrailInfos.pop_front();

		TRAIL_INFO Desc{};
		Desc.vPos[0] = vNewPos[0];
		Desc.vPos[1] = vNewPos[1];
		m_TrailInfos.push_back(Desc);

		auto tTrailInfo = m_TrailInfos.begin();

		_float vTexcoordX = m_TrailInfos.size() * 2;
		for (size_t i = 0; i < m_TrailInfos.size() * 2;)
		{
			vResult[i].vPosition = (*tTrailInfo).vPos[0];
			vResult[i].vTexcoord = _float2((i / (vTexcoordX - 2.f)), 0.f);
			++i;

			vResult[i].vPosition = (*tTrailInfo).vPos[1];
			vResult[i].vTexcoord = _float2(((i - 1) / (vTexcoordX - 2.f)), 1.f);
			++i;

			++tTrailInfo;
		}

		m_pContext->Unmap(m_pVB, 0);

		m_iCurrentIndices = (m_TrailInfos.size() - 1) * 2 * 3;

		m_fCurrentTime = 0.f;
	}

	return S_OK;
}

CVIBuffer_Trail* CVIBuffer_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Trail::Clone(void* pArg)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();
}

