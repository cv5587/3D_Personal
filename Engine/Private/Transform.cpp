#include "..\Public\Transform.h"

#include "Shader.h"
#include"Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent{ pDevice, pContext }
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent{ rhs }, m_WorldMatrix(rhs.m_WorldMatrix)
{
}



void CTransform::Set_State(STATE eState, _fvector vState)
{
	_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	WorldMatrix.r[eState] = vState;

	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

void CTransform::Set_State_Matrix( _fmatrix mState)
{
	_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	WorldMatrix= mState;	

	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		TRANSFORM_DESC* pTransformDesc = (TRANSFORM_DESC*)pArg;

		m_fSpeedPerSec = pTransformDesc->fSpeedPerSec;
		m_fRotationPerSec = pTransformDesc->fRotationPerSec;
	}


	return S_OK;
}

void CTransform::Scaling(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	Set_State(STATE_RIGHT, Get_State(STATE_RIGHT) * fScaleX);
	Set_State(STATE_UP, Get_State(STATE_UP) * fScaleY);
	Set_State(STATE_LOOK, Get_State(STATE_LOOK) * fScaleZ);
}

void CTransform::Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * fScaleX);
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * fScaleY);
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * fScaleZ);
}

HRESULT CTransform::Go_Straight(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	_vector vCalPosition = Get_State(STATE_POSITION);
	vCalPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	_float4 NorDir{};
	if (nullptr == pNavigation ? true : pNavigation->isMove(vCalPosition, &NorDir))
		Set_State(STATE_POSITION, vCalPosition);
	else
	{

		_vector SlideDir = XMVector3Normalize(vLook) - XMVector3Dot(XMVector3Normalize(vLook), XMVector3Normalize(XMLoadFloat4(&NorDir))) * XMVector3Normalize(XMLoadFloat4(&NorDir));
		_vector	 SlidePosition = Get_State(STATE_POSITION);
		SlidePosition += XMVector3Normalize(SlideDir) * m_fSpeedPerSec * fTimeDelta;


		if (pNavigation->isMove(SlidePosition))
			Set_State(STATE_POSITION, SlidePosition);
	}

	return S_OK;
}

HRESULT CTransform::Go_Backward(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	_vector vCalPosition = Get_State(STATE_POSITION);
	vCalPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	_float4 NorDir{};
	if (nullptr == pNavigation ? true : pNavigation->isMove(vCalPosition, &NorDir))
		Set_State(STATE_POSITION, vCalPosition);
	else
	{

		_vector SlideDir = XMVector3Normalize(vLook) - XMVector3Dot(XMVector3Normalize(vLook), XMVector3Normalize(XMLoadFloat4(&NorDir))) * XMVector3Normalize(XMLoadFloat4(&NorDir));
		_vector	 SlidePosition = Get_State(STATE_POSITION);
		SlidePosition -= XMVector3Normalize(SlideDir) * m_fSpeedPerSec * fTimeDelta;


		if (pNavigation->isMove(SlidePosition))
			Set_State(STATE_POSITION, SlidePosition);
	}

	return S_OK;
}

HRESULT CTransform::Go_Left(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vRight = Get_State(STATE_RIGHT);

	//vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;
	//_float4 SlideDir{};
	//if (nullptr == pNavigation ? true : pNavigation->isMove(vPosition, &SlideDir))
	//	Set_State(STATE_POSITION, vPosition);

	_vector vCalPosition = Get_State(STATE_POSITION);	
	vCalPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	_float4 NorDir{};
	if (nullptr == pNavigation ? true : pNavigation->isMove(vCalPosition, &NorDir))
		Set_State(STATE_POSITION, vCalPosition);
	else
	{

		_vector SlideDir = XMVector3Normalize(vRight) - XMVector3Dot(XMVector3Normalize(vRight), XMVector3Normalize(XMLoadFloat4(&NorDir))) * XMVector3Normalize(XMLoadFloat4(&NorDir));
		_vector	 SlidePosition = Get_State(STATE_POSITION);
		SlidePosition -= XMVector3Normalize(SlideDir) * m_fSpeedPerSec * fTimeDelta;


		if (pNavigation->isMove(SlidePosition))
			Set_State(STATE_POSITION, SlidePosition);
	}

	return S_OK;
}

