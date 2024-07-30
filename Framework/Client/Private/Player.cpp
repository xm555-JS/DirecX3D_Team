#include "stdafx.h"
#include "..\Public\Player.h"
#include "PhysXCollider.h"
#include "Math_Utillity.h"
#include "Easing_Utillity.h"

#include "GameInstance.h"
#include "Collider.h"
#include "State_Handler.h"

#include "Sound_Manager.h"
#include "Navigation.h"

#include "UI_Solid.h"
#include "UI_Attacked_Num.h"
#include "Item_Manager.h"
#include "Quest_Manager.h"
#include "Item.h"
#include "Enemy_Normal.h"

#include "Weapon_Sword.h"
#include "Weapon_2Gun.h"
#include "Weapon_ShieldAxe.h"
#include "Weapon_CubeL.h"
#include "Weapon_CubeR.h"

//Mobility
#include "Mobility_Glider.h"
#include "Mobility_MotorCycle.h"

#include "Light.h"
#include "Camera.h"

#include "Bullet_Player_Cube.h"
#include "Bullet_Player_Cube_Main.h"
#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

#include "Imgui_Manager.h"
#include "Minigame_Manager.h"


CPlayer::CPlayer()
	: CUnit()
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CUnit(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	//CTransform::TRANSFORMDESC		TransformDesc;
	//FAILEDCHECK(__super::Initialize(&TransformDesc));

	m_pInfo = new PLAYERDESC();
	m_pCurInfo = new PLAYERDESC();

	memcpy(m_pInfo, pArg, sizeof(PLAYERDESC));
	memcpy(m_pCurInfo, pArg, sizeof(PLAYERDESC));

	__super::Initialize(nullptr);

	FAILEDCHECK(SetUp_Components());

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_PLAYER, this);
	pGameInstance->Add_LateTickGroup(LTG_PLAYER, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	FAILEDCHECK(SetUp_State_Handler());

	pGameInstance->Add_CallbackList(this, EVENT_PLAYER_DAMAGED);

	((PLAYERDESC*)m_pCurInfo)->fSkillGauge = 0.f;

	//m_pState_Handler->Set_Animation();

	pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_LOADING_SCREEN), ID_UINT(UI, UI_LOADING_SCREEN),
		&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 21, 5, { 0.f,0.f }, this));

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;

	if (/*STATE_PLAYER_SKATE_CALL == m_pState_Handler->Get_CurStateType()
		||*/ STATE_PLAYER_SKATE_RUN == m_pState_Handler->Get_CurStateType()
		|| STATE_PLAYER_SKATE_RUN_SWERVE_L == m_pState_Handler->Get_CurStateType()
		|| STATE_PLAYER_SKATE_RUN_SWERVE_R == m_pState_Handler->Get_CurStateType())
	{
		if (XMVectorGetY(Get_Pos()) <= 10.6f)
		{
			IMPL_ONCE(m_bSoundSkate, MGR(CSound_Manager)->PlaySurroundSound(TEXT("State_Ing.mp3"), CSound_Manager::CHANNEL_PLAYER, Get_Pos(), 0.5f));
			IMPL_ALARM(m_fSoundSkate, MGR(CSound_Manager)->PlaySurroundSound(TEXT("State_Ing.mp3"), CSound_Manager::CHANNEL_PLAYER, Get_Pos(), 0.5f), 0.4f, fTimeDelta);
		}

	}
	else
	{
		RESET_ONCE(m_bSoundSkate);
		RESET_ALARM(m_fSoundSkate);
	}

	if (KEY_DOWN(VK_DOWN))
		m_bUseDissolve = true;

	_float4 vPos;
	XMStoreFloat4(&vPos, Get_Pos());
	if (vPos.y < m_fSwimHeight + 0.1f)
		m_pTransformCom->Swim(m_fSwimHeight, m_pNaviCom);


	if(m_bUseDissolve)
		m_fDissolveTimeAcc += fTimeDelta;

	if (m_pTransformCom->Is_InGlide())
	{
		m_fNavigationAccTime += fTimeDelta;
		if (1.f < m_fNavigationAccTime)
		{
			m_fNavigationAccTime = 0.f;
			m_pNaviCom->Find_CurrentIndex(m_pTransformCom);
		}
	}

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Gun->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Body->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_UIIntrc->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Volume->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Range->Update(m_pTransformCom->Get_WorldMatrix());
	m_pState_Handler->Tick(fTimeDelta);
	m_pTransformCom->Tick(fTimeDelta);

#if defined(_WEAPON)
	if (m_pCurWeapon_L)
		m_pCurWeapon_L->Tick(fTimeDelta);
	if (m_pCurWeapon_R)
		m_pCurWeapon_R->Tick(fTimeDelta);
	if (m_pMobilityGlider)
		m_pMobilityGlider->Tick(fTimeDelta);
	if (m_pMobilityMotorCycle)
		m_pMobilityMotorCycle->Tick(fTimeDelta);
#endif
	//m_pModelCom->Set_CurrentAnimation(m_iAnimNum);

	

	Input_WeaponKey();
	Input_DashKey(fTimeDelta);
	Input_StaminaKey(fTimeDelta);
	SetUp_SkillGauge();
	SetUp_CoolTime(fTimeDelta);

	Gain_Item();
	Create_UI_Enemy_Indicator();

	//_vector DebugVector = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	//Set_NearTarget();

	for (auto& iter : *m_pCol_Volume->Get_StayColliders())
	{
		Push_Other(iter, m_pCol_Volume, m_pTransformCom, m_pNaviCom);
	}

	if (nullptr != m_pTarget )
	{
		if (m_pTarget->Is_Dead() || 
			0 >= ((CUnit*)m_pTarget)->Get_UnitCurInfo()->fHP)
		{
			m_pTarget = nullptr;
		}
	}

#if defined(_TEST_SHIN)
	if (KEY_DOWN('K'))
	{
		Set_AddCubeBullet();
	}
#endif

#if defined(_TEST_YJS)
	if (KEY_DOWN('U'))
	{
		_uint iItem = Random(1, ITEM_END - 1);
		CItem_Manager::Get_Instance()->Add_Item((ITEM_TYPE)iItem);
		CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE,10);
	}
	if (KEY_DOWN('Y')) // 마을
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(256.f, 0.f, 256.f, 1.f));
		m_pNaviCom->Find_CurrentIndex(m_pTransformCom);
	}
	if (KEY_DOWN('N')) //용
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(100.f, 153.f, 430.27, 1.f));
		m_pNaviCom->Find_CurrentIndex(m_pTransformCom);
	}
	if (KEY_DOWN('M')) //웨버
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(436.26f, 153.46f, 308.f, 1.f));
		m_pNaviCom->Set_CurCellDesc(25591); // 웨버
	}
	if (KEY_DOWN(VK_OEM_COMMA)) // ','  // 로벨라.
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(138.44f, 22.39f, 123.12f, 1.f));
		m_pNaviCom->Find_CurrentIndex(m_pTransformCom);
	}
	if (KEY_DOWN('I'))
	{
		//CMinigame_Manager::Get_Instance()->Notice_Minigame();
		/*pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 4.f, 0.5f, 1.f, 0.025f);
		pGameInstance->Add_DirCA(_float2(1.f, -1.f), _float4(0.015f, 0.f, 0.0075f, 0.f), 4.f, 0.5f);*/
		//pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 4.f, 1.f, 1.f, DEBUGFLOAT4(0).w);
		//pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), DEBUGFLOAT4(0), 4.f, 1.f);
		//pGameInstance->Add_DirCA(_float2(DEBUGFLOAT4(1).x, DEBUGFLOAT4(1).y), DEBUGFLOAT4(0), 1.f, 1.f);
		//pGameInstance->Add_Camera_Shaking(DEBUGVECTOR(0), DEBUGFLOAT4(1).x, DEBUGFLOAT4(1).y, DEBUGFLOAT4(1).z);
		//pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, DEBUGFLOAT, DEBUGFLOAT, 1.f, DEBUGFLOAT4(0).z, false, true);
		//pGameInstance->Add_DirCA(_float2(DEBUGFLOAT4(1).x, DEBUGFLOAT4(1).y), DEBUGFLOAT4(0), DEBUGFLOAT4(1).z, DEBUGFLOAT4(1).z);
		//pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), DEBUGFLOAT4(1), DEBUGFLOAT, DEBUGFLOAT, false, true);
		//pGameInstance->Add_Camera_Shaking(DEBUGVECTOR(2), DEBUGFLOAT4(3).x, DEBUGFLOAT4(3).y, DEBUGFLOAT4(3).z);
		//pGameInstance->Add_DirCA(_float2(1.f, 2.f), { 0.028f,0.01f,0.007f,0.f }, 0.3f, 0.3f);
		//pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), {0.017f, 0.004f, 0.004f, 0.f}, 1.f, 1.f, true);
	}
	if (KEY_DOWN('O'))
	{
		/*pGameInstance->Stop_RadialBlur();
		pGameInstance->Stop_RadialCA();*/
	}
#endif

#if 1
	m_fWaitRecoverTime += fTimeDelta;
	m_fRecoverhpTime += fTimeDelta;
	if (7.f < m_fWaitRecoverTime)
	{
		if (2.f < m_fRecoverhpTime)
		{
			m_fPreHp = m_pCurInfo->fHP;
			m_pCurInfo->fHP += m_pInfo->fHP * 0.05f;
			if (m_pInfo->fHP < m_pCurInfo->fHP)
			{
				m_pCurInfo->fHP = m_pInfo->fHP;
			}
			pGameInstance->Add_Event(EVENT_HPCHANGE, this, &m_fPreHp);
			m_fRecoverhpTime = 0.f;
		}
	}
	if (m_pCurInfo->fHP < m_pInfo->fHP * 0.1f)
	{
		IMPL_ONCE(m_bScreenEffect0, pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), { 0.03f, 0.0f, 0.015f, 0.f }, 1.2f, 1.2f, true));
		RESET_ONCE(m_bScreenEffect1);
	}
	else if (m_pCurInfo->fHP >= m_pInfo->fHP * 0.1f)
	{
		IMPL_ONCE(m_bScreenEffect1, pGameInstance->Stop_RadialCA()); // 함수 수정하면 주석 풀기
		RESET_ONCE(m_bScreenEffect0);
	}
