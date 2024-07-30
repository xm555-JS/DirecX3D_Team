#pragma once

#include "Client_Defines.h"
#include "State_Enemy_Normal.h"

BEGIN(Client)

class CState_Enemy_HyenaGang_Shooter_Attack_Normal_Shot : public CState_Enemy_Normal
{
public:
	CState_Enemy_HyenaGang_Shooter_Attack_Normal_Shot(CState_Manager* pManager, STATE_TYPE eState);
	CState_Enemy_HyenaGang_Shooter_Attack_Normal_Shot(const CState_Enemy_HyenaGang_Shooter_Attack_Normal_Shot& rhs);
	virtual ~CState_Enemy_HyenaGang_Shooter_Attack_Normal_Shot() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;



private:
	_bool			m_bOneShot = true;


public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Enemy_HyenaGang_Shooter_Attack_Normal_Shot* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END