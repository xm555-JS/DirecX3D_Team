#include "stdafx.h"
#include "Bullet_Player_Cube.h"
#include "Player.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Effect_Manager.h"
#include "Imgui_Manager.h"

#include "Sound_Manager.h"

CBullet_Player_Cube::CBullet_Player_Cube()
	: CBullet()
{
}

CBullet_Player_Cube::CBullet_Player_Cube(const CBullet_Player_Cube& rhs)
	: CBullet(rhs)
{
}

HRESULT CBullet_Player_Cube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_Player_Cube::Initialize(void* pArg)
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
	m_pTransformCom->LookTo(pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);
	XMStoreFloat4(&m_BulletDesc.vLookAt, pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);

	m_BulletDesc.fBulletSpeed = 2.f;
	m_BulletDesc.fBulletDeadTime = 4.f;
	m_fDamage = 200.f;  //10.f;
	m_fDeadTimeAcc = 0.f;
	m_fAccTime = 0.f;
	m_fAccLerfTime = 0.f;
	m_pOwner = pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER), 0);

	Set_Parent(m_pOwner);
	RESET_ONCE(m_bSound);
	_matrix OffsetMatrix = XMMatrixScaling(0.7f, 0.7f, 0.7f);
	EFFECT_EXCUTE("Player_Cube_Object_Small", OffsetMatrix, 1.f, this, CEffect_Manager::FOLLOW_POS);
	return S_OK;
}

HRESULT CBullet_Player_Cube::Initialize_Pooling(void* pArg)
{
	USEGAMEINSTANCE;

	m_pCol_Sphere->Clear_Colliders();

	pGameInstance->Add_TickGroup(TG_WEAPON, this);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);

	if (nullptr != pArg)
	{
		memcpy(&m_vPos, pArg, sizeof(_float4));
	}

	m_BulletDesc.vStartPos = m_vPos;
	m_pTransformCom->LookTo(pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);
	XMStoreFloat4(&m_BulletDesc.vLookAt, pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);

	m_BulletDesc.fBulletSpeed = 2.f;
	m_BulletDesc.fBulletDeadTime = 4.f;
	m_fDamage = 200.f;  //10.f;
	m_fDeadTimeAcc = 0.f;
	m_fAccTime = 0.f;
	m_fAccLerfTime = 0.f;
	m_bShoot = false;
	m_bSetTarget = false;
	m_pOwner = pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER), 0);
	m_pTarget = nullptr;
	m_pTransformCom->Reset_StraightAccTime();

	Set_Parent(m_pOwner);
	RESET_ONCE(m_bSound);

	_matrix OffsetMatrix = XMMatrixScaling(0.7f, 0.7f, 0.7f);
	EFFECT_EXCUTE("Player_Cube_Object_Small", OffsetMatrix, 1.f, this, CEffect_Manager::FOLLOW_POS);

	m_fAccTime_Effect = 0.f;
	m_iEffect = 0;

	return S_OK;
}

