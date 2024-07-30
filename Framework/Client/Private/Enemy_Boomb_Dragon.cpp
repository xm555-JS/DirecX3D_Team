#include "stdafx.h"
#include "State_Handler.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Boomb_Dragon.h"

CEnemy_Boomb_FrostDragon::CEnemy_Boomb_FrostDragon()
	: CBullet_Normal()
{
}

CEnemy_Boomb_FrostDragon::CEnemy_Boomb_FrostDragon(const CBullet_Normal& rhs)
	: CBullet_Normal(rhs)
{
}

HRESULT CEnemy_Boomb_FrostDragon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Boomb_FrostDragon::Initialize(void * pArg)
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

	m_vStartLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	return S_OK;
}

HRESULT CEnemy_Boomb_FrostDragon::Initialize_Pooling(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));
		m_pTargetObj = m_BulletDesc.pTarget;
		m_fBulltSpeed = m_BulletDesc.fBulletSpeed;
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_BulletDesc.WorldMatrix));
	}

	m_fFrustumRange = 2.f;
	m_vStartLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	return S_OK;
}

void CEnemy_Boomb_FrostDragon::Tick(_float fTimeDelta)
{
	if (true == m_bDead || nullptr == m_pTargetObj)
		return;

	_float	fLerpTime = 3.f;
	//_vector vCamLook = pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);

	m_fAccTime += fTimeDelta;

	_float	fCurTime = min(1.f, (m_fAccTime) / fLerpTime);

	if(1.f != fCurTime)
	{
		_matrix TargetMatrix = XMLoadFloat4x4(m_pTargetObj->Get_WorldMatrix());
		_vector vTargetLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - TargetMatrix.r[3]);

		//vCurLook
		_vector vFinalLook = XMVectorLerp(m_vStartLook, vTargetLook, fCurTime);
		m_pTransformCom->LookTo(vFinalLook);
		m_pTransformCom->Go_Straight(fTimeDelta, m_fBulltSpeed);
	}
	else
	{
		_matrix TargetMatrix = XMLoadFloat4x4(m_pTargetObj->Get_WorldMatrix());
		_vector vTargetPos = TargetMatrix.r[3];
		m_pTransformCom->LookAt(vTargetPos);
		m_pTransformCom->Chase(vTargetPos, fTimeDelta, m_fBulltSpeed);
	}

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
}

void CEnemy_Boomb_FrostDragon::LateTick(_float fTimeDelta)
{
	if(0.5f < m_fAccTime && IsInFrustum())
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);

	_float4 vCurPos;
	XMStoreFloat4(&vCurPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	if (152.f > vCurPos.y)
		GAMEINSTANCE->Add_PoolingObject(this);
}

HRESULT CEnemy_Boomb_FrostDragon::SetUp_Components()
{
	__super::SetUp_Components();

	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_NORMAL_1), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	m_pModelCom->Set_Owner(this);

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Boomb_FrostDragon::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	return S_OK;
}

CEnemy_Boomb_FrostDragon* CEnemy_Boomb_FrostDragon::Create()
{
	CEnemy_Boomb_FrostDragon* pInstance = new CEnemy_Boomb_FrostDragon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Boomb_FrostDragon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Boomb_FrostDragon::Clone(void* pArg)
{
	CEnemy_Boomb_FrostDragon* pInstance = new CEnemy_Boomb_FrostDragon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Boomb_FrostDragon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Boomb_FrostDragon::Free()
{
	__super::Free();
}
