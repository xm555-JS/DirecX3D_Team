#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Client)

class CState_Dragon_CameraAction : public CState_Boss
{
public:
	CState_Dragon_CameraAction(CState_Manager* pManager, STATE_TYPE eState);
	CState_Dragon_CameraAction(const CState_Dragon_CameraAction& rhs);
	virtual ~CState_Dragon_CameraAction() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

private:
	_float m_fTimeAcc_Effect0 = 0.f;
	DECL_ONCE(m_bEffect0);
public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

public:
	static CState_Dragon_CameraAction* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

