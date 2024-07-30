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

class CState_Boss abstract : public CState_AI
{
public:
	CState_Boss(CState_Manager* pManager, STATE_TYPE eState);
	CState_Boss(const CState_Boss& rhs);
	virtual ~CState_Boss() = default;

public:
	_bool Get_ChangePhase() { return m_bChangePhase; }

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	
	void Tick_AnimationFix(_float fTimeDelta, string strBone, CTransform* pTransform);
	void Tick_AnimationLerp(_float fTimeDelta, string strBone, CTransform* pTransform, _matrix RotateOffsetMatrix, _uint iEasingtype = 0);
	virtual void Exit(void* pArg = nullptr) override;

protected:
	CGameObject*			m_pOwner = nullptr;
	CGameObject*			m_pTarget = nullptr;
	CTransform*				m_pOwnerTranformCom = nullptr;
	CNavigation*			m_pNaviCom = nullptr;
	CCollider*				m_pColCom = nullptr;

protected:
	_float					m_fAccTime = 0.f;
	_float					m_fTimeDelta = 0.f;
	_float4					m_vTargetPos;
	_float					m_fStateTime = 0.f;
	_bool					m_bChangePhase = false;

	_float4					m_vColScale;

protected:
	STATE_TYPE		Ready_NextState();
	virtual _bool Check_Condition(CState_Handler* pHandler) override;


public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END


