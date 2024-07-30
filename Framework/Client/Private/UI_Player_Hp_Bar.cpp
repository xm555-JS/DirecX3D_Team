#include "stdafx.h"
#include "..\Public\UI_Player_Hp_Bar.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Player_Hp_Bar::CUI_Player_Hp_Bar()
	: CUI()
{
}

CUI_Player_Hp_Bar::CUI_Player_Hp_Bar(const CUI_Player_Hp_Bar & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_Hp_Bar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Player_Hp_Bar::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 6;


	SetUp_UIpos();

	USEGAMEINSTANCE;
	m_pPlayer = (CPlayer*)PLAYERPTR;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	pGameInstance->Add_CallbackList(this, EVENT_HPCHANGE);

	return S_OK;
}

void CUI_Player_Hp_Bar::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_HpGauge(fTimeDelta);
}

void CUI_Player_Hp_Bar::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Player_Hp_Bar::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Player_Hp_Bar::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_HPCHANGE == tEvent.eEventType)
	{
		if (m_pPlayer == (CUnit*)tEvent.lParam)
		{
			m_fAccTime = 0.f;
			m_fPreHp = *(_float*)(tEvent.wParam);
			if (m_fPreHp > m_pPlayer->Get_UnitCurInfo()->fHP)//감소
			{
				m_bDcrsHp = true;
				m_bIncrsHp = false;
			}
			else if (m_fPreHp < m_pPlayer->Get_UnitCurInfo()->fHP)//증가
			{
				m_bDcrsHp = false;
				m_bIncrsHp = true;
			}
		}
	}
}

void CUI_Player_Hp_Bar::SetUp_HpGauge(_float fTimeDelta)
{
	if (m_pPlayer)
	{
		// Hp가 감소했는지, 증가했는지랑 Hp가 증감소했을때 증감소전 Hp가필요함 
		_float fMaxHp = m_pPlayer->Get_UnitInfo()->fHP;
		_float fCurHp = m_pPlayer->Get_UnitCurInfo()->fHP;

		if (m_bDcrsHp)
		{
			m_fSizeRatioX = fCurHp / fMaxHp;
		}
		if (m_bIncrsHp)
		{
			m_fAccTime += fTimeDelta;
			if (0.5f < m_fAccTime)
			{
				m_fAccTime = 0.5f;
			}
			_vector vStartPoint = XMVectorSet(m_fPreHp, 1.f, 1.f, 1.f);
			_vector vTargetPoint = XMVectorSet(fCurHp, 1.f, 1.f, 1.f);
			_float4 vCurPoint;
			XMStoreFloat4(&vCurPoint, CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
			m_fSizeRatioX = vCurPoint.x / fMaxHp;
		}
	}
}

HRESULT CUI_Player_Hp_Bar::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_PLAYER_STATUS), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Player_Hp_Bar::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 0.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));

	return S_OK;
}

CUI_Player_Hp_Bar * CUI_Player_Hp_Bar::Create()
{
	CUI_Player_Hp_Bar*		pInstance = new CUI_Player_Hp_Bar();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_Hp_Bar");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Player_Hp_Bar::Clone(void * pArg)
{
	CUI_Player_Hp_Bar*		pInstance = new CUI_Player_Hp_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_Hp_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Player_Hp_Bar::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
	pGameInstance->Remove_CallbackList(this, EVENT_HPCHANGE);
}
