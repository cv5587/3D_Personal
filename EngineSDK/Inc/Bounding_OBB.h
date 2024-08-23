#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_OBB final : public CBounding
{
public:
	typedef struct BOUNDING_OBB_DESC : public CBounding::BOUNDING_DESC
	{
		_float3		vExtents;
		_float3		vRotation;
	}BOUNDING_OBB_DESC;
private:
	CBounding_OBB();
	virtual ~CBounding_OBB() = default;

public:
	virtual void* Get_Desc() override {
		return m_pDesc;
	}

public:
	HRESULT Initialize(const void* pArg);
	virtual void Tick(_fmatrix WorldMatrix) override;
	virtual _bool Intersect(CCollider::TYPE eTargetType, CBounding* pTargetBounding) override;
	virtual _bool IntersectRay(_vector* pRayArray, _float* fDist) override;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif

private:
	BoundingOrientedBox* m_pOriginalDesc = { nullptr };
	BoundingOrientedBox* m_pDesc = { nullptr };

public:
	static CBounding_OBB* Create(const void* pArg);
	virtual void Free() override;
};

END