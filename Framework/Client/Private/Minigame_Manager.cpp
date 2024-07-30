#include "stdafx.h"
#include "Minigame_Manager.h"
#include "GameInstance.h"
#include "UI.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CMinigame_Manager)

CMinigame_Manager::CMinigame_Manager()
{
}

HRESULT CMinigame_Manager::Initialize()
{
	return S_OK;
}

void CMinigame_Manager::Notice_Minigame()
{
	Load_UIData("Minigame");
	m_iGainNum = 0;
}

void CMinigame_Manager::Start_Minigame()
{
}

void CMinigame_Manager::Finish_Minigame(_uint iCurLevel)
{
	m_bFinishedGame[m_iCur_Minigame] = true;

	++m_iCur_Minigame;
	// 완료UI생성
	USEGAMEINSTANCE;
	list<CGameObject*>* pGameObjectlist = pGameInstance->Get_Objects(iCurLevel, ID_UINT(UI, UI_MINIGAME_GAIN));
	for (auto& pMinigame : *pGameObjectlist)
	{
		pGameInstance->Add_PoolingObject(pMinigame);
	}
	Load_UIData("Minigame_Result");

	FIXCURSOR(false);
	CPlayer* pPlayer = (CPlayer*)PLAYERPTR;
	pPlayer->Set_EnableKeyInput(false);
}

void CMinigame_Manager::Load_UIData(string strFileName)
{
	USEGAMEINSTANCE;
	json Data;

	wstring strFolder = TEXT("UI/");
	wstring strTemp = towstring(strFileName);
	wstring strFullName = strFolder + strTemp;

	const wchar_t* szName = strFullName.c_str();
	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(szName).c_str(), &Data);

	_uint iPrototypeID;

	for (auto& elem : Data)
	{
		CUI::UIDesc UIDesc;
		string strTemp = "";
		iPrototypeID = elem["iPrototypeID"];
		UIDesc.fInitX = elem["fInitX"];
		UIDesc.fInitY = elem["fInitY"];
		UIDesc.fInitSizeX = elem["fInitSizeX"];
		UIDesc.fInitSizeY = elem["fInitSizeY"];
		UIDesc.fAngle = elem["fAngle"];
		UIDesc.iTextureIdx = elem["iTextureIdx"];
		UIDesc.iRenderLayer = elem["iRenderLayer"];
		strTemp = elem["strText"];
		UIDesc.vFontPos.x = elem["vFontPosx"];
		UIDesc.vFontPos.y = elem["vFontPosy"];
		UIDesc.vFontColor.x = elem["vFontColorx"];
		UIDesc.vFontColor.y = elem["vFontColory"];
		UIDesc.vFontColor.z = elem["vFontColorz"];
		UIDesc.vFontColor.w = elem["vFontColorw"];
		UIDesc.fFontSize = elem["fFontSize"];
		UIDesc.bRenderFontText = elem["bRenderFontText"];
		UIDesc.bRenderFontNum = elem["bRenderFontNum"];

		ID_UINT IDPrototype;
		IDPrototype.iID = iPrototypeID;

		CGameObject* pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, IDPrototype, IDPrototype, &UIDesc);
		CUI* pUI = (CUI*)pGameObject;
		pUI->Set_FontText(strTemp);
	}
}

void CMinigame_Manager::Free()
{
	__super::Free();
}
