#include "Calculator.h"
#include "GameInstance.h"
CCalculator::CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWinSizeX, _uint iWinSizeY)
	:m_pGameInstance{CGameInstance::GetInstance()}
	,m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_iWinSizeX{ iWinSizeX }
	, m_iWinSizeY{ iWinSizeY }
{
	Safe_AddRef(m_pGameInstance);	
	Safe_AddRef(m_pDevice);	
	Safe_AddRef(m_pContext);	
}

HRESULT CCalculator::Initialize(HWND hWnd)
{
	//near,far,left,right,top,bottom
	 m_FrustumPoints[0] = { -1.f,1.f,0.f };
	 m_FrustumPoints[1] = { 1.f,1.f,0.f};
	 m_FrustumPoints[2] = { 1.f,-1.f,0.f };
	 m_FrustumPoints[3] = { -1.f,-1.f,0.f };
	 m_FrustumPoints[4] = { -1.f,1.f,1.f };
	 m_FrustumPoints[5] = { 1.f,1.f,1.f };
	 m_FrustumPoints[6] = { 1.f,-1.f,1.f };
	 m_FrustumPoints[7] = { -1.f,-1.f,1.f };

	 m_hWnd = hWnd;

	 _uint		iNumViews = { 1 };

	 D3D11_VIEWPORT		ViewportDesc={};

	 m_pContext->RSGetViewports(&iNumViews, &ViewportDesc);

	 m_iWinSizeX = ViewportDesc.Width;
	 m_iWinSizeY = ViewportDesc.Height;

	 D3D11_TEXTURE2D_DESC		TextureDesc={};

	 ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	 TextureDesc.Width = m_iWinSizeX;
	 TextureDesc.Height = m_iWinSizeY;
	 TextureDesc.MipLevels = 1;
	 TextureDesc.ArraySize = 1;
	 TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;

	 TextureDesc.SampleDesc.Quality = 0;
	 TextureDesc.SampleDesc.Count = 1;

	 TextureDesc.Usage = D3D11_USAGE_STAGING;
	 TextureDesc.BindFlags = 0;
	 TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	 TextureDesc.MiscFlags = 0;

	 if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		 return E_FAIL;

	return S_OK;
}

_vector CCalculator::Picking_on_Terrain(HWND hWnd, _matrix TerrainWorldMatrixInverse, _matrix mViewMatrixInverse, _matrix mProjMatrixInverse, _float4* pVtxPos, _int* pTerrainUV,_float* pWinSize)
{

	POINT ptmouse = {};
	GetCursorPos(&ptmouse);
	ScreenToClient(hWnd, &ptmouse);


	//뷰포트-투영-뷰스페이스-월드
	_vector	mousepos = {};

	//투영으로 내림
	mousepos.m128_f32[0] = ((2.0f * (_float)ptmouse.x) / (_float)pWinSize[0]) - 1.0f;
	mousepos.m128_f32[1] = (1.0f - ((2.0f * (_float)ptmouse.y) / (_float)pWinSize[1]));
	mousepos.m128_f32[2] = 0.f;

	//뷰스페이스로 내려줌
	mousepos = XMVector3TransformCoord(mousepos, mProjMatrixInverse);

	_vector Raypos, Raydir;
	Raypos = { 0.f,0.f,0.f };
	Raydir = mousepos - Raypos;

	//월드로 내려줌
	Raypos = XMVector3TransformCoord(Raypos, mViewMatrixInverse);
	Raydir = XMVector3TransformNormal(Raydir, mViewMatrixInverse);


	//로컬로 내려줌
	
	Raypos = XMVector3TransformCoord(Raypos, TerrainWorldMatrixInverse);
	Raydir = XMVector3TransformNormal(Raydir, TerrainWorldMatrixInverse);
	Raydir = XMVector3Normalize(Raydir);


	_uint 	VtxIdx[3]{};
	float fDist(0.f);

	for (int i = 0; i < pTerrainUV[1] - 1; i++)
	{
		for (int j = 0; j < pTerrainUV[0] - 1; j++)
		{
			_uint 	dwIndex = _uint(i * pTerrainUV[0] + j);

			VtxIdx[0] = dwIndex + (_uint)pTerrainUV[0];
			VtxIdx[1] = dwIndex + (_uint)pTerrainUV[0] + 1;
			VtxIdx[2] = dwIndex + 1;

			if (TriangleTests::Intersects(
				(_fvector)Raypos, (_fvector)Raydir,
				(_fvector)XMLoadFloat4(&pVtxPos[VtxIdx[0]]), (_gvector)XMLoadFloat4(&pVtxPos[VtxIdx[1]]),
				(_hvector)XMLoadFloat4(&pVtxPos[VtxIdx[2]]), fDist)
				)
			{
				_vector d = {};
				d = (Raydir * fDist) + Raypos;
				return d;
			}
			// 왼쪽 아래
			VtxIdx[0] = dwIndex + (_uint)pTerrainUV[0];
			VtxIdx[1] = dwIndex + 1;
			VtxIdx[2] = dwIndex;

			if (TriangleTests::Intersects(
				(_fvector)Raypos, (_fvector)Raydir,
				(_fvector)XMLoadFloat4(&pVtxPos[VtxIdx[0]]), (_gvector)XMLoadFloat4(&pVtxPos[VtxIdx[1]]),
				(_hvector)XMLoadFloat4(&pVtxPos[VtxIdx[2]]), fDist)
				)
			{
				_vector d = {};
				d = (Raydir * fDist) + Raypos;
				return d;
			}


		}
	}

	return _vector{ 0.f,0.f,0.f,0.f };
}


