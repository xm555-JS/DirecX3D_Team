#pragma once

#include "Client_Defines.h"
#include "State_AI.h"
#include "Imgui_Manager.h"
BEGIN(Engine)
class CTransform;
class Animation;
class Channel;
class CNavigation;
class CCollider;
END

BEGIN(Client)

class CState_Enemy_Normal abstract : public CState_AI
{
protected:
	enum COLLIDER_OWNER
	{
		COLLIDER_WEAPON,
		COLLIDER_OWNER,

		COLLIDER_OWNER_END,
	};
	enum HIT_NUM
	{
		HIT_1,
		HIT_2,
		HIT_3,
		HIT_4,
		HIT_5,
		HIT_6,
		HIT_7,
		HIT_8,
		HIT_9,
		HIT_10,

		HIT_END
	};
public:
	CState_Enemy_Normal(CState_Manager* pManager, STATE_TYPE eState);
	CState_Enemy_Normal(const CState_Enemy_Normal& rhs);
	virtual ~CState_Enemy_Normal() = default;

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Exit(void* pArg = nullptr) override;

public:
	virtual _bool Check_Condition(CState_Handler* pHandler) override;
	virtual void Set_DamagedAction(_bool* bCheckHit, _float fSpeed = 0.15f);

protected:	
	_float Get_PlayerPosLength();
	_float Get_StartPosLength(CState_Handler* _pState_Handler);
	_vector Get_LookAtPlayerPos_Dir();
	_vector Get_LookAtStartPos_Dir();
	_uint Get_CurrentKeyFrame();

	_bool Is_AnimFinished();
	_bool Is_OwnerDead(CState_Handler* _pState_Handler);
	_bool Is_Damaged();
	_bool Is_Damaged(CState_Handler* _pState_Handler);
	void Set_CurrentHp();

protected:
	CGameObject*		m_pOwner = nullptr;
	CGameObject*		m_pTarget = nullptr;
	CTransform*			m_pOwnerTranformCom = nullptr;
	CNavigation*		m_pNaviCom = nullptr;

protected:
	_vector				m_vStartColScale;
	_vector				m_vStartColPos;

	_float4				m_vStartLook;

	_float				m_fSpeed = 0.f;
	_float				m_fAccTime = 0.f;
	_float				m_fEasingTime = 0.f;

	string				m_strRootBoneName;

protected:
	_float				m_fRunStartDistance = 0.f;
	_float				m_fRunEndDistance = 0.f;

	_float				m_fReturnStartDistance = 0.f;
	_float				m_fPlayerPosLength = 0.f; //ReturnEndDistance조건
	_float				m_fStartPosLength = 0.f; //ReturnEndDistance조건
	
	_float				m_fBackStartDistance = 0.f;
	_float				m_fBackEndDistance = 0.f;
	
	_float				m_fAttackLength_Min = 0.f;
	_float				m_fAttackLength_Max = 0.f;
	
	_float				m_fPatrolLength = 0.f;

	_float				m_fStopTime = 0.f;

protected:
	_uint				m_iTurnDir = 0.f;
	CCollider*			m_pColCom[COLLIDER_END];
	_bool				m_bHit[HIT_END] = { false ,false ,false ,false ,false ,false ,false ,false ,false ,false };
	_bool				m_bPushed = false;

public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
#if defined(_TEST_JJ)

#endif
};

END


