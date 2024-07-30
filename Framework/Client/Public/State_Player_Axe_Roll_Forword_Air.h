#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Axe_Roll_Forword_Air : public CState_Player
{
public:
	CState_Player_Axe_Roll_Forword_Air(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Axe_Roll_Forword_Air(const CState_Player_Axe_Roll_Forword_Air& rhs);
	virtual ~CState_Player_Axe_Roll_Forword_Air() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Enter(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void Exit(void* pArg = nullptr);

private:

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

	 
public:
	static CState_Player_Axe_Roll_Forword_Air* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END