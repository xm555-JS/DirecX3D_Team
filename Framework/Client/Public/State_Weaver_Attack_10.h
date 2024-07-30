#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Weaver_Attack_10 : public CState_Boss
{
public:
	CState_Weaver_Attack_10(CState_Manager* pManager, STATE_TYPE eState);
	CState_Weaver_Attack_10(const CState_Weaver_Attack_10& rhs);
	virtual ~CState_Weaver_Attack_10() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	void Excute_Particles(_uint _iEffectNum0, char* _szBoneName);

private:
	_float			m_fTimeAcc_Effect0 = 0.f;
	_uint			m_iEffectNum0 = 0.f;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Weaver_Attack_10* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

