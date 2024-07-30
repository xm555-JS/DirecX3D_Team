#pragma once

#include "Client_Defines.h"
#include "UI_Order.h"

BEGIN(Client)

class CUI_Order_Anim final : public CUI_Order
{
public:
	CUI_Order_Anim();
	CUI_Order_Anim(const CUI_Order_Anim& rhs);
	virtual ~CUI_Order_Anim() = default;

public:
	void Set_Ten(_bool bTen) { m_bTen = bTen; }

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
	_bool	m_bTen = false;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Order_Anim* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END