#include "stdafx.h"
#include "..\Public\Camera_Target_Position.h"
#include "GameInstance.h"
#include "Model.h"
#include "HIerarchyNode.h"
#include "UI_Cursor.h"
#include "Player.h"

CCamera_Target_Position::CCamera_Target_Position()
	: CCamera()
{
}

CCamera_Target_Position::CCamera_Target_Position(const CCamera_Target_Position& rhs)
	: CCamera(rhs)
{
}

void CCamera_Target_Position::Set_TargetObject(CGameObject* pTarget)
{
	m_pTargetTransform = (CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM));
	m_pTarget = pTarget;
	m_pTargetModel = (CModel*)m_pTarget->Get_Component(MODEL_DYNAMIC);
	//m_fMaxOffset = 5.f;
	//m_fMinOffset = 0.5f;
	


	assert(m_pTargetTransform);
}

HRESULT CCamera_Target_Position::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		__debugbreak();

	return S_OK;
}

HRESULT CCamera_Target_Position::Initialize(void* pArg)
{
	//CTransform::TRANSFORMDESC		TransformDesc;
	//if (FAILED(CGameObject::Initialize(&TransformDesc)))
	//	__debugbreak();

	/* 트랜스폼 컴포넌늩를 추가한다. */
	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	m_fRotateSpeed = 1.5f;

	USEGAMEINSTANCE;
	pGameInstance->Add_CallbackList(this, EVENT_IMGUIWINDOWSTATE);

	return S_OK;
}

void CCamera_Target_Position::Tick_Camera(_float fTimeDelta)
{
	if (nullptr == m_pTargetTransform)
		return;

	USEGAMEINSTANCE;
	//// 마우스 고정하면 UI_Cursor Render안되도록
	//CUI_Cursor* pCursor = dynamic_cast<CUI_Cursor*>(pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI)));
	//if (pCursor)
	//{
	//	//if (m_bCursorFixState)
	//	//{
	//	//	pCursor->Set_Render(false);
	//	//}
	//	if
	//	{
	//		pCursor->Set_Render(true);
	//	}
	//	if (KEY_PRESSING(VK_MENU))
	//	{
	//		pCursor->Set_Render(true);
	//	}
	//}
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	if (KEY_DOWN(VK_MENU))
	{
		pGameInstance->Set_CursorFix(false);

	}

	if (KEY_UP(VK_MENU))
	{
		pGameInstance->Set_CursorFix(true);

	}


	if (pGameInstance->Get_CursorFix())
	{
		//_vector		vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
		//_matrix		CombinedMatrix = m_pTargetModel->Find_HierarcyNode("Root_Bone")->Get_CombinedMatrix() * m_pTargetModel->Find_HierarcyNode("Root_Bone")->Get_TransformMatrix() * m_pTargetTransform->Get_WorldMatrix();
		//_vector		vTargetPos = CombinedMatrix.r[3];
		_long		MouseMove = 0;
		if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_X))
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), /*fTimeDelta*/0.02f, m_fRotateSpeed * MouseMove * 0.1f);

		if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_Y))
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), /*fTimeDelta*/0.02f, m_fRotateSpeed * MouseMove * 0.1f);

		if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_WHEEL))
		{
			_vector vCurOffset = XMLoadFloat3(&m_vOffset);
			
			_float	fMouseMove = -MouseMove;
			fMouseMove = max(-100.f, fMouseMove);
			fMouseMove = min(100.f, fMouseMove);
			fMouseMove *= 0.1f;
			fMouseMove = (fMouseMove + 100.f) / 100.f;

			//_vector vDirOffset = XMVector3Normalize(vCurOffset);
			//_float	fDirOffset = XMVectorGetX(XMVector3Length(vCurOffset));

			//fDirOffset = max(0.5f, fDirOffset - MouseMove * 0.01f);
			//vDirOffset *= fDirOffset;

			//vCurOffset = max(0.5, vCurOffset * MouseMove);
			//vCurOffset = min(vCurOffset * MouseMove);

			XMStoreFloat3(&m_vOffset, vCurOffset * fMouseMove);
		}


		//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		//m_pTransformCom->Set_Offset(XMLoadFloat3(&m_vOffset));






	}

	_vector		vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
	m_pTransformCom->Set_Offset(XMLoadFloat3(&m_vOffset));
	

	//if (FAILED(Bind_PipeLine()))
	//	return;

	__super::Tick(fTimeDelta);
}


void CCamera_Target_Position::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//_matrix Worldmatrix;
	//memcpy(&Worldmatrix, &(m_pTargetTransform->Get_WorldMatrix()), sizeof(_matrix));
	//m_pTransformCom->Set_WorldMatrix(Worldmatrix);
	//m_pTransformCom->Set_Offset(XMLoadFloat3(&m_vOffset));

	//if (FAILED(Bind_PipeLine()))
	//	return;
}

HRESULT CCamera_Target_Position::Render()
{
	return S_OK;
}

void CCamera_Target_Position::On_EventMessege(EVENTDESC tEvent)
{
	//if (EVENT_IMGUIWINDOWSTATE == tEvent.eEventType)
	//{
	//	m_bCursorFixState = !(*((_bool*)tEvent.lParam));
	//}
}


CCamera_Target_Position* CCamera_Target_Position::Create()
{
	CCamera_Target_Position* pInstance = new CCamera_Target_Position();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Target_Position");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Target_Position::Clone(void* pArg)
{
	CCamera_Target_Position* pInstance = new CCamera_Target_Position(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Target_Position");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Target_Position::Free()
{
	GAMEINSTANCE->Remove_CallbackList(this, EVENT_IMGUIWINDOWSTATE);
	__super::Free();
}

