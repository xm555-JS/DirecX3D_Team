#include "stdafx.h"
#include "Window_Tool_Map.h"

#if defined(USE_IMGUI)

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Imgui_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MeshContainer.h"
#include "VIBuffer.h"

#include "Navigation.h"
#include "Component.h"

#include "MapObj_Instance.h"

#include "DebugDraw.h"
#include "Cell.h"
#include "Shader.h"

#include "Enemy.h"

#define JSONPATH_MAP			"../Bin/Resources/Json/Map/"
#define JSONPATH_NAVIGATION		"../Bin/Resources/Json/Navigation/"
#define JSONPATH_DYNAMIC_MODEL	"../Bin/Resources/Json/Enemy/"

CWindow_Tool_Map::CWindow_Tool_Map()
{
	ZeroMemory(&io, sizeof(ImGuiIO));
}


CWindow_Tool_Map::~CWindow_Tool_Map()
{
}

void CWindow_Tool_Map::Initialize()
{
	CWindow_Imgui::Initialize_Children();
	USEGAMEINSTANCE;

	m_pDevice = DEVICE.Get();
	m_pContext = CONTEXT.Get();

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_TerrainDesc.fSizeX = 0.f;
	m_TerrainDesc.fSizeZ = 0.f;
	m_TerrainDesc.iNumVerticesX = 0;
	m_TerrainDesc.iNumVerticesZ = 0;
	m_TerrainDesc.pHeightMapFilePath = nullptr;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	FAILEDCHECK(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout));

	m_pShaderCell = CShader::Create(TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS_DECLARATION::Element, VTXPOS_DECLARATION::iNumElements);
	NULLCHECK(m_pShaderCell);


	GAMEINSTANCE->Add_CallbackList(this, LEVEL_CHANGE); // 레벨 바뀔 때 마다 Prototype 새로 가져오기.
	m_iSelectModel = 0;
}

void CWindow_Tool_Map::On_EventMessege(EVENTDESC tEvent)
{
	if (LEVEL_CHANGE == tEvent.eEventType)
	{
		GAMEINSTANCE->Get_Prototypes(ID_UINT(MAP), &m_pPrototypes);
		GAMEINSTANCE->Get_Prototypes(ID_UINT(ENEMY, ENEMY_NORMAL), &m_pEnemyPrototypes, 2);
		GAMEINSTANCE->Get_Prototypes(ID_UINT(ENEMY, ENEMY_BOSS), &m_pBossPrototypes, 2);
	}
}

void CWindow_Tool_Map::Render(void)
{
	// 점 Render
	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->SetWorld(XMMatrixIdentity());

	USEGAMEINSTANCE;
	m_pEffect->SetView(pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ));


	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	if (m_bColRender)
	{
		for (auto& Prototype : m_pPrototypes)
		{
			list<CGameObject*>* plistObject = nullptr;
			ID_UINT	ObjectID;
			ObjectID.iID = Prototype.first;
			plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID);

			if (nullptr == plistObject)
				continue;

			for (auto& pObject : *plistObject)
			{
				_float4 vPos;

				XMStoreFloat4(&vPos, pObject->Get_Pos());

				_float fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat4(&pGameInstance->Get_CamPosition()) - XMLoadFloat4(&vPos)));
				if (30.f > fDistance)
				{
					if (ObjectID == ID_UINT(MAP, MAP_GRASS_DESERT, MODEL_GRASS_DESERT)
						|| ObjectID == ID_UINT(MAP, MAP_GRASS, MODEL_GRASS)
						|| ObjectID == ID_UINT(MAP, MAP_TREE, MODEL_TREE))
					{
						BoundingSphere ColSphere(_float3(vPos.x, vPos.y, vPos.z), 0.2f);
						DX::Draw(m_pBatch, ColSphere);
					}

				}
			}
		}
	}

	if (m_bCellCenterRender)
	{
		for (auto& pCell : m_Cells)
		{
			if (nullptr == pCell)
				continue;

			_float3 vCenter = pCell->Get_Center();

			_float fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat4(&pGameInstance->Get_CamPosition()) - XMLoadFloat3(&vCenter)));
			if (30.f > fDistance)
			{
				BoundingSphere pColSphere(vCenter, 0.2f);

				if (CELL_NORMAL == pCell->Get_Property())
					DX::Draw(m_pBatch, pColSphere, DirectX::Colors::Green);
				else if (CELL_NOMOVE == pCell->Get_Property())
					DX::Draw(m_pBatch, pColSphere, DirectX::Colors::Red);
				else
					DX::Draw(m_pBatch, pColSphere);
			}
		}
	}

	if (m_bCellPointRender)
	{
		for (auto& iter : m_vecPoints)
		{
			_float fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat4(&pGameInstance->Get_CamPosition()) - XMLoadFloat3(&iter.Point)));
			if (30.f > fDistance)
			{
				BoundingSphere pColSphere(iter.Point, 0.2f);
				DX::Draw(m_pBatch, pColSphere, iter.Color);
			}
		}
	}

	m_pBatch->End();


	// 셀 Render
	NULLCHECK(m_pShaderCell);

	_float4x4		WorldMatrix;

	m_pShaderCell->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCell->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));


	// 초록색으로 표시
	{
		XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

		m_pShaderCell->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
		_float4 vTemp = { 0.f, 1.f, 0.f, 1.f };
		m_pShaderCell->Set_RawValue("g_vColor", /*&DirectX::Colors::Green,*/ &vTemp, sizeof(_float4));
		m_pShaderCell->Begin(0);

		for (auto& pCell : m_Cells)
		{
			if (nullptr != pCell)
				pCell->Render_Able(pGameInstance->Get_CamPosition());
		}
	}

}

void CWindow_Tool_Map::Tick(_float fTimeDelta)
{
	io = ImGui::GetIO();

	//ImGui::SetNextWindowPos(ImVec2{ 0,0 });
	//ImGui::SetNextWindowSize(ImVec2{ g_iWinCX * 0.25f, g_iWinCY });
	ImGui::Begin("Map Tool");

	if (ImGui::BeginTabBar("##Map_Tab", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Object"))
		{
			m_eTabState = TAB_STATE::OBJECT;

			Save_Load_Data(); ImGui::Separator();
			Show_ObjectPrototypes(fTimeDelta);
			Picking_Setting(fTimeDelta);
			Create_Delete_Setting(); ImGui::Separator();
			Show_CreatedObjects(fTimeDelta);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Navigation"))
		{
			m_eTabState = TAB_STATE::NAVIGATION;

			Navigation_Setting();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Dynamic_Model"))
		{
			m_eTabState = TAB_STATE::DYNAMIC_MODEL;

			Dynamic_Model_Setting();
			Dynamic_Model_Create_Delete_Setting();
			Show_Created_Dynamic_Objects(fTimeDelta);

			ImGui::EndTabItem();
		}
		//if (ImGui::BeginTabItem("Terrain"))
		//{
		//	m_eTabState = TAB_STATE::TERRAIN;
		//
		//	Terrain_Setting();
		//	Terrain_Create_Delete_Setting();
		//
		//	ImGui::EndTabItem();
		//}



		ImGui::EndTabBar();
	}
	ImGui::End();

	CWindow_Imgui::Tick_Children(fTimeDelta);

	//Render();
}

void CWindow_Tool_Map::Show_ObjectPrototypes(_float fTimeDelta)
{
	ImGui::Text("Objects Prototypes List");

	if (ImGui::BeginListBox("##Object_Prototypes", ImVec2(-FLT_MIN, 4 * ImGui::GetTextLineHeightWithSpacing())))
	{
		int n = 1;
		for (auto& Prototype : m_pPrototypes)
		{
			const bool is_selected = (m_iObject_idx == n);
			if (ImGui::Selectable(Get_StaticObjectIDStr(Prototype.first).c_str(), is_selected))
				m_iObject_idx = n;

			if (is_selected)
			{
				m_iSelectedObjectPrototype = Prototype;
				ImGui::SetItemDefaultFocus();
			}
			n += 1;
		}

		ImGui::EndListBox();
	}

	ImGui::Text("Selected Object Models List");
	if (ImGui::BeginListBox("##Models", ImVec2(-FLT_MIN, 6 * ImGui::GetTextLineHeightWithSpacing())))
	{
		map<_uint, CComponent*> pModelPrototypes;
		ID_UINT ID;
		ID.iID = m_iSelectedObjectPrototype.first;
		FAILEDCHECK(GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_STATIC, ID.tNums.Num2), &pModelPrototypes, 2));

		int n = 1;
		for (auto& Prototype : pModelPrototypes)
		{
			const bool is_selected = (m_iModel_idx == n);
			if (ImGui::Selectable(Prototype.second->Get_ModelFileName().c_str(), is_selected))
				m_iModel_idx = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
			n += 1;
		}

		ImGui::EndListBox();
	}
}

void CWindow_Tool_Map::Show_CreatedObjects(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	int iObjectNum = 0;
	list<CGameObject*>* plistObjects = nullptr;
	ID_UINT	ObjectID;
	ObjectID.iID = m_iSelectedObjectPrototype.first;

	// 터레인은 불러올 필요 x
	if (ObjectID == ID_UINT(MAP, MAP_TERRAIN)
		|| ObjectID == ID_UINT(MAP, MAP_WATER)
		|| ObjectID == ID_UINT(MAP, MAP_TRIGGER)
		|| ObjectID == ID_UINT(MAP, MAP_WATER_BASE)
		|| ObjectID == ID_UINT(MAP, MAP_PORTAL)
		|| ObjectID == ID_UINT(MAP, MAP_REGION))
		return;

	plistObjects = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID); // 프로로타입 ID 를 가진 Layer들에 접근.

	if (nullptr == plistObjects)
		return;

	//ImGui::DragFloat("Rotate", &m_fObjectRotate, 0.5f, -360.f, 360.f);
	//ImGui::DragFloat3("Size", m_fObjectScale, 0.01f, 0.001f, 2000.f);

	int iNum = 0;
	string LayerIDStr;
	for (auto& pObject : *plistObjects)
	{
		LayerIDStr = pObject->Get_Component(ID_UINT(MODEL_STATIC))->Get_ModelFileName() + "_" + to_string(iNum++);

		ImGui::SetNextItemOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode((void*)(intptr_t)iObjectNum++, LayerIDStr.c_str()))
		{
			float fObjectPos[3] = { 0.f, 0.f, 0.f };
			float fObjectScale[3] = { 0.f, 0.f, 0.f };
			float fYTurn = 0.f;
			CTransform* pTransformCom = pObject->Get_Transform();

			_float3 vPos;
			XMStoreFloat3(&vPos, pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
			//ImGui::Text("Pos.x: %f, Pos.y: %2f, Pos.z: %f", vPos.x, vPos.y, vPos.z);
			fObjectPos[0] = vPos.x;
			fObjectPos[1] = vPos.y;
			fObjectPos[2] = vPos.z;

			m_fObjectScale[0] = pTransformCom->Get_Scaled().x;
			m_fObjectScale[1] = pTransformCom->Get_Scaled().y;
			m_fObjectScale[2] = pTransformCom->Get_Scaled().z;

			ImGui::DragFloat3("Position", fObjectPos, 0.01f);
			ImGui::DragFloat3("Size", m_fObjectScale, 0.01f, 0.001f, 2000.f);

			pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&XMFLOAT3(fObjectPos[0], fObjectPos[1], fObjectPos[2])), 1.f));
			pTransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));

			if (KEY_PRESSING('Q'))
			{
				if (m_iTurnMode == X_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTransformCom->Turn(pTransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(0.5f));
				}
				else if (m_iTurnMode == Y_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTransformCom->Turn(pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(0.5f));
				}
				else if (m_iTurnMode == Z_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTransformCom->Turn(pTransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(0.5f));
				}
			}
			if (KEY_PRESSING('E'))
			{
				if (m_iTurnMode == X_TURN)
				{
					m_fObjectTurnAngle -= 3.f;
					pTransformCom->Turn(pTransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(-0.5f));
				}
				else if (m_iTurnMode == Y_TURN)
				{
					m_fObjectTurnAngle -= 3.f;
					pTransformCom->Turn(pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-0.5f));
				}
				else if (m_iTurnMode == Z_TURN)
				{
					m_fObjectTurnAngle -= 3.f;
					pTransformCom->Turn(pTransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(-0.5f));
				}
			}
			//ImGui::DragFloat("Rotate", &m_fObjectRotate, 0.5f, -360.f, 360.f);
			//pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(m_fObjectRotate));
			//pTransformCom->Rotation(XMVectorSet(XMConvertToRadians(m_fObjectRotate[0]), XMConvertToRadians(m_fObjectRotate[1]), XMConvertToRadians(m_fObjectRotate[2]), 0.f));

			if (ImGui::Button("Delete"))
				pGameInstance->Add_DeadObject(pObject);

			ImGui::TreePop();
		}

		if (KEY_DOWN(VK_NUMPAD7))
		{
			CGameObject* pObject = plistObjects->back();
			pGameInstance->Add_DeadObject(pObject);
		}

		if (KEY_PRESSING(VK_NUMPAD8))
		{
			CGameObject* pObject = plistObjects->back();
			pGameInstance->Add_DeadObject(pObject);
		}

		if (KEY_TOGGLE(VK_NUMPAD9))
		{
			CGameObject* pObject = plistObjects->back();
			pGameInstance->Add_DeadObject(pObject);
		}

		if (KEY_DOWN(VK_NUMPAD4))
		{
			CGameObject* pObject = plistObjects->front();
			pGameInstance->Add_DeadObject(pObject);
		}

		if (KEY_PRESSING(VK_NUMPAD5))
		{
			CGameObject* pObject = plistObjects->front();
			pGameInstance->Add_DeadObject(pObject);
		}

		if (KEY_TOGGLE(VK_NUMPAD6))
		{
			CGameObject* pObject = plistObjects->front();
			pGameInstance->Add_DeadObject(pObject);
		}
	}
}

