#include "stdafx.h"
//#include "PhysXCollider.h"
//#include "Math_Utillity.h"
//#include "Easing_Utillity.h"
#include "Sound_Manager.h"
#include "Equipment.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Player.h"
#include "Weapon.h"

CEquipment::CEquipment()
	: CGameObject()
{
}

CEquipment::CEquipment(const CEquipment& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEquipment::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEquipment::Initialize(void* pArg)
{
	m_fFrustumRange = 1.f;
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());


	/* For.Com_Model */
	ID_UINT iID;
	_uint iModelNum = *(_uint*)pArg;
	if (iModelNum > 255)
	{
		iID.tNums.Num2 = iModelNum % 255;
		iID.tNums.Num3 = iModelNum / 255;
	}
	else
	{
		iID.tNums.Num2 = iModelNum;
		iID.tNums.Num3 = NULL;
	}

	switch (iID.tNums.Num2)
	{
	case 1:
		m_eWeapon_Type = WEAPON_CUBE;
		break;
	case 2:
		m_eWeapon_Type = WEAPON_CUBE;
		break;
	case 3:
		m_eWeapon_Type = WEAPON_2GUN;
		break;
	case 5:
		m_eWeapon_Type = WEAPON_SHIELDAXE;
		break;
	case 7:
		m_eWeapon_Type = WEAPON_SWORD;
		break;
	}

	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_EQUIPMENT, iID.tNums.Num2, iID.tNums.Num3), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	return S_OK;
}

void CEquipment::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());

	Check_OwnWeapon();
	Check_EquipedWeapon();
	Select_Weapon();	
	SetUp_UsingState();
}

void CEquipment::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_EQUIPMENT), m_pCol_Sphere);
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
}

HRESULT CEquipment::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(0);

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));

		if(m_bOwnWeapon && !m_bIsUsing)
			m_pModelCom->Render(i, m_pShaderCom);
	}

#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif // _DEBUG


	_bool			bUseOutline = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bOutline", &bUseOutline, sizeof(_bool)));


	return S_OK;
}

void CEquipment::Check_OwnWeapon()
{
	USEGAMEINSTANCE;

	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;

	for (_uint i = 0; i < 4; ++i)
	{
		if (m_eWeapon_Type == pPlayer->Get_OwnWeapon()[i])
		{
			m_bOwnWeapon = true;
			break;
		}
		else
		{
			m_bOwnWeapon = false;
		}
	}

}

void CEquipment::Check_EquipedWeapon()
{
	USEGAMEINSTANCE;

	_uint iType;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	m_bEquiped_Weapon = false;

	for (_uint i = 0; i < WPN_SLOT_END; ++i)
	{
		iType = pPlayer->Get_Slot_Weapon(i);
		if (m_eWeapon_Type == iType)
		{
			m_bEquiped_Weapon = true;
			break;
		}
	}
}

void CEquipment::Select_Weapon()
{
	USEGAMEINSTANCE;

	_float fDist = 0;
	if (m_pCol_Sphere->Get_BoundingSphere()->Intersects(XMLoadFloat4(&pGameInstance->Get_CamPosition()), XMVector3Normalize(XMLoadFloat3(&pGameInstance->Get_RayDir())), fDist)
		&& KEY_DOWN(VK_LBUTTON) && m_bOwnWeapon)
	{
		pGameInstance->Add_Event(EVENT_WEAPON_CHANGE, &m_eWeapon_Type, &m_bEquiped_Weapon);
		MGR(CSound_Manager)->PlaySound(TEXT("Choose_Weapon.wav"), CSound_Manager::CHANNEL_UI, 0.32f);
	}
}

void CEquipment::SetUp_UsingState()
{
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	CPlayer::PLAYER_WEAPON ePlayerWeapon;

	switch (m_eWeapon_Type)
	{
	case WEAPON_CUBE:
		ePlayerWeapon = CPlayer::PLAYER_WEAPON_CUBE_R;
		break;

	case WEAPON_2GUN:
		ePlayerWeapon = CPlayer::PLAYER_WEAPON_GUN_R;
		break;

	case WEAPON_SHIELDAXE:
		ePlayerWeapon = CPlayer::PLAYER_WEAPON_SHIELDAXE;
		break;

	case WEAPON_SWORD:
		ePlayerWeapon = CPlayer::PLAYER_WEAPON_SWORD;
		break;
	}

	if ((pPlayer->Get_CurWeaponR()) == (pPlayer->Get_Weapon(ePlayerWeapon)))
	{
		m_bIsUsing = (pPlayer->Get_CurWeaponR())->Get_IsUsing();
	}
	else
	{
		m_bIsUsing = false;
	}

}

HRESULT CEquipment::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Collider */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.8f, 0.8f, 0.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CEquipment::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	_float			fLineBrightness = 0.8f;
	_bool			bUseOutline = true;
	_float4			vOutlineColor = { 1.f, 1.f, 1.f, 1.f };

	if (m_bEquiped_Weapon)
	{
		vOutlineColor = { 1.f, 0.2f, 0.2f, 1.f };
	}
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	if (!pPlayer->Get_InEquip_Window())
	{
		bUseOutline = false;
	}
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fLineBrightness", &fLineBrightness, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bOutline", &bUseOutline, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fOutlineColor", &vOutlineColor, sizeof(_float4)));
	
	return S_OK;
}

CEquipment* CEquipment::Create()
{
	CEquipment* pInstance = new CEquipment();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEquipment");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEquipment::Clone(void* pArg)
{
	CEquipment* pInstance = new CEquipment(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEquipment");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEquipment::Free()
{
	__super::Free();
}
