#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Client)

class CState_Player_Swim_Loop_B : public CState_Player
{
public:
	CState_Player_Swim_Loop_B(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Swim_Loop_B(const CState_Player_Swim_Loop_B& rhs);
	virtual ~CState_Player_Swim_Loop_B() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Enter(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void Exit(void* pArg = nullptr);

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;
	
private:
	DECL_ONCE(m_bEffect6);
	DECL_ONCE(m_bEffect7);
	DECL_ONCE(m_bEffect8);
	DECL_ONCE(m_bEffect9);

	_uint m_iCurKeyFrame = 0;
	_float m_fAccTime = 0.f;

private:
	_float  m_fTimeAcc_Effect0 = 0.f;
	_float  m_fTimeAcc_Effect1 = 0.f;
	_float  m_fTimeAcc_Effect2 = 0.f;

public:
	static CState_Player_Swim_Loop_B* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END