#endif // 0



	if (m_fShield != 1.f)
	{
		m_fShieldTime += fTimeDelta;
		if (5.f < m_fShieldTime)
		{
			m_fShield = 1.f;
			m_fShieldTime = 0.f;
		}
	}

	m_iReserveEnableKeyInput++;
	if (5 == m_iReserveEnableKeyInput)
	{
		m_bEnableKeyInput = true;
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*if (KEY_PRESSING(VK_UP))
		m_pTransformCom->Go_Straight(fTimeDelta, 5.f);
	if (KEY_PRESSING(VK_DOWN))
		m_pTransformCom->Go_Back(fTimeDelta, 5.f);
	if (KEY_PRESSING(VK_LEFT))
		m_pTransformCom->Go_Left(fTimeDelta, 5.f);
	if (KEY_PRESSING(VK_RIGHT))
		m_pTransformCom->Go_Right(fTimeDelta, 5.f);*/

#pragma region CubeBullet
	if (WEAPON_CUBE == m_eCurWeapon)
	{
		list<CGameObject*>* CubeBullets = (GAMEINSTANCE->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_CUBE_BULLET)));

		if (m_bCubeBulletCreate || (nullptr != CubeBullets && m_iCubeBulletCnt != CubeBullets->size()))
		{
			m_fCubeLerpTime += fTimeDelta;
			Set_CubeBulletPos();
			m_iCubeBulletCnt = CubeBullets->size();
		}
		else
		{
			m_fCubeLerpTime = 0.f;
			for(_uint i = 0; i < 5; ++i)
				m_bCubeBulletGetPos[i] = false;
		}

	}
	else
	{
		list<CGameObject*>* CubeBullets = (pGameInstance->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_CUBE_BULLET)));
		list<CGameObject*>* Cubes = (pGameInstance->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_CUBE)));
		if (nullptr == Cubes || 0 == Cubes->size())
			return;

		for (auto& iter : *Cubes)
		{
			pGameInstance->Add_PoolingObject(iter);
		}

		if (nullptr == CubeBullets || 0 == CubeBullets->size())
			return;

		for (auto& iter : *CubeBullets)
		{
			pGameInstance->Add_PoolingObject(iter);
		}
		Cubes->clear();
		CubeBullets->clear();
		m_iCubeBulletCnt = 0;
	}
#pragma endregion CubeBullet
}

void CPlayer::LateTick(_float fTimeDelta)
{
	if((m_pTransformCom->Is_InSkate() && 10.6f < XMVectorGetY(Get_Pos())) ||
		(!m_pTransformCom->Is_InAir() && !m_pTransformCom->Is_InWater()))
		m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);

#if defined(_WEAPON)
	if(m_pCurWeapon_L)
		m_pCurWeapon_L->LateTick(fTimeDelta);
	if (m_pCurWeapon_R)
		m_pCurWeapon_R->LateTick(fTimeDelta);
	if (m_pMobilityGlider)
		m_pMobilityGlider->LateTick(fTimeDelta);
	if (m_pMobilityMotorCycle)
		m_pMobilityMotorCycle->LateTick(fTimeDelta);
#endif

	USEGAMEINSTANCE;
	
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_ATTACK), m_pCol_Sphere);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_ATTACK), m_pCol_Gun);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_BODY), m_pCol_Body);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_UIINTRC), m_pCol_UIIntrc);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_VOLUME), m_pCol_Volume);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_ATTACK_RANGE), m_pCol_Range);
	//GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CPlayer::Render()
{
	NULLCHECK(m_pShaderCom);
	NULLCHECK(m_pModelCom);

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

		m_pModelCom->Render(i, m_pShaderCom, 0, "g_Bones");
	}

//#if defined(_WEAPON)
//	if (m_pCurWeapon_L)
//		m_pCurWeapon_L->Render();
//	if (m_pCurWeapon_R)
//		m_pCurWeapon_R->Render();
//	if (m_pMobilityGlider)
//		m_pMobilityGlider->Render();
//	if (m_pMobilityMotorCycle)
//		m_pMobilityMotorCycle->Render();
//#endif

	

	//_bool		bGlow = false;
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));

	//m_pNaviCom->Render();

#ifdef _DEBUG
	m_pCol_Sphere->Render();
	m_pCol_Gun->Render();
	m_pCol_Body->Render();
	m_pCol_Volume->Render();
	//m_pCol_UIIntrc->Render();
#endif // _DEBUG

	//_bool	bUseToon = false;
	//_bool	bUseOutline = false;
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bToon", &bUseToon, sizeof(_bool)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bOutline", &bUseOutline, sizeof(_bool)));

	return S_OK;
}

HRESULT CPlayer::Render(void* pArg)
{
	RENDERDESC	Desc;
	memcpy(&Desc, pArg, sizeof(RENDERDESC));

	if (RENDERTYPE_DEPTH == Desc.eType)
	{
		FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));

		CLight* pTargetLight = (CLight*)Desc.lParam;
		pTargetLight->SetUp_DepthComponents(m_pShaderCom);

		//m_pShaderCom->Begin(3);

		for (_uint i = 0; i < m_iNumMeshContainers; ++i)
		{
			FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

			m_pModelCom->Render(i, m_pShaderCom, 3, "g_Bones");
		}

//#if defined(_WEAPON)
//		if (m_pCurWeapon_L)
//		{
//			if(m_pCurWeapon_L->Get_IsUsing())
//				m_pCurWeapon_L->Render(pArg);
//		}
//		if (m_pCurWeapon_R)
//		{
//			if (m_pCurWeapon_R->Get_IsUsing())
//				m_pCurWeapon_R->Render(pArg);
//		}
//		if (m_pMobilityGlider)
//		{
//			if (m_pMobilityGlider->Get_IsUsing())
//				m_pMobilityGlider->Render(pArg);
//		}
//		if (m_pMobilityMotorCycle)
//		{
//			if (m_pMobilityMotorCycle->Get_IsUsing())
//				m_pMobilityMotorCycle->Render(pArg);
//		}
//#endif
	}

	return S_OK;
}

void CPlayer::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_PLAYER_DAMAGED == tEvent.eEventType)
	{
		_float fDamage = *(_float*)(tEvent.lParam);

		if (700.f < fDamage)
			m_bDamaged[2] = true;
		else if (500.f < fDamage && 700.f >= fDamage)
			m_bDamaged[1] = true;
		else if (500.f >= fDamage)
			m_bDamaged[0] = true;

	}
}

void CPlayer::Damaged(ELEMENTAL_PROPERTIE eElement, _float fPower)
{
	USEGAMEINSTANCE;
	if (m_bEvasion)
	{
		list<CGameObject*>* pEvasions = pGameInstance->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_TIME_ILLUSION));
		if (nullptr == pEvasions || 1 > pEvasions->size())
		{
			_float4 vPos;
			XMStoreFloat4(&vPos, Get_Pos()); 
			((PLAYERDESC*)m_pCurInfo)->bAbleSkill_S = true;
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_TIME_ILLUSION), ID_UINT(PLAYER, WEAPON_TIME_ILLUSION), &vPos);
		}
	}
	else
	{
		m_fWaitRecoverTime = 0.f;
		m_fPreHp = m_pCurInfo->fHP;
		m_pCurInfo->fHP -= fPower * m_fShield;
		if (0.f > m_pCurInfo->fHP)
			m_pCurInfo->fHP = 0.f;

		m_fDamage = fPower;
		pGameInstance->Add_Event(EVENT_HPCHANGE, this, &m_fPreHp);

		_uint iTotalAttacked = fPower;
		_uint iAttacked = 0;
		list<_uint> Attackedlist;
		Attackedlist.clear();
		while (true)
		{
			iAttacked = iTotalAttacked % 10;
			Attackedlist.push_back(iAttacked);

			if (iTotalAttacked < 10)
				break;

			iTotalAttacked /= 10;
		}

		_uint iIdx = 1;
		_float fDeltaY = fRandom(-50.f, 0.f);
		Attackedlist.reverse();

		CUI_Attacked_Num* pAttacked_Num = nullptr;

		pAttacked_Num = (CUI_Attacked_Num*)(pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ATTACKED_NUM), ID_UINT(UI, UI_ATTACKED_NUM),
			&CUI::UIDesc(200.f, 200.f, 200.f, 200.f, 0.f, 10, 300, { 0.f, fDeltaY }, this, iIdx)));

		for (auto& Num : Attackedlist)
		{
#ifdef _UI
			pAttacked_Num = (CUI_Attacked_Num*)(pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ATTACKED_NUM), ID_UINT(UI, UI_ATTACKED_NUM),
				&CUI::UIDesc(200.f, 200.f, 200.f, 200.f, 0.f, Num, 300, { 22.5f * iIdx, fDeltaY }, this, iIdx)));
			//pAttacked_Num->Set_Element(eElement);
#endif
			iIdx++;
		}

		pGameInstance->Add_Event(EVENT_PLAYER_DAMAGED, &m_fDamage, nullptr);
	}
}

HRESULT CPlayer::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_01), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));

