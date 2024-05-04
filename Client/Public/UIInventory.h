#pragma once
#include "Client_Defines.h"
#include "Base.h"
BEGIN(Engine)
class CGameInstance;
class CItem;
END

BEGIN(Client)
class CUIInventory :
    public CBase
{
private:
    CUIInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CUIInventory() = default;
public:
    HRESULT Initialize();
    HRESULT Item_SetInfo(class CGameObject* pPickObject, _float fTotalWeight);
    void Render_UI(_uint LayerIndex);
    void Render_TypeUI(_uint LayerIndex);



private:
    class CGameInstance* m_pGameInstance = { nullptr };
    //ui�� ���� ǥ�ÿ� ���� �͵�(������Ÿ��), �⺻ ����ǰ(����,����,������)
    //�����¿� Ŭ��������
    vector<wstring> m_UIInventory;
    vector<class CUIObject*> m_TypeUI;
private:
    //���μ��� ���� ũ�� ����
    _float m_fX, m_fY;
public:
    static CUIInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;
};
END
