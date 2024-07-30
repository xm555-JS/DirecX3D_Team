#include "..\Public\Npc_Shirley.h"
#include "State_Handler.h"
#include "Model.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Level_GamePlay.h"

#include "Quest_Manager.h"
#include "Item_Manager.h"
#include "Minigame_Manager.h"
#include "UI_PlaceMark.h"
#include "UI_Quest_Complete.h"

#include "Player.h"
#include "Enemy.h"
#include "Portal.h"

#include "Effect_Manager.h"
#include "Navigation.h"

#include "Imgui_Manager.h"
#include "Camera.h"
#include "Camera_Action.h"

CNpc_Shirley::CNpc_Shirley()
	: CNpc()
{
}

CNpc_Shirley::CNpc_Shirley(const CNpc& rhs)
	: CNpc(rhs)
{
}

HRESULT CNpc_Shirley::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc_Shirley::Initialize(void* pArg)
{
	FAILEDCHECK(__super::Initialize(pArg));
	FAILEDCHECK(SetUp_Components());
	FAILEDCHECK(SetUp_State_Handler());

	// 테스트용 위치
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(289.f, 11.f, 234.f, 1.f));

	// 실제 위치
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(276.5f, 12.8f, 244.5f, 1.f));

	USEGAMEINSTANCE;

#ifdef _UI

	// PlaceMark1 Able

	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
		&CUI::UIDESC(200.f, 200.f, 38.f, 38.f, 0.f, 0, 400));
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
		&CUI::UIDESC(200.f, 200.f, 43.f, 43.f, 0.f, 1, 400));
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
		&CUI::UIDESC(200.f, 200.f, 32.f, 32.f, 0.f, 2, 400));
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
		&CUI::UIDESC(200.f, 200.f, 27.f, 27.f, 0.f, 3, 400));

	list<CGameObject*>* pGameObjectlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK));
	for (auto& pPlaceMark : *pGameObjectlist)
	{
		((CUI_PlaceMark*)pPlaceMark)->Set_Able(true);
		((CUI_PlaceMark*)pPlaceMark)->Set_GameObject(this);
		((CUI_PlaceMark*)pPlaceMark)->Set_Offset_TargetPosY(2.f);
	}

#endif // _UI

	pGameInstance->Add_CallbackList(this, EVENT_MONSTER_DEAD);
	pGameInstance->Add_CallbackList(this, EVENT_QUEST);

	m_pUI_Complete[0] = (CUI_Quest_Complete*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_COMPLETE), ID_UINT(UI, UI_QUEST_COMPLETE),
		&CUI::UIDESC(1174.f, 192.f, 199.92f, 38.73f, 0.f, 2, 800));
	m_pUI_Complete[1] = (CUI_Quest_Complete*)pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_COMPLETE), ID_UINT(UI, UI_QUEST_COMPLETE),
		&CUI::UIDESC(1185.f, 195.f, 200.f, 32.14f, 0.f, 5, 790));

	return S_OK;
}

void CNpc_Shirley::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	IMPL_ONCE(m_bEffect0,
		EFFECT_POS("NPC_Shirley_A", m_pTransformCom->Get_WorldMatrix_ptr());
	EFFECT_POS("NPC_Shirley_B", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.05f, 0.6f, -0.07f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(0.5f, 0.5f, 0.5f, 0.f)););

	Quest();				// 퀘스트 알림
	Script();				// 대화
	Non_Script_Quest();		// 스크립트가 없는 퀘스트
	Collider_ToPlayer();	// 플레이어랑 충돌
}

void CNpc_Shirley::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CNpc_Shirley::Render()
{
	FAILEDCHECK(__super::Render());

	return S_OK;
}

void CNpc_Shirley::On_EventMessege(EVENTDESC tEvent)
{
	USEGAMEINSTANCE;
	if (EVENT_MONSTER_DEAD == tEvent.eEventType)
	{
		if (ID_UINT(ENEMY, ENEMY_NORMAL, HYENAGANG_MINIONS).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID()
			|| ID_UINT(ENEMY, ENEMY_NORMAL, HYENAGANG_SHOOTER).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID()
			|| ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_GUNNER).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID()
			|| ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_DOUBLEEDGEDSWORDADEPT).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID()
			|| ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_SWORDGHOST).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID()
			|| ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID()
			|| ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION_MECHANIC).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID())
		{
			++m_iDead_Normal;
		}
		else if (ID_UINT(ENEMY, ENEMY_NORMAL, GANGSTER_CAP).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID()
			|| ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_DOORKEEPER).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID()
			|| ID_UINT(ENEMY, ENEMY_NORMAL, HABAKA_ADMIRAL).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID())
		{
			++m_iDead_Eliete;
		}
		else if (ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_ROBELLA).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID()
			|| ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID()
			|| ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_WEAVER).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID())
		{
			++m_iDead_Boss;
		}
	}

	if (EVENT_QUEST == tEvent.eEventType)
	{
		++m_iQuestEvent;
	}

}

