#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Dragon_Fall_Land : public CState_Boss
{
public:
	CState_Dragon_Fall_Land(CState_Manager* pManager, STATE_TYPE eState);
	CState_Dragon_Fall_Land(const CState_Dragon_Fall_Land& rhs);
	virtual ~CState_Dragon_Fall_Land() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Dragon_Fall_Land* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