#if defined(_NOISE_TEX)
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));
#endif
	
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 1.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 2.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_OBB), ID_UINT(COLLIDER, COLLIDER_OBB, COL_PLAYER_ATTACK), (CComponent**)&m_pCol_Gun, &ColliderDesc));
	m_pCol_Gun->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_BODY), (CComponent**)&m_pCol_Body, &ColliderDesc));
	m_pCol_Body->Set_Owner(this);
	//m_pCol_Body->Set_ColliderState(false);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_VOLUME), (CComponent**)&m_pCol_Volume, &ColliderDesc));
	m_pCol_Volume->Set_Owner(this);
	
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_UIINTRC), (CComponent**)&m_pCol_UIIntrc, &ColliderDesc));
	m_pCol_UIIntrc->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(20.f, 20.f, 20.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 1.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK_RANGE), (CComponent**)&m_pCol_Range, &ColliderDesc));
	m_pCol_Range->Set_Owner(this);

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();
	m_pModelCom->Set_RootEasing(true);
	m_pModelCom->Set_AnimationGroup(ANIM_PLAYER_FEMALE);

	USEGAMEINSTANCE;
	pGameInstance->SetUp_HierarchyNodePtr(ANIM_PLAYER_FEMALE, m_pModelCom);

#if defined(_MAP)
	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
		return E_FAIL;
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);
#endif

	ZeroMemory(&m_Weapons, sizeof(CWeapon*) * PLAYER_WEAPON_END);
#if defined(_WEAPON)
	m_Weapons[PLAYER_WEAPON_SWORD] = CWeapon_Sword::Create();
	m_Weapons[PLAYER_WEAPON_SWORD]->Set_CurLevel(m_iCurLevel);
	m_Weapons[PLAYER_WEAPON_SWORD]->Initialize(nullptr);
	m_Weapons[PLAYER_WEAPON_SWORD]->Setup_WeaponState(this, string("Point003"));
	m_Weapons[PLAYER_WEAPON_SWORD]->Set_Clone();
	m_Weapons[PLAYER_WEAPON_SWORD]->Set_IsDissolve(true);
	m_Weapons[PLAYER_WEAPON_SWORD]->Set_IsUsing(false);

	// 왼손
	m_Weapons[PLAYER_WEAPON_GUN_L] = CWeapon_2Gun::Create();
	((CWeapon_2Gun*)m_Weapons[PLAYER_WEAPON_GUN_L])->Set_LeftHand();
	m_Weapons[PLAYER_WEAPON_GUN_L]->Set_CurLevel(m_iCurLevel);
	m_Weapons[PLAYER_WEAPON_GUN_L]->Initialize(nullptr);
	m_Weapons[PLAYER_WEAPON_GUN_L]->Setup_WeaponState(this, string("Point002"));
	m_Weapons[PLAYER_WEAPON_GUN_L]->Set_Clone();
	m_Weapons[PLAYER_WEAPON_GUN_L]->Set_IsDissolve(true);
	m_Weapons[PLAYER_WEAPON_GUN_L]->Set_IsUsing(false);

	// 오른손
	m_Weapons[PLAYER_WEAPON_GUN_R] = CWeapon_2Gun::Create();
	m_Weapons[PLAYER_WEAPON_GUN_R]->Set_CurLevel(m_iCurLevel);
	m_Weapons[PLAYER_WEAPON_GUN_R]->Initialize(nullptr);
	m_Weapons[PLAYER_WEAPON_GUN_R]->Setup_WeaponState(this, string("Point003"));
	m_Weapons[PLAYER_WEAPON_GUN_R]->Set_Clone();
	m_Weapons[PLAYER_WEAPON_GUN_R]->Set_IsDissolve(true);
	m_Weapons[PLAYER_WEAPON_GUN_R]->Set_IsUsing(false);

	m_Weapons[PLAYER_WEAPON_SHIELDAXE] = CWeapon_ShieldAxe::Create();
	m_Weapons[PLAYER_WEAPON_SHIELDAXE]->Set_CurLevel(m_iCurLevel);
	m_Weapons[PLAYER_WEAPON_SHIELDAXE]->Initialize(nullptr);
	m_Weapons[PLAYER_WEAPON_SHIELDAXE]->Setup_WeaponState(this, string("Point003"));
	m_Weapons[PLAYER_WEAPON_SHIELDAXE]->Set_Clone();
	m_Weapons[PLAYER_WEAPON_SHIELDAXE]->Set_IsDissolve(true);
	m_Weapons[PLAYER_WEAPON_SHIELDAXE]->Set_IsUsing(false);

	// 왼손
	m_Weapons[PLAYER_WEAPON_CUBE_L] = CWeapon_CubeL::Create();
	m_Weapons[PLAYER_WEAPON_CUBE_L]->Set_CurLevel(m_iCurLevel);
	m_Weapons[PLAYER_WEAPON_CUBE_L]->Initialize(nullptr);
	m_Weapons[PLAYER_WEAPON_CUBE_L]->Setup_WeaponState(this, string("Point002"));
	m_Weapons[PLAYER_WEAPON_CUBE_L]->Set_Clone();
	m_Weapons[PLAYER_WEAPON_CUBE_L]->Set_IsDissolve(true);
	m_Weapons[PLAYER_WEAPON_CUBE_L]->Set_IsUsing(false);

	// 오른손
	m_Weapons[PLAYER_WEAPON_CUBE_R] = CWeapon_CubeR::Create();
	m_Weapons[PLAYER_WEAPON_CUBE_R]->Set_CurLevel(m_iCurLevel);
	m_Weapons[PLAYER_WEAPON_CUBE_R]->Initialize(nullptr);
	m_Weapons[PLAYER_WEAPON_CUBE_R]->Setup_WeaponState(this, string("Point003"));
	m_Weapons[PLAYER_WEAPON_CUBE_R]->Set_Clone();
	m_Weapons[PLAYER_WEAPON_CUBE_R]->Set_IsDissolve(true);
	m_Weapons[PLAYER_WEAPON_CUBE_R]->Set_IsUsing(false);

	//Mobility
	m_Weapons[PLAYER_MOBILITY_GLIDER] = CMobility_Glider::Create();
	m_Weapons[PLAYER_MOBILITY_GLIDER]->Set_CurLevel(m_iCurLevel);
	m_Weapons[PLAYER_MOBILITY_GLIDER]->Initialize(nullptr);
	m_Weapons[PLAYER_MOBILITY_GLIDER]->Setup_WeaponState(this, string("Bone_wp"));
	m_Weapons[PLAYER_MOBILITY_GLIDER]->Set_Clone();
	m_Weapons[PLAYER_MOBILITY_GLIDER]->Set_IsDissolve(true);
	m_Weapons[PLAYER_MOBILITY_GLIDER]->Set_IsUsing(false);

	m_Weapons[PLAYER_MOBILITY_MOTORCYCLE] = CMobility_MotorCycle::Create();
	m_Weapons[PLAYER_MOBILITY_MOTORCYCLE]->Set_CurLevel(m_iCurLevel);
	m_Weapons[PLAYER_MOBILITY_MOTORCYCLE]->Initialize(this);
	m_Weapons[PLAYER_MOBILITY_MOTORCYCLE]->Setup_WeaponState(this, string("Root_Bone"));
	m_Weapons[PLAYER_MOBILITY_MOTORCYCLE]->Set_Clone();
	m_Weapons[PLAYER_MOBILITY_MOTORCYCLE]->Set_IsDissolve(true);
	m_Weapons[PLAYER_MOBILITY_MOTORCYCLE]->Set_IsUsing(false);

	m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_SWORD];

	m_pMobilityGlider = m_Weapons[PLAYER_MOBILITY_GLIDER];
	m_pMobilityMotorCycle = m_Weapons[PLAYER_MOBILITY_MOTORCYCLE];
#endif
	// 임시로 스킬세팅
	m_eCurWeapon = m_eSlot_Weapon[WPN_SLOT_M] = WEAPON_SWORD;
	//m_eCurWeapon = m_eSlot_Weapon[WPN_SLOT_M] = WEAPON_SHIELDAXE;
	m_eNextWeapon = m_eCurWeapon;
	//m_eSlot_Weapon[WPN_SLOT_L] = WEAPON_NONE;WEAPON_2GUN
	m_eSlot_Weapon[WPN_SLOT_L] = WEAPON_NONE;
	//m_eSlot_Weapon[WPN_SLOT_R] = WEAPON_NONE; //WEAPON_2GUN
	m_eSlot_Weapon[WPN_SLOT_R] = WEAPON_NONE;
	//m_eSlot_Weapon[WPN_SLOT_R] = WEAPON_SHIELDAXE;

	m_iSlot_WeaponKey[WPN_SLOT_M] = 'Q';
	m_iSlot_WeaponKey[WPN_SLOT_L] = 'E';
	m_iSlot_WeaponKey[WPN_SLOT_R] = 'R';


	// 초기 무기보유 세팅
	for (_uint i = 0; i < 4; ++i)
	{
		m_eOwnWeapons[i] = WEAPON_NONE;
	}
	m_eOwnWeapons[0] = WEAPON_SWORD;
	/*m_eOwnWeapons[1] = WEAPON_2GUN;
	m_eOwnWeapons[2] = WEAPON_SHIELDAXE;
	m_eOwnWeapons[3] = WEAPON_CUBE;*/

	// 스킬 쿨타임 초기화
	for (_uint i = 0; i < WPN_SLOT_END; i++)
	{
		m_fCoolTime_ChangeWeapon[i] = 4.f;
	}
	for (_uint i = 0; i < WEAPON_HAND1; i++)
	{
		m_fCoolTime_Skill_N[i] = 8.f;
	}

	for (_uint i = 0; i < WPN_SLOT_END; i++)
	{
		m_fTimeChangeWeapon[i] = 0.f;
	}
	for (_uint i = 0; i < WEAPON_HAND1; i++)
	{
		m_fTimeSkill_N[i] = 0.f;
	}

	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	_bool	bUseToon = true;
	_bool	bUseOutline = true;
	//_bool	bUseRimLight = true;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bToon", &bUseToon, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bOutline", &bUseOutline, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bDissolve", &m_bUseDissolve, sizeof(_bool)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bRimLight", &bUseRimLight, sizeof(_bool)));

	_float	fLineBrightness = 0.583f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fTimeAcc", &m_fDissolveTimeAcc, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fLineBrightness", &fLineBrightness, sizeof(_float)));



	_float4 vLook;
	//XMStoreFloat4(&vLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	_matrix ViewMatrixInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW)));
	XMStoreFloat4(&vLook, XMVector3Normalize(ViewMatrixInv.r[2]));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vLook", &vLook, sizeof(_float4)));

	//_float4 vRimLightColor = _float4( 0.f, 0.f, 1.f, 1.f );
	_float fTime = 1.f;
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_RimLightColor", &vRimLightColor, sizeof(_float4)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fRimLightInterval", &fTime, sizeof(_float)));

