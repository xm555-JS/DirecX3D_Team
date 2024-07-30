#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Damaged_Low : public CState_Player
{
public:
	CState_Player_Damaged_Low(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Damaged_Low(const CState_Player_Damaged_Low& rhs);
	virtual ~CState_Player_Damaged_Low() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:

private:
	_float	m_fDamage = 0.f;
	_float	m_fstandard = 0.f;

public:
	static CState_Player_Damaged_Low* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END