#include "..\Public\Camera_Manager.h"
#include "Camera.h"
#include "GameInstance.h"
#include "PipeLine.h"

#include "Easing_Utillity.h"

IMPLEMENT_SINGLETON(CCamera_Manager)

CCamera_Manager::CCamera_Manager()
{
}

HRESULT CCamera_Manager::Initialize(HWND hWnd, _uint iWinCX, _uint iWinCY)
{
	m_hWnd = hWnd;
	m_iWinCX = iWinCX;
	m_iWinCY = iWinCY;

	return S_OK;
}

void CCamera_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentCamera)
		return;

	m_fTimeDelta = fTimeDelta;

	if (true == m_pCurrentCamera->Is_Dead())
	{
		m_pCurrentCamera = nullptr;
		return;
	}

	if (GetFocus() && m_bCursorFixState)
	{
		POINT pt;
		pt.x = m_iWinCX / 2;
		pt.y = m_iWinCY / 2;

		ClientToScreen(m_hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
		//SetCursor(NULL);
	}

	Shaking_Camera(m_pCurrentCamera, fTimeDelta);

	if (1 < m_PlayACs.size())
		m_PlayACs.pop();

	m_pCurrentCamera->Tick_Camera(fTimeDelta);

	if (m_PlayACs.size())
		Tick_ActionCamera();

	if (m_pNextCamera)
		Tick_MoveCamera(m_pMoveCamera, m_pNextCamera, m_pPreCamera);
}

void CCamera_Manager::Set_CurrentCamera(CCamera* pCurrentCamera)
{
	m_pCurrentCamera = pCurrentCamera;
	Tick(m_fTimeDelta);
	GAMEINSTANCE->Set_CurRenderer(m_pCurrentCamera->Get_Renderer());
	CCamera::CAMERADESC	CameraDesc = m_pCurrentCamera->Get_CameraDesc();
	CPipeLine::Get_Instance()->Set_NearFar(CameraDesc.fNear, CameraDesc.fFar);
	CPipeLine::Get_Instance()->Tick();
}

void CCamera_Manager::Set_NextCamera(CCamera* pNextCamera, _float fTotalMoveTime)
{
	m_pNextCamera = pNextCamera;
	m_pPreCamera = m_pCurrentCamera;
	m_fTotalMoveTime = fTotalMoveTime;

	m_pMoveCamera->Set_CameraDesc(m_pCurrentCamera->Get_CameraDesc());
	m_pMoveCamera->Get_Transform()->Set_WorldMatrix(m_pCurrentCamera->Get_Transform()->Get_WorldMatrix());
	m_pCurrentCamera = pNextCamera;
	m_fMoveTimeAcc = 0.f;
}

CCamera* CCamera_Manager::Find_Camera(const _tchar* pCameraTag)
{
	auto	iter = find_if(m_Cameras.begin(), m_Cameras.end(), CTag_Finder(pCameraTag));

	if (iter == m_Cameras.end())
		return nullptr;

	return iter->second;
}

list<CCamera*>* CCamera_Manager::Find_MC(const _tchar* pMCTag)
{
	auto	iter = find_if(m_MCs.begin(), m_MCs.end(), CTag_Finder(pMCTag));

	if (iter == m_MCs.end())
		return nullptr;

	return &iter->second;
}

HRESULT CCamera_Manager::Add_MC(const _tchar* pMRTTag, const _tchar* pCameraTag)
{
	CCamera* pCamera = Find_Camera(pCameraTag);
	if (nullptr == pCamera)
		__debugbreak();

	list<CCamera*>* pMCList = Find_MC(pMRTTag);

	if (nullptr == pMCList)
	{
		list<CCamera*>		MCList;
		MCList.push_back(pCamera);

		m_MCs.emplace(pMRTTag, MCList);
	}
	else
	{
		pMCList->push_back(pCamera);
	}

	return S_OK;
}

