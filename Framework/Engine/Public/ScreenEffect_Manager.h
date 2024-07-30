#pragma once

#include "Base.h"

BEGIN(Engine)

class CScreenEffect_Manager final : public CBase
{
public:
	typedef struct DirCA_Desc
	{
		_float2		vDir;
		_float4		vRGBPower;
		_float		fTotalPlayTime;
		_float		fCurPlayTime;
		_float		fCycleTime;
		_bool		bLoop;
		_bool		bContinue;
	}DIRCA_DESC;

	typedef struct RadialCA_Desc
	{
		_float2		vPoint;
		_float4		vPower;
		_float		fTotalPlayTime;
		_float		fCurPlayTime;
		_float		fCycleTime;
		_bool		bLoop;
		_bool		bContinue;
	}RADIALCA_DESC;

	typedef struct RadialBlur_Desc
	{
		_float2		vPoint;
		_float		fPower;
		_float		fTotalPlayTime;
		_float		fCurPlayTime;
		_float		fCycleTime;
		_float		fRadialBlurStart;
		_float		fRadialBlurWidth;
		_bool		bLoop;
		_bool		bContinue;
	}RADIALBLUR_DESC;

	DECLARE_SINGLETON(CScreenEffect_Manager)



private:
	CScreenEffect_Manager();
	virtual ~CScreenEffect_Manager() = default;


public:
	HRESULT Initialize();
	void	Tick(_float fTimeDelta);

public:
	void	Add_DirCA(_float2 vDir, _float4 vRGBPower, _float fTotalPlayTime, _float fCycleTime, _bool bLoop = false, _bool bContinue = false);
	void	Add_RadialCA(_float2 vPoint, _float4 vPower, _float fTotalPlayTime, _float fCycleTime, _bool bLoop = false, _bool bContinue = false);
	void	Add_RadialBlur(_float2 vPoint, _float fPower, _float fTotalPlayTime, _float fCycleTime, _float fRadialBlurStart, _float fRadialBlurWidth, _bool bLoop = false, _bool bContinue = false);

	void	Stop_DirCA();
	void	Stop_RadialCA();
	void	Stop_RadialBlur();

private:
	class CRender_Manager* m_pRender_Manager= nullptr;

private:
	list<DIRCA_DESC>		m_DirCADescs;
	list<RADIALCA_DESC>		m_RadialCADescs;
	list<RADIALBLUR_DESC>	m_RadialBlurDescs;

private:
	void	Tick_DirCA(_float fTimeDelta);
	void	Tick_RadialCA(_float fTimeDelta);
	void	Tick_RadialBlur(_float fTimeDelta);
	

public:
	virtual void Free() override;
};

END