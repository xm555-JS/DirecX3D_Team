#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "State_Handler.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Sound_Manager.h"

BEGIN(Engine)
class CModel;
class CAnimation;
class CCollider;
END


BEGIN(Client)
class CState_Manager;
class CUnit;


class CState abstract : public CBase
{
public:
	CState(CState_Manager* pState_Manager, STATE_TYPE eState);
	CState(const CState& rhs);
	virtual ~CState() = default;

public:
	STATE_TYPE			Get_State() { return m_eState; }
	_float				Get_KeyPressTimeAcc() { return m_fKeyPressTimeAcc; }
	vector<STATE_TYPE>*	Get_ReserveState() { return &m_ReserveStates; }

	void				Reset_EndCurState() { m_bEndCurState = false; }
	void				Reset_SoundOption();
	void				Set_StateHandler(CState_Handler* pHandler) { m_pState_Handler = pHandler; }
	void				Set_Model(CModel* pModel) { m_pTargetModel = pModel; }
	void				Set_ReserveState(vector<STATE_TYPE>* ReserveState);


public:
	virtual HRESULT	Initialize_Prototype() { return S_OK; }
	virtual HRESULT	Initialize(void* pArg) PURE;
	virtual void	Enter(void* pArg = nullptr) {};
	virtual void	Tick(_float fTimeDelta);
	virtual void	Exit(void* pArg = nullptr) {};


public:
	virtual _bool	Check_Condition(CState_Handler* pHandler) PURE;
	STATE_TYPE		Ready_NextState();


protected:
	CState_Manager*			m_pState_Manager = nullptr;
	CModel*					m_pTargetModel = nullptr;
	CState_Handler*			m_pState_Handler = nullptr;

protected:
	vector<STATE_TYPE>		m_ForceStates;
	vector<STATE_TYPE>		m_ReserveStates;
	_bool					m_bEndCurState = false;
	STATE_TYPE				m_eState = STATE_END;
	_float4x4				m_RootBoneMatrix;


protected: /* For State Element*/
	_float					m_fKeyPressTimeAcc = 0.f;
	_float					m_fAnimSpeed       = 1.f;

protected:
	_bool						m_bSound[20] = { true,true,true,true,true,true,true,true,true,true,true,true,
												true,true,true,true,true,true,true,true };
	_float						m_fSound[10] = { 0.f };
	CSound_Manager::CHANNELID	m_eSoundChannel[10] = { CSound_Manager::CH_END };


protected:
	virtual void	Hit_Unit(CCollider* pCollider, ELEMENTAL_PROPERTIE eElement, _float fPower,	_uint iStartKeyFrame, _uint iEndKeyFrame, CAnimation* pTargetAnimation, _bool* bCheckHit, string EffectName = "") PURE;
	//(string strEffectName, _float4x4* matParent, _fvector vPos, _fvector vRotation, _float fPlaySpeed, _fvector vScale, _fvector vRevolution
	//void			Create_Effect_Positioning(string strEffectName, _float4x4* matParent, _fvector vPos = XMVectorSet(0.f, 0.f, 0.f, 1.f), _fvector vRotation = XMVectorSet(0.f, 0.f, 0.f, 0.f), _float fPlaySpeed = 1.f, _fvector vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f), _fvector vRevolution = XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//void			Create_Effect_Positioning_Rotaion(string strEffectName, _float4x4* matParent,  _fvector vPos = XMVectorSet(0.f, 0.f, 0.f, 1.f), _matrix matRotation = XMMatrixIdentity(), _float fPlaySpeed = 1.f, _fvector vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f), _fvector vRevolution = XMVectorSet(0.f, 0.f, 0.f, 0.f));
	_uint			Get_CurKeyFrame(_uint iChannelIndex = 0);
	_bool			Get_IsFinished(_uint iChannelIndex = 0);
	void			Load_UI(wstring strFileName);
	void			Set_CurKeyFrame(_uint iKeyFrame);

public:
	virtual CState* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END