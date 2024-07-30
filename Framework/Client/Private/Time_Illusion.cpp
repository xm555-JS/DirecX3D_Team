#include "stdafx.h"

#include "..\Public\Bullet.h"

#include "GameInstance.h"
#include "Navigation.h"
#include "..\Public\Time_Illusion.h"
#include "Effect_Base.h"
#include "Player.h"

CTime_Illusion::CTime_Illusion()
	: CGameObject()
{
}

CTime_Illusion::CTime_Illusion(const CTime_Illusion& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTime_Illusion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTime_Illusion::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_WEAPON, this);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);

	FAILEDCHECK(SetUp_Components());

	if (nullptr != pArg)
	{
		_float4 vPos;
		memcpy(&vPos, pArg, sizeof(_float4));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&vPos));
	}

	EFFECT_POS("TimeIllusion_Dome", m_pTransformCom->Get_WorldMatrix_ptr());
	m_fTimeAcc_Effect0 = 0.f;

	//pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 0.5f, 0.5f, 1.f, 0.017f, false, true);
	//pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), {0.f,0.02f,0.01f,0.f}, 0.5f, 0.5f, false, true);
	return S_OK;
}

HRESULT CTime_Illusion::Initialize_Pooling(void* pArg)
{
	USEGAMEINSTANCE;

	m_pCol_Sphere->Clear_Colliders();

	pGameInstance->Add_TickGroup(TG_WEAPON, this);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);

	if (nullptr != pArg)
	{
		_float4 vPos;
		memcpy(&vPos, pArg, sizeof(_float4));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&vPos));
	}

	m_fAccTime = 0.f;

	EFFECT_POS("TimeIllusion_Dome", m_pTransformCom->Get_WorldMatrix_ptr());
	m_fTimeAcc_Effect0 = 0.f;
	//pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 0.5f, 0.5f, 1.f, 0.017f, false, true);
	//pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), { 0.f,0.02f,0.01f,0.f }, 0.5f, 0.5f, false, true);
	return S_OK;
}

void CTime_Illusion::Tick(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	m_fTimeAcc_Effect0 += fTimeDelta;
	USEGAMEINSTANCE;
	if (0.5f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect0 = 0.f;
		EFFECT_POS("TimeIllusion_P", m_pTransformCom->Get_WorldMatrix_ptr());
	}
	if (m_fAccTime > 7.f)
	{
		pGameInstance->Add_PoolingObject(this);
		pGameInstance->Stop_RadialBlur();
		pGameInstance->Stop_RadialCA();
	}


	for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(iter->Get_Owner());
		if (pPlayer)
		{
			pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 0.5f, 0.5f, 1.f, 0.017f, false, true);
			pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), { 0.f,0.02f,0.01f,0.f }, 0.5f, 0.5f, false, true);
		}
	}
	for (auto& iter : *m_pCol_Sphere->Get_StayColliders())
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(iter->Get_Owner());
		if (!pPlayer)
		{
			iter->Get_Owner()->Set_TiemRate(0.02f);
		}
	}
	for (auto& iter : *m_pCol_Sphere->Get_ExitColliders())
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(iter->Get_Owner());
		if (pPlayer)
		{
			pGameInstance->Stop_RadialBlur();
			pGameInstance->Stop_RadialCA();
		}
	}

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
}

void CTime_Illusion::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_EVASION), m_pCol_Sphere);
#ifdef _DEBUG
	pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
#endif // _DEBUG
}

HRESULT CTime_Illusion::Render()
{

#ifdef _DEBUG
	
	if(nullptr != m_pCol_Sphere)
		m_pCol_Sphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CTime_Illusion::SetUp_Components()
{
	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(18.5f, 18.5f, 18.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_EVASION), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

CTime_Illusion* CTime_Illusion::Create()
{
	CTime_Illusion* pInstance = new CTime_Illusion();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTime_Illusion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTime_Illusion::Clone(void* pArg)
{
	CTime_Illusion* pInstance = new CTime_Illusion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTime_Illusion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTime_Illusion::Free()
{
	__super::Free();
}
