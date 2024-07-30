#pragma once

#include "Client_Defines.h"
#include "UI_Order.h"

BEGIN(Client)

class CUI_Order_TenResult final : public CUI_Order
{
public:
	CUI_Order_TenResult();
	CUI_Order_TenResult(const CUI_Order_TenResult& rhs);
	virtual ~CUI_Order_TenResult() = default;

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
	_float m_fWaitTime = 0.f;
	_float m_fAccTime = 0.f;
	_bool  m_bRender = false;
	_bool  m_bOverlap = false;

	DECL_ONCE(m_bCreateButton);
	DECL_ONCE(m_bCreateConverse);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Order_TenResult* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END