void CWindow_Tool_Map::Save_Load_Data()
{
	ImGui::Separator();
	ImVec2 size = ImGui::GetItemRectSize();
	if (ImGui::Button("Save", ImVec2(size.x * 0.5f, 0)))
	{
		// Tab 별로 분류하기.
		ImGui::OpenPopup("Save?");
	}
	Show_Save_Window();

	ImGui::SameLine();
	if (ImGui::Button("Load", ImVec2(-FLT_MIN, 0)))
	{
		ImGui::OpenPopup("Load?");
	}
	Show_Load_Window();
}

void CWindow_Tool_Map::Show_Save_Window()
{
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("File Name : ");
		switch (m_eTabState)
		{
		case TAB_STATE::OBJECT:
			ImGui::InputText(".json", m_strFileName, IM_ARRAYSIZE(m_strFileName));

			break;
		case TAB_STATE::NAVIGATION:
			ImGui::InputText(".bin", m_strFileName, IM_ARRAYSIZE(m_strFileName));
			break;
		case TAB_STATE::DYNAMIC_MODEL:
			ImGui::InputText(".json", m_strFileName, IM_ARRAYSIZE(m_strFileName));
			break;
		case TAB_STATE::TERRAIN:
			ImGui::InputText(".bin", m_strFileName, IM_ARRAYSIZE(m_strFileName));
			break;
		default:
			break;
		}
		ImGui::Separator();

		static bool dont_ask_me_next_time = false;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PopStyleVar();

		if (ImGui::Button("Save", ImVec2(120, 0)))
		{
			switch (m_eTabState)
			{
			case TAB_STATE::OBJECT:
				Save_ObjectData();
				break;
			case TAB_STATE::NAVIGATION:
				Save_NavigationData();
				break;
			case TAB_STATE::DYNAMIC_MODEL:
				Save_Dynamic_ModelData();
				break;
			case TAB_STATE::TERRAIN:
				Save_Terrain_Data();
				break;
			default:
				break;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}

void CWindow_Tool_Map::Show_Load_Window()
{
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Load?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		stringvec vecFiles;
		_uint iSubstrCharNumbers = 0;
		switch (m_eTabState)
		{
		case TAB_STATE::OBJECT:
			read_directory(JSONPATH_MAP, vecFiles);
			iSubstrCharNumbers = 5; // ".json"
			break;
		case TAB_STATE::NAVIGATION:
			read_directory(JSONPATH_NAVIGATION, vecFiles);
			iSubstrCharNumbers = 4; // ".bin"
			break;
		case TAB_STATE::DYNAMIC_MODEL:
			read_directory(JSONPATH_DYNAMIC_MODEL, vecFiles);
			iSubstrCharNumbers = 5; // ".json"
			break;
		case TAB_STATE::TERRAIN:
			read_directory(JSONPATH_DYNAMIC_MODEL, vecFiles);
			iSubstrCharNumbers = 4; // ".bin"
			break;
		default:
			break;
		}

		///
		if (ImGui::BeginListBox("##JsonFiles", ImVec2(-FLT_MIN, 4 * ImGui::GetTextLineHeightWithSpacing())))
		{
			int n = 1;
			for (auto& File : vecFiles)
			{
				const bool is_selected = (m_iObject_idx == n);
				if (ImGui::Selectable(File.c_str(), is_selected))
					m_iObject_idx = n;

				if (is_selected)
				{
					strcpy_s(m_strFileName, File.substr(0, File.length() - iSubstrCharNumbers).c_str()); // ".json" 5글자 제거.
					ImGui::SetItemDefaultFocus();
				}
				n += 1;
			}

			ImGui::EndListBox();
		}
		///

		ImGui::Separator();

		static bool dont_ask_me_next_time = false;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PopStyleVar();

		if (ImGui::Button("Load", ImVec2(120, 0)))
		{
			switch (m_eTabState)
			{
			case TAB_STATE::OBJECT:
				Load_ObjectData();
				break;
			case TAB_STATE::NAVIGATION:
				Load_NavigationData();
				break;
			case TAB_STATE::DYNAMIC_MODEL:
				Load_Dynamic_ModelData();
				break;
			case TAB_STATE::TERRAIN:
				Load_Terrain_Data();
				break;
			default:
				break;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }

		ImGui::EndPopup();
	}
}

void CWindow_Tool_Map::Save_ObjectData(void)
{
	USEGAMEINSTANCE;
	list<GAMEOBJECT_DESC> listObjectDesc;

	// m_pPrototypes : Prototype Tag에 "Map"이란 것이 들어가있는 프로토타입.
	for (auto& Prototype : m_pPrototypes)
	{
		list<CGameObject*>* plistObject = nullptr;
		ID_UINT	ObjectID;
		ObjectID.iID = Prototype.first;
		plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID); // 프로로타입 ID 를 가진 Layer들에 접근.

		if (nullptr == plistObject)
			continue;

		if (ObjectID == ID_UINT(MAP, MAP_TERRAIN)
			|| ObjectID == ID_UINT(MAP, MAP_WATER)
			|| ObjectID == ID_UINT(MAP, MAP_TRIGGER)
			|| ObjectID == ID_UINT(MAP, MAP_WATER_BASE)
			|| ObjectID == ID_UINT(MAP, MAP_PORTAL)
			|| ObjectID == ID_UINT(MAP, MAP_REGION))
		{
			// m_pPrototypes 모든 원형 객체 중 모델이 없는 객체는 제외
			continue;
		}
		else
		{
			// 해당 레이어 태그에 들어있는 오브젝트들 저장.
			for (auto& pObject : *plistObject)
			{
				GAMEOBJECT_DESC tInfo;

				tInfo.iObjectID = ObjectID.iID; // (== pObject->Get_PrototypeID());
				tInfo.iModelID = pObject->Get_Component(ID_UINT(MODEL_STATIC))->Get_PrototypeID().iID;

				tInfo.WorldMatrix = pObject->Get_Transform()->Get_WorldMatrix();

				listObjectDesc.push_back(tInfo);
			}
		}
	}

	//sort(listObjectDesc.begin(), listObjectDesc.end(), [](GAMEOBJECT_DESC pSour, GAMEOBJECT_DESC pDest)
	//	{
	//		return pSour.iModelID < pDest.iModelID;
	//	});

	json jSave;
	for (auto& Object : listObjectDesc)
	{
		json jObjects;
		jObjects.emplace("iObjectID", Object.iObjectID);
		jObjects.emplace("iModelID", Object.iModelID);
		jObjects.emplace("vRight.x", XMVectorGetX(Object.WorldMatrix.r[0]));
		jObjects.emplace("vRight.y", XMVectorGetY(Object.WorldMatrix.r[0]));
		jObjects.emplace("vRight.z", XMVectorGetZ(Object.WorldMatrix.r[0]));
		jObjects.emplace("vUp.x", XMVectorGetX(Object.WorldMatrix.r[1]));
		jObjects.emplace("vUp.y", XMVectorGetY(Object.WorldMatrix.r[1]));
		jObjects.emplace("vUp.z", XMVectorGetZ(Object.WorldMatrix.r[1]));
		jObjects.emplace("vLook.x", XMVectorGetX(Object.WorldMatrix.r[2]));
		jObjects.emplace("vLook.y", XMVectorGetY(Object.WorldMatrix.r[2]));
		jObjects.emplace("vLook.z", XMVectorGetZ(Object.WorldMatrix.r[2]));
		jObjects.emplace("vPosition.x", XMVectorGetX(Object.WorldMatrix.r[3]));
		jObjects.emplace("vPosition.y", XMVectorGetY(Object.WorldMatrix.r[3]));
		jObjects.emplace("vPosition.z", XMVectorGetZ(Object.WorldMatrix.r[3]));

		jSave["MapObj"].push_back(jObjects);
	}
	string str(m_strFileName);
	pGameInstance->Save_Json(pGameInstance->json_Complete_Path(towstring("Map/" + str).c_str()).c_str(), jSave);

	//MSG_BOX_USER("Status", "SAVE DONE!");
}

void CWindow_Tool_Map::Load_ObjectData(void)
{
	USEGAMEINSTANCE;

	json jLoad;
	string str(m_strFileName);

	_uint PreID = 0;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/" + str).c_str()).c_str(), &jLoad);

	json jSourLoad = jLoad["MapObj"];

	for (auto& ObjectData : jSourLoad)
	{
		GAMEOBJECT_DESC tInfo;
		tInfo.iObjectID = ObjectData["iObjectID"];
		tInfo.iModelID = ObjectData["iModelID"];

		tInfo.WorldMatrix.r[0] = XMVectorSet(ObjectData["vRight.x"], ObjectData["vRight.y"], ObjectData["vRight.z"], 0.f);
		tInfo.WorldMatrix.r[1] = XMVectorSet(ObjectData["vUp.x"], ObjectData["vUp.y"], ObjectData["vUp.z"], 0.f);
		tInfo.WorldMatrix.r[2] = XMVectorSet(ObjectData["vLook.x"], ObjectData["vLook.y"], ObjectData["vLook.z"], 0.f);
		tInfo.WorldMatrix.r[3] = XMVectorSet(ObjectData["vPosition.x"], ObjectData["vPosition.y"], ObjectData["vPosition.z"], 1.f);

		ID_UINT iID, iModelID;
		iID.iID = tInfo.iObjectID;

		iModelID.iID = tInfo.iModelID;
		_uint iModelNum = 0;
		iModelNum = iModelID.tNums.Num2 + iModelID.tNums.Num3 * 255;
		pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), iID, iID, &iModelNum);
		pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), iID)->back()->Get_Transform()->Set_WorldMatrix(tInfo.WorldMatrix);

	}
}

void CWindow_Tool_Map::Create_Delete_Setting()
{
	ImVec2 size = ImGui::GetItemRectSize();
	if (ImGui::Button("Create", ImVec2(160.f, 0)))
		Create_Object();

	ImGui::SameLine();
	if (ImGui::Button("All Delete", ImVec2(160.f, 0)))
		Delete_AllObject();
}