#if defined(_NOISE_TEX)
	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 5));
#endif

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDissolve", &m_bDissolve, sizeof(_bool)));	

	//_bool		bGlow = true;
	//_float4		vGlowColor = { 1.f, 0.f, 0.f, 1.f };

	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vGlowColor", &vGlowColor, sizeof(_float4)));

	//if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
	//	__debugbreak();

	return S_OK;
}

HRESULT CPlayer::SetUp_State_Handler()
{
	//m_pModelCom->Set_CurrentAnimation(m_iAnimNum);
	 
	m_pState_Handler->Set_ModelComponent(m_pModelCom);
	m_pState_Handler->Set_Owner(this);
	m_pState_Handler->Set_Player(this);

#pragma region Normal
	m_pState_Handler->Set_Animation(STATE_PLAYER_IDLE, 102);						//177

	m_pState_Handler->Set_Animation(STATE_PLAYER_HAND_IDLE, 102);						//177

	m_pState_Handler->Set_Animation(STATE_PLAYER_WALK, 1);							//134
	m_pState_Handler->Set_Animation(STATE_PLAYER_RUN, 2);							//619
																					//
	m_pState_Handler->Set_Animation(STATE_PLAYER_JUMP_A, 3);						//436
	m_pState_Handler->Set_Animation(STATE_PLAYER_JUMP_B, 4);						//424
	m_pState_Handler->Set_Animation(STATE_PLAYER_JUMP_FALL, 5);						//431
	m_pState_Handler->Set_Animation(STATE_PLAYER_JUMP_END, 6);						//426
																					//
	m_pState_Handler->Set_Animation(STATE_PLAYER_ROLL_BACK, 103);					//129//7
	m_pState_Handler->Set_Animation(STATE_PLAYER_ROLL_BACK_AIR, 104);				//130//8
	m_pState_Handler->Set_Animation(STATE_PLAYER_ROLL_FORWORD, 105);				//131//9
	m_pState_Handler->Set_Animation(STATE_PLAYER_ROLL_FORWORD_AIR, 106);			//132//10
	m_pState_Handler->Set_Animation(STATE_PLAYER_ROLL_FORWORD_END, 107);			//133//11

	m_pState_Handler->Set_Animation(STATE_PLAYER_HAND_ROLL_BACK, 103);					//129//7
	m_pState_Handler->Set_Animation(STATE_PLAYER_HAND_ROLL_BACK_AIR, 104);				//130//8
	m_pState_Handler->Set_Animation(STATE_PLAYER_HAND_ROLL_FORWORD, 105);				//131//9
	m_pState_Handler->Set_Animation(STATE_PLAYER_HAND_ROLL_FORWORD_AIR, 106);			//132//10
	m_pState_Handler->Set_Animation(STATE_PLAYER_HAND_ROLL_FORWORD_END, 107);			//133//11

	m_pState_Handler->Set_Animation(STATE_PLAYER_EAT, 53);
	m_pState_Handler->Set_Animation(STATE_PLAYER_DAMAGED_LOW, 55);
	m_pState_Handler->Set_Animation(STATE_PLAYER_DAMAGED_MEDIUM, 54);
	m_pState_Handler->Set_Animation(STATE_PLAYER_DAMAGED_HIGH_BEGIN, 63);
	m_pState_Handler->Set_Animation(STATE_PLAYER_DAMAGED_HIGH_END, 57);

	m_pState_Handler->Set_Animation(STATE_PLAYER_SWIM_DIE, 58);						//58
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWIM_LOOP_A, 59);					//59
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWIM_LOOP_B, 60);					//60
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWIM_IDLE, 61);					//65
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWIM_DASH, 62);					//68

																						
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKATE_CALL, 197);						//197
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKATE_STOP_LAND, 198);					//198
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKATE_STOP_WATER, 199);				//199
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKATE_RUN, 200);						//200
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKATE_RUN_SWERVE_L, 201);				//201
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKATE_RUN_SWERVE_R, 202);				//202
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKATE_JUMP, 203);						//203
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKATE_LANDING, 204);					//204
									
	m_pState_Handler->Set_Animation(STATE_PLAYER_GLIDE_UP, 149);						//149
	m_pState_Handler->Set_Animation(STATE_PLAYER_GLIDE_UP_END, 190);					//149
	m_pState_Handler->Set_Animation(STATE_PLAYER_GLIDE_LOOP, 150);						//150
	m_pState_Handler->Set_Animation(STATE_PLAYER_GLIDE_BEGIN_F, 154);					//154
	m_pState_Handler->Set_Animation(STATE_PLAYER_GLIDE_LOOP_F, 151);					//151
	m_pState_Handler->Set_Animation(STATE_PLAYER_GLIDE_STOP, 152);						//152
	m_pState_Handler->Set_Animation(STATE_PLAYER_GLIDE_END, 153);						//153

	m_pState_Handler->Set_Animation(STATE_PLAYER_MOUNT_UP, 189);						//189
	m_pState_Handler->Set_Animation(STATE_PLAYER_MOUNT_DRIVE, 194);						//190
	m_pState_Handler->Set_Animation(STATE_PLAYER_MOUNT_FREE, 191);						//191
	m_pState_Handler->Set_Animation(STATE_PLAYER_MOUNT_IDLE, 192);						//192
	m_pState_Handler->Set_Animation(STATE_PLAYER_MOUNT_JUMP_START, 193);				//193
	m_pState_Handler->Set_Animation(STATE_PLAYER_MOUNT_TURN_L, 195);					//194
	m_pState_Handler->Set_Animation(STATE_PLAYER_MOUNT_TURN_R, 196);					//195
	m_pState_Handler->Set_Animation(STATE_PLAYER_MOUNT_ROLL, 194);						//195
	m_pState_Handler->Set_Animation(STATE_PLAYER_MOUNT_ROLL_AIR, 194);					//195
	m_pState_Handler->Set_Animation(STATE_PLAYER_MOUNT_DRIVE_AIR, 194);						//190
	
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_INTO_DOWN, 155);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_INTO_UP, 156);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_OUT_ROLL, 157);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_OUT, 158);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_OUT_JUMP, 159);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_STOP, 160);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_LEFT, 161);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_LEFT_DOWN, 162);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_LEFT_UP, 163);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_RIGHT, 164);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_RIGHT_DOWN, 165);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_RIGHT_UP, 166);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_UP, 167);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_ROLL_LEFT, 168);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_ROLL_LEFT_END, 169);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_ROLL_RIGHT, 170);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_ROLL_RIGHT_END, 171);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_ROLL_UP, 172);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_ROLL_UP_END, 173);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_ROLL_UP_LEFT, 174);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_ROLL_UP_LEFT_END, 175);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_ROLL_UP_RIGHT, 176);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_ROLL_UP_RIGHT_END, 177);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CLIMB_DOWN, 178);
#pragma endregion Normal

#pragma region Weapon Base
	m_pState_Handler->Set_Animation(STATE_PLAYER_ATTACK, 0);
	m_pState_Handler->Set_Animation(STATE_PLAYER_ATTACK_AIR, 0);
	m_pState_Handler->Set_Animation(STATE_PLAYER_ATTACK_BASE_CHARGE, 0);
	m_pState_Handler->Set_Animation(STATE_PLAYER_ATTACK_JUMP_CHARGE, 0);
	m_pState_Handler->Set_Animation(STATE_PLAYER_ATTACK_ROLL, 0);

	m_pState_Handler->Set_Animation(STATE_PLAYER_SKILL_NORMAL, 0);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKILL_SPECIAL, 0);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKILL_NORMAL_AIR, 0);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SKILL_SPECIAL_AIR, 0);
#pragma endregion Weapon Base

#pragma region Sword
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_IDLE, 0);						//177

	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_A, 12);						//136
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_B, 13);						//140
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_C, 14);						//141
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_D, 15);						//144
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_E, 16);						//146
													
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_AIR_A, 25);					//159
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_AIR_B, 26);					//160
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_AIR_C, 27);					//161
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_AIR_D, 28);					//162

	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_SKILL_NORMAL, 29);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_SKILL_NORMAL_AIR, 179);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_SKILL_SPECIAL, 23);				// 17 ~ 21	
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_SKILL_SPECIAL_A, 17);			// 17 ~ 21
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_SKILL_SPECIAL_B, 18);			// 17 ~ 21
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_SKILL_SPECIAL_C, 19);			// 17 ~ 21
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_SKILL_SPECIAL_D, 20);			// 17 ~ 21
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_SKILL_SPECIAL_E, 21);			// 17 ~ 21		

	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ROLL_BACK, 7);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ROLL_BACK_AIR, 8);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ROLL_FORWORD, 9);			
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ROLL_FORWORD_AIR, 10);		
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ROLL_FORWORD_END, 11);	
																			
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_ROLL, 22);					//164
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_BASE_CHARGE, 23);			//147
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_A, 126);			//153
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_B, 127);			//153
	m_pState_Handler->Set_Animation(STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_C, 128);			//153
#pragma endregion Sword


