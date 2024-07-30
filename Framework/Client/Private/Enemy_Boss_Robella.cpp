#include "stdafx.h"
#include "State_Handler.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Boss_Robella.h"

#include "Navigation.h"
#include "Quest_Manager.h"
#include "Portal.h"

CEnemy_Boss_Robella::CEnemy_Boss_Robella()
	: CEnemy_Boss()
{
}

CEnemy_Boss_Robella::CEnemy_Boss_Robella(const CEnemy_Boss& rhs)
	: CEnemy_Boss(rhs)
{
}

HRESULT CEnemy_Boss_Robella::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Boss_Robella::Initialize(void* pArg)
{
	m_fFrustumRange = 5.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	// 실제 위치
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(138.44f, 22.39f, 118.12f, 1.f));
	//// 테스트 위치
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(320.f, 11.f, 250.f, 1.f));

	if (FAILED(SetUp_Components()))
		__debugbreak();

	FAILEDCHECK(SetUp_State_Handler());
	FAILEDCHECK(Create_UI_Status("RobellaStatus"));


	m_pInfo = new UNITDESC();
	m_pCurInfo = new UNITDESC();

	*m_pInfo = UNITDESC(ELEMENTAL_DEFAULT, 40000.f, 10.f, 2.f);
	//memcpy(m_pInfo, &UNITDESC(ELEMENTAL_DEFAULT, 20000.f, 10.f, 2.f), sizeof(UNITDESC));
	memcpy(m_pCurInfo, m_pInfo, sizeof(UNITDESC));

	return S_OK;
}

void CEnemy_Boss_Robella::Tick(_float fTimeDelta)
{
#if defined(_TEST_JJS)
	// UI Test
	USEGAMEINSTANCE;
	if (KEY_DOWN('G'))
	{
		m_fPreHp = m_pCurInfo->fHP;
		m_pCurInfo->fHP -= 2000.f;
		pGameInstance->Add_Event(EVENT_HPCHANGE, this, &m_fPreHp);
	}
#endif

	__super::Tick(fTimeDelta);

	m_fAttackTime += fTimeDelta;

	m_pState_Handler->Tick(fTimeDelta);

	for (auto& iter : *m_pCol_AttackRange->Get_EnterColliders())
	{
		m_pTarget = iter->Get_Owner();

		MGR(CSound_Manager)->ChangeBGM(TEXT("Robella_Enter.wav"), 0.3f, 0.1f);
	}

	m_pCol_Attack->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_AttackRange->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Body->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Volume->Update(m_pTransformCom->Get_WorldMatrix());

	//퀘스트있을때 PlaceMark 생성
	if (QUEST_KILL_ROBELLA == CQuest_Manager::Get_Instance()->Get_CurQuset())
	{
		IMPL_ONCE(m_bCreate_PlaceMark, Create_UI_Place_Mark(6.f));
	}
	if (m_pCurInfo->fHP <= 0)
	{
		Pooling_UI_PlaceMark();

		MGR(CSound_Manager)->ChangeBGM(TEXT("Robella_Island.wav"), 0.3f, 0.1f);
	}
}

void CEnemy_Boss_Robella::LateTick(_float fTimeDelta)
{
	//if (!m_pTransformCom->Is_InAir())
	//	m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}

	for (auto& iter : *m_pCol_Volume->Get_StayColliders())
	{
		Push_EachOther(iter, m_pCol_Volume, m_pTransformCom);
	}

	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Attack);
	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_CHASERANGE), m_pCol_AttackRange);
	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_HITRANGE), m_pCol_Body);
	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_BOSS_VOLUME), m_pCol_Volume);

}

HRESULT CEnemy_Boss_Robella::Render()
{
	__super::Render();

#ifdef _DEBUG
	m_pCol_Attack->Render();
	m_pCol_AttackRange->Render();
	m_pCol_Body->Render();
	m_pCol_Volume->Render();
#endif // DEBUG

	_float4 vColor = { 1.f, 1.f, 1.f, 1.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &vColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	_bool bUseRimLight = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bRimLight", &bUseRimLight, sizeof(_bool)));
	return S_OK;
}

HRESULT CEnemy_Boss_Robella::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_ROBELLA), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pDissolveTextureCom));
	m_iDissolveTextureNoise = 252;
	m_iDissolveTextureColor = 32;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 1.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Attack, &ColliderDesc));
	m_pCol_Attack->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(40.f, 40.f, 40.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACKRANGE), (CComponent**)&m_pCol_AttackRange, &ColliderDesc));
	m_pCol_AttackRange->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(8.f, 8.f, 8.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_HITRANGE), (CComponent**)&m_pCol_Body, &ColliderDesc));
	m_pCol_Body->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_BOSS_VOLUME), (CComponent**)&m_pCol_Volume, &ColliderDesc));
	m_pCol_Volume->Set_Owner(this);

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	//if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
	//	return E_FAIL;
	//m_pNaviCom->Find_CurrentIndex(m_pTransformCom);

	return S_OK;
}

