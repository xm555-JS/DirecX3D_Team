#include "..\Public\ScreenEffect_Manager.h"

#include "GameInstance.h"
#include "Render_Manager.h"
#include "Easing_Utillity.h"


IMPLEMENT_SINGLETON(CScreenEffect_Manager);

CScreenEffect_Manager::CScreenEffect_Manager()
{
	m_pRender_Manager = CRender_Manager::Get_Instance();
}

HRESULT CScreenEffect_Manager::Initialize()
{

	return S_OK;
}

void CScreenEffect_Manager::Tick(_float fTimeDelta)
{
	Tick_RadialCA(fTimeDelta);
	Tick_DirCA(fTimeDelta);
	Tick_RadialBlur(fTimeDelta);
}

void CScreenEffect_Manager::Add_DirCA(_float2 vDir, _float4 vRGBPower, _float fTotalPlayTime, _float fCycleTime, _bool bLoop, _bool bContinue)
{
	if(m_DirCADescs.size())
		m_DirCADescs.pop_back();

	DIRCA_DESC Desc;
	Desc.vDir = vDir;
	Desc.vRGBPower = vRGBPower;
	Desc.fTotalPlayTime = fTotalPlayTime;
	Desc.fCurPlayTime = 0.f;
	Desc.fCycleTime = fCycleTime;
	Desc.bLoop = bLoop;
	Desc.bContinue = bContinue;

	m_DirCADescs.push_back(Desc);
}

void CScreenEffect_Manager::Add_RadialCA(_float2 vPoint, _float4 vPower, _float fTotalPlayTime, _float fCycleTime, _bool bLoop, _bool bContinue)
{
	if (m_RadialCADescs.size())
		m_RadialCADescs.pop_back();

	RADIALCA_DESC Desc;
	Desc.vPoint = vPoint;
	Desc.vPower = vPower;
	Desc.fTotalPlayTime = fTotalPlayTime;
	Desc.fCurPlayTime = 0.f;
	Desc.fCycleTime = fCycleTime;
	Desc.bLoop = bLoop;
	Desc.bContinue = bContinue;

	m_RadialCADescs.push_back(Desc);
}

void CScreenEffect_Manager::Add_RadialBlur(_float2 vPoint, _float fPower, _float fTotalPlayTime, _float fCycleTime, _float fRadialBlurStart, _float fRadialBlurWidth, _bool bLoop, _bool bContinue)
{
	if (m_RadialBlurDescs.size())
		m_RadialBlurDescs.pop_back();

	RADIALBLUR_DESC Desc;
	Desc.vPoint = vPoint;
	Desc.fPower = fPower;
	Desc.fTotalPlayTime = fTotalPlayTime;
	Desc.fCurPlayTime = 0.f;
	Desc.fCycleTime = fCycleTime;
	Desc.fRadialBlurStart = fRadialBlurStart;
	Desc.fRadialBlurWidth = fRadialBlurWidth;
	Desc.bLoop = bLoop;
	Desc.bContinue = bContinue;

	m_RadialBlurDescs.push_back(Desc);
}

void CScreenEffect_Manager::Stop_DirCA()
{
	if (0 < m_DirCADescs.size())
	{
		m_DirCADescs.front().bLoop = false;
		m_DirCADescs.front().bContinue = false;
		//if (true == m_DirCADescs.front().bContinue)
		//{
		//	m_DirCADescs.front().fCurPlayTime -= m_DirCADescs.front().fCycleTime;
		//	m_DirCADescs.front().bContinue = false;
		//}
	}
}

void CScreenEffect_Manager::Stop_RadialCA()
{
	if (0 < m_RadialCADescs.size())
	{
		m_RadialCADescs.front().bLoop = false;
		m_RadialCADescs.front().bContinue = false;
		//if (true == m_RadialCADescs.front().bContinue)
		//{
		//	m_RadialCADescs.front().fCurPlayTime -= m_RadialCADescs.front().fCycleTime;
		//	m_RadialCADescs.front().bContinue = false;
		//}
	}
}

void CScreenEffect_Manager::Stop_RadialBlur()
{
	if (0 < m_RadialBlurDescs.size())
	{
		m_RadialBlurDescs.front().bLoop = false;
		m_RadialBlurDescs.front().bContinue = false;
		//if (true == m_RadialBlurDescs.front().bContinue)
		//{
		//	m_RadialBlurDescs.front().fCurPlayTime -= m_RadialBlurDescs.front().fCycleTime;
		//	m_RadialBlurDescs.front().bContinue = false;
		//}
	}
}

