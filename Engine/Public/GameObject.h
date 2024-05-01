#pragma once

#include "Transform.h"
#include "VIBuffer_Terrain.h"
BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct : public CTransform::TRANSFORM_DESC
	{
		wstring ProtoTypeTag;
		wstring		ModelTag;
		_float4x4		vPrePosition ;

	}GAMEOBJECT_DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Tick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Choice_Render();
public:
	class		CComponent* Get_Transform();
	class		CComponent* Get_Component(const wstring& strComponentTag);

public:
	void		Set_ID(_int ID) {
		m_iRenderID = ID;
	}
	_bool Compare_ID(const _int& ID) {
		if (ID == m_iRenderID)
			return true;
		else
			return false;
	}

public:
	HRESULT Save_Data(ofstream* fout);
	virtual void Make_Description(void* pArg) ;


protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

protected:
	class CGameInstance* m_pGameInstance = { nullptr };
	class CTransform* m_pTransformCom = { nullptr };
	static const _tchar* m_pTransformTag;

protected:
	wstring m_ProtoTypeTag;//참조원형
	wstring m_ModelTag;//모델 태그
	_int m_iRenderID = { -1 };


protected:
	map<const wstring, class CComponent*>		m_Components;

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag,
		CComponent** ppOut, void* pArg = nullptr);



public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END