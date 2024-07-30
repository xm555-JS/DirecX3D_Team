#include "stdafx.h"
#include "..\Public\UI_Player_Hp_Back.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"

CUI_Player_Hp_Back::CUI_Player_Hp_Back()
	: CUI()
{
}

CUI_Player_Hp_Back::CUI_Player_Hp_Back(const CUI_Player_Hp_Back & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_Hp_Back::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Player_Hp_Back::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 5;


	SetUp_UIpos();

	USEGAMEINSTANCE;
	m_pPlayer = (CPlayer*)PLAYERPTR;

	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);
	pGameInstance->Add_CallbackList(this, EVENT_HPCHANGE);
	return S_OK;
}

void CUI_Player_Hp_Back::Tick(_float fTimeDelta)
{	
	CUI::Tick(fTimeDelta);
	SetUp_HpBackGauge(fTimeDelta);	
}

void CUI_Player_Hp_Back::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Player_Hp_Back::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Player_Hp_Back::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_HPCHANGE == tEvent.eEventType)
	{
		if (m_pPlayer == (CUnit*)tEvent.lParam)
		{
			m_fAccTime = 0.f;
			m_fPreHp = *(_float*)(tEvent.wParam);
			if (m_fPreHp > m_pPlayer->Get_UnitCurInfo()->fHP)//����
			{
				m_bDcrsHp = true;
				m_bIncrsHp = false;
			}
			else if (m_fPreHp < m_pPlayer->Get_UnitCurInfo()->fHP)//����
			{
				m_bDcrsHp = false;
				m_bIncrsHp = true;
			}
		}
	}
}

void CUI_Player_Hp_Back::SetUp_HpBackGauge(_float fTimeDelta)
{
	if (m_pPlayer)
	{
		// Hp�� �����ߴ���, �����ߴ����� Hp�� ������������ �������� Hp���ʿ��� 
		_float fMaxHp = m_pPlayer->Get_UnitInfo()->fHP;
		_float fCurHp = m_pPlayer->Get_UnitCurInfo()->fHP;

		if (m_bIncrsHp)
		{
			m_fSizeRatioX = fCurHp / fMaxHp;
		}
		if (m_bDcrsHp)
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

HRESULT CUI_Player_Hp_Back::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_PLAYER_STATUS), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Player_Hp_Back::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	_float2 vSizeRatio = { m_fSizeRatioX, 1.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSizeRatio", &vSizeRatio, sizeof(_float2)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bDecrease", &m_bDcrsHp, sizeof(_bool)));

	return S_OK;
}

CUI_Player_Hp_Back * CUI_Player_Hp_Back::Create()
{
	CUI_Player_Hp_Back*		pInstance = new CUI_Player_Hp_Back();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_Hp_Back");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Player_Hp_Back::Clone(void * pArg)
{
	CUI_Player_Hp_Back*		pInstance = new CUI_Player_Hp_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_Hp_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Player_Hp_Back::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
	pGameInstance->Remove_CallbackList(this, EVENT_HPCHANGE);
}