HRESULT CNpc_Shirley::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_NPC, MODEL_SHIRLEY), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

HRESULT CNpc_Shirley::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	//STATE_NCP_SHIRLEY_BRILLIANT,
	//	STATE_NCP_SHIRLEY_GREETING,
	//	STATE_NCP_SHIRLEY_HUNGRY,
	//	STATE_NCP_SHIRLEY_SURPRISE,
	//	STATE_NCP_SHIRLEY_THANK,
	//	STATE_NCP_SHIRLEY_CONVERSATION_A,
	//	STATE_NCP_SHIRLEY_CONVERSATION_B,
	//	STATE_NCP_SHIRLEY_CONVERSATION_C,
	//	STATE_NCP_SHIRLEY_WORRY_A,
	//	STATE_NCP_SHIRLEY_WORRY_B,

	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_IDLE, 0);
	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_BRILLIANT, 13);
	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_GREETING, 5);
	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_HUNGRY, 12);
	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_SURPRISE, 3);
	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_THANK, 6);
	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_CONVERSATION_A, 1);
	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_CONVERSATION_B, 2);
	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_CONVERSATION_C, 10);
	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_WORRY_A, 4);
	m_pState_Handler->Set_Animation(STATE_NCP_SHIRLEY_WORRY_B, 11);

	m_pState_Handler->SetUp_StartState(STATE_NCP_SHIRLEY_IDLE);
	return S_OK;
}

