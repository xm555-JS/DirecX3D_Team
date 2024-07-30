#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Mount_TurnL : public CState_Player
{
public:
	CState_Player_Mount_TurnL(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Mount_TurnL(const CState_Player_Mount_TurnL& rhs);
	virtual ~CState_Player_Mount_TurnL() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Player_Mount_TurnL* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END