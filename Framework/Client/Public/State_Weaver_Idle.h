#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Weaver_Idle : public CState_Boss
{
public:
	CState_Weaver_Idle(CState_Manager* pManager, STATE_TYPE eState);
	CState_Weaver_Idle(const CState_Weaver_Idle& rhs);
	virtual ~CState_Weaver_Idle() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	_float m_fEasingTime = 0.f;
	_float4	m_vStartLook;
	STATE_TYPE m_ePreState;
	_uint	m_iPhase = 0;

public:
	static CState_Weaver_Idle* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

