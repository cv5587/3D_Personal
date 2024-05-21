#pragma once

#include "Component.h"

/* 1. 객체가 움직일 수 있는 길을 삼각형의 집합으로 표현하자. */
/* 2. 셀들의 정보를 기반으로해서 움직일 수 있냐? 없냐? 검사를 수행한다. */

BEGIN(Engine)

class ENGINE_DLL CNavigation final
	: public CComponent
{
public:
	typedef struct
	{
		_int		iCurrentCellIndex = { -1 };
	}NAVIGATION_DESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& strNavigationDataFile);
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Update(const _float4x4* pWorldMatrix);
	_bool isMove(_fvector vPosition, _float4* LineDir=nullptr);
	_bool isReflect(_fvector vPosition, _float4* LineDir=nullptr);
	HRESULT Make_Cell(const _float3* vPoint);
	void Snap_Point(_vector vPoint,_float3* StorePoint,_float SnapReach);
	//툴용
public:
	_int* Get_CurrentCell() { return &m_iCurrentCellIndex; }
	void Delete_Index(_int CellIndex);
	void Save_Data();
	void Load_Data();
	void Undo_Cell();
	void Set_Cells(vector<class CCell*>		Cells){ m_Cells = Cells; }
	vector<class CCell*> Get_Cells() {
		return m_Cells;
	}
	//네비 올라타기
public://월드 받아와서 계산하기. 트랜스폼 가져와서 바로 적용.
	HRESULT Set_OnNavigation(class CTransform* pTransform);
	_bool Compare_Height(class CTransform* pTransform);
	//네비 반사 벡터
public:
	_bool Cell_Reflect(_fvector* vDirection, _fvector vPosition);
#ifdef _DEBUG
public:
	HRESULT Render();
#endif
	void Set_CellIndex(_int Index) { m_iCurrentCellIndex = Index; }
	void Reset_CellIndex() {
		m_iCurrentCellIndex = -1;
	}
private:
	vector<class CCell*>		m_Cells;
	static _float4x4			m_WorldMatrix;
	_int						m_iCurrentCellIndex = { -1 };


#ifdef _DEBUG
	class CShader* m_pShader = { nullptr };
#endif

private:
	HRESULT SetUp_Neighbors();

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationDataFile);
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();

};

END