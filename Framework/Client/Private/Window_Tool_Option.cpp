#include "stdafx.h"
#include "..\Public\Window_Tool_Option.h"

#if defined(USE_IMGUI)

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Imgui_Manager.h"

#include "GameInstance.h"

#include "Level_Loading.h"
#include "Level_GamePlay.h"

#include "Camera.h"

CWindow_Tool_Option::CWindow_Tool_Option()
{
}


CWindow_Tool_Option::~CWindow_Tool_Option()
{
}

void CWindow_Tool_Option::Initialize()
{
	Setup_ShaderName();





	CWindow_Imgui::Initialize_Children();
}

void CWindow_Tool_Option::Tick(_float fTimeDelta)
{
	//static float f = 0.0f;
	//static int counter = 0;
	Show_Window_LevelSelect();
	Show_Window_Shader();
	Show_Window_Debug();
	Show_Window_Camera();


	CWindow_Imgui::Tick_Children(fTimeDelta);
}

void CWindow_Tool_Option::ShowLevelList(_uint* iSelected, _uint iLevelIndex)
{
	USEGAMEINSTANCE;

	switch (iLevelIndex)
	{
	case LEVEL::LEVEL_LOGO:
		if (ImGui::Selectable("Logo", iSelected == &iLevelIndex))
		{
			pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_LOGO));
		}
		break;
		//case LEVEL::LEVEL_MENU:
		//	if (ImGui::Selectable("Menu", iSelected == &iLevelIndex))
		//	{
		//		pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_MENU));
		//	}
		//	break;
	case LEVEL::LEVEL_GAMEPLAY:
		if (ImGui::Selectable("GamePlay", iSelected == &iLevelIndex))
		{
			pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_GAMEPLAY));
		}
		break;

	case LEVEL::LEVEL_EDIT_ANIM:
		if (ImGui::Selectable("Edit_Anim", iSelected == &iLevelIndex))
		{
			pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_EDIT_ANIM));
		}
		break;

	case LEVEL::LEVEL_EDIT_UI:
		if (ImGui::Selectable("Edit_UI", iSelected == &iLevelIndex))
		{
			pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_EDIT_UI));
		}
		break;
	default:
		break;
	}
}

