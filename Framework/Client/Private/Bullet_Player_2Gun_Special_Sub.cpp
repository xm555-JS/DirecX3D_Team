#include "stdafx.h"
#include "Bullet_Player_2Gun_Special_Sub.h"
#include "Player.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Effect_Manager.h"
#include "Sound_Manager.h"



CBullet_Player_2Gun_Special_Sub::CBullet_Player_2Gun_Special_Sub()
	: CBullet()
{
}

CBullet_Player_2Gun_Special_Sub::CBullet_Player_2Gun_Special_Sub(const CBullet_Player_2Gun_Special_Sub& rhs)
	: CBullet(rhs)
{
}

HRESULT CBullet_Player_2Gun_Special_Sub::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_Player_2Gun_Special_Sub::Initialize(void* pArg)
{
	m_fFrustumRange = 0.5f;
	FAILEDCHECK(__super::Initialize(pArg));

	FAILEDCHECK(SetUp_Components());

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_WEAPON, this);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);

	if (nullptr != pArg)
	{
		m_pTarget = (CGameObject*)pArg;
	}

	XMStoreFloat4(&m_BulletDesc.vStartPos, m_pTarget->Get_Pos());
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_BulletDesc.vStartPos));

	m_pOwner = pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER), 0);

	m_BulletDesc.fBulletSpeed = 0.f;
	m_BulletDesc.fBulletDeadTime = 2.f;
	m_fDamage = fRandom(150.f, 250.f);  //10.f;
	m_fDeadTimeAcc = 0;

	EFFECT_EXCUTE("Player_Gun_Attack_Bullet", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS_RUL);
	return S_OK;
}

HRESULT CBullet_Player_2Gun_Special_Sub::Initialize_Pooling(void* pArg)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_WEAPON, this);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);

	if (nullptr != pArg)
	{
		m_pTarget = (CGameObject*)pArg;
	}

	XMStoreFloat4(&m_BulletDesc.vStartPos, m_pTarget->Get_Pos());
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_BulletDesc.vStartPos));

	m_fFrustumRange = 0.5f;
	m_BulletDesc.fBulletSpeed = 0.f;
	m_BulletDesc.fBulletDeadTime = 2.f;
	m_fDamage = fRandom(150.f, 250.f);  //10.f;
	m_fDeadTimeAcc = 0;

	m_pCol_Sphere->Clear_Colliders();

	EFFECT_EXCUTE("Player_Gun_Attack_Bullet", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS_RUL);
	return S_OK;
}

void CBullet_Player_2Gun_Special_Sub::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;
	USEGAMEINSTANCE;


	m_fTimeAcc_Effect0 += fTimeDelta;

	if (0.04f < m_fTimeAcc_Effect0)
	{
		MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_B.wav"), CSound_Manager::CHANNEL_PLAYER, Get_Pos(), 0.5f);

		m_fTimeAcc_Effect0 = 0.f;
		_float fSize = fRandom(0.5f, 1.f);
		_float3 fPos = { fRandom(-0.2f, 0.2f), fRandom(-0.2f, 0.2f), fRandom(-0.2f, 0.2f) };

		EFFECT_POS("Player_Gun_Skill_Special_C",
			m_pTransformCom->Get_WorldMatrix_ptr(),
			XMVectorSet(fPos.x, fPos.y, fPos.z, 1.f),
			XMVectorSet(XMConvertToRadians(fRandom(-180.f, 180.f)), XMConvertToRadians(fRandom(-180.f, 180.f)), XMConvertToRadians(fRandom(-180.f, 180.f)), 1.f),
			fRandom(1.f, 2.f),
			XMVectorSet(fSize, fSize, fSize, 1.f));
		fSize = fRandom(1.f, 2.f);
		fPos = { fRandom(-0.2f, 0.2f), fRandom(-0.2f, 0.2f) + 1.f, fRandom(-0.2f, 0.2f) };
		EFFECT_POS("Player_Gun_Skill_Special_C",
			m_pTransformCom->Get_WorldMatrix_ptr(),
			XMVectorSet(fPos.x, fPos.y, fPos.z, 1.f),
			XMVectorSet(XMConvertToRadians(fRandom(-180.f, 180.f)), XMConvertToRadians(fRandom(-180.f, 180.f)), XMConvertToRadians(fRandom(-180.f, 180.f)), 1.f),
			fRandom(1.f, 2.f),
			XMVectorSet(fSize, fSize, fSize, 1.f));
	}

	XMStoreFloat4(&m_BulletDesc.vStartPos, m_pTarget->Get_Pos());
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_BulletDesc.vStartPos));

	m_fDeadTimeAcc += fTimeDelta;
	if (m_BulletDesc.fBulletDeadTime < m_fDeadTimeAcc)
		pGameInstance->Add_PoolingObject(this);

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
	for (auto& iter : *m_pCol_Sphere->Get_StayColliders())
	{
		m_fAttackTimeAcc += fTimeDelta;
		if (0.5f < m_fAttackTimeAcc)
		{
			m_fAttackTimeAcc = 0.f;
			((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_LIGHTNING, fRandom(100.f, 120.f));
		}
	}
}

void CBullet_Player_2Gun_Special_Sub::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (IsInFrustum())
	{
		pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_ATTACK), m_pCol_Sphere);

}

HRESULT CBullet_Player_2Gun_Special_Sub::Render()
{
	//if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
	//	return E_FAIL;

	//_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	/* 셰이더 전역변수에 값을 던진다. */
	//FAILEDCHECK(SetUp_ShaderResource());
	//
	//m_pShaderCom->Begin(0);
	//
	//for (_uint i = 0; i < iNumMeshContainers; ++i)
	//{
	//	FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));
	//	FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));
	//
	//	m_pModelCom->Render(i, m_pShaderCom);
	//}

#ifdef _DEBUG

	if (nullptr != m_pCol_Sphere)
		m_pCol_Sphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CBullet_Player_2Gun_Special_Sub::SetUp_Components()
{
	__super::SetUp_Components();

	//FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	//FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_ARROW), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	//m_pModelCom->Set_Owner(this);


	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, .1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

#if 0
HRESULT CBullet_Player_2Gun::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	return S_OK;
}
#endif // 0



CBullet_Player_2Gun_Special_Sub* CBullet_Player_2Gun_Special_Sub::Create()
{
	CBullet_Player_2Gun_Special_Sub* pInstance = new CBullet_Player_2Gun_Special_Sub();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet_Player_2Gun_Special_Sub");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_Player_2Gun_Special_Sub::Clone(void* pArg)
{
	CBullet_Player_2Gun_Special_Sub* pInstance = new CBullet_Player_2Gun_Special_Sub(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet_Player_2Gun_Special_Sub");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_Player_2Gun_Special_Sub::Free()
{
	__super::Free();
}
