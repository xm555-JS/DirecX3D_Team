#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Run : public CState_Player
{
public:
	CState_Player_Run(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Run(const CState_Player_Run& rhs);
	virtual ~CState_Player_Run() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Enter(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void Exit(void* pArg = nullptr);

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	_float m_fTimeAcc_Effect = 0.f;
	_bool m_bToggle_Effect = false;

public:
	static CState_Player_Run* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END