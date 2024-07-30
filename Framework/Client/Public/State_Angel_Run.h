#pragma once

#include "Client_Defines.h"
#include "State_AI.h"

BEGIN(Client)

class CState_Angel_Run : public CState_AI
{
public:
	CState_Angel_Run(CState_Manager* pManager, STATE_TYPE eState);
	CState_Angel_Run(const CState_Angel_Run& rhs);
	virtual ~CState_Angel_Run() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Angel_Run* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END



