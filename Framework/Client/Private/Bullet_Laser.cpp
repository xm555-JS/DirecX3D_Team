#include "stdafx.h"
#include "State_Handler.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Bullet_Laser.h"

CBullet_Laser::CBullet_Laser()
	: CBullet()
{
}

CBullet_Laser::CBullet_Laser(const CBullet& rhs)
	: CBullet(rhs)
{
}

HRESULT CBullet_Laser::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_Laser::Initialize(void * pArg)
{
	m_fFrustumRange = 3.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();	

	if (FAILED(SetUp_Components()))
		__debugbreak();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	return S_OK;
}

void CBullet_Laser::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;
}

void CBullet_Laser::LateTick(_float fTimeDelta)
{
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
}

HRESULT CBullet_Laser::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	return S_OK;
}

HRESULT CBullet_Laser::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	return S_OK;
}

void CBullet_Laser::Free()
{
	__super::Free();
}
