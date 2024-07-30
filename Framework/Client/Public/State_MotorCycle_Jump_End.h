#pragma once

#include "Client_Defines.h"
#include "State_MotorCycle.h"

BEGIN(Client)

class CState_MotorCycle_Jump_End : public CState_MotorCycle
{
public:
	CState_MotorCycle_Jump_End(CState_Manager* pManager, STATE_TYPE eState);
	CState_MotorCycle_Jump_End(const CState_MotorCycle_Jump_End& rhs);
	virtual ~CState_MotorCycle_Jump_End() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_MotorCycle_Jump_End* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END