void CScreenEffect_Manager::Tick_DirCA(_float fTimeDelta)
{
	if (0 == m_DirCADescs.size() )
		return;

	DIRCA_DESC* pDesc = &m_DirCADescs.front();
	_float2	vRDir, vGDir, vBDir;

	_bool	bCAEnd = false;
	if (pDesc->fTotalPlayTime < pDesc->fCurPlayTime)
	{
		if (true == pDesc->bLoop)
		{
			pDesc->fCurPlayTime -= pDesc->fCycleTime;
		}
		else
		{
			bCAEnd = true;
			pDesc->fCurPlayTime = pDesc->fTotalPlayTime;
		}
	}
	else if (pDesc->fTotalPlayTime * 0.5f < pDesc->fCurPlayTime)
	{
		if (true == pDesc->bContinue)
		{
			pDesc->fCurPlayTime -= fTimeDelta;
		}
	}
	_vector	vPower;
	vPower = CEasing_Utillity::sinfInOut(
		XMVectorSet(0.f, 0.f, 0.f, 0.f),
		XMVectorSet(pDesc->vRGBPower.x, pDesc->vRGBPower.y, pDesc->vRGBPower.z, pDesc->vRGBPower.w),
		_double(pDesc->fCurPlayTime),
		_double(pDesc->fCycleTime * 0.5f)
	);

	pDesc->fCurPlayTime += fTimeDelta;

	if (bCAEnd)
		m_DirCADescs.pop_front();
	
	XMStoreFloat2(&vRDir, XMVector2Normalize(XMLoadFloat2(&pDesc->vDir)) * XMVectorGetX(vPower));
	XMStoreFloat2(&vGDir, XMVector2Normalize(XMLoadFloat2(&pDesc->vDir)) * XMVectorGetY(vPower));
	XMStoreFloat2(&vBDir, XMVector2Normalize(XMLoadFloat2(&pDesc->vDir)) * XMVectorGetZ(vPower));

	m_pRender_Manager->Set_DirCA(vRDir, vGDir, vBDir);
}

void CScreenEffect_Manager::Tick_RadialCA(_float fTimeDelta)
{
	if (0 == m_RadialCADescs.size())
		return;

	RADIALCA_DESC* pDesc = &m_RadialCADescs.front();

	_bool	bCAEnd = false;
	if (pDesc->fTotalPlayTime < pDesc->fCurPlayTime)
	{
		if (true == pDesc->bLoop)
		{
			pDesc->fCurPlayTime -= pDesc->fCycleTime;
		}
		else
		{
			bCAEnd = true;
			pDesc->fCurPlayTime = pDesc->fTotalPlayTime;
		}
	}
	else if (pDesc->fTotalPlayTime * 0.5f < pDesc->fCurPlayTime)
	{
		if (true == pDesc->bContinue)
		{
			pDesc->fCurPlayTime -= fTimeDelta;
		}
	}
	_float4	vFinalPower;
	XMStoreFloat4(&vFinalPower, CEasing_Utillity::sinfInOut(
		XMVectorSet(0.f, 0.f, 0.f, 0.f),
		XMLoadFloat4(&pDesc->vPower),
		_double(pDesc->fCurPlayTime),
		_double(pDesc->fCycleTime * 0.5f)
	));

	pDesc->fCurPlayTime += fTimeDelta;

	if (bCAEnd)
		m_RadialCADescs.pop_front();

	m_pRender_Manager->Set_RadialCA(pDesc->vPoint, vFinalPower);
}

void CScreenEffect_Manager::Tick_RadialBlur(_float fTimeDelta)
{
	if (0 == m_RadialBlurDescs.size())
		return;

	RADIALBLUR_DESC* pDesc = &m_RadialBlurDescs.front();

	_bool	bCAEnd = false;
	if (pDesc->fTotalPlayTime < pDesc->fCurPlayTime)
	{
		if (true == pDesc->bLoop)
		{
			pDesc->fCurPlayTime -= pDesc->fCycleTime;
		}
		else
		{
			bCAEnd = true;
			pDesc->fCurPlayTime = pDesc->fTotalPlayTime;
		}
	}
	else if (pDesc->fTotalPlayTime * 0.5f < pDesc->fCurPlayTime)
	{
		if (true == pDesc->bContinue)
		{
			pDesc->fCurPlayTime -= fTimeDelta;
		}
	}


	_float	fFinalPower;
	fFinalPower = XMVectorGetX(CEasing_Utillity::sinfInOut(
		XMVectorSet(0.f, 0.f, 0.f, 0.f),
		XMVectorSet(pDesc->fRadialBlurWidth, 0.f, 0.f, 0.f),
		_double(pDesc->fCurPlayTime),
		_double(pDesc->fCycleTime * 0.5f)
	));

	pDesc->fCurPlayTime += fTimeDelta;

	if (bCAEnd)
		m_RadialBlurDescs.pop_front();

	m_pRender_Manager->Set_RadialBlur(pDesc->vPoint, pDesc->fPower, pDesc->fRadialBlurStart, fFinalPower);
}

void CScreenEffect_Manager::Free()
{
	__super::Free();
}