void CCamera_Manager::Add_Camera_Shaking(_fvector vAxis, _float fPower, _float fPlayTime, _float fCycleTime)
{
	SHAKING_DESC	Desc;
	ZeroMemory(&Desc, sizeof(SHAKING_DESC));

	NULLCHECK(m_pCurrentCamera);

	//_matrix CurCameraRotatinoMatrix = m_pCurrentCamera->Get_Transform()->Get_RotationMatrix();
	//_vector vFinalAxis = XMVector3TransformNormal(vAxis, CurCameraRotatinoMatrix);


	//XMStoreFloat4(&Desc.vAxis, vFinalAxis);
	XMStoreFloat4(&Desc.vAxis, vAxis);
	Desc.vAxis.w = 0.f;
	Desc.fPower = fPower;
	Desc.fTotalPlayTime = fPlayTime;
	Desc.fCurPlayTime = 0.f;
	Desc.fCycleTime = fCycleTime;

	m_ShakingDescs.push_back(Desc);
}

void CCamera_Manager::Play_ActionCamera(const _tchar* pCameraTag, _float fTotalMoveTime)
{
	CCamera* pCamera = Find_Camera(pCameraTag);
	NULLCHECK(pCamera);

	m_fReserveTotalMoveTime = fTotalMoveTime;

	m_pOriginCamera = m_pCurrentCamera;
	m_PlayACs.push(pCamera);

	pCamera->Reset_CamInfo();
	Set_NextCamera(pCamera, fTotalMoveTime);
}

void CCamera_Manager::Play_MAC(const _tchar* pCameraTag, _float fTotalMoveTime)
{
	list<CCamera*>* pMC = Find_MC(pCameraTag);
	NULLCHECK(pMC);

	m_fReserveTotalMoveTime = fTotalMoveTime;

	m_pOriginCamera = m_pCurrentCamera;
	for (auto& pCamera : *pMC)
	{
		m_PlayACs.push(pCamera);
		pCamera->Reset_CamInfo();
	}

	Set_NextCamera(m_PlayACs.front(), fTotalMoveTime);
}

HRESULT CCamera_Manager::Reserve_MoveCam()
{
	USEGAMEINSTANCE;
	CCamera::CAMERADESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	m_pMoveCamera = CCamera::Create();
	NULLCHECK(m_pMoveCamera);

	m_pMoveCamera->Set_Clone();
	m_pMoveCamera->Initialize(&CameraDesc);

	return S_OK;
}

void CCamera_Manager::Shaking_Camera(class CCamera* pTargetCamera, _float fTimeDelta)
{
	if (0 == m_ShakingDescs.size())
	{
		CTransform* pTargetTransform = pTargetCamera->Get_Transform();
		_vector	vUp = { 0.f, 1.f, 0.f, 0.f };
		_vector vTargetCameraUp = XMVector3Normalize(pTargetTransform->Get_State(CTransform::STATE_UP));
		_vector vTargetCameraLook = XMVector3Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK));
		_vector vFinalUp = XMVector3Normalize(vTargetCameraUp * 1.f + vUp);
		_vector vRight = XMVector3Cross(vFinalUp, vTargetCameraLook);
		vFinalUp = XMVector3Cross(vTargetCameraLook, vRight);

		pTargetTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		pTargetTransform->Set_State(CTransform::STATE_UP, vFinalUp);
		pTargetTransform->Set_State(CTransform::STATE_LOOK, vTargetCameraLook);

		return;
	}

	NULLCHECK(pTargetCamera);

	_vector vTargetCameraLook = pTargetCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	_vector vTargetCameraRight = pTargetCamera->Get_Transform()->Get_State(CTransform::STATE_RIGHT);
	_vector	vTotalLookPower = { 0.f, 0.f, 0.f, 0.f };
	_vector	vTotalRightPower = { 0.f, 0.f, 0.f, 0.f };

	auto& iter = m_ShakingDescs.begin();
	for (; iter != m_ShakingDescs.end();)
	{
		_bool	bShakingEnd = false;

		if (iter->fTotalPlayTime < iter->fCurPlayTime)
		{
			bShakingEnd = true;
			iter->fCurPlayTime = iter->fTotalPlayTime;
		}


		_float fPower = XMVectorGetX(CEasing_Utillity::sinfInOut(
			XMVectorSet(0.f, 0.f, 0.f, 0.f),
			XMVectorSet(iter->fPower, 0.f, 0.f, 0.f),
			_double(iter->fCurPlayTime + iter->fCycleTime * 0.25f),
			_double(iter->fCycleTime * 0.5f)
		)) - iter->fPower * 0.5f;

		_fvector	vAxis = XMLoadFloat4(&iter->vAxis);
		_matrix CurCameraRotatinoMatrix = m_pCurrentCamera->Get_Transform()->Get_RotationMatrix();
		_vector vFinalAxis = XMVector3TransformNormal(vAxis, CurCameraRotatinoMatrix);

		_vector vQuaternion = XMQuaternionRotationAxis(vFinalAxis, fPower);
		_vector vLookPower = XMVector3Rotate(vTargetCameraLook, vQuaternion);
		_vector vRightPower = XMVector3Rotate(vTargetCameraRight, vQuaternion);


		vTotalLookPower += vLookPower;
		vTotalRightPower += vRightPower;



		iter->fCurPlayTime += fTimeDelta;

		if (bShakingEnd)
		{
			iter = m_ShakingDescs.erase(iter);
			continue;
		}
		else
			++iter;
	}

	vTotalLookPower = XMVector3Normalize(vTotalLookPower);
	vTotalRightPower = XMVector3Normalize(vTotalRightPower);

	_vector vUp = XMVector3Normalize(XMVector3Cross(vTotalLookPower, vTotalRightPower));
	_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, vTotalLookPower));

	CTransform* pTargetTransform = pTargetCamera->Get_Transform();

	pTargetTransform->Set_State(CTransform::STATE_LOOK, vTotalLookPower);
	pTargetTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	pTargetTransform->Set_State(CTransform::STATE_UP, vUp);
}

