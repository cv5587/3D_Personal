#pragma once

#include"Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Terrain;
class CNavigation;
END

BEGIN(Client)
class CTerrain :
    public CGameObject
{
public:
	typedef struct TERRAIN_DESC :public CGameObject::GAMEOBJECT_DESC
	{
		_int TerrainUV[2];
	}TERRAIN_DESC;
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

	_int* m_TerrainUV = { nullptr };


public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
