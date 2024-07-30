#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Sword_Attack_Base_Charge : public CState_Player
{
public:
	CState_Player_Sword_Attack_Base_Charge(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Sword_Attack_Base_Charge(const CState_Player_Sword_Attack_Base_Charge& rhs);
	virtual ~CState_Player_Sword_Attack_Base_Charge() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_bool m_bJump = false;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Player_Sword_Attack_Base_Charge* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END