void CCamera_Manager::Tick_MoveCamera(CCamera* pMoveCamera, CCamera* pNextCamera, CCamera* pPreCamera)
{
	if (m_fMoveTimeAcc == m_fTotalMoveTime)
	{
		m_pCurrentCamera = pNextCamera;
		m_pNextCamera = nullptr;
		m_pPreCamera = nullptr;
		m_fTotalMoveTime = 0.f;
		m_fMoveTimeAcc = 0.f;
		return;
	}

	m_fMoveTimeAcc += m_fTimeDelta;

	if (m_fTotalMoveTime < m_fMoveTimeAcc)
		m_fMoveTimeAcc = m_fTotalMoveTime;

	_vector vPreCamPos = pPreCamera->Get_Pos();
	_vector vNextCamPos = pNextCamera->Get_Pos();

	_vector vPreCamLook = pPreCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	_vector vNextCamLook = pNextCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

	_vector vFinalPos = CEasing_Utillity::sinfInOut(vPreCamPos, vNextCamPos, m_fMoveTimeAcc, m_fTotalMoveTime);
	_vector vFinalLook = XMVector3Normalize(CEasing_Utillity::sinfInOut(vPreCamLook, vNextCamLook, m_fMoveTimeAcc, m_fTotalMoveTime));

	m_pMoveCamera->Set_Pos(vFinalPos);
	m_pMoveCamera->Get_Transform()->LookTo(vFinalLook);

	m_pMoveCamera->Tick_Camera(m_fTimeDelta);
}

void CCamera_Manager::Tick_ActionCamera()
{
	if (m_PlayACs.front()->Is_Finish())
	{
		m_PlayACs.pop();

		if (0 < m_PlayACs.size())
		{
			Set_CurrentCamera(m_PlayACs.front());
		}
		else
		{
			Set_NextCamera(m_pOriginCamera, m_fReserveTotalMoveTime);
			m_fReserveTotalMoveTime = 0.f;
			m_pOriginCamera = nullptr;
		}
	}
}

void CCamera_Manager::Free()
{
	__super::Free();
	Safe_Release(m_pMoveCamera);

	for (auto& Pair : m_MCs)
	{
		Pair.second.clear();
	}
	m_MCs.clear();
	m_Cameras.clear();
}