void CWindow_Tool_Map::Y_Picking(_float fTimeDelta) //For Object
{
	USEGAMEINSTANCE;

	ID_UINT ID;
	ID.iID = m_iSelectedObjectPrototype.first;

	if (KEY_PRESSING(VK_CONTROL) && m_bCreate == false)
	{
		_vector		vPickedPos{ 0.f, 0.f, 0.f, 0.f };
		pGameInstance->Picking(m_fPicking_Height, &vPickedPos);

		_float fMousePosX = XMVectorGetX(vPickedPos);
		_float fMousePosZ = XMVectorGetZ(vPickedPos);

		if (m_bEffect0 == false)
		{
			// Tab키 누르면 한 번만 생성하게
			Create_Object();
			m_bEffect0 = true;
		}

		if (nullptr == pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID))
			return;

		CTransform* pTarget_TransformCom = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID)->back()->Get_Transform();

		pTarget_TransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(fMousePosX, m_fPicking_Height, fMousePosZ, 1.f));

		if (KEY_PRESSING('Q'))
		{
			if (m_iTurnMode == X_TURN)
			{
				m_fObjectTurnAngle += 3.f;
				pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(0.5f));
			}
			else if (m_iTurnMode == Y_TURN)
			{
				m_fObjectTurnAngle += 3.f;
				pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(0.5f));
			}
			else if (m_iTurnMode == Z_TURN)
			{
				m_fObjectTurnAngle += 3.f;
				pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(0.5f));
			}
		}
		if (KEY_PRESSING('E'))
		{
			if (m_iTurnMode == X_TURN)
			{
				m_fObjectTurnAngle -= 3.f;
				pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(-0.5f));
			}
			else if (m_iTurnMode == Y_TURN)
			{
				m_fObjectTurnAngle -= 3.f;
				pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-0.5f));
			}
			else if (m_iTurnMode == Z_TURN)
			{
				m_fObjectTurnAngle -= 3.f;
				pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(-0.5f));
			}
		}

		// 오브젝트의 scale값 받기
		m_fObjectScale[0] = pTarget_TransformCom->Get_Scaled().x;
		m_fObjectScale[1] = pTarget_TransformCom->Get_Scaled().y;
		m_fObjectScale[2] = pTarget_TransformCom->Get_Scaled().z;

		if (KEY_PRESSING('Z'))
		{
			// scale x값 감소
			m_fObjectScale[0] -= 0.1f;
			m_fObjectScale[1] -= 0.1f;
			m_fObjectScale[2] -= 0.1f;

			if (m_fObjectScale[0] < 0.01f ||
				m_fObjectScale[1] < 0.01f ||
				m_fObjectScale[2] < 0.01f)
			{
				m_fObjectScale[0] = 0.01f;
				m_fObjectScale[1] = 0.01f;
				m_fObjectScale[2] = 0.01f;
			}

			if (0.f >= m_fObjectScale[0])
			{
				m_fObjectScale[0] = 0.1f;
				m_fObjectScale[1] = 0.1f;
				m_fObjectScale[2] = 0.1f;
			}
			pTarget_TransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));
		}
		else if (KEY_PRESSING('C'))
		{
			// scale x값 증가
			m_fObjectScale[0] += 0.1f;
			m_fObjectScale[1] += 0.1f;
			m_fObjectScale[2] += 0.1f;

			pTarget_TransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));
		}

		if (KEY_DOWN(VK_LBUTTON) && !io.WantCaptureMouse)
		{
			// Tab키 누른 상태에서 왼쪽 클릭을 하면 객체 생성
			m_bCreate = true;
		}
		if (KEY_PRESSING('X'))
		{
			m_fCreateSpeedTime += fTimeDelta;
			if (0.2f < m_fCreateSpeedTime)
			{
				m_bCreate = true;
				m_fCreateSpeedTime = 0.f;
			}
			// Tab키 누른 상태에서 왼쪽 클릭을 하면 객체 생성
		}
	}
	else if (KEY_UP(VK_CONTROL) && m_bCreate == false)
	{
		// 만약에 객체를 생성하지 않고 Tab키를 때면 객체가 삭제
		CGameObject* pObject = nullptr;
		pObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID)->back();
		pGameInstance->Add_DeadObject(pObject);
		m_fObjectTurnAngle = 0.f;
		m_bEffect0 = false;
	}
	else
	{
		// 원래 상태로 복귀
		m_bCreate = false;
		m_bEffect0 = false;
	}
}

void CWindow_Tool_Map::Tile_Picking(_float fTimeDelta)
{
	//메쉬 피킹
	USEGAMEINSTANCE;

	// Tile의 원형들을 가져오기
	GAMEINSTANCE->Get_Prototypes(ID_UINT(MAP, MAP_TERRAIN), &m_pTilePrototypes, 2);

	// 선택된 객체의 원형을 가져오기
	ID_UINT ID;
	ID.iID = m_iSelectedObjectPrototype.first;

	// Tag에 <Map, MAP_TERRAIN> 있는것들 순회
	for (auto& Prototype : m_pTilePrototypes)
	{
		list<CGameObject*>* plistObject = nullptr;
		ID_UINT	ObjectID;
		ObjectID.iID = Prototype.first;
		plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID);

		if (nullptr == plistObject)
			return;

		if ((KEY_PRESSING(VK_CONTROL) || KEY_TOGGLE(VK_NUMPAD0)) && m_bCreate == false)
		{
			if (m_bEffect0 == false)
			{
				// Tab키 누르면 한 번만 생성하게
				Create_Object();
				m_bEffect0 = true;
			}

			// pObject를 순회
			for (auto& pObject : *plistObject)
			{
				// 버퍼 들고오기
				CVIBuffer* pVIBuffer = (CVIBuffer*)pObject->Get_Component(ID_UINT(VIBUFFER));

				// 순회하면서 Transform 던져주기
				CTransform* pTransformCom = pObject->Get_Transform();

				// Picking된 Pos를 받을 vector
				_vector		vPickedPos{ 0.f, 0.f, 0.f, 0.f };
				_uint3		vOutIndices{ 0,0,0 };

				if (pGameInstance->Picking_Terrain(pVIBuffer, pTransformCom, &vPickedPos, nullptr, &vOutIndices))
				{
					// 피킹된 좌표 받기
					_float fMousePosX = XMVectorGetX(vPickedPos);
					_float fMousePosY = XMVectorGetY(vPickedPos);
					_float fMousePosZ = XMVectorGetZ(vPickedPos);

					const _float3* pVerticesPos = pVIBuffer->Get_VerticesPos();
					_vector	vTerrainPosX = XMVector3TransformCoord(XMLoadFloat3(&pVerticesPos[vOutIndices.ix]),
																					pTransformCom->Get_WorldMatrix());
					_vector	vTerrainPosY = XMVector3TransformCoord(XMLoadFloat3(&pVerticesPos[vOutIndices.iy]), 
																					pTransformCom->Get_WorldMatrix());
					_vector	vTerrainPosZ = XMVector3TransformCoord(XMLoadFloat3(&pVerticesPos[vOutIndices.iz]), 
																					pTransformCom->Get_WorldMatrix());

					_vector vDirXY = XMVector3Normalize(vTerrainPosY - vTerrainPosX);
					_vector vDirYZ = XMVector3Normalize(vTerrainPosZ - vTerrainPosY);

					_vector vTargetUp = XMVector3Normalize(XMVector3Cross(vDirXY, vDirYZ));

					CTransform* pTarget_TransformCom = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), 
																							ID)->back()->Get_Transform();

					pTarget_TransformCom->UpTo(vTargetUp);

					pTarget_TransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(fMousePosX, fMousePosY, fMousePosZ, 1.f));

					if (KEY_PRESSING('Q'))
					{
						if (m_iTurnMode == X_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(0.5f));
						}
						else if (m_iTurnMode == Y_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(0.5f));
						}
						else if (m_iTurnMode == Z_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(0.5f));
						}
					}
					if (KEY_PRESSING('E'))
					{
						if (m_iTurnMode == X_TURN)
						{
							m_fObjectTurnAngle -= 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(-0.5f));
						}
						else if (m_iTurnMode == Y_TURN)
						{
							m_fObjectTurnAngle -= 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-0.5f));
						}
						else if (m_iTurnMode == Z_TURN)
						{
							m_fObjectTurnAngle -= 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(-0.5f));
						}
					}

					// 오브젝트의 scale값 받기
					m_fObjectScale[0] = pTarget_TransformCom->Get_Scaled().x;
					m_fObjectScale[1] = pTarget_TransformCom->Get_Scaled().y;
					m_fObjectScale[2] = pTarget_TransformCom->Get_Scaled().z;

					if (KEY_PRESSING('Z'))
					{
						// scale x값 증가
						m_fObjectScale[0] -= 0.1f;
						m_fObjectScale[1] -= 0.1f;
						m_fObjectScale[2] -= 0.1f;

						if (m_fObjectScale[0] < 0.01f ||
							m_fObjectScale[1] < 0.01f ||
							m_fObjectScale[2] < 0.01f)
						{
							m_fObjectScale[0] = 0.01f;
							m_fObjectScale[1] = 0.01f;
							m_fObjectScale[2] = 0.01f;
						}

						if (0.f >= m_fObjectScale[0])
						{
							m_fObjectScale[0] = 0.1f;
							m_fObjectScale[1] = 0.1f;
							m_fObjectScale[2] = 0.1f;
						}
						pTarget_TransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));
					}
					else if (KEY_PRESSING('C'))
					{
						// scale x값 증가
						m_fObjectScale[0] += 0.1f;
						m_fObjectScale[1] += 0.1f;
						m_fObjectScale[2] += 0.1f;



						pTarget_TransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));
					}
					//if (KEY_PRESSING('Z'))
					//{
					//	// scale x값 증가
					//	m_fObjectScale[0] += 0.5f;

					//	pTarget_TransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));
					//}
					//if (KEY_PRESSING('X'))
					//{
					//	// scale x값 증가
					//	m_fObjectScale[1] += 0.5f;
					//	pTarget_TransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));
					//}
					//if (KEY_PRESSING('C'))
					//{
					//	// scale x값 증가
					//	m_fObjectScale[2] += 0.5f;
					//	pTarget_TransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));
					//}

					if (KEY_DOWN(VK_LBUTTON) && !io.WantCaptureMouse)
					{
						// Tab키 누른 상태에서 왼쪽 클릭을 하면 객체 생성
						m_bCreate = true;
					}
					if (KEY_PRESSING('X'))
					{
						m_fCreateSpeedTime += fTimeDelta;
						if (m_fPicking_Speed < m_fCreateSpeedTime)
						{
							m_bCreate = true;
							m_fCreateSpeedTime = 0.f;
						}
						// Tab키 누른 상태에서 왼쪽 클릭을 하면 객체 생성
					}

					if (KEY_PRESSING(VK_OEM_3))
					{
						m_fCreateSpeedTime += fTimeDelta;
						if (m_fPicking_Speed < m_fCreateSpeedTime)
						{
							m_bCreate = true;
							m_fCreateSpeedTime = 0.f;
						}

						if (m_iTurnMode == X_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(0.5f));
						}
						else if (m_iTurnMode == Y_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(0.5f));
						}
						else if (m_iTurnMode == Z_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(0.5f));
						}
					}

					/*if (KEY_TOGGLE(VK_NUMPAD0))
					{
						m_fCreateSpeedTime += fTimeDelta;
						if (m_fPicking_Speed < m_fCreateSpeedTime)
						{
							m_bCreate = true;
							m_fCreateSpeedTime = 0.f;
						}

						if (m_iTurnMode == X_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(0.5f));
						}
						else if (m_iTurnMode == Y_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(0.5f));
						}
						else if (m_iTurnMode == Z_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(0.5f));
						}
					}*/
				}
			}
		}
		else if (KEY_UP(VK_CONTROL) && m_bCreate == false)
		{
			// 만약에 객체를 생성하지 않고 Tab키를 때면 객체가 삭제
			CGameObject* pObject = nullptr;
			pObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID)->back();
			pGameInstance->Add_DeadObject(pObject);

			m_bEffect0 = false;
		}
		else
		{
			// 원래 상태로 복귀
			m_bCreate = false;
			m_bEffect0 = false;
		}
	}
}

