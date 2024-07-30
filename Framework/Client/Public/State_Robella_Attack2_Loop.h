#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Robella_Attack2_Loop : public CState_Boss
{
public:
	CState_Robella_Attack2_Loop(CState_Manager* pManager, STATE_TYPE eState);
	CState_Robella_Attack2_Loop(const CState_Robella_Attack2_Loop& rhs);
	virtual ~CState_Robella_Attack2_Loop() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	DECL_ONCE(m_bEffect0);

	_bool	m_bCreate = false;

	_uint	m_iPlayCnt = 0;

public:
	static CState_Robella_Attack2_Loop* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

