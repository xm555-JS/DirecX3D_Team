#include "stdafx.h"
#include "State_Handler.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Boss_Dragon.h"
#include "UI.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "MapObj_Instance.h"
#include "Quest_Manager.h"

#include "Imgui_Manager.h"

CEnemy_Boss_FrostDragon::CEnemy_Boss_FrostDragon()
	: CEnemy_Boss()
{
}

CEnemy_Boss_FrostDragon::CEnemy_Boss_FrostDragon(const CEnemy_Boss& rhs)
	: CEnemy_Boss(rhs)
{
}

HRESULT CEnemy_Boss_FrostDragon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Boss_FrostDragon::Initialize(void* pArg)
{
	m_fFrustumRange = 10.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	FAILEDCHECK(SetUp_State_Handler());

	m_pInfo = new UNITDESC();
	m_pCurInfo = new UNITDESC();

	*m_pInfo = UNITDESC(ELEMENTAL_DEFAULT, 40000.f, 10.f, 2.f);
	//memcpy(m_pInfo, &UNITDESC(ELEMENTAL_DEFAULT, 20000.f, 10.f, 2.f), sizeof(UNITDESC));
	memcpy(m_pCurInfo, m_pInfo, sizeof(UNITDESC));

	USEGAMEINSTANCE;

	//51.f, 96.f, 442.f, 1.f_float4(100.f, 160.f, 435.27, 1.f)
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(99.625f, 163.5f, 500.87f, 1.f));
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	//m_pTransformCom->Set_Scaled(_float3(1.5f, 1.5f, 1.5f));

	FAILEDCHECK(Create_UI_Status("FrostDragonStatus"));



	return S_OK;
}

void CEnemy_Boss_FrostDragon::Tick(_float fTimeDelta)
{
	//Picking(CVIBuffer * pVIBuffer, CTransform * pTransform, _fvector _vRayPos, _fvector _vRayDir, _vector * vOut, _float * pHight)
	__super::Tick(fTimeDelta);

	m_pState_Handler->Tick(fTimeDelta);

#if defined(_TEST_JJS)
	// UI Test
	USEGAMEINSTANCE;
	if (KEY_DOWN('H'))
	{
		m_fPreHp = m_pCurInfo->fHP;
		m_pCurInfo->fHP -= 2000.f;
		pGameInstance->Add_Event(EVENT_HPCHANGE, this, &m_fPreHp);
	}
#endif

	//if (KEY_DOWN('Y'))
	//{
	//	_uint iTotalDamage = Random(0, 9999);
	//	// 데미지가 얼마인지.	
	//	_uint iDamage = 0;
	//	list<_uint> Damagelist;
	//	Damagelist.clear();
	//	while (0 != iTotalDamage)
	//	{
	//		iDamage = iTotalDamage % 10;
	//		Damagelist.push_back(iDamage);

	//		if (iTotalDamage < 10)
	//			break;

	//		iTotalDamage /= 10;
	//	}

	//	_uint iIdx = 0;
	//	Damagelist.reverse();
	//	for (auto& Num : Damagelist)
	//	{
	//		NULLCHECK(pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_DAMAGE_NUM), ID_UINT(UI, UI_DAMAGE_NUM), &CUI::UIDesc(200.f, 200.f, 200.f, 200.f, 0.f, Num, 300, { 25.f * iIdx, 0.f }, this , iIdx)));

	//		iIdx++;
	//	}
	//}

	SetUp_ColMatrix();
	m_pCol_Attack->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_AttackRange->Update(m_pTransformCom->Get_WorldMatrix());


	////퀘스트있을때 PlaceMark 생성
	//if (QUEST_KILL_JUMPING_FROSTDRAGON == CQuest_Manager::Get_Instance()->Get_CurQuset())
	//{
	//	IMPL_ONCE(m_bCreate_PlaceMark, Create_UI_Place_Mark(4.f));
	//}
	if (m_pCurInfo->fHP <= 0)
	{
		MGR(CSound_Manager)->ChangeBGM(TEXT("Stone_Island.mp3"), 0.3f, 0.1f);
	}
}

