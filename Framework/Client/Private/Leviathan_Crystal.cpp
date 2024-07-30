#include "stdafx.h"
#include "Leviathan_Crystal.h"
#include "Navigation.h"

#include "GameInstance.h"
#include "UI_PlaceMark.h"
#include "UI_Solid.h"
#include "Quest_Manager.h"


#include "Effect_Base.h"
#include "Imgui_Manager.h"

CLeviathan_Crystal::CLeviathan_Crystal()
	: CEnemy_Normal()
{
}

CLeviathan_Crystal::CLeviathan_Crystal(const CLeviathan_Crystal& rhs)
	: CEnemy_Normal(rhs)
{
}

HRESULT CLeviathan_Crystal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLeviathan_Crystal::Initialize(void* pArg)
{
	m_fFrustumRange = 2.f;
	FAILEDCHECK(__super::Initialize(pArg));

	if (FAILED(SetUp_Components()))
		__debugbreak();
	//FAILEDCHECK(Create_UI_Status(0, 1.15f));

	USEGAMEINSTANCE;
	/*pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR), ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR),
		&CUI_Solid::UISolidDesc(this, 3, { 0.f, 5.5f }));
	pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR), ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR),
		&CUI_Solid::UISolidDesc(this, 5, { 0.f, 5.5f }));
	pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR), ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR),
		&CUI_Solid::UISolidDesc(this, 4, { 0.f, 5.5f }));*/

	Create_UI_Place_Mark(4.f);

	for (auto& pPlaceMark : m_PlaceMarklist)
	{
		((CUI_PlaceMark*)pPlaceMark)->Set_Able(true);
		((CUI_PlaceMark*)pPlaceMark)->Set_GameObject(this);
		((CUI_PlaceMark*)pPlaceMark)->Set_Offset_TargetPosY(6.f);
	}
	
	return S_OK;
}

void CLeviathan_Crystal::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (true == m_bDead)
		return;

	IMPL_ONCE(m_bEffect1,
		EFFECT_POS("Boss_Leviathan_Crystal_Create", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 3.f, 0.f, 1.f));
	);

	if (0 >= m_pCurInfo->fHP)
	{
		Pooling_UI_PlaceMark();
		pGameInstance->Add_DeadObject(this);
		EFFECT_POS("Boss_Leviathan_Crystal_Dead", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 3.f, 0.f, 1.f));
	}

	m_fTimeAcc_Effect0 += fTimeDelta;
	if (5.f <= m_fTimeAcc_Effect0)
	{
		IMPL_ONCE(m_bEffect0,
			EFFECT_EXCUTE("Boss_Leviathan_Crystal_Loop", XMMatrixTranslation(0.f, 3.f, 0.f), 1.f, this, CEffect_Manager::FOLLOW_POS, "", &m_Effects);
		);
	}


	__super::Tick(fTimeDelta);

	m_pCol_Sphere[COL_ENEMY_BOSS_VOLUME]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Sphere[COL_ENEMY_HITRANGE]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Tick(fTimeDelta);

	for (auto& iter : *m_pCol_Sphere[COL_ENEMY_BOSS_VOLUME]->Get_StayColliders())
	{
		Push_EachOther(iter, m_pCol_Sphere[COL_ENEMY_BOSS_VOLUME], m_pTransformCom);
	}

	//퀘스트있을때 PlaceMark 생성
	/*if (QUEST_KILL_GANG_SWARM == CQuest_Manager::Get_Instance()->Get_CurQuset())
	{
		IMPL_ONCE(m_bCreate_PlaceMark, Create_UI_Place_Mark(2.f));
	}
	if (m_pCurInfo->fHP <= 0)
	{
		Pooling_UI_PlaceMark();
	}*/
}

void CLeviathan_Crystal::LateTick(_float fTimeDelta)
{
#ifndef _MAPTOOL
	if (!m_pTransformCom->Is_InAir())
		m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);
#endif

	USEGAMEINSTANCE;
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_BOSS_VOLUME), m_pCol_Sphere[COL_ENEMY_BOSS_VOLUME]);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_HITRANGE), m_pCol_Sphere[COL_ENEMY_HITRANGE]);

	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
}

void CLeviathan_Crystal::On_Dead()
{
	USEGAMEINSTANCE;
	pGameInstance->Add_Event(EVENT_MONSTER_DEAD, this, nullptr);

	for (auto& Effect : m_Effects)
		Effect->Set_EffectOff();

}

HRESULT CLeviathan_Crystal::Render()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

#if 0
	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

		m_pModelCom->Render(i, m_pShaderCom, 0, "g_Bones");
	}
#endif // 0

#ifdef _DEBUG
	m_pCol_Sphere[COL_ENEMY_BOSS_VOLUME]->Render();
	m_pCol_Sphere[COL_ENEMY_HITRANGE]->Render();
#endif // _DEBUG

#if 0
	_bool bDissolve = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDissolve", &bDissolve, sizeof(_bool)));

	_bool bUseRimLight = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bRimLight", &bUseRimLight, sizeof(_bool)));
#endif // 0

	return S_OK;
}

HRESULT CLeviathan_Crystal::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_BOSS_VOLUME), (CComponent**)&m_pCol_Sphere[COL_ENEMY_BOSS_VOLUME], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_BOSS_VOLUME]->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_HITRANGE), (CComponent**)&m_pCol_Sphere[COL_ENEMY_HITRANGE], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_HITRANGE]->Set_Owner(this);

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
		return E_FAIL;
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);

	return S_OK;
}

void CLeviathan_Crystal::Push_EachOther(CCollider* pTargetCol, CCollider* pVolumeCol, CTransform* pTargetTransform)
{
	_float fTargetColRadius = pTargetCol->Get_ColliderDesc()->vScale.x * 0.5f;
	_float fColRadius = pVolumeCol->Get_ColliderDesc()->vScale.x * 0.5f;

	CGameObject* pTarget = pTargetCol->Get_Owner();

	_vector vTargetPos = pTargetCol->Get_Owner()->Get_Pos();
	_vector vPos = pVolumeCol->Get_Owner()->Get_Pos();
	_vector vDir = vPos - vTargetPos;
	_vector vTargetDir = vTargetPos - vPos;

	_float fLength = XMVectorGetX(XMVector3Length(vDir));
	_float fPushLength = (fColRadius + fTargetColRadius - fLength);

	vDir = XMVector3Normalize(vDir) * fPushLength;
	vTargetDir = XMVector3Normalize(vTargetDir) * fPushLength;

	vTargetPos += vTargetDir;

	//((CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM)))->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
	CNavigation* pTargetNaviCom = dynamic_cast<CNavigation*>(pTarget->Get_Component(ID_UINT(NAVIGATION)));
	((CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM)))->MoveTo(vTargetPos, pTargetNaviCom);
}

CLeviathan_Crystal* CLeviathan_Crystal::Create()
{
	CLeviathan_Crystal* pInstance = new CLeviathan_Crystal();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLeviathan_Crystal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeviathan_Crystal::Clone(void* pArg)
{
	CLeviathan_Crystal* pInstance = new CLeviathan_Crystal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLeviathan_Crystal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeviathan_Crystal::Free()
{
	__super::Free();
}
