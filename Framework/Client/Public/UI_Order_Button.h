#pragma once

#include "Client_Defines.h"
#include "UI_Order.h"

BEGIN(Client)

class CUI_Order_Button final : public CUI_Order
{
public:
	enum ORDER_BUTTON
	{
		ORDERB_EXIT = 0,
		ORDERB_TEN = 4,
		ORDERB_ONE = 5,

		ORDERB_END
	};

public:
	CUI_Order_Button();
	CUI_Order_Button(const CUI_Order_Button& rhs);
	virtual ~CUI_Order_Button() = default;

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
	void Pooling_Order_Objects();
	void Interact_Button(_float fTimeDelta);
	void Order_Item(_uint iOrderNum);
	void Create_UI_Alert();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Order_Button* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END