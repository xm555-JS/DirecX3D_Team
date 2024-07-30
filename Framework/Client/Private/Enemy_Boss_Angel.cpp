#include "stdafx.h"
#include "State_Handler.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Boss_Angel.h"

CEnemy_Boss_Angel::CEnemy_Boss_Angel()
	: CEnemy_Boss()
{
}

CEnemy_Boss_Angel::CEnemy_Boss_Angel(const CEnemy_Boss& rhs)
	: CEnemy_Boss(rhs)
{
}

HRESULT CEnemy_Boss_Angel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Boss_Angel::Initialize(void* pArg)
{
	m_fFrustumRange = 5.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	FAILEDCHECK(SetUp_State_Handler());

	m_pInfo = new UNITDESC();
	m_pCurInfo = new UNITDESC();

	*m_pInfo = UNITDESC(ELEMENTAL_DEFAULT, 100.f, 10.f, 2.f);
	//memcpy(m_pInfo, &UNITDESC(ELEMENTAL_DEFAULT, 100.f, 10.f, 2.f), sizeof(UNITDESC));
	memcpy(m_pCurInfo, m_pInfo, sizeof(UNITDESC));

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(3.f, -8.f, 3.f, 1.f));
	m_pTransformCom->Set_Scaled(_float3(1.5f, 1.5f, 1.5f));

	return S_OK;
}

void CEnemy_Boss_Angel::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	m_pState_Handler->Tick(fTimeDelta);
}

void CEnemy_Boss_Angel::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CEnemy_Boss_Angel::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_ANGEL), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();
	return S_OK;
}

HRESULT CEnemy_Boss_Angel::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	return S_OK;
}

HRESULT CEnemy_Boss_Angel::SetUp_State_Handler()
{
	/*
		// Boss_Angel
		STATE_BOSS_ANGEL_READY,				//22
		STATE_BOSS_ANGEL_STAND,				//10
		STATE_BOSS_ANGEL_IDLE,				//13
		STATE_BOSS_ANGEL_BACK,				//16
		STATE_BOSS_ANGEL_TURNL,				//14
		STATE_BOSS_ANGEL_TURNR,				//15
		STATE_BOSS_ANGEL_RUN,				//17
		STATE_BOSS_ANGEL_RUNL,				//18
		STATE_BOSS_ANGEL_RUNR,				//19

		STATE_BOSS_ANGEL_SWING_ATTACK,		//5

		STATE_BOSS_ANGEL_LASER_ATTACK,		//7
		STATE_BOSS_ANGEL_LASER_ATTACKA,		//0 -> 1, 2
		STATE_BOSS_ANGEL_LASER_ATTACKB,		//1
		STATE_BOSS_ANGEL_LASER_ATTACKC,		//2

		STATE_BOSS_ANGEL_SKILL_ATTACK,		//3
		STATE_BOSS_ANGEL_SKILL_ATTACK,		//4

		STATE_BOSS_ANGEL_ULT_ATTACK,		//8
		STATE_BOSS_ANGEL_ULT_ATTACK_END,	//9

		STATE_BOSS_ANGEL_SHIELD,			//12
		STATE_BOSS_ANGEL_END,				//11
		*/
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_READY, 22);		//등장전 
	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_STAND, 10);		//두둥등장

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_IDLE, 13);			//가만

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_BACK, 16);			//앞으로
	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_TURNL, 14);		//왼쪽턴->16
	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_TURNR, 15);		//오른쪽턴->16

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_RUN, 17);			//대쉬
	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_RUNL, 18);			//앞을보며 옆으로 이동
	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_RUNR, 19);			//오른쪽

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_SWING_ATTACK, 5);	//물공

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_LASER_ATTACK, 7);	//평타 한방

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_LASER_ATTACKA, 0);		//Ready
	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_LASER_ATTACKB, 1);		//왼쪽으로
	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_LASER_ATTACKC, 2);		//오른쪽으로

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_SKILL_ATTACKA, 3);		//준비
	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_SKILL_ATTACKB, 4);		//발싸

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_ULT_ATTACK, 8);		//준비-작동
	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_ULT_ATTACK_END, 9);	//끝

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_SHIELD, 12);			//쉴드모션
	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_RESET, 11);			//매 이동시 리셋동작

	m_pState_Handler->Set_Animation(STATE_BOSS_ANGEL_DEAD, 21);				//주금

	m_pState_Handler->SetUp_StartState(STATE_BOSS_ANGEL_READY);

	return S_OK;
}


CEnemy_Boss_Angel* CEnemy_Boss_Angel::Create()
{
	CEnemy_Boss_Angel* pInstance = new CEnemy_Boss_Angel();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Boss_Angel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Boss_Angel::Clone(void* pArg)
{
	CEnemy_Boss_Angel* pInstance = new CEnemy_Boss_Angel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Boss_Angel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Boss_Angel::Free()
{
	__super::Free();
}
