#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent{ pDevice, pContext }
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent{ rhs }
	, m_iNumTextures { rhs.m_iNumTextures }
	, m_Textures{ rhs.m_Textures }
{
	for (auto& pSRV : m_Textures)
		Safe_AddRef(pSRV);	
}

HRESULT CTexture::Initialize_Prototype(const wstring & strTextureFilePath, _uint iNumTextures)
{
	_tchar			szTextureFilePath[MAX_PATH] = TEXT("");
	_tchar			szEXT[MAX_PATH] = TEXT("");

	m_Textures.reserve(iNumTextures);

	m_iNumTextures = iNumTextures;

	for (size_t i = 0; i < iNumTextures; i++)
	{
		/* ..\Bin\Resources\Textures\Default0.jpg */
		wsprintf(szTextureFilePath, strTextureFilePath.c_str(), i);

		wsprintf(m_szTextureFilePath, szTextureFilePath);

		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

		ID3D11ShaderResourceView*		pSRV = { nullptr };

		HRESULT			hr{};

		if (false == lstrcmp(szEXT, TEXT(".dds")))		
			hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);

		// else if(false == lstrcmp(szEXT, TEXT(".tga")))

		else
			hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);

		if (FAILED(hr))
			return E_FAIL;

		m_Textures.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize_Prototype(ifstream* fin)
{
	fin->read(reinterpret_cast<char*>(&m_iNumTextures), sizeof(_uint));

	fin->read(reinterpret_cast<char*>(m_szTextureFilePath), sizeof(_tchar)*MAX_PATH);	

	m_Textures.reserve(m_iNumTextures);

	_tchar			szEXT[MAX_PATH] = TEXT("");
	for (size_t i = 0; i < m_iNumTextures; i++)
	{
		_wsplitpath_s(m_szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

		ID3D11ShaderResourceView* pSRV = { nullptr };

		HRESULT			hr{};

		if (false == lstrcmp(szEXT, TEXT(".dds")))
			hr = CreateDDSTextureFromFile(m_pDevice, m_szTextureFilePath, nullptr, &pSRV);

		// else if(false == lstrcmp(szEXT, TEXT(".tga")))

		else
			hr = CreateWICTextureFromFile(m_pDevice, m_szTextureFilePath, nullptr, &pSRV);

		if (FAILED(hr))
			return E_FAIL;

		m_Textures.push_back(pSRV);
	}
	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_Textures[iTextureIndex]);

}

HRESULT CTexture::Save_Texture(ofstream* fout)
{
	fout->write(reinterpret_cast<char*>(&m_iNumTextures), sizeof(_uint));

	fout->write(reinterpret_cast<char*>(m_szTextureFilePath), sizeof(_tchar) * MAX_PATH);

	return S_OK;
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed To Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream* fin)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(fin)))
	{
		MSG_BOX("Failed To Load : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_Textures)
		Safe_Release(pSRV);	
	m_Textures.clear();
}
