#include "stdafx.h"
#include "..\Public\UI_Minigame_Explain.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Minigame_Manager.h"
#include "Player.h"

CUI_Minigame_Explain::CUI_Minigame_Explain()
	: CUI()
{
}

CUI_Minigame_Explain::CUI_Minigame_Explain(const CUI_Minigame_Explain & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Minigame_Explain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Minigame_Explain::Initialize(void * pArg)
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

HRESULT CUI_Minigame_Explain::Initialize_Pooling(void* pArg)
{
	m_fAccTime = 0.f;
	m_fAlphaRatio = 1.f;

	m_fSizeTime = 0.f;
	m_fTimeDir = 1.f;

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

void CUI_Minigame_Explain::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	if (1 == m_tUIDesc.iTextureIdx)
	{
		MinigameWindow_Anim(fTimeDelta);
	}
	else
	{
		MinigameExplain_Anim(fTimeDelta);
	}

	if (0 == m_tUIDesc.iTextureIdx)
	{
		Interact_Button(fTimeDelta);
	}
}

void CUI_Minigame_Explain::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Minigame_Explain::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Minigame_Explain::Create_Minigame_Explain()
{
	if (MINIGAME_GLIDE == CMinigame_Manager::Get_Instance()->Get_CurMinigame())
	{
		Load_UIData("Minigame_Glide");
	}
	else if (MINIGAME_BIKE == CMinigame_Manager::Get_Instance()->Get_CurMinigame())
	{
		Load_UIData("Minigame_Bike");
	}
}

void CUI_Minigame_Explain::MinigameWindow_Anim(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fAccTime += fTimeDelta;

	_vector vStartPoint = XMVectorSet(1.f, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitSizeY, 1.f, 1.f, 1.f);

	_float4 vCurPoint;
	if (0.3f < m_fAccTime)
	{
		m_fAccTime = 0.3f;
		Create_Minigame_Explain();
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.3f));

	m_fSizeY = vCurPoint.x;
}

void CUI_Minigame_Explain::MinigameExplain_Anim(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fAccTime += fTimeDelta;

	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);

	_float4 vCurPoint;
	if (0.5f < m_fAccTime)
	{
		m_fAccTime = 0.5f;
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));

	m_fAlphaRatio = vCurPoint.x;
}

void CUI_Minigame_Explain::Interact_Button(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	RECT rcSelect;
	SetRect(&rcSelect, m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f
		, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rcSelect, ptMouse))
	{
		//버튼 커졌다 작아지는거아이템쪽에 잇ㅇ륻스

		if (KEY_DOWN(VK_LBUTTON))
		{
			MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
			list<CGameObject*>* pGameObejctlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_MINIGAME_EXPLAIN));

			for (auto& pExplain : *pGameObejctlist)
			{
				pGameInstance->Add_PoolingObject(pExplain);
			}

			// Minigame_Start 생성
			FIXCURSOR(true);
			/*CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
			pPlayer->Set_EnableKeyInput(true);*/
			pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_MINIGAME_START), ID_UINT(UI, UI_MINIGAME_START),
				&UIDESC(580.f, 355.f, 125.f, 117.f, 0.f, 0, 50));
			pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_MINIGAME_START), ID_UINT(UI, UI_MINIGAME_START),
				&UIDESC(700.f, 355.f, 125.f, 117.f, 0.f, 1, 50));
		}
	}
}

HRESULT CUI_Minigame_Explain::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MINIGAME_EXPLAIN), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Minigame_Explain::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Minigame_Explain * CUI_Minigame_Explain::Create()
{
	CUI_Minigame_Explain*		pInstance = new CUI_Minigame_Explain();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Minigame_Explain");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Minigame_Explain::Clone(void * pArg)
{
	CUI_Minigame_Explain*		pInstance = new CUI_Minigame_Explain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Minigame_Explain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Explain::Free()
{
	__super::Free();
}