void CWindow_Tool_Map::Mesh_Picking(_float fTimeDelta)
{
	//메쉬 피킹
	USEGAMEINSTANCE;

	// Tile의 원형들을 가져오기
	pGameInstance->Get_Prototypes(ID_UINT(MAP, MAP_ROCK), &m_pMeshPrototypes, 2);
	pGameInstance->Get_Prototypes(ID_UINT(MAP, MAP_GRAY_ROCK), &m_pMeshPrototypes, 2);
	pGameInstance->Get_Prototypes(ID_UINT(MAP, MAP_ROCK_ALL), &m_pMeshPrototypes, 2);
	pGameInstance->Get_Prototypes(ID_UINT(MAP, MAP_STRUCTS_ALL), &m_pMeshPrototypes, 2);

	// 선택된 객체의 원형을 가져오기
	ID_UINT ID;
	ID.iID = m_iSelectedObjectPrototype.first;

	if ((KEY_PRESSING(VK_CONTROL) || KEY_TOGGLE(VK_NUMPAD0)) && m_bCreate == false)
	{

		_vector				vTargetPickedPos{ 0.f, 0.f, 0.f, 0.f };
		_uint3				TargetOutIndices{ 0,0,0 };
		CMeshContainer* pTargetMeshContainer = nullptr;
		CTransform* pTargetTransform = nullptr;
		_float				fTargetLength = 0.f;


		// Tag에 <Map, MAP_TERRAIN> 있는것들 순회
		for (auto& Prototype : m_pMeshPrototypes)
		{
			list<CGameObject*>* plistObject = nullptr;
			ID_UINT	ObjectID;
			ObjectID.iID = Prototype.first;
			plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID);

			if (nullptr == plistObject)
				continue;

			if (m_bEffect0 == false)
			{
				// Tab키 누르면 한 번만 생성하게
				Create_Object();
				m_bEffect0 = true;
			}

			// pObject를 순회
			for (auto& pObject : *plistObject)
			{
				// 버퍼 들고오기
				CModel* pMdoel = (CModel*)pObject->Get_Component(ID_UINT(MODEL_STATIC));
				vector<CMeshContainer*>* pMeshContainers = pMdoel->Get_MeshContainers();

				// 순회하면서 Transform 던져주기
				CTransform* pTransformCom = pObject->Get_Transform();

				for (auto& pMeshContainer : *pMeshContainers)
				{
					// Picking된 Pos를 받을 vector
					_vector		vPickedPos{ 0.f, 0.f, 0.f, 0.f };
					_uint3		vOutIndices{ 0,0,0 };

					if (pGameInstance->Picking_Terrain(pMeshContainer, pTransformCom, &vPickedPos, nullptr, &vOutIndices))
					{
						_float fLength = XMVectorGetX(XMVector3Length(XMLoadFloat4(&pGameInstance->Get_CamPosition()) - vPickedPos));
						// 피킹된 좌표 받기
						if (nullptr == pTargetTransform)
						{
							vTargetPickedPos = vPickedPos;
							TargetOutIndices = vOutIndices;
							pTargetMeshContainer = pMeshContainer;
							pTargetTransform = pTransformCom;
							fTargetLength = fLength;
						}
						else
						{
							if (fLength < fTargetLength)
							{
								vTargetPickedPos = vPickedPos;
								TargetOutIndices = vOutIndices;
								pTargetMeshContainer = pMeshContainer;
								pTargetTransform = pTransformCom;
								fTargetLength = fLength;
							}
						}
					}
				}
			}
		}

		if (pTargetTransform)
		{
			// 피킹된 좌표 받기
			_float fMousePosX = XMVectorGetX(vTargetPickedPos);
			_float fMousePosY = XMVectorGetY(vTargetPickedPos);
			_float fMousePosZ = XMVectorGetZ(vTargetPickedPos);

			const _float3* pVerticesPos = pTargetMeshContainer->Get_VerticesPos();
			_vector	vTerrainPosX = XMVector3TransformCoord(XMLoadFloat3(&pVerticesPos[TargetOutIndices.ix]), pTargetTransform->Get_WorldMatrix());
			_vector	vTerrainPosY = XMVector3TransformCoord(XMLoadFloat3(&pVerticesPos[TargetOutIndices.iy]), pTargetTransform->Get_WorldMatrix());
			_vector	vTerrainPosZ = XMVector3TransformCoord(XMLoadFloat3(&pVerticesPos[TargetOutIndices.iz]), pTargetTransform->Get_WorldMatrix());

			_vector vDirXY = XMVector3Normalize(vTerrainPosY - vTerrainPosX);
			_vector vDirYZ = XMVector3Normalize(vTerrainPosZ - vTerrainPosY);

			_vector vTargetUp = XMVector3Normalize(XMVector3Cross(vDirXY, vDirYZ));

			CTransform* pTarget_TransformCom = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID)->back()->Get_Transform();
			pTarget_TransformCom->UpTo(vTargetUp);

			pTarget_TransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(fMousePosX, fMousePosY, fMousePosZ, 1.f));

			if (KEY_PRESSING('Q'))
			{
				if (m_iTurnMode == X_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(0.5f));
				}
				else if (m_iTurnMode == Y_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(2.f));
				}
				else if (m_iTurnMode == Z_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(0.5f));
				}
			}
			if (KEY_PRESSING('E'))
			{
				if (m_iTurnMode == X_TURN)
				{
					m_fObjectTurnAngle -= 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(-0.5f));
				}
				else if (m_iTurnMode == Y_TURN)
				{
					m_fObjectTurnAngle -= 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-2.f));
				}
				else if (m_iTurnMode == Z_TURN)
				{
					m_fObjectTurnAngle -= 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(-0.5f));
				}
			}

			// 오브젝트의 scale값 받기
			m_fObjectScale[0] = pTarget_TransformCom->Get_Scaled().x;
			m_fObjectScale[1] = pTarget_TransformCom->Get_Scaled().y;
			m_fObjectScale[2] = pTarget_TransformCom->Get_Scaled().z;

			if (KEY_PRESSING('Z'))
			{
				// scale x값 증가
				m_fObjectScale[0] -= 0.1f;
				m_fObjectScale[1] -= 0.1f;
				m_fObjectScale[2] -= 0.1f;

				if (m_fObjectScale[0] < 0.01f ||
					m_fObjectScale[1] < 0.01f ||
					m_fObjectScale[2] < 0.01f)
				{
					m_fObjectScale[0] = 0.01f;
					m_fObjectScale[1] = 0.01f;
					m_fObjectScale[2] = 0.01f;
				}

				if (0.f >= m_fObjectScale[0])
				{
					m_fObjectScale[0] = 0.1f;
					m_fObjectScale[1] = 0.1f;
					m_fObjectScale[2] = 0.1f;
				}
				pTarget_TransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));
			}
			else if (KEY_PRESSING('C'))
			{
				// scale x값 증가
				m_fObjectScale[0] += 0.1f;
				m_fObjectScale[1] += 0.1f;
				m_fObjectScale[2] += 0.1f;



				pTarget_TransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));
			}

			if (KEY_DOWN(VK_LBUTTON) && !io.WantCaptureMouse)
			{
				// Tab키 누른 상태에서 왼쪽 클릭을 하면 객체 생성
				m_bCreate = true;
			}
			if (KEY_PRESSING('X'))
			{
				m_fCreateSpeedTime += fTimeDelta;
				if (m_fPicking_Speed < m_fCreateSpeedTime)
				{
					m_bCreate = true;
					m_fCreateSpeedTime = 0.f;
				}
				// Tab키 누른 상태에서 왼쪽 클릭을 하면 객체 생성
			}

			if (KEY_PRESSING(VK_OEM_3))
			{
				m_fCreateSpeedTime += fTimeDelta;
				if (m_fPicking_Speed < m_fCreateSpeedTime)
				{
					m_bCreate = true;
					m_fCreateSpeedTime = 0.f;
				}

				if (m_iTurnMode == X_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(0.5f));
				}
				else if (m_iTurnMode == Y_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(0.5f));
				}
				else if (m_iTurnMode == Z_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(0.5f));
				}
			}

			/*if (KEY_TOGGLE(VK_NUMPAD0))
			{
				m_fCreateSpeedTime += fTimeDelta;
				if (m_fPicking_Speed < m_fCreateSpeedTime)
				{
					m_bCreate = true;
					m_fCreateSpeedTime = 0.f;
				}

				if (m_iTurnMode == X_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(0.5f));
				}
				else if (m_iTurnMode == Y_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(0.5f));
				}
				else if (m_iTurnMode == Z_TURN)
				{
					m_fObjectTurnAngle += 3.f;
					pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(0.5f));
				}
			}*/
		}
	}
	else if (KEY_UP(VK_CONTROL) && m_bCreate == false)
	{
		// 만약에 객체를 생성하지 않고 Tab키를 때면 객체가 삭제
		CGameObject* pObject = nullptr;
		pObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID)->back();
		pGameInstance->Add_DeadObject(pObject);

		m_bEffect0 = false;
	}
	else
	{
		// 원래 상태로 복귀
		m_bCreate = false;
		m_bEffect0 = false;
	}
}

void CWindow_Tool_Map::Picking_Delete(_float fTimedelta)
{
	USEGAMEINSTANCE;
	_float fDist = 0;
	// 점 Render

	for (auto& Prototype : m_pPrototypes)
	{
		list<CGameObject*>* plistObject = nullptr;
		ID_UINT	ObjectID;
		ObjectID.iID = Prototype.first;
		plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID);

		if (nullptr == plistObject)
			continue;

		for (auto& pObject : *plistObject)
		{
			_float4 vPos;

			XMStoreFloat4(&vPos, pObject->Get_Pos());

			_float fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat4(&pGameInstance->Get_CamPosition()) - XMLoadFloat4(&vPos)));
			if (30.f > fDistance)
			{
				BoundingSphere ColSphere(_float3(vPos.x, vPos.y, vPos.z), 0.2f);
				if (ColSphere.Intersects(XMLoadFloat4(&pGameInstance->Get_CamPosition()), XMVector3Normalize(XMLoadFloat3(&pGameInstance->Get_RayDir())), fDist))
				{
					if (KEY_DOWN(VK_LBUTTON))
					{

						if (ObjectID == ID_UINT(MAP, MAP_GRASS_DESERT, MODEL_GRASS_DESERT)
							|| ObjectID == ID_UINT(MAP, MAP_GRASS, MODEL_GRASS)
							|| ObjectID == ID_UINT(MAP, MAP_TREE, MODEL_TREE))
						{
							pGameInstance->Add_DeadObject(pObject);
						}

					}
				}
			}
		}
	}

}

_uint CWindow_Tool_Map::Navi_Picking()
{
	_uint iNaviIndex = 0;

	USEGAMEINSTANCE;

	map<_uint, CComponent*> pPrototypes;
	pGameInstance->Get_Component_Prototypes(ID_UINT(COMPONENT::NAVIGATION), &pPrototypes);

	for (auto& pPrototype : pPrototypes)
	{
		CNavigation* pNavi = (CNavigation*)pPrototype.second;

		if (KEY_DOWN(VK_LBUTTON))
		{
			for (auto& pCell : pNavi->Get_Cells())
			{
				// 버퍼 들고오기
				CVIBuffer* pVIBuffer = (CVIBuffer*)pCell->Get_Buffer();

				if (pGameInstance->Picking(pVIBuffer, XMMatrixIdentity()))
				{
					iNaviIndex = pCell->Get_Index();
					break;
				}
			}
		}
	}

	return iNaviIndex;
}

void CWindow_Tool_Map::Create_Object(void)
{
	USEGAMEINSTANCE;


	map<_uint, CComponent*> pModelPrototypes;
	ID_UINT ID;
	ID.iID = m_iSelectedObjectPrototype.first;
	FAILEDCHECK(GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_STATIC, ID.tNums.Num2), &pModelPrototypes, 2));
	if (m_iModel_idx > pModelPrototypes.size())
	{
		m_iModel_idx = pModelPrototypes.size();
	}


	if (m_iSelectedObjectPrototype.first && m_iModel_idx)
	{
		pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), m_iSelectedObjectPrototype.first, m_iSelectedObjectPrototype.first, &m_iModel_idx);

		ID_UINT ID;
		ID.iID = m_iSelectedObjectPrototype.first;
		CTransform* pTarget_TransformCom = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID)->back()->Get_Transform();
		pTarget_TransformCom->Rotation(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(m_fObjectTurnAngle));
		pTarget_TransformCom->Set_Scaled(_float3(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]));
	}
}

void CWindow_Tool_Map::Delete_AllObject(void)
{
	USEGAMEINSTANCE;
	// m_pPrototypes : Prototype Tag에 "Map"이란 것이 들어가있는 프로토타입.
	for (auto& Prototype : m_pPrototypes)
	{
		list<CGameObject*>* plistObject = nullptr;
		ID_UINT	ObjectID;
		ObjectID.iID = Prototype.first;

		if (ID_UINT(MAP, MAP_TRIGGER).iID == ObjectID.iID ||
			ID_UINT(MAP, MAP_TERRAIN).iID == ObjectID.iID ||
			ID_UINT(MAP, MAP_WATER).iID == ObjectID.iID)
			continue;

		plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID); // 프로로타입 ID 를 가진 Layer들에 접근.

		if (nullptr == plistObject)
			continue;

		// 해당 레이어 태그에 들어있는 오브젝트들 저장.
		for (auto& pObject : *plistObject)
		{
			pGameInstance->Add_DeadObject(pObject);
		}
	}
}

