#pragma once

#include "Client_Defines.h"
#include "State_Enemy_Normal.h"

BEGIN(Client)

class CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting : public CState_Enemy_Normal
{
public:
	CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting(CState_Manager* pManager, STATE_TYPE eState);
	CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting(const CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting& rhs);
	virtual ~CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);
	DECL_ONCE(m_bEffect2);
	DECL_ONCE(m_bEffect3);
	DECL_ONCE(m_bEffect4);
	DECL_ONCE(m_bEffect5);
	DECL_ONCE(m_bEffect6);
	DECL_ONCE(m_bEffect7);

public:
	static CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END