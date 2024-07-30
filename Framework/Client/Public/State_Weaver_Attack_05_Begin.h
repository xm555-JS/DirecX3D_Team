#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Weaver_Attack_05_Begin : public CState_Boss
{
public:
	CState_Weaver_Attack_05_Begin(CState_Manager* pManager, STATE_TYPE eState);
	CState_Weaver_Attack_05_Begin(const CState_Weaver_Attack_05_Begin& rhs);
	virtual ~CState_Weaver_Attack_05_Begin() = default;

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
	_float m_fEasingTime = 0.f;
	_float m_fDir = 1.f;
	_float m_fMoveSpeed = 0.f;

	_float4	m_vStartLook;
	_float4	m_vStartPos;

	_float m_fTimeAcc_Effect0 = 0.f;
	_float m_fTimeAcc_Effect1 = 0.f;
	_uint	m_iEffectNum0 = 0;
	_bool	m_bToggle_Effect = false;
	DECL_ONCE(m_bEffect1);
	DECL_ONCE(m_bEffect2);
	DECL_ONCE(m_bEffect3);
	DECL_ONCE(m_bEffect4);
	DECL_ONCE(m_bEffect5);
	DECL_ONCE(m_bEffect6);

public:
	static CState_Weaver_Attack_05_Begin* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

