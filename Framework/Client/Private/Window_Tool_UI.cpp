#include "stdafx.h"
#include "Window_Tool_UI.h"

#if defined(USE_IMGUI)

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Imgui_Manager.h"

#include "GameObject.h"
#include "UI_Loading_Screen.h"

#include "GameInstance.h"


CWindow_Tool_UI::CWindow_Tool_UI()
{
}


CWindow_Tool_UI::~CWindow_Tool_UI()
{
}

void CWindow_Tool_UI::Initialize()
{
	USEGAMEINSTANCE;

	CWindow_Imgui::Initialize_Children();
}

void CWindow_Tool_UI::Tick(_float fTimeDelta)
{
	ImGui::Begin("Tool UI");                        
	
	Show_UIPrototypes();

	ImGui::End();

	if (show_UIObject_window)
	{
		ImGui::Begin("UI Objects");

		Show_UIObjects();

		ImGui::End();
	}

	CWindow_Imgui::Tick_Children(fTimeDelta);
}

void CWindow_Tool_UI::Show_UIPrototypes()
{
	USEGAMEINSTANCE;

	_uint  iPrototypeID;
	static _int iPrototypeIdx = 0;
	pGameInstance->Get_Prototypes(ID_UINT(UI), &m_pUIPrototypes);

	// Prototype목록을 띄움
	ImGui::Text("UI Prototype List");
	if (ImGui::BeginListBox("##UI Prototype", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing())))
	{
		int n = 0;
		for (auto& elem : m_pUIPrototypes)
		{
			const bool is_selected = (iPrototypeIdx == n);
			if (ImGui::Selectable(SetUp_UIName(elem.first), is_selected))
				iPrototypeIdx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				iPrototypeID = elem.first;
				ImGui::SetItemDefaultFocus();
			}
			++n;
		}
		ImGui::EndListBox();
	}

	// Prototype List에서 선택한 객체 생성
	if (ImGui::Button("Add UI"))
	{
		// int값 바로 넣는걸로 수정해야함
		CGameObject* pGameObject = pGameInstance->Add_GameObject(LEVEL_EDIT_UI, iPrototypeID, iPrototypeID);
		NULLCHECK(pGameObject);

		m_UIObjectlist.push_back(pGameObject);
	}

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::Checkbox("UI Objects", &show_UIObject_window);
	ImGui::Separator();

	static char szFileName[MAX_PATH] = "";
	ImGui::Text("Input FileName");
	ImGui::InputText("## FileName", szFileName, IM_ARRAYSIZE(szFileName));
	


	string strFileName(szFileName);

	// 저장 및 불러오기
	if (ImGui::Button("Save Data"))
	{
		Save_UIFile(strFileName);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Data"))
	{
		Load_UIFile(strFileName);
	}

}

static _int  iTextureIdx[MAX_PATH];
static _bool bRenderFontText[MAX_PATH];
static _bool bRenderFontNum[MAX_PATH];

