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

	// �׽�Ʈ�� ��ġ
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(289.f, 11.f, 234.f, 1.f));

	// ���� ��ġ
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

	Quest();				// ����Ʈ �˸�
	Script();				// ��ȭ
	Non_Script_Quest();		// ��ũ��Ʈ�� ���� ����Ʈ
	Collider_ToPlayer();	// �÷��̾�� �浹
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
		// �̱⸦ �ϸ� ����Ʈ �Ϸ�
		if (m_iQuestEvent >= 1)
		{
			// ����Ʈ�� ���� ���� -> ����Ʈ�� �Ϸ� ���� ����
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;

			m_iQuestEvent = 0;
		}
		break;
	}

	case QUEST_COLLECT_LETTUCE:
	{
		// ���� 5�� ��������
		_uint iQuestItem = CItem_Manager::Get_Instance()->Find_ItemNum(ITEM_LETTUCE);

		if (iQuestItem >= 5)
		{
			// 5�� �������� ����Ʈ �Ϸ�
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
		// �� �аŸ��� óġ�ϸ� ����Ʈ �Ϸ�
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
		// ������ ĸ�� óġ�ϸ� ����Ʈ �Ϸ�
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
		// �κ��� óġ�ϸ� ����Ʈ �Ϸ�
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
		// �丮�� �ϸ� �Ϸ�
		if (m_iQuestEvent >= 1)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;

			// ����Ʈ ���� �ʱ�ȭ
			m_iDead_Normal = 0;
			m_iDead_Eliete = 0;
			m_iDead_Boss = 0;
			m_iQuestEvent = 0;
		}
		break;
	}

	case QUEST_ORDER:
	{
		// �̱⸦ �ϸ� ����Ʈ �Ϸ�
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
		// ��ī�� �аŸ� �� �ϼ��ε��� ������ �Ϸ�
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
		// ����Ű�۸� �� �ϼ��ε��� ������ �Ϸ�
		if (m_iDead_Eliete >= 2)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;
			m_iDead_Eliete = 0.f;
			// ��Ż����
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
		// ���ν�Ʈ �巡���� ������ �Ϸ�
		if (m_iDead_Boss >= 1)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;
			m_iDead_Boss = 0;

			// �̴ϰ��� ����
			MGR(CMinigame_Manager)->Notice_Minigame();
		}
		break;
	}

	case QUEST_MAKE_COOKING_1:
	{
		// �丮�� �ϸ� �Ϸ�
		if (m_iQuestEvent >= 1)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;

			// ����Ʈ ���� �ʱ�ȭ
			m_iDead_Normal = 0;
			m_iDead_Eliete = 0;
			m_iDead_Boss = 0;
			m_iQuestEvent = 0;
		}
		break;
	}

	case QUEST_KILL_MUTATION:
	{
		// �������� ���͸� ������ ����Ʈ �Ϸ�
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
		// �Ϲ�ī ���͸� ������ ����Ʈ �Ϸ�
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

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL), ID_UINT(MAP, MAP_PORTAL), &PortalDesc); // ����

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
		// ������ ������ �Ϸ�
		if (m_iDead_Boss >= 1)
		{
			CQuest_Manager::Get_Instance()->Achieve_Quest();
			m_bQuestClear = true;
			m_bQuestPlay = false;

			// ����Ʈ ���� �ʱ�ȭ
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
	// ��ũ��Ʈ�� ���� ����Ʈ
	// 
	USEGAMEINSTANCE;

	switch (CQuest_Manager::Get_Instance()->Get_CurQuset())
	{
	case QUEST_KILL_GANG_SWARM:

		if (m_bQuestClear == true)
		{
			// ��ũ��Ʈ�� ���� ����Ʈ�� �Ϸ��� �غ� ��
			m_bNonScript = true;

			Show_UI_Complete();

			if (KEY_UP('F'))
			{
				CQuest_Manager::Get_Instance()->Complete_Quest();

				// ���� ���·� �ʱ�ȭ
				m_bQuestClear = false;
				Hide_UI_Complete();

				// ����
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// �ٷ� ���� ����Ʈ ����
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

				// ���� ���·� �ʱ�ȭ
				m_bQuestClear = false;
				Hide_UI_Complete();


				// ����
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// �ٷ� ���� ����Ʈ ����
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
			// ��ũ��Ʈ�� ���� ����Ʈ�� �Ϸ��� �غ� ��
			m_bNonScript = true;

			Show_UI_Complete();

			if (KEY_UP('F'))
			{
				CQuest_Manager::Get_Instance()->Complete_Quest();

				// ���� ���·� �ʱ�ȭ
				m_bQuestClear = false;
				Hide_UI_Complete();

				// ����
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// �ٷ� ���� ����Ʈ ����
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

				// ���� ���·� �ʱ�ȭ
				m_bQuestClear = false;
				Hide_UI_Complete();

				// ����
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// �ٷ� ���� ����Ʈ ����
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
			// ��ũ��Ʈ�� ���� ����Ʈ�� �Ϸ��� �غ� ��
			m_bNonScript = true;

			Show_UI_Complete();

			if (KEY_UP('F'))
			{
				CQuest_Manager::Get_Instance()->Complete_Quest();

				// ���� ���·� �ʱ�ȭ
				m_bQuestClear = false;
				Hide_UI_Complete();

				// ����
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// �ٷ� ���� ����Ʈ ����
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

				// ���� ���·� �ʱ�ȭ
				m_bQuestClear = false;
				Hide_UI_Complete();

				// ����
				m_iBlackCoreNum = 5;
				CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
				Create_UI_Alert();

				// �ٷ� ���� ����Ʈ ����
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

	// ��ũ��Ʈ�� Ŭ���� �ϸ� ���� ��ũ��Ʈ�� ����.
	if (KEY_UP(VK_LBUTTON) && !m_bQuestPlay)
	{
		Delete_Talk_Window();
		++m_iScriptPhase;
		MGR(CSound_Manager)->PlaySound(TEXT("UI_Click.wav"), CSound_Manager::CHANNEL_UI, 0.5f);
	}

	// ���� ����Ʈ �߿� ���� �ɾ��ٸ�?
	if (m_bQuestPlay && m_bScript)
	{
		if (KEY_UP(VK_LBUTTON))
		{
			//pGameInstance->Able_TickGroup(TG_UI);
			pGameInstance->Able_LateTickGroup(LTG_UI);
			Delete_Talk_Window();
			m_bScript = false;
			((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

			// ī�޶�
			CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
			pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);

			return;
		}

		//Delete_Talk_Window();
		Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�ٽ� ����ó�� ��ȭ�ο�����.."));
		pGameInstance->Disable_LateTickGroup(LTG_UI);
		return;
	}
	else
	{
		// ��ȭ����
		switch (CQuest_Manager::Get_Instance()->Get_CurQuset())
		{

		case QUEST_CUBE_ORDER:

			if (m_iScriptPhase == CUBE_ORDER_SCRIPT_1)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�ȳ�"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_2)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("ó�� ���� ���̳� ���� �ȸ���� �� �ʴ�?"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_3)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("���� ��â�̶�� ��"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_4)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�츮 ���� �� �Ծ� �ٵ� �ñ�� ���� ���� �ʳ�\n���� ���� �ڼ����ϰŵ�"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_5)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�ٵ�... �� ������ �ִ� ���Ⱑ �ϳ���?\n���� �ֺ��� �ʹ� ������ ���� ��Ḧ �ϳ� �ٲ�"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_6)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�ֹ��� ���ؼ� ���⸦ �ϳ� �̾ƺ�"));
			}
			else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_7)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("�׷� �˰ھ�"));
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

				// ī�޶�
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);

				// �̱� ��ȭ �ֱ�
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
					Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("� ������ �������?"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == CUBE_ORDER_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// �������� �� ���� ���·� ��ȣ�ۿ����� �� ����Ʈ �Ϸ�
					CQuest_Manager::Get_Instance()->Complete_Quest();
					CItem_Manager::Get_Instance()->Use_Item(ITEM_LETTUCE, 5);
					// ���� ���·� �ʱ�ȭ
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// ����
					m_iBlackCoreNum = 5;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// ī�޶�
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_COLLECT_LETTUCE:

			if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_1)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�� ����?"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_2)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("Ȥ�� �ð� �־�? �ʹ� �ٺ��� �׷��� �� ������ �� ������?"));
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_3)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("���⸸ �ص� �ٺ� ���̳�?\n���� �� �����ָ� ��?"));
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_4)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�� ������ �ִ� ���߸� 5���� ������"));
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_5)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("�׷� �˰ھ�"));
			}
			else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_6)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("����!"));
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

				// ī�޶�
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
			}

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("��? �Ա���! ���߰�.. �ϳ�.. ��.. ��.. �ټ�! �� �³� ���� ����"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == COLLECT_LETTUCE_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// �������� �� ���� ���·� ��ȣ�ۿ����� �� ����Ʈ �Ϸ�
					CQuest_Manager::Get_Instance()->Complete_Quest();
					CItem_Manager::Get_Instance()->Use_Item(ITEM_LETTUCE, 5);
					// ���� ���·� �ʱ�ȭ
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// ����
					m_iBlackCoreNum = 5;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// ī�޶�
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_KILL_GANG_SWARM:

			if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_1)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("(�ȸ��� �ٻڰ� ���� �ϰ� �ִ�)"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_2)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("�ȸ��� �ñ��ߴ� �ǵ� �� �̷��� �ٻ� �ž�?"));
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_3)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("���� �ִ����� ������..\n��� ���� ���͵��� ���� �غ��ϴ� �� ����\n������ ������ �𸣰ھ�\n�ܼ��� ���� �������� ��������..."));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_4)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("����..? ��� ���� ��û ū �����ݾ�"));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_5)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("���� ���� �迡 Ȥ�� �� �� �� ���� ������ �� ������?\n���� ���� ������ �аŸ����� ó���� ��"));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_GANG_SWARM_SCRIPT_6)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("�˰ھ�\n���� �� ����� �ٸ� ������ �ʿ��ϸ� ����"));
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

				// ī�޶�
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
					Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("������ �аŸ����� �̷� ū ���⸦ ����� �־��ٴ�...\n�ܼ��� ���� ������ �ƴ� �� ����"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == KILL_ROBELLA_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// �������� �� ���� ���·� ��ȣ�ۿ����� �� ����Ʈ �Ϸ�
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// ���� ���·� �ʱ�ȭ
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// ����
					m_iBlackCoreNum = 10;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// ī�޶�
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);

				}
			}
			break;

		case QUEST_MAKE_COOKING:

			if (m_iScriptPhase == MAKE_COOKING_SCRIPT_1)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("��â�� ��ħ �� �Ծ� Ȥ�� ���� ���ƴٴϸ鼭 ��� ä���� �ϰ� �־�?"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT_2)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("�� ���� ���� ���� ��� ���� �־�"));
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT_3)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�׷� ���� �ֺ��� �丮 �κ��ѵ� ���� �丮�� ������!\n�װ� ������ �аŸ��� ó���Ϸ� ���� ��\nȤ�ó� ���� ��ġġ�� ������ ������ �߰ŵ�..."));
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT_4)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�丮�� �̸� ����� ������ �׳� ��Ẹ�� �� ���� �ְ�, ���� ȸ���� �� �� ������ �� ����!"));
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

				// ī�޶�
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
			}

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == MAKE_COOKING_SCRIPT_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("��~ ����...\n�� ���� �Ծ�þ�? ���� ���� ���� ���� ������.. �� ���ڴ�!"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == MAKE_COOKING_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// �������� �� ���� ���·� ��ȣ�ۿ����� �� ����Ʈ �Ϸ�
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// ���� ���·� �ʱ�ȭ
					m_iQuestEvent = 0;
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// ����
					m_iBlackCoreNum = 5;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// ī�޶�
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_ORDER:

			if (m_iScriptPhase == ORDER_SCRIPT_1)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("��â�� ������ �� ����� �� ���� �־�?"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == ORDER_SCRIPT_2)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("�� ������ �������� �����ϰ� ���"));
			}
			else if (m_iScriptPhase == ORDER_SCRIPT_3)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�����̴�! ���� �� �� �ھ �� ������ �ʾҾ�?\n����Ӹ� �ƴ϶� �پ��� ��ᵵ �����ϱ� �ٽ� �� �� �غ�!"));
			}
			else if (m_iScriptPhase == ORDER_SCRIPT_4)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�и� ���� ���忡 �ʿ��� �͵��� ���� �ž�"));
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

				// ī�޶�
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
			}

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == ORDER_SCRIPT_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("� ���ѵ� �ʿ��� �͵��� ���Ծ�?"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == ORDER_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// �������� �� ���� ���·� ��ȣ�ۿ����� �� ����Ʈ �Ϸ�
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// ���� ���·� �ʱ�ȭ
					m_iQuestEvent = 0;
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// ����
					m_iBlackCoreNum = 5;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// ī�޶�
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_KILL_MECHANICALCITY_SWARM:

			if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_1)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("��... ���⵵ �ɻ�ġ �ʳ�"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_2)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("�ȸ��� �� ���� ���̾�?"));
			}
			else if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_3)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�̹��� �� �༮�� ���� �ٸ� ������ �ִ� ��� �аŸ����� ������ �־�\n�ٵ� ���°� �̻��� ��ġ ������ ���� ����..\n��� ���͵��� ������ ��ī�ο�����"));
			}
			else if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_3)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("���� ���¸� ���� �ò�"));
			}
			else if (m_iScriptPhase == KILL_MECHANICALCITY_SWARM_4)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("������ �����ϸ� ������ �ھ�� ���� ���⸦ ����� ��!"));
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

				// ī�޶�
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
					Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("���ν�Ʈ �巡���� ������?\n�װ� ��Ȳ�� ���� ���� �� ���縦 �ߴµ�\nȮ���� �� ���͵��� � �Ϳ� ������ ������ �� ����"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == KILL_FROST_DRAGON_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// �������� �� ���� ���·� ��ȣ�ۿ����� �� ����Ʈ �Ϸ�
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// ���� ���·� �ʱ�ȭ
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// ����
					m_iBlackCoreNum = 10;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// ī�޶�
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_MAKE_COOKING_1:

			if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_1)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("���� ���Ϳ� ���� ���� �ذ�� �� �ƴ�����! �ݰ��굵 ���İ�, �̹����� Ȥ��...?"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_2)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("�¾� ������ ���� ������ ��Ḧ �����Ծ�"));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_3)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("����..���� �̹��� �� ���� �ٸ� �丮�� ������"));
				//pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_4)
			{
				//pGameInstance->Able_LateTickGroup(LTG_UI);
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�� ��������� �� ���Ը�... ����"));
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

				// ī�޶�
				CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
				pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
			}

			if (m_bQuestClear)
			{
				if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_CLEAR)
				{
					Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("�� �� ���ִ� �ž�? ����!\n...��? �̰� �� ���̳İ�?\n�̾� �ʹ� ���־ �׸�.."));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == MAKE_COOKING_SCRIPT1_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// �������� �� ���� ���·� ��ȣ�ۿ����� �� ����Ʈ �Ϸ�
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// ���� ���·� �ʱ�ȭ
					m_iQuestEvent = 0;
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// ����
					m_iBlackCoreNum = 5;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// ī�޶�
					CCamera* pPlayerCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_PlayerCamera();
					pGameInstance->Set_NextCamera(pPlayerCamera, 1.f);
				}
			}
			break;

		case QUEST_KILL_MUTATION:

			if (m_iScriptPhase == KILL_MUTATION_SCRIPT_1)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("��â�� ���͵��� �������� �� ���ܳ����� �˾Ƴ� �� ����"));
				pGameInstance->Disable_LateTickGroup(LTG_UI);
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_2)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("���� ���� ������ ���͵��� ������ ���� ���� �ž�?"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_3)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("������ ���ο� ������ ���밨�� ���ÿ� ������ ���� ���� ������ �� ����\n�ֱٿ� ��Ÿ�� ������ �ܱⰣ�� ��û �������� �ְŵ�"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_4)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("�� �༮���� ��� �ܱⰣ�� ������ �ž�?"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_5)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("������ ������ ������� �̿��ϰ� �־�"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_6)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("���? ����� �̿��ؼ� ��� �������� �ž�?"));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_7)
			{
				Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("������� �������Ѽ� �ڽŵ��� ���¿� ���� �ְŵ�...\n�׷��� ������ �����̶�� �ٸ� ���͵鿡�� �������� �ִ� �͵� ������ �ž� �Ƹ���.."));
			}
			else if (m_iScriptPhase == KILL_MUTATION_SCRIPT_8)
			{
				Create_Talk_Window(PLAYER, TEXT("���ΰ�"), TEXT("�������� �༮���̳�...\n���� �� �༮���� �����̶�� ������ �� ����"));
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

				// ī�޶�
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
					Create_Talk_Window(NPC, TEXT("�ȸ�"), TEXT("���� ������ ������ �ƴ϶�� �׷� �� �������� ������ ����..?"));
					pGameInstance->Disable_LateTickGroup(LTG_UI);
				}
				else if (m_iScriptPhase == KILL_WEAVER_SCRIPT_CLEAR_END)
				{
					pGameInstance->Able_LateTickGroup(LTG_UI);
					++m_iScriptPhase;

					// �������� �� ���� ���·� ��ȣ�ۿ����� �� ����Ʈ �Ϸ�
					CQuest_Manager::Get_Instance()->Complete_Quest();

					// ���� ���·� �ʱ�ȭ
					m_bScript = false;
					m_bQuestClear = false;
					((CPlayer*)pPlayer)->Set_EnableKeyInput(true);

					// ����
					m_iBlackCoreNum = 10;
					CItem_Manager::Get_Instance()->Add_Item(ITEM_BLACKCORE, m_iBlackCoreNum);
					Create_UI_Alert();

					// ī�޶�
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

	// �÷��̾�� �浹���� ��
	for (auto& iter : *m_pCol_Sphere->Get_StayColliders())
	{
		// ���࿡ Non_Script ����Ʈ�� �ϰ� ������ ��ȣ�ۿ� ���ϰ�
		if (m_bNonScript == true)
			return;

		// FŰ�� ��ȣ�ۿ��϶�� UI ����
		Create_UI_GuideWindow_Talk(TEXT("�ȸ�"));

		if (KEY_DOWN('F'))
		{
			MGR(CSound_Manager)->PlaySound(TEXT("Gain_Item.mp3"), CSound_Manager::CHANNEL_UI, 0.45f);
			// ��ũ��Ʈ ����
			m_bScript = true;
			((CPlayer*)pPlayer)->Set_EnableKeyInput(false);

			// ī�޶� ����
			CCamera* pCookCamera = ((CLevel_GamePlay*)pGameInstance->Get_CurLevel())->Get_ShirleyCamera();
			pGameInstance->Set_NextCamera(pCookCamera, 1.f);
		}
		if (KEY_UP('F'))
		{
			// FŰ�� ��ȣ�ۿ��϶�� UI, FŰ�� ������ ����
			Delete_UI_GuideWindow_Talk();
		}
	}
	for (auto& iter : *m_pCol_Sphere->Get_ExitColliders())
	{
		// FŰ�� ��ȣ�ۿ��϶�� UI, �浹�� ������ �� �� UI ����
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
	swprintf_s(szTemp, TEXT("�� �ھ�x%d ȹ��"), m_iBlackCoreNum);

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
