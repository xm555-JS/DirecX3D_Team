#pragma once

#include "Client_Defines.h"
#include "State_Enemy_Normal.h"

BEGIN(Client)

class CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing : public CState_Enemy_Normal
{
public:
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing(CState_Manager* pManager, STATE_TYPE eState);
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing(const CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing& rhs);
	virtual ~CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing() = default;

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
	DECL_ONCE(m_bEffect1);

private:
	_float m_fTime = 0.f;
	_uint m_iCount = 0.f;

public:
	static CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END