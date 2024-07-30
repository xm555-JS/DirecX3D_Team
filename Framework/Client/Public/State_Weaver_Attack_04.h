#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Weaver_Attack_04 : public CState_Boss
{
public:
	CState_Weaver_Attack_04(CState_Manager* pManager, STATE_TYPE eState);
	CState_Weaver_Attack_04(const CState_Weaver_Attack_04& rhs);
	virtual ~CState_Weaver_Attack_04() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

	void	Set_HurricanePos();

private:
	CGameObject*	m_pHurricane = nullptr;
	_bool			m_bCreateObj = false;

	_float			m_fTimeAcc_Effect0 = 0.f;
	_float			m_fTimeAcc_Effect1 = 0.f;
	_float			m_fTimeAcc_Effect2 = 0.f;
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);
	DECL_ONCE(m_bEffect2);
	DECL_ONCE(m_bEffect3);
	DECL_ONCE(m_bEffect4);
	DECL_ONCE(m_bEffect5);
	_uint			m_iEffect = 0.f;

	_bool			m_bHit[20] = { false, };

	_uint			m_iSoundChannel = 0;

public:
	static CState_Weaver_Attack_04* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

