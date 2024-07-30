#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Robella_Attack9_Begin : public CState_Boss
{
public:
	CState_Robella_Attack9_Begin(CState_Manager* pManager, STATE_TYPE eState);
	CState_Robella_Attack9_Begin(const CState_Robella_Attack9_Begin& rhs);
	virtual ~CState_Robella_Attack9_Begin() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	CGameObject* m_pPlayer = nullptr;

private:
	DECL_ONCE(m_bEffect0);

	_bool	m_bCreate = false;

private:
	_float m_fAccTime = 0.f;
	_vector m_vPlayerPos = { 0.f,0.f ,0.f ,0.f };

public:
	static CState_Robella_Attack9_Begin* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

