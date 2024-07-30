#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Action final : public CCamera
{
public:
	typedef struct ActionCamDesc
	{
		ActionCamDesc()
		{
		}
		ActionCamDesc(_float3 _vTargetPos, _float3 _vTargetLook, _uint _iEasing, _float _fPlayTime)
		{
			vTargetPos = _vTargetPos;
			vTargetLook = _vTargetLook;
			iEasing = _iEasing;
			fPlayTime = _fPlayTime;
		}
		_float3		vTargetPos;
		_float3		vTargetLook;
		_uint		iEasing;
		_float		fPlayTime;
	}ACTIONCAMDESC;

public:
	CCamera_Action();
	CCamera_Action(const CCamera_Action& rhs);
	virtual ~CCamera_Action() = default;

public:
	ACTIONCAMDESC	Get_TargetPos(_uint iIdx) { return m_pActionDescs[iIdx]; }
	void			Set_TargetObejct(CGameObject* pTarget);
	void			Set_TargetObejct(CGameObject* pTarget, string strBoneName);
	void			Set_StartDesc(_float3 _vTargetPos, _float3 _vTargetLook);
	void			Set_EndDesc(_float3 _vTargetPos, _float3 _vTargetLook, _uint _iEasing, _float _fPlayTime);

	// TargetPos 까지 이동하는데 Easing의 방식으로 PlayTime동안 와라
	void			Set_ActionCamDesc(_float3 _vTargetPos, _float3 _vTargetLook, _uint _iEasing, _float _fPlayTime, _uint iIdx)
	{ 
		assert(0 <= iIdx);
		assert(m_iNumActionDesc - 2 > iIdx);
		m_pActionDescs[iIdx + 1] = ACTIONCAMDESC(_vTargetPos, _vTargetLook, _iEasing, _fPlayTime);
	}

	void			Reserve_NumActionDesc(_uint iNumActionDesc);
	void			Start_ActionCamera();
	virtual	void	Reset_CamInfo();


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick_Camera(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	ACTIONCAMDESC*		m_pActionDescs = nullptr;
	_uint				m_iNumActionDesc = 0;
	_float				m_fTimeAcc = 0.f;
	_uint				m_iCurCamIdx = 0;


private:
	CGameObject*		m_pTarget = nullptr;
	CTransform*			m_pTargetTransform = nullptr;
	string				m_pTargetBoneName = "";


private:
	void				Play_CurActionCam();

public:
	static CCamera_Action* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END