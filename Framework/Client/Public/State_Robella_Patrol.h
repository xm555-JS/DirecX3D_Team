#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Robella_Patrol : public CState_Boss
{
public:
	CState_Robella_Patrol(CState_Manager* pManager, STATE_TYPE eState);
	CState_Robella_Patrol(const CState_Robella_Patrol& rhs);
	virtual ~CState_Robella_Patrol() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	_float m_fAwake_Distance = 0.f;

public:
	static CState_Robella_Patrol* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

