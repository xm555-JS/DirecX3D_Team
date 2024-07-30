#include "stdafx.h"
#include "..\Public\State_Leviathan.h"
#include "GameInstance.h"
//#include "GameObject.h"
#include "State_Manager.h"

#include "Bullet_Normal.h"


CState_Leviathan::CState_Leviathan(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Boss(pManager, eState)
{
}

CState_Leviathan::CState_Leviathan(const CState_Leviathan& rhs)
	: CState_Boss(rhs)
{
	m_fCreateTime[SKILL_FIREBALL] = 0.3f;
	m_fCreateTime[SKILL_THUNDER] = 0.3f;
	m_fCreateTime[SKILL_ICESPEAR] = 0.3f;
}

void CState_Leviathan::Use_Skill(LEVIATHAN_SKILL eSkill, _float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fTimeAcc[eSkill] += fTimeDelta;
	if (m_fTimeAcc[eSkill] > m_fCreateTime[eSkill])
	{
		m_fTimeAcc[eSkill] -= m_fCreateTime[eSkill];

		if (SKILL_FIREBALL == eSkill)
		{
			CBullet_Normal::BULLETDESC BulletDesc;
			BulletDesc.pTarget = m_pOwner;
			XMStoreFloat4(&BulletDesc.TargetPos, m_pState_Handler->Get_Player()->Get_Pos());
			BulletDesc.TargetPos.x += fRandom(-15.f, 15.f);
			BulletDesc.TargetPos.z += fRandom(-15.f, 15.f);

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_LEVIATHAN_FIREBALL), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_LEVIATHAN_FIREBALL), &BulletDesc);
		}
		else if (SKILL_THUNDER == eSkill)
		{
			CBullet_Normal::BULLETDESC BulletDesc;
			BulletDesc.pTarget = m_pOwner;
			XMStoreFloat4(&BulletDesc.TargetPos, m_pState_Handler->Get_Player()->Get_Pos());
			BulletDesc.TargetPos.x += fRandom(-15.f, 15.f);
			BulletDesc.TargetPos.z += fRandom(-15.f, 15.f);
			BulletDesc.TargetPos.w = Random(0,1);

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_LEVIATHAN_THUNDER), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_LEVIATHAN_THUNDER), &BulletDesc);

		}
		else if (SKILL_ICESPEAR == eSkill)
		{
			CBullet_Normal::BULLETDESC BulletDesc;
			BulletDesc.pTarget = m_pOwner;
			XMStoreFloat4(&BulletDesc.TargetPos, m_pState_Handler->Get_Player()->Get_Pos());
			BulletDesc.TargetPos.x += fRandom(-20.f, 20.f);
			BulletDesc.TargetPos.z += fRandom(-20.f, 20.f);
			BulletDesc.TargetPos.w = Random(0, 1);

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_LEVIATHAN_ICESPEAR), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_LEVIATHAN_ICESPEAR), &BulletDesc);

		}
	}

}

void CState_Leviathan::Free()
{
	__super::Free();
}
