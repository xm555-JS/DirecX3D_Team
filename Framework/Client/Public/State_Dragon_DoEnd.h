#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Dragon_DoEnd : public CState_Boss
{
public:
	CState_Dragon_DoEnd(CState_Manager* pManager, STATE_TYPE eState);
	CState_Dragon_DoEnd(const CState_Dragon_DoEnd& rhs);
	virtual ~CState_Dragon_DoEnd() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	_float4 m_vCenter;
	_float4 m_vPos;
	_float4 m_vTargetPos;
	_float4 m_vFinalPos;
	_float m_fEasingTime = 0.f;
	_float4	m_vStartLook;
	STATE_TYPE m_ePreState;

public:
	static CState_Dragon_DoEnd* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

