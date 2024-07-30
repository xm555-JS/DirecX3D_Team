#pragma once

#include "Client_Defines.h"
#include "UI_Order.h"

BEGIN(Client)

class CUI_Order_Item final : public CUI_Order
{
public:
	CUI_Order_Item();
	CUI_Order_Item(const CUI_Order_Item& rhs);
	virtual ~CUI_Order_Item() = default;

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
	_float m_fColorTime = 0.f;
	_float m_fColorRatio = 1.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Order_Item* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END