void CWindow_Tool_Map::Picking_Setting(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	static int ePickingtMode = TILE_PICKING;

	ImGui::RadioButton("Y_Picking", &ePickingtMode, Y_PICKING); ImGui::SameLine();
	ImGui::RadioButton("Tile_Picking", &ePickingtMode, TILE_PICKING); ImGui::SameLine();
	ImGui::RadioButton("Mesh_Picking", &ePickingtMode, MESH_PICKING);
	ImGui::RadioButton("PickObj_Delete", &ePickingtMode, DELETE_PICKING);

	if (m_iModel_idx == 0)
		return;

	if (PICKING_MODE::Y_PICKING == ePickingtMode)
	{
		Y_Picking(fTimeDelta);
	}
	else if (PICKING_MODE::TILE_PICKING == ePickingtMode)
	{
		Tile_Picking(fTimeDelta);
	}
	else if (PICKING_MODE::MESH_PICKING == ePickingtMode)
	{
		Mesh_Picking(fTimeDelta);
	}
	else if (PICKING_MODE::DELETE_PICKING == ePickingtMode)
	{
		if (KEY_PRESSING(VK_SHIFT))
		{
			m_bColRender = true;
		}
		else
		{
			m_bColRender = false;
		}
		Picking_Delete(fTimeDelta);
	}

	if (PICKING_MODE::DELETE_PICKING != ePickingtMode)
	{
		m_bColRender = false;
	}


	if (KEY_PRESSING(VK_MENU))
	{
		_long		MouseMove = 0;
		if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_WHEEL))
		{
			_float vCurOffset = m_fPicking_Height;

			_float	fMouseMove = MouseMove;
			fMouseMove = max(-100.f, fMouseMove);
			fMouseMove = min(100.f, fMouseMove);
			fMouseMove *= 0.05f;
			fMouseMove = (fMouseMove + 100.f) / 100.f;

			m_fPicking_Height = vCurOffset * fMouseMove;
		}
	}

	ImGui::Text("Set Picking Y_Height");
	ImGui::DragFloat("drag float-Height", &m_fPicking_Height, 0.01f);

	ImGui::Text("Set Picking Speed ");
	ImGui::DragFloat("drag float-Speed", &m_fPicking_Speed, 0.01f);

	ImGui::Separator();

	if (KEY_DOWN(VK_RBUTTON))
	{
		m_iTurnMode += 1;
		if (TURN_MODE_END - 1 < m_iTurnMode)
			m_iTurnMode = 0;
	}

	ImGui::RadioButton("X_Turn", &m_iTurnMode, X_TURN); ImGui::SameLine();
	ImGui::RadioButton("Y_Turn", &m_iTurnMode, Y_TURN); ImGui::SameLine();
	ImGui::RadioButton("Z_Turn", &m_iTurnMode, Z_TURN);

}

string CWindow_Tool_Map::Get_StaticObjectIDStr(_uint _ID)
{
	string RtnStr = "";

	ID_UINT ID;
	ID.iID = _ID;
	switch (ID.tNums.Num1)
	{
	case MAP_TREE:
		RtnStr = "TREE";
		break;
	case MAP_ROCK:
		RtnStr = "ROCK";
		break;
	case MAP_TILE:
		RtnStr = "TILE";
		break;
	case MAP_GRASS:
		RtnStr = "GRASS";
		break;
	case MAP_STAIR:
		RtnStr = "STAIR";
		break;
	case MAP_STRUCTS:
		RtnStr = "STRUCTS";
		break;
	case MAP_ITEM:
		RtnStr = "ITEM";
		break;
	case MAP_GRAY_ROCK:
		RtnStr = "GRAY_ROCK";
		break;
	case MAP_TEST_PLAYER:
		RtnStr = "TEST_PLAYER";
		break;
	case MAP_GRASS_DESERT:
		RtnStr = "GRASS2";
		break;
	case MAP_STRUCTS_DESERT:
		RtnStr = "STRUCTS2";
		break;
	case MAP_ROCK_ALL:
		RtnStr = "ROCK_ALL";
		break;
	case MAP_GRASSMESH:
		RtnStr = "GRASSMESH";
		break;
	case MAP_ROOM:
		RtnStr = "Room";
		break;
	case MAP_EQUIPMENT:
		RtnStr = "Equipment";
		break;
	case MAP_STRUCTS_ALL:
		RtnStr = "STRUCTS_ALL";
		break;
	case MAP_SHELF_ITEM:
		RtnStr = "Shelf_Item";
		break;
	case MAP_COIN:
		RtnStr = "Coin";
		break;
	case MAP_RING:
		RtnStr = "Ring";
		break;
	default:
		RtnStr = "PleaseAddName";
		break;
	}

	return RtnStr;
}

///
/// Navigation
///

void CWindow_Tool_Map::Navigation_Setting(void)
{
	USEGAMEINSTANCE;
	// Save & Load
	Save_Load_Data();
	ImGui::Separator();

	// Select Option
	ImGui::Text("\"RButton to Change Mode!!\"");

	static int ePointSelectMode = SELECT_POINT;
	//const char* elems_names[SETTING_MODE_END] = { "Select Point", "Create Point", "Delete Point" };
	//const char* elem_name = (ePointSelectMode >= 0 && ePointSelectMode < SETTING_MODE_END) ? elems_names[ePointSelectMode] : "Unknown";
	//ImGui::SliderInt("Setting Mode", &ePointSelectMode, 0, SETTING_MODE_END - 1, elem_name);

	m_bCellCenterRender = false;
	m_bCellPointRender = false;

	ImGui::RadioButton("Create", &ePointSelectMode, CREATE_POINT); ImGui::SameLine();
	ImGui::RadioButton("Select", &ePointSelectMode, SELECT_POINT); ImGui::SameLine();
	ImGui::RadioButton("Delete", &ePointSelectMode, DELETE_POINT); //ImGui::SameLine();
	ImGui::RadioButton("Picking", &ePointSelectMode, PICKING_POINT);

	ImGui::Separator();
	ImVec2 size = ImGui::GetItemRectSize();
	if (ImGui::Button("Create_Navi", ImVec2(size.x * 0.5f, 0)))
	{
		Create_NaviCell_Terrain();
	}
	ImGui::Separator();

	if (KEY_DOWN(VK_RBUTTON))
	{
		ePointSelectMode += 1;
		if (SETTING_MODE_END - 1 < ePointSelectMode)
			ePointSelectMode = 0;
	}

	static int ePickingtMode_Navi = Y_PICKING;
	if (CREATE_POINT == ePointSelectMode)
	{
		ImGui::RadioButton("Y_Picking ", &ePickingtMode_Navi, Y_PICKING); ImGui::SameLine();
		ImGui::RadioButton("Tile_Picking ", &ePickingtMode_Navi, TILE_PICKING);

		ImGui::Text("Set Picking Y_Height ");
		ImGui::DragFloat("drag float ", &m_fPicking_Height, 0.01f);
	}

	static int ePickingtMode_Pick = CELL_NORMAL;
	if (PICKING_POINT == ePointSelectMode)
	{
		m_bCellCenterRender = true;
		const _uint    iScalarStep = 1;
		ImGui::Text("Set Picking Cell Setting ");
		ImGui::RadioButton("Normal ", &ePickingtMode_Pick, CELL_NORMAL);
		ImGui::RadioButton("UnableMove ", &ePickingtMode_Pick, CELL_NOMOVE);
		m_iCellProperty = ePickingtMode_Pick;
	}
	if (SELECT_POINT == ePointSelectMode ||
		DELETE_POINT == ePointSelectMode ||
		CREATE_POINT == ePointSelectMode)
	{
		m_bCellPointRender = true;
	}

	if (ImGui::Button("Delete All", ImVec2(-FLT_MIN, 0)))
		Delete_AllCellPoint();

	ImGui::Separator();

	if (KEY_DOWN(VK_LBUTTON) && !io.WantCaptureMouse)
	{
		if (SETTING_MODE::SELECT_POINT == ePointSelectMode)
		{
			Select_CellPoint();
		}
		else if (SETTING_MODE::CREATE_POINT == ePointSelectMode)
		{
			if (PICKING_MODE::Y_PICKING == ePickingtMode_Navi)
				Create_CellPoint_YHeightPicking();
			else if (PICKING_MODE::TILE_PICKING == ePickingtMode_Navi)
				Create_CellPoint_TilePicking();
		}
		else if (SETTING_MODE::DELETE_POINT == ePointSelectMode)
		{
			Delete_CellPoint();
		}
		else if (SETTING_MODE::PICKING_POINT == ePointSelectMode)
			Select_CellCenter();
		//Navi_Picking();
	}
}

void CWindow_Tool_Map::Save_NavigationData(void)
{
	string str(m_strFileName);
	HANDLE	hFile = CreateFile(towstring(JSONPATH_NAVIGATION + str + ".bin").c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte = 0;
	vector<CCell::CELLDESC>		vecCellDesc;

	//_uint iIndex = 0;


	for (size_t i = 0; i < m_Cells.size(); ++i)
	{
		CCell* pCell = m_Cells[i];

		CCell::CELLDESC CellDesc;

		//CellDesc.iIndex = pCell->Get_Index();
		CellDesc.iIndex = i;
		CellDesc.iProperty = 0;

		//CellDesc.vPoints;
		XMStoreFloat3(&CellDesc.vPoints[CCell::POINT_A], pCell->Get_Point(CCell::POINT_A));
		XMStoreFloat3(&CellDesc.vPoints[CCell::POINT_B], pCell->Get_Point(CCell::POINT_B));
		XMStoreFloat3(&CellDesc.vPoints[CCell::POINT_C], pCell->Get_Point(CCell::POINT_C));

		XMStoreFloat3(&CellDesc.vLine[CCell::LINE_AB], pCell->Get_Line(CCell::LINE_AB));
		XMStoreFloat3(&CellDesc.vLine[CCell::LINE_BC], pCell->Get_Line(CCell::LINE_BC));
		XMStoreFloat3(&CellDesc.vLine[CCell::LINE_CA], pCell->Get_Line(CCell::LINE_CA));

		//XMStoreFloat3(&CellDesc.vNormal[CCell::LINE_AB], pCell->Get_Normal(CCell::LINE_AB));
		//XMStoreFloat3(&CellDesc.vNormal[CCell::LINE_BC], pCell->Get_Normal(CCell::LINE_BC));
		//XMStoreFloat3(&CellDesc.vNormal[CCell::LINE_CA], pCell->Get_Normal(CCell::LINE_CA));

		// 평면 노말 구하기 (각도 구하기)
		XMStoreFloat3(&CellDesc.vPlaneNormal, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&CellDesc.vLine[CCell::LINE_AB]), XMLoadFloat3(&CellDesc.vLine[CCell::LINE_BC]))));
		pCell->Set_PlaneNormal(CellDesc.vPlaneNormal);
		//CellDesc.fRadianAngle = acosf(XMVectorGetX(XMVector3Dot(vPlaneNormal, XMVectorSet(0.f, 1.f, 0.f, 0.f))));

		_uint iNeighbor = 0;

		// Make Neighbor And Save //CellDesc.iNeighbor;
		//for (auto& pDestCell : m_Cells)
		for (size_t j = 0; j < m_Cells.size(); ++j)
		{
			CCell* pDestCell = m_Cells[j];

			if (pCell == pDestCell)
				continue;

			if (iNeighbor > 2)
				break;


			//123
			//11-2	시계
			//-1-12	반시계

			_bool	bReverse = false;
			if (true == pDestCell->Compare_Points(
				pCell->Get_Point(CCell::POINT_A),
				pCell->Get_Point(CCell::POINT_B), CCell::POINT_A, CCell::POINT_B, &bReverse))
			{
				//pCell->Set_Neighbor(CCell::LINE_AB, pDestCell->Get_Index());
				//CellDesc.iNeighbor[CCell::LINE_AB] = pDestCell->Get_Index();
				CellDesc.iNeighbor[CCell::LINE_AB] = j;

				//if (true == bReverse)
				//	Setup_ReversePoint(&CellDesc);


				iNeighbor++;
				continue;
			}

			if (true == pDestCell->Compare_Points(
				pCell->Get_Point(CCell::POINT_B),
				pCell->Get_Point(CCell::POINT_C), CCell::POINT_B, CCell::POINT_C, &bReverse))
			{
				//pCell->Set_Neighbor(CCell::LINE_BC, pDestCell->Get_Index());
				//CellDesc.iNeighbor[CCell::LINE_BC] = pDestCell->Get_Index();
				CellDesc.iNeighbor[CCell::LINE_BC] = j;

				//if (true == bReverse)
				//	Setup_ReversePoint(&CellDesc);

				iNeighbor++;
				continue;
			}

			if (true == pDestCell->Compare_Points(
				pCell->Get_Point(CCell::POINT_C),
				pCell->Get_Point(CCell::POINT_A), CCell::POINT_C, CCell::POINT_A, &bReverse))
			{
				//pCell->Set_Neighbor(CCell::LINE_CA, pDestCell->Get_Index());
				//CellDesc.iNeighbor[CCell::LINE_CA] = pDestCell->Get_Index();
				CellDesc.iNeighbor[CCell::LINE_CA] = j;

				//if (true == bReverse)
				//	Setup_ReversePoint(&CellDesc);

				iNeighbor++;
				continue;
			}
		}

		vecCellDesc.push_back(CellDesc);
	}

	for (auto& CellDesc : vecCellDesc)
	{
		_vector vPlaneNormal = XMLoadFloat3(&CellDesc.vPlaneNormal);
		//_vector vNeighborPlaneNormal[CCell::LINE_END];

		for (size_t i = 0; i < CCell::LINE_END; ++i)
		{
			//// 3D Normal
			//if (-1 == CellDesc.iNeighbor[i])
			//{
			CellDesc.vNormal[i] = _float3(CellDesc.vLine[i].z * -1.f, 0.f, CellDesc.vLine[i].x);
			//}
			//else
			//{
			//	vNeighborPlaneNormal[i] = m_Cells[CellDesc.iNeighbor[i]]->Get_PlaneNormal();
			//	vNeighborPlaneNormal[i] = XMVector3Normalize(vPlaneNormal + vNeighborPlaneNormal[i]);
			//	XMStoreFloat3(&CellDesc.vNormal[i], XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&CellDesc.vLine[i]), vNeighborPlaneNormal[i])));
			//}
		}

		WriteFile(hFile, &CellDesc, sizeof(CCell::CELLDESC), &dwByte, nullptr);
	}


	CloseHandle(hFile);

	//MSG_BOX_USER("Status", "SAVE DONE!");
}

