#include "stdafx.h"
#include "..\Public\UI_Minigame_Start.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Minigame_Manager.h"
#include "Level_GamePlay.h"
#include "Player.h"

CUI_Minigame_Start::CUI_Minigame_Start()
	: CUI()
{
}

CUI_Minigame_Start::CUI_Minigame_Start(const CUI_Minigame_Start & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Minigame_Start::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Minigame_Start::Initialize(void * pArg)
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

HRESULT CUI_Minigame_Start::Initialize_Pooling(void* pArg)
{
	m_fAccTime = 0.f;
	RESET_ONCE(m_bCreateReady);

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

void CUI_Minigame_Start::Tick(_float fTimeDelta)
{	
	USEGAMEINSTANCE;
	CUI::Tick(fTimeDelta);
	if (0 == m_tUIDesc.iTextureIdx)
	{
		InitBar_Anim(fTimeDelta, 340.f);
	}
	else if (1 == m_tUIDesc.iTextureIdx)
	{
		InitBar_Anim(fTimeDelta, 940.f);
	}
	else
	{
		m_fAccTime += fTimeDelta;
		if (1.f < m_fAccTime)
		{
			m_fAccTime = 0.f;
			m_tUIDesc.iTextureIdx += 1;
			if(m_iMaxTextureIdx < m_tUIDesc.iTextureIdx)
			{
				m_tUIDesc.iTextureIdx -= 1;
				pGameInstance->Add_PoolingObject(this);
				CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
				pPlayer->Set_EnableKeyInput(true);

				// 미니게임시작
				if (MINIGAME_GLIDE == CMinigame_Manager::Get_Instance()->Get_CurMinigame())
				{
					Load_UIData("Minigame_Ring");
#ifdef _MAP
					((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Ready_Layer_MiniGame(MINIGAME_GLIDE);
#endif // _MAP
				}
				else if (MINIGAME_BIKE == CMinigame_Manager::Get_Instance()->Get_CurMinigame())
				{
					Load_UIData("Minigame_Coin");
#ifdef _MAP
					((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Ready_Layer_MiniGame(MINIGAME_BIKE);
#endif // _MAP
				}
			}
		}
	}
}

void CUI_Minigame_Start::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Minigame_Start::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Minigame_Start::InitBar_Anim(_float fTimeDelta, _float fOffsetX)
{
	_vector vStartPoint = XMVectorSet(m_tUIDesc.fInitX, 0.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(fOffsetX, 1.f, 1.f, 1.f);

	_float4 vCurPoint;

	USEGAMEINSTANCE;
	m_fAccTime += fTimeDelta;
	if (1.f < m_fAccTime)
	{
		m_fAccTime = 1.f;
		pGameInstance->Add_PoolingObject(this);

	}
	if (0.5f < m_fAccTime && 500.f < fOffsetX)
	{
		IMPL_ONCE(m_bCreateReady,
			pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_MINIGAME_START), ID_UINT(UI, UI_MINIGAME_START),
				&UIDESC(640.f, 360.f, 460.f, 87.f, 0.f, 2, 50)));
	}

	XMStoreFloat4(&vCurPoint, CEasing_Utillity::ExpoOut(vStartPoint, vTargetPoint, m_fAccTime, 1.f));

	m_fX = vCurPoint.x;
}

HRESULT CUI_Minigame_Start::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_MINIGAME_START), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Minigame_Start::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());

	return S_OK;
}

CUI_Minigame_Start * CUI_Minigame_Start::Create()
{
	CUI_Minigame_Start*		pInstance = new CUI_Minigame_Start();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Minigame_Start");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Minigame_Start::Clone(void * pArg)
{
	CUI_Minigame_Start*		pInstance = new CUI_Minigame_Start(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Minigame_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Start::Free()
{
	__super::Free();
}
