#include "stdafx.h"
#include "..\Public\Window_Tool_Effect.h"

#if defined(USE_IMGUI)

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Imgui_Manager.h"

#include "Easing_Utillity.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Effect_Factory.h"
#include "Effect_Manager.h"

CWindow_Tool_Effect::CWindow_Tool_Effect()
	: m_pGameInstance(GAMEINSTANCE)
{
	ZeroMemory(&io, sizeof(ImGuiIO));
}

CWindow_Tool_Effect::~CWindow_Tool_Effect()
{
}

void CWindow_Tool_Effect::On_EventMessege(EVENTDESC tEvent)
{
	if (LEVEL_CHANGE == tEvent.eEventType)
	{
		if (LEVEL_GAMEPLAY == (_uint)tEvent.lParam)
		{
			m_pTextureCom_Mask = (CTexture*)m_pGameInstance->Clone_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK));
			m_pTextureCom_Noise = (CTexture*)m_pGameInstance->Clone_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE));
			GAMEINSTANCE->Get_Component_Prototypes(LEVEL_STATIC, ID_UINT(MODEL_STATIC, MODEL_EFFECT), &m_mapMeshEffectModel_Prototypes, 2);
		}
	}
}

void CWindow_Tool_Effect::Initialize()
{
	CWindow_Imgui::Initialize_Children();

	GAMEINSTANCE->Add_CallbackList(this, LEVEL_CHANGE); // 레벨 바뀔 때 마다 Prototype 새로 가져오기.

	// Show & Load Json File
	StringvecClear(m_vecSavedFiles);
	read_directory(JSONPATH_EFFECT, m_vecSavedFiles);
}

void CWindow_Tool_Effect::Tick(_float fTimeDelta)
{
	io = ImGui::GetIO();
	//ImGui::SetNextWindowPos(ImVec2{ g_iWinCX * 0.3f, 0 });
	ImGui::SetNextWindowSize(ImVec2{ g_iWinCX * 0.4f, g_iWinCY * 0.29f });
	Show_Window_SaveLoad();

	//ImGui::SetNextWindowPos(ImVec2{ 0, 0 });
	ImGui::SetNextWindowSize(ImVec2{ g_iWinCX * 0.3f, g_iWinCY });
	Show_Window_MeshEffect();

	//ImGui::SetNextWindowPos(ImVec2{ g_iWinCX - (g_iWinCX * 0.3f),0 });
	ImGui::SetNextWindowSize(ImVec2{ g_iWinCX * 0.3f, g_iWinCY });
	Show_Window_Particle();

	CWindow_Imgui::Tick_Children(fTimeDelta);
}

