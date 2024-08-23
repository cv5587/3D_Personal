#pragma once

#include "Base.h"

BEGIN(Engine)

class CQuadTree final : public CBase
{
public:
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_RB, CORNER_LB, CORNER_END };
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END };

private:
	CQuadTree();
	virtual ~CQuadTree() = default;

public:
	HRESULT Initialize(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	void Culling(const _float4* pVerticesPositions, _uint* pIndices, _uint* iNumIndices);
	void Make_Neighbors();


private:
	CQuadTree* m_pChildren[CORNER_END] = { nullptr };
	CQuadTree* m_pNeighbors[NEIGHBOR_END] = { nullptr, nullptr, nullptr, nullptr };
	_uint					m_iCenterIndex = { 0 };
	_uint					m_iCorners[CORNER_END] = { 0 };
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	_bool isDraw(const _float4* pVertexPositions);

public:
	static CQuadTree* Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	virtual void Free() override;
};

END