void CNpc_Shirley::Quest()
{
	USEGAMEINSTANCE;

	switch (CQuest_Manager::Get_Instance()->Get_CurQuset())
	{

	case QUEST_CUBE_ORDER:
	{
		// 뽑기를 하면 퀘스트 완료
		if (m_iQuestEvent >= 1)
		{
			// 퀘스트를 받은 상태 -> 퀘스트를 완료 가능 상태
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;

			m_iQuestEvent = 0;
		}
		break;
	}

	case QUEST_COLLECT_LETTUCE:
	{
		// 상추 5개 가져오기
		_uint iQuestItem = CItem_Manager::Get_Instance()->Find_ItemNum(ITEM_LETTUCE);

		if (iQuestItem >= 5)
		{
			// 5개 가져오면 퀘스트 완료
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;
		}
		/*else if (!bIsQuist && m_bScript)
		{
			CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
		}*/
		break;
	}

	case QUEST_KILL_GANG_SWARM:
	{
		// 갱 패거리를 처치하면 퀘스트 완료
		if (m_iDead_Normal >= 4)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;
		}
		/*else if (!bIsQuist && m_bScript)
		{
			CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
		}*/
		break;
	}

	case QUEST_KILL_GANG_CAP:
	{
		// 갱스터 캡을 처치하면 퀘스트 완료
		if (m_iDead_Eliete >= 2)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;
		}
		/*else if (!bIsQuist && m_bScript)
		{
			CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
		}*/
		break;
	}

	case QUEST_KILL_ROBELLA:
	{
		// 로벨라를 처치하면 퀘스트 완료
		if (m_iDead_Boss >= 1)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;

			CPortal::PORTALDESC PortalDesc;
			PortalDesc.m_vPos = _float4(138.44f, 22.38f, 118.12f, 1.f);
			PortalDesc.m_vTargetPos = _float4(295.f, 0.f, 240.f, 1.f);
			PortalDesc.m_bUseAble = true;
			PortalDesc.m_ePortalType = PORTAL_VILLAGE;

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL), ID_UINT(MAP, MAP_PORTAL), &PortalDesc);
			m_iDead_Boss = 0;
		}
		/*else if (!bIsQuist && m_bScript)
		{
			CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
		}*/
		break;
	}

	case QUEST_MAKE_COOKING:
	{
		// 요리를 하면 완료
		if (m_iQuestEvent >= 1)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;

			// 퀘스트 변수 초기화
			m_iDead_Normal = 0;
			m_iDead_Eliete = 0;
			m_iDead_Boss = 0;
			m_iQuestEvent = 0;
		}
		break;
	}

	case QUEST_ORDER:
	{
		// 뽑기를 하면 퀘스트 완료
		if (m_iQuestEvent >= 1)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;

			m_iQuestEvent = 0;
		}
		break;
	}

	case QUEST_KILL_MECHANICALCITY_SWARM:
	{
		// 메카닉 패거리 그 하수인들을 잡으면 완료
		if (m_iDead_Normal >= 5)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;
		}
		break;
	}

	case QUEST_KILL_DOORKEEPER:
	{
		// 도어키퍼를 그 하수인들을 잡으면 완료
		if (m_iDead_Eliete >= 2)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;
			m_iDead_Eliete = 0.f;
			// 포탈생성
			CPortal::PORTALDESC PortalDesc;
			PortalDesc.m_vPos = _float4(148.39f, 87.2f, 457.f, 1.f);
			PortalDesc.m_vTargetPos = _float4(100.f, 160.f, 435.27, 1.f);
			PortalDesc.m_bUseAble = true;
			PortalDesc.m_ePortalType = PORTAL_DRAGON;

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL), ID_UINT(MAP, MAP_PORTAL), &PortalDesc);

			CCamera::CAMERADESC CameraDesc;
			CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
			CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
			CameraDesc.fFovy = XMConvertToRadians(60.0f);
			CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
			CameraDesc.fNear = g_fNear;
			CameraDesc.fFar = g_fFar;
			CCamera_Action* pActionCamera = nullptr;
			NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
			pActionCamera->Reserve_NumActionDesc(3);
			pActionCamera->Set_TargetObejct(pGameInstance->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL))->back());
			pActionCamera->Set_StartDesc(_float3(-1.f, 5.f, 11.f), _float3(0.f, 3.f, 0.f));
			pActionCamera->Set_ActionCamDesc(_float3(-1.f, 6.f, 4.f), _float3(0.f, 3.f, 0.f), 15, 2.5f, 1);
			pActionCamera->Set_EndDesc(_float3(-1.f, 7.f, 10.f), _float3(0.f, 3.f, 0.f), 11, 2.f);
			pGameInstance->Add_Camera(TEXT("Dragon_Portal_Create"), pActionCamera);
			pGameInstance->Play_ActionCamera(TEXT("Dragon_Portal_Create"), 0.f);
		}
		break;
	}

	case QUEST_KILL_JUMPING_FROSTDRAGON:
	{
		// 프로스트 드래곤을 잡으면 완료
		if (m_iDead_Boss >= 1)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;
			m_iDead_Boss = 0;

			// 미니게임 시작
			MGR(CMinigame_Manager)->Notice_Minigame();
		}
		break;
	}

	case QUEST_MAKE_COOKING_1:
	{
		// 요리를 하면 완료
		if (m_iQuestEvent >= 1)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;

			// 퀘스트 변수 초기화
			m_iDead_Normal = 0;
			m_iDead_Eliete = 0;
			m_iDead_Boss = 0;
			m_iQuestEvent = 0;
		}
		break;
	}

	case QUEST_KILL_MUTATION:
	{
		// 돌연변이 몬스터를 잡으면 퀘스트 완료
		if (m_iDead_Normal >= 6)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;
		}
		break;
	}

	case QUEST_KILL_HABAKA:
	{
		// 하바카 몬스터를 잡으면 퀘스트 완료
		if (m_iDead_Eliete >= 2)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;
			m_iDead_Eliete = 0.f;

			CPortal::PORTALDESC PortalDesc;
			PortalDesc.m_vPos = _float4(437.04f, 54.77f, 307.399f, 1.f);
			PortalDesc.m_vTargetPos = _float4(436.21f, 153.46f, 308.08f, 1.f);
			PortalDesc.m_bUseAble = true;
			PortalDesc.m_ePortalType = PORTAL_WEAVER;

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL), ID_UINT(MAP, MAP_PORTAL), &PortalDesc); // 웨버

			CCamera::CAMERADESC CameraDesc;
			CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
			CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
			CameraDesc.fFovy = XMConvertToRadians(60.0f);
			CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
			CameraDesc.fNear = g_fNear;
			CameraDesc.fFar = g_fFar;
			CCamera_Action* pActionCamera = nullptr;
			NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
			pActionCamera->Reserve_NumActionDesc(3);
			pActionCamera->Set_TargetObejct(pGameInstance->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL))->back());
			pActionCamera->Set_StartDesc(_float3(-3.f, 5.f, 0.f), _float3(0.f, 3.f, 0.f));
			pActionCamera->Set_ActionCamDesc(_float3(-4.f, 4.f, 0.f), _float3(0.f, 3.f, 0.f), 15, 2.5f, 1);
			pActionCamera->Set_EndDesc(_float3(-8.f, 7.f, 0.f), _float3(0.f, 3.f, 0.f), 11, 2.f);
			pGameInstance->Add_Camera(TEXT("Weaver_Portal_Create"), pActionCamera);
			pGameInstance->Play_ActionCamera(TEXT("Weaver_Portal_Create"), 0.f);
		}
		break;
	}

	case QUEST_KILL_WEAVER:
	{
		// 웨버를 잡으면 완료
		if (m_iDead_Boss >= 1)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;

			// 퀘스트 변수 초기화
			m_iDead_Normal = 0;
			m_iDead_Eliete = 0;
			m_iDead_Boss = 0;
			m_iQuestEvent = 0;
		}
		break;
	}

	}
}

