#include "stdafx.h"
#include "..\Public\Camera_Action.h"
#include "GameInstance.h"

#include "Model.h"
#include "HIerarchyNode.h"

#include "Easing_Utillity.h"

CCamera_Action::CCamera_Action()
	: CCamera()
{
}

CCamera_Action::CCamera_Action(const CCamera_Action& rhs)
	: CCamera(rhs)
{
}

void CCamera_Action::Set_TargetObejct(CGameObject* pTarget)
{
	assert(pTarget);
	m_pTarget = pTarget;
	m_pTargetTransform = (CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM));
	m_pTargetBoneName = "";
}

void CCamera_Action::Set_TargetObejct(CGameObject* pTarget, string strBoneName)
{
	assert(pTarget);
	m_pTarget = pTarget;
	m_pTargetTransform = (CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM));
	m_pTargetBoneName = strBoneName;
}

void CCamera_Action::Set_StartDesc(_float3 _vTargetPos, _float3 _vTargetLook)
{
	m_pActionDescs[0] = ACTIONCAMDESC(_vTargetPos, _vTargetLook, CEasing_Utillity::TYPE_Linear, 0.f);
	m_pActionDescs[1] = ACTIONCAMDESC(_vTargetPos, _vTargetLook, CEasing_Utillity::TYPE_Linear, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat3(&_vTargetPos));
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&_vTargetLook), 1.f));
}

void CCamera_Action::Set_EndDesc(_float3 _vTargetPos, _float3 _vTargetLook, _uint _iEasing, _float _fPlayTime)
{
	m_pActionDescs[m_iNumActionDesc - 1] = ACTIONCAMDESC(_vTargetPos, _vTargetLook, _iEasing, _fPlayTime);
	m_pActionDescs[m_iNumActionDesc - 2] = ACTIONCAMDESC(_vTargetPos, _vTargetLook, _iEasing, _fPlayTime);
}

void CCamera_Action::Reserve_NumActionDesc(_uint iNumActionDesc)
{
	// 2 는 앞뒤로 하나씩 채워넣은것
	m_pActionDescs = new ACTIONCAMDESC[iNumActionDesc + 2];
	m_iNumActionDesc = iNumActionDesc + 2;
}

void CCamera_Action::Start_ActionCamera()
{
	m_iCurCamIdx = 1;
	m_fTimeAcc = 0.f;
	m_bIsFinish = false;
}

void CCamera_Action::Reset_CamInfo()
{
	m_iCurCamIdx = 1;
	m_fTimeAcc = 0.f;
	m_bIsFinish = false;
}

HRESULT CCamera_Action::Initialize_Prototype()
{
	FAILEDCHECK(__super::Initialize_Prototype());

	m_iCurCamIdx = 1;

	return S_OK;
}

HRESULT CCamera_Action::Initialize(void* pArg)
{
	/* 트랜스폼 컴포넌늩를 추가한다. */
	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	FAILEDCHECK(__super::Initialize(pArg));

	m_iCurCamIdx = 1;

	return S_OK;
}

void CCamera_Action::Tick_Camera(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (false == m_bIsFinish)
		Play_CurActionCam();

	CCamera::Tick_Camera(fTimeDelta);
}

void CCamera_Action::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Action::Render()
{
	return S_OK;
}

