#include "stdafx.h"
#include "..\Public\Window_LevelInfo.h"

#if defined(USE_IMGUI)


#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Imgui_Manager.h"

//#include "Window_ObjectDesc.h"
//#include "Window_ModeInfo.h"
#include "GameInstance.h"
//#include "SaveModel.h"

CWindow_LevelInfo::CWindow_LevelInfo()
{
}


CWindow_LevelInfo::~CWindow_LevelInfo()
{
}

void CWindow_LevelInfo::Add_Object(SAVEDESC* ObjectDesc)
{
	_int	iObjectNum;

	auto	iter = find_if(m_ObjectsNum.begin(), m_ObjectsNum.end(), [&](pair<_uint, _uint> tSour) {
		return tSour.first == ObjectDesc->iObjectID_fir;
	});

	if (iter == m_ObjectsNum.end())
	{
		m_ObjectsNum.emplace(ObjectDesc->iObjectID_fir, 0);
		iObjectNum = 0;
	}
	else
	{
		iter->second += 1;
		iObjectNum = iter->second;
	}

	ObjectDesc->iObjectID_num = iObjectNum;
	m_Objects.push_back(*ObjectDesc);
}

void CWindow_LevelInfo::Delete_Object(CGameObject * pObject)
{
	USEGAMEINSTANCE;
	pGameInstance->Add_DeadObject(pObject);

	auto iter = m_Objects.begin();
	for (; iter != m_Objects.end();)
	{
		if (pObject == (*iter).pObject)
		{
			iter = m_Objects.erase(iter);
			break;
		}
		else
		{
			++iter;
		}
	}
}



void CWindow_LevelInfo::Initialize()
{
	//m_pWindow_ObjectDesc = (CWindow_ObjectDesc*)CImgui_Manager::Get_Instance()->Get_Window(CImgui_Manager::WINDOWS::OBJECTDESC);

	CWindow_Imgui::Initialize_Children();
}


void CWindow_LevelInfo::Tick(_float fTimeDelta)
{
	ImGui::Begin("Level Info");                          // Create a window called "Hello, world!" and append into it.

	InputBox_FindLevel();
	Button_LoadLevel(); ImGui::SameLine();
	Button_SaveLevel(); ImGui::SameLine();
	Button_RefreshLevel();
	//ListBox_LevelList();
	ListBox_ObjectList();



	ImGui::End();

	CWindow_Imgui::Tick_Children(fTimeDelta);
}



void CWindow_LevelInfo::InputBox_FindLevel()
{
	ImGui::InputText("Level name", m_szLvNameBuffer, IM_ARRAYSIZE(m_szLvNameBuffer));
}


void CWindow_LevelInfo::ListBox_ObjectList()
{
	ImGui::Text("Object List");
	if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (size_t i = 0; i < m_Objects.size(); ++i)
		{
			char	szID_fir[MAX_PATH] = "";
			char	szID_num[MAX_PATH] = "";
			_itoa_s(m_Objects[i].iObjectID_fir, szID_fir, 10);
			_itoa_s(m_Objects[i].iObjectID_num, szID_num, 10);
			char	szObjectName[MAX_PATH] = "";
			strcat_s(szObjectName, szID_fir);
			strcat_s(szObjectName, "_");
			strcat_s(szObjectName, szID_num);


			const bool is_selected = (m_Object_CurIndex == i);
			if (ImGui::Selectable(szObjectName, is_selected))
				m_Object_CurIndex = i;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
				// 선택된 Index의 무언가를 해주면 된다.
				// 여기는 선택된 GameObject의 정보 표시와 수정, 삭제 기능 등을 표시 예정
			}
		}
		ImGui::EndListBox();
	}
}


void CWindow_LevelInfo::Button_SaveLevel()
{

	if (ImGui::Button("Save"))
	{
		string	strLevelName = m_szLvNameBuffer;
		wstring widestr = wstring(strLevelName.begin(), strLevelName.end());
		wstring	wstrMapFile = widestr + wstring(L"_MapInfo");
		wstring	wstrDoorFile = widestr + wstring(L"_DoorInfo");
		wstring	wstrOptionFile = widestr + wstring(L"_OptionInfo");


		vector<SAVEDESC>				MapObjects;
		vector<SAVEDESC>				DoorObjects;
		vector<SAVEDESC>				OptionObjects;

		for (auto& iter : m_Objects)
		{
			if (0 != iter.iOption)
			{
				if (COLLIDERTYPE::ZONE == iter.eType &&
					10 <= iter.iOption &&
					20 > iter.iOption)
				{
					DoorObjects.push_back(iter);
				}
				else
				{
					OptionObjects.push_back(iter);
				}
			}
			else
			{
				MapObjects.push_back(iter);
			}
		}

		HRESULT hr = S_OK;

		if (FAILED(Save_ObjectInfo(widestr, m_Objects)))
			hr = E_FAIL;

		if(FAILED(Save_ObjectInfo(wstrMapFile, MapObjects)))
			hr = E_FAIL;

		if (FAILED(Save_ObjectInfo(wstrDoorFile, DoorObjects)))
			hr = E_FAIL;

		if (FAILED(Save_ObjectInfo(wstrOptionFile, OptionObjects)))
			hr = E_FAIL;

		if (E_FAIL == hr)
			MSG_BOX("Failed to Save");
		else
			MSG_BOX("Save complete");

	}
}