HRESULT CTransform::Go_Right(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vRight = Get_State(STATE_RIGHT);

	_vector vCalPosition = Get_State(STATE_POSITION);
	vCalPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	_float4 NorDir{};
	if (nullptr == pNavigation ? true : pNavigation->isMove(vCalPosition, &NorDir))
		Set_State(STATE_POSITION, vCalPosition);
	else
	{

		_vector SlideDir = XMVector3Normalize(vRight) - XMVector3Dot(XMVector3Normalize(vRight), XMVector3Normalize(XMLoadFloat4(&NorDir))) * XMVector3Normalize(XMLoadFloat4(&NorDir));
		_vector	 SlidePosition = Get_State(STATE_POSITION);
		SlidePosition += XMVector3Normalize(SlideDir) * m_fSpeedPerSec * fTimeDelta;


		if (pNavigation->isMove(SlidePosition))
			Set_State(STATE_POSITION, SlidePosition);
	}
	return S_OK;
}

HRESULT CTransform::Go_LeftStraight(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);
	_vector vLeft = Get_State(STATE_RIGHT) * -1.f;

	_vector vCalPosition = Get_State(STATE_POSITION);
	vCalPosition += XMVector3Normalize(vLook+ vLeft) * m_fSpeedPerSec * fTimeDelta;

	_float4 NorDir{};
	if (nullptr == pNavigation ? true : pNavigation->isMove(vCalPosition, &NorDir))
		Set_State(STATE_POSITION, vCalPosition);
	else
	{

		_vector SlideDir = XMVector3Normalize(vLook + vLeft) - XMVector3Dot(XMVector3Normalize(vLook + vLeft), XMVector3Normalize(XMLoadFloat4(&NorDir))) * XMVector3Normalize(XMLoadFloat4(&NorDir));
		_vector	 SlidePosition = Get_State(STATE_POSITION);
		SlidePosition += XMVector3Normalize(SlideDir) * m_fSpeedPerSec * fTimeDelta;


		if (pNavigation->isMove(SlidePosition))
			Set_State(STATE_POSITION, SlidePosition);
	}

	return S_OK;
}

HRESULT CTransform::Go_RightStraight(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);
	_vector vRight = Get_State(STATE_RIGHT);

	_vector vCalPosition = Get_State(STATE_POSITION);
	vCalPosition += XMVector3Normalize(vLook + vRight) * m_fSpeedPerSec * fTimeDelta;

	_float4 NorDir{};
	if (nullptr == pNavigation ? true : pNavigation->isMove(vCalPosition, &NorDir))
		Set_State(STATE_POSITION, vCalPosition);
	else
	{

		_vector SlideDir = XMVector3Normalize(vLook + vRight) - XMVector3Dot(XMVector3Normalize(vLook + vRight), XMVector3Normalize(XMLoadFloat4(&NorDir))) * XMVector3Normalize(XMLoadFloat4(&NorDir));
		_vector	 SlidePosition = Get_State(STATE_POSITION);
		SlidePosition += XMVector3Normalize(SlideDir) * m_fSpeedPerSec * fTimeDelta;


		if (pNavigation->isMove(SlidePosition))
			Set_State(STATE_POSITION, SlidePosition);
	}

	return S_OK;
}

