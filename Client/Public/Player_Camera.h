#pragma once
#include "Client_Defines.h"
#include "Camera.h"
BEGIN(Engine)
class CBone;
END

BEGIN(Client)
class CPlayer_Camera final :
    public CCamera
{
public:
	typedef struct : public CCamera::CAMERA_DESC
	{
		class CBone* pCamBone;
		const _float4x4* pParentMatrix;
	}PLAYER_CAMERA_DESC;

private:
	CPlayer_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Camera(const CPlayer_Camera& rhs);
	virtual ~CPlayer_Camera() = default;
	const _float4x4* m_pParentMatrix;
	_float4x4 m_fixWorld;
	class CBone* m_pCamBone = { nullptr };
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	_vector Get_CamLook();
	void Set_CamMatrix();
public:
	static CPlayer_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END