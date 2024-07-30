#include "stdafx.h"
#include "..\Public\UI_Weapon_Skill.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "UI_Skill_M.h"

CUI_Weapon_Skill::CUI_Weapon_Skill()
	: CUI()
{
}

CUI_Weapon_Skill::CUI_Weapon_Skill(const CUI_Weapon_Skill & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Weapon_Skill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Weapon_Skill::Initialize(void * pArg)
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
	return S_OK;
}

void CUI_Weapon_Skill::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	USEGAMEINSTANCE;
	IMPL_ONCE(m_bOnce, m_pSkill_M = (CUI_Skill_M*)pGameInstance->Get_Object(m_iCurLevel, ID_UINT(UI, UI_SKILL_M)));
	m_tUIDesc.iTextureIdx = m_pSkill_M->Get_UIDesc().iTextureIdx;
}

void CUI_Weapon_Skill::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Weapon_Skill::Render()
{
	FAILEDCHECK(CUI::Render());
	_float fTemp = 1.f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fBright", &fTemp, sizeof(_float)));
	return S_OK;
}

HRESULT CUI_Weapon_Skill::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_SKILL_SLOTICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Weapon_Skill::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	if (m_pSkill_M && 0 < m_pSkill_M->Get_SkillTime())
	{
		_float fTemp = 0.5f;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fBright", &fTemp, sizeof(_float)));
	}

	return S_OK;
}

CUI_Weapon_Skill * CUI_Weapon_Skill::Create()
{
	CUI_Weapon_Skill*		pInstance = new CUI_Weapon_Skill();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Weapon_Skill");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Weapon_Skill::Clone(void * pArg)
{
	CUI_Weapon_Skill*		pInstance = new CUI_Weapon_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Weapon_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Weapon_Skill::Free()
{
	__super::Free();
}
