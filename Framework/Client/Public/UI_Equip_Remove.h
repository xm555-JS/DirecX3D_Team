#pragma once

#include "Client_Defines.h"
#include "UI_Equip_Window.h"

BEGIN(Client)

class CUI_Equip_Remove final : public CUI_Equip_Window
{
public:
	CUI_Equip_Remove();
	CUI_Equip_Remove(const CUI_Equip_Remove& rhs);
	virtual ~CUI_Equip_Remove() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;


private:
	void Interact_Button(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Equip_Remove* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END