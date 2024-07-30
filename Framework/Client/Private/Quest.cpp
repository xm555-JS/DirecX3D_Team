#include "..\Public\Quest.h"

#include "GameInstance.h"
#include "UI_Quest_Notice.h"
#include "Imgui_Manager.h"

CQuest::CQuest()
	: CGameObject()
{
}

CQuest::CQuest(const CQuest& rhs)
	: CGameObject(rhs)
{
}

HRESULT CQuest::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQuest::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));

	//원래 해줄거
	memcpy(&m_eQuest_Type, pArg, sizeof(QUEST_TYPE));

	////위치 테스트할때
	//m_eQuest_Type = QUEST_KILL_WEAVER;
	Create_UI_Quest();
	Create_UI_Quest_Notice(m_pUI_Quest->Get_vUIPos(), { 20.f,27.f }, 0);
	Create_UI_Quest_Notice({ 1175.f,105.f }, { 150.f,70.f }, 2);


	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_UI, this);
	pGameInstance->Add_LateTickGroup(LTG_UI, this);

	return S_OK;
}

void CQuest::Tick(_float fTimeDelta)
{
	if (m_bAchieve)
	{
		IMPL_ONCE(m_bAchieveOnce,
			Create_UI_Quest_Notice(m_pUI_Quest->Get_vUIPos(), { 20.f,27.f }, 1);
			m_pUI_Notice->Set_iTextureIdx(3);
			m_pUI_Notice->Set_AccTime(0.f);
			MGR(CSound_Manager)->PlaySound(TEXT("UI_Quest_Achieve.mp3"), CSound_Manager::CHANNEL_UI, 0.3f);
		);

		m_pUI_Quest->Set_iTextureIdx(0);
	}
	if (m_bAchieve && m_bComplete)
	{
		GAMEINSTANCE->Add_DeadObject(m_pUI_Quest);
		GAMEINSTANCE->Add_DeadObject(m_pUI_Notice);
		GAMEINSTANCE->Add_DeadObject(this);
	}
}

void CQuest::LateTick(_float fTimeDelta)
{
}

HRESULT CQuest::Render()
{
	return S_OK;
}

void CQuest::Create_UI_Quest_Notice(_float2 vPos, _float2 vSize, _uint iTextureIdx)
{
	if (2 <= iTextureIdx)
	{
		m_pUI_Notice = (CUI_Quest_Notice*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_NOTICE), ID_UINT(UI, UI_QUEST_NOTICE),
			&CUI::UIDESC(vPos.x, vPos.y, vSize.x, vSize.y, 0.f, iTextureIdx, 690));
	}
	else
	{
		GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_NOTICE), ID_UINT(UI, UI_QUEST_NOTICE),
			&CUI::UIDESC(vPos.x, vPos.y, vSize.x, vSize.y, 0.f, iTextureIdx, 690));
	}
}

void CQuest::Create_UI_Quest()
{
	switch (m_eQuest_Type)
	{

	case QUEST_CUBE_ORDER:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1070.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1090.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("1회 주문하기"));
		break;

	case QUEST_COLLECT_LETTUCE:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1070.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1090.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("상추 5개 수집하기"));
		break;
	case QUEST_KILL_GANG_SWARM:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1050.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1070.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("갱 패거리 몬스터 처치"));
		break;
	case QUEST_KILL_GANG_CAP:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1070.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1090.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("갱스터 캡 처치"));
		break;
	case QUEST_KILL_ROBELLA:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1070.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1090.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("로벨라 처치"));
		break;
	case QUEST_MAKE_COOKING:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1010.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1030.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("재료들을 모아서 요리하기"));
		break;


	case QUEST_ORDER:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(990.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1010.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("주문을 이용하여 아이템 뽑기"));
		break;


	case QUEST_KILL_MECHANICALCITY_SWARM:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1010.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1030.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("메카니컬시티 몬스터 처치"));
		break;
	case QUEST_KILL_DOORKEEPER:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1030.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1050.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("도어키퍼 몬스터 처치"));
		break;
	case QUEST_KILL_JUMPING_FROSTDRAGON:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1010.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1030.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("점핑 프로스트 드래곤 처치"));
		break;
	case QUEST_MAKE_COOKING_1:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1010.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1030.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("재료들을 모아서 요리하기"));
		break;

	case QUEST_KILL_MUTATION:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1070.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1090.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("돌연변이 처치"));
		break;
	case QUEST_KILL_HABAKA:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1070.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1090.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("하바카 처치"));
		break;
	case QUEST_KILL_WEAVER:
		m_pUI_Quest = (CUI*)GAMEINSTANCE->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_QUEST_MARK), ID_UINT(UI, UI_QUEST_MARK),
			&CUI::UIDESC(1050.f, 150.f, 37.f, 37.f, 0.f, 1, 700, { 0.f,0.f }, nullptr, 0.f, { 1070.f,132.f }, { 1.f, 1.f, 1.f, 1.f }, 0.35f));
		m_pUI_Quest->Set_Delfault_FontText(TEXT("공포의 원인 처치"));
		break;
	default:
		break;
	}
}

HRESULT CQuest::SetUp_Components()
{
	return S_OK;
}

HRESULT CQuest::SetUp_ShaderResource()
{
	return S_OK;
}

CQuest* CQuest::Create()
{
	CQuest* pInstance = new CQuest();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CQuest::Clone(void* pArg)
{
	CQuest* pInstance = new CQuest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuest::Free()
{
	__super::Free();
}
