#include "stdafx.h"
#include "..\Public\Anim_Default.h"
#include "GameInstance.h"

CAnim_Default::CAnim_Default()
	: CLoading_Anim()
{
}

HRESULT CAnim_Default::Initialize(void * pArg)
{
	m_fMotionChangeTime = 0.5f;
	m_iMaxDotCount = 2;
	m_fPlayTime = 6.f;

	return S_OK;
}

void CAnim_Default::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fMotionChangeAcc += fTimeDelta;

	if (m_fMotionChangeAcc > m_fMotionChangeTime)
	{
		if (m_iDotCount > m_iMaxDotCount)
			m_iDotCount = 0;
		++m_iDotCount;

		m_fMotionChangeAcc = 0.f;
	}

	if (m_fTimeAcc > m_fPlayTime)
		m_isFinished = true;

}

HRESULT CAnim_Default::Render()
{
	//USEGAMEINSTANCE;
	//switch (m_iDotCount)
	//{
	//case 0:
	//	pGameInstance->Render_Font(TEXT("Bebas_Neue_Bold64"), TEXT("Loading"), _float2(m_fXPosition, m_fYPosition), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.7f);
	//	break;
	//case 1:
	//	pGameInstance->Render_Font(TEXT("Bebas_Neue_Bold64"), TEXT("Loading."), _float2(m_fXPosition, m_fYPosition), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.7f);
	//	break;
	//case 2:
	//	pGameInstance->Render_Font(TEXT("Bebas_Neue_Bold64"), TEXT("Loading.."), _float2(m_fXPosition, m_fYPosition), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.7f);
	//	break;
	//case 3:
	//	pGameInstance->Render_Font(TEXT("Bebas_Neue_Bold64"), TEXT("Loading..."), _float2(m_fXPosition, m_fYPosition), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.7f);
	//	break;
	//default:
	//	break;
	//}

	return S_OK;
}

CLoading_Anim * CAnim_Default::Create(void * pArg)
{
	CAnim_Default*		pInstance = new CAnim_Default();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CAnim_Default");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnim_Default::Free()
{
	__super::Free();
}