void CWindow_LevelInfo::Button_LoadLevel()
{
	//if (ImGui::Button("Load"))
	//{
	//	USEGAMEINSTANCE;
	//	HANDLE	hFile;

	//	string	strLevelName = m_szLvNameBuffer;
	//	wstring widestr = wstring(strLevelName.begin(), strLevelName.end());

	//	if (!FAILED(pGameInstance->Open_Load_Handle(&hFile, DATAFILEPATHARG(widestr.c_str()))))
	//	{
	//		for (auto& SaveDesc : m_Objects)
	//		{
	//			pGameInstance->Add_DeadObject((CGameObject*)SaveDesc.pObject);
	//		}
	//		m_Objects.clear();
	//		m_ObjectsNum.clear();
	//		((CWindow_ObjectDesc*)m_pWindow_ObjectDesc)->Set_SelectObject(nullptr);

	//		DWORD			dwByte = 0;
	//		SAVEDESC		SaveDesc;
	//		while (true)
	//		{
	//			ReadFile(hFile, &SaveDesc, sizeof(SAVEDESC), &dwByte, nullptr);
	//			if (0 == dwByte)
	//				break;

	//			Add_Object(&SaveDesc);
	//			//m_Objects.push_back(SaveDesc);
	//		}
	//		CImgui_Manager::Get_Instance()->Set_MultiMode(false);
	//		MSG_BOX("Load complete");
	//	}
	//	CloseHandle(hFile);

	//	CGameObject*	pObject = nullptr;
	//	for (auto& elem : m_Objects)
	//	{
	//		pObject = (pGameInstance->Add_GameObject(LEVEL_EDIT, ID_UINT(SAVEMODEL), ID_UINT(SAVEMODEL), &elem));

	//		//CColliderModel* pCube = (CColliderModel*)pObject;
	//		CSaveModel* pCube = (CSaveModel*)pObject;
	//		//pCube->Create_SaveData(elem);
	//		elem.pObject = pCube;
	//	}
	//}
}

void CWindow_LevelInfo::Button_RefreshLevel()
{
	//if (ImGui::Button("Refresh"))
	//{
	//	USEGAMEINSTANCE;
	//	HANDLE	hFile;
	//	string	strLevelName = m_szLvNameBuffer;
	//	wstring widestr = wstring(strLevelName.begin(), strLevelName.end());

	//	DWORD		dwByte = 0;
	//	SAVEDESC	SaveDesc;

	//	if (!FAILED(pGameInstance->Open_Save_Handle(&hFile, DATAFILEPATHARG(widestr.c_str()))))
	//	{
	//		for (auto& iter : m_Objects)
	//		{
	//			//SaveDesc = *((CColliderModel*)iter.pObject)->Get_SaveDesc();
	//			SaveDesc = *((CSaveModel*)iter.pObject)->Get_SaveDesc();
	//			WriteFile(hFile, &(SaveDesc), sizeof(SAVEDESC), &dwByte, nullptr);
	//		}
	//	}
	//	CloseHandle(hFile);


	//	if (!FAILED(pGameInstance->Open_Load_Handle(&hFile, DATAFILEPATHARG(widestr.c_str()))))
	//	{
	//		for (auto& SaveDesc : m_Objects)
	//		{
	//			pGameInstance->Add_DeadObject((CGameObject*)SaveDesc.pObject);
	//		}
	//		m_Objects.clear();
	//		m_ObjectsNum.clear();
	//		((CWindow_ObjectDesc*)m_pWindow_ObjectDesc)->Set_SelectObject(nullptr);

	//		while (true)
	//		{
	//			ReadFile(hFile, &SaveDesc, sizeof(SAVEDESC), &dwByte, nullptr);
	//			if (0 == dwByte)
	//				break;

	//			Add_Object(&SaveDesc);
	//		}
	//	}
	//	CloseHandle(hFile);

	//	CGameObject*	pObject = nullptr;
	//	for (auto& elem : m_Objects)
	//	{
	//		pObject = (pGameInstance->Add_GameObject(LEVEL_EDIT, ID_UINT(SAVEMODEL), ID_UINT(SAVEMODEL), &elem));

	//		//CColliderModel* pCube = (CColliderModel*)pObject;
	//		CSaveModel* pCube = (CSaveModel*)pObject;
	//		//pCube->Create_SaveData(elem);
	//		elem.pObject = pCube;
	//	}
	//	CImgui_Manager::Get_Instance()->Set_MultiMode(false);
	//}
}


HRESULT CWindow_LevelInfo::Save_ObjectInfo(wstring wstrFileName, OBJECTS Objects)
{
	//USEGAMEINSTANCE;
	//HANDLE	hFile;
	//if (!FAILED(pGameInstance->Open_Save_Handle(&hFile, DATAFILEPATHARG(wstrFileName.c_str()))))
	//{
	//	DWORD		dwByte = 0;
	//	SAVEDESC	SaveDesc;
	//	for (auto& iter : Objects)
	//	{
	//		SaveDesc = *((CSaveModel*)iter.pObject)->Get_SaveDesc();
	//		WriteFile(hFile, &(SaveDesc), sizeof(SAVEDESC), &dwByte, nullptr);
	//	}
	//	CImgui_Manager::Get_Instance()->Set_MultiMode(false);
	//	//MSG_BOX("Save complete");
	//}
	//else
	//{
	//	CloseHandle(hFile);
	//	__debugbreak();
	//}
	//CloseHandle(hFile);
	return S_OK;
}

void CWindow_LevelInfo::Free()
{

}


//	m_LevelName =
//	{
//		"Induction",
//		"Optical",
//		"Cubism",
//		"Blackout",
//		"Clone",
//		"Dollhouse",
//		"Labyrinth",
//		"Whitespace",
//		"Retrospect"
//	};

#endif