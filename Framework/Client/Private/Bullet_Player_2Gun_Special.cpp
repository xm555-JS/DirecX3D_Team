#include "stdafx.h"
#include "Bullet_Player_2Gun_Special.h"
#include "Player.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Effect_Manager.h"


CBullet_Player_2Gun_Special::CBullet_Player_2Gun_Special()
	: CBullet()
{
}

CBullet_Player_2Gun_Special::CBullet_Player_2Gun_Special(const CBullet_Player_2Gun_Special& rhs)
	: CBullet(rhs)
{
}

HRESULT CBullet_Player_2Gun_Special::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_Player_2Gun_Special::Initialize(void* pArg)
{
	m_fFrustumRange = 0.5f;
	FAILEDCHECK(__super::Initialize(pArg));

	FAILEDCHECK(SetUp_Components());

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_WEAPON, this);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);

	if (nullptr != pArg)
	{
		memcpy(&m_vPos, pArg, sizeof(_float4));
	}

	m_BulletDesc.vStartPos = m_vPos;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_BulletDesc.vStartPos));

	m_pOwner = pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER), 0);

	m_BulletDesc.fBulletSpeed = 0.f;
	m_BulletDesc.fBulletDeadTime = 0.5f;
	m_fDamage = fRandom(150.f, 250.f);  //10.f;
	m_fDeadTimeAcc = 0;

	EFFECT_EXCUTE("Player_Gun_Attack_Bullet", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS_RUL);
	return S_OK;
}

HRESULT CBullet_Player_2Gun_Special::Initialize_Pooling(void* pArg)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_WEAPON, this);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);

	if (nullptr != pArg)
	{
		memcpy(&m_vPos, pArg, sizeof(_float4));
	}

	m_BulletDesc.vStartPos = m_vPos;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_BulletDesc.vStartPos));

	m_fFrustumRange = 0.5f;
	m_BulletDesc.fBulletSpeed = 0.f;
	m_BulletDesc.fBulletDeadTime = 0.5f;
	m_fDamage = fRandom(150.f, 250.f);  //10.f;
	m_fDeadTimeAcc = 0;

	m_pCol_Sphere->Clear_Colliders();

	m_fTimeAcc_Effect0 = 0.f;
	m_fTimeAcc_Effect1 = 0.f;
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);

	EFFECT_EXCUTE("Player_Gun_Attack_Bullet", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS_RUL);
	return S_OK;
}

void CBullet_Player_2Gun_Special::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;
	USEGAMEINSTANCE;


	IMPL_ONCE(m_bEffect0,
		EFFECT_POS("Player_Gun_Skill_Special_B",
			m_pTransformCom->Get_WorldMatrix_ptr(),
			XMVectorSet(0.f, 0.7f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			1.f,
			XMVectorSet(2.f, 2.f, 2.f, 1.f)
		)
	);

	m_fTimeAcc_Effect0 += fTimeDelta;

	if (0.06f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect1 = 0.f;
		_float fSize = fRandom(0.5f, 1.f);
		_float3 fPos = { fRandom(-3.5f, 3.5f), fRandom(-3.5f, 3.f) + 0.7f, fRandom(-3.5f, 3.5f) + 0.f };
		EFFECT_POS("Player_Gun_Hit",
			m_pTransformCom->Get_WorldMatrix_ptr(),
			XMVectorSet(fPos.x, fPos.y, fPos.z, 1.f),
			XMVectorSet(XMConvertToRadians(fRandom(-180.f, 180.f)), XMConvertToRadians(fRandom(-180.f, 180.f)), XMConvertToRadians(fRandom(-180.f, 180.f)), 1.f),
			fRandom(1.f, 2.f),
			XMVectorSet(fSize, fSize, fSize, 1.f));
		fSize = fRandom(0.5f, 1.f);
		fPos = { fRandom(-3.5f, 3.5f), fRandom(-3.5f, 3.f) + 0.7f, fRandom(-3.5f, 3.5f) + 0.f };
		EFFECT_POS("Player_Gun_Hit",
			m_pTransformCom->Get_WorldMatrix_ptr(),
			XMVectorSet(fPos.x, fPos.y, fPos.z, 1.f),
			XMVectorSet(XMConvertToRadians(fRandom(-180.f, 180.f)), XMConvertToRadians(fRandom(-180.f, 180.f)), XMConvertToRadians(fRandom(-180.f, 180.f)), 1.f),
			fRandom(1.f, 2.f),
			XMVectorSet(fSize, fSize, fSize, 1.f));
	}

	IMPL_ONCE(m_bEffect1,
		EFFECT_POS("Player_Gun_Skill_Special_C",
			m_pTransformCom->Get_WorldMatrix_ptr(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(180.f), 0.f, 1.f),
			1.f,
			XMVectorSet(2.f, 2.f, 2.f, 1.f)
		)
	);
	IMPL_ONCE(m_bEffect2,
		EFFECT_POS("Player_Gun_Skill_Special_C",
			m_pTransformCom->Get_WorldMatrix_ptr(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(180.f), 0.f, 1.f),
			1.f,
			XMVectorSet(3.f, 3.f, 3.f, 1.f)
		)
	);
	IMPL_ONCE(m_bEffect3,
		EFFECT_POS("Player_Gun_Skill_Special_C",
			m_pTransformCom->Get_WorldMatrix_ptr(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(XMConvertToRadians(90.f), 0.f, 0.f, 1.f),
			1.f,
			XMVectorSet(4.f, 4.f, 4.f, 1.f)
		)
	);
	/// 맞는 애들은
	/// Player_Gun_Skill_Special_C 
	/// 이펙트 생성해주면 됨.



	m_fDeadTimeAcc += fTimeDelta;
	if (m_BulletDesc.fBulletDeadTime < m_fDeadTimeAcc)
		pGameInstance->Add_PoolingObject(this);

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
	for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
	{
		((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_LIGHTNING, m_fDamage);
		pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(PLAYER, WEAPON_2GUN_BULLET_SPECIAL_SUB), ID_UINT(PLAYER, WEAPON_2GUN_BULLET_SPECIAL_SUB), iter->Get_Owner());
	}
}

void CBullet_Player_2Gun_Special::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (IsInFrustum())
	{
		pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_ATTACK), m_pCol_Sphere);

	}

HRESULT CBullet_Player_2Gun_Special::Render()
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

HRESULT CBullet_Player_2Gun_Special::SetUp_Components()
{
	__super::SetUp_Components();

	//FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	//FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_ARROW), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	//m_pModelCom->Set_Owner(this);


	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(8.f, 8.f, 8.f);
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



CBullet_Player_2Gun_Special* CBullet_Player_2Gun_Special::Create()
{
	CBullet_Player_2Gun_Special* pInstance = new CBullet_Player_2Gun_Special();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet_Player_2Gun_Special");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_Player_2Gun_Special::Clone(void* pArg)
{
	CBullet_Player_2Gun_Special* pInstance = new CBullet_Player_2Gun_Special(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet_Player_2Gun_Special");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_Player_2Gun_Special::Free()
{
	__super::Free();
}
