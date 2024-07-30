#include "stdafx.h"
#include "..\Public\UI_Minigame_Result.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Minigame_Manager.h"
#include "Player.h"
#include "Item_Manager.h"

CUI_Minigame_Result::CUI_Minigame_Result()
	: CUI()
{
}

CUI_Minigame_Result::CUI_Minigame_Result(const CUI_Minigame_Result & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Minigame_Result::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Minigame_Result::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

HRESULT CUI_Minigame_Result::Initialize_Pooling(void* pArg)
{
	m_fAlphaRatio = 1.f;
	m_fAccTime = 0.f;
	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	return S_OK;
}

void CUI_Minigame_Result::Tick(_float fTimeDelta)
{	
	USEGAMEINSTANCE;
	CUI::Tick(fTimeDelta);
	
	if (6 == m_tUIDesc.iTextureIdx)
	{
		_uint iGainNum = CMinigame_Manager::Get_Instance()->Get_GainNum();
		wchar_t szTemp[MAX_PATH] = L"";
		swprintf_s(szTemp, TEXT("%02d"), iGainNum);
		m_strDefault_FontText = wstring(szTemp);

		RECT rcSelect;
		SetRect(&rcSelect, 0.f, 0.f, g_iWinCX, g_iWinCY);

		POINT	ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if (PtInRect(&rcSelect, ptMouse))
		{
			if (KEY_DOWN(VK_LBUTTON))
			{
				list<CGameObject*>* pGameObjectlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_MINIGAME_RESULT));

				for (auto& pResult : *pGameObjectlist)
				{
					pGameInstance->Add_PoolingObject(pResult);
				}

				FIXCURSOR(true);
				CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
				pPlayer->Set_EnableKeyInput(true);
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, iGainNum);
			}
		}
	}

	if (0 == m_tUIDesc.iTextureIdx)
	{
		BackIcon_Anim(fTimeDelta, m_tUIDesc.fInitX + 400.f);
	}
	else if (3 == m_tUIDesc.iTextureIdx)
	{
		BackIcon_Anim(fTimeDelta, m_tUIDesc.fInitX - 400.f);
	}
	else
	{
		FadeIn_Icon(fTimeDelta);
	}
}

void CUI_Minigame_Result::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Minigame_Result::Render()
{
	FAILEDCHECK(CUI::Render());
	//Font_Hancomdodum
	if (6 == m_tUIDesc.iTextureIdx && 0.95f < m_fAlphaRatio)
	{
		GAMEINSTANCE->Render_Font(TEXT("Font_Hancomdodum"), m_strDefault_FontText.c_str(), m_tUIDesc.vFontPos, XMLoadFloat4(&m_tUIDesc.vFontColor), m_tUIDesc.fFontSize);
	}
	return S_OK;
}

void CUI_Minigame_Result::BackIcon_Anim(_float fTimeDelta, _float fInitX)
{
	_vector vStartPoint = XMVectorSet(fInitX, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitX, 1.f, 1.f, 1.f);

	_float4 vCurPoint;

	USEGAMEINSTANCE;
	m_fAccTime += fTimeDelta;
	if (0.7f < m_fAccTime)
	{
		m_fAccTime = 0.7f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.7f));

	m_fX = vCurPoint.x;
}

void CUI_Minigame_Result::FadeIn_Icon(_float fTimeDelta)
{
	_vector vStartPoint = XMVectorSet(0.f, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);

	_float4 vCurPoint;

	USEGAMEINSTANCE;
	m_fAccTime += fTimeDelta;
	if (1.5f < m_fAccTime)
	{
		m_fAccTime = 0.8f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::ExpoOut(vStartPoint, vTargetPoint, m_fAccTime, 1.5f));

	m_fAlphaRatio = vCurPoint.x;
}

HRESULT CUI_Minigame_Result::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MINIGAME_RESULT), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Minigame_Result::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Minigame_Result * CUI_Minigame_Result::Create()
{
	CUI_Minigame_Result*		pInstance = new CUI_Minigame_Result();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Minigame_Result");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Minigame_Result::Clone(void * pArg)
{
	CUI_Minigame_Result*		pInstance = new CUI_Minigame_Result(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Minigame_Result");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Result::Free()
{
	__super::Free();
}
