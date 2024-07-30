#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)
class CEffect_Base;
class CState_Player_Skate abstract : public CState_Player
{
public:
	CState_Player_Skate(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Skate(const CState_Player_Skate& rhs);
	virtual ~CState_Player_Skate() = default;

public:
	_float Get_RunSpeed() { return m_fRunSpeed; }
	list<CEffect_Base*>* Get_Effects() { return &m_listEffects; }

public:
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;


protected:
	_float	m_fForce = 0.f;
	_float	m_fRunSpeed = 0.f;
	list<CEffect_Base*>	m_listEffects;

public:
	virtual void Free() override;
};

END