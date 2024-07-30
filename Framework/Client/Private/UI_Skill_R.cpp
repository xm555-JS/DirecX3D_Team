#include "stdafx.h"
#include "..\Public\UI_Skill_R.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "UI.h"

CUI_Skill_R::CUI_Skill_R()
	: CUI()
{
}

CUI_Skill_R::CUI_Skill_R(const CUI_Skill_R & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Skill_R::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Skill_R::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	pGameInstance->Add_CallbackList(this, EVENT_SKILLCHANGE_R);

	m_pShortKey = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_SHORTKEY), ID_UINT(UI, UI_SHORTKEY),
		&UIDesc(1235.f, 704.f, 17.5f, 17.5f, 0.f, 9.f, 299.f));

	m_pSkillTime = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_SKILL_TIME), ID_UINT(UI, UI_SKILL_TIME),
		&UIDESC(1233.f, 665.f, 34.f, 47.5f, 0.f, 22, 299.f));

	return S_OK;
}

void CUI_Skill_R::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_SkillIcon();
	Change_SkillAnim(fTimeDelta);
	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;

	if (pPlayer)
	{
		m_iSkillTime = pPlayer->Get_TimeChangeWeapon(WPN_SLOT_R);
		m_pSkillTime->Set_iTextureIdx(m_iSkillTime);
		if (0 == m_iSkillTime)
		{
			m_pSkillTime->Set_iTextureIdx(22);
		}
	}
}

void CUI_Skill_R::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Skill_R::Render()
{
	FAILEDCHECK(CUI::Render());
	_float fTemp = 1.f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fBright", &fTemp, sizeof(_float)));
	return S_OK;
}

void CUI_Skill_R::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_SKILLCHANGE_R == tEvent.eEventType)
	{
		m_bChange = true;
		m_fAccTime = 0.f;
		m_fDir = 1.f;
		MGR(CSound_Manager)->PlaySound(TEXT("UI_Skill_Swap.wav"), CSound_Manager::CHANNEL_UI, 0.3f);
	}
}

void CUI_Skill_R::SetUp_SkillIcon()
{
	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;

	if (pPlayer && m_pShortKey)
	{
		_uint iType = pPlayer->Get_Slot_Weapon(WPN_SLOT_R);
		switch (iType)
		{
		case WEAPON_SWORD:
			m_tUIDesc.iTextureIdx = 2;
			break;

		case WEAPON_2GUN:
			m_tUIDesc.iTextureIdx = 0;
			break;

		case WEAPON_CUBE:
			m_tUIDesc.iTextureIdx = 1;
			break;

		case WEAPON_SHIELDAXE:
			m_tUIDesc.iTextureIdx = 3;
			break;

		case WEAPON_NONE:
			m_tUIDesc.iTextureIdx = 4;
			break;

		default:
			break;
		}

		_uint iKey = pPlayer->Get_Slot_WeaponKey(WPN_SLOT_R);
		switch (iKey)
		{
		case 'Q':
			m_pShortKey->Set_iTextureIdx(9);
			break;

		case 'E':
			m_pShortKey->Set_iTextureIdx(8);
			break;

		case 'R':
			m_pShortKey->Set_iTextureIdx(10);
			break;

		default:
			break;
		}
	}
}

void CUI_Skill_R::Change_SkillAnim(_float fTimeDelta)
{
	if (m_bChange)
	{
		m_fAccTime += m_fDir * fTimeDelta;
		_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitSizeX, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(0.01f, 1.f, 1.f, 1.f);
		_float4 vCurPoint;

		if (0.3f < m_fAccTime)
		{
			m_fAccTime = 0.3f;
			m_fDir *= -1.f;
		}
		if (0.f > m_fAccTime)
		{
			m_bChange = false;
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicInOut(vStartPoint, vTargetPoint, m_fAccTime, 0.3f));
		m_fSizeX = vCurPoint.x;
	}
	if (!m_bChange)
	{
		m_fSizeX = m_tUIDesc.fInitSizeX;
	}
}

HRESULT CUI_Skill_R::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_WEAPON_SLOTICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Skill_R::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	
	_float fTemp;
	if (0 < m_iSkillTime)
	{
		fTemp = 0.5f;
	}
	else
	{
		fTemp = 1.3f;
	}
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fBright", &fTemp, sizeof(_float)));
	return S_OK;
}

CUI_Skill_R * CUI_Skill_R::Create()
{
	CUI_Skill_R*		pInstance = new CUI_Skill_R();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Skill_R");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Skill_R::Clone(void * pArg)
{
	CUI_Skill_R*		pInstance = new CUI_Skill_R(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Skill_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Skill_R::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
	pGameInstance->Remove_CallbackList(this, EVENT_SKILLCHANGE_R);
}