#pragma region 2Gun
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_IDLE, 101);						//177

	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_A, 30);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_B, 31);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_C, 32);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_D, 33);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_E, 34);

	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_AIR_A, 35);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_AIR_B, 36);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_AIR_C, 37);

	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_A, 38);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_B, 39);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_C, 40);

	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_ZOOM_BEGIN, 45);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_ZOOM, 46);

	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_BEGIN, 47);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKFRONT, 48);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKBACK, 49);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKLEFT, 50);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKRIGHT, 51);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_IDLE, 52);

	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ATTACK_ROLL, 125);

	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_SKILL_NORMAL, 41);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_SKILL_NORMAL_AIR, 42);

	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_SKILL_SPECIAL, 44);
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_SKILL_SPECIAL_AIR, 43);

	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ROLL_BACK, 139);					//129//7
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ROLL_BACK_AIR, 140);				//130//8
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ROLL_FORWORD, 141);				//131//9
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ROLL_FORWORD_AIR, 142);			//132//10
	m_pState_Handler->Set_Animation(STATE_PLAYER_2GUN_ROLL_FORWORD_END, 143);			//133//11
#pragma endregion 2Gun


#pragma region ShieldAxe
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_IDLE, 99);						//177

	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_A, 64);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_B, 65);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_C, 66);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_D, 67);

	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_BASE_CHARGE_BEGIN, 72);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_BASE_CHARGE_LOOP, 74);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_BASE_CHARGE_END, 73);

	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_ROLL, 76);//	,
	
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_AIR_A, 78);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_AIR_B, 79);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_AIR_C, 80);

	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_A, 92);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_B, 94);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_C, 96);

	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_BEGIN, 87);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_LOOP, 86);
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_END, 85);

	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_SKILL_NORMAL, 84);//	
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ATTACK_SKILL_SPEAICAL, 98);//	

	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ROLL_BACK, 129);					//129//7
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ROLL_BACK_AIR, 130);				//130//8
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ROLL_FORWORD, 131);				//131//9
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ROLL_FORWORD_AIR, 132);			//132//10
	m_pState_Handler->Set_Animation(STATE_PLAYER_SHIELD_ROLL_FORWORD_END, 133);			//133//11

	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_IDLE, 100);						//177

	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_A, 68);
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_B, 69);
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_C, 70);
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_D, 71);

	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_BASE_CHARGE, 75);//	STATE_PLAYER_SHIELD_ATTACK_ROLL,

	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_ROLL, 77);//	
	
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_AIR_A, 81);
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_AIR_B, 82);
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_AIR_C, 83);

	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_SKILL_NORMAL_AIR_BEGIN, 91);
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_SKILL_NORMAL_AIR_LOOP, 90);
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_SKILL_NORMAL_AIR_END, 89);

	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_SKILL_NORMAL, 88);

	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_A, 93);
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_B, 95);
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_C, 97);

	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ROLL_BACK, 134);					//129//7
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ROLL_BACK_AIR, 135);				//130//8
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ROLL_FORWORD, 136);				//131//9
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ROLL_FORWORD_AIR, 137);			//132//10
	m_pState_Handler->Set_Animation(STATE_PLAYER_AXE_ROLL_FORWORD_END, 138);			//133//11

#pragma endregion ShieldAxe


#pragma region Cube
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_IDLE, 108);						//177

	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_A, 109);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_B, 110);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_C, 111);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_D, 112);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_E, 113);

	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_AIR_A, 115);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_AIR_B, 116);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_AIR_C, 117);

	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_BASE_CHARGE, 114);//	STATE_PLAYER_SHIELD_ATTACK_ROLL,

	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_BEGIN, 120);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_LOOP, 121);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_END, 122);

	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL, 123);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL_AIR, 124);

	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_SKILL_SPECIAL, 119);
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ATTACK_SKILL_SPECIAL_AIR, 118);

	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ROLL_BACK, 144);					//129//7
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ROLL_BACK_AIR, 145);				//130//8
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ROLL_FORWORD, 146);				//131//9
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ROLL_FORWORD_AIR, 147);			//132//10
	m_pState_Handler->Set_Animation(STATE_PLAYER_CUBE_ROLL_FORWORD_END, 148);			//133//11

#pragma endregion Cube


#pragma region UI
	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_APPEAR, 0);
	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_IDLE, 0);

	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_SWORD_APPEAR, 181);
	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_SWORD_IDLE, 182);

	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_2GUN_APPEAR, 185);
	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_2GUN_IDLE, 186);

	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_SHIELD_APPEAR, 183);
	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_SHIELD_IDLE, 184);

	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_CUBE_APPEAR, 180);
	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_CUBE_IDLE, 101);

	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_NONE_APPEAR, 187);
	m_pState_Handler->Set_Animation(STATE_PLAYER_UI_NONE_IDLE, 188);
#pragma endregion UI


	//m_pState_Handler->SetUp_StartState(STATE_PLAYER_SWIM_IDLE);
	m_pState_Handler->SetUp_StartState(STATE_PLAYER_HAND_IDLE);


	return S_OK;
}


void CPlayer::Input_Key(_float fTimeDelta)
{

}

void CPlayer::Change_Weapon(_byte iKey, WEAPON_SLOT eWeaponSlot)
{
	USEGAMEINSTANCE;
	m_iNextKey = iKey;
	m_eNextWeaponSlot = eWeaponSlot;
	m_eNextWeapon = m_eSlot_Weapon[m_eNextWeaponSlot];
}

void CPlayer::Change_Weapon_Slot()
{
	USEGAMEINSTANCE;
	WEAPON_TYPE eTemp = WEAPON_END;
	m_fTimeChangeWeapon[m_eNextWeaponSlot] = m_fCoolTime_ChangeWeapon[m_eNextWeaponSlot];

	m_iSlot_WeaponKey[m_eNextWeaponSlot] = m_iSlot_WeaponKey[WPN_SLOT_M];
	m_iSlot_WeaponKey[WPN_SLOT_M] = m_iNextKey;
	eTemp = m_eSlot_Weapon[WPN_SLOT_M];
	m_eCurWeapon = m_eSlot_Weapon[WPN_SLOT_M] = m_eSlot_Weapon[m_eNextWeaponSlot];

	if (WEAPON_SWORD == m_eSlot_Weapon[m_eNextWeaponSlot])
	{
		m_pCurWeapon_L = nullptr;
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_SWORD];
	}
	else if (WEAPON_2GUN == m_eSlot_Weapon[m_eNextWeaponSlot])
	{
		m_pCurWeapon_L = m_Weapons[PLAYER_WEAPON_GUN_L];
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_GUN_R];
	}
	else if (WEAPON_SHIELDAXE == m_eSlot_Weapon[m_eNextWeaponSlot])
	{
		((CWeapon_ShieldAxe*)m_Weapons[PLAYER_WEAPON_SHIELDAXE])->Reset_Weapon();
		m_pCurWeapon_L = nullptr;
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_SHIELDAXE];
	}
	else if (WEAPON_CUBE == m_eSlot_Weapon[m_eNextWeaponSlot])
	{
		_matrix OffsetMatrix = XMMatrixIdentity();
		OffsetMatrix.r[3] = XMVectorSet(0.7f, 1.5f, 0.f, 1.f);
		_matrix TargetMatrix = OffsetMatrix;
		_float4	vTargetPos;
		XMStoreFloat4(&vTargetPos, TargetMatrix.r[3]);
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_CUBE), ID_UINT(PLAYER, WEAPON_CUBE), &vTargetPos);
		m_pCurWeapon_L = m_Weapons[PLAYER_WEAPON_CUBE_L];
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_CUBE_R];
	}
	else
	{
		m_pCurWeapon_L = nullptr;
		m_pCurWeapon_R = nullptr;
	}

	m_eSlot_Weapon[m_eNextWeaponSlot] = eTemp;

	if (WPN_SLOT_L == m_eNextWeaponSlot)
		pGameInstance->Add_Event(EVENT_SKILLCHANGE_L, nullptr, nullptr);
	else
		pGameInstance->Add_Event(EVENT_SKILLCHANGE_R, nullptr, nullptr);
}

