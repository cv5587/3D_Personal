#pragma once

#include "Engine_Defines.h"

/* 모든 클래스들의 부모가 되는 클래스다. == SuperClass */
/* 레퍼런스 카운트를 관리한다. AddRef, Release or Delete */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	_uint AddRef();
	_uint Release();

private:
	_uint		m_iRefCnt = { 0 };

public:
	virtual void Free() = 0;
};

END