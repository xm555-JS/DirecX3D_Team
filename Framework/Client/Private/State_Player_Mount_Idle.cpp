#include "stdafx.h"
#include "..\Public\State_Player_Mount_Idle.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

//#include "Imgui_Manager.h"

CState_Player_Mount_Idle::CState_Player_Mount_Idle(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Mount_Idle::CState_Player_Mount_Idle(const CState_Player_Mount_Idle& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Mount_Idle::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_BASE_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_GLIDE_UP);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);
	m_ForceStates.push_back(STATE_PLAYER_HAND_IDLE);

	m_ReserveStates.push_back(STATE_PLAYER_MOUNT_DRIVE);						//153

	return S_OK;
}

HRESULT CState_Player_Mount_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Mount_Idle::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	m_bEndCurState = true;
	m_pPlayer->Get_Transform()->Reset_FrontAccTime();
	m_pPlayer->Set_Target(nullptr);
}

void CState_Player_Mount_Idle::Tick(_float fTimeDelta)
{
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeIdle.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f));
	IMPL_ALARM(m_fSound[0], m_eSoundChannel[2] = MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_BikeIdle.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.25f), 0.5f, fTimeDelta);

	USEGAMEINSTANCE;

	//if (KEY_DOWN('F'))
	//{
	//	list<CGameObject*>* pGameObjects = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_MESH));
	//	if (nullptr != pGameObjects)
	//	{
	//		for (auto& pGameObject : *pGameObjects)
	//		{
	//			pGameInstance->Add_PoolingObject(pGameObject);
	//		}
	//	}
	//}

	_vector vCurLook = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->UpNaviLook(vCurLook, pPlayerNavi);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Mount_Idle::Exit(void* pArg)
{
	MGR(CSound_Manager)->StopSound(m_eSoundChannel[2]);
	m_fKeyPressTimeAcc = 0.f;
	__super::Exit();
}

_bool CState_Player_Mount_Idle::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	//키 조정해야함
	if (KEY_NONE('W') &&
		KEY_NONE('A') &&
		KEY_NONE('S') &&
		KEY_NONE('D'))
	{
		return true;
	}

	return false;
}


CState_Player_Mount_Idle* CState_Player_Mount_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Mount_Idle* pInstance = new CState_Player_Mount_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Mount_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Mount_Idle::Clone(void* pArg)
{
	CState_Player_Mount_Idle* pInstance = new CState_Player_Mount_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Mount_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Mount_Idle::Free()
{
}
