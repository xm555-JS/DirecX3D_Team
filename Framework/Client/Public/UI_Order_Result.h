#pragma once

#include "Client_Defines.h"
#include "UI_Order.h"

BEGIN(Client)

class CUI_Order_Result final : public CUI_Order
{
public:
	CUI_Order_Result();
	CUI_Order_Result(const CUI_Order_Result& rhs);
	virtual ~CUI_Order_Result() = default;

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
	DECL_ONCE(m_bCreate);

	_float m_fAccTime = 0.f;
	_float m_fTimeDir = 1.f;

private:
	void FadeOut_Black_Background(_float fTimeDelta);
	void FadeInOut_Yellow_Light(_float fTimeDelta);
	void Rotation_ItemBack_Circle(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Order_Result* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END