#pragma once

#include "Client_Defines.h"
#include "State.h"

// 문제 터질 여지 있음 터질수 삭제 요망
#ifdef _DEBUG
#include "Imgui_Manager.h"  
#endif // _DEBUG


BEGIN(Engine)
class CCollider;
END

BEGIN(Client)


class CState_Player abstract : public CState
{
public:
	CState_Player(CState_Manager* pState_Manager, STATE_TYPE eState);
	CState_Player(const CState_Player& rhs);
	virtual ~CState_Player() = default;


public:
	virtual void Enter(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void Exit(void* pArg = nullptr);


protected:
	_float			m_fPlayerLookTimeAcc = 0.f;
	_float			m_fTimeDelta = 0.f;
	_float4			m_vPlayerLook;

	_uint			m_iCurAnimGroup = 0;
	_uint			m_iCurAnimState = 0;

	_float			m_fPreStamina = 0.f;

	_float4			m_vColScale;

protected: //전투
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);
	DECL_ONCE(m_bEffect2);
	DECL_ONCE(m_bEffect3);
	DECL_ONCE(m_bEffect4);
	DECL_ONCE(m_bEffect5);

	DECL_ONCE(m_bShaked0);
	DECL_ONCE(m_bShaked1);
	DECL_ONCE(m_bShaked2);
	DECL_ONCE(m_bShaked3);
	DECL_ONCE(m_bShaked4);
	DECL_ONCE(m_bShaked5);

	DECL_ONCE(m_bGauge0);
	DECL_ONCE(m_bGauge1);
	DECL_ONCE(m_bGauge2);
	DECL_ONCE(m_bGauge3);
	DECL_ONCE(m_bGauge4);
	DECL_ONCE(m_bGauge5);

	_bool	m_bHit0 = false;
	_bool	m_bHit1 = false;
	_bool	m_bHit2 = false;
	_bool	m_bHit3 = false;
	_bool	m_bHit4 = false;
	_bool	m_bHit5 = false;

protected:
	_float			m_fOldGravity = 0.f;
	CCollider*		m_pColCom = nullptr;
	class CPlayer*	m_pPlayer = nullptr;

protected:
	void	Hit_Unit(CCollider* pCollider, ELEMENTAL_PROPERTIE eElement, _float fPower, _uint iStartKeyFrame, _uint iEndKeyFrame, CAnimation* pTargetAnimation, _bool* bCheckHit, string EffectName = "");
	virtual void	Set_PlayerLook(_float fTimeDelta);
	void	Use_StaminaGauge(_float fGaugeRatio);
	_bool	Enable_Use_Stamina(class CState_Handler* pHandler, _bool bCreateAlertWindow = true);
	_bool	Enable_Use_Dash(class CState_Handler* pHandler, _bool bCreateAlertWindow = true);

public:
	virtual CState* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END