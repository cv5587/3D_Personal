#pragma once
#include "Bounding.h"

BEGIN(Engine)
class CBounding_UI final:
    public CBounding
{
public:
	typedef struct : public CBounding::BOUNDING_DESC
	{
		_float3		vExtents;
	}BOUNDING_UI_DESC;
private:
	CBounding_UI();
	virtual ~CBounding_UI() = default;

public:
	virtual void* Get_Desc() override {
		return m_pDesc;
	}

public:
	HRESULT Initialize(const void* pArg);
	virtual void Tick(_fmatrix WorldMatrix) override;
	virtual _bool Intersect(CCollider::TYPE eTargetType, CBounding* pTargetBounding) override;
	virtual _bool IntersectRay(FXMVECTOR Origin, FXMVECTOR Direction, float& Dist) override;
#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif

private:
	BoundingBox* m_pOriginalDesc = { nullptr };//������ ������ ��ġ
	BoundingBox* m_pDesc = { nullptr };//���尡 ��������ġ 

private:
	_float3 Compute_Min(const BoundingBox* pTargetBounding);
	_float3 Compute_Max(const BoundingBox* pTargetBounding);

public:
	static CBounding_UI* Create(const void* pArg);
	virtual void Free() override;
};

END