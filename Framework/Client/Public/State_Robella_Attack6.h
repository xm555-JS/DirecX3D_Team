#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Robella_Attack6 : public CState_Boss
{
public:
	CState_Robella_Attack6(CState_Manager* pManager, STATE_TYPE eState);
	CState_Robella_Attack6(const CState_Robella_Attack6& rhs);
	virtual ~CState_Robella_Attack6() = default;

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

public:
	static CState_Robella_Attack6* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

