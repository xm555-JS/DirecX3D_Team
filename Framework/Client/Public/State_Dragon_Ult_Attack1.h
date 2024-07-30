#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Dragon_Ult_Attack1 : public CState_Boss
{
public:
	CState_Dragon_Ult_Attack1(CState_Manager* pManager, STATE_TYPE eState);
	CState_Dragon_Ult_Attack1(const CState_Dragon_Ult_Attack1& rhs);
	virtual ~CState_Dragon_Ult_Attack1() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	_float m_fGoalY;
	_float m_fEasingTime = 0.f;
	_float m_vDistance = 0.f;
	_bool  m_bAnimSkip = false;

	_float4	m_vStartLook;
	_float4 m_vTargetPos;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Dragon_Ult_Attack1* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

