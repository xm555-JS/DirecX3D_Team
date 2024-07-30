#pragma once

#include "Client_Defines.h"
#include "State_Npc.h"

BEGIN(Client)

class CState_Shirley_Conversation_A : public CState_Npc
{
public:
	CState_Shirley_Conversation_A(CState_Manager* pManager, STATE_TYPE eState);
	CState_Shirley_Conversation_A(const CState_Shirley_Conversation_A& rhs);
	virtual ~CState_Shirley_Conversation_A() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Shirley_Conversation_A* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END