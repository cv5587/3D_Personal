#include "..\Public\Mesh.h"
#include "Bone.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{

}

CMesh::CMesh(const CMesh& rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CMesh::Initialize(CModel::MODELTYPE eModelType, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, const vector<class CBone*>& Bones)
{
	strcpy_s(m_szName, pAIMesh->mName.data);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;

	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iNumVertexBuffers = 1;

	m_iNumVertices = pAIMesh->mNumVertices;

	m_iIndexStride = 4;
	m_iNumIndices = pAIMesh->mNumFaces * 3;

#pragma region VERTEX_BUFFER 

	HRESULT			hr = eModelType == CModel::TYPE_NONANIM ? Ready_Vertices_For_NonAnimMesh(pAIMesh, PreTransformMatrix) : Ready_Vertices_For_AnimMesh(pAIMesh, Bones);

	if (FAILED(hr))
		return E_FAIL;



#pragma endregion

#pragma region INDEX_BUFFER 
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	m_IBufferDesc = m_BufferDesc;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	m_SaveIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_SaveIndices, sizeof(_uint) * m_iNumIndices);

	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		pIndices[iNumIndices++] = m_SaveIndices[iNumIndices] = pAIMesh->mFaces[i].mIndices[0];
		pIndices[iNumIndices++] = m_SaveIndices[iNumIndices] = pAIMesh->mFaces[i].mIndices[1];
		pIndices[iNumIndices++] = m_SaveIndices[iNumIndices] = pAIMesh->mFaces[i].mIndices[2];
	}

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize(CModel::MODELTYPE eModelType, ifstream* fin)
{
	fin->read(reinterpret_cast<char*>(m_szName), MAX_PATH);

	fin->read(reinterpret_cast<char*>(&m_iMaterialIndex), sizeof(_uint));

	fin->read(reinterpret_cast<char*>(&m_iNumBones), sizeof(_uint));

	fin->read(reinterpret_cast<char*>(&m_iNumVertexBuffers), sizeof(_uint));
	
	fin->read(reinterpret_cast<char*>(&m_ePrimitiveTopology), sizeof(D3D11_PRIMITIVE_TOPOLOGY));
	
	fin->read(reinterpret_cast<char*>(&m_iIndexFormat), sizeof(DXGI_FORMAT));
	
#pragma region VERTEX_BUFFER 


		fin->read(reinterpret_cast<char*>(&m_iNumVertices), sizeof(_uint));

		fin->read(reinterpret_cast<char*>(&m_iIndexStride), sizeof(_uint));

		fin->read(reinterpret_cast<char*>(&m_iNumIndices), sizeof(_uint));

		fin->read(reinterpret_cast<char*>(&m_iVertexStride), sizeof(_uint));

		fin->read(reinterpret_cast<char*>(&m_BufferDesc.ByteWidth), sizeof(_uint));
		fin->read(reinterpret_cast<char*>(&m_BufferDesc.Usage), sizeof(D3D11_USAGE));
		fin->read(reinterpret_cast<char*>(&m_BufferDesc.BindFlags), sizeof(_uint));
		fin->read(reinterpret_cast<char*>(&m_BufferDesc.CPUAccessFlags), sizeof(_uint));
		fin->read(reinterpret_cast<char*>(&m_BufferDesc.MiscFlags), sizeof(_uint));
		fin->read(reinterpret_cast<char*>(&m_BufferDesc.StructureByteStride), sizeof(_uint));



		if (CModel::TYPE_ANIM == eModelType)
			{
				m_SaveAnimVertices = new VTXANIMMESH[m_iNumVertices];

				fin->read(reinterpret_cast<char*>(m_SaveAnimVertices), sizeof(VTXANIMMESH) * m_iNumVertices);
				//블렌드웨이트 왜 0이ㅓ냐
				for (size_t i = 0; i < m_iNumVertices; i++)
				{
					VTXANIMMESH p = m_SaveAnimVertices[i];
				}
				m_InitialData.pSysMem = m_SaveAnimVertices;
				

				_uint NumBoneIndices;
				fin->read(reinterpret_cast<char*>(&NumBoneIndices), sizeof(_uint));
				for (size_t i = 0; i < NumBoneIndices; i++)
				{
					_uint Boneidx;
					fin->read(reinterpret_cast<char*>(&Boneidx), sizeof(_uint));
					m_BoneIndices.emplace_back(Boneidx);
				}

				_uint NumOffset;
				fin->read(reinterpret_cast<char*>(&NumOffset), sizeof(_uint));
				for (size_t i = 0; i < NumOffset; i++)
				{
					_float4x4 Offset;
					fin->read(reinterpret_cast<char*>(&Offset), sizeof(_float4x4));
					m_OffsetMatrices.emplace_back(Offset);
				}
			}
		else
		{
				m_SaveVertices = new VTXMESH[m_iNumVertices];
				fin->read(reinterpret_cast<char*>(m_SaveVertices), sizeof(VTXMESH) * m_iNumVertices);

				m_InitialData.pSysMem = m_SaveVertices;
		}

		if (FAILED(__super::Create_Buffer(&m_pVB)))
			return E_FAIL;

#pragma endregion

#pragma region INDEX_BUFFER 

	fin->read(reinterpret_cast<char*>(&m_BufferDesc.ByteWidth), sizeof(_uint));
	fin->read(reinterpret_cast<char*>(&m_BufferDesc.Usage), sizeof(D3D11_USAGE));
	fin->read(reinterpret_cast<char*>(&m_BufferDesc.BindFlags), sizeof(_uint));
	fin->read(reinterpret_cast<char*>(&m_BufferDesc.CPUAccessFlags), sizeof(_uint));
	fin->read(reinterpret_cast<char*>(&m_BufferDesc.MiscFlags), sizeof(_uint));
	fin->read(reinterpret_cast<char*>(&m_BufferDesc.StructureByteStride), sizeof(_uint));

	m_SaveIndices = new _uint[m_iNumIndices];
	fin->read(reinterpret_cast<char*>(m_SaveIndices), sizeof(_uint) * m_iNumIndices);

	m_InitialData.pSysMem = m_SaveIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	//Safe_Delete_Array(m_SaveIndices);
#pragma endregion

	return S_OK;
}

