#pragma once

#include "Client_Defines.h"
#include "UI_Order.h"

BEGIN(Client)

class CUI_Order_Icon final : public CUI_Order
{
public:
	CUI_Order_Icon();
	CUI_Order_Icon(const CUI_Order_Icon& rhs);
	virtual ~CUI_Order_Icon() = default;

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
	_float m_fDir = 1.f;
	_float m_fMoveUVSpeed = 0.f;

private:
	void SetUp_BlackCoreNum();
	void Icon_Anim(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Order_Icon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END