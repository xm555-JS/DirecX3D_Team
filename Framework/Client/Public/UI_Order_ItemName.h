#pragma once

#include "Client_Defines.h"
#include "UI_Order.h"

BEGIN(Client)

class CUI_Order_ItemName final : public CUI_Order
{
public:
	CUI_Order_ItemName();
	CUI_Order_ItemName(const CUI_Order_ItemName& rhs);
	virtual ~CUI_Order_ItemName() = default;

public:
	_float Get_AccTime() { return m_fAccTime; }

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
	_float m_fAccTime = 0.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Order_ItemName* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END