#include "..\Public\Bounding_OBB.h"
#include "DebugDraw.h"

CBounding_OBB::CBounding_OBB()
{
}

HRESULT CBounding_OBB::Initialize(const void* pArg)
{
	const BOUNDING_OBB_DESC* pDesc = static_cast<const BOUNDING_OBB_DESC*>(pArg);

	_float4		vQuaternion;

	XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYaw(pDesc->vRotation.x, pDesc->vRotation.y, pDesc->vRotation.z));

	m_pOriginalDesc = new BoundingOrientedBox(pDesc->vCenter, pDesc->vExtents, vQuaternion);
	m_pDesc = new BoundingOrientedBox(*m_pOriginalDesc);

	return S_OK;
}

void CBounding_OBB::Tick(_fmatrix WorldMatrix)
{
	m_pOriginalDesc->Transform(*m_pDesc, WorldMatrix);
}

_bool CBounding_OBB::Intersect(CCollider::TYPE eTargetType, CBounding* pTargetBounding)
{
	_bool			isColl{ false };

	switch (eTargetType)
	{
	case CCollider::TYPE_AABB:
		isColl = m_pDesc->Intersects(*(static_cast<BoundingBox*>(pTargetBounding->Get_Desc())));
		break;
	case CCollider::TYPE_OBB:
		isColl = m_pDesc->Intersects(*(static_cast<BoundingOrientedBox*>(pTargetBounding->Get_Desc())));
		break;
	case CCollider::TYPE_SPHERE:
		isColl = m_pDesc->Intersects(*(static_cast<BoundingSphere*>(pTargetBounding->Get_Desc())));
		break;
	}

	return isColl;
}

_bool CBounding_OBB::IntersectRay(_vector* pRayArray, _float* fDist)
{
	_bool			isColl{ false };


	isColl = m_pDesc->Intersects(pRayArray[0], pRayArray[1], *fDist);

	return isColl;
}


#ifdef _DEBUG

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

#endif

CBounding_OBB* CBounding_OBB::Create(const void* pArg)
{
	CBounding_OBB* pInstance = new CBounding_OBB();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CBounding_OBB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_OBB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalDesc);
	Safe_Delete(m_pDesc);
}