void CWindow_Tool_Map::Load_NavigationData(void)
{
	_ulong		dwByte = 0;

	string str(m_strFileName);
	wstring test = towstring(JSONPATH_NAVIGATION + str + ".bin");
	HANDLE		hFile = CreateFile(test.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	while (true)
	{
		CCell::CELLDESC		CellDesc;

		// Client에서는 이렇게만 불러주면 됨.
		ReadFile(hFile, &CellDesc, sizeof(CCell::CELLDESC), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell* pCell = CCell::Create(CellDesc);

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);


	Check_SameCell();


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 툴에서 수정하기 위해 추가적으로 해주는 작업. (위에서 m_Cell 을 저장했으므로, 각 Cell들의 Points 들을 저장해줌.)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (auto& Cell : m_Cells)
	{
		// 셀의 세 점이 기존 가지고 있는 점과 중복되는지 체크.
		bool bRtnA = true;
#if 1
		bool bRtnB = true;
		bool bRtnC = true;
		for (auto& fPoint : m_vecPoints)
		{
			if (XMVector3Equal(XMLoadFloat3(&fPoint.Point), Cell->Get_Point(CCell::POINT_A)))
				bRtnA = false;

			if (XMVector3Equal(XMLoadFloat3(&fPoint.Point), Cell->Get_Point(CCell::POINT_B)))
				bRtnB = false;

			if (XMVector3Equal(XMLoadFloat3(&fPoint.Point), Cell->Get_Point(CCell::POINT_C)))
				bRtnC = false;

		}
		// 중복되어있지 않은 점만 저장한다.
		if (bRtnA)
		{
			POINT_DESC stTemp;
			stTemp.Color = DirectX::Colors::White;
			XMStoreFloat3(&stTemp.Point, Cell->Get_Point(CCell::POINT_A));
			m_vecPoints.push_back(stTemp);
		}
		if (bRtnB)
		{
			POINT_DESC stTemp;
			stTemp.Color = DirectX::Colors::White;
			XMStoreFloat3(&stTemp.Point, Cell->Get_Point(CCell::POINT_B));
			m_vecPoints.push_back(stTemp);
		}
		if (bRtnC)
		{
			POINT_DESC stTemp;
			stTemp.Color = DirectX::Colors::White;
			XMStoreFloat3(&stTemp.Point, Cell->Get_Point(CCell::POINT_C));
			m_vecPoints.push_back(stTemp);
		}
#endif // 1

	}
}

void CWindow_Tool_Map::Create_NaviCell_Terrain()
{
	USEGAMEINSTANCE;

	list<CGameObject*>* plistObject = nullptr;
	plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID_UINT(MAP, MAP_NAVI));


	if (nullptr == plistObject)
		return;

	for (auto& pObject : *plistObject)
	{
		CVIBuffer* pBuffer = (CVIBuffer*)pObject->Get_Component(ID_UINT(VIBUFFER));

		CTransform* pTransformCom = pObject->Get_Transform();

		_uint	iNum = pBuffer->Get_NumVertice();
		_uint	iNumIndices = pBuffer->Get_NumPrimitive();

		POINT_DESC stTemp;

		for (_uint i = 0; i < iNumIndices; ++i)
		{
			vector<_float3>				vecSelectedPoint;

			if (pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).ix].y <= 10.5f
				|| pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iy].y <= 10.5f
				|| pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iz].y <= 10.5f)
				continue;

			if (pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).ix].y == 0.f
				&& pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iy].y == 0.f
				&& pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iz].y == 0.f)
				continue;

			if (pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).ix].y != pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iy].y
				|| pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).ix].y != pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iz].y
				|| pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iy].y != pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iz].y)
				continue;
			//if (pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).ix].y == 0.f
			//	&& pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iy].y == 0.f
			//	&& pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iz].y == 0.f)
			//	continue;
			//
			//if (pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).ix].y != pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iy].y
			//	|| pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).ix].y != pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iz].y
			//	|| pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iy].y != pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iz].y)
			//	continue;

			stTemp.Point = pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).ix];
			vecSelectedPoint.push_back(stTemp.Point);

			stTemp.Point = pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iy];
			vecSelectedPoint.push_back(stTemp.Point);

			stTemp.Point = pBuffer->Get_VerticesPos()[pBuffer->Get_Indices(i).iz];
			vecSelectedPoint.push_back(stTemp.Point);

			// Cell 생성
			_float3 vPoints[CCell::POINT_END];
			vPoints[CCell::POINT_A] = vecSelectedPoint[0];
			vPoints[CCell::POINT_B] = vecSelectedPoint[1];
			vPoints[CCell::POINT_C] = vecSelectedPoint[2];

			CCell* pCell = CCell::Create(vPoints, _int(m_Cells.size()));
			m_Cells.push_back(pCell);
		}
	}

}

void CWindow_Tool_Map::Create_CellPoint_YHeightPicking()
{
	USEGAMEINSTANCE;
	_vector vPickedPos;
	pGameInstance->Picking(m_fPicking_Height, &vPickedPos);
	POINT_DESC stTemp;
	XMStoreFloat3(&stTemp.Point, XMVectorSet(XMVectorGetX(vPickedPos), m_fPicking_Height, XMVectorGetZ(vPickedPos), 0.f));
	m_vecPoints.push_back(stTemp);
}

void CWindow_Tool_Map::Create_CellPoint_TilePicking()
{
	USEGAMEINSTANCE;

	_vector		vPickedPos{ 0.f, 0.f, 0.f, 0.f };

	vector<POINT_DESC>         m_vecPickedPoints;

	for (auto& Prototype : m_pPrototypes)
	{
		list<CGameObject*>* plistObject = nullptr;
		ID_UINT	ObjectID;
		ObjectID.iID = Prototype.first;
		plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID);

		if (nullptr == plistObject)
			continue;

		for (auto& pObject : *plistObject)
		{
			// 버퍼 들고오기
			CVIBuffer* pVIBuffer = (CVIBuffer*)pObject->Get_Component(ID_UINT(VIBUFFER));
			CModel* pModel = (CModel*)pObject->Get_Component(ID_UINT(MODEL_STATIC));

			CTransform* pTransformCom = pObject->Get_Transform();

			if (pModel != nullptr)
			{
				// 터레인이 아닌(버퍼가 없는) 모델들을 피킹하기 위해서
				vector<CMeshContainer*>* pMeshContainer = pModel->Get_MeshContainers();

				for (auto& pMeshContainer : *pMeshContainer)
				{
					if (pGameInstance->Picking(pMeshContainer, pTransformCom, &vPickedPos))
					{
						POINT_DESC stTemp;
						XMStoreFloat3(&stTemp.Point, vPickedPos);
						m_vecPickedPoints.push_back(stTemp);
					}
				}
			}
			else if (nullptr != pVIBuffer)
			{
				if (pGameInstance->Picking(pVIBuffer, pTransformCom, &vPickedPos))
				{
					POINT_DESC stTemp;
					XMStoreFloat3(&stTemp.Point, vPickedPos);
					m_vecPickedPoints.push_back(stTemp);
				}
			}
		}
	}
	_float vTemp = 0.f;
	POINT_DESC NearPoint;
	_bool bIsFirsetTime = true;
	for (auto& Point : m_vecPickedPoints)
	{
		if (bIsFirsetTime)
		{
			vTemp = XMVectorGetX(XMVector3Length(XMLoadFloat4(&GAMEINSTANCE->Get_CamPosition()) - XMVectorSet(Point.Point.x, Point.Point.y, Point.Point.z, 1.f)));
			NearPoint = Point;
			bIsFirsetTime = false;
		}
		else
		{
			if (vTemp > XMVectorGetX(XMVector3Length(XMLoadFloat4(&GAMEINSTANCE->Get_CamPosition()) - XMVectorSet(Point.Point.x, Point.Point.y, Point.Point.z, 1.f))))
			{
				vTemp = XMVectorGetX(XMVector3Length(XMLoadFloat4(&GAMEINSTANCE->Get_CamPosition()) - XMVectorSet(Point.Point.x, Point.Point.y, Point.Point.z, 1.f)));
				NearPoint = Point;
			}
		}
	}
	m_vecPoints.push_back(NearPoint);
}

void CWindow_Tool_Map::Select_CellPoint()
{
	_float fDist = 0;

	for (auto& iter : m_vecPoints)
	{
		USEGAMEINSTANCE;
		BoundingSphere ColSphere(iter.Point, 0.2f);
		if (ColSphere.Intersects(XMLoadFloat4(&pGameInstance->Get_CamPosition()), XMVector3Normalize(XMLoadFloat3(&pGameInstance->Get_RayDir())), fDist))
		{
			iter.Color = DirectX::Colors::Red;

			for (auto& TempCellPoint : m_vecSelectedPoint)
			{
				// 같은 점 고를 시.
				if (XMVector3Equal(XMLoadFloat3(&TempCellPoint), XMLoadFloat3(&iter.Point)))
					return;
			}
			m_vecSelectedPoint.push_back(iter.Point);

			// 3점 안채워져 있으면 추가.
			if (3 > m_vecSelectedPoint.size())
			{
				return;
			}
			else // 3점이 모두 선택되면,
			{
				// 여기서 정렬한뒤 벡터에 넣어준다.
				CELL_POINT TempCellPoint;

				TempCellPoint.PointA = m_vecSelectedPoint[0];
				TempCellPoint.PointB = m_vecSelectedPoint[1];
				TempCellPoint.PointC = m_vecSelectedPoint[2];

				//_vector LineAB = XMLoadFloat3(&TempCellPoint.PointB) - XMLoadFloat3(&TempCellPoint.PointA);
				//_vector LineAC = XMLoadFloat3(&TempCellPoint.PointC) - XMLoadFloat3(&TempCellPoint.PointA);
				//_vector LineAB_xzNormal = XMVector2Normalize({ -XMVectorGetZ(LineAB), XMVectorGetX(LineAB), 0.f, 0.f });
				//_vector LineAC_xz = XMVector2Normalize({ XMVectorGetX(LineAC), XMVectorGetZ(LineAC), 0.f, 0.f });

				//_float	fCheckDot = XMVectorGetX(XMVector2Dot(LineAB_xzNormal, LineAC_xz));

				//CELL_POINT CellPoint;
				//if (0.f < fCheckDot)
				//{
				//	CellPoint.PointA = TempCellPoint.PointA;
				//	CellPoint.PointB = TempCellPoint.PointC;
				//	CellPoint.PointC = TempCellPoint.PointB;
				//}
				//else
				//{
				//	CellPoint.PointA = TempCellPoint.PointA;
				//	CellPoint.PointB = TempCellPoint.PointB;
				//	CellPoint.PointC = TempCellPoint.PointC;
				//}

				//for (auto& iter : m_vecSelectedPoint)
				//{
				//	// 가장 왼쪽 점
				//	if (iter.x == min(min(m_vecSelectedPoint[0].x, m_vecSelectedPoint[1].x), m_vecSelectedPoint[2].x))
				//		TempCellPoint.PointA = iter;
				//	// 가장 오른쪽 점.
				//	else if (iter.x == max(max(m_vecSelectedPoint[0].x, m_vecSelectedPoint[1].x), m_vecSelectedPoint[2].x))
				//		TempCellPoint.PointB = iter;

				//	else
				//		TempCellPoint.PointC = iter;
				//}

				CELL_POINT CellPoint;
				_vector LineAB = XMLoadFloat3(&TempCellPoint.PointB) - XMLoadFloat3(&TempCellPoint.PointA);
				_vector LineAC = XMLoadFloat3(&TempCellPoint.PointC) - XMLoadFloat3(&TempCellPoint.PointA);

				// Cell ACB
				if (0 > XMVectorGetY(XMVector3Cross(LineAB, LineAC)))
				{
					CellPoint.PointA = TempCellPoint.PointA;
					CellPoint.PointB = TempCellPoint.PointC;
					CellPoint.PointC = TempCellPoint.PointB;
				}
				// Cell ABC
				else
				{
					CellPoint.PointA = TempCellPoint.PointA;
					CellPoint.PointB = TempCellPoint.PointB;
					CellPoint.PointC = TempCellPoint.PointC;
				}

				m_vecCells.push_back(CellPoint);

				for (auto& iter : m_vecPoints)
				{
					if (XMVector4Equal(iter.Color, DirectX::Colors::Red))
					{
						iter.Color = DirectX::Colors::White;
					}
				}

				m_vecSelectedPoint.clear();
				vector<_float3>().swap(m_vecSelectedPoint);

				// Cell 생성
				_float3 vPoints[CCell::POINT_END];
				vPoints[CCell::POINT_A] = CellPoint.PointA;
				vPoints[CCell::POINT_B] = CellPoint.PointB;
				vPoints[CCell::POINT_C] = CellPoint.PointC;

#if 1 // 만든 Cell이 중복된 Cell인지 체크
				_bool bIsSamePointed = false;
				if (0 == m_Cells.size()) // 최초의 Cell 일 때.
					bIsSamePointed = false;

				for (auto& pCell : m_Cells)
				{
					if (XMVector3Equal(XMLoadFloat3(&vPoints[CCell::POINT_A]), pCell->Get_Point(CCell::POINT_A)) &&
						XMVector3Equal(XMLoadFloat3(&vPoints[CCell::POINT_B]), pCell->Get_Point(CCell::POINT_B)) &&
						XMVector3Equal(XMLoadFloat3(&vPoints[CCell::POINT_C]), pCell->Get_Point(CCell::POINT_C)))
					{
						// 같은 점이 있을 시.
						bIsSamePointed = true;
					}
					else
						continue; // 다른 점이면 그냥 넘어감.
				}
				if (!bIsSamePointed)
#endif // 1
				{
					CCell* pCell = CCell::Create(vPoints, _int(m_Cells.size()));
					m_Cells.push_back(pCell);
				}
				break;
			}
		}
	}
}