void CNpc_Shirley::Non_Script_Quest()
{
	// 스크립트가 없는 퀘스트
	// 
	USEGAMEINSTANCE;

	switch (CQuest_Manager::Get_Instance()->Get_CurQuset())
	{
	case QUEST_KILL_GANG_SWARM:

		if (m_bQuestClear == true)
		{
			// 스크립트가 없는 퀘스트를 완료할 준비가 됨
			m_bNonScript = true;

			Show_UI_Complete();

			if (KEY_UP('F'))
			{
				CQuest_Manager::Get_Instance()->Complete_Quest();

				// 원래 상태로 초기화
				m_bQuestClear = false;
				Hide_UI_Complete();

				// 보상
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// 바로 다음 퀘스트 진행
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
			}
		}
		break;

	case QUEST_KILL_GANG_CAP:

		if (m_bQuestClear == true)
		{
			Show_UI_Complete();
			if (KEY_UP('F'))
			{
				CQuest_Manager::Get_Instance()->Complete_Quest();

				// 원래 상태로 초기화
				m_bQuestClear = false;
				Hide_UI_Complete();


				// 보상
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// 바로 다음 퀘스트 진행
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
			}
		}
		break;

	case QUEST_KILL_ROBELLA:

		if (m_bQuestClear == true)
			m_bNonScript = false;

		Able_UI_PlaceMark();

		break;

	case QUEST_KILL_MECHANICALCITY_SWARM:

		if (m_bQuestClear == true)
		{
			// 스크립트가 없는 퀘스트를 완료할 준비가 됨
			m_bNonScript = true;

			Show_UI_Complete();

			if (KEY_UP('F'))
			{
				CQuest_Manager::Get_Instance()->Complete_Quest();

				// 원래 상태로 초기화
				m_bQuestClear = false;
				Hide_UI_Complete();

				// 보상
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// 바로 다음 퀘스트 진행
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
			}
		}
		break;

	case QUEST_KILL_DOORKEEPER:

		if (m_bQuestClear == true)
		{
			Show_UI_Complete();

			if (KEY_UP('F'))
			{
				CQuest_Manager::Get_Instance()->Complete_Quest();

				// 원래 상태로 초기화
				m_bQuestClear = false;
				Hide_UI_Complete();

				// 보상
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// 바로 다음 퀘스트 진행
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
			}
		}
		break;

	case QUEST_KILL_JUMPING_FROSTDRAGON:

		if (m_bQuestClear == true)
			m_bNonScript = false;
		Able_UI_PlaceMark();

		break;

	case QUEST_KILL_MUTATION:

		if (m_bQuestClear == true)
		{
			// 스크립트가 없는 퀘스트를 완료할 준비가 됨
			m_bNonScript = true;

			Show_UI_Complete();

			if (KEY_UP('F'))
			{
				CQuest_Manager::Get_Instance()->Complete_Quest();

				// 원래 상태로 초기화
				m_bQuestClear = false;
				Hide_UI_Complete();

				// 보상
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// 바로 다음 퀘스트 진행
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
			}
		}
		break;

	case QUEST_KILL_HABAKA:

		if (m_bQuestClear == true)
		{
			Show_UI_Complete();
			if (KEY_UP('F'))
			{
				CQuest_Manager::Get_Instance()->Complete_Quest();

				// 원래 상태로 초기화
				m_bQuestClear = false;
				Hide_UI_Complete();

				// 보상
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// 바로 다음 퀘스트 진행
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
			}
		}
		break;

	case QUEST_KILL_WEAVER:

		if (m_bQuestClear == true)
			m_bNonScript = false;
		Able_UI_PlaceMark();

		break;

	}
}

