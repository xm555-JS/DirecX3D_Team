#include "..\Public\Npc_Cooker.h"
#include "State_Handler.h"
#include "Model.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Quest_Manager.h"
#include "Item_Manager.h"
#include "UI.h"
#include "Player.h"
#include "Camera.h"
#include "Level_GamePlay.h"
#include "Animation.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"
#include "Channel.h"

CNpc_Cooker::CNpc_Cooker()
	: CNpc()
{
}

CNpc_Cooker::CNpc_Cooker(const CNpc& rhs)
	: CNpc(rhs)
{
}

HRESULT CNpc_Cooker::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc_Cooker::Initialize(void* pArg)
{
	FAILEDCHECK(__super::Initialize(pArg));
	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(SetUp_State_Handler());

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(284.f, 11.f, 250.f, 1.f));
	m_pTransformCom->Set_Scaled({ 1.5f,1.5f,1.5f });


	m_pModelCom->Set_CurrentAnimation(m_iAnimIdx);
	m_vecAnims = m_pModelCom->Get_Animation();


	USEGAMEINSTANCE;
	pGameInstance->Add_CallbackList(this, EVENT_COOK);
	return S_OK;
}

void CNpc_Cooker::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	IMPL_ONCE(m_bEffect5,
	EFFECT_POS("NPC_Shirley_A", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, -0.2f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(2.3f, 2.3f, 2.3f, 1.f));
	//EFFECT_POS("NPC_Shirley_A", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(2.3f, 2.3f, 2.3f, 1.f));
	EFFECT_POS("NPC_Shirley_B", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.05f, 0.6f, -0.07f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(0.5f, 0.5f, 0.5f, 0.f)););


#ifdef _UI
	Interact_Cooker();
#endif // _UI
	USEGAMEINSTANCE;

	// Cooking
	if (0 == m_iAnimIdx)
	{
		m_fCookTime += 0.4f * fTimeDelta;
		// Effect
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Cook1_Charge_P", m_pTransformCom->Get_WorldMatrix_ptr()));

		if (1.1f < m_fCookTime)
		{
			// Effect
			IMPL_ONCE(m_bEffect1, EFFECT_POS("Cook1_Done_P", m_pTransformCom->Get_WorldMatrix_ptr()));
			IMPL_ONCE(m_bEffect2, EFFECT_POS("Cook2_Done_P1", m_pTransformCom->Get_WorldMatrix_ptr()));

			m_iAnimIdx = 1;
			m_pModelCom->Set_CurrentAnimation(m_iAnimIdx);
			Create_UI_Alert();
			m_fCookTime = 0.f;

			list<CGameObject*>* pGameObjects = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_COOK_GAUGENUM));
			for (auto& pGaugeNum : *pGameObjects)
			{
				pGameInstance->Add_PoolingObject(pGaugeNum);
			}
			m_pProgress_UnitNum = nullptr;
			m_pProgress_TenNum = nullptr;
			m_pProgress_HundredNum = nullptr;
		}
	}
	// Done
	if (1 == m_iAnimIdx)
	{
		_uint iCurKeyFrame = m_pModelCom->Get_Animation()[m_iAnimIdx]->GetChannel()[1]->Get_NumCurrentKeyFrame();
		if (24 <= iCurKeyFrame)
			IMPL_ONCE(m_bEffect3, EFFECT_POS("Cook2_Jump", m_pTransformCom->Get_WorldMatrix_ptr());
			MGR(CSound_Manager)->PlaySound(TEXT("UI_Cooking_End.wav"), CSound_Manager::CHANNEL_UI, 0.40f);
			);
		if (33 <= iCurKeyFrame)
			IMPL_ONCE(m_bEffect4, EFFECT_POS("Cook2_Land", m_pTransformCom->Get_WorldMatrix_ptr()));

		if (m_vecAnims[1]->Get_IsFinished())
		{
			// Idle
			m_iAnimIdx = 2;
			m_pModelCom->Set_CurrentAnimation(m_iAnimIdx);
			pGameInstance->Able_LateTickGroup(LTG_UI_MENU);
			RESET_ONCE(m_bEffect0);
			RESET_ONCE(m_bEffect1);
			RESET_ONCE(m_bEffect2);
			RESET_ONCE(m_bEffect3);
			RESET_ONCE(m_bEffect4);

			// ¿ä¸® ¸¸µé¸é Äù½ºÆ® Å¬¸®¾î
			pGameInstance->Add_Event(EVENT_QUEST, nullptr, nullptr);
		}
	}

	if (m_pProgress_UnitNum)
	{
		SetUp_ProgressNum();
	}
}

void CNpc_Cooker::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CNpc_Cooker::Render()
{
	FAILEDCHECK(__super::Render());

	return S_OK;
}

void CNpc_Cooker::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_COOK == tEvent.eEventType)
	{
		m_iAnimIdx = 0;
		m_pModelCom->Set_CurrentAnimation(m_iAnimIdx);

		m_iFoodType = *(_uint*)tEvent.lParam;
		m_iFoodNum = *(_uint*)tEvent.wParam;

		Create_Cook_Progress();
	}
}

