#include "..\Public\VIBuffer_Instance_Point.h"

CVIBuffer_Instance_Point::CVIBuffer_Instance_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Instance{ pDevice, pContext }
{
}

CVIBuffer_Instance_Point::CVIBuffer_Instance_Point(const CVIBuffer_Instance_Point& rhs)
	: CVIBuffer_Instance{ rhs }
{
}

HRESULT CVIBuffer_Instance_Point::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CVIBuffer_Instance_Point::Initialize(void* pArg)
{

	INSTANCE_DESC* InstanceDesc = static_cast<INSTANCE_DESC*>(pArg);

	m_iNumInstance = InstanceDesc->iNumInstance;

	m_RandomNumber = mt19937_64(m_RandomDevice());

	m_InstanceDesc.iNumInstance = InstanceDesc->iNumInstance;
	m_InstanceDesc.vOffsetPos = InstanceDesc->vOffsetPos;
	m_InstanceDesc.vPivotPos = InstanceDesc->vPivotPos;
	m_InstanceDesc.vRange = InstanceDesc->vRange;
	m_InstanceDesc.vSize = InstanceDesc->vSize;
	m_InstanceDesc.vSpeed = InstanceDesc->vSpeed;
	m_InstanceDesc.vLifeTime = InstanceDesc->vLifeTime;
	m_InstanceDesc.isLoop = InstanceDesc->isLoop;
	m_InstanceDesc.fScaleSize = InstanceDesc->fScaleSize;

	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	m_iIndexFormat = DXGI_FORMAT_R16_UINT;
	m_iNumVertexBuffers = 2;
	m_iVertexStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;

	m_iIndexStride = 2;
	m_iIndexCountPerInstance = 1;
	m_iNumIndices = m_iIndexCountPerInstance * m_iNumInstance;
	m_iInstanceStride = sizeof(VTXMATRIX);


#pragma region VERTEX_BUFFER 

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXPOINT* pVertices = new VTXPOINT[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOINT) * m_iNumVertices);

	pVertices[0].vPosition = _float3(0.0f, 0.0f, 0.f);
	pVertices[0].vPSize = _float2(0.1f, 0.3f);

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER 
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion


#pragma region INSTANCE_BUFFER
	m_InstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_InstanceBufferDesc.MiscFlags = 0;
	m_InstanceBufferDesc.StructureByteStride = m_iInstanceStride;

	VTXMATRIX* pInstanceVertices = new VTXMATRIX[m_iNumInstance];
	ZeroMemory(pInstanceVertices, sizeof(VTXMATRIX) * m_iNumInstance);

	m_pSpeeds = new _float[m_iNumInstance];
	ZeroMemory(m_pSpeeds, sizeof(_float) * m_iNumInstance);

	m_pOriginalPositions = new _float3[m_iNumInstance];
	ZeroMemory(m_pOriginalPositions, sizeof(_float3) * m_iNumInstance);

	uniform_real_distribution<float>	RangeX(InstanceDesc->vPivotPos.x - InstanceDesc->vRange.x * 0.5f, InstanceDesc->vPivotPos.x + InstanceDesc->vRange.x * 0.5f);
	uniform_real_distribution<float>	RangeY(InstanceDesc->vPivotPos.y - InstanceDesc->vRange.y * 0.5f, InstanceDesc->vPivotPos.y + InstanceDesc->vRange.y * 0.5f);
	uniform_real_distribution<float>	RangeZ(InstanceDesc->vPivotPos.z - InstanceDesc->vRange.z * 0.5f, InstanceDesc->vPivotPos.z + InstanceDesc->vRange.z * 0.5f);

	uniform_real_distribution<float>	Size(InstanceDesc->vSize.x, InstanceDesc->vSize.y);
	uniform_real_distribution<float>	Speed(InstanceDesc->vSpeed.x, InstanceDesc->vSpeed.y);

	uniform_real_distribution<float>	LifeTime(InstanceDesc->vLifeTime.x, InstanceDesc->vLifeTime.y);



	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float	fSize = Size(m_RandomNumber);
		pInstanceVertices[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		pInstanceVertices[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		pInstanceVertices[i].vLook = _float4(0.f, 0.f, fSize, 0.f);
		pInstanceVertices[i].vTranslation = _float4(RangeX(m_RandomNumber), RangeY(m_RandomNumber), RangeZ(m_RandomNumber), 1.f);
		m_pOriginalPositions[i] = _float3(pInstanceVertices[i].vTranslation.x, pInstanceVertices[i].vTranslation.y, pInstanceVertices[i].vTranslation.z);
		pInstanceVertices[i].vLifeTime.x = LifeTime(m_RandomNumber);

		m_pSpeeds[i] = Speed(m_RandomNumber);
	}
	m_InitialData.pSysMem = pInstanceVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InitialData, &m_pVBInstance)))
		return E_FAIL;


	Safe_Delete_Array(pInstanceVertices);
#pragma endregion

	return S_OK;
}



CVIBuffer_Instance_Point* CVIBuffer_Instance_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext )
{
	CVIBuffer_Instance_Point* pInstance = new CVIBuffer_Instance_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Instance_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Instance_Point::Clone(void* pArg)
{
	CVIBuffer_Instance_Point* pInstance = new CVIBuffer_Instance_Point(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_Instance_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Instance_Point::Free()
{
	__super::Free();
}
