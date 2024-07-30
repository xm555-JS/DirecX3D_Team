#include "stdafx.h"
#include "Bullet_Player_Cube_Main.h"
#include "Player.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Effect_Manager.h"
#include "Imgui_Manager.h"

#include "Sound_Manager.h"

CBullet_Player_Cube_Main::CBullet_Player_Cube_Main()
	: CBullet()
{
}

CBullet_Player_Cube_Main::CBullet_Player_Cube_Main(const CBullet_Player_Cube_Main& rhs)
	: CBullet(rhs)
{
}

HRESULT CBullet_Player_Cube_Main::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_Player_Cube_Main::Initialize(void* pArg)
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
	_matrix OffsetMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	EFFECT_EXCUTE("Player_Cube_Object", OffsetMatrix, 1.f, this, CEffect_Manager::FOLLOW_POS);
	return S_OK;
}

HRESULT CBullet_Player_Cube_Main::Initialize_Pooling(void* pArg)
{
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
	m_bShoot = false;
	m_bSetTarget = false;
	m_pOwner = pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER), 0);
	m_pTarget = nullptr;
	m_pTransformCom->Reset_StraightAccTime();

	Set_Parent(m_pOwner);
	RESET_ONCE(m_bSound);

	_matrix OffsetMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	EFFECT_EXCUTE("Player_Cube_Object", OffsetMatrix, 1.f, this, CEffect_Manager::FOLLOW_POS);

	m_fAccTime_Effect = 0.f;
	m_iEffect = 0;

	return S_OK;
}

void CBullet_Player_Cube_Main::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;
	USEGAMEINSTANCE;
	IMPL_ONCE(m_bSound, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_IceForm.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, Get_Pos(), 0.5f));

	_float4 vTargetPos, vOwnerPos;

	m_fAccTime_Effect += fTimeDelta;

	if (0.03f < m_fAccTime_Effect)
	{
		m_fAccTime_Effect = 0.f;
		if (0 == m_iEffect)
			EFFECT_POS("Player_Cube_Object_P1", m_pTransformCom->Get_WorldMatrix_ptr());
		else if (1 == m_iEffect)													 
			EFFECT_POS("Player_Cube_Object_P2", m_pTransformCom->Get_WorldMatrix_ptr());
		else if (2 == m_iEffect)													
			EFFECT_POS("Player_Cube_Object_P3", m_pTransformCom->Get_WorldMatrix_ptr());
		else if (3 == m_iEffect)													
			EFFECT_POS("Player_Cube_Object_P4", m_pTransformCom->Get_WorldMatrix_ptr());

		m_iEffect += 1;
		if (3 < m_iEffect)
			m_iEffect = 0;
	}

	m_fTurnAccTime += fTimeDelta;
	XMStoreFloat4(&vOwnerPos, m_pOwner->Get_Pos());
	m_pTransformCom->OrbitY(m_vPos, vOwnerPos, m_fTurnAccTime * 1.5f);
	XMStoreFloat4(&m_vStartLook, (m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - m_pOwner->Get_Pos()));
	m_pTransformCom->YFixedLook(XMLoadFloat4(&m_vStartLook));

}

void CBullet_Player_Cube_Main::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (IsInFrustum())
	{
		pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}

}

HRESULT CBullet_Player_Cube_Main::Render()
{

#ifdef _DEBUG

#endif // _DEBUG

	return S_OK;
}

HRESULT CBullet_Player_Cube_Main::SetUp_Components()
{
	__super::SetUp_Components();

	return S_OK;
}

CBullet_Player_Cube_Main* CBullet_Player_Cube_Main::Create()
{
	CBullet_Player_Cube_Main* pInstance = new CBullet_Player_Cube_Main();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet_Player_Cube_Main");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_Player_Cube_Main::Clone(void* pArg)
{
	CBullet_Player_Cube_Main* pInstance = new CBullet_Player_Cube_Main(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet_Player_Cube_Main");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_Player_Cube_Main::Free()
{
	__super::Free();
}
