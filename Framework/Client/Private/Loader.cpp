#include "stdafx.h"
#include "..\Public\Loader.h"

#include "Loader_Logo.h"
//#include "Loader_Menu.h"
#include "Loader_Edit_Anim.h"
#include "Loader_GamePlay.h"
#include "Loader_Edit_UI.h"

/* UI */
#include "UI_Loading_Screen.h"

CLoader::CLoader()
{
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	if (FAILED(pLoader->Loading()))
		MSG_BOX("Failed to Loading");

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;


	switch (m_eNextLevel)
	{
	case LEVEL_LOGO:
		m_pCurLoader = CLoader_Logo::Create();
		break;
	//case LEVEL_MENU:
	//	m_pCurLoader = CLoader_Menu::Create();
	//	break;
	case LEVEL_EDIT_ANIM:
		m_pCurLoader = CLoader_Edit_Anim::Create();
		break;
	case LEVEL_GAMEPLAY:
		m_pCurLoader = CLoader_GamePlay::Create();
		break;
	case LEVEL_EDIT_UI:
		m_pCurLoader = CLoader_Edit_UI::Create();
		break;
	}

	m_pCurLoader->Create_CriticalSection();
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, m_pCurLoader, 0, nullptr);
	if (0 == m_hThread)
		__debugbreak();

	return S_OK;
}


HRESULT CLoader::Loading()
{
	lstrcpy(m_szLoadingText, TEXT("Loading for Object Prototype"));
	if (FAILED(Loading_Object()))
		__debugbreak();

	lstrcpy(m_szLoadingText, TEXT("Loading for Texture Prototype"));
	if (FAILED(Loading_Texture()))
		__debugbreak();

	lstrcpy(m_szLoadingText, TEXT("Loading for Model Prototype"));
	if (FAILED(Loading_Model()))
		__debugbreak();

	lstrcpy(m_szLoadingText, TEXT("Loading for Shader Prototype"));
	if (FAILED(Loading_Shader()))
		__debugbreak();

	lstrcpy(m_szLoadingText, TEXT("Finish Loading"));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Create_CriticalSection()
{
	InitializeCriticalSection(&m_CriticalSection);
	return S_OK;
}


CLoader * CLoader::Create(LEVEL eNextLevel)
{
	CLoader*		pInstance = new CLoader();

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}
	//__debugbreak();


	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_pCurLoader->m_CriticalSection);
	CloseHandle(m_hThread);

	Safe_Release(m_pCurLoader);
}
