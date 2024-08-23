#include "..\Public\Light_Manager.h"
#include "Light.h"

CLight_Manager::CLight_Manager()
{
}

const LIGHT_DESC* CLight_Manager::Get_LightDesc(_uint iIndex) const
{
	if (iIndex >= m_Lights.size())
		return nullptr;

	auto	iter = m_Lights.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Set_Switch(_uint iIndex, _bool bSwitch)
{
	if (iIndex >= m_Lights.size())
		return E_FAIL;

	auto	iter = m_Lights.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	(*iter)->Set_Switch(bSwitch);
	return S_OK;
}

HRESULT CLight_Manager::Set_Position(_uint iIndex, _fvector vPosition)
{
	const LIGHT_DESC* pDesc =  Get_LightDesc(iIndex);
	if (nullptr == pDesc)
		return E_FAIL;

	XMStoreFloat4(const_cast<_float4*>( & (pDesc->vPosition)), (_vector)vPosition);

	return S_OK;
}

HRESULT CLight_Manager::Set_Range(_uint iIndex, _float fRange)
{
	const LIGHT_DESC* pDesc = Get_LightDesc(iIndex);
	if (nullptr == pDesc)
		return E_FAIL;

	_vector vRange = { fRange ,0.f ,0.f , 0.f };
	XMStoreFloat(const_cast<_float*>(&pDesc->fRange), vRange);	

	return S_OK;
}

HRESULT CLight_Manager::Initialize()
{

	return S_OK;
}

HRESULT CLight_Manager::Rotate(_float fTimeDelta)
{
		_vector vPosition = XMLoadFloat4(&Get_LightDesc(0)->vPosition);
		vPosition=XMVectorSetW(vPosition, 1.f);
		_vector vParents = XMVectorSet(800.f, 0.f, 380.f , 1.f);
		_vector Sub = XMVectorSubtract(vPosition, vParents);

		//_float		vScaled = XMVector3Length(vPosition).m128_f32[0];

		_float fRotationPerSec = XMConvertToRadians(1.f);	//속도 조절 알아서

		_matrix		RotationMatrix = XMMatrixRotationAxis(_vector{ 1.f, 0.f, 0.f ,0.f }, fRotationPerSec * fTimeDelta);
	
		_vector vRotateVec =XMVector3TransformCoord(Sub, RotationMatrix);

		_vector vFinPosition = XMVectorAdd(vRotateVec, vParents);
		vFinPosition = XMVectorSetW(vFinPosition, 1.f);
		//_matrix LightPos = XMMatrixTranslation(XMVectorGetX(vPosition), XMVectorGetY(vPosition), XMVectorGetZ(vPosition));

		//_matrix Result = LightPos * RotationMatrix * Parents;
		
		XMStoreFloat4(const_cast<_float4*>(&Get_LightDesc(0)->vPosition), vFinPosition);
		//XMStoreFloat4(const_cast<_float4*>(&Get_LightDesc(0)->vPosition), XMVector3TransformCoord(vPosition, RotationMatrix));



	return S_OK;
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& LightDesc)
{
	CLight* pLight = CLight::Create(LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.emplace_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{
		if (pLight->Get_Switch())
			pLight->Render(pShader, pVIBuffer);

	}

	return S_OK;
}

CLight_Manager* CLight_Manager::Create()
{
	CLight_Manager* pInstance = new CLight_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLight_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
