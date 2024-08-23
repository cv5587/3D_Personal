#include "Client_Defines.h"
#include "BlendObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CLensFlare :
    public CBlendObject
{
public:
	typedef struct LENSFLARE_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		const _float4x4* pParentsMatrix;
		_float4x4 fControlMatrix;
	}LENSFLARE_DESC;
private:
	CLensFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLensFlare(const CLensFlare& rhs);
	virtual ~CLensFlare() = default;

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
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float							m_fFrame = {};
	const _float4x4* m_pParentsMatrix = { nullptr };
	_float4x4 m_fControlMatrix = {};
private:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();

public:
	void Reset();
public:
	static CLensFlare* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END