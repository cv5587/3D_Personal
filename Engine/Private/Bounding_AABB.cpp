#include "..\Public\Bounding_AABB.h"
#include "DebugDraw.h"

CBounding_AABB::CBounding_AABB()
{
}

HRESULT CBounding_AABB::Initialize(const void* pArg)
{
	const BOUNDING_AABB_DESC* pDesc = static_cast<const BOUNDING_AABB_DESC*>(pArg);

	m_pOriginalDesc = new BoundingBox(pDesc->vCenter, pDesc->vExtents);
	m_pDesc = new BoundingBox(*m_pOriginalDesc);

	return S_OK;
}

void CBounding_AABB::Tick(_fmatrix WorldMatrix)
{
	//행렬 회전값 제거 하고 월드로 곱해주는 과정 (AABB는 회전값이 들어가있으면 오류가 생김)
	_matrix		TransformMatrix = WorldMatrix;

	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[0]);
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[1]);
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(WorldMatrix.r[2]);

	m_pOriginalDesc->Transform(*m_pDesc, TransformMatrix);//로컬 박스 월드로 변환
}

_bool CBounding_AABB::Intersect(CCollider::TYPE eTargetType, CBounding* pTargetBounding)
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

_bool CBounding_AABB::IntersectRay(_vector* pRayArray, _float* fDist)
{
	_bool			isColl{ false };

	_vector Origin = pRayArray[0];
	_vector Ray = pRayArray[1];

	isColl = m_pDesc->Intersects(Origin, Ray, *fDist);

	return isColl;
}

#ifdef _DEBUG

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

#endif

_bool CBounding_AABB::Intersect_AABB(const BoundingBox* pTargetBounding)
{
	_float3		vSourMin = Compute_Min(m_pDesc);
	_float3		vSourMax = Compute_Max(m_pDesc);

	_float3		vDestMin = Compute_Min(pTargetBounding);
	_float3		vDestMax = Compute_Max(pTargetBounding);

	/* 너비상 비교. */
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return false;

	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return false;

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return false;

	return true;




	return _bool();
}

_float3 CBounding_AABB::Compute_Min(const BoundingBox* pBounding)
{
	return _float3{ pBounding->Center.x - pBounding->Extents.x,
		pBounding->Center.y - pBounding->Extents.y,
		pBounding->Center.z - pBounding->Extents.z };
}

_float3 CBounding_AABB::Compute_Max(const BoundingBox* pBounding)
{
	return _float3{ pBounding->Center.x + pBounding->Extents.x,
		pBounding->Center.y + pBounding->Extents.y,
		pBounding->Center.z + pBounding->Extents.z };
}

CBounding_AABB* CBounding_AABB::Create(const void* pArg)
{
	CBounding_AABB* pInstance = new CBounding_AABB();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CBounding_AABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalDesc);
	Safe_Delete(m_pDesc);
}

