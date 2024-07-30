#pragma once

#include "Base.h"

/* 현재 게임내에 보여줘야할 레벨을 보관한다. */
/* 레벨교체시에 기존 레벨을 삭제한다. */
/* 기존레벨에서 사용했던 객체와 자원(리소스등)을 정리한다. */

BEGIN(Engine)
class CCamera;

class CCamera_Manager final : public CBase
{
public:
	typedef struct Shaking_Desc
	{
		_float4		vAxis;
		_float		fPower;
		_float		fTotalPlayTime;
		_float		fCurPlayTime;
		_float		fCycleTime;
	}SHAKING_DESC;

	DECLARE_SINGLETON(CCamera_Manager)


public:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	CCamera*	Get_CurrentCamera() { return m_pCurrentCamera; }
	_bool		Get_CursorFix() { return m_bCursorFixState; }

	void		Set_CurrentCamera(CCamera* pCurrentCamera);
	void		Set_NextCamera(CCamera* pNextCamera, _float fTotalMoveTime);
	void		Set_CursorFix(_bool bFixState) { m_bCursorFixState = bFixState; }


public:
	CCamera*			Find_Camera(const _tchar* pCameraTag);
	list<CCamera*>*		Find_MC(const _tchar* pMCTag);

	HRESULT				Add_MC(const _tchar* pMCTag, const _tchar* pCameraTag);
	void				Add_Camera_Shaking(_fvector vAxis, _float fPower, _float fPlayTime, _float fCycle);
	void				Add_Camera(const _tchar* pCameraTag, CCamera* pCamera) { m_Cameras.emplace(pCameraTag, pCamera); }

	void				Play_ActionCamera(const _tchar* pCameraTag, _float fTotalMoveTime);
	void				Play_MAC(const _tchar* pCameraTag, _float fTotalMoveTime);

	HRESULT		Reserve_MoveCam();


public:
	HRESULT		Initialize(HWND hWnd, _uint iWinCX, _uint iWinCY);
	void		Tick(_float fTimeDelta);

	
private:
	CCamera*			m_pCurrentCamera = nullptr;
	CCamera*			m_pNextCamera = nullptr;
	CCamera*			m_pPreCamera = nullptr;

	CCamera*			m_pMoveCamera = nullptr;

private:
	CCamera*			m_pOriginCamera = nullptr;

private:
	_float				m_fTotalMoveTime = 0.f;
	_float				m_fReserveTotalMoveTime = 0.f;
	_float				m_fMoveTimeAcc = 0.f;

	_float				m_fTimeDelta = 0.f;

private:
	_uint				m_iWinCX = 0;
	_uint				m_iWinCY = 0;
	HWND				m_hWnd;

private:
	_bool				m_bCursorFixState = false;


private:
	list<SHAKING_DESC>		m_ShakingDescs;
	queue<CCamera*>			m_PlayACs;

private:
	map<const _tchar*, CCamera*>					m_Cameras;
	typedef map<const _tchar*, CCamera*>			CAMERAS;

	map<const _tchar*, list<CCamera*>>				m_MCs;
	typedef map<const _tchar*, list<CCamera*>>		MCS;

private:
	void			Shaking_Camera(class CCamera* pTargetCamera, _float fTimeDelta);

private:
	void			Tick_MoveCamera(CCamera* pMoveCamera, CCamera* pNextCamera, CCamera* pPreCamera);
	void			Tick_ActionCamera();
	

public:
	virtual void Free() override;
};

END