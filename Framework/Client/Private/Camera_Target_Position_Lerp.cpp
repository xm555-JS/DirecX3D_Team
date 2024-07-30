#include "stdafx.h"
#include "..\Public\Camera_Target_Position_Lerp.h"
#include "GameInstance.h"
#include "Model.h"

#include "Navigation.h"

#include "VIBuffer_Terrain_Tess_Custom.h"
//#include "Imgui_Manager.h"

CCamera_Target_Position_Lerp::CCamera_Target_Position_Lerp()
	: CCamera_Target_Position()
{
}

CCamera_Target_Position_Lerp::CCamera_Target_Position_Lerp(const CCamera_Target_Position_Lerp& rhs)
	: CCamera_Target_Position(rhs)
{
}

void CCamera_Target_Position_Lerp::Set_TargetObject(CGameObject* pTarget)
{
	m_pTargetTransform = (CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM));
	m_pTarget = pTarget;
	m_pTargetModel = (CModel*)m_pTarget->Get_Component(MODEL_DYNAMIC);


	_vector		vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
	m_pNextTransfrom->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);

	_vector		vTargetLook = m_pTargetTransform->Get_State(CTransform::STATE_LOOK);
	m_pTransformCom->LookTo(vTargetLook);
	m_pNextTransfrom->LookTo(vTargetLook);

	assert(m_pTargetTransform);
}

// assert 1.f < fLerpRate
void CCamera_Target_Position_Lerp::Set_PosLerpRate(_float fLerpRate)
{
	assert(0.f < fLerpRate);
	assert(1.f >= fLerpRate);
	m_fPosLerpRate = fLerpRate;
}

void CCamera_Target_Position_Lerp::Set_LookLerpRate(_float fLerpRate)
{
	assert(0.f < fLerpRate);
	assert(1.f >= fLerpRate);
	m_fLookLerpRate = fLerpRate;
}

// fPower ´Â 0 ~ 1 »çÀÌ
void CCamera_Target_Position_Lerp::Set_NextOffset(_vector vNextOffset, _float fPower)
{
	assert(0.f < fPower);
	assert(1.f > fPower);

	XMStoreFloat3(&m_vNextOffset, vNextOffset);
	m_fOffsetMovePower = fPower;
	m_vOriOffset = m_vOffset;
}


HRESULT CCamera_Target_Position_Lerp::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TRANSFORM), ID_UINT(TRANSFORM, 2), (CComponent**)&m_pNextTransfrom));

	FAILEDCHECK(SetUp_Components());

	return S_OK;
}

void CCamera_Target_Position_Lerp::Tick_Camera(_float fTimeDelta)
{

	if (nullptr == m_pTargetTransform)
		return;

	USEGAMEINSTANCE;

	if (KEY_DOWN(VK_MENU))
		pGameInstance->Set_CursorFix(false);

	if (KEY_UP(VK_MENU))
		pGameInstance->Set_CursorFix(true);

	if (pGameInstance->Get_CursorFix())
	{

		_long		MouseMove = 0;
		if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_X))
			m_pNextTransfrom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta, m_fRotateSpeed * MouseMove * 0.1f);

		if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_Y))
			m_pNextTransfrom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta, m_fRotateSpeed * MouseMove * 0.1f);

		if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_WHEEL))
		{
			_vector vCurOffset = XMLoadFloat3(&m_vOffset);

			_float	fMouseMove = -MouseMove;
			fMouseMove = max(-100.f, fMouseMove);
			fMouseMove = min(100.f, fMouseMove);
			fMouseMove *= 0.1f;
			fMouseMove = (fMouseMove + 100.f) / 100.f;

			XMStoreFloat3(&m_vOffset, vCurOffset * fMouseMove);
		}


		//_vector vNextOffset = XMLoadFloat3(&m_vNextOffset);
		//_vector vOriOffset = XMLoadFloat3(&m_vOriOffset);
		//_vector vCurOffset = XMLoadFloat3(&m_vOffset);
		//
		//vCurOffset = vOriOffset + (vNextOffset - vCurOffset) * (1.f - m_fOffsetMovePower);
		//
		//XMStoreFloat3(&m_vOffset, vCurOffset);



		_vector		vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
		m_pNextTransfrom->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		m_pNextTransfrom->Set_Offset(XMLoadFloat3(&m_vOffset));
		_vector vCamPos = m_pNextTransfrom->Get_State(CTransform::STATE_TRANSLATION);

		if (20.f < XMVectorGetX(vCamPos) &&
			20.f < XMVectorGetZ(vCamPos) &&
			492.f > XMVectorGetX(vCamPos) &&
			492.f > XMVectorGetZ(vCamPos))
			vTargetPos = Calculate_TerrainPos(vTargetPos, vCamPos);
		else
			vTargetPos += (vCamPos - vTargetPos);



		_vector		vCurPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		_vector		vFinalPos = XMVectorLerp(vCurPos, vTargetPos, m_fPosLerpRate);

		_vector		vCurLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		_vector		vTargetLook = m_pNextTransfrom->Get_State(CTransform::STATE_LOOK);

		_vector		vFinalLook = XMVectorLerp(vCurLook, vTargetLook, m_fLookLerpRate);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vFinalPos);
		m_pTransformCom->LookTo(vFinalLook);
	}


	CCamera::Tick_Camera(fTimeDelta);
}

