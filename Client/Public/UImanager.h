#pragma once
#include "Client_Defines.h"
#include "Base.h"
BEGIN(Engine)
class CGameInstance;
END 

BEGIN(Client)
class CUImanager :
    public CBase
{
public:
    enum KEYID{ KEY_LEFTCLICK, KEY_RIGHTCLICK, KEY_END };
    enum BASEID{ BASE_BAG, BASE_WEIGHT, BASE_DURABILITY, ID_END};
    enum TYPEID{ TYPE_STUFF, TYPE_MEDIKIT, TYPE_KINKLING, TYPE_END};
    enum LayerIndex { Layer_KEY, Layer_END };
private:
    CUImanager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CUImanager() = default;
public:
     HRESULT Initialize() ;
     void Render_KeyUI(_uint LayerIndex);

private:
    class CGameInstance* m_pGameInstance = { nullptr };
    //ui를 정보 표시에 대한 것들(아이템타입), 기본 구성품(가방,무게,내구도)
    //마우좌우 클릭나누자
    vector<wstring> m_UISelector;
private:
    //화면 중앙 기점(기준)
    _float m_fX, m_fY;
public:
    static CUImanager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;
};

END