#pragma once

#include "Client_Defines.h"
#include "State_MotorCycle.h"

BEGIN(Client)

class CState_MotorCycle_Jump_Loop : public CState_MotorCycle
{
public:
	CState_MotorCycle_Jump_Loop(CState_Manager* pManager, STATE_TYPE eState);
	CState_MotorCycle_Jump_Loop(const CState_MotorCycle_Jump_Loop& rhs);
	virtual ~CState_MotorCycle_Jump_Loop() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_MotorCycle_Jump_Loop* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END