void CPlayer::Input_WeaponKey()
{
	USEGAMEINSTANCE;
	WEAPON_TYPE eTemp = WEAPON_END;
	if (KEY_DOWN('Q'))
	{
		if ('Q' == m_iSlot_WeaponKey[WPN_SLOT_L] && 1.f > m_fTimeChangeWeapon[WPN_SLOT_L] && WEAPON_NONE != m_eSlot_Weapon[WPN_SLOT_L])
		{
			Change_Weapon('Q', WPN_SLOT_L);
			//m_iSlot_WeaponKey[WPN_SLOT_L] = m_iSlot_WeaponKey[WPN_SLOT_M];
			//m_iSlot_WeaponKey[WPN_SLOT_M] = 'Q';
			//eTemp = m_eSlot_Weapon[WPN_SLOT_M];
			//m_eCurWeapon = m_eSlot_Weapon[WPN_SLOT_M] = m_eSlot_Weapon[WPN_SLOT_L];
			//m_eSlot_Weapon[WPN_SLOT_L] = eTemp;
			//pGameInstance->Add_Event(EVENT_SKILLCHANGE_L, nullptr, nullptr);
			m_bWeaponChange = true;
		}
		else if ('Q' == m_iSlot_WeaponKey[WPN_SLOT_R] && 1.f > m_fTimeChangeWeapon[WPN_SLOT_R] && WEAPON_NONE != m_eSlot_Weapon[WPN_SLOT_R])
		{
			Change_Weapon('Q', WPN_SLOT_R);
			//m_iSlot_WeaponKey[WPN_SLOT_R] = m_iSlot_WeaponKey[WPN_SLOT_M];
			//m_iSlot_WeaponKey[WPN_SLOT_M] = 'Q';
			//eTemp = m_eSlot_Weapon[WPN_SLOT_M];
			//m_eCurWeapon = m_eSlot_Weapon[WPN_SLOT_M] = m_eSlot_Weapon[WPN_SLOT_R];
			//m_eSlot_Weapon[WPN_SLOT_R] = eTemp;
			//pGameInstance->Add_Event(EVENT_SKILLCHANGE_R, nullptr, nullptr);
			m_bWeaponChange = true;
		}
	}
	if (KEY_DOWN('E'))
	{
		if ('E' == m_iSlot_WeaponKey[WPN_SLOT_L] && 1.f > m_fTimeChangeWeapon[WPN_SLOT_L] && WEAPON_NONE != m_eSlot_Weapon[WPN_SLOT_L])
		{
			Change_Weapon('E', WPN_SLOT_L);
			//m_iSlot_WeaponKey[WPN_SLOT_L] = m_iSlot_WeaponKey[WPN_SLOT_M];
			//m_iSlot_WeaponKey[WPN_SLOT_M] = 'E';
			//eTemp = m_eSlot_Weapon[WPN_SLOT_M];
			//m_eCurWeapon = m_eSlot_Weapon[WPN_SLOT_M] = m_eSlot_Weapon[WPN_SLOT_L];
			//m_eSlot_Weapon[WPN_SLOT_L] = eTemp;
			//pGameInstance->Add_Event(EVENT_SKILLCHANGE_L, nullptr, nullptr);
			m_bWeaponChange = true;

		}
		else if ('E' == m_iSlot_WeaponKey[WPN_SLOT_R] && 1.f > m_fTimeChangeWeapon[WPN_SLOT_R] && WEAPON_NONE != m_eSlot_Weapon[WPN_SLOT_R])
		{
			Change_Weapon('E', WPN_SLOT_R);
			//m_iSlot_WeaponKey[WPN_SLOT_R] = m_iSlot_WeaponKey[WPN_SLOT_M];
			//m_iSlot_WeaponKey[WPN_SLOT_M] = 'E';
			//eTemp = m_eSlot_Weapon[WPN_SLOT_M];
			//m_eCurWeapon = m_eSlot_Weapon[WPN_SLOT_M] = m_eSlot_Weapon[WPN_SLOT_R];
			//m_eSlot_Weapon[WPN_SLOT_R] = eTemp;
			//pGameInstance->Add_Event(EVENT_SKILLCHANGE_R, nullptr, nullptr);
			m_bWeaponChange = true;

		}
	}
	if (KEY_DOWN('R'))
	{
		if ('R' == m_iSlot_WeaponKey[WPN_SLOT_L] && 1.f > m_fTimeChangeWeapon[WPN_SLOT_L] && WEAPON_NONE != m_eSlot_Weapon[WPN_SLOT_L])
		{
			Change_Weapon('R', WPN_SLOT_L);
			//m_iSlot_WeaponKey[WPN_SLOT_L] = m_iSlot_WeaponKey[WPN_SLOT_M];
			//m_iSlot_WeaponKey[WPN_SLOT_M] = 'R';
			//eTemp = m_eSlot_Weapon[WPN_SLOT_M];
			//m_eCurWeapon = m_eSlot_Weapon[WPN_SLOT_M] = m_eSlot_Weapon[WPN_SLOT_L];
			//m_eSlot_Weapon[WPN_SLOT_L] = eTemp;
			//pGameInstance->Add_Event(EVENT_SKILLCHANGE_L, nullptr, nullptr);
			m_bWeaponChange = true;

		}
		else if ('R' == m_iSlot_WeaponKey[WPN_SLOT_R] && 1.f > m_fTimeChangeWeapon[WPN_SLOT_R] && WEAPON_NONE != m_eSlot_Weapon[WPN_SLOT_R])
		{
			Change_Weapon('R', WPN_SLOT_R);
			//m_iSlot_WeaponKey[WPN_SLOT_R] = m_iSlot_WeaponKey[WPN_SLOT_M];
			//m_iSlot_WeaponKey[WPN_SLOT_M] = 'R';
			//eTemp = m_eSlot_Weapon[WPN_SLOT_M];
			//m_eCurWeapon = m_eSlot_Weapon[WPN_SLOT_M] = m_eSlot_Weapon[WPN_SLOT_R];
			//m_eSlot_Weapon[WPN_SLOT_R] = eTemp;
			//pGameInstance->Add_Event(EVENT_SKILLCHANGE_R, nullptr, nullptr);
			m_bWeaponChange = true;
		}
	}
}

void CPlayer::Input_DashKey(_float fTimeDelta)
{
	/*USEGAMEINSTANCE;
	if ((KEY_DOWN(VK_RBUTTON) || KEY_DOWN(VK_SHIFT))
		&& 0.f < (((PLAYERDESC*)m_pCurInfo)->fDashGauge - ((PLAYERDESC*)m_pInfo)->fDashGauge / 3))
	{
		((PLAYERDESC*)m_pCurInfo)->fDashGauge -= ((PLAYERDESC*)m_pInfo)->fDashGauge / 3;
	}*/
	((PLAYERDESC*)m_pCurInfo)->fDashGauge += 0.4f * fTimeDelta;
	if (((PLAYERDESC*)m_pInfo)->fDashGauge < ((PLAYERDESC*)m_pCurInfo)->fDashGauge)
	{
		((PLAYERDESC*)m_pCurInfo)->fDashGauge = ((PLAYERDESC*)m_pInfo)->fDashGauge;
	}
}

void CPlayer::Set_EnableKeyInput(_bool bEnableKeyInput)
{
	if (bEnableKeyInput)
	{
		m_iReserveEnableKeyInput = 0;
	}
	else
	{
		m_bEnableKeyInput = bEnableKeyInput;
	}
}

void CPlayer::SetUp_Cur_HandWeapon()
{
	if (WEAPON_SWORD == m_eSlot_Weapon[WPN_SLOT_M])
	{
		m_pCurWeapon_L = nullptr;
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_SWORD];
	}
	else if (WEAPON_2GUN == m_eSlot_Weapon[WPN_SLOT_M])
	{
		m_pCurWeapon_L = m_Weapons[PLAYER_WEAPON_GUN_L];
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_GUN_R];
	}
	else if (WEAPON_SHIELDAXE == m_eSlot_Weapon[WPN_SLOT_M])
	{
		((CWeapon_ShieldAxe*)m_Weapons[PLAYER_WEAPON_SHIELDAXE])->Reset_Weapon();
		m_pCurWeapon_L = nullptr;
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_SHIELDAXE];
	}
	else if (WEAPON_CUBE == m_eSlot_Weapon[WPN_SLOT_M])
	{
		/*_matrix OffsetMatrix = XMMatrixIdentity();
		OffsetMatrix.r[3] = XMVectorSet(0.7f, 1.5f, 0.f, 1.f);
		_matrix TargetMatrix = OffsetMatrix;
		_float4	vTargetPos;
		XMStoreFloat4(&vTargetPos, TargetMatrix.r[3]);
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_CUBE), ID_UINT(PLAYER, WEAPON_CUBE), &vTargetPos);*/
		m_pCurWeapon_L = m_Weapons[PLAYER_WEAPON_CUBE_L];
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_CUBE_R];
	}
}

void CPlayer::Keep_RealCellIdx()
{
m_iRealCellIdx = m_pNaviCom->Get_CurCellIdx();
}

void CPlayer::SetUp_RealCellIdx()
{
	m_pNaviCom->Set_CurCellDesc(m_iRealCellIdx);
}

void CPlayer::Set_AddCubeBullet()
{
	list<CGameObject*>* CubeBullets = (GAMEINSTANCE->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_CUBE_BULLET)));

	if (nullptr == CubeBullets || 5 > CubeBullets->size())
		++m_iCubeHitCnt;

	if (5 < m_iCubeHitCnt)
	{
		_matrix OffsetMatrix = XMMatrixIdentity();
		OffsetMatrix.r[3] = XMVectorSet(0.f, 1.2f, 0.f, 1.f);
		_matrix TargetMatrix = OffsetMatrix;
		_float4	vTargetPos;
		XMStoreFloat4(&vTargetPos, TargetMatrix.r[3]);
		GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_CUBE_BULLET), ID_UINT(PLAYER, WEAPON_CUBE_BULLET), &vTargetPos);
		m_bCubeBulletCreate = true;
		m_iCubeHitCnt = 0;
	}
}

void CPlayer::Healing()
{
	USEGAMEINSTANCE;
	_uint iTotalHealNum = CItem_Manager::Get_Instance()->Use_Food();
	m_fPreHp = m_pCurInfo->fHP;
	m_pCurInfo->fHP += iTotalHealNum;
	if (m_pInfo->fHP < m_pCurInfo->fHP)
	{
		m_pCurInfo->fHP = m_pInfo->fHP;
	}
	pGameInstance->Add_Event(EVENT_HPCHANGE, this, &m_fPreHp);

	_uint iHeal = 0;
	list<_uint> Heallist;
	Heallist.clear();
	//while (0 != iTotalHealNum)
	while (true)
	{
		iHeal = iTotalHealNum % 10;
		Heallist.push_back(iHeal);

		if (iTotalHealNum < 10)
			break;

		iTotalHealNum /= 10;
	}

	_uint iIdx = 0;
	Heallist.reverse();
	for (auto& Num : Heallist)
	{
#ifdef _UI
		NULLCHECK(pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_DAMAGE_NUM), ID_UINT(UI, UI_HEAL_NUM),
			&CUI::UIDesc(200.f, 200.f, 200.f, 200.f, 0.f, Num, 300, { 20.f * iIdx, 0.f }, this, iIdx)));
#endif // _UI
		iIdx++;
	}

	Create_UI_Alert(TEXT("사용 성공"));
}

