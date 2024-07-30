#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Weaver_Attack_07 : public CState_Boss
{
public:
	CState_Weaver_Attack_07(CState_Manager* pManager, STATE_TYPE eState);
	CState_Weaver_Attack_07(const CState_Weaver_Attack_07& rhs);
	virtual ~CState_Weaver_Attack_07() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	void Excute_Particles(_uint _iEffectNum0, char* _szBoneName);

private:
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);
	_float			m_fTimeAcc_Effect0 = 0.f;
	_float			m_fTimeAcc_Effect1 = 0.f;
	_float			m_fTimeAcc_Effect2 = 0.f;
	_uint			m_iEffectNum0 = 0.f;
	_uint			m_iEffectNum1 = 0.f;
	_bool			m_bToggle_Effect = false;

	_bool			m_bHit[23] = { false, };

	_float4			m_vStartLook;

public:
	static CState_Weaver_Attack_07* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