void CWindow_Tool_Option::Show_Window_LevelSelect()
{
	ImGui::Begin("Level Select");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::TreeNode("Level select"))
	{
		static _uint selected = -1;
		for (int iLevelIndex = 0; iLevelIndex < LEVEL_END; ++iLevelIndex)
		{
			ShowLevelList(&selected, iLevelIndex);
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void CWindow_Tool_Option::Show_Window_Shader()
{
	if (ImGui::Begin("Shader"))                          // Create a window called "Hello, world!" and append into it.
	{
		Show_Window_ShaderBloom();
		Show_Window_ShaderBlur();
		Show_Window_ShaderGlow();
		Show_Window_ShaderShadow();
		Show_Window_ShaderSaturation();
		Show_Window_ShaderBrightness();

	}
	ImGui::End();
}

void CWindow_Tool_Option::Show_Window_Debug()
{
	if (ImGui::Begin("Debug"))                          // Create a window called "Hello, world!" and append into it.
	{
		ImGui::Text("debug vector");
		ImGui::DragFloat("float", &m_fDebugfloat, 0.005f);

		//ImGui::NewLine();

		for (size_t i = 0; i < m_vDebugfloat4.size(); ++i)
		{
			ImGui::NewLine();
			string	strText = "debug vector index" + to_string(i);
			string	strTextX = "X" + to_string(i);
			string	strTextY = "Y" + to_string(i);
			string	strTextZ = "Z" + to_string(i);
			string	strTextW = "W" + to_string(i);
			ImGui::Text(strText.c_str());
			ImGui::DragFloat(strTextX.c_str(), &m_vDebugfloat4[i].x, 0.005f);
			ImGui::DragFloat(strTextY.c_str(), &m_vDebugfloat4[i].y, 0.005f);
			ImGui::DragFloat(strTextZ.c_str(), &m_vDebugfloat4[i].z, 0.005f);
			ImGui::DragFloat(strTextW.c_str(), &m_vDebugfloat4[i].w, 0.005f);
		}

		if (ImGui::Button("Add Debug vector"))
		{
			m_vDebugfloat4.push_back(_float4(0.f, 0.f, 0.f, 0.f));
		}

		//ImGui::Text("debug vector index 0");
		//ImGui::DragFloat("X0", &m_vDebugfloat4[0].x, 0.005f);
		//ImGui::DragFloat("Y0", &m_vDebugfloat4[0].y, 0.005f);
		//ImGui::DragFloat("Z0", &m_vDebugfloat4[0].z, 0.005f);
		//ImGui::DragFloat("W0", &m_vDebugfloat4[0].w, 0.005f);

		//ImGui::Text("debug vector index 1");
		//ImGui::DragFloat("X1", &m_vDebugfloat4[1].x, 0.005f);
		//ImGui::DragFloat("Y1", &m_vDebugfloat4[1].y, 0.005f);
		//ImGui::DragFloat("Z1", &m_vDebugfloat4[1].z, 0.005f);
		//ImGui::DragFloat("W1", &m_vDebugfloat4[1].w, 0.005f);

		//ImGui::Text("debug vector index 2");
		//ImGui::DragFloat("X2", &m_vDebugfloat4[2].x, 0.005f);
		//ImGui::DragFloat("Y2", &m_vDebugfloat4[2].y, 0.005f);
		//ImGui::DragFloat("Z2", &m_vDebugfloat4[2].z, 0.005f);
		//ImGui::DragFloat("W2", &m_vDebugfloat4[2].w, 0.005f);

		//ImGui::Text("debug vector index 3");
		//ImGui::DragFloat("X3", &m_vDebugfloat4[3].x, 0.005f);
		//ImGui::DragFloat("Y3", &m_vDebugfloat4[3].y, 0.005f);
		//ImGui::DragFloat("Z3", &m_vDebugfloat4[3].z, 0.005f);
		//ImGui::DragFloat("W3", &m_vDebugfloat4[3].w, 0.005f);

	}
	ImGui::End();
}

void CWindow_Tool_Option::Show_Window_Camera()
{
	if (ImGui::Begin("Camera"))                          // Create a window called "Hello, world!" and append into it.
	{
		USEGAMEINSTANCE;

		_float4 vCamPos = pGameInstance->Get_CamPosition();
		CCamera* pCurCamera = pGameInstance->Get_CurrentCamera();
		_float3 vCamOffset = pCurCamera->Get_Offset();
		_float3 vLook;
		XMStoreFloat3(&vLook,((CTransform*)pCurCamera->Get_Component(ID_UINT(TRANSFORM)))->Get_State(CTransform::STATE_LOOK));
		CCamera::CAMERADESC CamDesc = pCurCamera->Get_CameraDesc();
		ImGui::Text("Current Camera World Pos : X:%.2f  Y:%.2f  Z:%.2f  ", vCamPos.x, vCamPos.y, vCamPos.z);
		ImGui::Text("Current Camera World Look : X:%.2f  Y:%.2f  Z:%.2f  ", vLook.x, vLook.y, vLook.z);
		ImGui::Text("Current Camera Target LookAt : X:%.2f  Y:%.2f  Z:%.2f  ", CamDesc.vAt.x, CamDesc.vAt.y, CamDesc.vAt.z);
		ImGui::Text("Current Camera Offset Pos : X:%.2f  Y:%.2f  Z:%.2f  ", vCamOffset.x, vCamOffset.y, vCamOffset.z);


		ImGui::RadioButton("Target", &m_iCameraType, CAMERA_FIXED);
		ImGui::RadioButton("Player", &m_iCameraType, CAMERA_TARGET_POSITION);
		//ImGui::RadioButton("Free", &m_iCameraType, CAMERA_FREE);


		CLevel* pCurLevel = pGameInstance->Get_CurLevel();

		if (pCurLevel)
		{
			vector<CCamera*>* pLevelGameplayCamera = pCurLevel->Get_FreeCameras();

			if (nullptr == pLevelGameplayCamera)
			{
				ImGui::End();
				return;
			}


			for (_int i = 0; i < pLevelGameplayCamera->size(); ++i)
			{
				string	strCameraType = "Free_";
				char	szFreeCameraIdx[MAX_PATH];
				_itoa_s(i, szFreeCameraIdx, MAX_PATH, 10);
				strCameraType = strCameraType + szFreeCameraIdx;
				ImGui::RadioButton(strCameraType.c_str(), &m_iCameraType, CAMERA_END + i + 1);

			}

			if (pLevelGameplayCamera->size() >= m_iCameraType - (CAMERA_END + 1))
			{
				pGameInstance->Set_CurrentCamera((*pLevelGameplayCamera)[m_iCameraType - (CAMERA_END + 1)]);
			}

			if (ImGui::Button("Add Free Camera"))
			{


				pCurLevel->Add_FreeCamera();
			}
		}


	}
	ImGui::End();
}

void CWindow_Tool_Option::Setup_ShaderName()
{
	m_ShaderName[SHADER_BLUR] = "Blur";
	m_ShaderName[SHADER_BLOOM] = "Bloom";
	m_ShaderName[SHADER_GLOW] = "Glow";
	m_ShaderName[SHADER_SHADOW] = "Shadow";
	m_ShaderName[SHADER_SATURATION] = "Saturation";
	m_ShaderName[SHADER_BRIGHTNESS] = "Brightness";
}



void CWindow_Tool_Option::Show_Window_ShaderBloom()
{
	if (ImGui::Checkbox(m_ShaderName[SHADER_BLOOM].c_str(), &m_ShaderSelectState[SHADER_BLOOM]))
		//if (ImGui::CollapsingHeader(m_ShaderName[SHADER_BLOOM].c_str(), &m_ShaderSelectState[SHADER_BLOOM], ImGuiTreeNodeFlags_Selected))
	{
		//m_fBloomPower;
	}
}

void CWindow_Tool_Option::Show_Window_ShaderBlur()
{
	ImGui::Checkbox(m_ShaderName[SHADER_BLUR].c_str(), &m_ShaderSelectState[SHADER_BLUR]);
	if (ImGui::CollapsingHeader(m_ShaderName[SHADER_BLUR].c_str(), &m_ShaderSelectState[SHADER_BLUR]))
	{
		;
	}

}

void CWindow_Tool_Option::Show_Window_ShaderGlow()
{
	ImGui::Checkbox(m_ShaderName[SHADER_GLOW].c_str(), &m_ShaderSelectState[SHADER_GLOW]);
	if (ImGui::CollapsingHeader(m_ShaderName[SHADER_GLOW].c_str(), &m_ShaderSelectState[SHADER_GLOW]))
	{

	}
}

void CWindow_Tool_Option::Show_Window_ShaderShadow()
{
	ImGui::Checkbox(m_ShaderName[SHADER_SHADOW].c_str(), &m_ShaderSelectState[SHADER_SHADOW]);
	if (ImGui::CollapsingHeader(m_ShaderName[SHADER_SHADOW].c_str(), &m_ShaderSelectState[SHADER_SHADOW]))
	{

	}
}

void CWindow_Tool_Option::Show_Window_ShaderSaturation()
{
	ImGui::Checkbox(m_ShaderName[SHADER_SATURATION].c_str(), &m_ShaderSelectState[SHADER_SATURATION]);
	if (ImGui::CollapsingHeader(m_ShaderName[SHADER_SATURATION].c_str(), &m_ShaderSelectState[SHADER_SATURATION]))
	{

	}
}

void CWindow_Tool_Option::Show_Window_ShaderBrightness()
{
	ImGui::Checkbox(m_ShaderName[SHADER_BRIGHTNESS].c_str(), &m_ShaderSelectState[SHADER_BRIGHTNESS]);
	if (ImGui::CollapsingHeader(m_ShaderName[SHADER_BRIGHTNESS].c_str(), &m_ShaderSelectState[SHADER_BRIGHTNESS]))
	{

	}
}

void CWindow_Tool_Option::Free()
{

}

#endif