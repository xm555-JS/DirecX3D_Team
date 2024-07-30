#include "stdafx.h"
#include "Quest_Manager.h"
#include "GameInstance.h"
#include "Quest.h"
#include "Sound_Manager.h"

IMPLEMENT_SINGLETON(CQuest_Manager)

CQuest_Manager::CQuest_Manager()
{
}

HRESULT CQuest_Manager::Initialize()
{
	// ����Ʈ ������ ������� pushback
	m_Questlist.push_back(QUEST_CUBE_ORDER);

	m_Questlist.push_back(QUEST_COLLECT_LETTUCE);
	m_Questlist.push_back(QUEST_KILL_GANG_SWARM);
	m_Questlist.push_back(QUEST_KILL_GANG_CAP);
	m_Questlist.push_back(QUEST_KILL_ROBELLA);
	m_Questlist.push_back(QUEST_MAKE_COOKING);

	m_Questlist.push_back(QUEST_ORDER);

	m_Questlist.push_back(QUEST_KILL_MECHANICALCITY_SWARM);
	m_Questlist.push_back(QUEST_KILL_DOORKEEPER);
	m_Questlist.push_back(QUEST_KILL_JUMPING_FROSTDRAGON);
	m_Questlist.push_back(QUEST_MAKE_COOKING_1);

	m_Questlist.push_back(QUEST_KILL_MUTATION);
	m_Questlist.push_back(QUEST_KILL_HABAKA);
	m_Questlist.push_back(QUEST_KILL_WEAVER);

	return S_OK;
}

void CQuest_Manager::Receive_Quest(_uint iCurLevel)
{
	// ����Ʈ�� ���� ����
	if (!m_Questlist.size())
		return;

	m_pCurQuest = (CQuest*)GAMEINSTANCE->Add_GameObject(iCurLevel, ID_UINT(QUEST), ID_UINT(QUEST), &(*m_Questlist.begin()));

	m_bIsQuest = true;
}

void CQuest_Manager::Achieve_Quest()
{
	// ����Ʈ�� �Ϸ� ������ ����
	if (m_pCurQuest)
		m_pCurQuest->Set_Achieve_Quest();
}

void CQuest_Manager::Complete_Quest()
{
	// ����Ʈ �Ϸ� ����
	if (m_pCurQuest)
		m_pCurQuest->Set_Complete_Quest();

	m_Questlist.pop_front();
	m_bIsQuest = false;
}

void CQuest_Manager::Free()
{
	__super::Free();
}