void CMesh::Fill_Matrices(vector<class CBone*>& Bones, _float4x4* pMeshBoneMatrices)
{
	for (size_t i = 0; i < m_iNumBones; i++)
	{
		XMStoreFloat4x4(&pMeshBoneMatrices[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * XMLoadFloat4x4(Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix()));
	}
}

HRESULT CMesh::Ready_Vertices_For_NonAnimMesh(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{
	m_iVertexStride = sizeof(VTXMESH);

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	m_VBufferDesc = m_BufferDesc;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_SaveVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(m_SaveVertices, sizeof(VTXMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition,
			XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix)));

		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));

		m_SaveVertices[i].vPosition = pVertices[i].vPosition;
		m_SaveVertices[i].vNormal = pVertices[i].vNormal;
		m_SaveVertices[i].vTexcoord = pVertices[i].vTexcoord;
		m_SaveVertices[i].vTangent = pVertices[i].vTangent;
	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_Vertices_For_AnimMesh(const aiMesh* pAIMesh, const vector<class CBone*>& Bones)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	m_VBufferDesc = m_BufferDesc;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	m_SaveAnimVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(m_SaveAnimVertices, sizeof(VTXANIMMESH) * m_iNumVertices);	

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));

		m_SaveAnimVertices[i].vPosition = pVertices[i].vPosition;
		m_SaveAnimVertices[i].vNormal = pVertices[i].vNormal;
		m_SaveAnimVertices[i].vTexcoord = pVertices[i].vTexcoord;
		m_SaveAnimVertices[i].vTangent = pVertices[i].vTangent;
	}

	/* 이 메시에 영향을 주는 뼈의 갯수를 가져온다. */
	m_iNumBones = pAIMesh->mNumBones;

	/* 뼈들를 순회하면서 각각의 뼈가 어떤 정점들에게 영향을 주는가를 캐치한다. */
	for (size_t i = 0; i < m_iNumBones; i++)
	{
		/* pAIBone = i번째 뼈다 */
		aiBone* pAIBone = pAIMesh->mBones[i];

		/*pAIBone->mOffsetMatrix*/
		_float4x4		OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_OffsetMatrices.emplace_back(OffsetMatrix);

		_uint		iBoneIndex = { 0 };

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
			{
				if (true == pBone->Compare_Name(pAIBone->mName.data))
					return true;

				++iBoneIndex;

				return false;
			});

		m_BoneIndices.emplace_back(iBoneIndex);

		/* pAIBone->mNumWeights : i번째 뼈가 몇개의 정점들에게 영향을 주는가? */
		/* j루프 : 이 뼈가 영향을 주는 정점들을 순회한다. */
		for (size_t j = 0; j < pAIBone->mNumWeights; j++)
		{
			aiVertexWeight		VertexWeight = pAIBone->mWeights[j];

			/* VertexWeight.mVertexId : 이 뼈가 영향을 주는 정점들 중, j번째 정점의 인덱스 */
			/* pVertices[VertexWeight.mVertexId].vBlendIndices: 이 정점에게 영햐응ㄹ 주는 뼈의 인덱스를 최대 네개 저장한다. */
			if (0.0f == pVertices[VertexWeight.mVertexId].vBlendWeights.x)
			{
				
				m_SaveAnimVertices[VertexWeight.mVertexId].vBlendIndices.x = pVertices[VertexWeight.mVertexId].vBlendIndices.x = i;
				m_SaveAnimVertices[VertexWeight.mVertexId].vBlendWeights.x = pVertices[VertexWeight.mVertexId].vBlendWeights.x = VertexWeight.mWeight;
			}

			else if (0.0f == pVertices[VertexWeight.mVertexId].vBlendWeights.y)
			{
				m_SaveAnimVertices[VertexWeight.mVertexId].vBlendIndices.y = pVertices[VertexWeight.mVertexId].vBlendIndices.y = i;
				m_SaveAnimVertices[VertexWeight.mVertexId].vBlendWeights.y =pVertices[VertexWeight.mVertexId].vBlendWeights.y = VertexWeight.mWeight;
			}

			else if (0.0f == pVertices[VertexWeight.mVertexId].vBlendWeights.z)
			{
				m_SaveAnimVertices[VertexWeight.mVertexId].vBlendIndices.z = pVertices[VertexWeight.mVertexId].vBlendIndices.z = i;
				m_SaveAnimVertices[VertexWeight.mVertexId].vBlendWeights.z =pVertices[VertexWeight.mVertexId].vBlendWeights.z = VertexWeight.mWeight;
			}

			else
			{
				m_SaveAnimVertices[VertexWeight.mVertexId].vBlendIndices.w = pVertices[VertexWeight.mVertexId].vBlendIndices.w = i;
				m_SaveAnimVertices[VertexWeight.mVertexId].vBlendWeights.w =pVertices[VertexWeight.mVertexId].vBlendWeights.w = VertexWeight.mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;


		_uint		iBoneIndex = { 0 };

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
			{
				if (true == pBone->Compare_Name(m_szName))
					return true;

				++iBoneIndex;

				return false;
			});

		_float4x4		OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

		m_OffsetMatrices.emplace_back(OffsetMatrix);

		m_BoneIndices.emplace_back(iBoneIndex);
	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Save_Meshes(ofstream* fout, CModel::MODELTYPE eModelType)
{
	fout->write(reinterpret_cast<char*>(m_szName), MAX_PATH);	

	fout->write(reinterpret_cast<char*>(&m_iMaterialIndex), sizeof(_uint));

	fout->write(reinterpret_cast<char*>(&m_iNumBones), sizeof(_uint));

	fout->write(reinterpret_cast<char*>(&m_iNumVertexBuffers), sizeof(_uint));
	
	fout->write(reinterpret_cast<char*>(&m_ePrimitiveTopology), sizeof(D3D11_PRIMITIVE_TOPOLOGY));
	
	fout->write(reinterpret_cast<char*>(&m_iIndexFormat), sizeof(DXGI_FORMAT));

	fout->write(reinterpret_cast<char*>(&m_iNumVertices), sizeof(_uint));

	fout->write(reinterpret_cast<char*>(&m_iIndexStride), sizeof(_uint));

	fout->write(reinterpret_cast<char*>(&m_iNumIndices), sizeof(_uint));

	fout->write(reinterpret_cast<char*>(&m_iVertexStride), sizeof(_uint));

	fout->write(reinterpret_cast<char*>(&m_VBufferDesc.ByteWidth), sizeof(_uint));
	fout->write(reinterpret_cast<char*>(&m_VBufferDesc.Usage), sizeof(D3D11_USAGE));
	fout->write(reinterpret_cast<char*>(&m_VBufferDesc.BindFlags), sizeof(_uint));
	fout->write(reinterpret_cast<char*>(&m_VBufferDesc.CPUAccessFlags), sizeof(_uint));
	fout->write(reinterpret_cast<char*>(&m_VBufferDesc.MiscFlags), sizeof(_uint));
	fout->write(reinterpret_cast<char*>(&m_VBufferDesc.StructureByteStride), sizeof(_uint));

	if (CModel::TYPE_ANIM == eModelType)
	{

		fout->write(reinterpret_cast<char*>(m_SaveAnimVertices), sizeof(VTXANIMMESH) * m_iNumVertices);
		
		_uint NumBoneIndices= m_BoneIndices.size();
		fout->write(reinterpret_cast<char*>(&NumBoneIndices), sizeof(_uint));

		for (auto& Boneidx : m_BoneIndices)
			 fout->write(reinterpret_cast<char*>(&Boneidx), sizeof(_uint));

		_uint NumOffsetMatrices = m_OffsetMatrices.size();
		fout->write(reinterpret_cast<char*>(&NumOffsetMatrices), sizeof(_uint));

		for (auto& offset : m_OffsetMatrices)
			fout->write(reinterpret_cast<char*>(&offset), sizeof(_float4x4));
	}
	else
	{
		fout->write(reinterpret_cast<char*>(m_SaveVertices), sizeof(VTXMESH) * m_iNumVertices);
	}

	fout->write(reinterpret_cast<char*>(&m_IBufferDesc.ByteWidth), sizeof(_uint));
	fout->write(reinterpret_cast<char*>(&m_IBufferDesc.Usage), sizeof(D3D11_USAGE));
	fout->write(reinterpret_cast<char*>(&m_IBufferDesc.BindFlags), sizeof(_uint));
	fout->write(reinterpret_cast<char*>(&m_IBufferDesc.CPUAccessFlags), sizeof(_uint));
	fout->write(reinterpret_cast<char*>(&m_IBufferDesc.MiscFlags), sizeof(_uint));
	fout->write(reinterpret_cast<char*>(&m_IBufferDesc.StructureByteStride), sizeof(_uint));

	fout->write(reinterpret_cast<char*>(m_SaveIndices), sizeof(_uint) * m_iNumIndices);


	return S_OK;
}

//HRESULT CMesh::Load_Meshes(ifstream* fin)
//{
//	fin->read(m_szName, MAX_PATH);
//	fin->read(reinterpret_cast<char*>(&m_iMaterialIndex), sizeof(_uint));
//	fin->read(reinterpret_cast<char*>(&m_iNumBones), sizeof(_uint));
//
//	_uint NumBoneidx;
//	fin->read(reinterpret_cast<char*>(&NumBoneidx), sizeof(_uint));
//
//	m_BoneIndices.reserve(NumBoneidx);
//
//	for (size_t i = 0; i < NumBoneidx; i++)
//	{
//		_uint Boneidx{0};
//		fin->read(reinterpret_cast<char*>(&Boneidx), sizeof(_uint));
//		m_BoneIndices.emplace_back(Boneidx);
//	}
//
//	_uint NumOffset = {0};
//	fin->read(reinterpret_cast<char*>(&NumOffset), sizeof(_uint));
//
//	m_OffsetMatrices.reserve(NumOffset);
//
//	for (size_t i = 0; i < NumOffset; i++)
//	{
//		_float4x4 offset;
//		fin->read(reinterpret_cast<char*>(&offset), sizeof(_float4x4));
//		m_OffsetMatrices.emplace_back(offset);
//	}
//
//	return S_OK;
//}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::MODELTYPE eModelType, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, const vector<class CBone*>& Bones)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eModelType, pAIMesh, PreTransformMatrix, Bones)))
	{
		MSG_BOX("Failed To Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::MODELTYPE eModelType, ifstream* fin)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eModelType,fin)))
	{
		MSG_BOX("Failed To Load : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	return nullptr;
}

void CMesh::Free()
{
	Safe_Delete_Array(m_SaveVertices);
	Safe_Delete_Array(m_SaveAnimVertices);
	Safe_Delete_Array(m_SaveIndices);
	__super::Free();

}

