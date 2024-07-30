#include "stdafx.h"
#include "State_Handler.h"
#include "Navigation.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Boss_Weaver.h"

#include "HIerarchyNode.h"
#include "Quest_Manager.h"

#include "Imgui_Manager.h"
#include "Event_Manager.h"

CEnemy_Boss_Weaver::CEnemy_Boss_Weaver()
	: CEnemy_Boss()
{
}

CEnemy_Boss_Weaver::CEnemy_Boss_Weaver(const CEnemy_Boss& rhs)
	: CEnemy_Boss(rhs)
{
}

HRESULT CEnemy_Boss_Weaver::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Boss_Weaver::Initialize(void * pArg)
{
	m_fFrustumRange = 4.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	// 테스트 위치
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(320.f, 0.f, 250.f, 1.f));

	// 실제 위치  434.13 289.51
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(434.13f, 152.f, 289.51f, 1.f));

	if (FAILED(SetUp_Components()))
		__debugbreak();

	FAILEDCHECK(SetUp_State_Handler());

	m_pInfo = new UNITDESC();
	m_pCurInfo = new UNITDESC();

	*m_pInfo = UNITDESC(ELEMENTAL_DEFAULT, 40000.f, 10.f, 2.f);
	//memcpy(m_pInfo, &UNITDESC(ELEMENTAL_DEFAULT, 20000.f, 10.f, 2.f), sizeof(UNITDESC));
	memcpy(m_pCurInfo, m_pInfo, sizeof(UNITDESC));

	FAILEDCHECK(Create_UI_Status("WeaverStatus"));

	m_pTarget = nullptr;
	m_bRenderKnife = true;
	RESET_ONCE(m_bEffect0);
	return S_OK;
}

void CEnemy_Boss_Weaver::Tick(_float fTimeDelta)
{
#if defined(_TEST_JJS)
	// UI Test
	USEGAMEINSTANCE;
	if (KEY_DOWN('J'))
	{
		m_fPreHp = m_pCurInfo->fHP;
		m_pCurInfo->fHP -= 2000.f;
		pGameInstance->Add_Event(EVENT_HPCHANGE, this, &m_fPreHp);
	}
#endif
	__super::Tick(fTimeDelta);
	//IMPL_ONCE(m_bEffect0,
	//	EFFECT_EXCUTE("Boss_Weaver_Aura", XMMatrixTranslation(0.f, 0.8f, -0.1f), 1.f, this, CEffect_Manager::FOLLOW_POS_RUL););

	m_pState_Handler->Tick(fTimeDelta);

	//타겟 설정
	for (auto& iter : *m_pCol_ChaseRange->Get_EnterColliders())
	{
		m_pTarget = iter->Get_Owner();

		MGR(CSound_Manager)->ChangeBGM(TEXT("Weaver_Enter.mp3"), 0.3f, 0.1f);
	}

	SetUp_ColMatrix();
	m_pCol_Attack[ATTACK_06]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Body->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_AttackRange->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_ChaseRange->Update(m_pTransformCom->Get_WorldMatrix());

	m_pCol_Laser->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Volume->Update(m_pTransformCom->Get_WorldMatrix());

	if (m_pCurInfo->fHP <= 0)
	{
		MGR(CSound_Manager)->ChangeBGM(TEXT("City_Island.mp3"), 0.3f, 0.1f);
	}
}

void CEnemy_Boss_Weaver::LateTick(_float fTimeDelta)
{
	if (!m_pTransformCom->Is_InAir())
		m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);

	USEGAMEINSTANCE;
	for(_uint i = 0; i < ATTACK_END; ++i)
		pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Attack[i]);

	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Laser);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACKRANGE), m_pCol_AttackRange);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_CHASERANGE), m_pCol_ChaseRange);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_HITRANGE), m_pCol_Body);
	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_BOSS_VOLUME), m_pCol_Volume);

	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}

	for (auto& iter : *m_pCol_Volume->Get_StayColliders())
	{
		Push_EachOther(iter, m_pCol_Volume, m_pTransformCom);
	}
}

HRESULT CEnemy_Boss_Weaver::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		if (2 == i && !m_bRenderKnife)
		{
			continue;
		}

		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));
		//FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));

		m_pModelCom->Render(i, m_pShaderCom, 0, "g_Bones");
	}

#ifdef _DEBUG
	for (size_t i = 0; i < ATTACK_END; ++i)
		m_pCol_Attack[i]->Render();

	m_pCol_Laser->Render();
	m_pCol_AttackRange->Render();
	m_pCol_ChaseRange->Render();
	m_pCol_Body->Render();
	m_pCol_Volume->Render();
#endif // DEBUG

	_bool bUseRimLight = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bRimLight", &bUseRimLight, sizeof(_bool)));
	_float4 vColor = { 1.f, 1.f, 1.f, 1.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &vColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	return S_OK;
}

