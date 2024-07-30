#include "stdafx.h"
#include "..\Public\UI_Minigame_Gain.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Minigame_Manager.h"
#include "Player.h"
#include "State_Handler.h"

CUI_Minigame_Gain::CUI_Minigame_Gain()
	: CUI()
{
}

CUI_Minigame_Gain::CUI_Minigame_Gain(const CUI_Minigame_Gain & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Minigame_Gain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Minigame_Gain::Initialize(void * pArg)
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

HRESULT CUI_Minigame_Gain::Initialize_Pooling(void* pArg)
{
	m_iMinute = 0;
	m_iSecond = 0;
	RESET_ONCE(m_bSettingTime);
	m_fAccTime = 0.f;
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

void CUI_Minigame_Gain::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	//m_strDefault_FontText

	if (0 == m_tUIDesc.iTextureIdx || 1 == m_tUIDesc.iTextureIdx)
	{
		_uint iGainNum = CMinigame_Manager::Get_Instance()->Get_GainNum();
		wchar_t szTemp[MAX_PATH] = L"";
		swprintf_s(szTemp, TEXT("x %02d"), iGainNum);
		m_strDefault_FontText = wstring(szTemp);
	}
	else if (3 == m_tUIDesc.iTextureIdx)
	{
		if (MINIGAME_GLIDE == CMinigame_Manager::Get_Instance()->Get_CurMinigame())
		{
			SetUp_Timer(TIME_UP, fTimeDelta);
			
		}
		else if (MINIGAME_BIKE == CMinigame_Manager::Get_Instance()->Get_CurMinigame())
		{
			SetUp_Timer(TIME_DOWN, fTimeDelta , 1, 5);
		}

		wchar_t szTemp[MAX_PATH] = L"";
		swprintf_s(szTemp, TEXT("%02d : %02d"), m_iMinute, m_iSecond);
		m_strDefault_FontText = wstring(szTemp);
	}
}

void CUI_Minigame_Gain::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Minigame_Gain::Render()
{
	FAILEDCHECK(CUI::Render());
	if (0 == m_tUIDesc.iTextureIdx || 1 == m_tUIDesc.iTextureIdx || 3 == m_tUIDesc.iTextureIdx)
	{
		GAMEINSTANCE->Render_Font(TEXT("Font_Hancomdodum_Bold"), m_strDefault_FontText.c_str(), m_tUIDesc.vFontPos, XMLoadFloat4(&m_tUIDesc.vFontColor), m_tUIDesc.fFontSize);
	}
	return S_OK;
}

void CUI_Minigame_Gain::SetUp_Timer(TIMER_TYPE eType, _float fTimeDelta, _int iMinute, _int iSecond)
{
	m_fAccTime += fTimeDelta;
	if (TIME_UP == eType)
	{
		if (1.f < m_fAccTime)
		{
			m_fAccTime = 0.f;
			++m_iSecond;
		}
		if (60 == m_iSecond)
		{
			m_iSecond = 0;
			++m_iMinute;
		}
		_float4 vPos;
		CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
		XMStoreFloat4(&vPos, pPlayer->Get_Pos());
		if (30.2f > vPos.y)
		{
			CMinigame_Manager::Get_Instance()->Finish_Minigame();
			CMinigame_Manager::Get_Instance()->Set_FinishedGame(MINIGAME_GLIDE);
			MGR(CSound_Manager)->PlaySound(TEXT("Player_GliderOff.mp3"), CSound_Manager::CHANNEL_PLAYER, 0.9f);
		}

	}
	else if (TIME_DOWN == eType)
	{
		IMPL_ONCE(m_bSettingTime,
			m_iMinute = iMinute;
			m_iSecond = iSecond);
	
		if (1.f < m_fAccTime)
		{
			m_fAccTime = 0.f;
			--m_iSecond;
		}
		if (0 > m_iSecond)
		{
			--m_iMinute;
			m_iSecond = 59;
		}
		if (0 == m_iMinute && 0 == m_iSecond)
		{
			// °ÔÀÓ ³¡
			CMinigame_Manager::Get_Instance()->Finish_Minigame();
			CMinigame_Manager::Get_Instance()->Set_FinishedGame(MINIGAME_BIKE);
			MGR(CSound_Manager)->PlaySound(TEXT("Player_GliderOff.mp3"), CSound_Manager::CHANNEL_PLAYER, 0.9f);
		}
	}
}

HRESULT CUI_Minigame_Gain::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MINIGAME_GAIN), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Minigame_Gain::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Minigame_Gain * CUI_Minigame_Gain::Create()
{
	CUI_Minigame_Gain*		pInstance = new CUI_Minigame_Gain();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Minigame_Gain");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Minigame_Gain::Clone(void * pArg)
{
	CUI_Minigame_Gain*		pInstance = new CUI_Minigame_Gain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Minigame_Gain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Gain::Free()
{
	__super::Free();
}
