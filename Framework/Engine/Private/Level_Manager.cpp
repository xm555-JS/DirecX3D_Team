#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}



void CLevel_Manager::On_EventMessege(EVENTDESC tEvent)
{
 	if (LEVEL_CHANGE == tEvent.eEventType)
	{
		/* 이전레벨에서 사용한 자원들을 모두 소거한다. */
		if (nullptr != m_pCurrentLevel)
		{
			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			if (FAILED(pGameInstance->Clear(m_iCurrentLevelID)))
			{
				MSG_BOX("Failed to clear Level");
			}
				
			Safe_Release(pGameInstance);
		}

		Safe_Release(m_pCurrentLevel);

		m_pCurrentLevel = (CLevel*)tEvent.wParam;

		m_iCurrentLevelID = (_uint)tEvent.lParam;
	}
}

HRESULT CLevel_Manager::Initialize()
{
	USEGAMEINSTANCE;

	pGameInstance->Add_CallbackList(this, LEVEL_CHANGE);

	return S_OK;
}

void CLevel_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(fTimeDelta);
}

void CLevel_Manager::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->LateTick(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		__debugbreak();

	return m_pCurrentLevel->Render();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
