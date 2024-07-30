#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Dragon_Weak_Begin : public CState_Boss
{
public:
	CState_Dragon_Weak_Begin(CState_Manager* pManager, STATE_TYPE eState);
	CState_Dragon_Weak_Begin(const CState_Dragon_Weak_Begin& rhs);
	virtual ~CState_Dragon_Weak_Begin() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Dragon_Weak_Begin* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