HRESULT CEnemy_Boss_Robella::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &m_vColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(1.f, 0.f, 0.f, 0.95f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 0.41f, 0.16f, 0.47f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	_bool	bUseToon = false;
	//_bool	bUseOutline = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bToon", &bUseToon, sizeof(_bool)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bOutline", &bUseOutline, sizeof(_bool)));

	return S_OK;
}

HRESULT CEnemy_Boss_Robella::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	/*
		STATE_BOSS_ROBELLA_READY,			// 33
		STATE_BOSS_ROBELLA_CHANGE_PHASE,	// 15
		STATE_BOSS_ROBELLA_APEAR,			// 0

		// 1페이즈 IDLE
		STATE_BOSS_ROBELLA_IDLE,			// 33
		// 2페이즈 IDLE
		STATE_BOSS_ROBELLA_IDLE2,			// 18

		// 1페이즈 공격
		STATE_BOSS_ROBELLA_ATTACK_01,		// 1 불똥
		STATE_BOSS_ROBELLA_ATTACK_02_BEGIN,	// 2
		STATE_BOSS_ROBELLA_ATTACK_02_LOOP,	// 4 꿀렁꿀렁
		STATE_BOSS_ROBELLA_ATTACK_02_END,	// 3
		STATE_BOSS_ROBELLA_ATTACK_03,		// 5 빙글빙글

		//  2페이즈 공격
		STATE_BOSS_ROBELLA_ATTACK_04,		// 6 회전후 등장
		STATE_BOSS_ROBELLA_ATTACK_05,		// 8 채찎
		STATE_BOSS_ROBELLA_ATTACK_06,		// 10 애니메이션 두배속도해야함 채찍2
		STATE_BOSS_ROBELLA_ATTACK_07,		// 11 삿대질
		STATE_BOSS_ROBELLA_ATTACK_08,		// 12 혼자화이팅
		STATE_BOSS_ROBELLA_ATTACK_09_BEGIN,	// 13 어퍼컷함
		STATE_BOSS_ROBELLA_ATTACK_09_END,	// 14 어퍼컷 마무리

		STATE_BOSS_ROBELLA_ATTACK_ULT_READY,// 17 혼자 화이팅2
		STATE_BOSS_ROBELLA_ATTACK_ULT,		// 19 꽃잎으로 숨고 꿀럭꿀럭하다가 등장해서 회전

		STATE_BOSS_ROBELLA_DEAD,			// 29
		STATE_BOSS_ROBELLA_DEAD_LOOP,		// 30

		STATE_BOSS_ROBELLA_1PHASE_DOWN,		// 40
		STATE_BOSS_ROBELLA_1PHASE_DOWN_LOOP,// 41
		STATE_BOSS_ROBELLA_1PHASE_DOWN_Up,	// 42

		STATE_BOSS_ROBELLA_2PHASE_DOWN,		// 36
		STATE_BOSS_ROBELLA_2PHASE_DOWN_LOOP,// 38
		STATE_BOSS_ROBELLA_2PHASE_DOWN_Up,	// 37

		STATE_BOSS_ROBELLA_END,
	*/

	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_READY, 33);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_CHANGE_PHASE, 15);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_APEAR, 0);

	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_CAM_ACTION, 6);

	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_IDLE, 33);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_IDLE2, 18);

	//1Phase
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_01, 1);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_02_BEGIN, 2);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_02_LOOP, 4);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_02_END, 3);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_03, 5);

	//2Phase
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_04, 6);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_05, 8);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_06, 10);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_07, 11);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_08, 12);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_09_BEGIN, 13);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_09_END, 14);

	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_ULT_READY, 17);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_ATTACK_ULT, 19);

	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_DEAD, 29);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_DEAD_LOOP, 30);

	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_1PHASE_DOWN, 40);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_1PHASE_DOWN_LOOP, 41);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_1PHASE_DOWN_UP, 42);

	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_2PHASE_DOWN, 36);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_2PHASE_DOWN_LOOP, 37);
	m_pState_Handler->Set_Animation(STATE_BOSS_ROBELLA_2PHASE_DOWN_UP, 38);

	m_pState_Handler->SetUp_StartState(STATE_BOSS_ROBELLA_READY);

	return S_OK;
}


CEnemy_Boss_Robella* CEnemy_Boss_Robella::Create()
{
	CEnemy_Boss_Robella* pInstance = new CEnemy_Boss_Robella();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Boss_Robella");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Boss_Robella::Clone(void* pArg)
{
	CEnemy_Boss_Robella* pInstance = new CEnemy_Boss_Robella(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Boss_Robella");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Boss_Robella::Free()
{
	__super::Free();
}