void CBullet_Player_Cube::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;
	USEGAMEINSTANCE;
	IMPL_ONCE(m_bSound,MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_IceForm.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, Get_Pos(), 0.5f));

	_float4 vOwnerPos;

	m_fAccTime_Effect += fTimeDelta;

	if (0.03f < m_fAccTime_Effect)
	{
		m_fAccTime_Effect = 0.f;
		if (0 == m_iEffect)
			EFFECT_POS("Player_Cube_Object_P1", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 1.f, XMVectorSet(0.7f, 0.7f, 0.7f, 1.f));
		else if (1 == m_iEffect)
			EFFECT_POS("Player_Cube_Object_P2", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 1.f, XMVectorSet(0.7f, 0.7f, 0.7f, 1.f));
		else if (2 == m_iEffect)
			EFFECT_POS("Player_Cube_Object_P3", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 1.f, XMVectorSet(0.7f, 0.7f, 0.7f, 1.f));
		else if (3 == m_iEffect)
			EFFECT_POS("Player_Cube_Object_P4", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 1.f, XMVectorSet(0.7f, 0.7f, 0.7f, 1.f));

		m_iEffect += 1;
		if (3 < m_iEffect)
			m_iEffect = 0;
	}

	if (m_bShoot) // 특정상황에서 슛
	{
		_float	fLerpTime = 1.5f;

		m_fAccTime += fTimeDelta;
		m_BulletDesc.fBulletSpeed = 20.f;
		_float	fCurLerp = min(1.f, m_fAccTime / fLerpTime);

		if (!m_bSetTarget)
		{
			m_pTarget = ((CPlayer*)m_pOwner)->Get_Target();

			if (nullptr != m_pTarget)
				XMStoreFloat4(&m_pActionDescs, m_pTarget->Get_Pos());
			else
				XMStoreFloat4(&m_pActionDescs, (m_pOwner->Get_Pos() + 10.f * XMVector3Normalize(XMLoadFloat4x4(m_pOwner->Get_WorldMatrix()).r[2])));

			m_bSetTarget = true;
		}

		_vector vTargetLook = XMVector3Normalize(XMLoadFloat4(&m_pActionDescs) - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

		if (1.5f > m_fAccTime)
		{
			//vCurLook
			_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vTargetLook, fCurLerp);
			m_pTransformCom->LookTo(vFinalLook);
			m_pTransformCom->Go_AccStraight(fTimeDelta, m_BulletDesc.fBulletSpeed);
		}
		else
		{
			m_pTransformCom->LookAt(XMLoadFloat4(&m_pActionDescs));
			m_pTransformCom->Go_AccStraight(fTimeDelta * 0.7f, m_BulletDesc.fBulletSpeed * 0.7f);
		}

		m_fDeadTimeAcc += fTimeDelta;
		for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
		{
			MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Hit_Ice_Small.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, Get_Pos(), 0.5f);
			pGameInstance->Add_PoolingObject(this);
			((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_ICE, m_fDamage);
			return;
		}
		if (0.1f >= XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_pActionDescs) - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION))))
		{
			MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Hit_Ice_Small.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, Get_Pos(), 0.5f);
			pGameInstance->Add_PoolingObject(this);
			return;
		}
	}
	else if (!((CPlayer*)m_pOwner)->Get_CubeBulletCreate())
	{
		m_fTurnAccTime += fTimeDelta;
		XMStoreFloat4(&vOwnerPos, m_pOwner->Get_Pos());
		m_pTransformCom->OrbitY(m_vPos, vOwnerPos, m_fTurnAccTime * 1.5f);
		XMStoreFloat4(&m_vStartLook, (m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - m_pOwner->Get_Pos()));
		m_pTransformCom->YFixedLook(XMLoadFloat4(&m_vStartLook));
	}


	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
	for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
	{
		MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, Get_Pos(), 0.5f);
		pGameInstance->Add_PoolingObject(this);
		if (nullptr != (CUnit*)iter->Get_Owner() ||
			0 <= ((CUnit*)iter->Get_Owner())->Get_UnitCurInfo()->fHP)
			((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_ICE, m_fDamage);
	}
}

void CBullet_Player_Cube::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}

	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_ATTACK), m_pCol_Sphere);

}

HRESULT CBullet_Player_Cube::Render()
{

#ifdef _DEBUG

	if (nullptr != m_pCol_Sphere)
		m_pCol_Sphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CBullet_Player_Cube::SetUp_Components()
{
	__super::SetUp_Components();


	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

CBullet_Player_Cube* CBullet_Player_Cube::Create()
{
	CBullet_Player_Cube* pInstance = new CBullet_Player_Cube();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet_Player_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_Player_Cube::Clone(void* pArg)
{
	CBullet_Player_Cube* pInstance = new CBullet_Player_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet_Player_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_Player_Cube::Free()
{
	__super::Free();
}
