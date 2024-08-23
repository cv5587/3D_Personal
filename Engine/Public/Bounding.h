#pragma once

#include "Collider.h"

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct BOUNDING_DESC : public CCollider::COLLIDER_DESC
	{
		_float3		vCenter;
	}BOUNDING_DESC;

protected:
	CBounding();
	virtual ~CBounding() = default;

public:
	virtual void* Get_Desc() = 0;

public:
	virtual void Tick(_fmatrix WorldMatrix) = 0;
public:
	virtual _bool Intersect(CCollider::TYPE eTargetType, CBounding* pTargetBounding) = 0;
	virtual _bool IntersectRay(_vector* pRayArray, _float* fDist) = 0;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) = 0;
#endif

public:
	virtual void Free() override;
};

END