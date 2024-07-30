#pragma once

#include "Client_Defines.h"
#include "State_Player_Skate.h"

BEGIN(Client)

class CState_Player_Skate_Stop_Water : public CState_Player_Skate
{
public:
	CState_Player_Skate_Stop_Water(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Skate_Stop_Water(const CState_Player_Skate_Stop_Water& rhs);
	virtual ~CState_Player_Skate_Stop_Water() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_float4 m_vCurPos;
	_float4 m_vTargetPos;
	_float m_fDownTime = 0.f;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Player_Skate_Stop_Water* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END