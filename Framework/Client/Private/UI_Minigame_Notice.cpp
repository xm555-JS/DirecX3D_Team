#include "stdafx.h"
#include "..\Public\UI_Minigame_Notice.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Minigame_Manager.h"
#include "Player.h"

CUI_Minigame_Notice::CUI_Minigame_Notice()
	: CUI()
{
}

CUI_Minigame_Notice::CUI_Minigame_Notice(const CUI_Minigame_Notice & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Minigame_Notice::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Minigame_Notice::Initialize(void * pArg)
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
	m_fY = m_tUIDesc.fInitY - 350.f;
	MGR(CSound_Manager)->PlaySound(TEXT("UI_Minigame_Notice.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);

	return S_OK;
}

HRESULT CUI_Minigame_Notice::Initialize_Pooling(void* pArg)
{
	m_fAccTime = 0.f;
	m_fTimeDir = 1.f;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 0;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	m_fY = m_tUIDesc.fInitY - 350.f;
	MGR(CSound_Manager)->PlaySound(TEXT("UI_Minigame_Notice.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);

	return S_OK;
}

void CUI_Minigame_Notice::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	USEGAMEINSTANCE;
	m_fAccTime += fTimeDelta * m_fTimeDir;

	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitY - 350.f, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(m_tUIDesc.fInitY, 1.f, 1.f, 1.f);

	_float4 vCurPoint;
	if (0.8f < m_fAccTime)
	{
		m_fAccTime = 0.8f;
	}
	if (0.f > m_fAccTime)
	{
		m_fAccTime = 0.f;
		list<CGameObject*>* pGameObejctlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_MINIGAME_NOTICE));

		for (auto& pNotice : *pGameObejctlist)
		{
			pGameInstance->Add_PoolingObject(pNotice);
		}

		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_MINIGAME_EXPLAIN), ID_UINT(UI, UI_MINIGAME_EXPLAIN),
			&UIDESC(640.f, 360.f, 1040.f, 450.f, 0.f, 1, 50));

		/*CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
		pPlayer->Set_EnableKeyInput(false);*/
		FIXCURSOR(false);
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, m_fAccTime, 0.8f));

	m_fY = vCurPoint.x;

	RECT rcSelect;
	SetRect(&rcSelect, 0.f, 0.f, g_iWinCX, g_iWinCY);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rcSelect, ptMouse))
	{
		if (1 == m_tUIDesc.iTextureIdx && KEY_DOWN(VK_LBUTTON))
		{
			list<CGameObject*>* pGameObejctlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_MINIGAME_NOTICE));

			for (auto& pNotice : *pGameObejctlist)
			{
				((CUI_Minigame_Notice*)pNotice)->Set_TimeDir(-1);
			}
			CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
			pPlayer->Set_EnableKeyInput(false);
		}	
	}
}

void CUI_Minigame_Notice::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Minigame_Notice::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Minigame_Notice::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MINIGAME_ICON), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Minigame_Notice::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Minigame_Notice * CUI_Minigame_Notice::Create()
{
	CUI_Minigame_Notice*		pInstance = new CUI_Minigame_Notice();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Minigame_Notice");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Minigame_Notice::Clone(void * pArg)
{
	CUI_Minigame_Notice*		pInstance = new CUI_Minigame_Notice(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Minigame_Notice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Notice::Free()
{
	__super::Free();
}
