#include "..\Public\VIBuffer_Instance.h"

CVIBuffer_Instance::CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Instance::CVIBuffer_Instance(const CVIBuffer_Instance& rhs)
	: CVIBuffer{ rhs }
//	, m_iNumInstance{ rhs.m_iNumInstance }
//	, m_iInstanceStride{ rhs.m_iInstanceStride }
//	, m_iIndexCountPerInstance{ rhs.m_iIndexCountPerInstance }
//	, m_InstanceBufferDesc{ rhs.m_InstanceBufferDesc }
//	, m_pSpeeds{ rhs.m_pSpeeds }
//	, m_pOriginalPositions{ rhs.m_pOriginalPositions }
//	, m_InstanceDesc{ rhs.m_InstanceDesc }

{
//	m_pDevice->CreateBuffer(&m_InstanceBufferDesc, nullptr, &m_pVBInstance);
//	m_pContext->CopyResource(m_pVBInstance, rhs.m_pVBInstance);
}

HRESULT CVIBuffer_Instance::Initialize_Prototype(const INSTANCE_DESC& InstanceDesc)
{
	m_iNumInstance = InstanceDesc.iNumInstance;

	m_RandomNumber = mt19937_64(m_RandomDevice());

	return S_OK;
}

HRESULT CVIBuffer_Instance::Initialize(void* pArg)
{


	return S_OK;
}

HRESULT CVIBuffer_Instance::Bind_Buffers()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint					iVertexStrides[] = {
		m_iVertexStride,
		m_iInstanceStride
	};

	_uint					iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_iIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

HRESULT CVIBuffer_Instance::Render()
{
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Instance::Spread(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;

		pVertices[i].vLifeTime.y += fTimeDelta;

		_vector			vDir = XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_InstanceDesc.vOffsetPos), 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeeds[i] * fTimeDelta);

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			if (true == m_InstanceDesc.isLoop)
			{
				pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
				pVertices[i].vLifeTime.y = 0.f;
			}
		}

	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instance::Drop(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;
		pVertices[i].vTranslation.y -= m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vTranslation.x -= m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vTranslation.z += m_pSpeeds[i] * fTimeDelta;
		pVertices[i].vLifeTime.y += fTimeDelta;

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			if (true == m_InstanceDesc.isLoop)
			{
				pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
				pVertices[i].vLifeTime.y = 0.f;
			}
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instance::Up(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;

		pVertices[i].vLifeTime.y += fTimeDelta;


		_vector vEndPos = XMVector3TransformCoord(XMLoadFloat4(&m_InstanceDesc.vStartPos), XMLoadFloat4x4(m_InstanceDesc.pParentMatrix));
		//_vector			vDir = XMVectorSetW(XMLoadFloat3(&m_InstanceDesc.vOffsetPos) - XMLoadFloat4(&pVertices[i].vTranslation), 0.f);
		_vector			vDir = XMVectorSetW(XMLoadFloat4x4(m_InstanceDesc.pParentMatrix).r[3] - vEndPos, 0.f);
		//_vector			vUpper = XMVectorSetW(XMLoadFloat4x4(m_InstanceDesc.pParentMatrix).r[3] - XMLoadFloat4(&pVertices[i].vTranslation), 0.f);

		//	_vector Up = (XMLoadFloat4(&pVertices[i].vUp));
		_vector vUpper = XMVector3Normalize(vDir);

		_vector Look = (XMLoadFloat4(&pVertices[i].vLook));
		Look = XMVector3Normalize(Look);

		_vector Right = XMVector3Cross(vUpper, Look);
		Right = XMVector3Normalize(Right);


		_vector Up = XMVector3Cross(Look, Right);
		Up = XMVector3Normalize(Up);

		XMStoreFloat4(&pVertices[i].vUp, Up);
		XMStoreFloat4(&pVertices[i].vLook, Look);
		XMStoreFloat4(&pVertices[i].vRight, Right);


		//XMStoreFloat4(& pVertices[i].vRight, XMVector3Cross(XMLoadFloat4(&pVertices[i].vUp), XMLoadFloat4(&pVertices[i].vLook)));
		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeeds[i] * fTimeDelta);

		if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			if (true == m_InstanceDesc.isLoop)
			{
				pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
				pVertices[i].vLifeTime.y = 0.f;
			}
		}

	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instance::Reset()
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;

		pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
		pVertices[i].vLifeTime.y = 0.f;
			
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Instance::Scaling(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;

		_float fXSize = XMVectorGetX(XMVector4Length(XMLoadFloat4(&pVertices[i].vRight)));
		fXSize += m_InstanceDesc.fScaleSize*fTimeDelta;	
		_float fYSize = XMVectorGetX(XMVector4Length(XMLoadFloat4(&pVertices[i].vUp)));
		fYSize += m_InstanceDesc.fScaleSize*fTimeDelta;
		_float fZSize = XMVectorGetX(XMVector4Length(XMLoadFloat4(&pVertices[i].vLook)));
		fZSize += m_InstanceDesc.fScaleSize * fTimeDelta;

		
		XMStoreFloat4(&pVertices[i].vRight, XMVector4Normalize(XMLoadFloat4(&pVertices[i].vRight)) * fXSize);
		XMStoreFloat4(&pVertices[i].vUp, XMVector4Normalize(XMLoadFloat4(&pVertices[i].vUp)) * fYSize);
		XMStoreFloat4(&pVertices[i].vLook, XMVector4Normalize(XMLoadFloat4(&pVertices[i].vLook)) * fZSize);

//		pVertices[i].vTranslation = _float4(m_pOriginalPositions[i].x, m_pOriginalPositions[i].y, m_pOriginalPositions[i].z, 1.f);
	//	pVertices[i].vLifeTime.y = 0.f;

	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

_bool CVIBuffer_Instance::Get_Live()
{
	if (true != m_InstanceDesc.isLoop)
	{
		D3D11_MAPPED_SUBRESOURCE		SubResource{};

		m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		for (size_t i = 0; i < m_iNumInstance; i++)
		{
			VTXMATRIX* pVertices = (VTXMATRIX*)SubResource.pData;

			if (pVertices[i].vLifeTime.y < pVertices[i].vLifeTime.x)
			{
				m_pContext->Unmap(m_pVBInstance, 0);
				return true;
			}
			
		}
	}
	else
	{
		return true;
	}
	m_pContext->Unmap(m_pVBInstance, 0);

	return false;
}

void CVIBuffer_Instance::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pSpeeds);
	Safe_Delete_Array(m_pOriginalPositions);


	Safe_Release(m_pVBInstance);
}
