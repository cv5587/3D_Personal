#pragma once
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Terrain final:
    public CVIBuffer
{

private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& strHeightMapFilePath);
	virtual HRESULT Initialize(void* pArg) override;
	virtual _float Compute_Height(const _float3& vLocalPos) override;

public:
	void Save_Terrain_UV(ofstream* fout);

private:
	_uint				m_iNumVerticesX = { 0 };
	_uint				m_iNumVerticesZ = { 0 };
	_int*				 m_pTerrainUV = {nullptr};
	 _tchar			m_strHeightMapFilePath[MAX_PATH] = TEXT("");

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strHeightMapFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END