void CEnemy_Boss_Weaver::On_Dead()
{
	USEGAMEINSTANCE;
	pGameInstance->Add_Event(EVENT_MONSTER_DEAD, this, nullptr);
}

HRESULT CEnemy_Boss_Weaver::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_WEAVER), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pDissolveTextureCom));
	m_iDissolveTextureNoise = 279;
	m_iDissolveTextureColor = 32;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 8.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);

	for (_uint i = 0; i < ATTACK_06; ++i)
	{
		FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_AABB), ID_UINT(COLLIDER, COLLIDER_AABB, COL_ENEMY_ATTACK, i), (CComponent**)&m_pCol_Attack[i], &ColliderDesc));
		m_pCol_Attack[i]->Set_Owner(this);
	}

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 2.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK, 6), (CComponent**)&m_pCol_Attack[ATTACK_06], &ColliderDesc));
	m_pCol_Attack[ATTACK_06]->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 5.f, 40.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 3.f, 20.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_OBB), ID_UINT(COLLIDER, COLLIDER_OBB, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Laser, &ColliderDesc));
	m_pCol_Laser->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(20.f, 20.f, 20.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACKRANGE), (CComponent**)&m_pCol_AttackRange, &ColliderDesc));
	m_pCol_AttackRange->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(40.f, 40.f, 40.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_CHASERANGE), (CComponent**)&m_pCol_ChaseRange, &ColliderDesc));
	m_pCol_ChaseRange->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_HITRANGE), (CComponent**)&m_pCol_Body, &ColliderDesc));
	m_pCol_Body->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_BOSS_VOLUME), (CComponent**)&m_pCol_Volume, &ColliderDesc));
	m_pCol_Volume->Set_Owner(this);

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
		return E_FAIL;
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);
	return S_OK;
}

HRESULT CEnemy_Boss_Weaver::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &m_vColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(0.f, 0.9, 1.f, 0.1f);
	_vector vAlphaZeroGlow = XMVectorSet(0.f, 0.6f, 1.f, 0.f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	_bool	bUseToon = false;
	//_bool	bUseOutline = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bToon", &bUseToon, sizeof(_bool)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bOutline", &bUseOutline, sizeof(_bool)));

	return S_OK;
}

