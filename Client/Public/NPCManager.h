#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "UImanager.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CInventory;
END

BEGIN(Client)
class CNPCManager final :
    public CBase
{
public:
    typedef struct {
        CUImanager* pUImanager;
        CInventory* pPlayerInventory;
    }NPC_DESC;
private:
    CNPCManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CNPCManager() = default;
public:
    HRESULT Initialize(void* pArg);
    HRESULT Tick(_float fTimeDelta);
    HRESULT Late_Tick(_float fTimeDelta);

    _bool Talk(_int TalkIndex);

    wstring Get_CurrentQuestName();
private:
    class CGameInstance* m_pGameInstance = { nullptr };
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    //레이어 이름을 가지고 있음
    class CUImanager* m_pUImanager = { nullptr };
    class CTalkManager* m_pTalkManager = { nullptr };
    class CQuestManager* m_pQuestManager = { nullptr };
    class CInventory* m_pPlayerInventory = { nullptr };
    class CData_Manager* m_pDatamanager = { nullptr };
private:
    _int m_iCurrenQuestID = { 0 };//현제 진행중 퀘스트 아이디.
    
    class CUITEXT* m_QuestpText = { nullptr };
    //wstring* m_wTalkData = { nullptr };
public:
    static CNPCManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual void Free() override;
};

END