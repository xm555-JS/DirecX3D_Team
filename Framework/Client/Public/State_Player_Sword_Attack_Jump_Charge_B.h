#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Sword_Attack_Jump_Charge_B : public CState_Player
{
public:
	CState_Player_Sword_Attack_Jump_Charge_B(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Sword_Attack_Jump_Charge_B(const CState_Player_Sword_Attack_Jump_Charge_B& rhs);
	virtual ~CState_Player_Sword_Attack_Jump_Charge_B() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_float m_fTimeAcc = 0.f;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Player_Sword_Attack_Jump_Charge_B* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END