void CNpc_Shirley::Script()
{
	USEGAMEINSTANCE;

	CGameObject* pPlayer = PLAYERPTR;

	if (m_bScript == false)
		return;

	// 스크립트는 클릭을 하면 다음 스크립트로 간다.
	if (KEY_UP(VK_LBUTTON) && !m_bQuestPlay)
	{
		Delete_Talk_Window();
		++m_iScriptPhase;
		MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
	}

	// 만약 퀘스트 중에 말을 걸었다면?
	if (m_bQuestPlay && m_bScript)
	{
		if (KEY_UP(VK_LBUTTON))
		{
			//pGameInstance->Able_TickGroup(TG_UI);
			pGameInstance->Able_LateTickGroup(LTG_UI);
			Delete_Talk_Window();
			m_bScript = false;
			((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

			// 카메라
			CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
			pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);

			return;
		}

		//Delete_Talk_Window();
		Create_Talk_Window(NPC, TEXT("셜리"), TEXT("다시 예전처럼 평화로웠으면.."));
		pGameInstance->Disable_LateTickGroup(LTG_UI);
		return;
	}
	else
	{
		// 대화내용
		switch (CQuest_Manager::Get_Instance()->Get_CurQuset())
		{

		case QUEST_CUBE_ORDER:

			if (m_iScriptPhase == CUBE_ORDER_SCRIPT_1)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("안녕"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_2)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("처음 보는 얼굴이네 나는 셜리라고 해 너는?"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_3)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("나는 윤창이라고 해"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_4)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("우리 섬에 잘 왔어 근데 시기는 별로 좋지 않네\n지금 섬이 뒤숭숭하거든"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_5)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("근데... 너 가지고 있는 무기가 하나네?\n여기 주변은 너무 위험해 내가 재료를 하나 줄께"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_6)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("주문을 통해서 무기를 하나 뽑아봐"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_7)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("그래 알겠어"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_8)
			{
				pGameInstance->Able_LateTickGroup(LTG_UI);
				++m_iScriptPhase;
				m_bScript = false;
				m_bQuestPlay = true;
				((CPlayer*)pPlayer)->Set_EnableKeyInput(true);
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Quest_Receive.wav"), CSound_Manager::CHANNEL_UI, 0.1f);

				// 카메라
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);

				// 뽑기 재화 주기
				m_iBlackCoreNum = 1;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// PlaceMark1 Disable
				Pooling_UI_PlaceMark();
			}

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == CUBE_ORDER_SCRIPT_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("셜리"), TEXT("어때 조금은 든든하지?"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// 아이템을 다 모은 상태로 상호작용했을 때 퀘스트 완료
					CQuest_Manager::Get_Instance()->Complete_Quest();
					CItem_Manager::Get_Instance()->Use_Item(ITEM_LETTUCE, 5);
					// 원래 상태로 초기화
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// 보상
					m_iBlackCoreNum = 5;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// 카메라
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_COLLECT_LETTUCE:

			if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_1)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("또 보네?"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_2)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("혹시 시간 있어? 너무 바빠서 그런데 날 도와줄 수 있을까?"));
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_3)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("보기만 해도 바빠 보이네?\n내가 뭘 도와주면 돼?"));
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_4)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("섬 주위에 있는 상추를 5개만 구해줘"));
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_5)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("그래 알겠어"));
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_6)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("고마워!"));
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_7)
			{
				pGameInstance->Able_LateTickGroup(LTG_UI);
				++m_iScriptPhase;
				m_bScript = false;
				m_bQuestPlay = true;
				((CPlayer*)pPlayer)->Set_EnableKeyInput(true);
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Quest_Receive.wav"), CSound_Manager::CHANNEL_UI, 0.1f);

				// 카메라
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
			}

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("셜리"), TEXT("엇? 왔구나! 상추가.. 하나.. 둘.. 셋.. 다섯! 딱 맞네 정말 고마워"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// 아이템을 다 모은 상태로 상호작용했을 때 퀘스트 완료
					CQuest_Manager::Get_Instance()->Complete_Quest();
					CItem_Manager::Get_Instance()->Use_Item(ITEM_LETTUCE, 5);
					// 원래 상태로 초기화
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// 보상
					m_iBlackCoreNum = 5;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// 카메라
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_KILL_GANG_SWARM:

			if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_1)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("(셜리가 바쁘게 일을 하고 있다)"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_2)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("셜리야 궁금했던 건데 왜 이렇게 바쁜 거야?"));
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_3)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("여기 있는지도 몰랐네..\n사실 요즘 몬스터들이 무언갈 준비하는 거 같아\n문제는 뭔지는 모르겠어\n단순한 영역 다툼인지 전쟁인지..."));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_4)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("전쟁..? 듣고 나니 엄청 큰 문제잖아"));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_5)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("말이 나온 김에 혹시 한 번 더 나를 도와줄 수 있을까?\n앞쪽 섬에 갱스터 패거리들을 처리해 줘"));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_6)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("알겠어\n나도 널 도울게 다른 도움이 필요하면 말해"));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_7)
			{
				pGameInstance->Able_LateTickGroup(LTG_UI);
				++m_iScriptPhase;
				m_bScript = false;
				m_bQuestPlay = true;
				((CPlayer*)pPlayer)->Set_EnableKeyInput(true);
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Quest_Receive.wav"), CSound_Manager::CHANNEL_UI, 0.1f);

				// 카메라
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);

				Able_UI_PlaceMark();
			}
			break;

		case QUEST_KILL_ROBELLA:

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == KILL_ROBELLA_SCRIPT_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("셜리"), TEXT("갱스터 패거리들이 이런 큰 무기를 숨기고 있었다니...\n단순한 영역 다툼은 아닌 거 같아"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == KILL_ROBELLA_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// 아이템을 다 모은 상태로 상호작용했을 때 퀘스트 완료
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// 원래 상태로 초기화
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// 보상
					m_iBlackCoreNum = 10;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// 카메라
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);

				}
			}
			break;

		case QUEST_MAKE_COOKING:

			if (m_iScriptPhase == MAKE_COOKING_SCRIPT_1)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("윤창아 마침 잘 왔어 혹시 섬을 돌아다니면서 재료 채집은 하고 있어?"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT_2)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("응 지금 여러 가지 재료 갖고 있어"));
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT_3)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("그럼 여기 주변에 요리 로봇한데 가서 요리를 만들어봐!\n네가 갱스터 패거리를 처리하러 갔을 때\n혹시나 많이 다치치는 않을까 걱정을 했거든..."));
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT_4)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("요리를 미리 만들어 놓으면 그냥 재료보다 더 맛도 있고, 너의 회복이 좀 더 도움이 될 꺼야!"));
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT_5)
			{
				pGameInstance->Able_LateTickGroup(LTG_UI);
				++m_iScriptPhase;
				m_iQuestEvent = 0;
				m_bScript = false;
				m_bQuestPlay = true;
				((CPlayer*)pPlayer)->Set_EnableKeyInput(true);
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Quest_Receive.wav"), CSound_Manager::CHANNEL_UI, 0.1f);

				// 카메라
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
			}

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == MAKE_COOKING_SCRIPT_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("셜리"), TEXT("음~ 냄새...\n이 음식 먹어봤어? 맛이 없을 수가 없는 냄새야.. 못 참겠다!"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == MAKE_COOKING_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// 아이템을 다 모은 상태로 상호작용했을 때 퀘스트 완료
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// 원래 상태로 초기화
					m_iQuestEvent = 0;
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// 보상
					m_iBlackCoreNum = 5;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// 카메라
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_ORDER:

			if (m_iScriptPhase == ORDER_SCRIPT_1)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("윤창아 저번에 그 무기는 잘 쓰고 있어?"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == ORDER_SCRIPT_2)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("응 저번에 전투에서 유용하게 썼어"));
			}
			else if (m_iScriptPhase == ORDER_SCRIPT_3)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("다행이다! 내가 준 블랙 코어가 좀 모이지 않았어?\n무기뿐만 아니라 다양한 재료도 나오니까 다시 한 번 해봐!"));
			}
			else if (m_iScriptPhase == ORDER_SCRIPT_4)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("분명 너의 성장에 필요한 것들이 나올 거야"));
			}
			else if (m_iScriptPhase == ORDER_SCRIPT_5)
			{
				pGameInstance->Able_LateTickGroup(LTG_UI);
				++m_iScriptPhase;
				m_iQuestEvent = 0;
				m_bScript = false;
				m_bQuestPlay = true;
				((CPlayer*)pPlayer)->Set_EnableKeyInput(true);
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Quest_Receive.wav"), CSound_Manager::CHANNEL_UI, 0.1f);

				// 카메라
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
			}

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == ORDER_SCRIPT_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("셜리"), TEXT("어때 너한데 필요한 것들이 나왔어?"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == ORDER_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// 아이템을 다 모은 상태로 상호작용했을 때 퀘스트 완료
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// 원래 상태로 초기화
					m_iQuestEvent = 0;
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// 보상
					m_iBlackCoreNum = 5;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// 카메라
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_KILL_MECHANICALCITY_SWARM:

			if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_1)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("음... 여기도 심상치 않네"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_2)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("셜리야 또 무슨 일이야?"));
			}
			else if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_3)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("이번엔 갱 녀석들 말고 다른 영역에 있는 기계 패거리들이 문제가 있어\n근데 상태가 이상해 마치 공포에 떠는 듯한..\n모든 몬스터들이 성격이 날카로워졌어"));
			}
			else if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_3)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("내가 상태를 보고 올께"));
			}
			else if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_4)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("조심해 위험하면 저번에 코어로 뽑은 무기를 사용해 봐!"));
			}
			else if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_5)
			{
				pGameInstance->Able_LateTickGroup(LTG_UI);
				++m_iScriptPhase;
				m_bScript = false;
				m_bQuestPlay = true;
				((CPlayer*)pPlayer)->Set_EnableKeyInput(true);
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Quest_Receive.wav"), CSound_Manager::CHANNEL_UI, 0.1f);

				// 카메라
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				Able_UI_PlaceMark();
			}
			break;

		case QUEST_KILL_JUMPING_FROSTDRAGON:

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == KILL_FROST_DRAGON_SCRIPT_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("셜리"), TEXT("프로스트 드래곤이 공격을?\n네가 상황을 보러 갔을 때 조사를 했는데\n확실한 건 몬스터들이 어떤 것에 공포를 느끼는 거 같아"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == KILL_FROST_DRAGON_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// 아이템을 다 모은 상태로 상호작용했을 때 퀘스트 완료
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// 원래 상태로 초기화
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// 보상
					m_iBlackCoreNum = 10;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// 카메라
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_MAKE_COOKING_1:

			if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_1)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("아직 몬스터에 관한 일이 해결된 건 아니지만! 금강산도 식후경, 이번에도 혹시...?"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_2)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("맞아 바위가 많은 섬에서 재료를 가져왔어"));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_3)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("좋아..좋아 이번엔 그 재료로 다른 요리를 만들어보자"));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_4)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("다 만들어지면 나 한입만... 히히"));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_5)
			{
				pGameInstance->Able_LateTickGroup(LTG_UI);
				++m_iScriptPhase;
				m_iQuestEvent = 0;
				m_bScript = false;
				m_bQuestPlay = true;
				((CPlayer*)pPlayer)->Set_EnableKeyInput(true);
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Quest_Receive.wav"), CSound_Manager::CHANNEL_UI, 0.1f);

				// 카메라
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
			}

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("셜리"), TEXT("엇 한 입주는 거야? 고마워!\n...음? 이게 한 입이냐고?\n미안 너무 맛있어서 그만.."));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// 아이템을 다 모은 상태로 상호작용했을 때 퀘스트 완료
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// 원래 상태로 초기화
					m_iQuestEvent = 0;
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// 보상
					m_iBlackCoreNum = 5;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// 카메라
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_KILL_MUTATION:

			if (m_iScriptPhase == KILL_MUTATION_SCRIPT_1)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("윤창아 몬스터들의 공포감이 왜 생겨났는지 알아낸 거 같아"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_2)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("무슨 이유 때문에 몬스터들이 저렇게 겁을 먹은 거야?"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_3)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("원인은 새로운 세력의 적대감과 동시에 강력한 힘에 대한 공포인 거 같아\n최근에 나타난 세력이 단기간에 엄청 강해지고 있거든"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_4)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("그 녀석들은 어떻게 단기간에 강해진 거야?"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_5)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("강해진 이유는 사람들을 이용하고 있어"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_6)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("사람? 사람을 이용해서 어떻게 강해지는 거야?"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_7)
			{
				Create_Talk_Window(NPC, TEXT("셜리"), TEXT("사람들을 감염시켜서 자신들의 세력에 쓰고 있거든...\n그렇게 강력한 세력이라면 다른 몬스터들에게 공포감을 주는 것도 가능할 거야 아마도.."));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_8)
			{
				Create_Talk_Window(PLAYER, TEXT("주인공"), TEXT("꺼림직한 녀석들이네...\n만약 이 녀석들이 원인이라면 가만둘 수 없지"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_9)
			{
				pGameInstance->Able_LateTickGroup(LTG_UI);
				++m_iScriptPhase;
				m_bScript = false;
				m_bQuestPlay = true;
				((CPlayer*)pPlayer)->Set_EnableKeyInput(true);
				CQuest_Manager::Get_Instance()->Receive_Quest(m_iCurLevel);
				MGR(CSound_Manager)->PlaySound(TEXT("UI_Quest_Receive.wav"), CSound_Manager::CHANNEL_UI, 0.1f);

				// 카메라
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				Able_UI_PlaceMark();
			}
			break;

		case QUEST_KILL_WEAVER:

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == KILL_WEAVER_SCRIPT_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("셜리"), TEXT("웨버 세력이 원인이 아니라면 그럼 왜 공포감을 느끼는 거지..?"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == KILL_WEAVER_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// 아이템을 다 모은 상태로 상호작용했을 때 퀘스트 완료
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// 원래 상태로 초기화
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// 보상
					m_iBlackCoreNum = 10;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// 카메라
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;
		}
	}
}