void CWindow_Tool_Effect::Show_Window_SaveLoad()
{
	ImVec2 size = ImGui::GetItemRectSize();
	if (ImGui::Begin("EffectTool_SaveLoad", 0/*, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize*/))
	{
		ImGui::Text("LOAD SAVE Effects");

		// Mouse
		USEGAMEINSTANCE;
		if (KEY_DOWN(VK_SPACE))
		{
			m_bIsCameraLock = !m_bIsCameraLock;
		}
		if (m_bIsCameraLock)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.0f), "Camera Locked");
		}


		if (ImGui::Button("Delete Json", ImVec2(size.x * 0.4f, 0)))
		{
			ImGui::OpenPopup("Delete?");
		}
		ImGui::SameLine();
		if (ImGui::Button("Save Json", ImVec2(-FLT_MIN, 0)))
		{
			if (strcmp(m_szFileName_Cur, m_szFileName_Save))
			{
				strcpy_s(m_szFileName_Save, m_szFileName_Cur);
			}
			ImGui::OpenPopup("Save?");
		}

		// Show & Load Json File
		// 위에서 1번함.
		//read_directory(JSONPATH_EFFECT, m_vecSavedFiles);

		// Show Exist Json.
		if (ImGui::BeginListBox("##Effect Json Files", ImVec2(size.x * 0.4f, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			int n = 1;
			for (auto& File : m_vecSavedFiles)
			{
				const bool is_selected = (m_iJsonFile_idx == n);
				if (ImGui::Selectable(File.c_str(), is_selected))
					m_iJsonFile_idx = n;

				if (is_selected)
				{
					strcpy_s(m_szFileName_Cur, File.substr(0, File.length() - 5).c_str()); // ".json" 5글자 제거.
					ImGui::SetItemDefaultFocus();
				}
				n += 1;
			}
			ImGui::EndListBox();
		}

		// Load
		if (strcmp(m_szFileName_Cur, m_szFileName_Last)) // 새로운 이름 누르면 불러옴.
		{
			string str(m_szFileName_Cur);
			if (FAILED(CEffect_Manager::Get_Instance()->Load_EffectsFile(str)))
				MSG_BOX("Failed To Load_Effect");

			strcpy_s(m_szFileName_Last, m_szFileName_Cur);
		}

		// Save
		if (ImGui::BeginPopupModal("Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("File Name : ");
			ImGui::InputText(".json", m_szFileName_Save, IM_ARRAYSIZE(m_szFileName_Save));
			ImGui::Separator();

			static bool dont_ask_me_next_time = false;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::PopStyleVar();

			if (ImGui::Button("Save", ImVec2(120, 0)))
			{
				string str(m_szFileName_Save);
				if (!strcmp(m_szFileName_Save, m_szMetaFileName))
					MSG_BOX("Failed : Can't Change Meta File");
				else
				{
					if (FAILED(CEffect_Manager::Get_Instance()->Save_EffectsFile(str)))
					{
						MSG_BOX("Failed To Save_Effect");
					}
					else
					{
						StringvecClear(m_vecSavedFiles);
						read_directory(JSONPATH_EFFECT, m_vecSavedFiles);
						int n = 1;
						for (auto& File : m_vecSavedFiles)
						{
							if (!strcmp(File.c_str(), m_szFileName_Save))
								m_iJsonFile_idx = n;

							n += 1;
						}
						MSG_BOX_USER(TEXT("Successed!"), TEXT("Json Saved"));
					}
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		// Delete
		if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are You Sure?");
			ImGui::Separator();

			static bool dont_ask_me_next_time = false;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::PopStyleVar();

			if (ImGui::Button("Delete", ImVec2(120, 0)))
			{
				string str(m_szFileName_Cur);
				if (!strcmp(m_szFileName_Cur, m_szMetaFileName))
					MSG_BOX("Failed : Can't Change Meta File");
				else
				{
					string str1;
					string str2(m_szFileName_Cur);
					str1 = JSONPATH_EFFECT + str2 + ".json";
					if (-1 == remove(str1.c_str()))
						MSG_BOX("Failed To Delete Json");

					StringvecClear(m_vecSavedFiles);
					read_directory(JSONPATH_EFFECT, m_vecSavedFiles);
					int n = 1;
					for (auto& File : m_vecSavedFiles)
					{
						if (!strcmp(File.c_str(), m_szFileName_Save))
							m_iJsonFile_idx = n;

						n += 1;
					}
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		//ImGui::Text("Effect List");
		if (ImGui::BeginListBox("##Effects", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			//이펙트 매니저에 저장된 이펙트들을 보여준다.
			int n = 0;
			for (auto& Effect : CEffect_Manager::Get_Instance()->Get_Effects())
			{
				const bool is_selected = (m_iEffect_idx == n);
				if (ImGui::Selectable(Effect.first.c_str(), is_selected))
					m_iEffect_idx = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
				n += 1;
			}

			ImGui::EndListBox();
		}

		// //

		static char EffectName[128] = "";
		ImGui::InputText("Input Effect_Name", EffectName, IM_ARRAYSIZE(EffectName));
		if (ImGui::Button("Add_Effect"))
		{
			if (NULL != m_CurrentEffectDesc_Mesh.size() || NULL != m_CurrentEffectDesc_Particle.size())
			{
				string str(EffectName);
				if (NULL == str.size())
				{
					MSG_BOX("Please Input Effect Name!");
				}
				else
				{
					m_CurrentEffect_Name = str;
					if (FAILED(CEffect_Manager::Get_Instance()->Add_Effect(str, m_CurrentEffectDesc_Mesh, m_CurrentEffectDesc_Particle)))
						MSG_BOX("Failed Add_Effect");
				}
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Sub_Effect"))
		{
			// 선택된 키값을 찾아서 삭제하기.
			int n = 0;
			for (auto& Effect : CEffect_Manager::Get_Instance()->Get_Effects())
			{
				if (n == m_iEffect_idx)
				{
					CEffect_Manager::Get_Instance()->Delete_Effect(Effect.first);
				}
				n++;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Play_Effect"))
		{
			// 선택된 키값을 찾아서 실행하기.
			int n = 0;
			for (auto& Effect : CEffect_Manager::Get_Instance()->Get_Effects())
			{
				if (n == m_iEffect_idx)
				{
					_matrix matWorld = XMMatrixIdentity();
					CEffect_Manager::Get_Instance()->ExcuteEffect(Effect.first, matWorld);
				}
				n++;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Get_Effect_Component"))
		{
			// 선택된 키값을 찾아서 실행하기.
			int n = 0;
			for (auto& Effect : CEffect_Manager::Get_Instance()->Get_Effects())
			{
				if (n == m_iEffect_idx)
				{
					m_CurrentEffect_Name = Effect.second->Get_EffectDesc_EffectName();
					m_CurrentEffectDesc_Mesh = Effect.second->Get_EffectDesc_Mesh();
					m_CurrentEffectDesc_Particle = Effect.second->Get_EffectDesc_Particle();
				}
				n++;
			}
		}
	}
	ImGui::End();
}

void CWindow_Tool_Effect::Show_Window_MeshEffect()
{
	ImVec2 size = ImGui::GetItemRectSize();
	if (ImGui::Begin("EffectTool_MeshEffect", 0/*, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize*/))
	{
		static int MeshEffectDesc_iMeshIndex = 0;
		static string strTextureNum = "";
		strTextureNum = "Mesh Num: " + to_string(MeshEffectDesc_iMeshIndex + 1);
		ImGui::Text(strTextureNum.c_str());

		if (ImGui::BeginListBox("##Meshes", ImVec2(-FLT_MIN, 6 * ImGui::GetTextLineHeightWithSpacing())))
		{
			//for (int n = 0; n < m_mapMeshEffectModel_Prototypes.size(); n++)
			_uint n = 1;
			for (auto& Prototype : m_mapMeshEffectModel_Prototypes)
			{
				const bool is_selected = (MeshEffectDesc_iMeshIndex == n);
				if (ImGui::Selectable(Prototype.second->Get_ModelFileName().c_str(), is_selected))
					MeshEffectDesc_iMeshIndex = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
				n++;
			}

			ImGui::EndListBox();
		}

		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "Noise Texture");

		static int MeshEffectDesc_iNoiseIndex = 0;
		static _bool MeshEffectDesc_bIsDissolve = false;
		static int MeshEffectDesc_iDissolveNoiseTextureIndex = 0;
		static int MeshEffectDesc_iDissolveColorTextureIndex = 0;

		enum eNoiseTextureType { MESH_NOISE, DISSOLVE_NOISE, DISSOLVE_NOISE_COLOR };
		static int MeshEffect_NoiseTextureType = MESH_NOISE;
		ImGui::RadioButton("Mesh Noise", &MeshEffect_NoiseTextureType, MESH_NOISE); ImGui::SameLine();
		strTextureNum = "TexNum1: " + to_string(MeshEffectDesc_iNoiseIndex);
		ImGui::Text(strTextureNum.c_str());

		if (!MeshEffectDesc_bIsDissolve)
			ImGui::BeginDisabled();
		ImGui::RadioButton("Dissolve Noise", &MeshEffect_NoiseTextureType, DISSOLVE_NOISE); ImGui::SameLine();
		strTextureNum = "TexNum2: " + to_string(MeshEffectDesc_iDissolveNoiseTextureIndex);
		ImGui::Text(strTextureNum.c_str());
		ImGui::RadioButton("Dissolve Color", &MeshEffect_NoiseTextureType, DISSOLVE_NOISE_COLOR); ImGui::SameLine();
		strTextureNum = "TexNum3: " + to_string(MeshEffectDesc_iDissolveColorTextureIndex);
		ImGui::Text(strTextureNum.c_str());
		if (!MeshEffectDesc_bIsDissolve)
			ImGui::EndDisabled();


		if (ImGui::BeginListBox("##iNoiseIndex_Textures", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing())))
		{
			int i = 0;
			int Index = 0;
			if (nullptr != m_pTextureCom_Noise)
			{
				for (auto& TextureSRV : m_pTextureCom_Noise->Get_SRV())
				{
					int frame_padding = -1;                             // -1 == uses default padding (style.FramePadding)
					ImVec2 size = ImVec2(30.f, 30.f);                     // Size of the image we want to make visible
					ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
					ImVec2 uv1 = ImVec2(1.f, 1.f);							// UV coordinates for (32,32) in our texture
					ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
					ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint
					if (ImGui::ImageButton((void*)TextureSRV, size, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						if (MESH_NOISE == MeshEffect_NoiseTextureType)
						{
							MeshEffectDesc_iNoiseIndex = Index;
						}
						else if (DISSOLVE_NOISE == MeshEffect_NoiseTextureType)
						{
							MeshEffectDesc_iDissolveNoiseTextureIndex = Index;
						}
						else if (DISSOLVE_NOISE_COLOR == MeshEffect_NoiseTextureType)
						{
							MeshEffectDesc_iDissolveColorTextureIndex = Index;
						}
					}

					if (++i < 8)
						ImGui::SameLine();
					else
						i = 0;
					Index++;
				}
			}

			ImGui::EndListBox();
		}

		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "Mask Texture");
		static int MeshEffectDesc_iMaskIndex = 0;
		strTextureNum = "Mask TexNum: " + to_string(MeshEffectDesc_iMaskIndex);
		ImGui::Text(strTextureNum.c_str());

		if (ImGui::BeginListBox("##iMaskIndex_Textures", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			int i = 0;
			int Index = 0;
			if (nullptr != m_pTextureCom_Mask)
			{
				for (auto& TextureSRV : m_pTextureCom_Mask->Get_SRV())
				{
					int frame_padding = -1;                             // -1 == uses default padding (style.FramePadding)
					ImVec2 size = ImVec2(60.f, 60.f);                     // Size of the image we want to make visible
					ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
					ImVec2 uv1 = ImVec2(1.f, 1.f);							// UV coordinates for (32,32) in our texture
					ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
					ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint
					if (ImGui::ImageButton((void*)TextureSRV, size, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						MeshEffectDesc_iMaskIndex = Index;
					}

					if (++i < 5)
						ImGui::SameLine();
					else
						i = 0;
					Index++;
				}
			}

			ImGui::EndListBox();
		}
		/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Billboard
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "1. BillBoard");
		static _bool MeshEffectDesc_bIsBillboard = false;
		ImGui::Checkbox("- Enable Billboard", &MeshEffectDesc_bIsBillboard);

		// Life Time
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "2. Life Time");
		static float  MeshEffectDesc_fStartTimeOffset = 0.f;
		static float  MeshEffectDesc_fLifeDuration = 2.f;
		ImGui::Text("- Start Time Offset");
		if (ImGui::Button("Reset Offset", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fStartTimeOffset = 0.f; }
		ImGui::DragFloat("Offset", &MeshEffectDesc_fStartTimeOffset, 0.01f, 0.0f, 100.0f, "%.3f", 0);
		ImGui::Text("- Life Time Duration");
		if (ImGui::Button("Reset Duration", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fLifeDuration = 2.f; }
		ImGui::DragFloat("Duration", &MeshEffectDesc_fLifeDuration, 0.01f, 0.1f, 100.0f, "%.3f", 0);
		static _bool MeshEffectDesc_bIsLoop = false;
		ImGui::Checkbox("- Enable Loop", &MeshEffectDesc_bIsLoop);

		// Diffuse
		ImGui::Separator();
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.f, 1.f, 0.0f, 1.0f), "6. Diffuse");
		ImGui::SameLine();
		if (ImGui::Button("Easing Image")) ImGui::OpenPopup("Easing Image popup");

		static float MeshEffectDesc_vStartColorDiffuse[4] = { 1.f, 1.f, 1.f, 1.f };

		static int MeshEffectDesc_iColorEasingDiffuse_End = CEasing_Utillity::TYPE_End, MeshEffectDesc_iColorEasingDiffuse_Middle = CEasing_Utillity::TYPE_End;
		const char* combo_ColorEasing_value = szEasingTypes[MeshEffectDesc_iColorEasingDiffuse_End];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Diffuse Color Easing", combo_ColorEasing_value))
		{
			for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
			{
				const bool is_selected = (MeshEffectDesc_iColorEasingDiffuse_End == n);
				if (ImGui::Selectable(szEasingTypes[n], is_selected))
					MeshEffectDesc_iColorEasingDiffuse_End = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iColorEasingDiffuse_End)
		{
			const char* combo_ColorEasing_Middle_value = szEasingTypes[MeshEffectDesc_iColorEasingDiffuse_Middle];  // Pass in the preview value visible before opening the combo (it could be anything)
			if (ImGui::BeginCombo("Diffuse Middle Color Easing", combo_ColorEasing_Middle_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
				{
					const bool is_selected = (MeshEffectDesc_iColorEasingDiffuse_Middle == n);
					if (ImGui::Selectable(szEasingTypes[n], is_selected))
						MeshEffectDesc_iColorEasingDiffuse_Middle = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		ImGui::Text("-  Diffuse Start Color");
		if (ImGui::Button("Reset  Diffuse Start Color", ImVec2(size.x * 0.5f, 0)))
		{
			MeshEffectDesc_vStartColorDiffuse[0] = { 1.f };
			MeshEffectDesc_vStartColorDiffuse[1] = { 1.f };
			MeshEffectDesc_vStartColorDiffuse[2] = { 1.f };
			MeshEffectDesc_vStartColorDiffuse[3] = { 1.f };
		}
		ImGui::ColorEdit4("StartDiffuseColor", MeshEffectDesc_vStartColorDiffuse, ImGuiColorEditFlags_Float);

		static float MeshEffectDesc_fMiddleColorDiffuseTimePoint = MeshEffectDesc_fLifeDuration * 0.5;
		static float MeshEffectDesc_vMiddleColorDiffuse[4] = { 1.f, 1.f, 1.f, 1.f };
		static float MeshEffectDesc_vEndColorDiffuse[4] = { 1.f, 1.f, 1.f, 1.f };
		if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iColorEasingDiffuse_End)
		{
			if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iColorEasingDiffuse_Middle)
			{
				ImGui::Text("- Middle Diffuse Color");
				if (ImGui::Button("Reset Time Point_d ", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fMiddleColorDiffuseTimePoint = 0.f; }
				ImGui::DragFloat("Time Point_g", &MeshEffectDesc_fMiddleColorDiffuseTimePoint, 0.01f, 0.01f, MeshEffectDesc_fLifeDuration, "%.3f", 0);
				if (ImGui::Button("Reset Diffuse Middle Color", ImVec2(size.x * 0.5f, 0)))
				{
					MeshEffectDesc_vMiddleColorDiffuse[0] = { 1.f };
					MeshEffectDesc_vMiddleColorDiffuse[1] = { 1.f };
					MeshEffectDesc_vMiddleColorDiffuse[2] = { 1.f };
					MeshEffectDesc_vMiddleColorDiffuse[3] = { 1.f };
				}
				ImGui::ColorEdit4("Diffuse Middle Color", MeshEffectDesc_vMiddleColorDiffuse, ImGuiColorEditFlags_Float);
			}
			ImGui::Text("- Diffuse End Color");
			if (ImGui::Button("Reset Diffuse End Color", ImVec2(size.x * 0.5f, 0)))
			{
				MeshEffectDesc_vEndColorDiffuse[0] = { 1.f };
				MeshEffectDesc_vEndColorDiffuse[1] = { 1.f };
				MeshEffectDesc_vEndColorDiffuse[2] = { 1.f };
				MeshEffectDesc_vEndColorDiffuse[3] = { 1.f };
			}
			ImGui::ColorEdit4("Diffuse End Color", MeshEffectDesc_vEndColorDiffuse, ImGuiColorEditFlags_Float);
		}

		// Noise
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.f, 1.f, 0.0f, 1.0f), "6. Noise UV Speed");
		static _bool MeshEffectDesc_bIsClampSampling_Noise = false;
		ImGui::Checkbox("- Enable ClampSampling_Noise", &MeshEffectDesc_bIsClampSampling_Noise);
		ImGui::Text("- UVSpeedXY_Noise");
		static float  MeshEffectDesc_fUVSpeedXY_Noise[2] = { 0.f, 0.f };
		if (ImGui::Button("Reset UVSpeedX_Noise", ImVec2(size.x * 0.5f, 0))) MeshEffectDesc_fUVSpeedXY_Noise[0] = 0.f; ImGui::SameLine();
		if (ImGui::Button("Reset UVSpeedY_Noise", ImVec2(-FLT_MIN, 0))) MeshEffectDesc_fUVSpeedXY_Noise[1] = 0.f;
		ImGui::DragFloat2("UVSpeed_Noise", MeshEffectDesc_fUVSpeedXY_Noise, 0.01f, -100.0f, 100.0f);
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.f, 1.f, 0.0f, 1.0f), "7. Noise UV Scale Max");
		static _uint   MeshEffectDesc_iUVScaleXMax_Noise = 1;
		static _uint   MeshEffectDesc_iUVScaleYMax_Noise = 1;
		const _uint    iScalarStep_ = 1;
		ImGui::InputScalar("UVScaleXMax_Noise", ImGuiDataType_U8, &MeshEffectDesc_iUVScaleXMax_Noise, &iScalarStep_, NULL, "%u");
		ImGui::InputScalar("UVScaleYMax_Noise", ImGuiDataType_U8, &MeshEffectDesc_iUVScaleYMax_Noise, &iScalarStep_, NULL, "%u");
		static _bool MeshEffectDesc_bIsReverseXY_Noise = false;
		ImGui::Checkbox("- Reverse NoiseUV XY", &MeshEffectDesc_bIsReverseXY_Noise);

		// Mask Mode
		ImGui::Separator();
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.0f), "3. Mask Mode");
		enum MESHEFFECTTYPE { UVSPEEDMODE, SPRITEMODE, MODE_END };
		//static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
		static int MESHEFFECTTYPE_type = UVSPEEDMODE;
		ImGui::RadioButton("UVSpeed Mode", &MESHEFFECTTYPE_type, UVSPEEDMODE); ImGui::SameLine();
		ImGui::RadioButton("Sprite Mode", &MESHEFFECTTYPE_type, SPRITEMODE);
		static _bool MeshEffectDesc_bIsReverseXY_Mask = false;
		ImGui::Checkbox("- Reverse MaskUV XY", &MeshEffectDesc_bIsReverseXY_Mask);
		static bool MeshEffectDesc_bIsUVSpeedModeDisabled = false; // The Checkbox for that is inside the "Disabled" section at the bottom
		if (UVSPEEDMODE == MESHEFFECTTYPE_type)
		{
			MeshEffectDesc_bIsUVSpeedModeDisabled = false;
		}
		else
		{
			MeshEffectDesc_bIsUVSpeedModeDisabled = true;
		}

		if (MeshEffectDesc_bIsUVSpeedModeDisabled)
			ImGui::BeginDisabled();
		// UV Speed
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.0f), "4. Mask UVSpeed");
		static _bool MeshEffectDesc_bIsClampSampling_Mask = false;
		ImGui::Checkbox("- Enable ClampSampling", &MeshEffectDesc_bIsClampSampling_Mask);
		ImGui::Text("- UVSpeedXY");
		static float  MeshEffectDesc_fUVSpeedXY_Mask[2] = { 0.f, 0.f };
		if (ImGui::Button("Reset UVSpeedX", ImVec2(size.x * 0.5f, 0))) MeshEffectDesc_fUVSpeedXY_Mask[0] = 0.f; ImGui::SameLine();
		if (ImGui::Button("Reset UVSpeedY", ImVec2(-FLT_MIN, 0))) MeshEffectDesc_fUVSpeedXY_Mask[1] = 0.f;
		ImGui::DragFloat2("UVSpeed", MeshEffectDesc_fUVSpeedXY_Mask, 0.01f, -100.0f, 100.0f);

		if (MeshEffectDesc_bIsUVSpeedModeDisabled)
			ImGui::EndDisabled();

		if (!MeshEffectDesc_bIsUVSpeedModeDisabled)
			ImGui::BeginDisabled();
		// UV Scale
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.0f), "5. Mask Sprite");
		static _uint   MeshEffectDesc_iUVScaleXMax_Mask = 1;
		static _uint   MeshEffectDesc_iUVScaleYMax_Mask = 1;
		static _float  MeshEffectDesc_fSpriteSpeed = 0.05f;
		const _uint    iScalarStep = 1;
		ImGui::InputScalar("UVScaleXMax", ImGuiDataType_U8, &MeshEffectDesc_iUVScaleXMax_Mask, &iScalarStep, NULL, "%u");
		ImGui::InputScalar("UVScaleYMax", ImGuiDataType_U8, &MeshEffectDesc_iUVScaleYMax_Mask, &iScalarStep, NULL, "%u");
		ImGui::DragFloat("Sprite Speed", &MeshEffectDesc_fSpriteSpeed, 0.0001f, -100.f, 100.f, "%.4f", 0);

		if (!MeshEffectDesc_bIsUVSpeedModeDisabled)
			ImGui::EndDisabled();
		ImGui::Separator();


		// Shader
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "7. Shader");
		static _bool MeshEffectDesc_bIsSoftEffect = true;
		ImGui::Checkbox("===== Enable SoftEffect =====", &MeshEffectDesc_bIsSoftEffect);
		static _bool MeshEffectDesc_bIsGlow = false;
		ImGui::Checkbox("===== Enable Glow =====", &MeshEffectDesc_bIsGlow);

		static _bool MeshEffectDesc_bIsGlowFollowDiffuseColor = true;
		static float MeshEffectDesc_vStartColorGlow[4] = { 1.f, 1.f, 1.f, 1.f };
		static float MeshEffectDesc_fMiddleColorGlowTimePoint = MeshEffectDesc_fLifeDuration * 0.5;
		static float MeshEffectDesc_vMiddleColorGlow[4] = { 1.f, 1.f, 1.f, 1.f };
		static float MeshEffectDesc_vEndColorGlow[4] = { 1.f, 1.f, 1.f, 1.f };
		static int MeshEffectDesc_iColorEasingGlow_End = CEasing_Utillity::TYPE_End, MeshEffectDesc_iColorEasingGlow_Middle = CEasing_Utillity::TYPE_End;
		if (MeshEffectDesc_bIsGlow)
		{
			// Follow Diffuse Color Mode 
			// 하면 disable
			// 
			// 안하면 
			// 컬러 보간.
			ImGui::Checkbox("-- Follow Diffuse Color", &MeshEffectDesc_bIsGlowFollowDiffuseColor);

			if (MeshEffectDesc_bIsGlowFollowDiffuseColor)
				ImGui::BeginDisabled();

			if (ImGui::Button("Easing_Image")) ImGui::OpenPopup("Easing Image popup");


			const char* combo_ColorEasingGlow_value = szEasingTypes[MeshEffectDesc_iColorEasingGlow_End];  // Pass in the preview value visible before opening the combo (it could be anything)
			if (ImGui::BeginCombo("Glow Color Easing", combo_ColorEasingGlow_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
				{
					const bool is_selected = (MeshEffectDesc_iColorEasingGlow_End == n);
					if (ImGui::Selectable(szEasingTypes[n], is_selected))
						MeshEffectDesc_iColorEasingGlow_End = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iColorEasingGlow_End)
			{
				const char* combo_ColorEasingGlow_Middle_value = szEasingTypes[MeshEffectDesc_iColorEasingGlow_Middle];  // Pass in the preview value visible before opening the combo (it could be anything)
				if (ImGui::BeginCombo("Middle Glow Color Easing", combo_ColorEasingGlow_Middle_value))
				{
					for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
					{
						const bool is_selected = (MeshEffectDesc_iColorEasingGlow_Middle == n);
						if (ImGui::Selectable(szEasingTypes[n], is_selected))
							MeshEffectDesc_iColorEasingGlow_Middle = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}
			ImGui::Text("-  Glow Start Color (Random)");
			if (ImGui::Button("Reset  Glow Start Color", ImVec2(size.x * 0.5f, 0)))
			{
				MeshEffectDesc_vStartColorGlow[0] = { 1.f };
				MeshEffectDesc_vStartColorGlow[1] = { 1.f };
				MeshEffectDesc_vStartColorGlow[2] = { 1.f };
				MeshEffectDesc_vStartColorGlow[3] = { 1.f };
			}
			ImGui::ColorEdit4("StartGlowColor", MeshEffectDesc_vStartColorGlow, ImGuiColorEditFlags_Float);

			if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iColorEasingGlow_End)
			{
				if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iColorEasingGlow_Middle)
				{
					ImGui::Text("- Middle Glow Color");
					if (ImGui::Button("Reset Time Point_g ", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fMiddleColorGlowTimePoint = 0.f; }
					ImGui::DragFloat("Time Point_g ", &MeshEffectDesc_fMiddleColorGlowTimePoint, 0.01f, 0.01f, MeshEffectDesc_fLifeDuration, "%.3f", 0);
					if (ImGui::Button("Reset Glow Middle Color", ImVec2(size.x * 0.5f, 0)))
					{
						MeshEffectDesc_vMiddleColorGlow[0] = { 1.f };
						MeshEffectDesc_vMiddleColorGlow[1] = { 1.f };
						MeshEffectDesc_vMiddleColorGlow[2] = { 1.f };
						MeshEffectDesc_vMiddleColorGlow[3] = { 1.f };
					}
					ImGui::ColorEdit4("Glow Middle Color", MeshEffectDesc_vMiddleColorGlow, ImGuiColorEditFlags_Float);
				}
				ImGui::Text("- Glow End Color");
				if (ImGui::Button("Reset Glow End Color", ImVec2(size.x * 0.5f, 0)))
				{
					MeshEffectDesc_vEndColorGlow[0] = { 1.f };
					MeshEffectDesc_vEndColorGlow[1] = { 1.f };
					MeshEffectDesc_vEndColorGlow[2] = { 1.f };
					MeshEffectDesc_vEndColorGlow[3] = { 1.f };
				}
				ImGui::ColorEdit4("Glow End Color", MeshEffectDesc_vEndColorGlow, ImGuiColorEditFlags_Float);
			}

			if (MeshEffectDesc_bIsGlowFollowDiffuseColor)
				ImGui::EndDisabled();
		}

		// Dustortion
		static _bool MeshEffectDesc_bIsDistortion = false;
		ImGui::Checkbox("===== Enable Distortion =====", &MeshEffectDesc_bIsDistortion);
		static float MeshEffectDesc_fStartDistortionStrength = 0.f;
		static float MeshEffectDesc_fMiddleDistortionStrengthTimePoint = MeshEffectDesc_fLifeDuration * 0.5;
		static float MeshEffectDesc_fMiddleDistortionStrength = 1.f;
		static float MeshEffectDesc_fEndDistortionStrength = 1.f;
		static int MeshEffectDesc_iDistortionStrengthEasing_End = CEasing_Utillity::TYPE_End, MeshEffectDesc_iDistortionStrengthEasing_Middle = CEasing_Utillity::TYPE_End;
		if (MeshEffectDesc_bIsDistortion)
		{
			if (ImGui::Button("Easing_Image_di")) ImGui::OpenPopup("Easing Image popup");

			const char* combo_DistortionStrengthEasing_value = szEasingTypes[MeshEffectDesc_iDistortionStrengthEasing_End];  // Pass in the preview value visible before opening the combo (it could be anything)
			if (ImGui::BeginCombo("Distortion Easing", combo_DistortionStrengthEasing_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
				{
					const bool is_selected = (MeshEffectDesc_iDistortionStrengthEasing_End == n);
					if (ImGui::Selectable(szEasingTypes[n], is_selected))
						MeshEffectDesc_iDistortionStrengthEasing_End = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iDistortionStrengthEasing_End)
			{
				const char* combo_DistortionStrengthEasing_Middle_value = szEasingTypes[MeshEffectDesc_iDistortionStrengthEasing_Middle];  // Pass in the preview value visible before opening the combo (it could be anything)
				if (ImGui::BeginCombo("Middle Distortion Easing", combo_DistortionStrengthEasing_Middle_value))
				{
					for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
					{
						const bool is_selected = (MeshEffectDesc_iDistortionStrengthEasing_Middle == n);
						if (ImGui::Selectable(szEasingTypes[n], is_selected))
							MeshEffectDesc_iDistortionStrengthEasing_Middle = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}
			ImGui::Text("- Distortion Strength");
			if (ImGui::Button("Reset Distortion Strength ", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fStartDistortionStrength = 0.f; }
			ImGui::DragFloat("Distortion Strength", &MeshEffectDesc_fStartDistortionStrength, 0.01f, 0.f, 100.f, "%.3f", 0);

			if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iDistortionStrengthEasing_End)
			{
				if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iDistortionStrengthEasing_Middle)
				{
					ImGui::Text("- Distortion Middle Time Point");
					if (ImGui::Button("Reset Time Point_di ", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fMiddleDistortionStrengthTimePoint = 0.f; }
					ImGui::Text("- Middle DistortionStrength");
					ImGui::DragFloat("Time Point_di ", &MeshEffectDesc_fMiddleDistortionStrengthTimePoint, 0.01f, 0.01f, MeshEffectDesc_fLifeDuration, "%.3f", 0);
					if (ImGui::Button("Reset Middle DistortionStrength", ImVec2(size.x * 0.5f, 0)))
					{
						MeshEffectDesc_fMiddleDistortionStrength = 1.f;
					}
					ImGui::DragFloat("Middle Distortion Strength", &MeshEffectDesc_fMiddleDistortionStrength, 0.01f, 0.f, 100.f, "%.3f", 0);
				}
				ImGui::Text("- End Distortion Strength");
				if (ImGui::Button("Reset End Distortion", ImVec2(size.x * 0.5f, 0)))
				{
					MeshEffectDesc_fEndDistortionStrength = 1.f;
				}
				ImGui::DragFloat("End Distortion Strength", &MeshEffectDesc_fEndDistortionStrength, 0.01f, 0.f, 100.f, "%.3f", 0);
			}
		}

		// RimLight
		static _bool MeshEffectDesc_bIsRimLight = false;
		ImGui::Checkbox("===== Enable RimLight =====", &MeshEffectDesc_bIsRimLight);

		static float MeshEffectDesc_vStartRimLightColorRGBA[4] = { 1.f, 1.f, 1.f, 0.1f };
		static float MeshEffectDesc_fMiddleRimLightColorTimePoint = MeshEffectDesc_fLifeDuration * 0.5;
		static float MeshEffectDesc_vMiddleRimLightColorRGBA[4] = { 1.f, 1.f, 1.f, 0.1f };
		static float MeshEffectDesc_vEndRimLightColorRGBA[4] = { 1.f, 1.f, 1.f, 0.1f };
		static int MeshEffectDesc_iRimLightColorEasing_End = CEasing_Utillity::TYPE_End, MeshEffectDesc_iRimLightColorEasing_Middle = CEasing_Utillity::TYPE_End;
		if (MeshEffectDesc_bIsRimLight)
		{
			if (ImGui::Button("Easing_Image_rl")) ImGui::OpenPopup("Easing Image popup");

			const char* combo_ColorEasingRimLight_value = szEasingTypes[MeshEffectDesc_iRimLightColorEasing_End];  // Pass in the preview value visible before opening the combo (it could be anything)
			if (ImGui::BeginCombo("RimLight Color Easing", combo_ColorEasingRimLight_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
				{
					const bool is_selected = (MeshEffectDesc_iRimLightColorEasing_End == n);
					if (ImGui::Selectable(szEasingTypes[n], is_selected))
						MeshEffectDesc_iRimLightColorEasing_End = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iRimLightColorEasing_End)
			{
				const char* combo_ColorEasingRimLight_Middle_value = szEasingTypes[MeshEffectDesc_iRimLightColorEasing_Middle];  // Pass in the preview value visible before opening the combo (it could be anything)
				if (ImGui::BeginCombo("Middle RimLight Color Easing", combo_ColorEasingRimLight_Middle_value))
				{
					for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
					{
						const bool is_selected = (MeshEffectDesc_iRimLightColorEasing_Middle == n);
						if (ImGui::Selectable(szEasingTypes[n], is_selected))
							MeshEffectDesc_iRimLightColorEasing_Middle = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}
			ImGui::Text("-  RimLight Start Color (Random)");
			if (ImGui::Button("Reset RimLight Start Color", ImVec2(size.x * 0.5f, 0)))
			{
				MeshEffectDesc_vStartRimLightColorRGBA[0] = { 1.f };
				MeshEffectDesc_vStartRimLightColorRGBA[1] = { 1.f };
				MeshEffectDesc_vStartRimLightColorRGBA[2] = { 1.f };
				MeshEffectDesc_vStartRimLightColorRGBA[3] = { 1.f };
			}
			ImGui::ColorEdit4("RimLight StartColor", MeshEffectDesc_vStartRimLightColorRGBA, ImGuiColorEditFlags_Float);

			if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iRimLightColorEasing_End)
			{
				if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iRimLightColorEasing_Middle)
				{
					ImGui::Text("- Middle RimLight Color");
					if (ImGui::Button("Reset Time Point_rl ", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fMiddleRimLightColorTimePoint = 0.f; }
					ImGui::DragFloat("Time Point_rl ", &MeshEffectDesc_fMiddleRimLightColorTimePoint, 0.01f, 0.01f, MeshEffectDesc_fLifeDuration, "%.3f", 0);
					if (ImGui::Button("Reset RimLight Middle Color", ImVec2(size.x * 0.5f, 0)))
					{
						MeshEffectDesc_vMiddleRimLightColorRGBA[0] = { 1.f };
						MeshEffectDesc_vMiddleRimLightColorRGBA[1] = { 1.f };
						MeshEffectDesc_vMiddleRimLightColorRGBA[2] = { 1.f };
						MeshEffectDesc_vMiddleRimLightColorRGBA[3] = { 1.f };
					}
					ImGui::ColorEdit4("RimLight Middle Color", MeshEffectDesc_vMiddleRimLightColorRGBA, ImGuiColorEditFlags_Float);
				}
				ImGui::Text("- RimLight End Color");
				if (ImGui::Button("Reset RimLight End Color", ImVec2(size.x * 0.5f, 0)))
				{
					MeshEffectDesc_vEndRimLightColorRGBA[0] = { 1.f };
					MeshEffectDesc_vEndRimLightColorRGBA[1] = { 1.f };
					MeshEffectDesc_vEndRimLightColorRGBA[2] = { 1.f };
					MeshEffectDesc_vEndRimLightColorRGBA[3] = { 1.f };
				}
				ImGui::ColorEdit4("RimLight End Color", MeshEffectDesc_vEndRimLightColorRGBA, ImGuiColorEditFlags_Float);
			}
		}

		// Dissolve
		//static _bool MeshEffectDesc_bIsDissolve = false;
		ImGui::Checkbox("===== Enable Dissolve =====", &MeshEffectDesc_bIsDissolve);
		static float MeshEffectDesc_vDissolveColorRGBA[4] = { 1.f, 1.f,1.f, 1.f };
		static float MeshEffectDesc_fDissolveStrength = 0.1f;
		if (MeshEffectDesc_bIsDissolve)
		{
			static string strTextureNum = "";
			strTextureNum = "Noise Texture Num: " + to_string(MeshEffectDesc_iDissolveNoiseTextureIndex);
			ImGui::Text(strTextureNum.c_str());
			strTextureNum = "Color Texture Num: " + to_string(MeshEffectDesc_iDissolveColorTextureIndex);
			ImGui::Text(strTextureNum.c_str());

			ImGui::Text("- Dissolve Color");
			if (ImGui::Button("Reset Dissolve Color", ImVec2(size.x * 0.5f, 0)))
			{
				MeshEffectDesc_vDissolveColorRGBA[0] = { 1.f };
				MeshEffectDesc_vDissolveColorRGBA[1] = { 1.f };
				MeshEffectDesc_vDissolveColorRGBA[2] = { 1.f };
				MeshEffectDesc_vDissolveColorRGBA[3] = { 1.f };
			}
			ImGui::ColorEdit4("Dissolve Color", MeshEffectDesc_vDissolveColorRGBA, ImGuiColorEditFlags_Float);

			if (ImGui::Button("Reset Dissolve ", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fDissolveStrength = 0.f; }
			ImGui::DragFloat("Dissolve Strength", &MeshEffectDesc_fDissolveStrength, 0.001f, 0.f, 10.f, "%.3f", 0);
		}


		ImGui::BeginDisabled();
		// Trail
		static _bool MeshEffectDesc_bIsTrail = false;
		ImGui::Checkbox("- Enable TrailBuffer", &MeshEffectDesc_bIsTrail);
		static float MeshEffectDesc_vTrailColor[4] = { 1.f, 1.f, 1.f, 1.f };
		static float MeshEffectDesc_fTrailStrengh = 0.1f;
		if (MeshEffectDesc_bIsTrail)
		{
			ImGui::Text("- TrailBuffer Color");
			if (ImGui::Button("Reset TrailBuffer Color", ImVec2(size.x * 0.5f, 0)))
			{
				MeshEffectDesc_vTrailColor[0] = { 1.f };
				MeshEffectDesc_vTrailColor[1] = { 1.f };
				MeshEffectDesc_vTrailColor[2] = { 1.f };
				MeshEffectDesc_vTrailColor[3] = { 1.f };
			}
			ImGui::ColorEdit4("TrailBuffer Color", MeshEffectDesc_vTrailColor, ImGuiColorEditFlags_Float);
			if (ImGui::Button("Reset TrailBuffer ", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fTrailStrengh = 0.f; }
			ImGui::DragFloat("TrailBuffer Strength", &MeshEffectDesc_fTrailStrengh, 0.01f, 0.f, 100.f, "%.3f", 0);
		}

		// Blur
		static _bool MeshEffectDesc_bIsBlur = false;
		ImGui::Checkbox("- Enable Blur", &MeshEffectDesc_bIsBlur);

		ImGui::EndDisabled();

		// Position Offset
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "8. Position Offset");
		static float MeshEffectDesc_vPosition[3] = { 0.f, 0.f, 0.f };
		if (ImGui::Button("Reset Position Offset", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_vPosition[0] = 0.f;  MeshEffectDesc_vPosition[1] = 0.f;  MeshEffectDesc_vPosition[2] = 0.f; }
		ImGui::DragFloat3("Offset", MeshEffectDesc_vPosition, 0.01f, -100.0f, 100.0f);

		// Speed
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "9. Speed");
		ImGui::SameLine();
		if (ImGui::Button("Easing Image ")) ImGui::OpenPopup("Easing Image popup");
		static int MeshEffectDesc_iSpeedEasing_End = CEasing_Utillity::TYPE_End, MeshEffectDesc_iSpeedEasing_Middle = CEasing_Utillity::TYPE_End;

		enum ContentsType { SPEED_EASING_MODE, ACCELATION_MODE, CONTENTS_END };
		static int MeshEffect_Moving_type = SPEED_EASING_MODE;
		ImGui::RadioButton("Speed Easing Mode", &MeshEffect_Moving_type, SPEED_EASING_MODE); ImGui::SameLine();
		ImGui::RadioButton("Accelation Mode", &MeshEffect_Moving_type, ACCELATION_MODE);
		static bool MeshEffectDesc_bSpeedModeDisable = false; // The Checkbox for that is inside the "Disabled" section at the bottom
		if (SPEED_EASING_MODE == MeshEffect_Moving_type)
		{
			MeshEffectDesc_bSpeedModeDisable = false;
		}
		else
		{
			MeshEffectDesc_bSpeedModeDisable = true;
		}

		if (MeshEffectDesc_bSpeedModeDisable)
			ImGui::BeginDisabled();

		const char* combo_SpeedEasing_value = szEasingTypes[MeshEffectDesc_iSpeedEasing_End];
		if (ImGui::BeginCombo("Speed Easing", combo_SpeedEasing_value))
		{
			for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
			{
				const bool is_selected = (MeshEffectDesc_iSpeedEasing_End == n);
				if (ImGui::Selectable(szEasingTypes[n], is_selected))
					MeshEffectDesc_iSpeedEasing_End = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iSpeedEasing_End)
		{
			const char* combo_SpeedEasing_Middle_value = szEasingTypes[MeshEffectDesc_iSpeedEasing_Middle];
			if (ImGui::BeginCombo("Middle Speed Easing", combo_SpeedEasing_Middle_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
				{
					const bool is_selected = (MeshEffectDesc_iSpeedEasing_Middle == n);
					if (ImGui::Selectable(szEasingTypes[n], is_selected))
						MeshEffectDesc_iSpeedEasing_Middle = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		if (MeshEffectDesc_bSpeedModeDisable)
			ImGui::EndDisabled();

		ImGui::Text("- Start Speed X,Y,Z");
		static float MeshEffectDesc_vStartSpeedXYZ[3] = { 0.f, 0.f, 0.f };
		if (ImGui::Button("Reset Speed", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_vStartSpeedXYZ[0] = 0.f;  MeshEffectDesc_vStartSpeedXYZ[1] = 0.f;  MeshEffectDesc_vStartSpeedXYZ[2] = 0.f; }
		ImGui::DragFloat3("Start Speed", MeshEffectDesc_vStartSpeedXYZ, 0.01f, -100.0f, 100.0f);

		static float MeshEffectDesc_fMiddleSpeedTimePoint = MeshEffectDesc_fLifeDuration * 0.5;
		static float MeshEffectDesc_vMiddleSpeed[3] = { 1.f, 1.f, 1.f };
		static float MeshEffectDesc_vEndSpeed[3] = { 1.f, 1.f, 1.f };

		if (MeshEffectDesc_bSpeedModeDisable)
			ImGui::BeginDisabled();
		if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iSpeedEasing_End)
		{
			if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iSpeedEasing_Middle)
			{
				ImGui::Text("- Middle Speed Ratio");
				if (ImGui::Button("Reset Time Point_sp ", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fMiddleSpeedTimePoint = 0.f; }
				ImGui::DragFloat("Time Point_sp", &MeshEffectDesc_fMiddleSpeedTimePoint, 0.01f, 0.01f, MeshEffectDesc_fLifeDuration, "%.3f", 0);
				if (ImGui::Button("Reset Middle Speed", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_vMiddleSpeed[0] = 0.f;  MeshEffectDesc_vMiddleSpeed[1] = 0.f;  MeshEffectDesc_vMiddleSpeed[2] = 0.f; }
				ImGui::DragFloat3("Middle Speed", MeshEffectDesc_vMiddleSpeed, 0.01f, -100.0f, 100.0f);
			}
			ImGui::Text("- End Speed Ratio");
			if (ImGui::Button("Reset End Speed", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_vEndSpeed[0] = 0.f;  MeshEffectDesc_vEndSpeed[1] = 0.f;  MeshEffectDesc_vEndSpeed[2] = 0.f; }
			ImGui::DragFloat3("End Speed", MeshEffectDesc_vEndSpeed, 0.01f, -100.0f, 100.0f);
		}
		if (MeshEffectDesc_bSpeedModeDisable)
			ImGui::EndDisabled();

		if (!MeshEffectDesc_bSpeedModeDisable)
			ImGui::BeginDisabled();

		// Speed Accelation
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "10. Speed Accelation");
		ImGui::Text("- Speed Accelation");
		static float MeshEffectDesc_fSpeedAccelation[3] = { 0.f, 0.f, 0.f };
		if (ImGui::Button("Reset Speed Accelation", ImVec2(size.x * 0.5f, 0)))
		{
			MeshEffectDesc_fSpeedAccelation[0] = 0.f;
			MeshEffectDesc_fSpeedAccelation[1] = 0.f;
			MeshEffectDesc_fSpeedAccelation[2] = 0.f;
		}
		ImGui::DragFloat3("Speed Accelation XYZ", MeshEffectDesc_fSpeedAccelation, 0.01f, -100.0f, 100.0f, "%.02f");

		if (!MeshEffectDesc_bSpeedModeDisable)
			ImGui::EndDisabled();

		// Scale
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "11. Scale");
		ImGui::SameLine();
		if (ImGui::Button(" Easing Image ")) ImGui::OpenPopup("Easing Image popup");
		static float MeshEffectDesc_vStartScaleRUL[3] = { 1.f, 1.f, 1.f };
		static int MeshEffectDesc_iScaleEasing_End = CEasing_Utillity::TYPE_End, MeshEffectDesc_iScaleEasing_Middle = CEasing_Utillity::TYPE_End;
		const char* combo_ScaleEasing_value = szEasingTypes[MeshEffectDesc_iScaleEasing_End];
		if (ImGui::BeginCombo("Scale Easing", combo_ScaleEasing_value))
		{
			for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
			{
				const bool is_selected = (MeshEffectDesc_iScaleEasing_End == n);
				if (ImGui::Selectable(szEasingTypes[n], is_selected))
					MeshEffectDesc_iScaleEasing_End = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iScaleEasing_End)
		{
			const char* combo_ScaleEasing_Middle_value = szEasingTypes[MeshEffectDesc_iScaleEasing_Middle];
			if (ImGui::BeginCombo("Middle Scale Easing Ratio", combo_ScaleEasing_Middle_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
				{
					const bool is_selected = (MeshEffectDesc_iScaleEasing_Middle == n);
					if (ImGui::Selectable(szEasingTypes[n], is_selected))
						MeshEffectDesc_iScaleEasing_Middle = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		ImGui::Text("- Start Scale");
		if (ImGui::Button("Reset StartScale", ImVec2(size.x * 0.5f, 0)))
		{
			MeshEffectDesc_vStartScaleRUL[0] = 1.f;  MeshEffectDesc_vStartScaleRUL[1] = 1.f;  MeshEffectDesc_vStartScaleRUL[2] = 1.f;
		}
		ImGui::DragFloat3("StartScale", MeshEffectDesc_vStartScaleRUL, 0.01f, 0.01f, 100.0f);

		static float MeshEffectDesc_fMiddleScaleTimePoint = MeshEffectDesc_fLifeDuration * 0.5;
		static float MeshEffectDesc_vMiddleScaleRUL[3] = { 1.f, 1.f, 1.f };
		static float MeshEffectDesc_vEndScaleRUL[3] = { 1.f, 1.f, 1.f };
		if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iScaleEasing_End)
		{
			if (CEasing_Utillity::TYPE_End != MeshEffectDesc_iScaleEasing_Middle)
			{
				ImGui::Text("- Middle Scale Ratio");
				if (ImGui::Button("Reset Time Point_s ", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_fMiddleScaleTimePoint = 0.f; }
				ImGui::DragFloat("Time Point_s", &MeshEffectDesc_fMiddleScaleTimePoint, 0.01f, 0.01f, MeshEffectDesc_fLifeDuration, "%.3f", 0);
				if (ImGui::Button("Reset Middle Scale", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_vMiddleScaleRUL[0] = 1.f;  MeshEffectDesc_vMiddleScaleRUL[1] = 1.f;  MeshEffectDesc_vMiddleScaleRUL[2] = 1.f; }
				ImGui::DragFloat3("Middle Scale", MeshEffectDesc_vMiddleScaleRUL, 0.01f, 0.01f, 100.0f);
			}
			ImGui::Text("- End Scale Ratio");
			if (ImGui::Button("Reset End Scale", ImVec2(size.x * 0.5f, 0))) { MeshEffectDesc_vEndScaleRUL[0] = 1.f;  MeshEffectDesc_vEndScaleRUL[1] = 1.f;  MeshEffectDesc_vEndScaleRUL[2] = 1.f; }
			ImGui::DragFloat3("End Scale", MeshEffectDesc_vEndScaleRUL, 0.01f, 0.01f, 100.0f);
		}

		// Rotate // Degree
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "12. Rotate(Degree / Sec)");
		ImGui::Text("- Rotation(Degree)(Right Up Look)");
		static float MeshEffectDesc_vRotationRUL[3] = { 0.f, 0.f, 0.f };
		if (ImGui::Button("Reset Rotation RUL", ImVec2(size.x * 0.5f, 0)))
		{
			MeshEffectDesc_vRotationRUL[0] = 0.f;  MeshEffectDesc_vRotationRUL[1] = 0.f;  MeshEffectDesc_vRotationRUL[2] = 0.f;
		}
		ImGui::DragFloat3("Rotation RUL", MeshEffectDesc_vRotationRUL, 0.1f, -720.0f, 720.f);
		ImGui::Text("- Turn(Degree) Velocity(Right Up Look)");
		static float MeshEffectDesc_vRotationSpeedRUL[3] = { 0.f, 0.f, 0.f };
		if (ImGui::Button("Reset Rotation Speed", ImVec2(size.x * 0.5f, 0)))
		{
			MeshEffectDesc_vRotationSpeedRUL[0] = 0.f;  MeshEffectDesc_vRotationSpeedRUL[1] = 0.f;  MeshEffectDesc_vRotationSpeedRUL[2] = 0.f;
		}
		ImGui::DragFloat3("Speed RUL", MeshEffectDesc_vRotationSpeedRUL, 0.1f, -720.0f, 720.f);
		ImGui::Text("- Turn(Degree) Accelation");
		static float  MeshEffectDesc_fRotationAccelation[3] = { 0.f, 0.f , 0.f };
		if (ImGui::Button("Reset Rotation Acc", ImVec2(size.x * 0.5f, 0)))
		{
			MeshEffectDesc_fRotationAccelation[0] = 0.f;
			MeshEffectDesc_fRotationAccelation[1] = 0.f;
			MeshEffectDesc_fRotationAccelation[2] = 0.f;
		}
		ImGui::DragFloat3("Rotation Accelation", MeshEffectDesc_fRotationAccelation, 0.01f, -100.0f, 100.0f);

		m_CurrentMeshEffectDesc.iMeshIndex = MeshEffectDesc_iMeshIndex;
		m_CurrentMeshEffectDesc.iNoiseIndex = MeshEffectDesc_iNoiseIndex;
		m_CurrentMeshEffectDesc.iMaskIndex = MeshEffectDesc_iMaskIndex;
		m_CurrentMeshEffectDesc.bIsBillBoard = MeshEffectDesc_bIsBillboard;
		m_CurrentMeshEffectDesc.fLifeTime = { MeshEffectDesc_fStartTimeOffset, MeshEffectDesc_fLifeDuration };
		m_CurrentMeshEffectDesc.bIsLoop = MeshEffectDesc_bIsLoop;
		m_CurrentMeshEffectDesc.bIsClampSampling_Noise = MeshEffectDesc_bIsClampSampling_Noise;
		m_CurrentMeshEffectDesc.bIsReverseXY_Noise = MeshEffectDesc_bIsReverseXY_Noise;
		m_CurrentMeshEffectDesc.fUVSpeedXY_Noise = { MeshEffectDesc_fUVSpeedXY_Noise[0], MeshEffectDesc_fUVSpeedXY_Noise[1] };
		m_CurrentMeshEffectDesc.iUVScaleXMax_Noise = MeshEffectDesc_iUVScaleXMax_Noise;
		m_CurrentMeshEffectDesc.iUVScaleYMax_Noise = MeshEffectDesc_iUVScaleYMax_Noise;
		m_CurrentMeshEffectDesc.bIsUVSpeedMode = !MeshEffectDesc_bIsUVSpeedModeDisabled; // Disable 이므로 반대 값 저장.
		m_CurrentMeshEffectDesc.bIsClampSampling_Mask = MeshEffectDesc_bIsClampSampling_Mask;
		m_CurrentMeshEffectDesc.bIsReverseXY_Mask = MeshEffectDesc_bIsReverseXY_Mask;
		m_CurrentMeshEffectDesc.fUVSpeedXY_Mask = { MeshEffectDesc_fUVSpeedXY_Mask[0], MeshEffectDesc_fUVSpeedXY_Mask[1] };
		m_CurrentMeshEffectDesc.iUVScaleXMax_Mask = MeshEffectDesc_iUVScaleXMax_Mask;
		m_CurrentMeshEffectDesc.iUVScaleYMax_Mask = MeshEffectDesc_iUVScaleYMax_Mask;
		m_CurrentMeshEffectDesc.fSpriteSpeed = MeshEffectDesc_fSpriteSpeed;

		m_CurrentMeshEffectDesc.vStartDiffuseColorRGBA = { MeshEffectDesc_vStartColorDiffuse[0], MeshEffectDesc_vStartColorDiffuse[1], MeshEffectDesc_vStartColorDiffuse[2], MeshEffectDesc_vStartColorDiffuse[3] };
		m_CurrentMeshEffectDesc.iDiffuseColorEasing_End = MeshEffectDesc_iColorEasingDiffuse_End;
		m_CurrentMeshEffectDesc.iDiffuseColorEasing_Middle = MeshEffectDesc_iColorEasingDiffuse_Middle;
		m_CurrentMeshEffectDesc.fMiddleDiffuseColorTimePoint = MeshEffectDesc_fMiddleColorDiffuseTimePoint;
		m_CurrentMeshEffectDesc.vMiddleDiffuseColorRGBA = { MeshEffectDesc_vMiddleColorDiffuse[0], MeshEffectDesc_vMiddleColorDiffuse[1], MeshEffectDesc_vMiddleColorDiffuse[2] , MeshEffectDesc_vMiddleColorDiffuse[3] };
		m_CurrentMeshEffectDesc.vEndDiffuseColorRGBA = { MeshEffectDesc_vEndColorDiffuse[0], MeshEffectDesc_vEndColorDiffuse[1], MeshEffectDesc_vEndColorDiffuse[2] , MeshEffectDesc_vEndColorDiffuse[3] };
		m_CurrentMeshEffectDesc.bIsSoftEffect = MeshEffectDesc_bIsSoftEffect;

		m_CurrentMeshEffectDesc.bIsGlow = MeshEffectDesc_bIsGlow;
		m_CurrentMeshEffectDesc.bIsGlowFollowDiffuseColor = MeshEffectDesc_bIsGlowFollowDiffuseColor;
		m_CurrentMeshEffectDesc.vStartGlowColorRGBA = { MeshEffectDesc_vStartColorGlow[0], MeshEffectDesc_vStartColorGlow[1], MeshEffectDesc_vStartColorGlow[2] , MeshEffectDesc_vStartColorGlow[3] };
		m_CurrentMeshEffectDesc.iGlowColorEasing_End = MeshEffectDesc_iColorEasingGlow_End;
		m_CurrentMeshEffectDesc.iGlowColorEasing_Middle = MeshEffectDesc_iColorEasingGlow_Middle;
		m_CurrentMeshEffectDesc.fMiddleGlowColorTimePoint = MeshEffectDesc_fMiddleColorGlowTimePoint;
		m_CurrentMeshEffectDesc.vMiddleGlowColorRGBA = { MeshEffectDesc_vMiddleColorGlow[0], MeshEffectDesc_vMiddleColorGlow[1], MeshEffectDesc_vMiddleColorGlow[2] , MeshEffectDesc_vMiddleColorGlow[3] };
		m_CurrentMeshEffectDesc.vEndGlowColorRGBA = { MeshEffectDesc_vEndColorGlow[0], MeshEffectDesc_vEndColorGlow[1], MeshEffectDesc_vEndColorGlow[2] , MeshEffectDesc_vEndColorGlow[3] };

		m_CurrentMeshEffectDesc.bIsDistortion = MeshEffectDesc_bIsDistortion;
		m_CurrentMeshEffectDesc.fStartDistortionStrength = MeshEffectDesc_fStartDistortionStrength;
		m_CurrentMeshEffectDesc.iDistortionStrengthEasing_End = MeshEffectDesc_iDistortionStrengthEasing_End;
		m_CurrentMeshEffectDesc.iDistortionStrengthEasing_Middle = MeshEffectDesc_iDistortionStrengthEasing_Middle;
		m_CurrentMeshEffectDesc.fMiddleDistortionStrengthTimePoint = MeshEffectDesc_fMiddleDistortionStrengthTimePoint;
		m_CurrentMeshEffectDesc.fMiddleDistortionStrength = MeshEffectDesc_fMiddleDistortionStrength;
		m_CurrentMeshEffectDesc.fEndDistortionStrength = MeshEffectDesc_fEndDistortionStrength;

		m_CurrentMeshEffectDesc.bIsRimLight = MeshEffectDesc_bIsRimLight;
		m_CurrentMeshEffectDesc.vStartRimLightColorRGBA = { MeshEffectDesc_vStartRimLightColorRGBA[0], MeshEffectDesc_vStartRimLightColorRGBA[1], MeshEffectDesc_vStartRimLightColorRGBA[2], MeshEffectDesc_vStartRimLightColorRGBA[3] };
		m_CurrentMeshEffectDesc.iRimLightColorEasing_End = MeshEffectDesc_iRimLightColorEasing_End;
		m_CurrentMeshEffectDesc.iRimLightColorEasing_Middle = MeshEffectDesc_iRimLightColorEasing_Middle;
		m_CurrentMeshEffectDesc.fMiddleRimLightColorTimePoint = MeshEffectDesc_fMiddleRimLightColorTimePoint;
		m_CurrentMeshEffectDesc.vMiddleRimLightColorRGBA = { MeshEffectDesc_vMiddleRimLightColorRGBA[0], MeshEffectDesc_vMiddleRimLightColorRGBA[1], MeshEffectDesc_vMiddleRimLightColorRGBA[2], MeshEffectDesc_vMiddleRimLightColorRGBA[3] };
		m_CurrentMeshEffectDesc.vEndRimLightColorRGBA = { MeshEffectDesc_vEndRimLightColorRGBA[0], MeshEffectDesc_vEndRimLightColorRGBA[1], MeshEffectDesc_vEndRimLightColorRGBA[2], MeshEffectDesc_vEndRimLightColorRGBA[3] };

		m_CurrentMeshEffectDesc.bIsDissolve = MeshEffectDesc_bIsDissolve;
		m_CurrentMeshEffectDesc.iDissolveNoiseTextureIndex = MeshEffectDesc_iDissolveNoiseTextureIndex;
		m_CurrentMeshEffectDesc.iDissolveColorTextureIndex = MeshEffectDesc_iDissolveColorTextureIndex;
		m_CurrentMeshEffectDesc.vDissolveColorRGBA = { MeshEffectDesc_vDissolveColorRGBA[0], MeshEffectDesc_vDissolveColorRGBA[1], MeshEffectDesc_vDissolveColorRGBA[2], MeshEffectDesc_vDissolveColorRGBA[3] };
		m_CurrentMeshEffectDesc.fDissolveStrength = MeshEffectDesc_fDissolveStrength;

		m_CurrentMeshEffectDesc.bIsTrail = MeshEffectDesc_bIsTrail;
		m_CurrentMeshEffectDesc.vTrailColor = { MeshEffectDesc_vTrailColor[0], MeshEffectDesc_vTrailColor[1], MeshEffectDesc_vTrailColor[2], MeshEffectDesc_vTrailColor[3] };
		m_CurrentMeshEffectDesc.fTrailStrengh = MeshEffectDesc_fTrailStrengh;

		m_CurrentMeshEffectDesc.bIsBlur = MeshEffectDesc_bIsBlur;
		m_CurrentMeshEffectDesc.vPositionOffset = { MeshEffectDesc_vPosition[0], MeshEffectDesc_vPosition[1], MeshEffectDesc_vPosition[2] };
		m_CurrentMeshEffectDesc.bIsSpeedEasingMode = !MeshEffectDesc_bSpeedModeDisable;
		m_CurrentMeshEffectDesc.vStartSpeedXYZ = { MeshEffectDesc_vStartSpeedXYZ[0], MeshEffectDesc_vStartSpeedXYZ[1], MeshEffectDesc_vStartSpeedXYZ[2] };
		m_CurrentMeshEffectDesc.iSpeedEasing_End = MeshEffectDesc_iSpeedEasing_End;
		m_CurrentMeshEffectDesc.iSpeedEasing_Middle = MeshEffectDesc_iSpeedEasing_Middle;
		m_CurrentMeshEffectDesc.fMiddleSpeedTimePoint = MeshEffectDesc_fMiddleSpeedTimePoint;
		m_CurrentMeshEffectDesc.vMiddleSpeedXYZ = { MeshEffectDesc_vMiddleSpeed[0], MeshEffectDesc_vMiddleSpeed[1], MeshEffectDesc_vMiddleSpeed[2] };
		m_CurrentMeshEffectDesc.vEndSpeedXYZ = { MeshEffectDesc_vEndSpeed[0], MeshEffectDesc_vEndSpeed[1], MeshEffectDesc_vEndSpeed[2] };
		m_CurrentMeshEffectDesc.fSpeedAccelation = { MeshEffectDesc_fSpeedAccelation[0], MeshEffectDesc_fSpeedAccelation[1], MeshEffectDesc_fSpeedAccelation[2] };
		m_CurrentMeshEffectDesc.vStartScaleXYZ = { MeshEffectDesc_vStartScaleRUL[0], MeshEffectDesc_vStartScaleRUL[1], MeshEffectDesc_vStartScaleRUL[2] };
		m_CurrentMeshEffectDesc.iScaleEasing_End = MeshEffectDesc_iScaleEasing_End;
		m_CurrentMeshEffectDesc.iScaleEasing_Middle = MeshEffectDesc_iScaleEasing_Middle;
		m_CurrentMeshEffectDesc.fMiddleScaleTimePoint = MeshEffectDesc_fMiddleScaleTimePoint;
		m_CurrentMeshEffectDesc.vMiddleScaleXYZ = { MeshEffectDesc_vMiddleScaleRUL[0], MeshEffectDesc_vMiddleScaleRUL[1], MeshEffectDesc_vMiddleScaleRUL[2] };
		m_CurrentMeshEffectDesc.vEndScaleXYZ = { MeshEffectDesc_vEndScaleRUL[0], MeshEffectDesc_vEndScaleRUL[1], MeshEffectDesc_vEndScaleRUL[2] };
		m_CurrentMeshEffectDesc.vRotationRUL = { MeshEffectDesc_vRotationRUL[0], MeshEffectDesc_vRotationRUL[1], MeshEffectDesc_vRotationRUL[2] };
		m_CurrentMeshEffectDesc.vRotationSpeedRUL = { MeshEffectDesc_vRotationSpeedRUL[0], MeshEffectDesc_vRotationSpeedRUL[1], MeshEffectDesc_vRotationSpeedRUL[2] };
		m_CurrentMeshEffectDesc.fRotatationAccelation = { MeshEffectDesc_fRotationAccelation[0], MeshEffectDesc_fRotationAccelation[1], MeshEffectDesc_fRotationAccelation[2] };

		if (ImGui::BeginPopup("Easing Image popup", ImGuiWindowFlags_None))
		{
			if (nullptr != m_pTextureCom_Mask)
			{
				ImVec2 size = ImVec2(900.f, 642.f);
				ImVec2 uv0 = ImVec2(0.0f, 0.0f);
				ImVec2 uv1 = ImVec2(1.f, 1.f);
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint
				ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
				ImGui::Image((void*)m_pTextureCom_Mask->Get_SRV().front(), size, uv0, uv1, tint_col, border_col);
			}
			ImGui::EndPopup();
		}

		ImGui::Separator();
		ImGui::Separator();

		USEGAMEINSTANCE;
		if (ImGui::Button("[Play](Key:'E')", ImVec2(size.x * 0.5f, 0)) || KEY_DOWN('E'))
		{
			if (m_CurrentMeshEffectDesc.iMeshIndex)
			{
				m_pGameInstance->Add_GameObject(m_pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_MESH), ID_UINT(EFFECT, EFFECT_MESH), &m_CurrentMeshEffectDesc);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("[Stop](Key:'Q')", ImVec2(-FLT_MIN, 0)) || KEY_DOWN('Q'))
		{
			list<CGameObject*>* pGameObjects = m_pGameInstance->Get_Objects(m_pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_MESH));
			if (nullptr != pGameObjects)
			{
				for (auto& pGameObject : *pGameObjects)
				{
					m_pGameInstance->Add_DeadObject(pGameObject);
				}
			}
		}
		if (ImGui::Button("[Add to Component]:Mesh_Effect", ImVec2(-FLT_MIN, 0)))
		{
			m_CurrentEffectDesc_Mesh.push_back(m_CurrentMeshEffectDesc);
		}

		ImGui::Separator();
		ImGui::Separator();

		// Mesh_Effect_Components
		ImGui::Text("Mesh_Effect_Components");
		if (ImGui::BeginListBox("##MeshEffect_Component", ImVec2(-FLT_MIN, 3 * ImGui::GetTextLineHeightWithSpacing())))
		{

			for (int n = 0; n < m_CurrentEffectDesc_Mesh.size(); n++)
			{
				string MeshEffects = "MeshEffect" + to_string(n);
				const bool is_selected = (m_iMeshEffect_Component_idx == n);
				if (ImGui::Selectable(MeshEffects.c_str(), is_selected))
					m_iMeshEffect_Component_idx = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndListBox();
		}
		if (ImGui::Button("[Play]:Component", ImVec2(size.x * 0.5f, 0)))
		{
			if (m_iMeshEffect_Component_idx < m_CurrentEffectDesc_Mesh.size())
				m_pGameInstance->Add_GameObject(m_pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_MESH), ID_UINT(EFFECT, EFFECT_MESH), &m_CurrentEffectDesc_Mesh[m_iMeshEffect_Component_idx]);
		}
		ImGui::SameLine();
		if (ImGui::Button("[Stop]:Component", ImVec2(-FLT_MIN, 0)))
		{
			list<CGameObject*>* pGameObjects = m_pGameInstance->Get_Objects(m_pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_MESH));
			if (nullptr != pGameObjects)
			{
				for (auto& pGameObject : *pGameObjects)
				{
					m_pGameInstance->Add_DeadObject(pGameObject);
				}
			}
		}
		if (ImGui::Button("[Load]:Component", ImVec2(size.x * 0.5f, 0)))
		{
			if (0 < m_CurrentEffectDesc_Mesh.size())
			{
				try
				{
					// 예외 검출
					//m_CurrentEffectDesc_Mesh.at(m_iMeshEffect_Component_idx);      // vector::at throws an out-of-range
					MESHEFFECT_DESC MeshEffect = m_CurrentEffectDesc_Mesh.at(m_iMeshEffect_Component_idx);
					MeshEffectDesc_iMeshIndex = MeshEffect.iMeshIndex;
					MeshEffectDesc_iNoiseIndex = MeshEffect.iNoiseIndex;
					MeshEffectDesc_iMaskIndex = MeshEffect.iMaskIndex;
					MeshEffectDesc_bIsBillboard = MeshEffect.bIsBillBoard;
					MeshEffectDesc_fStartTimeOffset = MeshEffect.fLifeTime.x;
					MeshEffectDesc_fLifeDuration = MeshEffect.fLifeTime.y;
					MeshEffectDesc_bIsLoop = MeshEffect.bIsLoop;
					MeshEffectDesc_bIsClampSampling_Noise = MeshEffect.bIsClampSampling_Noise;
					MeshEffectDesc_bIsReverseXY_Noise = MeshEffect.bIsReverseXY_Noise;
					MeshEffectDesc_fUVSpeedXY_Noise[0] = MeshEffect.fUVSpeedXY_Noise.x;
					MeshEffectDesc_fUVSpeedXY_Noise[1] = MeshEffect.fUVSpeedXY_Noise.y;
					MeshEffectDesc_iUVScaleXMax_Noise = MeshEffect.iUVScaleXMax_Noise;
					MeshEffectDesc_iUVScaleYMax_Noise = MeshEffect.iUVScaleYMax_Noise;
					MeshEffectDesc_bIsUVSpeedModeDisabled = !MeshEffect.bIsUVSpeedMode;
					MeshEffect.bIsUVSpeedMode == true ? MESHEFFECTTYPE_type = UVSPEEDMODE : MESHEFFECTTYPE_type = SPRITEMODE;
					MeshEffectDesc_bIsClampSampling_Mask = MeshEffect.bIsClampSampling_Mask;
					MeshEffectDesc_bIsReverseXY_Mask = MeshEffect.bIsReverseXY_Mask;
					MeshEffectDesc_fUVSpeedXY_Mask[0] = MeshEffect.fUVSpeedXY_Mask.x;
					MeshEffectDesc_fUVSpeedXY_Mask[1] = MeshEffect.fUVSpeedXY_Mask.y;
					MeshEffectDesc_iUVScaleXMax_Mask = MeshEffect.iUVScaleXMax_Mask;
					MeshEffectDesc_iUVScaleYMax_Mask = MeshEffect.iUVScaleYMax_Mask;
					MeshEffectDesc_fSpriteSpeed = MeshEffect.fSpriteSpeed;
					MeshEffectDesc_vStartColorDiffuse[0] = MeshEffect.vStartDiffuseColorRGBA.x;
					MeshEffectDesc_vStartColorDiffuse[1] = MeshEffect.vStartDiffuseColorRGBA.y;
					MeshEffectDesc_vStartColorDiffuse[2] = MeshEffect.vStartDiffuseColorRGBA.z;
					MeshEffectDesc_vStartColorDiffuse[3] = MeshEffect.vStartDiffuseColorRGBA.w;
					MeshEffectDesc_iColorEasingDiffuse_End = MeshEffect.iDiffuseColorEasing_End;
					MeshEffectDesc_iColorEasingDiffuse_Middle = MeshEffect.iDiffuseColorEasing_Middle;
					MeshEffectDesc_fMiddleColorDiffuseTimePoint = MeshEffect.fMiddleDiffuseColorTimePoint;
					MeshEffectDesc_vMiddleColorDiffuse[0] = MeshEffect.vMiddleDiffuseColorRGBA.x;
					MeshEffectDesc_vMiddleColorDiffuse[1] = MeshEffect.vMiddleDiffuseColorRGBA.y;
					MeshEffectDesc_vMiddleColorDiffuse[2] = MeshEffect.vMiddleDiffuseColorRGBA.z;
					MeshEffectDesc_vMiddleColorDiffuse[3] = MeshEffect.vMiddleDiffuseColorRGBA.w;
					MeshEffectDesc_vEndColorDiffuse[0] = MeshEffect.vEndDiffuseColorRGBA.x;
					MeshEffectDesc_vEndColorDiffuse[1] = MeshEffect.vEndDiffuseColorRGBA.y;
					MeshEffectDesc_vEndColorDiffuse[2] = MeshEffect.vEndDiffuseColorRGBA.z;
					MeshEffectDesc_vEndColorDiffuse[3] = MeshEffect.vEndDiffuseColorRGBA.w;
					MeshEffectDesc_bIsSoftEffect = MeshEffect.bIsSoftEffect;
					MeshEffectDesc_bIsGlow = MeshEffect.bIsGlow;
					MeshEffectDesc_bIsGlowFollowDiffuseColor = MeshEffect.bIsGlowFollowDiffuseColor;
					MeshEffectDesc_vStartColorGlow[0] = MeshEffect.vStartGlowColorRGBA.x;
					MeshEffectDesc_vStartColorGlow[1] = MeshEffect.vStartGlowColorRGBA.y;
					MeshEffectDesc_vStartColorGlow[2] = MeshEffect.vStartGlowColorRGBA.z;
					MeshEffectDesc_vStartColorGlow[3] = MeshEffect.vStartGlowColorRGBA.w;
					MeshEffectDesc_iColorEasingGlow_End = MeshEffect.iGlowColorEasing_End;
					MeshEffectDesc_iColorEasingGlow_Middle = MeshEffect.iGlowColorEasing_Middle;
					MeshEffectDesc_fMiddleColorGlowTimePoint = MeshEffect.fMiddleGlowColorTimePoint;
					MeshEffectDesc_vMiddleColorGlow[0] = MeshEffect.vMiddleGlowColorRGBA.x;
					MeshEffectDesc_vMiddleColorGlow[1] = MeshEffect.vMiddleGlowColorRGBA.y;
					MeshEffectDesc_vMiddleColorGlow[2] = MeshEffect.vMiddleGlowColorRGBA.z;
					MeshEffectDesc_vMiddleColorGlow[3] = MeshEffect.vMiddleGlowColorRGBA.w;
					MeshEffectDesc_vEndColorGlow[0] = MeshEffect.vEndGlowColorRGBA.x;
					MeshEffectDesc_vEndColorGlow[1] = MeshEffect.vEndGlowColorRGBA.y;
					MeshEffectDesc_vEndColorGlow[2] = MeshEffect.vEndGlowColorRGBA.z;
					MeshEffectDesc_vEndColorGlow[3] = MeshEffect.vEndGlowColorRGBA.w;

					MeshEffectDesc_bIsDistortion = MeshEffect.bIsDistortion;
					MeshEffectDesc_fStartDistortionStrength = MeshEffect.fStartDistortionStrength;
					MeshEffectDesc_iDistortionStrengthEasing_End = MeshEffect.iDistortionStrengthEasing_End;
					MeshEffectDesc_iDistortionStrengthEasing_Middle = MeshEffect.iDistortionStrengthEasing_Middle;
					MeshEffectDesc_fMiddleDistortionStrengthTimePoint = MeshEffect.fMiddleDistortionStrengthTimePoint;
					MeshEffectDesc_fMiddleDistortionStrength = MeshEffect.fMiddleDistortionStrength;
					MeshEffectDesc_fEndDistortionStrength = MeshEffect.fEndDistortionStrength;

					MeshEffectDesc_bIsRimLight = MeshEffect.bIsRimLight;
					MeshEffectDesc_vStartRimLightColorRGBA[0] = MeshEffect.vStartRimLightColorRGBA.x;
					MeshEffectDesc_vStartRimLightColorRGBA[1] = MeshEffect.vStartRimLightColorRGBA.y;
					MeshEffectDesc_vStartRimLightColorRGBA[2] = MeshEffect.vStartRimLightColorRGBA.z;
					MeshEffectDesc_vStartRimLightColorRGBA[3] = MeshEffect.vStartRimLightColorRGBA.w;
					MeshEffectDesc_iRimLightColorEasing_End = MeshEffect.iRimLightColorEasing_End;
					MeshEffectDesc_iRimLightColorEasing_Middle = MeshEffect.iRimLightColorEasing_Middle;
					MeshEffectDesc_fMiddleRimLightColorTimePoint = MeshEffect.fMiddleRimLightColorTimePoint;
					MeshEffectDesc_vMiddleRimLightColorRGBA[0] = MeshEffect.vMiddleRimLightColorRGBA.x;
					MeshEffectDesc_vMiddleRimLightColorRGBA[1] = MeshEffect.vMiddleRimLightColorRGBA.y;
					MeshEffectDesc_vMiddleRimLightColorRGBA[2] = MeshEffect.vMiddleRimLightColorRGBA.z;
					MeshEffectDesc_vMiddleRimLightColorRGBA[3] = MeshEffect.vMiddleRimLightColorRGBA.w;
					MeshEffectDesc_vEndRimLightColorRGBA[0] = MeshEffect.vEndRimLightColorRGBA.x;
					MeshEffectDesc_vEndRimLightColorRGBA[1] = MeshEffect.vEndRimLightColorRGBA.y;
					MeshEffectDesc_vEndRimLightColorRGBA[2] = MeshEffect.vEndRimLightColorRGBA.z;
					MeshEffectDesc_vEndRimLightColorRGBA[3] = MeshEffect.vEndRimLightColorRGBA.w;

					MeshEffectDesc_bIsDissolve = MeshEffect.bIsDissolve;
					MeshEffectDesc_iDissolveNoiseTextureIndex = MeshEffect.iDissolveNoiseTextureIndex;
					MeshEffectDesc_iDissolveColorTextureIndex = MeshEffect.iDissolveColorTextureIndex;
					MeshEffectDesc_fDissolveStrength = MeshEffect.fDissolveStrength;

					MeshEffectDesc_bIsTrail = MeshEffect.bIsTrail;
					MeshEffectDesc_vTrailColor[0] = MeshEffect.vTrailColor.x;
					MeshEffectDesc_vTrailColor[1] = MeshEffect.vTrailColor.y;
					MeshEffectDesc_vTrailColor[2] = MeshEffect.vTrailColor.z;
					MeshEffectDesc_vTrailColor[3] = MeshEffect.vTrailColor.w;
					MeshEffectDesc_fTrailStrengh = MeshEffect.fTrailStrengh;

					MeshEffectDesc_bIsBlur = MeshEffect.bIsBlur;
					MeshEffectDesc_vPosition[0] = MeshEffect.vPositionOffset.x;
					MeshEffectDesc_vPosition[1] = MeshEffect.vPositionOffset.y;
					MeshEffectDesc_vPosition[2] = MeshEffect.vPositionOffset.z;
					MeshEffectDesc_bSpeedModeDisable = !MeshEffect.bIsSpeedEasingMode;
					MeshEffect.bIsSpeedEasingMode == true ? MeshEffect_Moving_type = SPEED_EASING_MODE : MeshEffect_Moving_type = ACCELATION_MODE;
					MeshEffectDesc_vStartSpeedXYZ[0] = MeshEffect.vStartSpeedXYZ.x;
					MeshEffectDesc_vStartSpeedXYZ[1] = MeshEffect.vStartSpeedXYZ.y;
					MeshEffectDesc_vStartSpeedXYZ[2] = MeshEffect.vStartSpeedXYZ.z;
					MeshEffectDesc_iSpeedEasing_End = MeshEffect.iSpeedEasing_End;
					MeshEffectDesc_iSpeedEasing_Middle = MeshEffect.iSpeedEasing_Middle;
					MeshEffectDesc_fMiddleSpeedTimePoint = MeshEffect.fMiddleSpeedTimePoint;
					MeshEffectDesc_vMiddleSpeed[0] = MeshEffect.vMiddleSpeedXYZ.x;
					MeshEffectDesc_vMiddleSpeed[1] = MeshEffect.vMiddleSpeedXYZ.y;
					MeshEffectDesc_vMiddleSpeed[2] = MeshEffect.vMiddleSpeedXYZ.z;
					MeshEffectDesc_vEndSpeed[0] = MeshEffect.vEndSpeedXYZ.x;
					MeshEffectDesc_vEndSpeed[1] = MeshEffect.vEndSpeedXYZ.y;
					MeshEffectDesc_vEndSpeed[2] = MeshEffect.vEndSpeedXYZ.z;
					MeshEffectDesc_fSpeedAccelation[0] = MeshEffect.fSpeedAccelation.x;
					MeshEffectDesc_fSpeedAccelation[1] = MeshEffect.fSpeedAccelation.y;
					MeshEffectDesc_fSpeedAccelation[2] = MeshEffect.fSpeedAccelation.z;
					MeshEffectDesc_vStartScaleRUL[0] = MeshEffect.vStartScaleXYZ.x;
					MeshEffectDesc_vStartScaleRUL[1] = MeshEffect.vStartScaleXYZ.y;
					MeshEffectDesc_vStartScaleRUL[2] = MeshEffect.vStartScaleXYZ.z;
					MeshEffectDesc_iScaleEasing_End = MeshEffect.iScaleEasing_End;
					MeshEffectDesc_iScaleEasing_Middle = MeshEffect.iScaleEasing_Middle;
					MeshEffectDesc_fMiddleScaleTimePoint = MeshEffect.fMiddleScaleTimePoint;
					MeshEffectDesc_vMiddleScaleRUL[0] = MeshEffect.vMiddleScaleXYZ.x;
					MeshEffectDesc_vMiddleScaleRUL[1] = MeshEffect.vMiddleScaleXYZ.y;
					MeshEffectDesc_vMiddleScaleRUL[2] = MeshEffect.vMiddleScaleXYZ.z;
					MeshEffectDesc_vEndScaleRUL[0] = MeshEffect.vEndScaleXYZ.x;
					MeshEffectDesc_vEndScaleRUL[1] = MeshEffect.vEndScaleXYZ.y;
					MeshEffectDesc_vEndScaleRUL[2] = MeshEffect.vEndScaleXYZ.z;
					MeshEffectDesc_vRotationRUL[0] = MeshEffect.vRotationRUL.x;
					MeshEffectDesc_vRotationRUL[1] = MeshEffect.vRotationRUL.y;
					MeshEffectDesc_vRotationRUL[2] = MeshEffect.vRotationRUL.z;
					MeshEffectDesc_vRotationSpeedRUL[0] = MeshEffect.vRotationSpeedRUL.x;
					MeshEffectDesc_vRotationSpeedRUL[1] = MeshEffect.vRotationSpeedRUL.y;
					MeshEffectDesc_vRotationSpeedRUL[2] = MeshEffect.vRotationSpeedRUL.z;
					MeshEffectDesc_fRotationAccelation[0] = MeshEffect.fRotatationAccelation.x;
					MeshEffectDesc_fRotationAccelation[1] = MeshEffect.fRotatationAccelation.y;
					MeshEffectDesc_fRotationAccelation[2] = MeshEffect.fRotatationAccelation.z;
				}
				catch (const std::out_of_range& oor)
				{
					cout << "Out of Range error: " << oor.what() << endl;
					m_iMeshEffect_Component_idx -= 1;
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("[Delete]:Component", ImVec2(-FLT_MIN, 0)))
		{
			if (0 < m_CurrentEffectDesc_Mesh.size())
			{
				try
				{
					// 예외 검출
					m_CurrentEffectDesc_Mesh.at(m_iMeshEffect_Component_idx);      // vector::at throws an out-of-range
					m_CurrentEffectDesc_Mesh.erase(m_CurrentEffectDesc_Mesh.begin() + m_iMeshEffect_Component_idx);
				}
				catch (const std::out_of_range& oor)
				{
					cout << "Out of Range error: " << oor.what() << endl;
					m_iMeshEffect_Component_idx -= 1;
				}
			}
		}

	}
	ImGui::End();
}

void CWindow_Tool_Effect::Show_Window_Particle()
{
	if (ImGui::Begin("EffectTool_Particle", 0/*, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize*/))
	{
		static int ParticleDesc_iMaskIndex = 0;
		static string strTextureNum2 = "";
		strTextureNum2 = "Particle TexNum: " + to_string(ParticleDesc_iMaskIndex);
		ImGui::Text(strTextureNum2.c_str());

		if (ImGui::BeginListBox("##Particles_Textures", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			int i = 0;
			int Index = 0;
			if (nullptr != m_pTextureCom_Mask)
			{
				for (auto& TextureSRV : m_pTextureCom_Mask->Get_SRV())
				{
					int frame_padding = -1;                             // -1 == uses default padding (style.FramePadding)
					ImVec2 size = ImVec2(30.f, 30.f);                     // Size of the image we want to make visible
					ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
					ImVec2 uv1 = ImVec2(1.f, 1.f);							// UV coordinates for (32,32) in our texture
					ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
					ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint
					if (ImGui::ImageButton((void*)TextureSRV, size, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						ParticleDesc_iMaskIndex = Index;
					}

					if (++i < 8)
						ImGui::SameLine();
					else
						i = 0;
					Index++;
				}
			}

			ImGui::EndListBox();
		}
		USEGAMEINSTANCE;
		ImVec2 size = ImGui::GetItemRectSize();
		ImGui::Separator();

		// 인스턴스 갯수.
		static int ParticleDesc_iNumInstance = 10;
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "1. NumInstance");
		ImGui::DragInt("NumInstance", &ParticleDesc_iNumInstance, 1, 1, 10000);

		ImGui::Text("- Select Billboard Mode");
		enum eBillBoardModeType { NORMAL_MODE, BILLBOARD_MODE, DIRECTIONAL_MODE, BILLBOARDTYPE_END };
		static int ParticleDesc_iBillBoardMode = NORMAL_MODE;
		ImGui::RadioButton("Normal_Mode", &ParticleDesc_iBillBoardMode, NORMAL_MODE); ImGui::SameLine();
		ImGui::RadioButton("BillBoard_Mode", &ParticleDesc_iBillBoardMode, BILLBOARD_MODE); ImGui::SameLine();
		ImGui::RadioButton("Directional_Mode", &ParticleDesc_iBillBoardMode, DIRECTIONAL_MODE);

		// Life Time
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "2. Life Time");
		static float  ParticleDesc_fStartTimeOffset = 0.f;
		static float  ParticleDesc_fLifeDuration = 2.f;
		ImGui::Text("- Start Time Offset");
		if (ImGui::Button("Reset Offset", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_fStartTimeOffset = 0.f; }
		ImGui::DragFloat("Offset", &ParticleDesc_fStartTimeOffset, 0.01f, 0.0f, 100.0f, "%.3f", 0);
		ImGui::Text("- Life Time Duration");
		if (ImGui::Button("Reset Duration", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_fLifeDuration = 2.f; }
		ImGui::DragFloat("Duration", &ParticleDesc_fLifeDuration, 0.01f, 0.1f, 100.0f, "%.3f", 0);
		static _bool ParticleDesc_bIsLoop = false;
		ImGui::Checkbox("- Enable Loop", &ParticleDesc_bIsLoop);

		// UV Scale
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "3. UVScaleXY");
		static _uint   ParticleDesc_iUVScaleXCur = 0;
		static _uint   ParticleDesc_iUVScaleYCur = 0;
		static _uint   ParticleDesc_iUVScaleXMax = 1;
		static _uint   ParticleDesc_iUVScaleYMax = 1;
		const _uint    iScalarStep = 1;
		if (ParticleDesc_iUVScaleXMax - 1 < ParticleDesc_iUVScaleXCur) ParticleDesc_iUVScaleXCur = ParticleDesc_iUVScaleXMax - 1;
		if (ParticleDesc_iUVScaleYMax - 1 < ParticleDesc_iUVScaleYCur) ParticleDesc_iUVScaleYCur = ParticleDesc_iUVScaleYMax - 1;
		ImGui::InputScalar("UVScaleXCur", ImGuiDataType_U8, &ParticleDesc_iUVScaleXCur, &iScalarStep, NULL, "%u");
		ImGui::InputScalar("UVScaleYCur", ImGuiDataType_U8, &ParticleDesc_iUVScaleYCur, &iScalarStep, NULL, "%u");
		ImGui::InputScalar("UVScaleXMax", ImGuiDataType_U8, &ParticleDesc_iUVScaleXMax, &iScalarStep, NULL, "%u");
		ImGui::InputScalar("UVScaleYMax", ImGuiDataType_U8, &ParticleDesc_iUVScaleYMax, &iScalarStep, NULL, "%u");
		static _bool ParticleDesc_bIsReverseXY_Mask = false;
		ImGui::Checkbox("- Reverse MaskUV XY", &ParticleDesc_bIsReverseXY_Mask);

		// Diffuse
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "4. Diffuse");
		ImGui::SameLine();
		if (ImGui::Button("Easing Image")) ImGui::OpenPopup("Easing Image popup");

		static float ParticleDesc_vStartColorDiffuse_Min[4] = { 1.f, 1.f, 1.f, 1.f };
		static float ParticleDesc_vStartColorDiffuse_Max[4] = { 1.f, 1.f, 1.f, 1.f };

		static int ParticleDesc_iColorEasingDiffuse_End = CEasing_Utillity::TYPE_End, ParticleDesc_iColorEasingDiffuse_Middle = CEasing_Utillity::TYPE_End;
		const char* combo_ColorEasing_value = szEasingTypes[ParticleDesc_iColorEasingDiffuse_End];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Diffuse Color Easing", combo_ColorEasing_value))
		{
			for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
			{
				const bool is_selected = (ParticleDesc_iColorEasingDiffuse_End == n);
				if (ImGui::Selectable(szEasingTypes[n], is_selected))
					ParticleDesc_iColorEasingDiffuse_End = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (CEasing_Utillity::TYPE_End != ParticleDesc_iColorEasingDiffuse_End)
		{
			const char* combo_ColorEasing_Middle_value = szEasingTypes[ParticleDesc_iColorEasingDiffuse_Middle];  // Pass in the preview value visible before opening the combo (it could be anything)
			if (ImGui::BeginCombo("Diffuse Middle Color Easing", combo_ColorEasing_Middle_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
				{
					const bool is_selected = (ParticleDesc_iColorEasingDiffuse_Middle == n);
					if (ImGui::Selectable(szEasingTypes[n], is_selected))
						ParticleDesc_iColorEasingDiffuse_Middle = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		ImGui::Text("-  Diffuse Start Color (Random)");
		if (ImGui::Button("Reset  Diffuse Start Color", ImVec2(size.x * 0.5f, 0)))
		{
			ParticleDesc_vStartColorDiffuse_Min[0] = { 1.f };
			ParticleDesc_vStartColorDiffuse_Min[1] = { 1.f };
			ParticleDesc_vStartColorDiffuse_Min[2] = { 1.f };
			ParticleDesc_vStartColorDiffuse_Min[3] = { 1.f };
			ParticleDesc_vStartColorDiffuse_Max[0] = { 1.f };
			ParticleDesc_vStartColorDiffuse_Max[1] = { 1.f };
			ParticleDesc_vStartColorDiffuse_Max[2] = { 1.f };
			ParticleDesc_vStartColorDiffuse_Max[3] = { 1.f };
		}
		ImGui::ColorEdit4("Min_StartDiffuseColor", ParticleDesc_vStartColorDiffuse_Min, ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("Max_StartDiffuseColor", ParticleDesc_vStartColorDiffuse_Max, ImGuiColorEditFlags_Float);

		static float ParticleDesc_fMiddleColorDiffuseTimePoint = ParticleDesc_fLifeDuration * 0.5;
		static float ParticleDesc_vMiddleColorDiffuse[4] = { 1.f, 1.f, 1.f, 1.f };
		static float ParticleDesc_vEndColorDiffuse[4] = { 1.f, 1.f, 1.f, 1.f };
		if (CEasing_Utillity::TYPE_End != ParticleDesc_iColorEasingDiffuse_End)
		{
			if (CEasing_Utillity::TYPE_End != ParticleDesc_iColorEasingDiffuse_Middle)
			{
				ImGui::Text("- Middle Diffuse Color");
				if (ImGui::Button("Reset Time Point_g", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_fMiddleColorDiffuseTimePoint = 0.f; }
				ImGui::DragFloat("Time Point_g", &ParticleDesc_fMiddleColorDiffuseTimePoint, 0.01f, 0.01f, ParticleDesc_fLifeDuration, "%.3f", 0);
				if (ImGui::Button("Reset Diffuse Middle Color", ImVec2(size.x * 0.5f, 0)))
				{
					ParticleDesc_vMiddleColorDiffuse[0] = { 1.f };
					ParticleDesc_vMiddleColorDiffuse[1] = { 1.f };
					ParticleDesc_vMiddleColorDiffuse[2] = { 1.f };
					ParticleDesc_vMiddleColorDiffuse[3] = { 1.f };
				}
				ImGui::ColorEdit4("Diffuse Middle Color", ParticleDesc_vMiddleColorDiffuse, ImGuiColorEditFlags_Float);
			}
			ImGui::Text("- Diffuse End Color");
			if (ImGui::Button("Reset Diffuse End Color", ImVec2(size.x * 0.5f, 0)))
			{
				ParticleDesc_vEndColorDiffuse[0] = { 1.f };
				ParticleDesc_vEndColorDiffuse[1] = { 1.f };
				ParticleDesc_vEndColorDiffuse[2] = { 1.f };
				ParticleDesc_vEndColorDiffuse[3] = { 1.f };
			}
			ImGui::ColorEdit4("Diffuse End Color", ParticleDesc_vEndColorDiffuse, ImGuiColorEditFlags_Float);
		}

		// Shader
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "5. Shader");
		static _bool ParticleDesc_bIsGlow = false;
		ImGui::Checkbox("- Enable Glow", &ParticleDesc_bIsGlow);

		static _bool ParticleDesc_bIsGlowFollowDiffuseColor = true;
		static float ParticleDesc_vStartColorGlow_Min[4] = { 1.f, 1.f, 1.f, 1.f };
		static float ParticleDesc_vStartColorGlow_Max[4] = { 1.f, 1.f, 1.f, 1.f };
		static float ParticleDesc_fMiddleColorGlowTimePoint = ParticleDesc_fLifeDuration * 0.5;
		static float ParticleDesc_vMiddleColorGlow[4] = { 1.f, 1.f, 1.f, 1.f };
		static float ParticleDesc_vEndColorGlow[4] = { 1.f, 1.f, 1.f, 1.f };
		static int ParticleDesc_iColorEasingGlow_End = CEasing_Utillity::TYPE_End, ParticleDesc_iColorEasingGlow_Middle = CEasing_Utillity::TYPE_End;
		if (ParticleDesc_bIsGlow)
		{
			// Follow Diffuse Color Mode 
			// 하면 disable
			// 
			// 안하면 
			// 컬러 보간.
			ImGui::Checkbox("-- Follow Diffuse Color", &ParticleDesc_bIsGlowFollowDiffuseColor);

			if (ParticleDesc_bIsGlowFollowDiffuseColor)
				ImGui::BeginDisabled();

			if (ImGui::Button("Easing_Image")) ImGui::OpenPopup("Easing Image popup");


			const char* combo_ColorEasingGlow_value = szEasingTypes[ParticleDesc_iColorEasingGlow_End];  // Pass in the preview value visible before opening the combo (it could be anything)
			if (ImGui::BeginCombo("Glow Color Easing", combo_ColorEasingGlow_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
				{
					const bool is_selected = (ParticleDesc_iColorEasingGlow_End == n);
					if (ImGui::Selectable(szEasingTypes[n], is_selected))
						ParticleDesc_iColorEasingGlow_End = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (CEasing_Utillity::TYPE_End != ParticleDesc_iColorEasingGlow_End)
			{
				const char* combo_ColorEasingGlow_Middle_value = szEasingTypes[ParticleDesc_iColorEasingGlow_Middle];  // Pass in the preview value visible before opening the combo (it could be anything)
				if (ImGui::BeginCombo("Middle Glow Color Easing", combo_ColorEasingGlow_Middle_value))
				{
					for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
					{
						const bool is_selected = (ParticleDesc_iColorEasingGlow_Middle == n);
						if (ImGui::Selectable(szEasingTypes[n], is_selected))
							ParticleDesc_iColorEasingGlow_Middle = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}
			ImGui::Text("-  Glow Start Color (Random)");
			if (ImGui::Button("Reset  Glow Start Color", ImVec2(size.x * 0.5f, 0)))
			{
				ParticleDesc_vStartColorGlow_Min[0] = { 1.f };
				ParticleDesc_vStartColorGlow_Min[1] = { 1.f };
				ParticleDesc_vStartColorGlow_Min[2] = { 1.f };
				ParticleDesc_vStartColorGlow_Min[3] = { 1.f };
				ParticleDesc_vStartColorGlow_Max[0] = { 1.f };
				ParticleDesc_vStartColorGlow_Max[1] = { 1.f };
				ParticleDesc_vStartColorGlow_Max[2] = { 1.f };
				ParticleDesc_vStartColorGlow_Max[3] = { 1.f };
			}
			ImGui::ColorEdit4("Min_StartGlowColor", ParticleDesc_vStartColorGlow_Min, ImGuiColorEditFlags_Float);
			ImGui::ColorEdit4("Max_StartGlowColor", ParticleDesc_vStartColorGlow_Max, ImGuiColorEditFlags_Float);

			if (CEasing_Utillity::TYPE_End != ParticleDesc_iColorEasingGlow_End)
			{
				if (CEasing_Utillity::TYPE_End != ParticleDesc_iColorEasingGlow_Middle)
				{
					ImGui::Text("- Middle Glow Color");
					if (ImGui::Button("Reset Time Point_g ", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_fMiddleColorGlowTimePoint = 0.f; }
					ImGui::DragFloat("Time Point_g ", &ParticleDesc_fMiddleColorGlowTimePoint, 0.01f, 0.01f, ParticleDesc_fLifeDuration, "%.3f", 0);
					if (ImGui::Button("Reset Glow Middle Color", ImVec2(size.x * 0.5f, 0)))
					{
						ParticleDesc_vMiddleColorGlow[0] = { 1.f };
						ParticleDesc_vMiddleColorGlow[1] = { 1.f };
						ParticleDesc_vMiddleColorGlow[2] = { 1.f };
						ParticleDesc_vMiddleColorGlow[3] = { 1.f };
					}
					ImGui::ColorEdit4("Glow Middle Color", ParticleDesc_vMiddleColorGlow, ImGuiColorEditFlags_Float);
				}
				ImGui::Text("- Glow End Color");
				if (ImGui::Button("Reset Glow End Color", ImVec2(size.x * 0.5f, 0)))
				{
					ParticleDesc_vEndColorGlow[0] = { 1.f };
					ParticleDesc_vEndColorGlow[1] = { 1.f };
					ParticleDesc_vEndColorGlow[2] = { 1.f };
					ParticleDesc_vEndColorGlow[3] = { 1.f };
				}
				ImGui::ColorEdit4("Glow End Color", ParticleDesc_vEndColorGlow, ImGuiColorEditFlags_Float);
			}

			if (ParticleDesc_bIsGlowFollowDiffuseColor)
				ImGui::EndDisabled();


		}
		static _bool ParticleDesc_bIsBlur = false;
		ImGui::Checkbox("- Enable Blur", &ParticleDesc_bIsBlur);

		// Mode
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "6. Start Point [Point]/[Cube]/[Sphere]");
		enum eSTARTPOINT_MODE { POINT, CUBE, SPHERE, STARTPOINT_MODE_LAST };
		static int ParticleDesc_iStartPointMode = POINT;
		const char* elems_names[3] = { "Point", "Cube", "Sphere" };
		const char* elem_name = (ParticleDesc_iStartPointMode >= 0 && ParticleDesc_iStartPointMode < STARTPOINT_MODE_LAST) ? elems_names[ParticleDesc_iStartPointMode] : "Unknown";
		ImGui::SliderInt("Start Point Mode", &ParticleDesc_iStartPointMode, 0, STARTPOINT_MODE_LAST - 1, elem_name);
		static float ParticleDesc_vRangeXYZ_Min[3] = { -1.f, -1.f, -1.f };
		static float ParticleDesc_vRangeXYZ_Max[3] = { 1.f, 1.f, 1.f };
		static float ParticleDesc_fRangeRadius = 1.f;
		if (eSTARTPOINT_MODE::CUBE == ParticleDesc_iStartPointMode)
		{
			ImGui::Text("- Range Box Setting");
			if (ImGui::Button("Reset RangeXYZ", ImVec2(size.x * 0.5f, 0)))
			{
				ParticleDesc_vRangeXYZ_Min[0] = -1.f;  ParticleDesc_vRangeXYZ_Min[1] = -1.f;  ParticleDesc_vRangeXYZ_Min[2] = -1.f;
				ParticleDesc_vRangeXYZ_Max[0] = 1.f;  ParticleDesc_vRangeXYZ_Max[1] = 1.f;  ParticleDesc_vRangeXYZ_Max[2] = 1.f;
			}
			ImGui::DragFloat3("RangeXYZ Min", ParticleDesc_vRangeXYZ_Min, 0.01f, -100.0f, 100.0f);
			ImGui::DragFloat3("RangeXYZ Max", ParticleDesc_vRangeXYZ_Max, 0.01f, -100.0f, 100.0f);
		}
		else if (eSTARTPOINT_MODE::SPHERE == ParticleDesc_iStartPointMode)
		{
			ImGui::Text("- Radius Setting");
			if (ImGui::Button("Reset Radius", ImVec2(size.x * 0.5f, 0)))
				ParticleDesc_fRangeRadius = 1.f;
			ImGui::DragFloat("Radius", &ParticleDesc_fRangeRadius, 0.01f, 0.1f, 100.0f);
		}

		// Position Offset
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "7. Position Offset");
		static float ParticleDesc_vPosition[3] = { 0.f, 0.f, 0.f };
		if (ImGui::Button("Reset Position Offset", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_vPosition[0] = 0.f;  ParticleDesc_vPosition[1] = 0.f;  ParticleDesc_vPosition[2] = 0.f; }
		ImGui::DragFloat3("Offset", ParticleDesc_vPosition, 0.01f, -100.0f, 100.0f);

		// Speed
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "8. Speed");
		ImGui::SameLine();
		if (ImGui::Button("Easing Image ")) ImGui::OpenPopup("Easing Image popup");
		static float ParticleDesc_vSpeed_Min[3] = { -10.f, -10.f, -10.f };
		static float ParticleDesc_vSpeed_Max[3] = { 10.f, 10.f, 10.f };
		static int ParticleDesc_iSpeedEasing_End = CEasing_Utillity::TYPE_End, ParticleDesc_iSpeedEasing_Middle = CEasing_Utillity::TYPE_End;

		enum ContentsType { SPEED_EASING_MODE, ACCELATION_MODE, POSITIONING_MODE, CONTENTS_END };
		static int ParticleDesc_iMovingMode = SPEED_EASING_MODE;
		ImGui::RadioButton("Speed Easing Mode", &ParticleDesc_iMovingMode, SPEED_EASING_MODE); ImGui::SameLine();
		ImGui::RadioButton("Accelation Mode", &ParticleDesc_iMovingMode, ACCELATION_MODE);
		ImGui::RadioButton("Positioning Mode", &ParticleDesc_iMovingMode, POSITIONING_MODE);

		const char* combo_SpeedEasing_value = szEasingTypes[ParticleDesc_iSpeedEasing_End];
		const char* combo_SpeedEasing_Middle_value = szEasingTypes[ParticleDesc_iSpeedEasing_Middle];
		if (ParticleDesc_iMovingMode == SPEED_EASING_MODE)
		{
			if (ImGui::BeginCombo("Speed Easing", combo_SpeedEasing_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
				{
					const bool is_selected = (ParticleDesc_iSpeedEasing_End == n);
					if (ImGui::Selectable(szEasingTypes[n], is_selected))
						ParticleDesc_iSpeedEasing_End = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (CEasing_Utillity::TYPE_End != ParticleDesc_iSpeedEasing_End)
			{
				if (ImGui::BeginCombo("Middle Speed Easing", combo_SpeedEasing_Middle_value))
				{
					for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
					{
						const bool is_selected = (ParticleDesc_iSpeedEasing_Middle == n);
						if (ImGui::Selectable(szEasingTypes[n], is_selected))
							ParticleDesc_iSpeedEasing_Middle = n;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}
		}

		ImGui::Text("- Start Speed X,Y,Z Min_Max (Random)");
		if (ImGui::Button("Reset Speed", ImVec2(size.x * 0.5f, 0)))
		{
			ParticleDesc_vSpeed_Min[0] = -10.f;  ParticleDesc_vSpeed_Min[1] = -10.f;  ParticleDesc_vSpeed_Min[2] = -10.f;
			ParticleDesc_vSpeed_Max[0] = 10.f;  ParticleDesc_vSpeed_Max[1] = 10.f;  ParticleDesc_vSpeed_Max[2] = 10.f;
		}
		ImGui::DragFloat3("Min_Speed", ParticleDesc_vSpeed_Min, 0.01f, -100.0f, 100.0f);
		ImGui::DragFloat3("Max_Speed", ParticleDesc_vSpeed_Max, 0.01f, -100.0f, 100.0f);

		static float ParticleDesc_fMiddleSpeedTimePoint = ParticleDesc_fLifeDuration * 0.5;
		static float ParticleDesc_vMiddleSpeed[3] = { 1.f, 1.f, 1.f };
		static float ParticleDesc_vEndSpeed[3] = { 1.f, 1.f, 1.f };

		if (ParticleDesc_iMovingMode == SPEED_EASING_MODE)
		{
			if (CEasing_Utillity::TYPE_End != ParticleDesc_iSpeedEasing_End)
			{
				if (CEasing_Utillity::TYPE_End != ParticleDesc_iSpeedEasing_Middle)
				{
					ImGui::Text("- Middle Speed Ratio");
					if (ImGui::Button("Reset Time Point_sp ", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_fMiddleSpeedTimePoint = 0.f; }
					ImGui::DragFloat("Time Point_sp", &ParticleDesc_fMiddleSpeedTimePoint, 0.01f, 0.01f, ParticleDesc_fLifeDuration, "%.3f", 0);
					if (ImGui::Button("Reset Middle Speed", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_vMiddleSpeed[0] = 0.f;  ParticleDesc_vMiddleSpeed[1] = 0.f;  ParticleDesc_vMiddleSpeed[2] = 0.f; }
					ImGui::DragFloat3("Middle Speed", ParticleDesc_vMiddleSpeed, 0.01f, -100.0f, 100.0f);
				}
				ImGui::Text("- End Speed Ratio");
				if (ImGui::Button("Reset End Speed", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_vEndSpeed[0] = 0.f;  ParticleDesc_vEndSpeed[1] = 0.f;  ParticleDesc_vEndSpeed[2] = 0.f; }
				ImGui::DragFloat3("End Speed", ParticleDesc_vEndSpeed, 0.01f, -100.0f, 100.0f);
			}
		}
		static float  ParticleDesc_fEachAccelation_MinMax[2] = { 0.f, 0.f };
		static float  ParticleDesc_fWholeAccelation_MinMax[3] = { 0.f, 0.f, 0.f };
		if (ParticleDesc_iMovingMode == ACCELATION_MODE)
		{
			// Each Particle Accelation
			ImGui::Separator();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "8-1. Each Particle Accelation");
			ImGui::Text("- Each Accelation (Random)");
			if (ImGui::Button("Reset Each Accelation", ImVec2(size.x * 0.5f, 0)))
			{
				ParticleDesc_fEachAccelation_MinMax[0] = 0.f;
				ParticleDesc_fEachAccelation_MinMax[1] = 0.f;
			}
			ImGui::DragFloat2("Min Each Accelation", ParticleDesc_fEachAccelation_MinMax, 0.01f, -100.0f, 100.0f);

			// Whole Accelation
			ImGui::Separator();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "8-2. Whole Accelation");
			ImGui::Text("- Whole Accelation (Random)");
			if (ImGui::Button("Reset Whole Accelation", ImVec2(size.x * 0.5f, 0)))
			{
				ParticleDesc_fWholeAccelation_MinMax[0] = 0.f;
				ParticleDesc_fWholeAccelation_MinMax[1] = 0.f;
				ParticleDesc_fWholeAccelation_MinMax[2] = 0.f;
			}
			ImGui::DragFloat3("Min_Max Whole Accelation", ParticleDesc_fWholeAccelation_MinMax, 0.01f, -100.0f, 100.0f);
		}
		static float ParticleDesc_vTargetPosition[3] = { 0.f, 0.f, 0.f };
		static bool	 ParticleDesc_bIsGatheringOnce = false;
		static float ParticleDesc_fMiddlePositioningTimePoint = ParticleDesc_fLifeDuration * 0.5;
		static float ParticleDesc_vMiddlePositioning[3] = { 0.f, 0.f, 0.f };
		static float ParticleDesc_vEndPositioning[3] = { 0.f, 0.f, 0.f };
		static int ParticleDesc_iPositioningEasing_End = CEasing_Utillity::TYPE_End, ParticleDesc_iPositioningEasing_Middle = CEasing_Utillity::TYPE_End;
		const char* combo_PositioningEasing_value = szEasingTypes[ParticleDesc_iPositioningEasing_End];
		const char* combo_PositioningEasing_Middle_value = szEasingTypes[ParticleDesc_iPositioningEasing_Middle];

		if (ParticleDesc_iMovingMode == POSITIONING_MODE)
		{
			if (ImGui::Button("Reset TargetPosition", ImVec2(size.x * 0.5f, 0)))
			{
				ParticleDesc_vTargetPosition[0] = 0.f;
				ParticleDesc_vTargetPosition[1] = 0.f;
				ParticleDesc_vTargetPosition[2] = 0.f;
			}
			ImGui::SameLine(); ImGui::Text("!) Use SpeedX for speed ");
			ImGui::DragFloat3("TargetPosition", ParticleDesc_vTargetPosition, 0.01f, -100.0f, 100.0f);

			//ImGui::Checkbox("- Gathering Once", &ParticleDesc_bIsGatheringOnce);

			if (ParticleDesc_bIsGatheringOnce)
			{
				if (ImGui::BeginCombo("Position Easing_po", combo_PositioningEasing_value))
				{
					for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
					{
						const bool is_selected = (ParticleDesc_iPositioningEasing_End == n);
						if (ImGui::Selectable(szEasingTypes[n], is_selected))
							ParticleDesc_iPositioningEasing_End = n;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (CEasing_Utillity::TYPE_End != ParticleDesc_iPositioningEasing_End)
				{
					if (ImGui::BeginCombo("Middle Position Easing_po", combo_PositioningEasing_Middle_value))
					{
						for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
						{
							const bool is_selected = (ParticleDesc_iPositioningEasing_Middle == n);
							if (ImGui::Selectable(szEasingTypes[n], is_selected))
								ParticleDesc_iPositioningEasing_Middle = n;

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
				}

				if (CEasing_Utillity::TYPE_End != ParticleDesc_iPositioningEasing_End)
				{
					if (CEasing_Utillity::TYPE_End != ParticleDesc_iPositioningEasing_Middle)
					{
						ImGui::Text("- Middle Positioning Ratio");
						if (ImGui::Button("Reset Time Point_po ", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_fMiddlePositioningTimePoint = 0.f; }
						ImGui::DragFloat("Time Point_po", &ParticleDesc_fMiddlePositioningTimePoint, 0.01f, 0.01f, ParticleDesc_fLifeDuration, "%.3f", 0);
					}
					ImGui::Text("- End Positioning Ratio");
				}
			}
		}



		// Scale
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "11. Scale");
		ImGui::SameLine();
		if (ImGui::Button(" Easing Image ")) ImGui::OpenPopup("Easing Image popup");
		static float ParticleDesc_vStartScale_Min[3] = { 1.f, 1.f, 1.f };
		static float ParticleDesc_vStartScale_Max[3] = { 1.f, 1.f, 1.f };
		static int ParticleDesc_iScaleEasing_End = CEasing_Utillity::TYPE_End, ParticleDesc_iScaleEasing_Middle = CEasing_Utillity::TYPE_End;
		const char* combo_ScaleEasing_value = szEasingTypes[ParticleDesc_iScaleEasing_End];
		if (ImGui::BeginCombo("Scale Easing", combo_ScaleEasing_value))
		{
			for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
			{
				const bool is_selected = (ParticleDesc_iScaleEasing_End == n);
				if (ImGui::Selectable(szEasingTypes[n], is_selected))
					ParticleDesc_iScaleEasing_End = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (CEasing_Utillity::TYPE_End != ParticleDesc_iScaleEasing_End)
		{
			const char* combo_ScaleEasing_Middle_value = szEasingTypes[ParticleDesc_iScaleEasing_Middle];
			if (ImGui::BeginCombo("Middle Scale Easing Ratio", combo_ScaleEasing_Middle_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(szEasingTypes); n++)
				{
					const bool is_selected = (ParticleDesc_iScaleEasing_Middle == n);
					if (ImGui::Selectable(szEasingTypes[n], is_selected))
						ParticleDesc_iScaleEasing_Middle = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		ImGui::Text("- Start Scale (Random)");
		if (ImGui::Button("Reset StartScale", ImVec2(size.x * 0.5f, 0)))
		{
			ParticleDesc_vStartScale_Min[0] = 1.f;  ParticleDesc_vStartScale_Min[1] = 1.f;  ParticleDesc_vStartScale_Min[2] = 1.f;
			ParticleDesc_vStartScale_Max[0] = 1.f;  ParticleDesc_vStartScale_Max[1] = 1.f;  ParticleDesc_vStartScale_Max[2] = 1.f;
		}
		ImGui::DragFloat3("Min_StartScale", ParticleDesc_vStartScale_Min, 0.01f, 0.01f, 100.0f);
		ImGui::DragFloat3("Max_StartScale", ParticleDesc_vStartScale_Max, 0.01f, 0.01f, 100.0f);

		static float ParticleDesc_fMiddleScaleTimePoint = ParticleDesc_fLifeDuration * 0.5;
		static float ParticleDesc_vMiddleScale[3] = { 1.f, 1.f, 1.f };
		static float ParticleDesc_vEndScale[3] = { 1.f, 1.f, 1.f };
		if (CEasing_Utillity::TYPE_End != ParticleDesc_iScaleEasing_End)
		{
			if (CEasing_Utillity::TYPE_End != ParticleDesc_iScaleEasing_Middle)
			{
				ImGui::Text("- Middle Scale Ratio");
				if (ImGui::Button("Reset Time Point_s ", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_fMiddleScaleTimePoint = 0.f; }
				ImGui::DragFloat("Time Point_s", &ParticleDesc_fMiddleScaleTimePoint, 0.01f, 0.01f, ParticleDesc_fLifeDuration, "%.3f", 0);
				if (ImGui::Button("Reset Middle Scale", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_vMiddleScale[0] = 1.f;  ParticleDesc_vMiddleScale[1] = 1.f;  ParticleDesc_vMiddleScale[2] = 1.f; }
				ImGui::DragFloat3("Middle Scale", ParticleDesc_vMiddleScale, 0.01f, 0.01f, 100.0f);
			}
			ImGui::Text("- End Scale Ratio");
			if (ImGui::Button("Reset End Scale", ImVec2(size.x * 0.5f, 0))) { ParticleDesc_vEndScale[0] = 1.f;  ParticleDesc_vEndScale[1] = 1.f;  ParticleDesc_vEndScale[2] = 1.f; }
			ImGui::DragFloat3("End Scale", ParticleDesc_vEndScale, 0.01f, 0.01f, 100.0f);
		}

		// Rotate // Degree
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.0f, 1.0f), "12. Rotate(Degree / Sec)");
		ImGui::Text("- Turn(Degree) Velocity(Right Up Look)");
		static float ParticleDesc_vRightUpLook_Min[3] = { 0.f, 0.f, 0.f };
		if (ImGui::Button("Reset RUL Min", ImVec2(size.x * 0.5f, 0)))
		{
			ParticleDesc_vRightUpLook_Min[0] = 0.f;  ParticleDesc_vRightUpLook_Min[1] = 0.f;  ParticleDesc_vRightUpLook_Min[2] = 0.f;
		}
		ImGui::DragFloat3("Min RUL", ParticleDesc_vRightUpLook_Min, 0.1f, -720.f, 720.f);
		static float ParticleDesc_vRightUpLook_Max[3] = { 0.f, 0.f, 0.f };
		if (ImGui::Button("Reset RUL Max", ImVec2(size.x * 0.5f, 0)))
		{
			ParticleDesc_vRightUpLook_Max[0] = 0.f;  ParticleDesc_vRightUpLook_Max[1] = 0.f;  ParticleDesc_vRightUpLook_Max[2] = 0.f;
		}
		ImGui::DragFloat3("Max RUL", ParticleDesc_vRightUpLook_Max, 0.1f, -720.f, 720.f);
		ImGui::Text("- Turn(Degree) Accelation (Random)");
		static float  ParticleDesc_fRotationAccelation_Min[3] = { 0.f, 0.f , 0.f };
		static float  ParticleDesc_fRotationAccelation_Max[3] = { 0.f, 0.f , 0.f };
		if (ImGui::Button("Reset Rotation Acc", ImVec2(size.x * 0.5f, 0)))
		{
			ParticleDesc_fRotationAccelation_Min[0] = 0.f;
			ParticleDesc_fRotationAccelation_Min[1] = 0.f;
			ParticleDesc_fRotationAccelation_Min[2] = 0.f;
			ParticleDesc_fRotationAccelation_Max[0] = 0.f;
			ParticleDesc_fRotationAccelation_Max[1] = 0.f;
			ParticleDesc_fRotationAccelation_Max[2] = 0.f;
		}
		ImGui::DragFloat3("Min Rotation Accelation", ParticleDesc_fRotationAccelation_Min, 0.01f, -100.0f, 100.0f);
		ImGui::DragFloat3("Max Rotation Accelation", ParticleDesc_fRotationAccelation_Max, 0.01f, -100.0f, 100.0f);


		if (ImGui::BeginPopup("Easing Image popup", ImGuiWindowFlags_None))
		{
			if (nullptr != m_pTextureCom_Mask)
			{
				ImVec2 size = ImVec2(900.f, 642.f);
				ImVec2 uv0 = ImVec2(0.0f, 0.0f);
				ImVec2 uv1 = ImVec2(1.f, 1.f);
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint
				ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
				ImGui::Image((void*)m_pTextureCom_Mask->Get_SRV().front(), size, uv0, uv1, tint_col, border_col);
			}
			ImGui::EndPopup();
		}


		m_CurrentParticleDesc.iMaskIndex = ParticleDesc_iMaskIndex;
		m_CurrentParticleDesc.iNumInstance = ParticleDesc_iNumInstance;
		m_CurrentParticleDesc.iBillBoardMode = ParticleDesc_iBillBoardMode;
		m_CurrentParticleDesc.fLifeTime = { ParticleDesc_fStartTimeOffset, ParticleDesc_fLifeDuration };
		m_CurrentParticleDesc.bIsLoop = ParticleDesc_bIsLoop;
		m_CurrentParticleDesc.bIsReverseXY_Mask = ParticleDesc_bIsReverseXY_Mask;
		m_CurrentParticleDesc.iUVScaleXCur = ParticleDesc_iUVScaleXCur;
		m_CurrentParticleDesc.iUVScaleYCur = ParticleDesc_iUVScaleYCur;
		m_CurrentParticleDesc.iUVScaleXMax = ParticleDesc_iUVScaleXMax;
		m_CurrentParticleDesc.iUVScaleYMax = ParticleDesc_iUVScaleYMax;

		m_CurrentParticleDesc.vStartDiffuseColorRGBA_Min = { ParticleDesc_vStartColorDiffuse_Min[0], ParticleDesc_vStartColorDiffuse_Min[1], ParticleDesc_vStartColorDiffuse_Min[2], ParticleDesc_vStartColorDiffuse_Min[3] };
		m_CurrentParticleDesc.vStartDiffuseColorRGBA_Max = { ParticleDesc_vStartColorDiffuse_Max[0], ParticleDesc_vStartColorDiffuse_Max[1], ParticleDesc_vStartColorDiffuse_Max[2], ParticleDesc_vStartColorDiffuse_Max[3] };
		m_CurrentParticleDesc.iDiffuseColorEasing_End = ParticleDesc_iColorEasingDiffuse_End;
		m_CurrentParticleDesc.iDiffuseColorEasing_Middle = ParticleDesc_iColorEasingDiffuse_Middle;
		m_CurrentParticleDesc.fMiddleDiffuseColorTimePoint = ParticleDesc_fMiddleColorDiffuseTimePoint;
		m_CurrentParticleDesc.vMiddleDiffuseColorRGBA = { ParticleDesc_vMiddleColorDiffuse[0], ParticleDesc_vMiddleColorDiffuse[1], ParticleDesc_vMiddleColorDiffuse[2], ParticleDesc_vMiddleColorDiffuse[3] };
		m_CurrentParticleDesc.vEndDiffuseColorRGBA = { ParticleDesc_vEndColorDiffuse[0], ParticleDesc_vEndColorDiffuse[1], ParticleDesc_vEndColorDiffuse[2], ParticleDesc_vEndColorDiffuse[3] };

		m_CurrentParticleDesc.bIsGlowFollowDiffuseColor = ParticleDesc_bIsGlowFollowDiffuseColor;
		m_CurrentParticleDesc.vStartGlowColorRGBA_Min = { ParticleDesc_vStartColorGlow_Min[0], ParticleDesc_vStartColorGlow_Min[1], ParticleDesc_vStartColorGlow_Min[2], ParticleDesc_vStartColorGlow_Min[3] };
		m_CurrentParticleDesc.vStartGlowColorRGBA_Max = { ParticleDesc_vStartColorGlow_Max[0], ParticleDesc_vStartColorGlow_Max[1], ParticleDesc_vStartColorGlow_Max[2], ParticleDesc_vStartColorGlow_Max[3] };
		m_CurrentParticleDesc.iGlowColorEasing_End = ParticleDesc_iColorEasingGlow_End;
		m_CurrentParticleDesc.iGlowColorEasing_Middle = ParticleDesc_iColorEasingGlow_Middle;
		m_CurrentParticleDesc.fMiddleGlowColorTimePoint = ParticleDesc_fMiddleColorGlowTimePoint;
		m_CurrentParticleDesc.vMiddleGlowColorRGBA = { ParticleDesc_vMiddleColorGlow[0], ParticleDesc_vMiddleColorGlow[1], ParticleDesc_vMiddleColorGlow[2], ParticleDesc_vMiddleColorGlow[3] };
		m_CurrentParticleDesc.vEndGlowColorRGBA = { ParticleDesc_vEndColorGlow[0], ParticleDesc_vEndColorGlow[1], ParticleDesc_vEndColorGlow[2], ParticleDesc_vEndColorGlow[3] };

		m_CurrentParticleDesc.bIsGlow = ParticleDesc_bIsGlow;
		m_CurrentParticleDesc.bIsBlur = ParticleDesc_bIsBlur;
		m_CurrentParticleDesc.vStartScaleXYZ_Min = { ParticleDesc_vStartScale_Min[0], ParticleDesc_vStartScale_Min[1], ParticleDesc_vStartScale_Min[2] };
		m_CurrentParticleDesc.vStartScaleXYZ_Max = { ParticleDesc_vStartScale_Max[0], ParticleDesc_vStartScale_Max[1], ParticleDesc_vStartScale_Max[2] };
		m_CurrentParticleDesc.iScaleEasing_End = ParticleDesc_iScaleEasing_End;
		m_CurrentParticleDesc.iScaleEasing_Middle = ParticleDesc_iScaleEasing_Middle;
		m_CurrentParticleDesc.fMiddleScaleTimePoint = ParticleDesc_fMiddleScaleTimePoint;
		m_CurrentParticleDesc.vMiddleScaleXYZ = { ParticleDesc_vMiddleScale[0], ParticleDesc_vMiddleScale[1], ParticleDesc_vMiddleScale[2] };
		m_CurrentParticleDesc.vEndScaleXYZ = { ParticleDesc_vEndScale[0], ParticleDesc_vEndScale[1], ParticleDesc_vEndScale[2] };
		m_CurrentParticleDesc.vPositionOffset = { ParticleDesc_vPosition[0], ParticleDesc_vPosition[1], ParticleDesc_vPosition[2] };
		m_CurrentParticleDesc.iStartPointMode = ParticleDesc_iStartPointMode;
		m_CurrentParticleDesc.fRangeRadius = ParticleDesc_fRangeRadius;
		m_CurrentParticleDesc.vRangeXYZ_Min = { ParticleDesc_vRangeXYZ_Min[0], ParticleDesc_vRangeXYZ_Min[1], ParticleDesc_vRangeXYZ_Min[2] };
		m_CurrentParticleDesc.vRangeXYZ_Max = { ParticleDesc_vRangeXYZ_Max[0], ParticleDesc_vRangeXYZ_Max[1], ParticleDesc_vRangeXYZ_Max[2] };
		m_CurrentParticleDesc.iMovingMode = ParticleDesc_iMovingMode;
		m_CurrentParticleDesc.vStartSpeedXYZ_Min = { ParticleDesc_vSpeed_Min[0], ParticleDesc_vSpeed_Min[1], ParticleDesc_vSpeed_Min[2] };
		m_CurrentParticleDesc.vStartSpeedXYZ_Max = { ParticleDesc_vSpeed_Max[0], ParticleDesc_vSpeed_Max[1], ParticleDesc_vSpeed_Max[2] };
		m_CurrentParticleDesc.iSpeedEasing_End = ParticleDesc_iSpeedEasing_End;
		m_CurrentParticleDesc.iSpeedEasing_Middle = ParticleDesc_iSpeedEasing_Middle;
		m_CurrentParticleDesc.fMiddleSpeedTimePoint = ParticleDesc_fMiddleSpeedTimePoint;
		m_CurrentParticleDesc.vMiddleSpeedXYZ = { ParticleDesc_vMiddleSpeed[0], ParticleDesc_vMiddleSpeed[1], ParticleDesc_vMiddleSpeed[2] };
		m_CurrentParticleDesc.vEndSpeedXYZ = { ParticleDesc_vEndSpeed[0], ParticleDesc_vEndSpeed[1], ParticleDesc_vEndSpeed[2] };
		m_CurrentParticleDesc.vTargetPosition = { ParticleDesc_vTargetPosition[0], ParticleDesc_vTargetPosition[1], ParticleDesc_vTargetPosition[2] };
		m_CurrentParticleDesc.bIsGatheringOnce = ParticleDesc_bIsGatheringOnce;
		m_CurrentParticleDesc.iPositioningEasing_End = ParticleDesc_iPositioningEasing_End;
		m_CurrentParticleDesc.iPositioningEasing_Middle = ParticleDesc_iPositioningEasing_Middle;
		m_CurrentParticleDesc.fMiddlePositioningTimePoint = ParticleDesc_fMiddlePositioningTimePoint;
		m_CurrentParticleDesc.fEachAccelation_MinMax = { ParticleDesc_fEachAccelation_MinMax[0], ParticleDesc_fEachAccelation_MinMax[1] };
		m_CurrentParticleDesc.fWholeAccelation = { ParticleDesc_fWholeAccelation_MinMax[0], ParticleDesc_fWholeAccelation_MinMax[1], ParticleDesc_fWholeAccelation_MinMax[2] };
		m_CurrentParticleDesc.vRotateRUL_Min = { ParticleDesc_vRightUpLook_Min[0], ParticleDesc_vRightUpLook_Min[1], ParticleDesc_vRightUpLook_Min[2] };
		m_CurrentParticleDesc.vRotateRUL_Max = { ParticleDesc_vRightUpLook_Max[0], ParticleDesc_vRightUpLook_Max[1], ParticleDesc_vRightUpLook_Max[2] };
		m_CurrentParticleDesc.fRotateAccelation_Min = { ParticleDesc_fRotationAccelation_Min[0], ParticleDesc_fRotationAccelation_Min[1], ParticleDesc_fRotationAccelation_Min[2] };
		m_CurrentParticleDesc.fRotateAccelation_Max = { ParticleDesc_fRotationAccelation_Max[0], ParticleDesc_fRotationAccelation_Max[1], ParticleDesc_fRotationAccelation_Max[2] };

		ImGui::Separator();
		ImGui::Separator();

		if (ImGui::Button("[Play](Key:'C')", ImVec2(size.x * 0.5f, 0)) || KEY_DOWN('C'))
		{
			m_pGameInstance->Add_GameObject(m_pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_PARTICLE_RECT), ID_UINT(EFFECT, EFFECT_PARTICLE_RECT), &m_CurrentParticleDesc);
		}
		ImGui::SameLine();
		if (ImGui::Button("[Stop](Key:'Z')", ImVec2(-FLT_MIN, 0)) || KEY_DOWN('Z'))
		{
			list<CGameObject*>* pGameObjects = m_pGameInstance->Get_Objects(m_pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_PARTICLE_RECT));
			if (nullptr != pGameObjects)
			{
				for (auto& pGameObject : *pGameObjects)
				{
					m_pGameInstance->Add_DeadObject(pGameObject);
				}
			}
		}
		if (ImGui::Button("[Add to Component]", ImVec2(-FLT_MIN, 0)))
		{
			m_CurrentEffectDesc_Particle.push_back(m_CurrentParticleDesc);
		}

		// Particle_Components
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Particle Components");
		if (ImGui::BeginListBox("##Particle_Components", ImVec2(-FLT_MIN, 3 * ImGui::GetTextLineHeightWithSpacing())))
		{

			for (int n = 0; n < m_CurrentEffectDesc_Particle.size(); n++)
			{
				string Particles = "Particles" + to_string(n);
				const bool is_selected = (m_iParticle_Component_idx == n);
				if (ImGui::Selectable(Particles.c_str(), is_selected))
					m_iParticle_Component_idx = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		if (ImGui::Button("[Play]:Component", ImVec2(size.x * 0.5f, 0)))
		{
			if (m_iParticle_Component_idx < m_CurrentEffectDesc_Particle.size())
				m_pGameInstance->Add_GameObject(m_pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_PARTICLE_RECT), ID_UINT(EFFECT, EFFECT_PARTICLE_RECT), &m_CurrentEffectDesc_Particle[m_iParticle_Component_idx]);
		}
		ImGui::SameLine();
		if (ImGui::Button("[Stop]:Component", ImVec2(-FLT_MIN, 0)))
		{
			list<CGameObject*>* pGameObjects = m_pGameInstance->Get_Objects(m_pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_PARTICLE_RECT));
			if (nullptr != pGameObjects)
			{
				for (auto& pGameObject : *pGameObjects)
				{
					m_pGameInstance->Add_DeadObject(pGameObject);
				}
			}
		}
		if (ImGui::Button("[Load]:Component", ImVec2(size.x * 0.5f, 0)))
		{
			if (0 < m_CurrentEffectDesc_Particle.size())
			{
				try
				{
					// 예외 검출
					//m_CurrentEffectDesc_Particle.at(m_iParticle_Component_idx);      // vector::at throws an out-of-range
					PARTICLE_DESC ParticleEffect = m_CurrentEffectDesc_Particle.at(m_iParticle_Component_idx);
					ParticleDesc_iMaskIndex = ParticleEffect.iMaskIndex;
					ParticleDesc_iNumInstance = ParticleEffect.iNumInstance;
					ParticleDesc_iBillBoardMode = ParticleEffect.iBillBoardMode;
					ParticleDesc_fStartTimeOffset = ParticleEffect.fLifeTime.x;
					ParticleDesc_fLifeDuration = ParticleEffect.fLifeTime.y;
					ParticleDesc_bIsLoop = ParticleEffect.bIsLoop;
					ParticleDesc_bIsReverseXY_Mask = ParticleEffect.bIsReverseXY_Mask;
					ParticleDesc_iUVScaleXCur = ParticleEffect.iUVScaleXCur;
					ParticleDesc_iUVScaleYCur = ParticleEffect.iUVScaleYCur;
					ParticleDesc_iUVScaleXMax = ParticleEffect.iUVScaleXMax;
					ParticleDesc_iUVScaleYMax = ParticleEffect.iUVScaleYMax;
					ParticleDesc_vStartColorDiffuse_Min[0] = ParticleEffect.vStartDiffuseColorRGBA_Min.x;
					ParticleDesc_vStartColorDiffuse_Min[1] = ParticleEffect.vStartDiffuseColorRGBA_Min.y;
					ParticleDesc_vStartColorDiffuse_Min[2] = ParticleEffect.vStartDiffuseColorRGBA_Min.z;
					ParticleDesc_vStartColorDiffuse_Min[3] = ParticleEffect.vStartDiffuseColorRGBA_Min.w;
					ParticleDesc_vStartColorDiffuse_Max[0] = ParticleEffect.vStartDiffuseColorRGBA_Max.x;
					ParticleDesc_vStartColorDiffuse_Max[1] = ParticleEffect.vStartDiffuseColorRGBA_Max.y;
					ParticleDesc_vStartColorDiffuse_Max[2] = ParticleEffect.vStartDiffuseColorRGBA_Max.z;
					ParticleDesc_vStartColorDiffuse_Max[3] = ParticleEffect.vStartDiffuseColorRGBA_Max.w;
					ParticleDesc_iColorEasingDiffuse_End = ParticleEffect.iDiffuseColorEasing_End;
					ParticleDesc_iColorEasingDiffuse_Middle = ParticleEffect.iDiffuseColorEasing_Middle;
					ParticleDesc_fMiddleColorDiffuseTimePoint = ParticleEffect.fMiddleDiffuseColorTimePoint;
					ParticleDesc_vMiddleColorDiffuse[0] = ParticleEffect.vMiddleDiffuseColorRGBA.x;
					ParticleDesc_vMiddleColorDiffuse[1] = ParticleEffect.vMiddleDiffuseColorRGBA.y;
					ParticleDesc_vMiddleColorDiffuse[2] = ParticleEffect.vMiddleDiffuseColorRGBA.z;
					ParticleDesc_vMiddleColorDiffuse[3] = ParticleEffect.vMiddleDiffuseColorRGBA.w;
					ParticleDesc_vEndColorDiffuse[0] = ParticleEffect.vEndDiffuseColorRGBA.x;
					ParticleDesc_vEndColorDiffuse[1] = ParticleEffect.vEndDiffuseColorRGBA.y;
					ParticleDesc_vEndColorDiffuse[2] = ParticleEffect.vEndDiffuseColorRGBA.z;
					ParticleDesc_vEndColorDiffuse[3] = ParticleEffect.vEndDiffuseColorRGBA.w;
					ParticleDesc_bIsGlowFollowDiffuseColor = ParticleEffect.bIsGlowFollowDiffuseColor;
					ParticleDesc_vStartColorGlow_Min[0] = ParticleEffect.vStartGlowColorRGBA_Min.x;
					ParticleDesc_vStartColorGlow_Min[1] = ParticleEffect.vStartGlowColorRGBA_Min.y;
					ParticleDesc_vStartColorGlow_Min[2] = ParticleEffect.vStartGlowColorRGBA_Min.z;
					ParticleDesc_vStartColorGlow_Min[3] = ParticleEffect.vStartGlowColorRGBA_Min.w;
					ParticleDesc_vStartColorGlow_Max[0] = ParticleEffect.vStartGlowColorRGBA_Max.x;
					ParticleDesc_vStartColorGlow_Max[1] = ParticleEffect.vStartGlowColorRGBA_Max.y;
					ParticleDesc_vStartColorGlow_Max[2] = ParticleEffect.vStartGlowColorRGBA_Max.z;
					ParticleDesc_vStartColorGlow_Max[3] = ParticleEffect.vStartGlowColorRGBA_Max.w;
					ParticleDesc_iColorEasingGlow_End = ParticleEffect.iGlowColorEasing_End;
					ParticleDesc_iColorEasingGlow_Middle = ParticleEffect.iGlowColorEasing_Middle;
					ParticleDesc_fMiddleColorGlowTimePoint = ParticleEffect.fMiddleGlowColorTimePoint;
					ParticleDesc_vMiddleColorGlow[0] = ParticleEffect.vMiddleGlowColorRGBA.x;
					ParticleDesc_vMiddleColorGlow[1] = ParticleEffect.vMiddleGlowColorRGBA.y;
					ParticleDesc_vMiddleColorGlow[2] = ParticleEffect.vMiddleGlowColorRGBA.z;
					ParticleDesc_vMiddleColorGlow[3] = ParticleEffect.vMiddleGlowColorRGBA.w;
					ParticleDesc_vEndColorGlow[0] = ParticleEffect.vEndGlowColorRGBA.x;
					ParticleDesc_vEndColorGlow[1] = ParticleEffect.vEndGlowColorRGBA.y;
					ParticleDesc_vEndColorGlow[2] = ParticleEffect.vEndGlowColorRGBA.z;
					ParticleDesc_vEndColorGlow[3] = ParticleEffect.vEndGlowColorRGBA.w;
					ParticleDesc_bIsGlow = ParticleEffect.bIsGlow;
					ParticleDesc_bIsBlur = ParticleEffect.bIsBlur;
					ParticleDesc_vStartScale_Min[0] = ParticleEffect.vStartScaleXYZ_Min.x;
					ParticleDesc_vStartScale_Min[1] = ParticleEffect.vStartScaleXYZ_Min.y;
					ParticleDesc_vStartScale_Min[2] = ParticleEffect.vStartScaleXYZ_Min.z;
					ParticleDesc_vStartScale_Max[0] = ParticleEffect.vStartScaleXYZ_Max.x;
					ParticleDesc_vStartScale_Max[1] = ParticleEffect.vStartScaleXYZ_Max.y;
					ParticleDesc_vStartScale_Max[2] = ParticleEffect.vStartScaleXYZ_Max.z;
					ParticleDesc_iScaleEasing_End = ParticleEffect.iScaleEasing_End;
					ParticleDesc_iScaleEasing_Middle = ParticleEffect.iScaleEasing_Middle;
					ParticleDesc_fMiddleScaleTimePoint = ParticleEffect.fMiddleScaleTimePoint;
					ParticleDesc_vMiddleScale[0] = ParticleEffect.vMiddleScaleXYZ.x;
					ParticleDesc_vMiddleScale[1] = ParticleEffect.vMiddleScaleXYZ.y;
					ParticleDesc_vMiddleScale[2] = ParticleEffect.vMiddleScaleXYZ.z;
					ParticleDesc_vEndScale[0] = ParticleEffect.vEndScaleXYZ.x;
					ParticleDesc_vEndScale[1] = ParticleEffect.vEndScaleXYZ.y;
					ParticleDesc_vEndScale[2] = ParticleEffect.vEndScaleXYZ.z;
					ParticleDesc_vPosition[0] = ParticleEffect.vPositionOffset.x;
					ParticleDesc_vPosition[1] = ParticleEffect.vPositionOffset.y;
					ParticleDesc_vPosition[2] = ParticleEffect.vPositionOffset.z;
					ParticleDesc_iStartPointMode = ParticleEffect.iStartPointMode;
					ParticleDesc_vRangeXYZ_Min[0] = ParticleEffect.vRangeXYZ_Min.x;
					ParticleDesc_vRangeXYZ_Min[1] = ParticleEffect.vRangeXYZ_Min.y;
					ParticleDesc_vRangeXYZ_Min[2] = ParticleEffect.vRangeXYZ_Min.z;
					ParticleDesc_vRangeXYZ_Max[0] = ParticleEffect.vRangeXYZ_Max.x;
					ParticleDesc_vRangeXYZ_Max[1] = ParticleEffect.vRangeXYZ_Max.y;
					ParticleDesc_vRangeXYZ_Max[2] = ParticleEffect.vRangeXYZ_Max.z;
					ParticleDesc_fRangeRadius = ParticleEffect.fRangeRadius;
					ParticleDesc_iMovingMode = ParticleEffect.iMovingMode;
					ParticleDesc_vSpeed_Min[0] = ParticleEffect.vStartSpeedXYZ_Min.x;
					ParticleDesc_vSpeed_Min[1] = ParticleEffect.vStartSpeedXYZ_Min.y;
					ParticleDesc_vSpeed_Min[2] = ParticleEffect.vStartSpeedXYZ_Min.z;
					ParticleDesc_vSpeed_Max[0] = ParticleEffect.vStartSpeedXYZ_Max.x;
					ParticleDesc_vSpeed_Max[1] = ParticleEffect.vStartSpeedXYZ_Max.y;
					ParticleDesc_vSpeed_Max[2] = ParticleEffect.vStartSpeedXYZ_Max.z;
					ParticleDesc_iSpeedEasing_End = ParticleEffect.iSpeedEasing_End;
					ParticleDesc_iSpeedEasing_Middle = ParticleEffect.iSpeedEasing_Middle;
					ParticleDesc_fMiddleSpeedTimePoint = ParticleEffect.fMiddleSpeedTimePoint;
					ParticleDesc_vMiddleSpeed[0] = ParticleEffect.vMiddleSpeedXYZ.x;
					ParticleDesc_vMiddleSpeed[1] = ParticleEffect.vMiddleSpeedXYZ.y;
					ParticleDesc_vMiddleSpeed[2] = ParticleEffect.vMiddleSpeedXYZ.z;
					ParticleDesc_vEndSpeed[0] = ParticleEffect.vEndSpeedXYZ.x;
					ParticleDesc_vEndSpeed[1] = ParticleEffect.vEndSpeedXYZ.y;
					ParticleDesc_vEndSpeed[2] = ParticleEffect.vEndSpeedXYZ.z;
					ParticleDesc_vTargetPosition[0] = ParticleEffect.vTargetPosition.x;
					ParticleDesc_vTargetPosition[1] = ParticleEffect.vTargetPosition.y;
					ParticleDesc_vTargetPosition[2] = ParticleEffect.vTargetPosition.z;
					ParticleDesc_bIsGatheringOnce = ParticleEffect.bIsGatheringOnce;
					ParticleDesc_iPositioningEasing_End = ParticleEffect.iPositioningEasing_End;
					ParticleDesc_iPositioningEasing_Middle = ParticleEffect.iPositioningEasing_Middle;
					ParticleDesc_fMiddlePositioningTimePoint = ParticleEffect.fMiddlePositioningTimePoint;

					ParticleDesc_fEachAccelation_MinMax[0] = ParticleEffect.fEachAccelation_MinMax.x;
					ParticleDesc_fEachAccelation_MinMax[1] = ParticleEffect.fEachAccelation_MinMax.y;
					ParticleDesc_fWholeAccelation_MinMax[0] = ParticleEffect.fWholeAccelation.x;
					ParticleDesc_fWholeAccelation_MinMax[1] = ParticleEffect.fWholeAccelation.y;
					ParticleDesc_fWholeAccelation_MinMax[2] = ParticleEffect.fWholeAccelation.z;
					ParticleDesc_vRightUpLook_Min[0] = ParticleEffect.vRotateRUL_Min.x;
					ParticleDesc_vRightUpLook_Min[1] = ParticleEffect.vRotateRUL_Min.y;
					ParticleDesc_vRightUpLook_Min[2] = ParticleEffect.vRotateRUL_Min.z;
					ParticleDesc_vRightUpLook_Max[0] = ParticleEffect.vRotateRUL_Max.x;
					ParticleDesc_vRightUpLook_Max[1] = ParticleEffect.vRotateRUL_Max.y;
					ParticleDesc_vRightUpLook_Max[2] = ParticleEffect.vRotateRUL_Max.z;
					ParticleDesc_fRotationAccelation_Min[0] = ParticleEffect.fRotateAccelation_Min.x;
					ParticleDesc_fRotationAccelation_Min[1] = ParticleEffect.fRotateAccelation_Min.y;
					ParticleDesc_fRotationAccelation_Min[2] = ParticleEffect.fRotateAccelation_Min.z;
					ParticleDesc_fRotationAccelation_Max[0] = ParticleEffect.fRotateAccelation_Max.x;
					ParticleDesc_fRotationAccelation_Max[1] = ParticleEffect.fRotateAccelation_Max.y;
					ParticleDesc_fRotationAccelation_Max[2] = ParticleEffect.fRotateAccelation_Max.z;
				}
				catch (const std::out_of_range& oor)
				{
					cout << "Out of Range error: " << oor.what() << endl;
					m_iParticle_Component_idx -= 1;
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("[Delete]:Component", ImVec2(-FLT_MIN, 0)))
		{
			if (0 < m_CurrentEffectDesc_Particle.size())
			{
				try
				{
					// 예외 검출
					m_CurrentEffectDesc_Particle.at(m_iParticle_Component_idx);      // vector::at throws an out-of-range
					m_CurrentEffectDesc_Particle.erase(m_CurrentEffectDesc_Particle.begin() + m_iParticle_Component_idx);
				}
				catch (const std::out_of_range& oor)
				{
					cout << "Out of Range error: " << oor.what() << endl;
					m_iParticle_Component_idx -= 1;
				}
			}
		}
	}
	ImGui::End();
}

void CWindow_Tool_Effect::Free()
{
	Safe_Release(m_pTextureCom_Mask);
	Safe_Release(m_pTextureCom_Noise);
}

#endif