void CEnemy_Boss_FrostDragon::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	for (_uint i = 0; i < BODY_END; ++i)
		pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_HITRANGE), m_pCol_Body[i]);

	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Attack);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_TailAttack);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Laser);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACKRANGE), m_pCol_AttackRange);

	if (IsInFrustum())
	{
		pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}

}

HRESULT CEnemy_Boss_FrostDragon::Render()
{
	//__super::Render();

	NULLCHECK(m_pShaderCom);
	NULLCHECK(m_pModelCom);

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 0, aiTextureType_DIFFUSE)); // 몸통
	m_pModelCom->Render(0, m_pShaderCom, 0, "g_Bones");
	FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", 1, aiTextureType_DIFFUSE)); // 꼬리
	m_pModelCom->Render(1, m_pShaderCom, 2, "g_Bones");


#ifdef _DEBUG
	for (_uint i = 0; i < BODY_END; ++i)
		m_pCol_Body[i]->Render();

	m_pCol_Attack->Render();
	m_pCol_TailAttack->Render();
	m_pCol_Laser->Render();
	m_pCol_AttackRange->Render();
#endif // _DEBUG

	_float4 vColor = {1.f, 1.f, 1.f, 1.f};
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &vColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	_bool bUseRimLight = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bRimLight", &bUseRimLight, sizeof(_bool)));

	return S_OK;
}

HRESULT CEnemy_Boss_FrostDragon::SetUp_Components()
{
	__super::SetUp_Components();

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_FROST_DRAGON), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pDissolveTextureCom));
	m_iDissolveTextureNoise = 198;
	m_iDissolveTextureColor = 32;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);

	for (_uint i = 0; i < BODY_END; ++i)
	{
		FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_HITRANGE, i), (CComponent**)&m_pCol_Body[i], &ColliderDesc));
		m_pCol_Body[i]->Set_Owner(this);
	}

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(40.f, 40.f, 40.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, -15.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACKRANGE), (CComponent**)&m_pCol_AttackRange, &ColliderDesc));
	m_pCol_AttackRange->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(15.f, 15.f, 15.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Attack, &ColliderDesc));
	m_pCol_Attack->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(10.f, 10.f, 10.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK, 1), (CComponent**)&m_pCol_TailAttack, &ColliderDesc));
	m_pCol_TailAttack->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(40.f, 3.f, 3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(20.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_OBB), ID_UINT(COLLIDER, COLLIDER_OBB, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Laser, &ColliderDesc));
	m_pCol_Laser->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Boss_FrostDragon::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &m_vColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(0.9f, 0.3f, 0.f, 0.8f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 0.6f, 0.4f, 0.f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	_bool	bUseToon = false;
	//_bool	bUseOutline = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bToon", &bUseToon, sizeof(_bool)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bOutline", &bUseOutline, sizeof(_bool)));

	return S_OK;
}

