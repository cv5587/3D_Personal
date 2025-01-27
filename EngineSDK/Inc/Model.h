#pragma once
#include "Component.h"
#include "Animation.h"

BEGIN(Engine)

class ENGINE_DLL CModel final:
    public CComponent
{
public :
	enum MODELTYPE{TYPE_NONANIM,TYPE_ANIM,TYPE_END};

	typedef struct tagAnimationDesc
	{
		tagAnimationDesc(_uint iAnimIndex, _bool isLoop)
			: iAnimIndex{ iAnimIndex }, isLoop{ isLoop } {}
		_uint		iAnimIndex = { 0 };
		_bool		isLoop = { false };
	}ANIMATION_DESC;

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_bool Get_AnimFinished() const {
		return m_Animations[m_AnimDesc.iAnimIndex]->Get_Finished();
	}
	const _float4x4* Get_BoneCombinedTransformationMatrix(const _char* pBoneName) const;
	const _float4x4* Get_BoneTransformationMatrix(const _char* pBoneName) const;
	 _float4x4* Get_ControlBoneMatrix(const _char* pBoneName) ;


public:
	virtual HRESULT	Initialize_Prototype(MODELTYPE eModelType, CAnimation::ANIMTYPE eAnimType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT	Initialize(void* pArg) override;
	HRESULT Render(_uint iMeshIndex);

	HRESULT Bind_Material(class CShader* pShaderCom, const _char* pConstantName, _uint iMeshIndex, aiTextureType eMaterialType);
	HRESULT Bind_BoneMatrices(class CShader* pShaderCom, const _char* pConstantName, _uint iMeshIndex);
	HRESULT Bind_BulletBoneMatrices(class CShader* pShaderCom, const _char* pConstantName, _uint iMeshIndex);
	void Play_Animation(_float fTimeDelta);
	void Shift_Animation(_float fTimeDelta);
	void Set_FirstAnimationIndex(const ANIMATION_DESC& AnimDesc) {
		m_PreAnimDesc = AnimDesc;
		m_AnimDesc = AnimDesc;
	}
	void Set_AnimationIndex(const ANIMATION_DESC& AnimDesc) {
		if (m_AnimDesc.iAnimIndex == AnimDesc.iAnimIndex)
			return;

		m_AnimDesc = AnimDesc;
		m_Animations[m_AnimDesc.iAnimIndex]->Reset();
		m_Animations[m_AnimDesc.iAnimIndex]->Shift_Reset();
	}
	void Set_NoShiftAnimationIndex(const ANIMATION_DESC& AnimDesc) {
		if (m_AnimDesc.iAnimIndex == AnimDesc.iAnimIndex)
			return;
		m_PreAnimDesc = m_AnimDesc;
		m_AnimDesc = AnimDesc;
		m_Animations[m_AnimDesc.iAnimIndex]->Reset();
		m_Animations[m_AnimDesc.iAnimIndex]->Shift_Reset();
	}

	void Set_RepeatAnimationIndex() {
		m_Animations[m_AnimDesc.iAnimIndex]->Reset();
		m_Animations[m_AnimDesc.iAnimIndex]->Shift_Reset();
	}
	HRESULT Make_Binary(const wstring FilePath);
	HRESULT Read_Binary( char* FilePath);

	const _float4x4* Get_CameraOffset(const _char* pName);

	_bool Check_AinmTime(_double isTime) {
	return 	m_Animations[m_AnimDesc.iAnimIndex]->Compare_CurrentTime(isTime);
	}
private:
	const aiScene* m_pAIScene = { nullptr };
	Assimp::Importer			m_Importer;
	_float4x4					m_PreTransformMatrix;
	MODELTYPE					m_eModelType = { TYPE_END };
	_float4x4					m_MeshBoneMatrices[512];

private:
	_uint						m_iNumMeshes = { 0 };
	vector<class CMesh*>		m_Meshes;

	_uint						m_iNumMaterials = { 0 };
	vector<MESH_MATERIAL>		m_Materials;

	vector<class CBone*>	m_Bones;

	_uint						m_iNumAnimations = { 0 };
	ANIMATION_DESC				m_AnimDesc{ 0, false };//지금애니메이션
	ANIMATION_DESC				m_PreAnimDesc{ 0, false };//바뀐애니메이션
	vector<class CAnimation*>	m_Animations;

public:
	class CBone* Get_CameraBone(const char* pBoneName);


private:
	HRESULT Ready_Meshes();
	HRESULT Ready_Materials(const _char* pModelFilePath);
	HRESULT Ready_Bones(const aiNode* pAINode, _int iParentIndex);
	HRESULT Ready_Animations(CAnimation::ANIMTYPE eAnimType);


public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eModelType, CAnimation::ANIMTYPE eAnimType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,  char* BinaryFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END