void CWindow_Tool_Map::Select_CellCenter()
{
	_float fDist = 0;

	for (auto& pCell : m_Cells)
	{
		if (nullptr == pCell)
			continue;

		USEGAMEINSTANCE;
		_float3 vCenter = pCell->Get_Center();
		BoundingSphere ColSphere(vCenter, 0.2f);

		if (ColSphere.Intersects(XMLoadFloat4(&pGameInstance->Get_CamPosition()), XMVector3Normalize(XMLoadFloat3(&pGameInstance->Get_RayDir())), fDist))
		{
			pCell->Set_Property(m_iCellProperty);
		}

	}
}

void CWindow_Tool_Map::Delete_CellPoint()
{
	_float fDist = 0;
	USEGAMEINSTANCE;
	for (auto& iter = m_vecPoints.begin(); iter != m_vecPoints.end();)
	{
		BoundingSphere ColSphere(iter->Point, 0.2f);
		if (ColSphere.Intersects(XMLoadFloat4(&pGameInstance->Get_CamPosition()), XMVector3Normalize(XMLoadFloat3(&pGameInstance->Get_RayDir())), fDist))
		{

			for (auto& Cell = m_Cells.begin(); Cell != m_Cells.end();)
			{
				// Cell 의 한점이라면, 해당 Cell 삭제.
				if (XMVector3Equal(XMLoadFloat3(&iter->Point), (*Cell)->Get_Point(CCell::POINT_A)) ||
					XMVector3Equal(XMLoadFloat3(&iter->Point), (*Cell)->Get_Point(CCell::POINT_B)) ||
					XMVector3Equal(XMLoadFloat3(&iter->Point), (*Cell)->Get_Point(CCell::POINT_C)))
				{
					CCell* temp = *Cell;
					Safe_Release(temp);
					Cell = m_Cells.erase(Cell);
				}
				else
				{
					++Cell;
				}
			}

			iter = m_vecPoints.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	//for (auto& iter : m_vecPoints)
	//	iter.Color = DirectX::Colors::Green;

	m_vecSelectedPoint.clear();
	vector<_float3>().swap(m_vecSelectedPoint);
}

void CWindow_Tool_Map::Delete_AllCellPoint()
{
	for (auto& iter = m_vecPoints.begin(); iter != m_vecPoints.end();)
	{
		iter = m_vecPoints.erase(iter);
	}
	for (auto& Cell = m_Cells.begin(); Cell != m_Cells.end();)
	{
		Cell = m_Cells.erase(Cell);
	}

	m_vecSelectedPoint.clear();
	vector<_float3>().swap(m_vecSelectedPoint);
}


///
/// Dynamic_Model
///

void CWindow_Tool_Map::Dynamic_Model_Setting()
{
	// Save & Load
	Save_Load_Data();
	ImGui::Separator();

	if (ImGui::TreeNode("Model_Type"))
	{
		/*RadioButton*/
		if (ImGui::RadioButton("PLAYER", m_iSelectModel == ANIMMODEL_PLAYER)) { m_iSelectModel = ANIMMODEL_PLAYER; }
		if (ImGui::RadioButton("NORMAL_ENEMY", m_iSelectModel == ANIMMODEL_NORMALMON)) { m_iSelectModel = ANIMMODEL_NORMALMON; }
		if (ImGui::RadioButton("NORMAL_BOSS", m_iSelectModel == ANIMMODEL_BOSS)) { m_iSelectModel = ANIMMODEL_BOSS; }
		ImGui::TreePop();
	}

	_uint iID = MODEL_PLAYER;

	switch (m_iSelectModel)
	{
		// 선택한 이름에 따라서 Model Prototypes 가져오게
	case ANIMMODEL_PLAYER:
		iID = MODEL_PLAYER;
		break;
	case ANIMMODEL_NORMALMON:
		iID = MODEL_ENEMY;
		break;
	case ANIMMODEL_BOSS:
		iID = MODEL_ENEMY_BOSS;
		break;
	}

	ImGui::Text("Selected Object Models List");
	if (ImGui::BeginListBox("##Models", ImVec2(-FLT_MIN, 6 * ImGui::GetTextLineHeightWithSpacing())))
	{
		map<_uint, CComponent*> pEnemyPrototypes;
		FAILEDCHECK(GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_DYNAMIC, iID), &pEnemyPrototypes, 2));

		int n = 1;
		for (auto& Prototype : pEnemyPrototypes)
		{
			// 모델 이름들 가져옴
			const bool is_selected = (m_iModelNum == n);
			if (ImGui::Selectable(Prototype.second->Get_ModelFileName().c_str(), is_selected))
				m_iModelNum = n;

			if (is_selected)
			{
				if (iID == MODEL_ENEMY && m_iModelNum == 1)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, HYENAGANG_MINIONS);
				else if (iID == MODEL_ENEMY && m_iModelNum == 2)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, HYENAGANG_SHOOTER);
				else if (iID == MODEL_ENEMY && m_iModelNum == 3)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, GANGSTER_CAP);
				else if (iID == MODEL_ENEMY && m_iModelNum == 4)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION);
				else if (iID == MODEL_ENEMY && m_iModelNum == 5)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION_MECHANIC);
				else if (iID == MODEL_ENEMY && m_iModelNum == 6)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, HABAKA_ADMIRAL);
				else if (iID == MODEL_ENEMY && m_iModelNum == 7)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION_ORC);
				else if (iID == MODEL_ENEMY && m_iModelNum == 8)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, HUNTERDOG);
				else if (iID == MODEL_ENEMY && m_iModelNum == 9)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION_HUNTERDOG);
				else if (iID == MODEL_ENEMY && m_iModelNum == 10)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_DOUBLEEDGEDSWORDADEPT);
				else if (iID == MODEL_ENEMY && m_iModelNum == 11)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_SWORDGHOST);
				else if (iID == MODEL_ENEMY && m_iModelNum == 12)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_GUNNER);
				else if (iID == MODEL_ENEMY && m_iModelNum == 13)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_DOORKEEPER);

				else if (iID == MODEL_ENEMY_BOSS && m_iModelNum == 1)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON);
				else if (iID == MODEL_ENEMY_BOSS && m_iModelNum == 2)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_WEAVER);
				else if (iID == MODEL_ENEMY_BOSS && m_iModelNum == 3)
					m_iSelectedDynamicPrototypeID = ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_ROBELLA);

				else if (iID == MODEL_PLAYER && m_iModelNum == 1)
					m_iSelectedDynamicPrototypeID = ID_UINT(PLAYER);

				ImGui::SetItemDefaultFocus();
			}

			n += 1;
		}

		ImGui::EndListBox();
	}
}

void CWindow_Tool_Map::Dynamic_Model_Create_Delete_Setting()
{
	ImVec2 size = ImGui::GetItemRectSize();

	//if (ImGui::Button("Create", ImVec2(size.x * 0.5f, 0)))
	Dynamic_Model_Tile_Picking();

	//ImGui::SameLine();
	//if (ImGui::Button("All Delete", ImVec2(-FLT_MIN, 0)))
	//	Delete_All_Dynamic_Model();
}

void CWindow_Tool_Map::Dynamic_Model_Create_Object(void)
{
	USEGAMEINSTANCE;

	// 선택된 객체의 "Prototype ID"를 ObjectID에 넣음
	ID_UINT	ObjectID;
	ObjectID = m_iSelectedDynamicPrototypeID;

	CGameObject* pEnemyNormal = pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ObjectID, ObjectID, &UNITDESC(ELEMENTAL_DEFAULT, 100.f, 10.f, 1.f));
}

void CWindow_Tool_Map::Dynamic_Model_Tile_Picking()
{
	//메쉬 피킹
	USEGAMEINSTANCE;

	// Tile의 원형들을 가져오기
	GAMEINSTANCE->Get_Prototypes(ID_UINT(MAP, MAP_TERRAIN), &m_pTilePrototypes, 2);

	// 선택된 객체의 "Prototype_ID"을 가져오기
	ID_UINT ID;
	ID = m_iSelectedDynamicPrototypeID;

	// Tag에 <Map, Map_Tile> 있는것들 순회
	for (auto& Prototype : m_pTilePrototypes)
	{
		list<CGameObject*>* plistObject = nullptr;
		ID_UINT	ObjectID;
		ObjectID.iID = Prototype.first;
		plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID);

		if (nullptr == plistObject)
			return;

		if (KEY_PRESSING(VK_CONTROL) && m_bCreate == false)
		{
			// 모델을 선택하지 않고 Tab을 눌렀을 떄 터지는걸 방지
			if (m_iModelNum == 0)
				return;


			if (m_bEffect0 == false)
			{
				// Tab키 누르면 한 번만 생성하게

				// 플레이어는 생성 x
				if (m_iSelectModel == ANIMMODEL_PLAYER)
				{
					m_bEffect0 = true;
					return;
				}
				else
				{
					// 나머지는 생성 O
					Dynamic_Model_Create_Object();
					m_bEffect0 = true;
				}

			}

			// pObject를 순회
			for (auto& pObject : *plistObject)
			{
				// 버퍼 들고오기
				CVIBuffer* pVIBuffer = (CVIBuffer*)pObject->Get_Component(ID_UINT(VIBUFFER));

				// 각 모델을 순회하면서 Transform 던져주기
				CTransform* pTransformCom = pObject->Get_Transform();

				// Picking된 Pos를 받을 vector
				_vector		vPickedPos{ 0.f, 0.f, 0.f, 0.f };

				if (pGameInstance->Picking_Terrain(pVIBuffer, pTransformCom, &vPickedPos))
				{
					// 피킹된 좌표 받기
					_float fMousePosX = XMVectorGetX(vPickedPos);
					_float fMousePosY = XMVectorGetY(vPickedPos);
					_float fMousePosZ = XMVectorGetZ(vPickedPos);

					CTransform* pTarget_TransformCom = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID)->back()->Get_Transform();

					pTarget_TransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(fMousePosX, fMousePosY, fMousePosZ, 1.f));

					if (KEY_PRESSING('Q'))
					{
						if (m_iTurnMode == X_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(0.5f));
						}
						else if (m_iTurnMode == Y_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(0.5f));
						}
						else if (m_iTurnMode == Z_TURN)
						{
							m_fObjectTurnAngle += 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(0.5f));
						}
					}
					if (KEY_PRESSING('E'))
					{
						if (m_iTurnMode == X_TURN)
						{
							m_fObjectTurnAngle -= 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(-0.5f));
						}
						else if (m_iTurnMode == Y_TURN)
						{
							m_fObjectTurnAngle -= 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-0.5f));
						}
						else if (m_iTurnMode == Z_TURN)
						{
							m_fObjectTurnAngle -= 3.f;
							pTarget_TransformCom->Turn(pTarget_TransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(-0.5f));
						}
					}

					if (KEY_DOWN(VK_LBUTTON) && !io.WantCaptureMouse)
					{
						// Tab키 누른 상태에서 왼쪽 클릭을 하면 객체 생성
						m_bCreate = true;
					}
				}
			}
		}
		else if (KEY_UP(VK_CONTROL) && m_bCreate == false)
		{
			// 만약에 객체를 생성하지 않고 Tab키를 때면 객체가 삭제
			// 플레이어는 객체를 삭제하면 안되니까 예외처리
			if (m_iSelectModel == ANIMMODEL_PLAYER)
			{
				m_bEffect0 = false;
				return;
			}
			else
			{
				CGameObject* pObject = nullptr;
				pObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID)->back();
				pGameInstance->Add_DeadObject(pObject);

				m_bEffect0 = false;
			}
		}
		else
		{
			// 원래 상태로 복귀
			m_bCreate = false;
			m_bEffect0 = false;
		}
	}
}