void CCamera_Action::Play_CurActionCam()
{
	if (m_fTimeAcc > m_pActionDescs[m_iCurCamIdx + 1].fPlayTime)
	{
		++m_iCurCamIdx;
		if (m_iCurCamIdx > m_iNumActionDesc - 3)
		{
			m_iCurCamIdx = 1;
			m_fTimeAcc = 0.f;
			m_bIsFinish = true;
			return;
		}

		m_fTimeAcc -= m_pActionDescs[m_iCurCamIdx].fPlayTime;
	}

	_float fMoveSpeed = XMVectorGetX(CEasing_Utillity::Easing(
		m_pActionDescs[m_iCurCamIdx + 1].iEasing,
		XMVectorSet(0.f, 0.f, 0.f, 0.f),
		XMVectorSet(1.f, 0.f, 0.f, 0.f),
		m_fTimeAcc,
		m_pActionDescs[m_iCurCamIdx + 1].fPlayTime));


	_vector vStartPos, vEndPos;
	_vector vPrePos, vNextPos;

	vStartPos = XMLoadFloat3(&m_pActionDescs[m_iCurCamIdx].vTargetPos);
	vEndPos = XMLoadFloat3(&m_pActionDescs[m_iCurCamIdx + 1].vTargetPos);

	vPrePos = XMLoadFloat3(&m_pActionDescs[m_iCurCamIdx - 1].vTargetPos);
	vNextPos = XMLoadFloat3(&m_pActionDescs[m_iCurCamIdx + 2].vTargetPos);

	_vector vFinalPos = XMVectorCatmullRom(vPrePos, vStartPos, vEndPos, vNextPos, fMoveSpeed);

	if (m_pTargetTransform)
	{
		_vector		vPos;
		if (m_pTargetBoneName.compare("")) // 뼈이름이 있다면,
		{
			//vPos = m_pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
			CModel* pModelCom = static_cast<CModel*>(m_pTarget->Get_Component(ID_UINT(MODEL_DYNAMIC)));
			_matrix WorldMatrix = (pModelCom->Find_HierarcyNode(m_pTargetBoneName.c_str())->Get_CombinedMatrix() * pModelCom->Find_HierarcyNode(m_pTargetBoneName.c_str())->Get_TransformMatrix() * XMLoadFloat4x4(m_pTarget->Get_WorldMatrix()));

			vPos = WorldMatrix.r[3];
		}
		else
		{
			vPos = m_pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
		}
		_vector		vRight = XMVector3Normalize(m_pTargetTransform->Get_State(CTransform::STATE_RIGHT));
		//_vector		vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP));
		_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector		vLook = XMVector3Normalize(m_pTargetTransform->Get_State(CTransform::STATE_LOOK));

		vPos += XMVector3Normalize(vRight) * XMVectorGetX(vFinalPos);
		vPos += XMVector3Normalize(vUp) * XMVectorGetY(vFinalPos);
		vPos += XMVector3Normalize(vLook) * XMVectorGetZ(vFinalPos);
		vPos = XMVectorSetW(vPos, 1.f);
		XMStoreFloat3(&m_vOffset, vPos);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vFinalPos);
	}



	_vector vStartLook, vEndLook;
	_vector vPreLook, vNextLook;

	vStartLook = XMLoadFloat3(&m_pActionDescs[m_iCurCamIdx].vTargetLook);
	vEndLook = XMLoadFloat3(&m_pActionDescs[m_iCurCamIdx + 1].vTargetLook);

	vPreLook = XMLoadFloat3(&m_pActionDescs[m_iCurCamIdx - 1].vTargetLook);
	vNextLook = XMLoadFloat3(&m_pActionDescs[m_iCurCamIdx + 2].vTargetLook);

	_vector vFinalLook = XMVectorCatmullRom(vPreLook, vStartLook, vEndLook, vNextLook, fMoveSpeed);
	XMStoreFloat4(&m_CameraDesc.vAt, vFinalLook);


	if (m_pTargetTransform)
	{
		_vector		vPos;
		if (m_pTargetBoneName.compare("")) // 뼈이름이 있다면,
		{
			//vPos = m_pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
			CModel* pModelCom = static_cast<CModel*>(m_pTarget->Get_Component(ID_UINT(MODEL_DYNAMIC)));
			vPos = (pModelCom->Find_HierarcyNode(m_pTargetBoneName.c_str())->Get_CombinedMatrix() * pModelCom->Find_HierarcyNode(m_pTargetBoneName.c_str())->Get_TransformMatrix() * XMLoadFloat4x4(m_pTarget->Get_WorldMatrix())).r[3];
		}
		else
		{
			vPos = m_pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
		}
		_vector		vRight = XMVector3Normalize(m_pTargetTransform->Get_State(CTransform::STATE_RIGHT));
		//_vector		vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP));
		_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector		vLook = XMVector3Normalize(m_pTargetTransform->Get_State(CTransform::STATE_LOOK));

		vPos += XMVector3Normalize(vRight) * XMVectorGetX(vFinalLook);
		vPos += XMVector3Normalize(vUp) * XMVectorGetY(vFinalLook);
		vPos += XMVector3Normalize(vLook) * XMVectorGetZ(vFinalLook);
		vPos = XMVectorSetW(vPos, 1.f);

		m_pTransformCom->LookAt(XMVectorSetW(vPos, 1.f));
	}
	else
	{
		m_pTransformCom->LookAt(XMVectorSetW(vFinalLook, 1.f));
	}

}

CCamera_Action* CCamera_Action::Create()
{
	CCamera_Action* pInstance = new CCamera_Action();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Action");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Action::Clone(void* pArg)
{
	CCamera_Action* pInstance = new CCamera_Action(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Action");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Action::Free()
{
	Safe_Delete_Array(m_pActionDescs);

	__super::Free();
}