HRESULT CTransform::Go_LeftBackward(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);
	_vector vRight = Get_State(STATE_RIGHT);

	_vector vCalPosition = Get_State(STATE_POSITION);
	vCalPosition -= XMVector3Normalize(vLook + vRight) * m_fSpeedPerSec * fTimeDelta;

	_float4 NorDir{};
	if (nullptr == pNavigation ? true : pNavigation->isMove(vCalPosition, &NorDir))
		Set_State(STATE_POSITION, vCalPosition);
	else
	{

		_vector SlideDir = XMVector3Normalize(vLook + vRight) - XMVector3Dot(XMVector3Normalize(vLook + vRight), XMVector3Normalize(XMLoadFloat4(&NorDir))) * XMVector3Normalize(XMLoadFloat4(&NorDir));
		_vector	 SlidePosition = Get_State(STATE_POSITION);
		SlidePosition -= XMVector3Normalize(SlideDir) * m_fSpeedPerSec * fTimeDelta;


		if (pNavigation->isMove(SlidePosition))
			Set_State(STATE_POSITION, SlidePosition);
	}

	return S_OK;
}

HRESULT CTransform::Go_RightBackward(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);
	_vector vLeft = Get_State(STATE_RIGHT) * -1.f;

	_vector vCalPosition = Get_State(STATE_POSITION);
	vCalPosition -= XMVector3Normalize(vLook + vLeft) * m_fSpeedPerSec * fTimeDelta;

	_float4 NorDir{};
	if (nullptr == pNavigation ? true : pNavigation->isMove(vCalPosition, &NorDir))
		Set_State(STATE_POSITION, vCalPosition);
	else
	{

		_vector SlideDir = XMVector3Normalize(vLook + vLeft) - XMVector3Dot(XMVector3Normalize(vLook + vLeft), XMVector3Normalize(XMLoadFloat4(&NorDir))) * XMVector3Normalize(XMLoadFloat4(&NorDir));
		_vector	 SlidePosition = Get_State(STATE_POSITION);
		SlidePosition -= XMVector3Normalize(SlideDir) * m_fSpeedPerSec * fTimeDelta;


		if (pNavigation->isMove(SlidePosition))
			Set_State(STATE_POSITION, SlidePosition);
	}

	return S_OK;
}

HRESULT CTransform::LookAt(_fvector vTargetPosition)
{
	_float3 vScaled = Get_Scaled();

	_vector	vLook = vTargetPosition - Get_State(STATE_POSITION);

	_vector	vRight = XMVector3Cross(XMVectorSet(0.0f, 1.f, 0.f, 0.f), vLook);

	_vector vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);

	return S_OK;
}

HRESULT CTransform::LookAt_For_LandObject(_fvector vTargetPosition)
{
	_float3 vScaled = Get_Scaled();

	_vector	vLook = vTargetPosition - Get_State(STATE_POSITION);

	_vector	vRight = XMVector3Cross(XMVectorSet(0.0f, 1.f, 0.f, 0.f), vLook);

	vLook = XMVector3Cross(vRight, XMVectorSet(0.0f, 1.f, 0.f, 0.f));

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);	
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);

	// XMVector3Dot();

	return S_OK;
}

HRESULT CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix =  XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_float3		vScaled = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Rotaion_Reset()
{
	_float3		vScaled = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix		RotationMatrix = XMMatrixIdentity();

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Rotate(_uint _rot, _float fTimeDelta)
{
	//rulp
	//_float3 vScaled = Get_Scaled();

	_vector vRight = Get_State(STATE_RIGHT);
	_vector vUp = Get_State(STATE_UP);
	_vector vLook = Get_State(STATE_LOOK);

	switch (_rot)
	{
	case Engine::CTransform::ROT_X:
	{
		_matrix Col = XMMatrixRotationAxis(vRight, XMConvertToRadians(fTimeDelta * m_fRotationPerSec));
		Set_State(STATE_UP, XMVector3TransformNormal(vUp, Col));
		Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, Col));
		break;
	}
	case Engine::CTransform::ROT_Y:
	{
		_matrix Col = XMMatrixRotationAxis(vUp, XMConvertToRadians(fTimeDelta * m_fRotationPerSec));
		Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, Col));
		Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, Col));
		break;
	}
	case Engine::CTransform::ROT_Z:
	{
		_matrix Col = XMMatrixRotationAxis(vLook, XMConvertToRadians(fTimeDelta * m_fRotationPerSec));
		Set_State(STATE_UP, XMVector3TransformNormal(vUp, Col));
		Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, Col));
		break;
	}
	case Engine::CTransform::ROT_END:
		break;
	default:
		break;
	}



	return S_OK;
}

