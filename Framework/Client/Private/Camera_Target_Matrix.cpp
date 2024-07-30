#include "stdafx.h"
#include "..\Public\Camera_Target_Matrix.h"
#include "GameInstance.h"

CCamera_Target_Matrix::CCamera_Target_Matrix()
	: CCamera()
{
}

CCamera_Target_Matrix::CCamera_Target_Matrix(const CCamera_Target_Matrix & rhs)
	: CCamera(rhs)
{
}

void CCamera_Target_Matrix::Set_TargetObject(CGameObject * pTarget, CTransform * pTargetTransform)
{
	if (nullptr == pTargetTransform)
	{
		m_pTargetTransform = (CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM));
		m_pTarget = pTarget;
	}

	assert(m_pTargetTransform);
}

HRESULT CCamera_Target_Matrix::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		__debugbreak();

	return S_OK;
}

HRESULT CCamera_Target_Matrix::Initialize(void * pArg)
{
	//CTransform::TRANSFORMDESC		TransformDesc;
	//if (FAILED(CGameObject::Initialize(&TransformDesc)))
	//	__debugbreak();

	/* 트랜스폼 컴포넌늩를 추가한다. */
	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	return S_OK;
}

void CCamera_Target_Matrix::Tick_Camera(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (nullptr == m_pTargetTransform)
		return;

	
	//_matrix Worldmatrix;
	//memcpy(&Worldmatrix, &(m_pTargetTransform->Get_WorldMatrix()), sizeof(_matrix));
	m_pTransformCom->Set_WorldMatrix(m_pTargetTransform->Get_WorldMatrix());
	//m_pTransformCom->Set_WorldMatrix(Worldmatrix);
	m_pTransformCom->Set_Offset(XMLoadFloat3(&m_vOffset));

	if (FAILED(Bind_PipeLine()))
		return;
}


void CCamera_Target_Matrix::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//_matrix Worldmatrix;
	//memcpy(&Worldmatrix, &(m_pTargetTransform->Get_WorldMatrix()), sizeof(_matrix));
	//m_pTransformCom->Set_WorldMatrix(Worldmatrix);
	//m_pTransformCom->Set_Offset(XMLoadFloat3(&m_vOffset));

	//if (FAILED(Bind_PipeLine()))
	//	return;
}

HRESULT CCamera_Target_Matrix::Render()
{
	return S_OK;
}


CCamera_Target_Matrix * CCamera_Target_Matrix::Create()
{
	CCamera_Target_Matrix*		pInstance = new CCamera_Target_Matrix();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Target_Matrix");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Target_Matrix::Clone(void * pArg)
{
	CCamera_Target_Matrix*		pInstance = new CCamera_Target_Matrix(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Target_Matrix");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Target_Matrix::Free()
{
	__super::Free();
}

