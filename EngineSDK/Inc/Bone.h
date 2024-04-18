#pragma once

#include "Base.h"

/* assimp에서는 뼈를 표현하기위한 타입으로 세가지 타입을 제공해준다. */
/* aiNode, aiBone, aiNodeAnim */

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const _float4x4* Get_CombinedTransformationMatrix() const {
		return &m_CombinedTransformationMatrix;
	}
	const _float4x4* Get_TransformationMatrix() const {
		return &m_TransformationMatrix;
	}
public:
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	_bool Compare_Name(const _char* pName) {
		return !strcmp(m_szName, pName);
	}

public:
	HRESULT Initialize(const aiNode* pBoneNode, _int iParentIndex);
	HRESULT Initialize(ifstream* fin);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);
	HRESULT Save_Bone(ofstream* fout);

private:
	_char				m_szName[MAX_PATH] = "";
	_int					m_iParentBoneIndex = { 0 };
	_float4x4			m_TransformationMatrix;
	_float4x4			m_CombinedTransformationMatrix;


public:
	static CBone* Create(const aiNode* pBoneNode, _int iParentIndex);
	static CBone* Create(ifstream* fin);
	CBone* Clone();
	virtual void Free() override;
};

END