void CNpc_Shirley::Collider_ToPlayer()
{
	USEGAMEINSTANCE;

	CGameObject* pPlayer = PLAYERPTR;

	// 플레이어랑 충돌했을 때
	for (auto& iter : *m_pCol_Sphere->Get_StayColliders())
	{
		// 만약에 Non_Script 퀘스트를 하고 있으면 상호작용 못하게
		if (m_bNonScript == true)
			return;

		// F키로 상호작용하라는 UI 생성
		Create_UI_GuideWindow_Talk(TEXT("셜리"));

		if (KEY_DOWN('F'))
		{
			MGR(CSound_Manager)->PlaySound(TEXT("Gain_Item.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
			// 스크립트 시작
			m_bScript = true;
			((CPlayer*)pPlayer)->Set_EnableKeyInput(false);

			// 카메라 변경
			CCamera* pCookCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_ShirleyCamera();
			pGameInstance->Set_NextCamera(pCookCamera, 1.f);
		}
		if (KEY_UP('F'))
		{
			// F키로 상호작용하라는 UI, F키를 누르면 삭제
			Delete_UI_GuideWindow_Talk();
		}
	}
	for (auto& iter : *m_pCol_Sphere->Get_ExitColliders())
	{
		// F키로 상호작용하라는 UI, 충돌이 끝났을 때 이 UI 삭제
		Delete_UI_GuideWindow_Talk();
	}
}

void CNpc_Shirley::Create_UI_Alert()
{
	CUI* pUI = nullptr;
	pUI = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ALERT_WINDOW), ID_UINT(UI, UI_ALERT_WINDOW),
		&CUI::UIDESC(600.f, 600.f, 400.f, 51.5f, 0.f, 0, 500.f, { 0.f,0.f }, nullptr, 0, { 465.f,586.f }, { 1.f,1.f,1.f,1.f }, 0.29f));


	(CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_ALERT_ICON), ID_UINT(UI, UI_ALERT_ICON),
		&CUI::UIDESC(447.f, 602.f, 45.f, 45.f, 0.f, ITEM_BLACKCORE, 490.f));

	wchar_t szTemp[MAX_PATH] = L"";
	swprintf_s(szTemp, TEXT("블랙 코어x%d 획득"), m_iBlackCoreNum);

	pUI->Set_Delfault_FontText(wstring(szTemp));
}

