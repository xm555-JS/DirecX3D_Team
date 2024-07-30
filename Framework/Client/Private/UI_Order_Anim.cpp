#include "stdafx.h"
#include "..\Public\UI_Order_Anim.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"

CUI_Order_Anim::CUI_Order_Anim()
	: CUI_Order()
{
}

CUI_Order_Anim::CUI_Order_Anim(const CUI_Order_Anim & rhs)
	: CUI_Order(rhs)
{
}

HRESULT CUI_Order_Anim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Order_Anim::Initialize(void * pArg)
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
	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);
	m_iSoundChannel = MGR(CSound_Manager)->PlaySound(TEXT("UI_Order_Anim.wav"), CSound_Manager::CHANNEL_UI, 0.4f);
	return S_OK;
}

HRESULT CUI_Order_Anim::Initialize_Pooling(void* pArg)
{
	m_fAlphaTime = 0.f;
	m_fSizeTime = 0.f;
	m_fMoveTime = 0.f;
	m_fFrameTime = 0.f;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;


	SetUp_UIpos();

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_UI_ORDERLIST, this);
	pGameInstance->Add_LateTickGroup(LTG_UI_ORDERLIST, this);
	m_iSoundChannel = MGR(CSound_Manager)->PlaySound(TEXT("UI_Order_Anim.wav"), CSound_Manager::CHANNEL_UI, 0.45f);
	return S_OK;
}

void CUI_Order_Anim::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	Move_Frame(0.028f, fTimeDelta);

	USEGAMEINSTANCE;
	RECT rcSelect;
	SetRect(&rcSelect, 1158.f, 12.f, 1270.f, 65.f);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	
	if (PtInRect(&rcSelect, ptMouse))
	{
		if (KEY_DOWN(VK_LBUTTON))
		{
			pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(ORDER), ID_UINT(ORDER), &m_bTen);
			pGameInstance->Add_PoolingObject(this);
			//MGR(CSound_Manager)->SetUp_FadeOutSound((CSound_Manager::CHANNELID)m_iSoundChannel,0.0f, 0.5f);
			MGR(CSound_Manager)->StopSound((CSound_Manager::CHANNELID)m_iSoundChannel);
			//MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
		}
	}
	if (m_tUIDesc.iTextureIdx == m_iMaxTextureIdx)
	{
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(ORDER), ID_UINT(ORDER), &m_bTen);
		pGameInstance->Add_PoolingObject(this);
		MGR(CSound_Manager)->StopSound((CSound_Manager::CHANNELID)m_iSoundChannel);
	}

	MGR(CSound_Manager)->Set_Volume(CSound_Manager::BGM, 0.f);
}

void CUI_Order_Anim::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Order_Anim::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

HRESULT CUI_Order_Anim::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_ORDER_ANIM), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Order_Anim::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));
	return S_OK;
}

CUI_Order_Anim * CUI_Order_Anim::Create()
{
	CUI_Order_Anim*		pInstance = new CUI_Order_Anim();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Order_Anim");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Order_Anim::Clone(void * pArg)
{
	CUI_Order_Anim*		pInstance = new CUI_Order_Anim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Order_Anim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Order_Anim::Free()
{
	__super::Free();
}