void CNpc_Cooker::Interact_Cooker()
{
	for (auto& iter : *m_pCol_Sphere->Get_StayColliders())
	{
		Create_UI_GuideWindow_Talk(TEXT("¿ä¸®"));
	}
	for (auto& iter : *m_pCol_Sphere->Get_ExitColliders())
	{
		Delete_UI_GuideWindow_Talk();
	}
	USEGAMEINSTANCE;
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;

	if (KEY_DOWN('F'))
	{
		if (m_GuideWindow_Talklist.size())
		{
			// ¿ä¸®Ã¢ »ý¼º, Ä«¸Þ¶ó º¯°æ
			MGR(CSound_Manager)->PlaySound(TEXT("Gain_Item.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
			CCamera* pCookCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_CookCamera();
			pGameInstance->Set_NextCamera(pCookCamera, 1.f);
			Create_Cook_WIndow();
			pGameInstance->Disable_TickGroup(TG_UI);
			pGameInstance->Disable_LateTickGroup(LTG_UI);
			pPlayer->Set_EnableKeyInput(false);
			FIXCURSOR(false);
			KEY_LOCK('F');
		}
		Delete_UI_GuideWindow_Talk();
	}
}

void CNpc_Cooker::Create_Cook_WIndow()
{
	USEGAMEINSTANCE;
	pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_FOODSELECT), ID_UINT(UI, UI_COOK_FOODSELECT),
		&CUI::UIDESC(-200.f, 360.f, 115.f, 115.f, 0.f, 6, 600));
	m_pUI_CookName = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_DETAIL), ID_UINT(UI, UI_COOK_DETAIL), // ÀÌ¸§
		&CUI::UIDESC(938.1f, 147.f, 127.3f, 50.92f, 0.f, 0, 400, { 0.f,0.f }, nullptr, 0, { 1046.f, 490.f }, { 0.4f,0.4f,0.4f,1.f }, 0.5f, false, false));
	m_pUI_CookDetail = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_DETAIL), ID_UINT(UI, UI_COOK_DETAIL), // ¼³¸í
		&CUI::UIDESC(1034.f, 254.2f, 321.9f, 138.65f, 0.f, 0, 600));
	m_pUI_Ingrd1 = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_INGREDIENT), ID_UINT(UI, UI_COOK_INGREDIENT), // Ã¹¹øÂ° Àç·á
		&CUI::UIDESC(908.f, 448.f, 59.f, 59.f, 0.f, 19, 600));
	m_pUI_Ingrd2 = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_INGREDIENT), ID_UINT(UI, UI_COOK_INGREDIENT), // µÎ¹øÂ° Àç·á
		&CUI::UIDESC(985.f, 448.f, 59.f, 59.f, 0.f, 19, 600));

	// Ã¹ ¹ø¤Š Àç·á °³¼ö
	m_pUI_IngrdNum[TENSNUM1] = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_INGRDNUM), ID_UINT(UI, UI_COOK_INGRDNUM),
		&CUI::UIDESC(894.5f, 464.f, 23.f, 31.f, 0.f, 22, 450));
	m_pUI_IngrdNum[UNITSNUM1] = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_INGRDNUM), ID_UINT(UI, UI_COOK_INGRDNUM),
		&CUI::UIDESC(906.5f, 464.f, 23.f, 31.f, 0.f, 22, 450));
	m_pUI_IngrdNum[NEEDNUM1] = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_INGRDNUM), ID_UINT(UI, UI_COOK_INGRDNUM),
		&CUI::UIDESC(927.5f, 464.f, 23.f, 31.f, 0.f, 22, 450));
	m_pUI_IngrdNum[SLASHNUM1] = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_INGRDNUM), ID_UINT(UI, UI_COOK_INGRDNUM),
		&CUI::UIDESC(919.f, 462.f, 23.f, 31.f, 0.f, 22, 450));

	// µÎ ¹øÂ° Àç·á °³¼ö
	m_pUI_IngrdNum[TENSNUM2] = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_INGRDNUM), ID_UINT(UI, UI_COOK_INGRDNUM),
		&CUI::UIDESC(972.f, 464.f, 23.f, 31.f, 0.f, 22, 450));
	m_pUI_IngrdNum[UNITSNUM2] = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_INGRDNUM), ID_UINT(UI, UI_COOK_INGRDNUM),
		&CUI::UIDESC(984.f, 464.f, 23.f, 31.f, 0.f, 22, 450));
	m_pUI_IngrdNum[NEEDNUM2] = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_INGRDNUM), ID_UINT(UI, UI_COOK_INGRDNUM),
		&CUI::UIDESC(1005.f, 464.f, 23.f, 31.f, 0.f, 22, 450));
	m_pUI_IngrdNum[SLASHNUM2] = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_INGRDNUM), ID_UINT(UI, UI_COOK_INGRDNUM),
		&CUI::UIDESC(996.5f, 462.f, 23.f, 31.f, 0.f, 22, 450));

	Load_UI("Cook_Window");
	Load_UI("Cook_Window1");
	Load_UI("Cook_Window2");
}

