#pragma once

#include "Client_Defines.h"
#include "State_Boss.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CState_Dragon_Chase : public CState_Boss
{
public:
	CState_Dragon_Chase(CState_Manager* pManager, STATE_TYPE eState);
	CState_Dragon_Chase(const CState_Dragon_Chase& rhs);
	virtual ~CState_Dragon_Chase() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	CCollider* m_pColCom = nullptr;

private:
	_float4			m_vSpot;
	_float4			m_vStartLook;
	_float			m_fGoalY = 0.f;
	_float			m_fEasingTime = 0.f;
	_float			m_fDownTime = 0.f;

public:
	static CState_Dragon_Chase* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END