_bool CCalculator::Pick_Object(_matrix InverseView, _matrix InverseProj, vector< const _float4x4*>* ObPos,_float radius)
{
	
	_matrix matVP;	
	matVP=XMMatrixMultiply(InverseProj, InverseView);	

	_vector vFrustumPoint[8] = {};

	vFrustumPoint[0]=XMVector3TransformCoord(XMLoadFloat3(&m_FrustumPoints[0]), matVP);
	vFrustumPoint[1]=XMVector3TransformCoord(XMLoadFloat3(&m_FrustumPoints[1]), matVP);
	vFrustumPoint[2]=XMVector3TransformCoord(XMLoadFloat3(&m_FrustumPoints[2]), matVP);
	vFrustumPoint[3]=XMVector3TransformCoord(XMLoadFloat3(&m_FrustumPoints[3]), matVP);
	vFrustumPoint[4]=XMVector3TransformCoord(XMLoadFloat3(&m_FrustumPoints[4]), matVP);
	vFrustumPoint[5]=XMVector3TransformCoord(XMLoadFloat3(&m_FrustumPoints[5]), matVP);
	vFrustumPoint[6]=XMVector3TransformCoord(XMLoadFloat3(&m_FrustumPoints[6]), matVP);
	vFrustumPoint[7]=XMVector3TransformCoord(XMLoadFloat3(&m_FrustumPoints[7]), matVP);

	_vector vPlanes[FS_END] = {};

	vPlanes[FS_NEAR]=XMPlaneFromPoints(vFrustumPoint[0], vFrustumPoint[1], vFrustumPoint[2]);	
	vPlanes[FS_FAR]=XMPlaneFromPoints(vFrustumPoint[4], vFrustumPoint[5], vFrustumPoint[6]);
	vPlanes[FS_LEFT]=XMPlaneFromPoints(vFrustumPoint[4], vFrustumPoint[0], vFrustumPoint[3]);
	vPlanes[FS_RIGHT]=XMPlaneFromPoints(vFrustumPoint[1], vFrustumPoint[5], vFrustumPoint[6]);
	vPlanes[FS_TOP]=XMPlaneFromPoints(vFrustumPoint[4], vFrustumPoint[5], vFrustumPoint[1]);
	vPlanes[FS_BOT]=XMPlaneFromPoints(vFrustumPoint[6], vFrustumPoint[7], vFrustumPoint[3]);


	for (int i = 0; i < ObPos->size(); i++)
	{
		 _vector pos = XMLoadFloat4x4((*ObPos)[i]).r[3];

		 for (size_t i = 0; i < FS_END; i++)
		 {
			 //하나라도 절두체에 걸치거나 안에 있으면
			 //체크해야될 옵젝 리스트에 포함시키자.
			 if ((XMVector3Dot(vPlanes[i], pos)).m128_f32[0] + vPlanes[i].m128_f32[3] < radius)
			 {

				 break;
			 }

			 			 
		 }
	}
	return false;
	//XMVector3Dot


}