HRESULT CEnemy_Boss_Weaver::SetUp_State_Handler()
{
	/*
		//Boss_Weaver
		STATE_BOSS_WEAVER_IDLE,				//21

		STATE_BOSS_WEAVER_APEAR,			//22 대기->아이들
		STATE_BOSS_WEAVER_RESET,			//15 모든 동작끝
		STATE_BOSS_WEAVER_READY,			//36 대기

		STATE_BOSS_WEAVER_WALK_B,			//25 후진
		STATE_BOSS_WEAVER_WALK_F,			//26 전진
		STATE_BOSS_WEAVER_WALK_L,			//27 좌
		STATE_BOSS_WEAVER_WALK_R,			//28 우

		STATE_BOSS_WEAVER_TURN_L,			//23 왼쪽으로 돎
		STATE_BOSS_WEAVER_TURN_R,			//24 오른쪽으로 돎

		STATE_BOSS_WEAVER_ATTACK_01,		//0	기본 평타 두대
		STATE_BOSS_WEAVER_ATTACK_02,		//1 칼 난자
		STATE_BOSS_WEAVER_ATTACK_03,		//2 칼을 한자루씩 날리고 바닥으로 숨겨서 다시 가져옴
		STATE_BOSS_WEAVER_ATTACK_04,		//3 뭔가 전체공격느낌의 칼을 사방으로 날리고 다시모아옴
		STATE_BOSS_WEAVER_ATTACK_05_1,		//4 대쉬공격 시작
		STATE_BOSS_WEAVER_ATTACK_05_2,		//5 대쉬공격 끝
		STATE_BOSS_WEAVER_ATTACK_06_BEGIN,	//6 4번보다 작은 느낌의 전체공격기
		STATE_BOSS_WEAVER_ATTACK_06_BREAK,	//7 중간에 조건 달성시 그로기 -> WEAKLOOP로 이어짐
		STATE_BOSS_WEAVER_ATTACK_06_END,	//8 종료
		STATE_BOSS_WEAVER_ATTACK_06_LOOP,	//9 루프
		STATE_BOSS_WEAVER_ATTACK_07,		//10 레이저 포
		STATE_BOSS_WEAVER_ATTACK_08,		//11 한자루씩 날리고 땅속으로 숨겨서 되돌아옴
		STATE_BOSS_WEAVER_ATTACK_09,		//13 칼을 모아서 바닥에 꽂음 충격파 좋을듯
		STATE_BOSS_WEAVER_ATTACK_10,		//14 칼을 모아서 재정비하는 느낌 리셋2 로 쓰면될듯
		STATE_BOSS_WEAVER_ATTACK_11_BEGIN,	//16 타깃에 칼을 꽂아서 들어올림 //홀딩 성공
		STATE_BOSS_WEAVER_ATTACK_11_LOOP,	//19 딜박히는중
		STATE_BOSS_WEAVER_ATTACK_11_END,	//18 끝
		STATE_BOSS_WEAVER_ATTACK_12,		//17 타깃에 그냥 칼을 꽂아서 공격 //홀딩 실패

		STATE_BOSS_WEAVER_SHIELD,			//20 실드 생기게하면댐

		STATE_BOSS_WEAVER_WEAK_BEGIN,		//37 그로기 시작 특정조건
		STATE_BOSS_WEAVER_WEAK_END,			//39 그로기 끝
		STATE_BOSS_WEAVER_WEAK_LOOP,		//38 그로기 중

		STATE_BOSS_WEAVER_DEAD,				//34 주금
		STAET_BOSS_WEAVER_END,
		*/
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_IDLE, 21);		//등장전 
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_CAM_ACTION, 22);		//등장전 
	
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_APEAR, 20);			//22 대기->아이들
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_RESET, 15);		//15 모든 동작끝
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_READY, 36);			//36 대기

	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_WALK_B, 25);			//25 후진
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_WALK_F, 26);		//26 전진
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_WALK_L, 28);			//27 좌
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_WALK_R, 27);			//28 우
		
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_TURN_L, 23);			//23 왼쪽으로 돎
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_TURN_R, 24);			//24 오른쪽으로 돎
		
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_01, 0);		//0	기본 평타 두대
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_02, 1);		//1 칼 난자
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_03, 2);		//2 칼을 한자루씩 날리고 바닥으로 숨겨서 다시 가져옴
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_04, 3);		//3 뭔가 전체공격느낌의 칼을 사방으로 날리고 다시모아옴
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_05_1, 4);		//4 대쉬공격 시작
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_05_2, 5);		//5 대쉬공격 끝
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_06_BEGIN, 6);	//6 4번보다 작은 느낌의 전체공격기
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_06_BREAK, 7);	//7 중간에 조건 달성시 그로기 -> WEAKLOOP로 이어짐
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_06_END, 8);	//8 종료
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_06_LOOP, 9);	//9 루프
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_07, 10);		//10 레이저 포
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_08, 11);		//11 한자루씩 날리고 땅속으로 숨겨서 되돌아옴
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_09, 13);		//13 칼을 모아서 바닥에 꽂음 충격파 좋을듯
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_10, 14);		//14 칼을 모아서 재정비하는 느낌 리셋2 로 쓰면될듯
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_11_BEGIN, 16);	//16 타깃에 칼을 꽂아서 들어올림
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_11_LOOP, 19);	//19 딜박히는중
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_11_END, 18);	//18 끝
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_ATTACK_12, 17);		//17 타깃에 그냥 칼을 꽂아서 공격
	
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_SHIELD, 20);			//20 실드 생기게하면댐
		
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_WEAK_BEGIN, 37);		//37 그로기 시작 특정조건
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_WEAK_END, 39);		//39 그로기 끝
	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_WEAK_LOOP, 38);		//38 그로기 중

	m_pState_Handler->Set_Animation(STATE_BOSS_WEAVER_DEAD, 34);			//34 주금

	m_pState_Handler->SetUp_StartState(STATE_BOSS_WEAVER_READY);

	return S_OK;
}

void CEnemy_Boss_Weaver::SetUp_ColMatrix()
{
	CHierarchyNode* pHierarchyNode = m_pModelCom->Find_HierarcyNode("Bone019");
	_matrix HierarchyMatrix, ColliderMatrix;
	ColliderMatrix = XMMatrixIdentity();
	ColliderMatrix = XMMatrixScaling(1.f, 1.f, 1.f);

	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Attack[ATTACK_00]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("Bone020");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Attack[ATTACK_01]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("Bone021");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Attack[ATTACK_02]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("Bone022");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Attack[ATTACK_03]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("Bone023");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Attack[ATTACK_04]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("Bone024");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Attack[ATTACK_05]->Update(ColliderMatrix);
}


CEnemy_Boss_Weaver* CEnemy_Boss_Weaver::Create()
{
	CEnemy_Boss_Weaver* pInstance = new CEnemy_Boss_Weaver();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Boss_Weaver");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Boss_Weaver::Clone(void* pArg)
{
	CEnemy_Boss_Weaver* pInstance = new CEnemy_Boss_Weaver(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Boss_Weaver");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Boss_Weaver::Free()
{
	__super::Free();
}
