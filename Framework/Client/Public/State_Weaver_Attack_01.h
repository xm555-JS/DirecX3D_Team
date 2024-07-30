#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Weaver_Attack_01 : public CState_Boss
{
public:
	CState_Weaver_Attack_01(CState_Manager* pManager, STATE_TYPE eState);
	CState_Weaver_Attack_01(const CState_Weaver_Attack_01& rhs);
	virtual ~CState_Weaver_Attack_01() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	CCollider* m_pColCom[6] = { nullptr, };
	_bool		m_bHit[6] = { false, };

	_bool		m_bHit0 = false;
	_bool		m_bHit1 = false;
	
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);

public:
	static CState_Weaver_Attack_01* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