_vector CCalculator::Picking_HitScreen()
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);


	//뷰포트-투영-뷰스페이스-월드
	_vector	MousePos = {};

	//투영으로 내림
	MousePos = XMVectorSetX(MousePos, ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f);
	MousePos = XMVectorSetY(MousePos, ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f);

	//float vDepth = m_pGameInstance->Compute_ProjZ(ptMouse, m_pHitScreenTexture);

	//if (vDepth == -1.f)
	//	return XMVectorSet(-1.f, -1.f, -1.f, -1.f);

	//MousePos = XMVectorSetZ(MousePos, vDepth);
	MousePos = XMVectorSetW(MousePos, 1.f);	


	MousePos = XMVector4Transform(MousePos, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::TS_PROJ));
	MousePos = XMVector4Transform(MousePos, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::TS_VIEW));

	_float per = 1.f / MousePos.m128_f32[3];

	MousePos = XMVectorScale(MousePos, per);

	return MousePos;
}

_int CCalculator::Picking_IDScreen()
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);


	if (ptMouse.x < 0)
		ptMouse.x = 0;
	if (ptMouse.x >= m_iWinSizeX)
		ptMouse.x = m_iWinSizeX;
	if (ptMouse.y < 0)
		ptMouse.y = 0;
	if (ptMouse.y >= m_iWinSizeY)
		ptMouse.y = m_iWinSizeY;

	m_pGameInstance->Copy_Resource(TEXT("Target_ObjectID"), m_pTexture2D);
	if (nullptr == m_pTexture2D)
		return -1;

	D3D11_MAPPED_SUBRESOURCE		SubResources{};

	m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &SubResources);

	/* 내 마우스 좌표가 존재하는 위치에 있는 텍스쳐 픽셀의 인덱스 */
	_uint		iIndex = ptMouse.y * m_iWinSizeX + ptMouse.x;

	if (iIndex > 1280 * 720)
		return -1;

	//int* pData = static_cast<int*>(SubResources.pData);

	//int vResult = pData[iIndex * 4];	


	_float4		vResult = ((_float4*)SubResources.pData)[iIndex];

	//int* pData = static_cast<int*>(SubResources.pData);

	_int iResult = (_int)vResult.x;

	m_pContext->Unmap(m_pTexture2D, 0);

	return iResult;
}

_int CCalculator::Picking_UIIDScreen()
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);


	if (ptMouse.x < 0)
		ptMouse.x = 0;
	if (ptMouse.x >= m_iWinSizeX)
		ptMouse.x = m_iWinSizeX;
	if (ptMouse.y < 0)
		ptMouse.y = 0;
	if (ptMouse.y >= m_iWinSizeY)
		ptMouse.y = m_iWinSizeY;

	m_pGameInstance->Copy_Resource(TEXT("Target_UIID"), m_pTexture2D);
	if (nullptr == m_pTexture2D)
		return -1;

	D3D11_MAPPED_SUBRESOURCE		SubResources{};

	m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &SubResources);

	/* 내 마우스 좌표가 존재하는 위치에 있는 텍스쳐 픽셀의 인덱스 */
	_uint		iIndex = ptMouse.y * m_iWinSizeX + ptMouse.x;

	if (iIndex > 1280 * 720)
		return -1;

	_float4		vResult = ((_float4*)SubResources.pData)[iIndex];

	//int* pData = static_cast<int*>(SubResources.pData);
	
	_int iResult = (_int)vResult.x;

	m_pContext->Unmap(m_pTexture2D, 0);

	return iResult;
}

_vector CCalculator::Picking_UI(_fmatrix ProjM)
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);


	//뷰포트-투영-뷰스페이스-월드
	_vector	MousePos = {};

	//투영으로 내림
	MousePos = XMVectorSetX(MousePos, ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f);
	MousePos = XMVectorSetY(MousePos, ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f);

	//뷰는 항등이기 때문
	_matrix  VPInverse = XMMatrixInverse(nullptr, ProjM);

	//월드로 내려옴
	MousePos = XMVector3TransformCoord(MousePos, VPInverse);

	//직교 투영으로 월드 까지 내린 마우스 포인터를 넘겨준다.
	return MousePos;
}