void CPlayer::Input_StaminaKey(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	_float fHightDiff = 0.f;
	m_pNaviCom->isUpperThanPlane(m_pTransformCom, &fHightDiff);
	STATE_TYPE eStateType = m_pState_Handler->Get_CurStateType();

	if (STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_ZOOM != eStateType &&
		STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_BEGIN != eStateType &&
		STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_IDLE != eStateType &&
		STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKBACK != eStateType &&
		STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKFRONT != eStateType &&
		STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKLEFT != eStateType &&
		STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKRIGHT != eStateType)
	{
		m_fStaminaTime += fTimeDelta;
		if (0.2f < m_fStaminaTime && 1.f > fHightDiff)
		{
			m_fStaminaTime = 0.f;
			m_fPreStamina = ((PLAYERDESC*)m_pCurInfo)->fStaminaGauge;
			((PLAYERDESC*)m_pCurInfo)->fStaminaGauge += ((PLAYERDESC*)m_pInfo)->fStaminaGauge / 20;
			pGameInstance->Add_Event(EVENT_STAMINACHANGE, &m_fPreStamina, &((PLAYERDESC*)m_pCurInfo)->fStaminaGauge);
		}
		if (((PLAYERDESC*)m_pInfo)->fStaminaGauge < ((PLAYERDESC*)m_pCurInfo)->fStaminaGauge)
		{
			((PLAYERDESC*)m_pCurInfo)->fStaminaGauge = ((PLAYERDESC*)m_pInfo)->fStaminaGauge;
		}
	}
	/*if (STATE_PLAYER_HAND_IDLE == eStateType 
		|| STATE_PLAYER_WALK == eStateType 
		|| STATE_PLAYER_RUN == eStateType)
	{
		m_fStaminaTime += fTimeDelta;
		if (0.2f < m_fStaminaTime)
		{
			m_fStaminaTime = 0.f;
			m_fPreStamina = ((PLAYERDESC*)m_pCurInfo)->fStaminaGauge;
			((PLAYERDESC*)m_pCurInfo)->fStaminaGauge += ((PLAYERDESC*)m_pInfo)->fStaminaGauge / 20;
			pGameInstance->Add_Event(EVENT_STAMINACHANGE, &m_fPreStamina, &((PLAYERDESC*)m_pCurInfo)->fStaminaGauge);
		}
		if (((PLAYERDESC*)m_pInfo)->fStaminaGauge < ((PLAYERDESC*)m_pCurInfo)->fStaminaGauge)
		{
			((PLAYERDESC*)m_pCurInfo)->fStaminaGauge = ((PLAYERDESC*)m_pInfo)->fStaminaGauge;
		}
	}*/
	if (0.f > ((PLAYERDESC*)m_pCurInfo)->fStaminaGauge)
	{
		((PLAYERDESC*)m_pCurInfo)->fStaminaGauge = 0.f;
	}
}

void CPlayer::SetUp_SkillGauge()
{
	if (((PLAYERDESC*)m_pCurInfo)->bAbleSkill_S
		&& ((PLAYERDESC*)m_pInfo)->fSkillGauge * 0.98f <= ((PLAYERDESC*)m_pCurInfo)->fSkillGauge)
	{
		((PLAYERDESC*)m_pCurInfo)->fSkillGauge = ((PLAYERDESC*)m_pInfo)->fSkillGauge * 0.98f;
	}
	else if (((PLAYERDESC*)m_pInfo)->fSkillGauge <= ((PLAYERDESC*)m_pCurInfo)->fSkillGauge)
	{
		m_bResetSkillGauge = true;
		((PLAYERDESC*)m_pCurInfo)->bAbleSkill_S = true;
		((PLAYERDESC*)m_pCurInfo)->fSkillGauge = 0.f;
	}
}

void CPlayer::SetUp_CoolTime(_float fTimeDelta)
{
	for (_uint i = 0; i < WPN_SLOT_END; i++)
	{
		m_fTimeChangeWeapon[i] -= fTimeDelta;
		if (0.f > m_fTimeChangeWeapon[i])
		{
			m_fTimeChangeWeapon[i] = 0.f;
		}
	}
	for (_uint i = 0; i < WEAPON_HAND1; i++)
	{
		m_fTimeSkill_N[i]-= fTimeDelta;
		if (0.f > m_fTimeSkill_N[i])
		{
			m_fTimeSkill_N[i] = 0.f;
		}
	}
	if (1.f > m_fTimeSkill_N[m_eSlot_Weapon[WPN_SLOT_M]])
	{
		((PLAYERDESC*)m_pCurInfo)->bAbleSkill_N = true;
	}
	else if (1.f <= m_fTimeSkill_N[m_eSlot_Weapon[WPN_SLOT_M]])
	{
		((PLAYERDESC*)m_pCurInfo)->bAbleSkill_N = false;
	}

}

void CPlayer::Gain_Item()
{
	USEGAMEINSTANCE;

	list<CItem*> Itemlist;
	CItem*		 pNearestItem = nullptr;

	_vector		 vPlayerPos = Get_Pos();
	_vector		 vItemPos, vDir;
	_float		 fDist, fNearDist = 100.f;

	ITEM_TYPE	 eItemType = ITEM_END;
	wstring		 strItemName = L"";
	wstring		 strText = L"";

	for (auto& pTargetCol : *m_pCol_UIIntrc->Get_StayColliders())
	{
		CItem* pItem = dynamic_cast<CItem*>(pTargetCol->Get_Owner());
		if (pItem)
		{
			Itemlist.push_back(pItem);
		}
	}
	for (auto& pItem : Itemlist)
	{
		vItemPos = pItem->Get_Pos();
		vDir = vPlayerPos - vItemPos;
		fDist = XMVectorGetX(XMVector3Length(vDir));
		if (fNearDist > fDist)
		{
			fNearDist = fDist;
			pNearestItem = pItem;
		}
	}

	if (pNearestItem)
	{
		eItemType = pNearestItem->Get_ItemType();
		switch (eItemType)
		{
		case ITEM_MUSHROOM:
			strItemName = TEXT("버섯");
			strText = TEXT("버섯x1 획득");
			break;
		case ITEM_CACTUS:
			strItemName = TEXT("선인장 볼");
			strText = TEXT("선인장 볼x1 획득");
			break;
		case ITEM_BEET:
			strItemName = TEXT("비트");
			strText = TEXT("비트x1 획득");
			break;
		case ITEM_BRACKEN:
			strItemName = TEXT("고사리");
			strText = TEXT("고사리x1 획득");
			break;
		case ITEM_WHEAT:
			strItemName = TEXT("호미키밀");
			strText = TEXT("호미키밀x1 획득");
			break;
		case ITEM_LETTUCE:
			strItemName = TEXT("상추");
			strText = TEXT("상추x1 획득");
			break;
		case ITEM_SHELL:
			strItemName = TEXT("호리조개");
			strText = TEXT("호리조개x1 획득");
			break;
		case ITEM_HERMITCRAB:
			strItemName = TEXT("소라게");
			strText = TEXT("소라게x1 획득");
			break;
		}

		if (!m_GuideWindow_Handlist.size())
		{
			json Data;
			pGameInstance->Load_Json(pGameInstance->json_Complete_Path(TEXT("UI/GuideWindow_Hand")).c_str(), &Data);
			_uint iPrototypeID;

			for (auto& elem : Data)
			{
#if defined(_UI)
				CUI::UIDesc UIDesc;
				string strTemp = "";
				iPrototypeID = elem["iPrototypeID"];
				UIDesc.fInitX = elem["fInitX"];
				UIDesc.fInitY = elem["fInitY"];
				UIDesc.fInitSizeX = elem["fInitSizeX"];
				UIDesc.fInitSizeY = elem["fInitSizeY"];
				UIDesc.fAngle = elem["fAngle"];
				UIDesc.iTextureIdx = elem["iTextureIdx"];
				UIDesc.iRenderLayer = elem["iRenderLayer"];
				strTemp = elem["strText"];
				UIDesc.vFontPos.x = elem["vFontPosx"];
				UIDesc.vFontPos.y = elem["vFontPosy"];
				UIDesc.vFontColor.x = elem["vFontColorx"];
				UIDesc.vFontColor.y = elem["vFontColory"];
				UIDesc.vFontColor.z = elem["vFontColorz"];
				UIDesc.vFontColor.w = elem["vFontColorw"];
				UIDesc.fFontSize = elem["fFontSize"];
				UIDesc.bRenderFontText = elem["bRenderFontText"];
				UIDesc.bRenderFontNum = elem["bRenderFontNum"];

				ID_UINT IDPrototype;
				IDPrototype.iID = iPrototypeID;
				CGameObject* pGameObject;
				NULLCHECK(pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, IDPrototype, IDPrototype, &UIDesc));
				CUI* pUI = (CUI*)pGameObject;
				pUI->Set_Delfault_FontText(strItemName);
				m_GuideWindow_Handlist.push_back(pUI);
#endif
			}
		}
		if (KEY_DOWN('F'))
		{
			MGR(CSound_Manager)->PlaySound(TEXT("Gain_Item.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
			Create_UI_Alert(strText, eItemType, true);
			CItem_Manager::Get_Instance()->Add_Item(eItemType);
			pNearestItem->Excute_Effect();
			pGameInstance->Add_DeadObject(pNearestItem);
			for (auto& elem : m_GuideWindow_Handlist)
			{
				pGameInstance->Add_PoolingObject(elem);
			}
			m_GuideWindow_Handlist.clear();
		}
	}
	else if (nullptr == pNearestItem)
	{
		if (m_GuideWindow_Handlist.size())
		{
			for (auto& elem : m_GuideWindow_Handlist)
			{
				pGameInstance->Add_PoolingObject(elem);
			}
			m_GuideWindow_Handlist.clear();
		}
	}
}

void CPlayer::Create_UI_Enemy_Indicator()
{
#if defined(_UI)
	for (auto& pTargetCol : *m_pCol_UIIntrc->Get_EnterColliders())
	{
		CEnemy_Normal* pEnemy = dynamic_cast<CEnemy_Normal*>(pTargetCol->Get_Owner());
		if (pEnemy)
		{
			CUI_Solid* pUI_Solid = nullptr;
			pUI_Solid = (CUI_Solid*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_INDICATOR), ID_UINT(UISOLID, UISOLID_ENEMY_INDICATOR),
				&CUI_Solid::UISolidDesc(pEnemy, 0));
			pEnemy->Set_IsPlayer_InRange(true);
			m_Enemy_Indicatorlist.push_back(pUI_Solid);
		}
	}
	for (auto& pTargetCol : *m_pCol_UIIntrc->Get_ExitColliders())
	{
		CEnemy_Normal* pEnemy = dynamic_cast<CEnemy_Normal*>(pTargetCol->Get_Owner());
		
		if (pEnemy && !pEnemy->Is_Dead())
		{
			pEnemy->Set_IsPlayer_InRange(false);
		}
	}
	for (auto iter = m_Enemy_Indicatorlist.begin(); iter != m_Enemy_Indicatorlist.end();)
	{
		if ((*iter)->Get_Object()->Is_Dead()
			|| false == ((CEnemy_Normal*)(*iter)->Get_Object())->Get_IsPlayer_InRange())
		{
			GAMEINSTANCE->Add_DeadObject(*iter);
			iter = m_Enemy_Indicatorlist.erase(iter);
		}
		else
		{
			++iter;
		}
	}
#endif
}

