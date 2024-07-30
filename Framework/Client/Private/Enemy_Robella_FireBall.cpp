#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Navigation.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Robella_FireBall.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CEnemy_Robella_FireBall::CEnemy_Robella_FireBall()
	: CBullet_Normal()
{
}

CEnemy_Robella_FireBall::CEnemy_Robella_FireBall(const CBullet_Normal& rhs)
	: CBullet_Normal(rhs)
{
}

HRESULT CEnemy_Robella_FireBall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Robella_FireBall::Initialize(void* pArg)
{
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
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_fDamage = fRandom(510.f, 580.f);

	m_pTransformCom->Jump(15.f, m_pNaviCom);

	EFFECT_EXCUTE("Boss_Robella_FireBall2_Ball", XMMatrixScaling(3.f, 3.f, 3.f), 1.f, this, CEffect_Manager::FOLLOW_POS);

	return S_OK;
}

HRESULT CEnemy_Robella_FireBall::Initialize_Pooling(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));
		m_pTargetObj = m_BulletDesc.pTarget;
		m_fBulltSpeed = m_BulletDesc.fBulletSpeed;
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_BulletDesc.WorldMatrix));
	}
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_fTimeAcc = 0.f;
	m_fTimeAcc_Effect0 = 0.f;
	m_fTimeAcc_Effect1 = 0.f;

	m_pCol_Sphere->Clear_Colliders();

	m_fDamage = fRandom(510.f, 580.f);

	m_pTransformCom->Jump(15.f, m_pNaviCom);

	EFFECT_EXCUTE("Boss_Robella_FireBall2_Ball", XMMatrixScaling(3.f, 3.f, 3.f), 1.f, this, CEffect_Manager::FOLLOW_POS);

	RESET_ONCE(m_bSound0);
	RESET_ONCE(m_bSound1);

	return S_OK;
}

void CEnemy_Robella_FireBall::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;
	m_pTransformCom->Go_Straight(fTimeDelta, m_fBulltSpeed, m_pNaviCom);
	m_pTransformCom->Tick(fTimeDelta);

	m_fTimeAcc += fTimeDelta;
	m_fTimeAcc_Effect0 += fTimeDelta;
	m_fTimeAcc_Effect1 += fTimeDelta;

	if (0.01f < m_fTimeAcc_Effect0)
	{
		EFFECT_POS("Boss_Robella_FireBall2_Ball_Trail_M", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(3.f, 3.f, 3.f, 0.f));
		m_fTimeAcc_Effect0 = 0.f;
	}
	if (0.05 < m_fTimeAcc_Effect1)
	{
		EFFECT_POS("Boss_Robella_FireBall2_Ball_Trail_P", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(3.f, 3.f, 3.f, 0.f));
		m_fTimeAcc_Effect1 = 0.f;

		IMPL_ONCE(m_bSound1, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Fire_Ball_Trail.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,
			m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 0.1f));
	}

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	_float fYpos = XMVectorGetY(m_pTargetObj->Get_Pos());
	
	if (fYpos > vPos.y || 5.f < m_fTimeAcc)//본체바닥정도잡아야함
	{
		pGameInstance->Add_PoolingObject(this); 
		_vector vPos = m_pTransformCom->Get_WorldMatrix().r[3];
		vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) - 0.2f);
		_float fAngle = fRandom(-180.f, 180.f);
		EFFECT_POS("Boss_Robella_FireBall2_Ball_Explosion", nullptr, vPos, XMVectorSet(0.f, XMConvertToRadians(fAngle), 0.f, 0.f), 1.f, XMVectorSet(3.f, 3.f, 3.f, 0.f));
		
		IMPL_ONCE(m_bSound0, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_FireBall.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,
			m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 0.1f));
		
		return;
	}

	for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
	{
		((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_FIRE, m_fDamage);
	}
	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
}

void CEnemy_Robella_FireBall::LateTick(_float fTimeDelta)
{
	if (!m_pTransformCom->Is_InAir())
		m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);

	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);
}

HRESULT CEnemy_Robella_FireBall::Render()
{
#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif

	return S_OK;
}

HRESULT CEnemy_Robella_FireBall::SetUp_Components()
{
	USEGAMEINSTANCE;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.5f, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemy_Robella_FireBall::SetUp_ShaderResource()
{
	return S_OK;
}

CEnemy_Robella_FireBall* CEnemy_Robella_FireBall::Create()
{
	CEnemy_Robella_FireBall* pInstance = new CEnemy_Robella_FireBall();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Robella_FireBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Robella_FireBall::Clone(void* pArg)
{
	CEnemy_Robella_FireBall* pInstance = new CEnemy_Robella_FireBall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Robella_FireBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Robella_FireBall::Free()
{
	__super::Free();
}
