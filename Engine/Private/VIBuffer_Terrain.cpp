#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVIBuffer{pDevice,pContext}
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs)
    :CVIBuffer{rhs},
    m_iNumVerticesX{rhs.m_iNumVerticesX},
     m_iNumVerticesZ{rhs.m_iNumVerticesZ}
{
	lstrcpy(m_strHeightMapFilePath, rhs.m_strHeightMapFilePath);
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const wstring& strHeightMapFilePath)
{
	wsprintf(m_strHeightMapFilePath, strHeightMapFilePath.c_str());	

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	
	if (nullptr != pArg)
	{
		m_pTerrainUV = new _int[2];	

		_int* sTerrainUV = (_int*)pArg;
		m_pTerrainUV[0] = sTerrainUV[0];
		m_pTerrainUV[1] = sTerrainUV[1];
	
	}
	
	_ulong			dwByte = { 0 };

	HANDLE			hFile = CreateFile(m_strHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER		fh;
	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);

	BITMAPINFOHEADER		ih;
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	_uint* pPixel = new _uint[ih.biWidth * ih.biHeight];
	ReadFile(hFile, pPixel, sizeof(_uint) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXNORTEX);
	if (nullptr != m_pTerrainUV)
	{
		if (m_pTerrainUV[0] > ih.biWidth || m_pTerrainUV[1] > ih.biHeight)
			return E_FAIL;
		m_iNumVertices = m_pTerrainUV[0] * m_pTerrainUV[1];
		m_iNumVerticesX = m_pTerrainUV[0];
		m_iNumVerticesZ = m_pTerrainUV[1];
	}
	else
	{
		m_iNumVertices = ih.biWidth * ih.biHeight;
		m_iNumVerticesX = ih.biWidth;
		m_iNumVerticesZ = ih.biHeight;
	}

	m_iIndexStride = 4;
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;


#pragma region VERTEX_BUFFER 

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];

	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	m_pVertexPositions = new _float4[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float4) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;
			_uint		heightiIndex = i * ih.biWidth + j;	
			pVertices[iIndex].vPosition = _float3(j, ((pPixel[heightiIndex] & 0x000000ff) - 125.f) , i);
			pVertices[iIndex].vNormal = _float3(0.0f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
			m_pVertexPositions[iIndex] = _float4(pVertices[iIndex].vPosition.x, pVertices[iIndex].vPosition.y, pVertices[iIndex].vPosition.z, 1.f);
		}
	}
#pragma endregion

#pragma region INDEX_BUFFER 

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSour, vDest, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSour = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDest = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDest = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}

	for (size_t i = 0; i < m_iNumVertices; i++)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pPixel);
	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);
#pragma endregion


	return S_OK;
}

_float CVIBuffer_Terrain::Compute_Height(const _float3& vLocalPos)
{
	_uint			iIndex = _uint(vLocalPos.z) * m_iNumVerticesX + _uint(vLocalPos.x);

	_uint			iIndices[] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex
	};

	_float		fWidth = vLocalPos.x - m_pVertexPositions[iIndices[0]].x;
	_float		fDepth = m_pVertexPositions[iIndices[0]].z - vLocalPos.z;

	_vector		vPlane = XMVectorZero();

	/* ¿À¸¥ÂÊ À§ »ï°¢Çü ¾È. */
	if (fWidth > fDepth)
	{
		vPlane = XMPlaneFromPoints(XMLoadFloat4(&m_pVertexPositions[iIndices[0]]),
			XMLoadFloat4(&m_pVertexPositions[iIndices[1]]),
			XMLoadFloat4(&m_pVertexPositions[iIndices[2]]));
	}
	/* ¿ÞÂÊ ¾Æ·¡ »ï°¢Çü ¾È. */
	else
	{
		vPlane = XMPlaneFromPoints(XMLoadFloat4(&m_pVertexPositions[iIndices[0]]),
			XMLoadFloat4(&m_pVertexPositions[iIndices[2]]),
			XMLoadFloat4(&m_pVertexPositions[iIndices[3]]));
	}

	/*
	ax + by + cz + d = 0
	y = (-ax - cz - d) / b
	*/
	return (-XMVectorGetX(vPlane) * vLocalPos.x - XMVectorGetZ(vPlane) * vLocalPos.z - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);
}

void CVIBuffer_Terrain::Save_Terrain_UV(ofstream* fout)
{
	fout->write((char*)m_pTerrainUV, sizeof(_int) * 2);
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strHeightMapFilePath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strHeightMapFilePath)))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{

	Safe_Delete_Array(m_pTerrainUV);
	__super::Free();
}
