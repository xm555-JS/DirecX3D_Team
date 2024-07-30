#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Swim_Loop_A : public CState_Player
{
public:
	CState_Player_Swim_Loop_A(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Swim_Loop_A(const CState_Player_Swim_Loop_A& rhs);
	virtual ~CState_Player_Swim_Loop_A() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Enter(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void Exit(void* pArg = nullptr);

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	_float  m_fTimeAcc_Effect0 = 0.f;
	_float  m_fTimeAcc_Effect1 = 0.f;
	_float  m_fTimeAcc_Effect2 = 0.f;

public:
	static CState_Player_Swim_Loop_A* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END