HRESULT CCamera_Target_Position_Lerp::SetUp_Components()
{
	///* For.Com_Navigation */
	//CNavigation::NAVIDESC		NaviDesc;
	//NaviDesc.m_iCurrentIndex = 0;
	//NaviDesc.pOwner = this;
	//FAILEDCHECK(CGameObject::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc));

	//m_pNaviCom->Find_CurrentIndex(m_pTransformCom);
	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_TERRAIN), ID_UINT(VIBUFFER), (CComponent**)&m_pTerrainBuffer));



	return S_OK;
}

_vector CCamera_Target_Position_Lerp::Calculate_TerrainPos(_vector vTargetPos, _vector vCamPos)
{
	//_float fOffsetScale = 2.f;
	_vector vDirCam = (vCamPos - vTargetPos);// *fOffsetScale;

	const _float3* pVerticesPos = m_pTerrainBuffer->Get_VerticesPos();

	for (size_t i = 10; i > 0; --i)
	{
		_vector vFinalPos = vTargetPos + vDirCam * (0.1f * i);

		_float fPosX = XMVectorGetX(vCamPos);
		_float fPosZ = XMVectorGetZ(vCamPos);

		_uint iPosX = (_uint)fPosX;
		_uint iPosZ = (_uint)fPosZ;

		_uint iVerticesIdx = iPosX + iPosZ * 512;

		_vector vPt[3];
		if (1.f > (fPosX - iPosX) + (fPosZ - iPosZ))
		{
			vPt[0] = XMLoadFloat3(&pVerticesPos[iVerticesIdx]);
			vPt[1] = XMLoadFloat3(&pVerticesPos[iVerticesIdx + 512]);
			vPt[2] = XMLoadFloat3(&pVerticesPos[iVerticesIdx + 1]);
		}
		else
		{
			vPt[0] = XMLoadFloat3(&pVerticesPos[iVerticesIdx + 1]);
			vPt[1] = XMLoadFloat3(&pVerticesPos[iVerticesIdx + 512]);
			vPt[2] = XMLoadFloat3(&pVerticesPos[iVerticesIdx + 513]);
		}

		_vector			Plane = XMPlaneFromPoints(vPt[0], vPt[1], vPt[2]);
		vTargetPos = XMVectorSetW(vTargetPos, 1.f);

		_float			fCheckHight = XMVectorGetX(XMVector4Dot(vFinalPos, Plane));

		if (0.f < fCheckHight)
			return vTargetPos + vDirCam * (0.1f * i);// / fOffsetScale;
	}

	return vTargetPos;
}

CCamera_Target_Position_Lerp* CCamera_Target_Position_Lerp::Create()
{
	CCamera_Target_Position_Lerp* pInstance = new CCamera_Target_Position_Lerp();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Target_Position_Lerp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Target_Position_Lerp::Clone(void* pArg)
{
	CCamera_Target_Position_Lerp* pInstance = new CCamera_Target_Position_Lerp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Target_Position_Lerp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Target_Position_Lerp::Free()
{
	__super::Free();
}

