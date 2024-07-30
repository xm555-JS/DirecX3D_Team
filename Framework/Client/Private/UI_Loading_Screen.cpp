#include "stdafx.h"
#include "..\Public\UI_Loading_Screen.h"

#include "GameInstance.h"
#include "Easing_Utillity.h"
#include "Player.h"
#include "Navigation.h"
#include "Camera.h"
#include "State_Handler.h"

#include "Level_GamePlay.h"

CUI_Loading_Screen::CUI_Loading_Screen()
	: CUI()
{
}

CUI_Loading_Screen::CUI_Loading_Screen(const CUI_Loading_Screen & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Loading_Screen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Loading_Screen::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(CUI::Initialize(&TransformDesc));
	USEGAMEINSTANCE;

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	// 로딩 텍스처 랜덤
	if (12 > m_tUIDesc.iTextureIdx)
	{
		m_tUIDesc.iTextureIdx = Random(0, 11);
	}

	SetUp_UIpos();

	if (16 == m_tUIDesc.iTextureIdx || 20 == m_tUIDesc.iTextureIdx)
	{
		pGameInstance->Add_TickGroup(TG_UI_MENU, this);
		pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);

	}
	else
	{
		pGameInstance->Add_TickGroup(TG_UI, this);
		pGameInstance->Add_LateTickGroup(LTG_UI, this);
	}

	if (LEVEL_GAMEPLAY == m_iCurLevel)
	{
		Create_Camera();
	}

	return S_OK;
}

HRESULT CUI_Loading_Screen::Initialize_Pooling(void* pArg)
{
	USEGAMEINSTANCE;

	m_fAlphaRatio = 0.f;
	m_fAccTime = 0.f;
	m_fWaitTIme = 0.f;

	RESET_ONCE(m_bWarpPlayer);

	if (nullptr != pArg)
	{
		memcpy(&m_tUIDesc, pArg, sizeof(UIDesc));
	}

	m_iPassIdx = 1;

	// 로딩 텍스처 랜덤
	if (12 > m_tUIDesc.iTextureIdx)
	{
		m_tUIDesc.iTextureIdx = Random(0, 11);
	}

	SetUp_UIpos();

	if (16 == m_tUIDesc.iTextureIdx || 20 == m_tUIDesc.iTextureIdx)
	{
		pGameInstance->Add_TickGroup(TG_UI_MENU, this);
		pGameInstance->Add_LateTickGroup(LTG_UI_MENU, this);

	}
	else
	{
		pGameInstance->Add_TickGroup(TG_UI, this);
		pGameInstance->Add_LateTickGroup(LTG_UI, this);
	}

	if (LEVEL_GAMEPLAY == m_iCurLevel)
	{
		Create_Camera();
	}

	return S_OK;
}

void CUI_Loading_Screen::Tick(_float fTimeDelta)
{
	CUI::Tick(fTimeDelta);

	if (16 == m_tUIDesc.iTextureIdx
		|| 20 == m_tUIDesc.iTextureIdx)
	{
		Move_Player(fTimeDelta);
	}
	if (21 == m_tUIDesc.iTextureIdx)
	{
		_vector vStartPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
		_float4 vCurPoint;

		m_fAccTime += fTimeDelta;
		if (0.5f < m_fAccTime)
		{
			m_fAccTime = 0.5f;
			GAMEINSTANCE->Add_PoolingObject(this);
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::sinfInOut(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
		m_fAlphaRatio = vCurPoint.x;
	}
}

void CUI_Loading_Screen::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_UI, this);
}

HRESULT CUI_Loading_Screen::Render()
{
	FAILEDCHECK(CUI::Render());

	return S_OK;
}

void CUI_Loading_Screen::Create_Camera()
{
	USEGAMEINSTANCE;

	m_pInvenCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_InvenCamera();
	m_pEquipCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_EquipCamera();
	m_pTargetCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
}

