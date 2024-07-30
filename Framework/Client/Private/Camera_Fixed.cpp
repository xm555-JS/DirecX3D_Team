#include "stdafx.h"
#include "..\Public\Camera_Fixed.h"
#include "GameInstance.h"

CCamera_Fixed::CCamera_Fixed()
	: CCamera()
{
}

CCamera_Fixed::CCamera_Fixed(const CCamera_Fixed & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Fixed::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		__debugbreak();

	return S_OK;
}

HRESULT CCamera_Fixed::Initialize(void * pArg)
{
	/* 트랜스폼 컴포넌늩를 추가한다. */
	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	m_fMoveSpeed = 5.f;
	m_fRotateSpeed = XMConvertToRadians(90.f);

	return S_OK;
}

void CCamera_Fixed::Tick_Camera(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (FAILED(Bind_PipeLine()))
		return;
}

void CCamera_Fixed::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Fixed::Render()
{
	return S_OK;
}

CCamera_Fixed * CCamera_Fixed::Create()
{
	CCamera_Fixed*		pInstance = new CCamera_Fixed();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Fixed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Fixed::Clone(void * pArg)
{
	CCamera_Fixed*		pInstance = new CCamera_Fixed(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Fixed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Fixed::Free()
{
	__super::Free();
}
