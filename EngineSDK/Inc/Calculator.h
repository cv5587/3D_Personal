#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CCalculator :
    public CBase
{
	enum {FS_NEAR,FS_FAR, FS_LEFT, FS_RIGHT , FS_TOP,FS_BOT,FS_END};
private:
	CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWinSizeX, _uint iWinSizeY);
	virtual ~CCalculator() = default;

public:
	 HRESULT Initialize(HWND hWnd);
	_vector Picking_on_Terrain(HWND hWnd, _matrix TerrainWorldMatrixInverse, _matrix mViewMatrixInverse, _matrix mProjMatrixInverse,_float4* pVtxPos,_int* pTerrainUV, _float* pWinSize);
	 _bool Pick_Object(_matrix InverseView, _matrix InverseProj,  vector< const _float4x4*>* ObPos, _float radius);
	 _vector Picking_HitScreen();
	 _int Picking_IDScreen();
	 _int Picking_UIIDScreen();
	 _vector Picking_UI(_fmatrix ProjM);
	 void World_MouseRay(_vector* RayArray);
	 HRESULT Tick();
public:
	_bool Get_PickPos(_float4* pPickPos);
	 _bool Compare_Float4(_float4 f1, _float4 f2);	
private:
	_float Compute_ProjZ(_float fX, _float fY);
private:
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*			m_pGameInstance = { nullptr };
	HWND									m_hWnd = {};
	_uint										m_iWinSizeX = { 0 };
	_uint										m_iWinSizeY = { 0 };
	_bool							m_isSuccess = { false };
	_float4							m_vPickPos = {};

	ID3D11Texture2D* m_pTexture2D = { nullptr };



	 _float3 m_FrustumPoints[8] = {  };

	 //XMPlaneFromPoints();
public:
	static CCalculator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	virtual void Free() override;
};
END