void CUI_Loading_Screen::Move_Player(_float fTimeDelta)
{
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	CTransform* pPlayerTransform = (CTransform*)pPlayer->Get_Component(TRANSFORM);
	CNavigation* pPlayerNavi = (CNavigation*)pPlayer->Get_Component(NAVIGATION);
	CState_Handler* pPlayerHandler = (CState_Handler*)pPlayer->Get_Component(STATE_HANDLER);
	_vector vStartPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
	_vector vTargetPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_float4 vCurPoint;

	m_fWaitTIme += fTimeDelta;
	if (0.15f < m_fWaitTIme)
	{
		_vector vStartPoint = XMVectorSet(1.f, 1.f, 1.f, 1.f);
		_vector vTargetPoint = XMVectorSet(0.f, 1.f, 1.f, 1.f);
		_float4 vCurPoint;

		m_fAccTime += fTimeDelta;
		if (0.5f < m_fAccTime)
		{
			m_fAccTime = 0.5f;
			GAMEINSTANCE->Add_PoolingObject(this);
		}

		XMStoreFloat4(&vCurPoint, CEasing_Utillity::sinfInOut(vStartPoint, vTargetPoint, m_fAccTime, 0.5f));
		m_fAlphaRatio = vCurPoint.x;
	}
	else
	{
		XMStoreFloat4(&vCurPoint, CEasing_Utillity::sinfInOut(vStartPoint, vTargetPoint, m_fWaitTIme, 0.15f));
		m_fAlphaRatio = vCurPoint.x;

		if (0.9f < m_fAlphaRatio)
		{
			// 메뉴 -> 인벤토리
			if (ID_UINT(UI, UI_MENU_BUTTON).iID == m_tUIDesc.pGameObject->Get_PrototypeID())
			{
				_vector vLookDir, vRightDir;
			
				if (16 == m_tUIDesc.iTextureIdx)
				{
					vLookDir = -m_pInvenCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);
					vRightDir = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookDir);
					IMPL_ONCE(m_bWarpPlayer,
						pPlayer->Keep_Realpos();
						pPlayer->Keep_RealCellIdx();
						pPlayer->Set_Pos(XMVectorSet(309.47f, 12.f, 237.67f, 1.f));
						//pPlayerNavi->Set_CurCellDesc(3076);
						pPlayerNavi->Find_CurrentIndex(pPlayer->Get_Transform());
						GAMEINSTANCE->Set_CurrentCamera(m_pInvenCamera);
						vLookDir = XMVector3Normalize(vLookDir);
						vRightDir = XMVector3Normalize(vRightDir);
						pPlayerTransform->Set_State(CTransform::STATE_LOOK, vLookDir);
						pPlayerTransform->Set_State(CTransform::STATE_RIGHT, vRightDir);
						pPlayerHandler->Set_PlayerLook(vLookDir));
				}
				else if (20 == m_tUIDesc.iTextureIdx)
				{
					vLookDir = -m_pEquipCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK);
					vRightDir = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookDir);
					IMPL_ONCE(m_bWarpPlayer,
						pPlayer->Set_InEquip_Window(true);//
						pPlayer->Keep_Realpos();
						pPlayer->Keep_RealCellIdx();
						pPlayer->Set_Pos(XMVectorSet(312.41f, 12.f, 238.2f, 1.f));
						pPlayerNavi->Find_CurrentIndex(pPlayer->Get_Transform());
						GAMEINSTANCE->Set_CurrentCamera(m_pEquipCamera);
						vLookDir = XMVector3Normalize(vLookDir);
						vRightDir = XMVector3Normalize(vRightDir);
						pPlayerTransform->Set_State(CTransform::STATE_LOOK, vLookDir);
						pPlayerTransform->Set_State(CTransform::STATE_RIGHT, vRightDir);
						pPlayerHandler->Set_PlayerLook(vLookDir));
				}
			}
			else
			{
			// 인벤토리 -> 메뉴
				IMPL_ONCE(m_bWarpPlayer,
					pPlayer->SetUp_Realpos();
					pPlayer->SetUp_RealCellIdx();
					GAMEINSTANCE->Set_CurrentCamera(m_pTargetCamera););
			}
		}
	}
	
}

HRESULT CUI_Loading_Screen::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEXUI), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_UI, TEXTURE_UI_LOADING_SCREEN), ID_UINT(TEXTURE), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom));

	return S_OK;
}

HRESULT CUI_Loading_Screen::SetUp_ShaderResource()
{
	FAILEDCHECK(CUI::SetUp_ShaderResource());
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)));

	return S_OK;
}

CUI_Loading_Screen * CUI_Loading_Screen::Create()
{
	CUI_Loading_Screen*		pInstance = new CUI_Loading_Screen();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Loading_Screen");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUI_Loading_Screen::Clone(void * pArg)
{
	CUI_Loading_Screen*		pInstance = new CUI_Loading_Screen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Loading_Screen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading_Screen::Free()
{
	__super::Free();
}
