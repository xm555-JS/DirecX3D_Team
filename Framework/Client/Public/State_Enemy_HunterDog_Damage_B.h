#pragma once

#include "Client_Defines.h"
#include "State_Enemy_Normal.h"

BEGIN(Client)

class CState_Enemy_HunterDog_Damage_B : public CState_Enemy_Normal
{
public:
	CState_Enemy_HunterDog_Damage_B(CState_Manager* pManager, STATE_TYPE eState);
	CState_Enemy_HunterDog_Damage_B(const CState_Enemy_HunterDog_Damage_B& rhs);
	virtual ~CState_Enemy_HunterDog_Damage_B() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Enemy_HunterDog_Damage_B* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END