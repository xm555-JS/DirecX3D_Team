#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Robella_Ready : public CState_Boss
{
public:
	CState_Robella_Ready(CState_Manager* pManager, STATE_TYPE eState);
	CState_Robella_Ready(const CState_Robella_Ready& rhs);
	virtual ~CState_Robella_Ready() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Robella_Ready* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

