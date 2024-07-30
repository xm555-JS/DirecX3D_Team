#include "stdafx.h"
#include "..\Public\UI_Menu_Back.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "Camera_Target_Position.h"

CUI_Menu_Back::CUI_Menu_Back()
	: CUI()
{
}

CUI_Menu_Back::CUI_Menu_Back(const CUI_Menu_Back & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Menu_Back::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Menu_Back::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 15;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Menu_Back::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetActive_MenuBack(fTimeDelta);
}

void CUI_Menu_Back::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Menu_Back::Render()
{
	if (m_bRender)
	{
		FAILEDCHECK(CUI::Render());
	}

	return S_OK;
}

void CUI_Menu_Back::SetActive_MenuBack(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;

	if (KEY_DOWN(VK_ESCAPE) && !m_bActive)
	{
		m_bActive = true;
		m_bRender = true;
		if (pPlayer)
		{
			pPlayer->Set_EnableKeyInput(false);
			FIXCURSOR(false);
		}
		MGR(CSound_Manager)->PlaySound(TEXT("UI_Open.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
	}
	else if (KEY_DOWN(VK_ESCAPE) && m_bActive)
	{
		m_bActive = false;
		m_bRender = false;
		if (pPlayer)
		{
			pPlayer->Set_EnableKeyInput(true);
			FIXCURSOR(true);
		}
		MGR(CSound_Manager)->PlaySound(TEXT("UI_Close.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
	}

	if (m_bActive)
	{
		m_fAccTime += fTimeDelta;
		if (0.55f < m_fAccTime)
		{
			m_fAccTime = 0.55f;
		}
		pGameInstance->Disable_LateTickGroup(LTG_UI_PLACENAME);
	}
	else
	{
		m_fAccTime -= fTimeDelta;
		if (0.f > m_fAccTime)
		{
			m_fAccTime = 0.f;
		}
		pGameInstance->Able_LateTickGroup(LTG_UI_PLACENAME);
	}

	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::QuarticIn(vStartPoint, vTargetPoint, m_fAccTime, 0.55f));
	m_fAlphaRatio = vCurPoint.x;
}

HRESULT CUI_Menu_Back::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MENU), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Menu_Back::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Menu_Back * CUI_Menu_Back::Create()
{
	CUI_Menu_Back*		pInstance = new CUI_Menu_Back();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Menu_Back");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Menu_Back::Clone(void * pArg)
{
	CUI_Menu_Back*		pInstance = new CUI_Menu_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Menu_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Menu_Back::Free()
{
	__super::Free();
}
