#pragma once

#include "Base.h"

/* 실제 객체들을 그리는 순서를 관리해야한다. */
/* 대부분의 객체들은 깊이 테스트를 통해서 알아서 차폐관리가 되고 있다. */
/* 깊이버퍼의 사용을 통해 알아서 깊이관리가된다 굉장히 큰 장점이긴한데. */
/* 이로 인해서 생기는 부작용이 존재하낟. */
/* 부작용 1. 스카이박스. : 스카이박스는 실제로 작게 그리낟.  */
/* 하늘 : 무조건 다른 객체들에게 덮여야한다(가장먼저그린다) + 하늘의 깊이는 다른객체들과 비교연산이 일어나서는 안된다.(하늘의 깊이를 깊이버퍼에 기록하지 않는다.) */
/* 부작용 2. 알파블렌드 :   */
/* 블렌드 : 내가 그릴려고하는 색과 이전에 그려져있던 색을 섞는다. */
/* 부작용 3. UI.  */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND, RENDER_NONLIGHT, RENDER_BLEND, RENDER_UI, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	void Draw();

private:
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };
	list<class CGameObject*>			m_RenderGroup[RENDER_END];

private:
	void  Render_Priority();
	void  Render_NonBlend();
	void  Render_NonLight();
	void  Render_Blend();
	void  Render_UI();


public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END