void CWindow_Tool_UI::Show_UIObjects()
{
	// 생성한 UI 객체 목록
	ImGui::Text("UI Clone List");
	USEGAMEINSTANCE;
	_int iObjectIdx = 0;
	static _bool bDrag[MAX_PATH];
	static _bool bFixSizeRatio[MAX_PATH];
	static _bool bEditFont[MAX_PATH];
	static _bool bMoveWithFont[MAX_PATH];


	list<CUI*> UIDraglist;
	UIDraglist.clear();

	for (auto& iter = m_UIObjectlist.begin(); iter != m_UIObjectlist.end();)
	{
		bool bIsErase = false;

		string strName(SetUp_UIName((*iter)->Get_PrototypeID()));
		string strSour = to_string(iObjectIdx);
		string strFullName = strName + strSour;

		if (ImGui::TreeNode(strFullName.c_str()))
		{
			bDrag[iObjectIdx];
			bFixSizeRatio[iObjectIdx];

			CGameObject* pGameObject = nullptr;

			_float fUIPos[2] = { 0.f, 0.f };
			_float fUISize[2] = { 0.f, 0.f };
			_float fOriginSize[2] = { 0.f, 0.f };
			_float fUIAngle = 0.f;
			_int   iRenderLayer = 0;

			_float fSizeRatio = 0.f;
						
			pGameObject = *iter;

			ImGui::SameLine();

			// 해당하는객체 삭제
			if (ImGui::Button("Delete UI"))
			{
				pGameInstance->Add_DeadObject(*iter);
				iter = m_UIObjectlist.erase(iter);

				for (size_t i = iObjectIdx; i < MAX_PATH-1; ++i)
				{
					iTextureIdx[i] = iTextureIdx[i + 1];
					bDrag[i] = bDrag[i + 1];
					bFixSizeRatio[i] = bFixSizeRatio[i + 1];
				}
				
				//iObjectIdx


				bIsErase = true;
			}

			// 마우스로 드래그해서 옮길 객체 선택 & 비율고정 후 크기조정
			ImGui::Checkbox("Drag", &bDrag[iObjectIdx]);
			ImGui::SameLine();
			ImGui::Checkbox("Fix Ratio Size", &bFixSizeRatio[iObjectIdx]);

			CUI* pUI = (CUI*)pGameObject;
			if (true == bDrag[iObjectIdx])
			{
				UIDraglist.push_back(pUI);
			}
			
			// Font 사용할 것인지 & 객체 이동할때 폰트 같이 이동할 것인지!
			ImGui::Checkbox("Edit Font", &bEditFont[iObjectIdx]);
			ImGui::SameLine();
			ImGui::Checkbox("Move With Font", &bMoveWithFont[iObjectIdx]);

			

			pUI->Set_bMoveWithFont(bMoveWithFont[iObjectIdx]);

			fUIPos[0] = pUI->Get_vUIPos().x;
			fUIPos[1] = pUI->Get_vUIPos().y;

			fOriginSize[0] = fUISize[0] = pUI->Get_vUISize().x;
			fOriginSize[1] = fUISize[1] = pUI->Get_vUISize().y;

			fUIAngle = pUI->Get_UIDesc().fAngle;
			iRenderLayer = pUI->Get_RenderLayer();
			

			fSizeRatio = fUISize[1] / fUISize[0];

			ImGui::DragFloat2("Position", fUIPos, 0.1f, 0.f, 1280.f);
			ImGui::DragFloat2("Size", fUISize, 0.1f, 0.1f, 3000.f);
			ImGui::DragFloat("Rotation", &fUIAngle, 0.1f, -360.f, 360.f);

			ImGui::DragInt("RenderLayer",&iRenderLayer, 1.0f, 0, 1000);

			if (true == bFixSizeRatio[iObjectIdx])
			{
				_float2 vDelta = { 0.f, 0.f };
				vDelta.y = (fOriginSize[0] - fUISize[0]) * fSizeRatio;
				vDelta.x = (fOriginSize[1] - fUISize[1]) / fSizeRatio;
				pUI->Set_vUISize(_float2(fUISize[0] - vDelta.x, fUISize[1] - vDelta.y));
			}
			else
				pUI->Set_vUISize(_float2(fUISize[0], fUISize[1]));

			pUI->Set_vUIPos(_float2(fUIPos[0], fUIPos[1]));
			pUI->Set_fUIAngle(fUIAngle);
			pUI->Set_RenderLayer(iRenderLayer);


			ImGui::Text("TextureIdx");
			if (ImGui::BeginListBox("##UI TextureIdx", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
			{
				for (_uint n = 0; n < pUI->Get_iMaxTextureIdx() + 1; n++)
				{
					const bool is_selected = (iTextureIdx[iObjectIdx] == n);
					if (ImGui::Selectable(to_string(n).c_str(), is_selected))
						iTextureIdx[iObjectIdx] = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
					{
						pUI->Set_iTextureIdx(iTextureIdx[iObjectIdx]);
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}

			char   strFontText[MAX_PATH] = "";
			_float fFontPos[2] = { 0.f,0.f };
			_float fFontSize = 0.f;
			ImVec4 vColor = ImVec4(1.f, 1.f, 1.f, 1.f);
			

			// Font 편집창
			if (true == bEditFont[iObjectIdx])
			{
				string strDest = pUI->Get_FontText();
				strcpy_s(strFontText, strDest.c_str());
				
				fFontPos[0] = pUI->Get_UIDesc().vFontPos.x;
				fFontPos[1] = pUI->Get_UIDesc().vFontPos.y;

				fFontSize = pUI->Get_UIDesc().fFontSize;

				_float4 vTempColor = pUI->Get_UIDesc().vFontColor;
				vColor = ImVec4(vTempColor.x, vTempColor.y,vTempColor.z, vTempColor.w);

				ImGui::Text("Edit Font");
				ImGui::Checkbox("Render Text", &bRenderFontText[iObjectIdx]);
				ImGui::SameLine();
				ImGui::Checkbox("Render Num", &bRenderFontNum[iObjectIdx]);
				ImGui::InputText("## FontText", strFontText, IM_ARRAYSIZE(strFontText));
				ImGui::DragFloat2("FontPosition", fFontPos, 0.1f, 0.f, 1280.f);
				ImGui::DragFloat("FontScale", &fFontSize, 0.01f, 0.f, 1.f);
				ImGui::ColorEdit4("FontColor", (float*)&vColor, ImGuiColorEditFlags_Float);


				strDest = string(strFontText);

				pUI->Set_FontText(strDest);
				pUI->Set_FontPos({ fFontPos[0],fFontPos[1] });
				pUI->Set_FontSize(fFontSize);
				pUI->Set_FontColor(_float4(vColor.x, vColor.y, vColor.z, vColor.w));
				pUI->Set_bRenderFontText(bRenderFontText[iObjectIdx]);
				pUI->Set_bRenderFontNum(bRenderFontNum[iObjectIdx]);
			}

			ImGui::NewLine();
			ImGui::TreePop();
		}
		++iObjectIdx;

		if (!bIsErase)
			++iter;
	}
	Drag_UI(UIDraglist);
}

void CWindow_Tool_UI::Drag_UI(list<CUI*> UIDraglist)
{
	RECT	rcSelect;
	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_float3 vMousePos = Get_MousePos(g_hWnd);
	_uint	iIdx = 0;


	for (auto& elem : UIDraglist)
	{
		_float2	vUIPos = elem->Get_vUIPos();
		_float2 vUISize = elem->Get_vUISize();

		USEGAMEINSTANCE;
		if (KEY_DOWN(VK_LBUTTON))
		{
			m_vDragDelta[iIdx].x = vUIPos.x - vMousePos.x;
			m_vDragDelta[iIdx].y = vUIPos.y - vMousePos.y;
		}
		if (KEY_PRESSING(VK_LBUTTON))
		{
			SetRect(&rcSelect, vUIPos.x - vUISize.x * 0.5f, vUIPos.y - vUISize.y * 0.5f
				, vUIPos.x + vUISize.x * 0.5f, vUIPos.y + vUISize.y * 0.5f);

			if (PtInRect(&rcSelect, ptMouse))
			{
				elem->Set_vUIPos(_float2(vMousePos.x + m_vDragDelta[iIdx].x, vMousePos.y + m_vDragDelta[iIdx].y));
			}
		}
		if (KEY_UP(VK_LBUTTON))
		{
			m_vDragDelta[iIdx] = { 0.f ,0.f };
		}
		++iIdx;
	}

}

HRESULT CWindow_Tool_UI::Save_UIFile(string strName) // 저장할때 Easing_Speed,-
{
	json Save;

	for (auto& elem : m_UIObjectlist)
	{
		CUI* pUI = (CUI*)elem;

		json data;

		data.emplace("iPrototypeID", elem->Get_PrototypeID());
		data.emplace("fInitX", pUI->Get_vUIPos().x);
		data.emplace("fInitY", pUI->Get_vUIPos().y);
		data.emplace("fInitSizeX", pUI->Get_vUISize().x);
		data.emplace("fInitSizeY", pUI->Get_vUISize().y);
		data.emplace("fAngle", pUI->Get_UIDesc().fAngle);
		data.emplace("iTextureIdx", pUI->Get_UIDesc().iTextureIdx);
		data.emplace("iRenderLayer", pUI->Get_RenderLayer());
		data.emplace("strText", pUI->Get_FontText());
		data.emplace("vFontPosx", pUI->Get_UIDesc().vFontPos.x);
		data.emplace("vFontPosy", pUI->Get_UIDesc().vFontPos.y);
		data.emplace("vFontColorx", pUI->Get_UIDesc().vFontColor.x);
		data.emplace("vFontColory", pUI->Get_UIDesc().vFontColor.y);
		data.emplace("vFontColorz", pUI->Get_UIDesc().vFontColor.z);
		data.emplace("vFontColorw", pUI->Get_UIDesc().vFontColor.w);
		data.emplace("fFontSize", pUI->Get_UIDesc().fFontSize);
		data.emplace("bRenderFontText", pUI->Get_UIDesc().bRenderFontText);
		data.emplace("bRenderFontNum", pUI->Get_UIDesc().bRenderFontNum);

		Save.push_back(data);

	}

	USEGAMEINSTANCE;
	wstring strFolder = TEXT("UI/");
	wstring strTemp = towstring(strName);
	wstring strFullName = strFolder + strTemp;

	const wchar_t* szName = strFullName.c_str();
	pGameInstance->Save_Json(pGameInstance->json_Complete_Path(szName).c_str(), Save);


	return S_OK;
}

HRESULT CWindow_Tool_UI::Load_UIFile(string strName) 
{
	USEGAMEINSTANCE;
	json Data;

	for (auto& elem : m_UIObjectlist)
	{
		pGameInstance->Add_DeadObject(elem);
	}
	m_UIObjectlist.clear();

	wstring strFolder = TEXT("UI/");
	wstring strTemp = towstring(strName);
	wstring strFullName = strFolder + strTemp;

	const wchar_t* szName = strFullName.c_str();
	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(szName).c_str(), &Data);


	CGameObject* pGameObject = nullptr;
	_uint iPrototypeID;

	for (auto& elem : Data)
	{
		CUI::UIDESC UIDesc;
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
	
		pGameObject = pGameInstance->Add_GameObject(LEVEL_EDIT_UI, iPrototypeID, iPrototypeID, &UIDesc);

		CUI* pUI = (CUI*)pGameObject;

		pUI->Set_FontText(strTemp);

		m_UIObjectlist.push_back(pGameObject);
	}
	
	_uint i = 0;
	for (auto& elem : m_UIObjectlist)
	{
		CUI* pUI = (CUI*)elem;

		iTextureIdx[i] = pUI->Get_UIDesc().iTextureIdx;
		bRenderFontText[i] = pUI->Get_UIDesc().bRenderFontText;
		bRenderFontNum[i] = pUI->Get_UIDesc().bRenderFontNum;

		++i;
	}


	return S_OK;
}


char* CWindow_Tool_UI::SetUp_UIName(_uint iPrototypeID)
{
	if (ID_UINT(UI, UI_LOADING_SCREEN).iID == iPrototypeID)
	{
		return "UI_LoadingScreen";
	}
	else if (ID_UINT(UI, UI_CURSOR).iID == iPrototypeID)
	{
		return "UI_Cursor";
	}
	else if (ID_UINT(UI, UI_LOADING_BAR).iID == iPrototypeID)
	{
		return "UI_Loading_Bar";
	}
	else if (ID_UINT(UI, UI_LOADING_ANIM).iID == iPrototypeID)
	{
		return "UI_Loading_Anim";
	}
	else if (ID_UINT(UI, UI_LOGO).iID == iPrototypeID)
	{
		return "UI_Logo";
	}
	else if (ID_UINT(UI, UI_LOGO_BACK).iID == iPrototypeID)
	{
		return "UI_Logo_Back";
	}
	else if (ID_UINT(UI, UI_LOGO_START).iID == iPrototypeID)
	{
		return "UI_Logo_Start";
	}
	else if (ID_UINT(UI, UI_SHORTKEY).iID == iPrototypeID)
	{
		return "UI_Shortkey";
	}
	else if (ID_UINT(UI, UI_MENU_BACK).iID == iPrototypeID)
	{
		return "UI_Menu_Back";
	}
	else if (ID_UINT(UI, UI_MENU_BUTTON).iID == iPrototypeID)
	{
		return "UI_Menu_Button";
	}
	else if (ID_UINT(UI, UI_MAIN_ICON).iID == iPrototypeID)
	{
		return "UI_Main_Icon";
	}
	else if (ID_UINT(UI, UI_DASH_BAR).iID == iPrototypeID)
	{
		return "UI_Dash_Bar";
	}
	else if (ID_UINT(UI, UI_PLAYER_HP_BAR).iID == iPrototypeID)
	{
		return "UI_Player_Hp_Bar";
	}
	else if (ID_UINT(UI, UI_PLAYER_HP_BACK).iID == iPrototypeID)
	{
		return "UI_Player_Hp_Back";
	}
	else if (ID_UINT(UI, UI_MAIN_BUTTON).iID == iPrototypeID)
	{
		return "UI_Main_Button";
	}
	else if (ID_UINT(UI, UI_SKILL_GAUGE).iID == iPrototypeID)
	{
		return "UI_Skill_Gauge";
	}
	else if (ID_UINT(UI, UI_SKILL_ENABLE).iID == iPrototypeID)
	{
		return "UI_Skill_Enable";
	}
	else if (ID_UINT(UI, UI_STAMINA_BAR).iID == iPrototypeID)
	{
		return "UI_Stamina_Bar";
	}
	else if (ID_UINT(UI, UI_STAMINA_BACK).iID == iPrototypeID)
	{
		return "UI_Stamina_Back";
	}
	else if (ID_UINT(UI, UI_INVEN_BUTTON).iID == iPrototypeID)
	{
		return "UI_Inven_Button";
	}
	else if (ID_UINT(UI, UI_INVEN_ICON).iID == iPrototypeID)
	{
		return "UI_Inven_Icon";
	}
	else if (ID_UINT(UI, UI_WEAPON_SKILL).iID == iPrototypeID)
	{
		return "UI_Weapon_Skill";
	}
	else if (ID_UINT(UI, UI_SKILL_L).iID == iPrototypeID)
	{
		return "UI_Skill_L";
	}
	else if (ID_UINT(UI, UI_SKILL_R).iID == iPrototypeID)
	{
		return "UI_Skill_R";
	}
	else if (ID_UINT(UI, UI_SKILL_M).iID == iPrototypeID)
	{
		return "UI_Skill_M";
	}
	else if (ID_UINT(UI, UI_FOOD_WINDOW).iID == iPrototypeID)
	{
		return "UI_Food_Window";
	}
	else if (ID_UINT(UI, UI_BOSS_HP_BAR).iID == iPrototypeID)
	{
		return "UI_Boss_Hp_Bar";
	}
	else if (ID_UINT(UI, UI_BOSS_NAME).iID == iPrototypeID)
	{
		return "UI_Boss_Name";
	}
	else if (ID_UINT(UI, UI_BOSS_HP_BACK).iID == iPrototypeID)
	{
		return "UI_Boss_Hp_Back";
	}
	else if (ID_UINT(UI, UI_DAMAGE_NUM).iID == iPrototypeID)
	{
		return "UI_Damage_Num";
	}
	else if (ID_UINT(UI, UI_GUIDE_HAND_ANIM).iID == iPrototypeID)
	{
		return "UI_Guide_Hand_Anim";
	}
	else if (ID_UINT(UI, UI_GUIDE_WINDOW).iID == iPrototypeID)
	{
		return "UI_Guide_Window";
	}
	else if (ID_UINT(UI, UI_FOOD_ICON).iID == iPrototypeID)
	{
		return "UI_Food_Icon";
	}
	else if (ID_UINT(UI, UI_ITEM).iID == iPrototypeID)
	{
		return "UI_Item";
	}
	else if (ID_UINT(UI, UI_CURFOOD_ICON).iID == iPrototypeID)
	{
		return "UI_CurFood_Icon";
	}
	else if (ID_UINT(UI, UI_FOOD_NUM).iID == iPrototypeID)
	{
		return "UI_Food_Num";
	}
	else if (ID_UINT(UI, UI_CURFOOD_NUM).iID == iPrototypeID)
	{
		return "UI_CurFood_Num";
	}
	else if (ID_UINT(UI, UI_ATTACKED_NUM).iID == iPrototypeID)
	{
		return "UI_Attacked_Num";
	}
	else if (ID_UINT(UI, UI_HEAL_NUM).iID == iPrototypeID)
	{
		return "UI_Heal_Num";
	}
	else if (ID_UINT(UI, UI_ITEM_NUM).iID == iPrototypeID)
	{
		return "UI_Item_Num";
	}
	else if (ID_UINT(UI, UI_BOSS_HP_LERP).iID == iPrototypeID)
	{
		return "UI_Boss_Hp_Lerp";
	}
	else if (ID_UINT(UI, UI_QUEST_MARK).iID == iPrototypeID)
	{
		return "UI_Quest_Mark";
	}
	else if (ID_UINT(UI, UI_GUN_CROSSHAIR).iID == iPrototypeID)
	{
		return "UI_Gun_CrossHair";
	}
	else if (ID_UINT(UI, UI_GUIDE_TALK_ANIM).iID == iPrototypeID)
	{
		return "UI_Guide_Talk_Anim";
	}
	else if (ID_UINT(UI, UI_ALERT_WINDOW).iID == iPrototypeID)
	{
		return "UI_Alert_Window";
	}
	else if (ID_UINT(UI, UI_ALERT_ICON).iID == iPrototypeID)
	{
		return "UI_Alert_Icon";
	}
	else if (ID_UINT(UI, UI_TALK_WINDOW).iID == iPrototypeID)
	{
		return "UI_Talk_Window";
	}
	else if (ID_UINT(UI, UI_PROCEEDING).iID == iPrototypeID)
	{
		return "UI_Proceeding";
	}
	else if (ID_UINT(UI, UI_PLACEMARK).iID == iPrototypeID)
	{
		return "UI_PlaceMark";
	}
	else if (ID_UINT(UI, UI_DIST_NUM).iID == iPrototypeID)
	{
		return "UI_Dist_Num";
	}
	else if (ID_UINT(UI, UI_QUEST_NOTICE).iID == iPrototypeID)
	{
		return "UI_Quest_Notice";
	}
	else if (ID_UINT(UI, UI_ATTACK_TARGET).iID == iPrototypeID)
	{
		return "UI_Attack_Target";
	}
	else if (ID_UINT(UI, UI_ITEM_DETAIL).iID == iPrototypeID)
	{
		return "UI_Item_Detail";
	}
	else if (ID_UINT(UI, UI_COOK_BUTTON).iID == iPrototypeID)
	{
		return "UI_Cook_Button";
	}
	else if (ID_UINT(UI, UI_COOK_ICON).iID == iPrototypeID)
	{
		return "UI_Cook_Icon";
	}
	else if (ID_UINT(UI, UI_COOK_FOOD).iID == iPrototypeID)
	{
		return "UI_Cook_Food";
	}
	else if (ID_UINT(UI, UI_COOK_INGREDIENT).iID == iPrototypeID)
	{
		return "UI_Cook_Ingredient";
	}
	else if (ID_UINT(UI, UI_COOK_FOODSELECT).iID == iPrototypeID)
	{
		return "UI_Cook_FoodSelect";
	}
	else if (ID_UINT(UI, UI_COOK_DETAIL).iID == iPrototypeID)
	{
		return "UI_Cook_Detail";
	}
	else if (ID_UINT(UI, UI_COOK_INGRDNUM).iID == iPrototypeID)
	{
		return "UI_Cook_Ingrd_Num";
	}
	else if (ID_UINT(UI, UI_EQUIP_BUTTON).iID == iPrototypeID)
	{
		return "UI_Equip_Button";
	}
	else if (ID_UINT(UI, UI_EQUIP_WEAPON_L).iID == iPrototypeID)
	{
		return "UI_Equip_Weapon_L";
	}
	else if (ID_UINT(UI, UI_EQUIP_WEAPON_M).iID == iPrototypeID)
	{
		return "UI_Equip_Weapon_M";
	}
	else if (ID_UINT(UI, UI_EQUIP_WEAPON_R).iID == iPrototypeID)
	{
		return "UI_Equip_Weapon_R";
	}
	else if (ID_UINT(UI, UI_EQUIP_ICON).iID == iPrototypeID)
	{
		return "UI_Equip_Icon";
	}
	else if (ID_UINT(UI, UI_EQUIP_DETAIL).iID == iPrototypeID)
	{
		return "UI_Equip_Detail";
	}
	else if (ID_UINT(UI, UI_COOK_GAUGEBAR).iID == iPrototypeID)
	{
		return "UI_Cook_GaugeBar";
	}
	else if (ID_UINT(UI, UI_COOK_GAUGENUM).iID == iPrototypeID)
	{
		return "UI_Cook_GaugeNum";
	}
	else if (ID_UINT(UI, UI_SKILL_TIME).iID == iPrototypeID)
	{
		return "UI_Skill_Time";
	}
	else if (ID_UINT(UI, UI_EQUIP_REMOVE).iID == iPrototypeID)
	{
		return "UI_Equip_Remove";
	}
	else if (ID_UINT(UI, UI_ORDER_ICON).iID == iPrototypeID)
	{
		return "UI_Order_Icon";
	}
	else if (ID_UINT(UI, UI_ORDER_BUTTON).iID == iPrototypeID)
	{
		return "UI_Order_Button";
	}
	else if (ID_UINT(UI, UI_ORDER_ANIM).iID == iPrototypeID)
	{
		return "UI_Order_Anim";
	}
	else if (ID_UINT(UI, UI_ORDER_ITEM).iID == iPrototypeID)
	{
		return "UI_Order_Item";
	}
	else if (ID_UINT(UI, UI_ORDER_RESULT).iID == iPrototypeID)
	{
		return "UI_Order_Result";
	}
	else if (ID_UINT(UI, UI_ORDER_RESULTBUTTON).iID == iPrototypeID)
	{
		return "UI_Order_ResultButton";
	}
	else if (ID_UINT(UI, UI_ORDER_ITEMNAME).iID == iPrototypeID)
	{
		return "UI_Order_ItemName";
	}
	else if (ID_UINT(UI, UI_ORDER_TENRESULT).iID == iPrototypeID)
	{
		return "UI_Order_TenResult";
	}
	else if (ID_UINT(UI, UI_PLACENAME).iID == iPrototypeID)
	{
		return "UI_PlaceName";
	}
	else if (ID_UINT(UI, UI_ENERGY_GEAR).iID == iPrototypeID)
	{
		return "UI_Energy_Gear";
	}
	else if (ID_UINT(UI, UI_MINIMAP_ARROW).iID == iPrototypeID)
	{
		return "UI_Minimap_Arrow";
	}
	else if (ID_UINT(UI, UI_MINIMAP).iID == iPrototypeID)
	{
		return "UI_Minimap";
	}
	else if (ID_UINT(UI, UI_QUEST_COMPLETE).iID == iPrototypeID)
	{
		return "UI_Quest_Complete";
	}
	else if (ID_UINT(UI, UI_MINIGAME_START).iID == iPrototypeID)
	{
		return "UI_Minigame_Start";
	}
	else if (ID_UINT(UI, UI_MINIGAME_NOTICE).iID == iPrototypeID)
	{
		return "UI_Minigame_Notice";
	}
	else if (ID_UINT(UI, UI_MINIGAME_EXPLAIN).iID == iPrototypeID)
	{
		return "UI_Minigame_Explain";
	}
	else if (ID_UINT(UI, UI_MINIGAME_GAIN).iID == iPrototypeID)
	{
		return "UI_Minigame_Gain";
	}
	else if (ID_UINT(UI, UI_MINIGAME_RESULT).iID == iPrototypeID)
	{
		return "UI_Minigame_Result";
	}
	else if (ID_UINT(UI, UI_CRYSTAL_HP_BAR).iID == iPrototypeID)
	{
		return "UI_Crystal_Hp_Bar";
	}
	else if (ID_UINT(UI, UI_FIN).iID == iPrototypeID)
	{
		return "UI_Fin";
	}

	return nullptr;
}

void CWindow_Tool_UI::Free()
{

}

#endif