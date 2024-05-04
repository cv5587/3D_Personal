#pragma once
#include "Client_Defines.h"
#include "Base.h"
BEGIN(Engine)
class CGameInstance;
class CItem;
END 

BEGIN(Client)
class CUImanager :
    public CBase
{
public:
    enum KEYID{ KEY_LEFTCLICK, KEY_RIGHTCLICK, KEY_END };
    enum BASEID{ BASE_BAG, BASE_WEIGHT, BASE_DURABILITY, BASE_END};
    enum TEXTID { TEXT_NAME, TEXT_INFO, TEXT_WEIGHT, TEXT_DURABILITY,
        TEXT_LCLICK, TEXT_RCLICK, TEXT_BAGWEIGHT, TEXT_TOTALWEIGHT, TEXT_END};
    //	enum ITEMTYPE {   ITEM_STUFF,  ITEM_MEDKITS,   ITEM_KINDLING, ITEM_CLOTH,    ITEM_FOOD, ITEM_EQUIPMENT,      ITEM_END};
    enum LayerIndex { Layer_KEY, Layer_Base, Layer_Type, Layer_Text, Layer_END };
    enum TypeID {TYPE_ONE , TYPE_TWO , TYPE_END};
private:
    CUImanager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CUImanager() = default;
public:
     HRESULT Initialize() ;
     HRESULT Item_SetInfo(class CGameObject* pPickObject, _float fTotalWeight);
     void Render_UI(_uint LayerIndex);
     void Render_TypeUI(_uint LayerIndex);



private:
    class CGameInstance* m_pGameInstance = { nullptr };
    //ui를 정보 표시에 대한 것들(아이템타입), 기본 구성품(가방,무게,내구도)
    //마우좌우 클릭나누자
    vector<wstring> m_UISelector;
    vector<class CUIObject*> m_TypeUI;
private:
    //가로세로 절반 크기 저장
    _float m_fX, m_fY;
public:
    static CUImanager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;
};

END