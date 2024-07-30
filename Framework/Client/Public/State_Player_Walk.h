#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Engine)
class CTransform;
END


BEGIN(Client)

class CState_Player_Walk : public CState_Player
{
public:
	CState_Player_Walk(CState_Manager* pManager, STATE_TYPE eState);
	CState_Player_Walk(const CState_Player_Walk& rhs);
	virtual ~CState_Player_Walk() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Enter(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void Exit(void* pArg = nullptr);

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;

private:
	void	EasingTurn(CTransform* pTargetTransform, _fvector vTargetLook);


public:
	static CState_Player_Walk* Create(CState_Manager* pManager, STATE_TYPE eState);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};

END