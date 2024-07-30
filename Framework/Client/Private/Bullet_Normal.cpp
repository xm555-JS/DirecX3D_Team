#include "stdafx.h"
#include "State_Handler.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Bullet_Normal.h"

CBullet_Normal::CBullet_Normal()
	: CBullet()
{
}

CBullet_Normal::CBullet_Normal(const CBullet& rhs)
	: CBullet(rhs)
{
}

HRESULT CBullet_Normal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_Normal::Initialize(void * pArg)
{
	m_fFrustumRange = 2.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();	

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	return S_OK;
}

void CBullet_Normal::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;
}

void CBullet_Normal::LateTick(_float fTimeDelta)
{
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
}

HRESULT CBullet_Normal::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	return S_OK;
}

HRESULT CBullet_Normal::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	return S_OK;
}

void CBullet_Normal::Free()
{
	__super::Free();
}
