#pragma once

#include "Client_Defines.h"
#include "State_AI.h"

BEGIN(Client)

class CState_Boss_KeepAttack : public CState_AI
{
public:
	CState_Boss_KeepAttack(CState_Manager* pManager, STATE_TYPE eState);
	CState_Boss_KeepAttack(const CState_Boss_KeepAttack& rhs);
	virtual ~CState_Boss_KeepAttack() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Boss_KeepAttack* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END