void CWindow_Tool_Map::Show_Created_Dynamic_Objects(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	int iObjectNum = 0;
	list<CGameObject*>* plistObjects = nullptr;
	ID_UINT	ObjectID;
	ObjectID = m_iSelectedDynamicPrototypeID;

	plistObjects = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID); // 프로로타입 ID 를 가진 Layer들에 접근.

	if (nullptr == plistObjects)
		return;

	int iNum = 0;
	string LayerIDStr;
	for (auto& pObject : *plistObjects)
	{
		LayerIDStr = pObject->Get_Component(ID_UINT(MODEL_DYNAMIC))->Get_ModelFileName() + "_" + to_string(iNum++);

		ImGui::SetNextItemOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode((void*)(intptr_t)iObjectNum++, LayerIDStr.c_str()))
		{
			float fObjectPos[3] = { 0.f, 0.f, 0.f };
			float	fHP = 0.f;
			float	fAttack = 0.f;
			float	fSpeed = 0.f;

			CTransform* pTransformCom = pObject->Get_Transform();

			_float3 vPos;
			XMStoreFloat3(&vPos, pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
			//ImGui::Text("Pos.x: %f, Pos.y: %2f, Pos.z: %f", vPos.x, vPos.y, vPos.z);
			fObjectPos[0] = vPos.x;
			fObjectPos[1] = vPos.y;
			fObjectPos[2] = vPos.z;

			ImGui::DragFloat3("Position", fObjectPos, 0.01f);
			pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&XMFLOAT3(fObjectPos[0], fObjectPos[1], fObjectPos[2])), 1.f));

			if (ImGui::Button("Delete"))
			{
				pGameInstance->Add_DeadObject(pObject);
			}

			ImGui::TreePop();
		}
	}
}

void CWindow_Tool_Map::Load_Dynamic_ModelData(void)
{
	USEGAMEINSTANCE;

	json jLoad;
	string str(m_strFileName);

	_uint PreID = 0;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Enemy/" + str).c_str()).c_str(), &jLoad);

	for (auto& ObjectData : jLoad)
	{
		GAMEOBJECT_DESC tInfo;
		tInfo.iObjectID = ObjectData["iObjectID"];
		tInfo.iModelID = ObjectData["iModelID"];

		tInfo.WorldMatrix.r[0] = XMVectorSet(ObjectData["vRight.x"], ObjectData["vRight.y"], ObjectData["vRight.z"], 0.f);
		tInfo.WorldMatrix.r[1] = XMVectorSet(ObjectData["vUp.x"], ObjectData["vUp.y"], ObjectData["vUp.z"], 0.f);
		tInfo.WorldMatrix.r[2] = XMVectorSet(ObjectData["vLook.x"], ObjectData["vLook.y"], ObjectData["vLook.z"], 0.f);
		tInfo.WorldMatrix.r[3] = XMVectorSet(ObjectData["vPosition.x"], ObjectData["vPosition.y"], ObjectData["vPosition.z"], 1.f);

		ID_UINT iID, iModelID;
		iID.iID = tInfo.iObjectID;
		iModelID.iID = tInfo.iModelID;

		pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), iID, iID, &UNITDESC(ELEMENTAL_DEFAULT, 100.f, 10.f, 1.f));
		pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), iID)->back()->Get_Transform()->Set_WorldMatrix(tInfo.WorldMatrix);
	}

	//MSG_BOX_USER("Status", "Load DONE!");
}

#pragma region Terrain

void CWindow_Tool_Map::Terrain_Setting(void)
{
	// Save & Load
	Save_Load_Data();
	ImGui::Separator();

	//Set_Terrain
	const float f32_two = 0.1f;
	const _uint f32_three = 1;

	ImGui::InputScalar("iNumVerticesX", ImGuiDataType_U32, &m_TerrainDesc.iNumVerticesX, &f32_three);
	ImGui::InputScalar("iNumVerticesZ", ImGuiDataType_U32, &m_TerrainDesc.iNumVerticesZ, &f32_three);
	ImGui::InputScalar("fSizeX", ImGuiDataType_Float, &m_TerrainDesc.fSizeX, &f32_two);
	ImGui::InputScalar("fSizeZ", ImGuiDataType_Float, &m_TerrainDesc.fSizeZ, &f32_two);


}

void CWindow_Tool_Map::Terrain_Create_Delete_Setting()
{
	ImVec2 size = ImGui::GetItemRectSize();
	if (ImGui::Button("Create", ImVec2(160.f, 0)))
		Terrain_Create_Object();

	ImGui::SameLine();
	if (ImGui::Button("All Delete", ImVec2(160.f, 0)))
		Terrain_Delete_Object();
}

void CWindow_Tool_Map::Terrain_Create_Object(void)
{
	USEGAMEINSTANCE;
	pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(MAP), ID_UINT(MAP, MAP_TERRAIN), &m_TerrainDesc);
}

void CWindow_Tool_Map::Terrain_Delete_Object(void)
{
	USEGAMEINSTANCE;

	list<CGameObject*>* plistObject = nullptr;
	plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID_UINT(MAP)); // 프로로타입 ID 를 가진 Layer들에 접근.

	if (nullptr == plistObject)
		return;

	// 해당 레이어 태그에 들어있는 오브젝트들 저장.
	for (auto& pObject : *plistObject)
	{
		pGameInstance->Add_DeadObject(pObject);
	}
}

void CWindow_Tool_Map::Terrain_Picking()
{
}

void CWindow_Tool_Map::Save_Terrain_Data(void)
{
}

void CWindow_Tool_Map::Load_Terrain_Data(void)
{
}

void CWindow_Tool_Map::Check_SameCell()
{
	list<CCell*>	TempCells;

	for (size_t i = 0; i < m_Cells.size(); ++i)
	{
		TempCells.push_back(m_Cells[i]);
	}

	for (size_t i = 0; i < m_Cells.size(); ++i)
	{
		for (size_t j = i + 1; j < m_Cells.size(); ++j)
		{
			_bool	bCheck[3] = { false };
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_A), m_Cells[j]->Get_Point(CCell::POINT_A)))
			{
				bCheck[0] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_A), m_Cells[j]->Get_Point(CCell::POINT_B)))
			{
				bCheck[0] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_A), m_Cells[j]->Get_Point(CCell::POINT_C)))
			{
				bCheck[0] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_B), m_Cells[j]->Get_Point(CCell::POINT_A)))
			{
				bCheck[1] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_B), m_Cells[j]->Get_Point(CCell::POINT_B)))
			{
				bCheck[1] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_B), m_Cells[j]->Get_Point(CCell::POINT_C)))
			{
				bCheck[1] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_C), m_Cells[j]->Get_Point(CCell::POINT_A)))
			{
				bCheck[2] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_C), m_Cells[j]->Get_Point(CCell::POINT_B)))
			{
				bCheck[2] = true;
			}
			if (XMVector3Equal(m_Cells[i]->Get_Point(CCell::POINT_C), m_Cells[j]->Get_Point(CCell::POINT_C)))
			{
				bCheck[2] = true;
			}

			if (bCheck[0] && bCheck[1] && bCheck[2])
				TempCells.remove(m_Cells[i]);
		}
	}

	m_Cells.clear();
	for (auto& pCell : TempCells)
	{
		m_Cells.push_back(pCell);
	}
}

#pragma endregion Terrain

void CWindow_Tool_Map::Save_Dynamic_ModelData(void)
{
	USEGAMEINSTANCE;
	list<GAMEOBJECT_DESC> listObjectDesc;

	// m_pPrototypes : Prototype Tag에 "Map"이란 것이 들어가있는 프로토타입.
	for (auto& Prototype : m_pEnemyPrototypes)
	{
		list<CGameObject*>* plistObject = nullptr;
		ID_UINT	ObjectID;
		ObjectID.iID = Prototype.first;
		plistObject = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID); // 프로로타입 ID 를 가진 Layer들에 접근.

		if (nullptr == plistObject)
			continue;

		// 해당 레이어 태그에 들어있는 오브젝트들 저장.
		for (auto& pObject : *plistObject)
		{
			GAMEOBJECT_DESC tInfo;
			tInfo.iObjectID = ObjectID.iID; // (== pObject->Get_PrototypeID());
			tInfo.iModelID = pObject->Get_Component(ID_UINT(MODEL_DYNAMIC))->Get_PrototypeID().iID;
			tInfo.WorldMatrix = pObject->Get_Transform()->Get_WorldMatrix();
			listObjectDesc.push_back(tInfo);
		}
	}

	json jSave;
	for (auto& Object : listObjectDesc)
	{
		json jObjects;
		jObjects.emplace("iObjectID", Object.iObjectID);
		jObjects.emplace("iModelID", Object.iModelID);
		jObjects.emplace("vRight.x", XMVectorGetX(Object.WorldMatrix.r[0]));
		jObjects.emplace("vRight.y", XMVectorGetY(Object.WorldMatrix.r[0]));
		jObjects.emplace("vRight.z", XMVectorGetZ(Object.WorldMatrix.r[0]));
		jObjects.emplace("vUp.x", XMVectorGetX(Object.WorldMatrix.r[1]));
		jObjects.emplace("vUp.y", XMVectorGetY(Object.WorldMatrix.r[1]));
		jObjects.emplace("vUp.z", XMVectorGetZ(Object.WorldMatrix.r[1]));
		jObjects.emplace("vLook.x", XMVectorGetX(Object.WorldMatrix.r[2]));
		jObjects.emplace("vLook.y", XMVectorGetY(Object.WorldMatrix.r[2]));
		jObjects.emplace("vLook.z", XMVectorGetZ(Object.WorldMatrix.r[2]));
		jObjects.emplace("vPosition.x", XMVectorGetX(Object.WorldMatrix.r[3]));
		jObjects.emplace("vPosition.y", XMVectorGetY(Object.WorldMatrix.r[3]));
		jObjects.emplace("vPosition.z", XMVectorGetZ(Object.WorldMatrix.r[3]));

		jSave.push_back(jObjects);
	}
	string str(m_strFileName);
	pGameInstance->Save_Json(pGameInstance->json_Complete_Path(towstring("Enemy/" + str).c_str()).c_str(), jSave);

	//MSG_BOX_USER("Status", "SAVE DONE!");
}

void CWindow_Tool_Map::Setup_ReversePoint(CCell::CELLDESC* pCelldesc)
{
	// A = A
	// B = C
	// C = B

	//XMStoreFloat3(&pCelldesc->vPoints[CCell::POINT_A], pCell->Get_Point(CCell::POINT_A));
	pCelldesc->vPoints[CCell::POINT_B] = pCelldesc->vPoints[CCell::POINT_C];
	pCelldesc->vPoints[CCell::POINT_C] = pCelldesc->vPoints[CCell::POINT_B];


	XMStoreFloat3(&pCelldesc->vLine[CCell::LINE_AB], -XMLoadFloat3(&pCelldesc->vLine[CCell::LINE_CA]));
	XMStoreFloat3(&pCelldesc->vLine[CCell::LINE_BC], -XMLoadFloat3(&pCelldesc->vLine[CCell::LINE_BC]));
	XMStoreFloat3(&pCelldesc->vLine[CCell::LINE_CA], -XMLoadFloat3(&pCelldesc->vLine[CCell::LINE_AB]));

	//XMStoreFloat3(&pCelldesc->vPlaneNormal, -XMLoadFloat3(&pCelldesc->vPlaneNormal));
}

void CWindow_Tool_Map::Free()
{
	Safe_Release(m_pShaderCell);

	Safe_Delete(m_pEffect);
	Safe_Delete(m_pBatch);
	Safe_Release(m_pInputLayout);

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();
}

#endif