#include "stdafx.h"
#include "..\Public\HunterDog.h"
#include "State_Handler.h"
#include "Navigation.h"

#include "GameInstance.h"

/*사냥개*/
CHunterDog::CHunterDog()
	: CEnemy_Normal()
{
}

CHunterDog::CHunterDog(const CHunterDog& rhs)
	: CEnemy_Normal(rhs)
{
}

HRESULT CHunterDog::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHunterDog::Initialize(void* pArg)
{
	m_fFrustumRange = 2.5f;

	FAILEDCHECK(__super::Initialize(pArg));

	if (FAILED(SetUp_Components()))
		__debugbreak();

	//if(pArg)
	//	memcpy(&m_pInfo, pArg, sizeof(ENEMYDESC));

	USEGAMEINSTANCE;


	//m_pTransformCom->Set_Scaled(_float3(m_pInfo.vScale.x, m_pInfo.vScale.y, m_pInfo.vScale.z));
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_pInfo.vPosition));
	//m_pModelCom->Set_CurrentAnimation(m_iAnimNum);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vStartPos));

	FAILEDCHECK(SetUp_State_Handler());
	FAILEDCHECK(Create_UI_Status(9));

	return S_OK;
}

void CHunterDog::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());

	m_pState_Handler->Tick(fTimeDelta);

	//USEGAMEINSTANCE;
	//if (KEY_DOWN(VK_NUMPAD5))
	//{
	//	m_pCurInfo->fHP -= 10.f;
	//}

	SetUp_NonAttacked(fTimeDelta);
}

void CHunterDog::LateTick(_float fTimeDelta)
{
#ifndef _MAPTOOL
	if (!m_pTransformCom->Is_InAir())
		m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);
#endif

	USEGAMEINSTANCE;
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_HITRANGE), m_pCol_Sphere);
	if (IsInFrustum())
		pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CHunterDog::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

		m_pModelCom->Render(i, m_pShaderCom, 0, "g_Bones");
	}

#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif // _DEBUG

	return S_OK;
}


HRESULT CHunterDog::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, HUNTERDOG), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
		return E_FAIL;
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);
	return S_OK;
}

HRESULT CHunterDog::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	return S_OK;
}
HRESULT CHunterDog::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_IDLE_NORMAL, 26);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_IDLE_ACT_A, 29);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_IDLE_BATTLE, 3);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_RUN, 27);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_RUN_RETURN, 27);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_WALK, 24);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_WALK_B, 30);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_WALK_L, 32);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_WALK_R, 31);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_WALK_TURN_L, 22);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_WALK_TURN_R, 23);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_ATTACK_BITE, 1);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_ATTACK_FIND_DASH_BITE, 0);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_DAMAGE_B, 9);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_DAMAGE_B_BACK, 8);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_DAMAGE_L, 19);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_DAMAGE_R, 20);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_DEAD_1, 21);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HUNTERDOG_DEAD_END, 21);



	m_pState_Handler->SetUp_StartState(STATE_ENEMY_HUNTERDOG_IDLE_NORMAL);

	return S_OK;
}

CHunterDog* CHunterDog::Create()
{
	CHunterDog* pInstance = new CHunterDog();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHunterDog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHunterDog::Clone(void* pArg)
{
	CHunterDog* pInstance = new CHunterDog(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHunterDog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHunterDog::Free()
{
	__super::Free();
}
