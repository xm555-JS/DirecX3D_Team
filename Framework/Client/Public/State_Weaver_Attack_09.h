#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Weaver_Attack_09 : public CState_Boss
{
public:
	CState_Weaver_Attack_09(CState_Manager* pManager, STATE_TYPE eState);
	CState_Weaver_Attack_09(const CState_Weaver_Attack_09& rhs);
	virtual ~CState_Weaver_Attack_09() = default;

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
	CGameObject*	m_pShockWave = nullptr;
	_bool			m_bCreateObj = false;

	_float			m_fTimeAcc_Effect0 = 0.f;
	_float			m_fTimeAcc_Effect1 = 0.f;
	_float			m_fTimeAcc_Effect2 = 0.f;
	_float			m_fTimeAcc_Effect3 = 0.f;
	_uint			m_iEffectNum0 = 0.f;
	_bool			m_bHit0 = false;
	_bool			m_bHit1 = false;

	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);
	DECL_ONCE(m_bEffect2);
	DECL_ONCE(m_bEffect3);
	DECL_ONCE(m_bEffect4);
	DECL_ONCE(m_bEffect5);


public:
	static CState_Weaver_Attack_09* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

