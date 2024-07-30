#include "stdafx.h"
#include "..\Public\Loading_Anim.h"
#include "GameInstance.h"

#include "Anim_Default.h"
//#include "Anim_Emotion.h"
//#include "Anim_InfinityDot.h"

CLoading_Anim::CLoading_Anim()
{
}

HRESULT CLoading_Anim::Initialize(void * pArg)
{
	memcpy(&m_eCurAnimNum, pArg, sizeof(_uint));

	switch (m_eCurAnimNum)
	{
	case Client::CLoading_Anim::ANIM_DEFAULT:
		m_pCurAnim = CAnim_Default::Create(nullptr);
		break;
	//case Client::CLoading_Anim::ANIM_EMOTICON:
	//	m_pCurAnim = CAnim_Emotion::Create(nullptr);
	//	break;
	//case Client::CLoading_Anim::ANIM_INFINITYDOT:
	//	m_pCurAnim = CAnim_InfinityDot::Create(nullptr);
	//	break;
	case Client::CLoading_Anim::ANIM_END:
		break;
	default:
		break;
	}

	m_pCurAnim->Set_CenterPosition();

	return S_OK;
}

void CLoading_Anim::Tick(_float fTimeDelta)
{
	m_pCurAnim->Tick(fTimeDelta);

	if (m_pCurAnim->Is_Finish())
		m_isFinished = true;
}

HRESULT CLoading_Anim::Render()
{
	if(FAILED(m_pCurAnim->Render()))
		__debugbreak();

	return S_OK;
}

void CLoading_Anim::Set_CenterPosition()
{
	m_fScreenRate = 0.43f;
	m_fXPosition = g_iWinCX * m_fScreenRate;
	m_fYPosition = g_iWinCY * m_fScreenRate;
}

CLoading_Anim * CLoading_Anim::Create(void * pArg)
{
	CLoading_Anim*		pInstance = new CLoading_Anim();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CLoading_Anim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Anim::Free()
{
	__super::Free();

	Safe_Release(m_pCurAnim);
}
