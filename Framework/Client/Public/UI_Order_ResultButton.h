#pragma once

#include "Client_Defines.h"
#include "UI_Order.h"

BEGIN(Client)

class CUI_Order_ResultButton final : public CUI_Order
{
public:
	enum ORDER_RESULTBUTTON
	{
		RESULTB_BACK = 0,
		RESULTB_ONE = 1,
		RESULTB_TEN = 2,
		RESULTB_SKIP = 3,

		RESULTB_END
	};

public:
	CUI_Order_ResultButton();
	CUI_Order_ResultButton(const CUI_Order_ResultButton& rhs);
	virtual ~CUI_Order_ResultButton() = default;

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
	static CUI_Order_ResultButton* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END