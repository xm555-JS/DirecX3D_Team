#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Nemesis_Patrol_Stand : public CState_Boss
{
public:
	CState_Nemesis_Patrol_Stand(CState_Manager* pManager, STATE_TYPE eState);
	CState_Nemesis_Patrol_Stand(const CState_Nemesis_Patrol_Stand& rhs);
	virtual ~CState_Nemesis_Patrol_Stand() = default;

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
	static CState_Nemesis_Patrol_Stand* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

