#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Leviathan_FireBall.h"

#include "Effect_Base.h"
#include "Imgui_Manager.h"

CEnemy_Leviathan_FireBall::CEnemy_Leviathan_FireBall()
	: CBullet_Normal()
{
}

CEnemy_Leviathan_FireBall::CEnemy_Leviathan_FireBall(const CBullet_Normal& rhs)
	: CBullet_Normal(rhs)
{
}

HRESULT CEnemy_Leviathan_FireBall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Leviathan_FireBall::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	if (nullptr != pArg)
	{
		memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));
		m_pTargetObj = m_BulletDesc.pTarget;
		//m_fBulltSpeed = m_BulletDesc.fBulletSpeed;
		//m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_BulletDesc.WorldMatrix));
	}
	m_fBulltSpeed = 50.f;

	_float	fPosRandomRange = 50.f;
	_float	fScale = 10.f;

	_vector vParentPos = m_pTargetObj->Get_Pos();
	vParentPos = XMVectorSetY(vParentPos, XMVectorGetY(vParentPos) + 100.f);
	_vector vPos = vParentPos +
		XMVectorSet(fRandom(-fPosRandomRange, fPosRandomRange), fRandom(-fPosRandomRange, fPosRandomRange), fRandom(-fPosRandomRange, fPosRandomRange), 1.f);

	_vector vLook = XMLoadFloat4(&m_BulletDesc.TargetPos) - vParentPos;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransformCom->LookTo(vLook);
	m_pTransformCom->Set_Scaled(_float3(fScale, fScale, fScale));

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_fDamage = fRandom(640.f, 2040.f);
	m_vDeadHeight = 10.5f;

	EFFECT_EXCUTE("Boss_Robella_FireBall2_Ball", XMMatrixScaling(30.f, 30.f, 30.f), 1.f, this, CEffect_Manager::FOLLOW_POS);

	return S_OK;
}

HRESULT CEnemy_Leviathan_FireBall::Initialize_Pooling(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));
		m_pTargetObj = m_BulletDesc.pTarget;
		//m_fBulltSpeed = m_BulletDesc.fBulletSpeed;
		//m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_BulletDesc.WorldMatrix));
	}
	m_fBulltSpeed = 50.f;

	_float	fPosRandomRange = 20.f;
	_float	fScale = 10.f;

	_vector vParentPos = m_pTargetObj->Get_Pos();
	vParentPos = XMVectorSetY(vParentPos, XMVectorGetY(vParentPos) + 100.f);
	_vector vPos = vParentPos +
		XMVectorSet(fRandom(-fPosRandomRange, fPosRandomRange), fRandom(-fPosRandomRange, fPosRandomRange), fRandom(-fPosRandomRange, fPosRandomRange), 1.f);

	_vector vLook = XMLoadFloat4(&m_BulletDesc.TargetPos) - vParentPos;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransformCom->LookTo(vLook);
	m_pTransformCom->Set_Scaled(_float3(fScale, fScale, fScale));


	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_fTimeAcc = 0.f;
	m_fTimeAcc_Effect0 = 0.f;
	m_fTimeAcc_Effect1 = 0.f;

	m_bShoot = false;

	m_pCol_Sphere->Clear_Colliders();

	m_fDamage = fRandom(640.f, 2040.f);
	m_vDeadHeight = 10.5f;

	EFFECT_EXCUTE("Boss_Robella_FireBall2_Ball", XMMatrixScaling(30.f, 30.f, 30.f), 1.f, this, CEffect_Manager::FOLLOW_POS);

	RESET_ONCE(m_bSound0);
	RESET_ONCE(m_bSound1);

	return S_OK;
}

void CEnemy_Leviathan_FireBall::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;
	m_pTransformCom->Go_Straight(fTimeDelta, m_fBulltSpeed);
	m_pTransformCom->Tick(fTimeDelta);

	m_fTimeAcc += fTimeDelta;
	m_fTimeAcc_Effect0 += fTimeDelta;
	m_fTimeAcc_Effect1 += fTimeDelta;

	if (0.01f < m_fTimeAcc_Effect0)
	{
		EFFECT_POS("Boss_Robella_FireBall2_Ball_Trail_M", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(30.f, 30.f, 30.f, 0.f));
		m_fTimeAcc_Effect0 = 0.f;
	}
	if (0.05 < m_fTimeAcc_Effect1)
	{
		EFFECT_POS("Boss_Robella_FireBall2_Ball_Trail_P", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(30.f, 30.f, 30.f, 0.f));
		m_fTimeAcc_Effect1 = 0.f;

		IMPL_ONCE(m_bSound0, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Fire_Ball_Trail.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,
			m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 0.1f));
	}


	_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	_bool IsUpperThanHight = pGameInstance->isUpperThanHight(XMLoadFloat4(&vPos), 1.05f);

	if (m_vDeadHeight > vPos.y /*|| 5.f < m_fTimeAcc*/ || !IsUpperThanHight)//본체바닥정도잡아야함
	{
		IMPL_ONCE(m_bSound1, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_FireBall.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,
			m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 0.1f));

		pGameInstance->Add_PoolingObject(this);
		_vector vPos = m_pTransformCom->Get_WorldMatrix().r[3];
		vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) - 0.2f);
		_float fAngle = fRandom(-180.f, 180.f);
		EFFECT_POS("Boss_Robella_FireBall2_Ball_Explosion", nullptr, vPos, XMVectorSet(0.f, XMConvertToRadians(fAngle), 0.f, 0.f), 1.f, XMVectorSet(15.f, 15.f, 15.f, 0.f));
		return;
	}

	for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
	{
		((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_FIRE, m_fDamage);
	}
	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
}

void CEnemy_Leviathan_FireBall::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);
}

HRESULT CEnemy_Leviathan_FireBall::Render()
{
#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif

	return S_OK;
}

HRESULT CEnemy_Leviathan_FireBall::SetUp_Components()
{
	USEGAMEINSTANCE;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Leviathan_FireBall::SetUp_ShaderResource()
{
	return S_OK;
}

CEnemy_Leviathan_FireBall* CEnemy_Leviathan_FireBall::Create()
{
	CEnemy_Leviathan_FireBall* pInstance = new CEnemy_Leviathan_FireBall();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Leviathan_FireBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Leviathan_FireBall::Clone(void* pArg)
{
	CEnemy_Leviathan_FireBall* pInstance = new CEnemy_Leviathan_FireBall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Leviathan_FireBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Leviathan_FireBall::Free()
{
	__super::Free();
}
