#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Bullet_Dragon.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CEnemy_Bullet_FrostDragon::CEnemy_Bullet_FrostDragon()
	: CBullet_Normal()
{
}

CEnemy_Bullet_FrostDragon::CEnemy_Bullet_FrostDragon(const CBullet_Normal& rhs)
	: CBullet_Normal(rhs)
{
}

HRESULT CEnemy_Bullet_FrostDragon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Bullet_FrostDragon::Initialize(void * pArg)
{
	m_fFrustumRange = 2.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	if (nullptr != pArg)
	{
		memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));
		m_pTargetObj = m_BulletDesc.pTarget;
		m_fBulltSpeed = m_BulletDesc.fBulletSpeed;
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_BulletDesc.WorldMatrix));
	}

	XMStoreFloat4(&m_vStartLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	m_fDamage = fRandom(510.f, 590.f);

	return S_OK;
}

HRESULT CEnemy_Bullet_FrostDragon::Initialize_Pooling(void* pArg)
{
	m_pCol_Sphere->Clear_Colliders();

	if (nullptr != pArg)
	{
		memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));
		m_pTargetObj = m_BulletDesc.pTarget;
		m_fBulltSpeed = m_BulletDesc.fBulletSpeed;
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_BulletDesc.WorldMatrix));
	}

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_pTransformCom->Reset_StraightAccTime();

	XMStoreFloat4(&m_vStartLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	m_fDamage = fRandom(510.f, 590.f);

	m_fAccTime = 0.f;
	m_fAccTime_Effect0 = 0.f;
	m_fAccTime_Effect1 = 0.f;
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bSound0);

	m_fFrustumRange = 2.f;
	return S_OK;
}

void CEnemy_Bullet_FrostDragon::Tick(_float fTimeDelta)
{
	if (true == m_bDead || nullptr == m_pTargetObj)
		return;

	USEGAMEINSTANCE;

	for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
	{
		pGameInstance->Add_PoolingObject(this);
		((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_MELEE, m_fDamage);
		EFFECT_POS("Boss_Dragon_Missile_Explosion", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(-0.5f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(2.f, 2.f, 2.f, 0.f));
		
		IMPL_ONCE(m_bSound0, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Missile_Boom.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,
			m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 1.f));
		
		return;
	}

	_float	fLerpTime = 3.f;
	//_vector vCamLook = pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

	m_fAccTime += fTimeDelta;

	m_fAccTime_Effect0 += fTimeDelta;
	m_fAccTime_Effect1 += fTimeDelta;

	if (0.5f < m_fAccTime_Effect1)
	{
		IMPL_ONCE(m_bEffect0, EFFECT_EXCUTE("Boss_Dragon_Missile_Fire", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS_RUL, ""));
		m_fAccTime_Effect1 = 0.f;
	}
	if (0.010f < m_fAccTime_Effect0)
	{
		EFFECT_POS("Boss_Dragon_Missile_Smoking" + to_string(Random(1, 4)), this->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, XMConvertToRadians(Random(-360, 360)), 1.f));
		m_fAccTime_Effect0 = 0.f;
	}

	_float	fCurTime = min(1.f, (m_fAccTime) / fLerpTime);

	if (1.f != fCurTime)
	{
		_matrix TargetMatrix = XMLoadFloat4x4(m_pTargetObj->Get_WorldMatrix());
		_vector vTargetLook = XMVector3Normalize(TargetMatrix.r[3] - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

		//vCurLook
		_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vTargetLook, fCurTime);
		m_pTransformCom->LookTo(vFinalLook);
		m_pTransformCom->Go_Straight(fTimeDelta, m_fBulltSpeed);
	}
	else if (3.5f > m_fAccTime)
	{
		_matrix TargetMatrix = XMLoadFloat4x4(m_pTargetObj->Get_WorldMatrix());
		_vector vTargetPos = TargetMatrix.r[3];
		m_pTransformCom->LookAt(vTargetPos);
		m_pTransformCom->Chase(vTargetPos, fTimeDelta, m_fBulltSpeed * 4.f);
	}
	else
	{
		m_fBulltSpeed += fTimeDelta;
		m_pTransformCom->Go_AccStraight(fTimeDelta, m_fBulltSpeed * 4.f);
	}

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
}

void CEnemy_Bullet_FrostDragon::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (IsInFrustum())
	{
		pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}


	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);

	_float4 vCurPos;
	XMStoreFloat4(&vCurPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	if (152.f > vCurPos.y)
	{
		pGameInstance->Add_PoolingObject(this);
		EFFECT_POS("Boss_Dragon_Missile_Explosion", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(-0.5f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(2.f, 2.f, 2.f, 0.f));
	
		IMPL_ONCE(m_bSound0, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Missile_Boom.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,
			m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 0.5f));
	}
}

HRESULT CEnemy_Bullet_FrostDragon::SetUp_Components()
{
	__super::SetUp_Components();

	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_NORMAL_3), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	m_pModelCom->Set_Owner(this);

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Bullet_FrostDragon::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	return S_OK;
}

CEnemy_Bullet_FrostDragon* CEnemy_Bullet_FrostDragon::Create()
{
	CEnemy_Bullet_FrostDragon* pInstance = new CEnemy_Bullet_FrostDragon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Bullet_FrostDragon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Bullet_FrostDragon::Clone(void* pArg)
{
	CEnemy_Bullet_FrostDragon* pInstance = new CEnemy_Bullet_FrostDragon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Bullet_FrostDragon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Bullet_FrostDragon::Free()
{
	__super::Free();
}