HRESULT CTransform::Rotate_Radian(_uint _rot, _float fRadian)
{

	//rulp
	//_float3 vScaled = Get_Scaled();

	_vector vRight = Get_State(STATE_RIGHT);
	_vector vUp = Get_State(STATE_UP);
	_vector vLook = Get_State(STATE_LOOK);

	switch (_rot)
	{
	case Engine::CTransform::ROT_X:
	{
		_matrix Col = XMMatrixRotationAxis(vRight, XMConvertToRadians(fRadian));
		Set_State(STATE_UP, XMVector3TransformNormal(vUp, Col));
		Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, Col));
		break;
	}
	case Engine::CTransform::ROT_Y:
	{
		_matrix Col = XMMatrixRotationAxis(vUp, XMConvertToRadians(fRadian));
		Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, Col));
		Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, Col));
		break;
	}
	case Engine::CTransform::ROT_Z:
	{
		_matrix Col = XMMatrixRotationAxis(vLook, XMConvertToRadians(fRadian));
		Set_State(STATE_UP, XMVector3TransformNormal(vUp, Col));
		Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, Col));
		break;
	}
	case Engine::CTransform::ROT_END:
		break;
	default:
		break;
	}



	return S_OK;
}

HRESULT CTransform::Throw(_float4* fDir, _float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	
	//중력계산 추가
	_float4 CalfDir = *fDir;

	_vector Direction = XMLoadFloat4(&CalfDir);
	
	_float Y = XMVectorGetY(Direction);

	Y -= 0.2f *  fTimeDelta;

	Direction = XMVectorSetY(Direction, Y);

	vPosition += XMVector3Normalize(Direction) * m_fSpeedPerSec * fTimeDelta;

	XMStoreFloat4(fDir , Direction);


	Set_State(STATE_POSITION, vPosition);



	return S_OK;
}

HRESULT CTransform::Patrol(_float fTimeDelta, CNavigation* pNavigation)
{

	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	_float4 SlideDir{};
	if (nullptr == pNavigation ? true : pNavigation->isMove(vPosition, &SlideDir))
		Set_State(STATE_POSITION, vPosition);
	else
	{
		//이거 내비게이션에 cell_reflect 쓸거임
		Rotate_Radian(STATE_UP, XMConvertToRadians(180.f));
	}

	return S_OK;
}

HRESULT CTransform::Escape(_fmatrix mPlayer, _float fTimeDelta,   CNavigation* pNavigation)
{

	_vector vPosition = Get_State(CTransform::STATE_POSITION);

	_vector vTargetPos, vScale, vRotation;
	if (XMMatrixDecompose(&vScale, &vRotation, &vTargetPos, mPlayer))
	{
		_vector vDir = vPosition - vTargetPos;

		vPosition += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

		_float4 SlideDir{};
		if (nullptr == pNavigation ? true : pNavigation->isMove(vPosition, &SlideDir))
			Set_State(STATE_POSITION, vPosition);
		else
		{
			//이거 내비게이션에 cell_reflect 쓸거임
			Rotate_Radian(STATE_UP, XMConvertToRadians(180.f));
		}
	}

	return S_OK;
}



HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);
}

void CTransform::Make_Description(void* pArg)
{
	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	pDesc->fRotationPerSec = m_fRotationPerSec;
	pDesc->fSpeedPerSec = m_fSpeedPerSec;

}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
