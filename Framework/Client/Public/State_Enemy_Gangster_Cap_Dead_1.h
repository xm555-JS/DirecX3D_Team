#pragma once

#include "Client_Defines.h"
#include "State_Enemy_Normal.h"

BEGIN(Client)

class CState_Enemy_Gangster_Cap_Dead_1 : public CState_Enemy_Normal
{
public:
	CState_Enemy_Gangster_Cap_Dead_1(CState_Manager* pManager, STATE_TYPE eState);
	CState_Enemy_Gangster_Cap_Dead_1(const CState_Enemy_Gangster_Cap_Dead_1& rhs);
	virtual ~CState_Enemy_Gangster_Cap_Dead_1() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Enemy_Gangster_Cap_Dead_1* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END