void CNpc_Cooker::Create_Cook_Progress()
{
	USEGAMEINSTANCE;
	pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_GAUGEBAR), ID_UINT(UI, UI_COOK_GAUGEBAR),
		&CUI::UIDESC(1160.f, 615.f, 180.f, 180.f, 0.f, 11, 600));

	pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_GAUGENUM), ID_UINT(UI, UI_COOK_GAUGENUM),
		&CUI::UIDESC(1205.f, 610.f, 70.f, 70.f, 0.f, 10, 600));
	m_pProgress_UnitNum = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_GAUGENUM), ID_UINT(UI, UI_COOK_GAUGENUM),
		&CUI::UIDESC(1166.f, 610.f, 70.f, 70.f, 0.f, 12, 600));
	m_pProgress_TenNum = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_GAUGENUM), ID_UINT(UI, UI_COOK_GAUGENUM),
		&CUI::UIDESC(1136.f, 610.f, 70.f, 70.f, 0.f, 12, 600));
	m_pProgress_HundredNum = (CUI*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_COOK_GAUGENUM), ID_UINT(UI, UI_COOK_GAUGENUM),
		&CUI::UIDESC(1106.f, 610.f, 70.f, 70.f, 0.f, 12, 600));
}

void CNpc_Cooker::Create_UI_Alert()
{
	CUI* pUI = nullptr;
	pUI = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ALERT_WINDOW), ID_UINT(UI, UI_ALERT_WINDOW),
		&CUI::UIDESC(600.f, 600.f, 400.f, 51.5f, 0.f, 0, 500.f, { 0.f,0.f }, nullptr, 0, { 465.f,586.f }, { 1.f,1.f,1.f,1.f }, 0.29f));


	(CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ALERT_ICON), ID_UINT(UI, UI_ALERT_ICON),
		&CUI::UIDESC(447.f, 602.f, 45.f, 45.f, 0.f, m_iFoodType, 490.f));

	wchar_t szTemp[MAX_PATH] = L"";

	switch (m_iFoodType)
	{
	case ITEM_BRACKEN_SOUP:
		swprintf_s(szTemp, TEXT("°í»ç¸®ÅÁx%d È¹µæ"), m_iFoodNum);
		break;

	case ITEM_CACTUS_JUICE:
		swprintf_s(szTemp, TEXT("¼±ÀÎÀå ÁÖ½ºx%d È¹µæ"), m_iFoodNum);
		break;

	case ITEM_MUSHROOM_TOASTING:
		swprintf_s(szTemp, TEXT("¹ö¼¸ººÀ½x%d È¹µæ"), m_iFoodNum);
		break;

	case ITEM_HERMITCRAB_SOUP:
		swprintf_s(szTemp, TEXT("¼Ò¶ó°ÔÅÁx%d È¹µæ"), m_iFoodNum);
		break;

	case ITEM_WHEAT_BREAD:
		swprintf_s(szTemp, TEXT("Àâ°î»§x%d È¹µæ"), m_iFoodNum);
		break;

	case ITEM_LETTUCE_SALAD:
		swprintf_s(szTemp, TEXT("»óÃß »ø·¯µåx%d È¹µæ"), m_iFoodNum);
		break;

	case ITEM_SHELL_STEW:
		swprintf_s(szTemp, TEXT("Á¶°³ Àü°ñx%d È¹µæ"), m_iFoodNum);
		break;

	case ITEM_BEET_SOUP:
		swprintf_s(szTemp, TEXT("ºñÆ® ¼öÇÁx%d È¹µæ"), m_iFoodNum);
		break;
	}

	pUI->Set_Delfault_FontText(wstring(szTemp));
}

void CNpc_Cooker::SetUp_ProgressNum()
{
	_uint iNum = m_fCookTime * 100.f;

	if (100 < iNum)
		iNum = 100;

	m_pProgress_UnitNum->Set_iTextureIdx(iNum % 10);
	iNum /= 10;
	m_pProgress_TenNum->Set_iTextureIdx(iNum % 10);
	iNum /= 10;
	m_pProgress_HundredNum->Set_iTextureIdx(iNum % 10);

	if (0 == m_pProgress_HundredNum->Get_UIDesc().iTextureIdx)
	{
		m_pProgress_HundredNum->Set_iTextureIdx(12);
		if (0 == m_pProgress_TenNum->Get_UIDesc().iTextureIdx)
		{
			m_pProgress_TenNum->Set_iTextureIdx(12);
		}
	}
}

HRESULT CNpc_Cooker::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_NPC, MODEL_COOKER), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

HRESULT CNpc_Cooker::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_IDLE, 0);

	m_pState_Handler->SetUp_StartState(STATE_NCP_SHIRLEY_IDLE);
	return S_OK;
}

CNpc_Cooker* CNpc_Cooker::Create()
{
	CNpc_Cooker* pInstance = new CNpc_Cooker();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNpc_Cooker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNpc_Cooker::Clone(void* pArg)
{
	CNpc_Cooker* pInstance = new CNpc_Cooker(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNpc_Cooker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpc_Cooker::Free()
{
	USEGAMEINSTANCE;
	__super::Free();
	pGameInstance->Remove_CallbackList(this, EVENT_COOK);
}
