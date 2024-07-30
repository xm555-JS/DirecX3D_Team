#pragma once

#include "Client_Defines.h"
#include "State_AI.h"

BEGIN(Engine)
class CTransform;
class Animation;
class Channel;
class CNavigation;
class CCollider;
END

BEGIN(Client)

class CState_Npc abstract : public CState_AI
{
public:
	CState_Npc(CState_Manager* pManager, STATE_TYPE eState);
	CState_Npc(const CState_Npc& rhs);
	virtual ~CState_Npc() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

protected:
	CGameObject*			m_pOwner = nullptr;
	CGameObject*			m_pTarget = nullptr;
	CTransform*				m_pOwnerTranformCom = nullptr;
	CNavigation*			m_pNaviCom = nullptr;
	CCollider*				m_pColCom = nullptr;

protected:
	_float					m_fAccTime = 0.f;
	_uint					m_iScriptPhase = 0;

protected:
	STATE_TYPE		Ready_NextState();
	virtual _bool	Check_Condition(CState_Handler* pHandler) override;


public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void	Free() override;
};

END


