#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class COrder final : public CGameObject
{
public:
	COrder();
	COrder(const COrder& rhs);
	virtual ~COrder() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

public:
	void Set_Skip();


private:
	_bool m_bTen = false;
	_uint m_iOrderIdx = 0;

	_float m_fClickTime = 0.f;
	_float m_fAccTime = 0.f;
	_uint  m_iUI_TenIdx = 0;
	_bool  m_bCreate_UI_TenOrders = false;

	CGameObject* m_pUI_SkipBtn = nullptr;


private:
	void Create_UI_TenOrders();
	void Pooling_Objects();

public:
	static COrder* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END