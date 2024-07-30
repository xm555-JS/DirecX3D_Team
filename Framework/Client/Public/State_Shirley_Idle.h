#pragma once

#include "Client_Defines.h"
#include "State_Npc.h"

BEGIN(Client)

class CState_Shirley_Idle : public CState_Npc
{
public:
	CState_Shirley_Idle(CState_Manager* pManager, STATE_TYPE eState);
	CState_Shirley_Idle(const CState_Shirley_Idle& rhs);
	virtual ~CState_Shirley_Idle() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	_uint m_iPreScriptPhase = 0;

public:
	static CState_Shirley_Idle* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END