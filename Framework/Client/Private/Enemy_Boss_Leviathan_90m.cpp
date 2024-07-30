#include "stdafx.h"
#include "State_Handler.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Boss_Leviathan_90m.h"

#include "Navigation.h"

CEnemy_Boss_Leviathan_90m::CEnemy_Boss_Leviathan_90m()
	: CEnemy_Boss()
{
}

CEnemy_Boss_Leviathan_90m::CEnemy_Boss_Leviathan_90m(const CEnemy_Boss& rhs)
	: CEnemy_Boss(rhs)
{
}

HRESULT CEnemy_Boss_Leviathan_90m::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Boss_Leviathan_90m::Initialize(void* pArg)
{
	m_fFrustumRange = 5.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();
	
	// 실제 위치
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(138.44f, 22.39f, 118.12f, 1.f));
	//// 테스트 위치
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(320.f, 11.f, 250.f, 1.f));

	if (FAILED(SetUp_Components()))
		__debugbreak();

	FAILEDCHECK(SetUp_State_Handler());
	//FAILEDCHECK(Create_UI_Status("RobellaStatus"));

	m_pInfo = new UNITDESC();
	m_pCurInfo = new UNITDESC();

	*m_pInfo = UNITDESC(ELEMENTAL_DEFAULT, 20000.f, 10.f, 2.f);
	//memcpy(m_pInfo, &UNITDESC(ELEMENTAL_DEFAULT, 20000.f, 10.f, 2.f), sizeof(UNITDESC));
	memcpy(m_pCurInfo, m_pInfo, sizeof(UNITDESC));

	return S_OK;
}

void CEnemy_Boss_Leviathan_90m::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pState_Handler->Tick(fTimeDelta);
}

void CEnemy_Boss_Leviathan_90m::LateTick(_float fTimeDelta)
{
	//if (!m_pTransformCom->Is_InAir())
	//	m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}

}

HRESULT CEnemy_Boss_Leviathan_90m::Render()
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

HRESULT CEnemy_Boss_Leviathan_90m::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_LEVIATHAN_90), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
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

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	//if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
	//	return E_FAIL;
	//m_pNaviCom->Find_CurrentIndex(m_pTransformCom);

	return S_OK;
}

HRESULT CEnemy_Boss_Leviathan_90m::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &m_vColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(1.f, 0.f, 0.f, 0.95f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 0.41f, 0.16f, 0.47f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	return S_OK;
}

HRESULT CEnemy_Boss_Leviathan_90m::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	

	m_pState_Handler->SetUp_StartState(STATE_BOSS_LEVIATHAN_IDLE);

	return S_OK;
}


CEnemy_Boss_Leviathan_90m* CEnemy_Boss_Leviathan_90m::Create()
{
	CEnemy_Boss_Leviathan_90m* pInstance = new CEnemy_Boss_Leviathan_90m();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Boss_Leviathan_90m");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Boss_Leviathan_90m::Clone(void* pArg)
{
	CEnemy_Boss_Leviathan_90m* pInstance = new CEnemy_Boss_Leviathan_90m(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Boss_Leviathan_90m");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Boss_Leviathan_90m::Free()
{
	__super::Free();
}
