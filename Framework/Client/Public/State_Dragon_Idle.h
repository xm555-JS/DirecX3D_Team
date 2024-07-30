#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Dragon_Idle : public CState_Boss
{
public:
	CState_Dragon_Idle(CState_Manager* pManager, STATE_TYPE eState);
	CState_Dragon_Idle(const CState_Dragon_Idle& rhs);
	virtual ~CState_Dragon_Idle() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

	void Move_Idle(_float fTimeDelta);

private:
	STATE_TYPE	m_ePreState = STAET_DRAGON_END;

	_float4		m_vCurPos;

	_float		m_fDownTime = 0.f;

	_uint		m_iKeyFrame = 0;
	_uint		m_iTime = 0;
	_float		m_fChaseDistance = 0.f;

	_float		m_fWingTime = 0.f;

public:
	static CState_Dragon_Idle* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