void CNpc_Shirley::Pooling_UI_PlaceMark()
{
	USEGAMEINSTANCE;
	list<CGameObject*>* pGameObjectlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK));
	for (auto& pPlaceMark : *pGameObjectlist)
	{
		pGameInstance->Add_PoolingObject(pPlaceMark);
	}
	/*pGameObjectlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_DIST_NUM));
	for (auto& pDist_Num : *pGameObjectlist)
	{
		pGameInstance->Add_PoolingObject(pDist_Num);
	}*/
}

void CNpc_Shirley::Able_UI_PlaceMark()
{
	USEGAMEINSTANCE;
	list<CGameObject*>* pGameObjectlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK));
	for (auto& pPlaceMark : *pGameObjectlist)
	{
		((CUI_PlaceMark*)pPlaceMark)->Set_Able(true);
	}
}

void CNpc_Shirley::Show_UI_Complete()
{
	if (m_bShow_UI_Complete)
	{
		m_pUI_Complete[0]->Set_TimeDir(1.f);
		m_pUI_Complete[1]->Set_TimeDir(1.f);
		m_bShow_UI_Complete = false;
	}
}

void CNpc_Shirley::Hide_UI_Complete()
{
	MGR(CSound_Manager)->PlaySound(TEXT("UI_Quest_Complete.wav"), CSound_Manager::CHANNEL_UI, 0.3f);
	m_pUI_Complete[0]->Set_TimeDir(-1.f);
	m_pUI_Complete[1]->Set_TimeDir(-1.f);
	m_bShow_UI_Complete = true;
}

CNpc_Shirley* CNpc_Shirley::Create()
{
	CNpc_Shirley* pInstance = new CNpc_Shirley();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNpc_Shirley");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNpc_Shirley::Clone(void* pArg)
{
	CNpc_Shirley* pInstance = new CNpc_Shirley(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNpc_Shirley");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpc_Shirley::Free()
{
	__super::Free();

	USEGAMEINSTANCE;
	pGameInstance->Remove_CallbackList(this, EVENT_MONSTER_DEAD);
	pGameInstance->Remove_CallbackList(this, EVENT_QUEST);
}
