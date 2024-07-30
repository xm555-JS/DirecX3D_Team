#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Climb_Right_Down : public CState_Player
{
public:
	CState_Player_Climb_Right_Down(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Climb_Right_Down(const CState_Player_Climb_Right_Down& rhs);
	virtual ~CState_Player_Climb_Right_Down() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Player_Climb_Right_Down* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END