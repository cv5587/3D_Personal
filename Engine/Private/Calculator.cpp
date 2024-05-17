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

	 D3D11_TEXTURE2D_DESC	textureDesc;
	 ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	 m_hWnd = hWnd;

	 textureDesc.Width = m_iWinSizeX;
	 textureDesc.Height = m_iWinSizeY;
	 textureDesc.MipLevels = 1;
	 textureDesc.ArraySize = 1;
	 //DXGI_FORMAT_R32_FLOAT : GPU에서 CPU로의 데이터 전송(복사)을 지원하는 리소스입니다.
	 textureDesc.Format = DXGI_FORMAT_R32_FLOAT;


	 textureDesc.SampleDesc.Quality = 0;
	 textureDesc.SampleDesc.Count = 1;

	 textureDesc.Usage = D3D11_USAGE_STAGING;
	 textureDesc.BindFlags = 0;
	 textureDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
	 textureDesc.MiscFlags = 0;


	 if (FAILED(m_pDevice->CreateTexture2D(&textureDesc, nullptr, &m_pHitScreenTexture)))
		 return E_FAIL;

	 textureDesc;
	 ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	 m_hWnd = hWnd;

	 textureDesc.Width = m_iWinSizeX;
	 textureDesc.Height = m_iWinSizeY;
	 textureDesc.MipLevels = 1;
	 textureDesc.ArraySize = 1;
	 //DXGI_FORMAT_R32_FLOAT : GPU에서 CPU로의 데이터 전송(복사)을 지원하는 리소스입니다.
	 textureDesc.Format = DXGI_FORMAT_R32_SINT;


	 textureDesc.SampleDesc.Quality = 0;
	 textureDesc.SampleDesc.Count = 1;

	 textureDesc.Usage = D3D11_USAGE_STAGING;
	 textureDesc.BindFlags = 0;
	 textureDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
	 textureDesc.MiscFlags = 0;


	 if (FAILED(m_pDevice->CreateTexture2D(&textureDesc, nullptr, &m_pIDScreenTexture)))
		 return E_FAIL;


	 textureDesc;
	 ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	 m_hWnd = hWnd;

	 textureDesc.Width = m_iWinSizeX;
	 textureDesc.Height = m_iWinSizeY;
	 textureDesc.MipLevels = 1;
	 textureDesc.ArraySize = 1;
	 //DXGI_FORMAT_R32_FLOAT : GPU에서 CPU로의 데이터 전송(복사)을 지원하는 리소스입니다.
	 textureDesc.Format = DXGI_FORMAT_R32_SINT;


	 textureDesc.SampleDesc.Quality = 0;
	 textureDesc.SampleDesc.Count = 1;

	 textureDesc.Usage = D3D11_USAGE_STAGING;
	 textureDesc.BindFlags = 0;
	 textureDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
	 textureDesc.MiscFlags = 0;


	 if (FAILED(m_pDevice->CreateTexture2D(&textureDesc, nullptr, &m_pUIIDScreenTexture)))
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

	float vDepth = m_pGameInstance->Compute_ProjZ(ptMouse, m_pHitScreenTexture);

	if (vDepth == -1.f)
		return XMVectorSet(-1.f, -1.f, -1.f, -1.f);

	MousePos = XMVectorSetZ(MousePos, vDepth);
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


	//뷰포트-투영-뷰스페이스-월드
	_vector	MousePos = {};

	//투영으로 내림
	MousePos = XMVectorSetX(MousePos, ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f);
	MousePos = XMVectorSetY(MousePos, ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f);

	_int iID = m_pGameInstance->Compute_ID(ptMouse, m_pIDScreenTexture);	

	return iID;
}

_int CCalculator::Picking_UIIDScreen()
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);


	//뷰포트-투영-뷰스페이스-월드
	_vector	MousePos = {};

	//투영으로 내림
	MousePos = XMVectorSetX(MousePos, ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f);
	MousePos = XMVectorSetY(MousePos, ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f);

	_int iID = m_pGameInstance->Compute_UIID(ptMouse, m_pUIIDScreenTexture);

	return iID;
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
	Safe_Release(m_pHitScreenTexture);
	Safe_Release(m_pIDScreenTexture);
	Safe_Release(m_pUIIDScreenTexture);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
