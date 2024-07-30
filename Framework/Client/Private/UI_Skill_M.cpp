#include "stdafx.h"
#include "..\Public\UI_Skill_M.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Skill_M::CUI_Skill_M()
	: CUI()
{
}

CUI_Skill_M::CUI_Skill_M(const CUI_Skill_M & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Skill_M::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Skill_M::Initialize(void * pArg)
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
	pGameInstance->Add_CallbackList(this, EVENT_SKILLCHANGE_L);
	pGameInstance->Add_CallbackList(this, EVENT_SKILLCHANGE_R);

	m_pSkillTime_Tens = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_SKILL_TIME), ID_UINT(UI, UI_SKILL_TIME),
		&UIDESC(1132.f, 665.f, 34.f, 47.5f, 0.f, 22, 299.f));
	m_pSkillTime_Units = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_SKILL_TIME), ID_UINT(UI, UI_SKILL_TIME),
		&UIDESC(1150.f, 665.f, 34.f, 47.5f, 0.f, 22, 299.f));

	return S_OK;
}

void CUI_Skill_M::Tick(_float fTimeDelta)
{
	CUI::Tick(fTimeDelta);
	SetUp_SkillIcon();
	Change_SkillAnim(fTimeDelta);

	_uint iTempSkillTime = m_iSkillTime;
	m_pSkillTime_Units->Set_iTextureIdx(iTempSkillTime % 10);
	iTempSkillTime /= 10;
	m_pSkillTime_Tens->Set_iTextureIdx(iTempSkillTime % 10);

	if (1 > m_iSkillTime)
	{
		m_pSkillTime_Units->Set_iTextureIdx(22);
		m_pSkillTime_Tens->Set_iTextureIdx(22);
	}

	if (0 == iTempSkillTime)
	{
		m_pSkillTime_Tens->Set_iTextureIdx(22);
		m_pSkillTime_Units->Set_vUIPos({1141.f, 665.f});
	}
	else
	{
		m_pSkillTime_Units->Set_vUIPos({ 1150.f, 665.f });
	}

}

void CUI_Skill_M::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Skill_M::Render()
{
	FAILEDCHECK(CUI::Render());
	_float fTemp = 1.f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fBright", &fTemp, sizeof(_float)));
	return S_OK;
}

void CUI_Skill_M::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_SKILLCHANGE_R == tEvent.eEventType || EVENT_SKILLCHANGE_L == tEvent.eEventType)
	{
		m_bChange = true;
		m_fAccTime = 0.f;
		m_fDir = 1.f;
	}
}

void CUI_Skill_M::SetUp_SkillIcon()
{
	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	if (pPlayer)
	{
		_uint iType = pPlayer->Get_Slot_Weapon(WPN_SLOT_M);
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

		default:
			break;
		}
		m_iSkillTime = pPlayer->Get_TimeSkill_N((WEAPON_TYPE)iType);
	}
}

void CUI_Skill_M::Change_SkillAnim(_float fTimeDelta)
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

HRESULT CUI_Skill_M::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_WEAPON_SLOTICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Skill_M::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float fTemp = 1.3f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fBright", &fTemp, sizeof(_float)));
	return S_OK;
}

CUI_Skill_M * CUI_Skill_M::Create()
{
	CUI_Skill_M*		pInstance = new CUI_Skill_M();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Skill_M");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Skill_M::Clone(void * pArg)
{
	CUI_Skill_M*		pInstance = new CUI_Skill_M(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Skill_M");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Skill_M::Free()
{
	__super::Free();
}
