#include "..\Public\Bounding_Sphere.h"
#include "DebugDraw.h"

CBounding_Sphere::CBounding_Sphere()
{

}

HRESULT CBounding_Sphere::Initialize(const void* pArg)
{
	const BOUNDING_SPHERE_DESC* pDesc = static_cast<const BOUNDING_SPHERE_DESC*>(pArg);

	m_pOriginalDesc = new BoundingSphere(pDesc->vCenter, pDesc->fRadius);
	m_pDesc = new BoundingSphere(*m_pOriginalDesc);

	return S_OK;
}

void CBounding_Sphere::Tick(_fmatrix WorldMatrix)
{
	m_pOriginalDesc->Transform(*m_pDesc, WorldMatrix);
}

_bool CBounding_Sphere::Intersect(CCollider::TYPE eTargetType, CBounding* pTargetBounding)
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

_bool CBounding_Sphere::IntersectRay(_vector* pRayArray, _float* fDist)
{
	_bool			isColl{ false };

	isColl = m_pDesc->Intersects(pRayArray[0], pRayArray[1], *fDist);

	return isColl;
}

#ifdef _DEBUG

HRESULT CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

#endif

CBounding_Sphere* CBounding_Sphere::Create(const void* pArg)
{
	CBounding_Sphere* pInstance = new CBounding_Sphere();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CBounding_Sphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_Sphere::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalDesc);
	Safe_Delete(m_pDesc);
}

