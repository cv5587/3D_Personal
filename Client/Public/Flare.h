#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CFlare final:
    public CPartObject
{
public:
	typedef struct FLARE_DESC : public CPartObject::PARTOBJ_DESC
	{
		_bool* pLit;
		_float* pBurnTime;
		const _float4x4* pCombinedTransformationMatrix;
	}FLARE_DESC;

private:
	CFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFlare(const CFlare& rhs);
	virtual ~CFlare() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
private:
	const _float4x4* m_pSocketMatrix = { nullptr };
	_bool* m_pBodyAnimFinish = { nullptr };

	_float m_fBurnTime ={0.f};
	_bool* m_pLit = { nullptr };//∫“ ∫Ÿ¿Ω?§§§§

	CGameObject* m_pEffect = { nullptr };
private:
	void Set_AnimationState();
public:
	HRESULT Add_Components();
	HRESULT Bind_ShaderResources();
	void Add_Particle();

protected:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

public:
	static CFlare* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END