HRESULT CEnemy_Boss_FrostDragon::SetUp_State_Handler()
{
	/*
		// Boss_Dragon
		STATE_BOSS_DRAGON_IDLE,				//9
		STATE_BOSS_DRAGON_APEAR,			//8
		STATE_BOSS_DRAGON_PATROL,			//22

		STATE_BOSS_DRAGON_TURNL,			//10
		STATE_BOSS_DRAGON_TURNR,			//11

		STATE_BOSS_DRAGON_ATTACK_01,		//0
		STATE_BOSS_DRAGON_ATTACK_02,		//1
		STATE_BOSS_DRAGON_ATTACK_03,		//2
		STATE_BOSS_DRAGON_ATTACK_04,		//3
		STATE_BOSS_DRAGON_ATTACK_05,		//4
		STATE_BOSS_DRAGON_ATTACK_06,		//5
		STATE_BOSS_DRAGON_ATTACK_07,		//6
		STATE_BOSS_DRAGON_ATTACK_08,		//7

		STATE_BOSS_DRAGON_ULT_ATTACK_1,		//26
		STATE_BOSS_DRAGON_ULT_ATTACK_2,		//27
		STATE_BOSS_DRAGON_ULT_ATTACK_3,		//28

		STATE_BOSS_DRAGON_FALL_LOOP,		//21
		STATE_BOSS_DRAGON_FALL_LAND,		//20

		STATE_BOSS_DRAGON_WEAK_BEGIN,		//29
		STATE_BOSS_DRAGON_WEAK_END,			//30
		STATE_BOSS_DRAGON_WEAK_LOOP,		//31

		STATE_BOSS_DRAGON_DEAD,				//18
		*/
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_CAM_ACTION, 3);			//초기

	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_PATROL, 25);			//초기
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_PATROL_APEAR, 22);	//초기

	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_APEAR, 8);			//등장
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_DOEND, 26);			//등장
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_RUNEND, 8);			//등장

	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_CHASE, 26);			//쫓아감
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_IDLE, 9);				//기본 
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_RUN, 9);				//날아감

	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_TURNL, 9);			//왼쪽턴
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_TURNR, 9);			//오른쪽턴

	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ATTACK_01, 0);		//
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ATTACK_02, 1);		//
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ATTACK_03, 2);		//
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ATTACK_04, 3);		//
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ATTACK_05, 4);		//
	//m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ATTACK_06, 5);		//
	//m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ATTACK_07, 6);		//
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ATTACK_08, 7);		//
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ATTACK_09, 26);		//

	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ULT_ATTACK_1, 26);	//회전
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ULT_ATTACK_2, 27);	//유지
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_ULT_ATTACK_3, 28);	//안쓸듯

	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_FALL_LOOP, 21);		//떨어지는중
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_FALL_LAND, 20);		//땅에 닿는다

	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_WEAK_BEGIN, 29);		//
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_WEAK_END, 30);		//발싸
	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_WEAK_LOOP, 31);		//땅에서 대기

	m_pState_Handler->Set_Animation(STATE_BOSS_DRAGON_DEAD, 18);			//주금

	m_pState_Handler->SetUp_StartState(STATE_BOSS_DRAGON_PATROL_APEAR);

	return S_OK;
}

void CEnemy_Boss_FrostDragon::SetUp_ColMatrix()
{
	CHierarchyNode* pHierarchyNode = m_pModelCom->Find_HierarcyNode("tongue_01");
	_matrix HierarchyMatrix, ColliderMatrix;
	ColliderMatrix = XMMatrixIdentity();
	ColliderMatrix = XMMatrixScaling(2.f, 2.f, 2.f);

	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_00]->Update(ColliderMatrix);

	ColliderMatrix.r[0] = XMVector3Normalize(HierarchyMatrix.r[0]);
	ColliderMatrix.r[1] = XMVector3Normalize(HierarchyMatrix.r[1]);
	ColliderMatrix.r[2] = XMVector3Normalize(HierarchyMatrix.r[2]);

	m_pCol_Laser->Update(ColliderMatrix);

	ColliderMatrix = XMMatrixIdentity();
	ColliderMatrix = XMMatrixScaling(2.f, 2.f, 2.f);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("Bone_-Neck1");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_01]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("Bone_Spine1");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_02]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei00");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_03]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei01");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_04]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei02");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_05]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei03");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_06]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei04");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_07]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei05");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_08]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei06");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_09]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei07");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_11]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei08");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_10]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei09");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_12]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei10");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_13]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei11");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_14]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei12");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_15]->Update(ColliderMatrix);

	pHierarchyNode = m_pModelCom->Find_HierarcyNode("bone_longwei13");
	HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix() * m_pTransformCom->Get_WorldMatrix());
	ColliderMatrix.r[3] = HierarchyMatrix.r[3];

	m_pCol_Body[BODY_16]->Update(ColliderMatrix);

	m_pCol_TailAttack->Update(ColliderMatrix);
}


CEnemy_Boss_FrostDragon* CEnemy_Boss_FrostDragon::Create()
{
	CEnemy_Boss_FrostDragon* pInstance = new CEnemy_Boss_FrostDragon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Boss_FrostDragon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Boss_FrostDragon::Clone(void* pArg)
{
	CEnemy_Boss_FrostDragon* pInstance = new CEnemy_Boss_FrostDragon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Boss_FrostDragon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Boss_FrostDragon::Free()
{
	__super::Free();
}