void CCalculator::World_MouseRay(_vector* RayArray)
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);


	//뷰포트-투영-뷰스페이스-월드
	_vector	MousePos = {};

	//투영으로 내림
	MousePos = XMVectorSetX(MousePos, ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f);
	MousePos = XMVectorSetY(MousePos, ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f);

	//뷰스페이스로 내려줌
	MousePos = XMVector3TransformCoord(MousePos, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::TS_PROJ));

	_vector Raypos, Raydir;
	Raypos = { 0.f,0.f,0.f };
	Raydir = MousePos - Raypos;

	//월드로 내려줌
	Raypos = XMVector3TransformCoord(Raypos, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::TS_VIEW));
	Raydir = XMVector3TransformNormal(Raydir, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::TS_VIEW));

	RayArray[0] = Raypos;
	RayArray[1] = XMVector3Normalize(Raydir);
}

HRESULT CCalculator::Tick()
{
	m_isSuccess = true;

	POINT		ptMouse = { };

	GetCursorPos(&ptMouse);

	ScreenToClient(m_hWnd, &ptMouse);

	if (ptMouse.x < 0)
		ptMouse.x = 0;
	if (ptMouse.x >= m_iWinSizeX)
		ptMouse.x = m_iWinSizeX;
	if (ptMouse.y < 0)
		ptMouse.y = 0;
	if (ptMouse.y >= m_iWinSizeY)
		ptMouse.y = m_iWinSizeY;

	_vector		vMousePos = XMVectorZero();

	/* 투영공간상의 좌표다. = 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 / w */
	vMousePos = XMVectorSetX(vMousePos, ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f);
	vMousePos = XMVectorSetY(vMousePos, ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f);
	vMousePos = XMVectorSetZ(vMousePos, Compute_ProjZ(ptMouse.x - 1, ptMouse.y - 1));
	vMousePos = XMVectorSetW(vMousePos, 1.f);

	if (XMVectorGetZ(vMousePos) < 0.0f)
		m_isSuccess = false;

	vMousePos = XMVector3TransformCoord(vMousePos, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::TS_PROJ));
	vMousePos = XMVector3TransformCoord(vMousePos, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::TS_VIEW));

	XMStoreFloat4(&m_vPickPos, vMousePos);

	return S_OK;
}

_bool CCalculator::Get_PickPos(_float4* pPickPos)
{
	*pPickPos = m_vPickPos;

	return m_isSuccess;
}

_bool CCalculator::Compare_Float4(_float4 f1, _float4 f2)
{
	_vector v1 = XMLoadFloat4(&f1);
	_vector v2 = XMLoadFloat4(&f2);
	if (v1.m128_f32[0] != v2.m128_f32[0])
		return false;
	if (v1.m128_f32[1] != v2.m128_f32[1])
		return false;
	if (v1.m128_f32[2] != v2.m128_f32[2])
		return false;
	if (v1.m128_f32[3] != v2.m128_f32[3])
		return false;
	return true;
}

_float CCalculator::Compute_ProjZ(_float fX, _float fY)
{
	if (fX < 0.f)
		fX = 0.f;
	if (fY < 0.f)
		fY = 0.f;

	m_pGameInstance->Copy_Resource(TEXT("Target_Depth"), m_pTexture2D);

	if (nullptr == m_pTexture2D)
		return 0.f;

	D3D11_MAPPED_SUBRESOURCE		SubResources{};

	m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &SubResources);

	/* 내 마우스 좌표가 존재하는 위치에 있는 텍스쳐 픽셀의 인덱스 */
	_uint		iIndex = fY * (m_iWinSizeX)+fX;

	_float4		vResult = ((_float4*)SubResources.pData)[iIndex];

	m_pContext->Unmap(m_pTexture2D, 0);


	if (0.0f == vResult.w)
		return -1.f;

	return vResult.x;
}

CCalculator* CCalculator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	CCalculator* pInstance = new CCalculator(pDevice,pContext, iWinSizeX, iWinSizeY);

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX("Failed To Created : CCalculator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCalculator::Free()
{
	Safe_Release(m_pTexture2D);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
