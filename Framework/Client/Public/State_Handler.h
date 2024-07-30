#pragma once

#include "Client_Defines.h"
#include "Component.h"

BEGIN(Engine);
class CModel;
END

BEGIN(Client)
class CState;
class CState_Manager;

class CState_Handler : public CComponent
{
private:
	CState_Handler();
	CState_Handler(const CState_Handler& rhs);
	virtual ~CState_Handler() = default;

public:
	_float				Get_TimeAcc() { return m_fTimeAcc; }
	_uint				Get_AnimationIdx(STATE_TYPE eState) { return m_AnimationIdx[eState]; }
	STATE_TYPE			Get_PreStateType() { return m_ePreStateType; }
	STATE_TYPE			Get_CurStateType() { return m_eCurStateType; }
	CState*				Get_CurState() { return m_pCurState; }
	CState*				Get_State(STATE_TYPE eState) { return m_arrStates[eState]; }
	CGameObject*		Get_Player() { return m_pPlayer; }
	_float4				Get_PlayerLook() { return m_vPlayerLook; }
	_bool				Get_JumpState() { return m_bJump; }

	_bool				Get_KeyInput() { return m_bKeyInput; }
	_float				Get_KeyPressingTime(_uint iKey);
	list<_uint>*		Get_KeyInputList() { return &m_KeyInput; }

	void		Set_ModelComponent(CModel* pModel) { m_pTargetModel = pModel; }
	void		Set_Animation(STATE_TYPE eState, _uint iAnimIdx);
	void		SetUp_StartState(STATE_TYPE eState, void* pArg = nullptr);
	void		Set_TimeAcc(_float fTimeAcc) { m_fTimeAcc = fTimeAcc; }
	void		Set_Player(CGameObject* pObject) { m_pPlayer = pObject; }
	void		Set_PlayerLook(_vector vLook) { XMStoreFloat4(&m_vPlayerLook, vLook); }
	void		Set_JumpState(_bool bState) { m_bJump = bState; }
	void		Set_CurStateType(STATE_TYPE eType) { m_eCurStateType = eType; }
	void		Set_CurState(CState* pState) { m_pCurState = pState; }

	void		Set_KeyInput(_bool bKeyInputState) { m_bKeyInput = bKeyInputState; }
	void		Set_KeyPressingTime(_uint iKey,_float fTime = 0.f);

	void		Add_KeyInput(_uint iKey) { m_KeyInput.push_back(iKey); }
	void		Add_KeyPressingTime(_uint iKey, _float fTimeDelta);

	void		Change_State(STATE_TYPE eState, void* pEnterArg = nullptr, void* pExitArg = nullptr);
	void		Begin_State(STATE_TYPE eState, void* pEnterArg = nullptr, void* pExitArg = nullptr);
	void		End_State(STATE_TYPE eState, void* pEnterArg = nullptr, void* pExitArg = nullptr);

public:
	virtual HRESULT	Initialize(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;
	void			Tick(_float fTimeDelta);


private:
	CModel*			m_pTargetModel = nullptr;
	CState_Manager* m_pState_Manager = nullptr;

private:
	CState*			m_arrStates[STATE_END];
	CState*			m_pCurState = nullptr;

private:
	_uint			m_AnimationIdx[STATE_END];

	STATE_TYPE		m_ePreStateType = STATE_END;
	STATE_TYPE		m_eCurStateType = STATE_END;

private: /* For State Element*/
	_float			m_fTimeAcc = 0.f;
	_bool			m_bKeyInput = false;

private:
	CGameObject*	m_pPlayer = nullptr;
	_float4			m_vPlayerLook = {0.f, 0.f, 1.f, 0.f};
	_bool			m_bJump = false;

private:
	list<_uint>			m_KeyInput;
	map<_uint, _float>	m_KeyPressings;

public:
	static CState_Handler* Create();
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END