void CPlayer::Create_UI_Alert(wstring strText, _uint iTextureIdx, _bool bIcon)
{
	CUI* pUI = nullptr;
	pUI = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ALERT_WINDOW), ID_UINT(UI, UI_ALERT_WINDOW),
		&CUI::UIDESC(600.f, 600.f, 400.f, 51.5f, 0.f, 0, 500.f, { 0.f,0.f }, nullptr, 0, { 465.f,586.f }, { 1.f,1.f,1.f,1.f }, 0.29f));

	if (bIcon)
	{
		(CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ALERT_ICON), ID_UINT(UI, UI_ALERT_ICON),
			&CUI::UIDESC(447.f, 602.f, 45.f, 45.f, 0.f, iTextureIdx, 490.f));
	}
	pUI->Set_Delfault_FontText(strText);
}

void CPlayer::Push_Other(CCollider* pTargetCol, CCollider* pVolumeCol, CTransform* pTargetTransform, CNavigation* pTargetNavigation)
{
	_float fTargetColRadius = pTargetCol->Get_ColliderDesc()->vScale.x * 0.5f;
	_float fColRadius = pVolumeCol->Get_ColliderDesc()->vScale.x * 0.5f;

	CGameObject* pTarget = pTargetCol->Get_Owner();

	_vector vTargetPos = pTargetCol->Get_Owner()->Get_Pos();
	_vector vPos = pVolumeCol->Get_Owner()->Get_Pos();
	_vector vDir = vPos - vTargetPos;
	_vector vTargetDir = vTargetPos - vPos;

	_float fLength = XMVectorGetX(XMVector3Length(vDir));
	_float fPushLength = (fColRadius + fTargetColRadius - fLength) * 0.5f;

	vDir = XMVector3Normalize(vDir) * fPushLength;
	vTargetDir = XMVector3Normalize(vTargetDir) * fPushLength;

	vPos += vDir;
	vTargetPos += vTargetDir;

	//pTargetTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	//((CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM)))->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
	
	pTargetTransform->MoveTo(vPos, pTargetNavigation);
	CNavigation* pTargetNaviCom = dynamic_cast<CNavigation*>(pTarget->Get_Component(ID_UINT(NAVIGATION)));
	((CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM)))->MoveTo(vTargetPos, pTargetNaviCom);
}

void CPlayer::Set_CubeBulletPos()
{
	list<CGameObject*>* CubeBullets = (GAMEINSTANCE->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_CUBE_BULLET)));
	if (nullptr == CubeBullets || 0 == CubeBullets->size())
		return;

	_float fDgree = 360 / (*CubeBullets).size();
	_matrix		TargetRotationMatrix, TargetCurRotationMatrix;
	_matrix		TargetFinalRotationMatrix;
	_matrix		TargetWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	_uint i = 0;
	_float fLerpTime = 0.9f;

	for (auto& iter : *CubeBullets)
	{		
		_float4 vPos, vCurPos, vFinalPos;

		TargetRotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(i * fDgree));
		TargetRotationMatrix.r[3] += XMVectorSet(0.f, 0.8f, 0.f, 0.f) + 1.3f * XMVector3Normalize(TargetRotationMatrix.r[2]);

		TargetFinalRotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(i * fDgree) + m_fCubeLerpTime);
		TargetFinalRotationMatrix.r[3] += XMVectorSet(0.f, 0.8f, 0.f, 0.f) + 1.3f * XMVector3Normalize(TargetFinalRotationMatrix.r[2]);

		TargetCurRotationMatrix = TargetFinalRotationMatrix * TargetWorldMatrix;
		TargetCurRotationMatrix.r[3] = XMVectorSet(0.f, 0.8f, 0.f, 0.f) + 1.3f * XMVector3Normalize(TargetCurRotationMatrix.r[2]);

		XMStoreFloat4(&vPos, TargetCurRotationMatrix.r[3]);

		TargetRotationMatrix *= TargetWorldMatrix;
		TargetFinalRotationMatrix *= TargetWorldMatrix;

		XMStoreFloat4(&vCurPos, TargetRotationMatrix.r[3]);
		XMStoreFloat4(&vFinalPos, TargetFinalRotationMatrix.r[3]);

		if(i != m_iCubeBulletCnt)
		{
			if (!m_bCubeBulletGetPos[i])
			{
				XMStoreFloat4(&m_vCubePos[i], iter->Get_Pos());
				m_bCubeBulletGetPos[i] = true;
			}
		}
		else
		{
			if (!m_bCubeBulletGetPos[i])
			{
				m_vCubePos[i] = vCurPos;
				m_bCubeBulletGetPos[i] = true;
			}
		}

		if (m_fCubeLerpTime > 1.f)
		{
			((CBullet_Player_Cube*)iter)->Reset_Turn();
			((CBullet_Player_Cube*)iter)->Set_StartPos(vPos);

			m_bCubeBulletCreate = false;
		}
		else
		{
			_vector vLerpPos = CEasing_Utillity::QuinticInOut(XMLoadFloat4(&m_vCubePos[i]), XMLoadFloat4(&vFinalPos), m_fCubeLerpTime, fLerpTime);

			iter->Set_Pos(vLerpPos);
		}

		++i;
	}
}

void CPlayer::Set_NearTarget()
{
	m_pTarget = nullptr;
	for (auto& iter : *m_pCol_Range->Get_StayColliders())
	{
		if (nullptr != m_pTarget )
		{
			_float fDistance = XMVectorGetX(XMVector3Length((iter->Get_Owner())->Get_Pos() - Get_Pos()));
			_float fTargetDis = XMVectorGetX(XMVector3Length(m_pTarget->Get_Pos() - Get_Pos()));

			if (fDistance < fTargetDis)
				m_pTarget = iter->Get_Owner();
		}
		else
		{
			m_pTarget = iter->Get_Owner();
		}

		if (0 >= ((CUnit*)iter->Get_Owner())->Get_UnitCurInfo()->fHP)
		{
			m_pTarget = nullptr;
			continue;
		}
	}
}

void CPlayer::Set_WeaponShadow(CLight* pLight)
{
	for (auto& pWeapon : m_Weapons)
	{
		if(pWeapon)
			pLight->Add_RenderObject(pWeapon);
	}
}

void CPlayer::Reset_CubeBullet()
{ 
	m_iCubeHitCnt = 0;
	m_iCubeBulletCnt = 0;
}

void CPlayer::Reset_WeaponChange()
{
	USEGAMEINSTANCE;

	if (m_eCurWeapon != m_eNextWeapon/* && !m_bInEquip_Window*/)
	{
		Change_Weapon_Slot();
	}

	m_bWeaponChange = false;
}

void CPlayer::SetUp_OwnWeapon(WEAPON_TYPE eWeaponType)
{
	for (_uint i = 0; i < 4; ++i)
	{
		if (WEAPON_NONE == m_eOwnWeapons[i])
		{
			m_eOwnWeapons[i] = eWeaponType;
			break;
		}
	}

	if (m_eOwnWeapons[1] == m_eOwnWeapons[2])
	{
		m_eOwnWeapons[2] = WEAPON_NONE;
	}
	if (m_eOwnWeapons[2] == m_eOwnWeapons[3])
	{
		m_eOwnWeapons[3] = WEAPON_NONE;
	}
	if (m_eOwnWeapons[1] == m_eOwnWeapons[3])
	{
		m_eOwnWeapons[3] = WEAPON_NONE;
	}
}

void CPlayer::SetUp_UI_Weapon(_uint iWeaponType)
{
	switch (iWeaponType)
	{
	case WEAPON_SWORD:
		m_pCurWeapon_L = nullptr;
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_SWORD];
		break;
	case WEAPON_2GUN:
		m_pCurWeapon_L = m_Weapons[PLAYER_WEAPON_GUN_L];
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_GUN_R];
		break;
	case WEAPON_SHIELDAXE:
		((CWeapon_ShieldAxe*)m_Weapons[PLAYER_WEAPON_SHIELDAXE])->Set_Weapon(1);
		m_pCurWeapon_L = nullptr;
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_SHIELDAXE];
		break;
	case WEAPON_CUBE:
		m_pCurWeapon_L = m_Weapons[PLAYER_WEAPON_CUBE_L];
		m_pCurWeapon_R = m_Weapons[PLAYER_WEAPON_CUBE_R];
		break;
	case WEAPON_NONE:
		m_pCurWeapon_L = nullptr;
		m_pCurWeapon_R = nullptr;
		break;
	default:
		__debugbreak();
		break;
	}
}

CPlayer * CPlayer::Create()
{
	CPlayer*		pInstance = new CPlayer();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	if (m_bClone)
	{
		for (auto& pWeapon : m_Weapons)
		{
			Safe_Release(pWeapon);
		}
	}

	USEGAMEINSTANCE;
	pGameInstance->Remove_CallbackList(this, EVENT_PLAYER_DAMAGED);

}
