#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Dragon_TurnL : public CState_Boss
{
public:
	CState_Dragon_TurnL(CState_Manager* pManager, STATE_TYPE eState);
	CState_Dragon_TurnL(const CState_Dragon_TurnL& rhs);
	virtual ~CState_Dragon_TurnL() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_float m_fEasingTime = 0.f;
	_float4	m_vStartLook;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Dragon_TurnL* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

