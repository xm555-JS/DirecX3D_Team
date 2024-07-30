#include "stdafx.h"
#include "..\Public\Window_Tool_Animation.h"

#if defined(USE_IMGUI)

#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Imgui_Manager.h"

#include "GameInstance.h"
#include "Component.h"
#include "Model.h"
#include "Animation.h"
#include "HIerarchyNode.h"
#include "Channel.h"

#include "GameObject.h"
#include "AnimObject.h"
#include "Weapon.h"

CWindow_Tool_Animation::CWindow_Tool_Animation()
{
}


CWindow_Tool_Animation::~CWindow_Tool_Animation()
{
}

void CWindow_Tool_Animation::Initialize()
{
	CWindow_Imgui::Initialize_Children();

    GAMEINSTANCE->Add_CallbackList(this, LEVEL_CHANGE); // 레벨 바뀔 때 마다 Prototype 새로 가져오기.
    m_iSelectModel = 0;
}

void CWindow_Tool_Animation::On_EventMessege(EVENTDESC tEvent)
{
    if (LEVEL_CHANGE == tEvent.eEventType)
    {
        if(LEVEL_EDIT_ANIM == GAMEINSTANCE->Get_CurLevelIndex())
        {
            GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER), &m_pPrototypeModels[ANIMMODEL_PLAYER], 2);
            GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_DYNAMIC, MODEL_NPC), &m_pPrototypeModels[ANIMMODEL_NPC], 2);
            GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY), &m_pPrototypeModels[ANIMMODEL_NORMALMON], 2);
            GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS), &m_pPrototypeModels[ANIMMODEL_BOSSMON], 2);
            GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_DYNAMIC, MODEL_BULLET), &m_pPrototypeModels[ANIMMODEL_BULLET], 2);
            GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_DYNAMIC, MODEL_ANIM_WEAPON), &m_pPrototypeModels[ANIMMODEL_WEAPON], 2);
            GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_STATIC, MODEL_WEAPON), &m_pPrototypeModels[NONANIMMODEL_WEAPON], 2);
        }
    }

}

void CWindow_Tool_Animation::Tick(_float fTimeDelta)
{
    USEGAMEINSTANCE;

    ImGui::Begin("Tool Animation", 0);


    if (ImGui::TreeNode("Model_Create"))
    {
        Show_ModelCreate();
        Create_Delete_Setting();

        ImGui::TreePop();
    }

    ImGui::Separator();
    if (ImGui::TreeNode("Model_Show"))
    {
        Show_CreatedObjects();

        ImGui::TreePop();
    }

	ImGui::End();//List Window End

    ImGui::Begin("HierarchyNodes", 0);


    if (ImGui::TreeNode("Model_HierarchyNodes"))
    {
        Show_HierarhcyNodes();

        ImGui::TreePop();
    }

    ImGui::End();//Frame Setting Window End

    ImGui::Begin("Animation_Frame", 0);

    Show_ProgressBar(fTimeDelta);

    ImGui::End();//Frame Setting Window End

    ImGui::Begin("Animation Group", 0); 

    Show_AnimTool();

    ImGui::End(); //AddTool End

    ImGui::Begin("Add Weapon Tool");

    if (ImGui::TreeNode("Weapon Tool"))
    {
        Show_WeaponTool();
        Create_Delete_Weapon();

        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Set Weapon Matrix"))
    {
        Show_CreatedWeapon();
        Set_WeaponMatrix();

        ImGui::TreePop();
    }


    ImGui::End(); //AddTool End

    Input_KeyState(fTimeDelta);


	CWindow_Imgui::Tick_Children(fTimeDelta);
}

void CWindow_Tool_Animation::Show_ModelCreate()
{
    if (ImGui::TreeNode("Model_Type"))
    {
        /*RadioButton*/
        if (ImGui::RadioButton("PLAYER", m_iSelectModel == ANIMMODEL_PLAYER)) { m_iSelectModel = ANIMMODEL_PLAYER; }
        if (ImGui::RadioButton("NPC", m_iSelectModel == ANIMMODEL_NPC)) { m_iSelectModel = ANIMMODEL_NPC; }
        if (ImGui::RadioButton("NORMAL_ENEMY", m_iSelectModel == ANIMMODEL_NORMALMON)) { m_iSelectModel = ANIMMODEL_NORMALMON; }
        if (ImGui::RadioButton("BOSS_ENEMY", m_iSelectModel == ANIMMODEL_BOSSMON)) { m_iSelectModel = ANIMMODEL_BOSSMON; }
        if (ImGui::RadioButton("BULLET", m_iSelectModel == ANIMMODEL_BULLET)) { m_iSelectModel = ANIMMODEL_BULLET; }
        if (ImGui::RadioButton("CAMERA_MOVEMENT", m_iSelectModel == ANIMMODEL_CAMERA)) { m_iSelectModel = ANIMMODEL_CAMERA; }
        if (ImGui::RadioButton("WEAPON", m_iSelectModel == ANIMMODEL_WEAPON)) { m_iSelectModel = ANIMMODEL_WEAPON; }
        // if (ImGui::RadioButton("END", m_iSelectModel == ANIMMODEL_END)) { m_iSelectModel = ANIMMODEL_END; }
        ImGui::TreePop();
    }

    _uint iID = MODEL_PLAYER;

    switch (m_iSelectModel)
    {
    case ANIMMODEL_PLAYER:
        iID = MODEL_PLAYER;
        break;
    case ANIMMODEL_NPC:
        iID = MODEL_NPC;
        break;
    case ANIMMODEL_NORMALMON:
        iID = MODEL_ENEMY;
        break;
    case ANIMMODEL_BOSSMON:
        iID = MODEL_ENEMY_BOSS;
        break;
    case ANIMMODEL_BULLET:
        iID = ANIMMODEL_BULLET;
    case ANIMMODEL_WEAPON:
        iID = MODEL_ANIM_WEAPON;
        break;
    }

   ImGui::Text("Selected Object Models List");
   if (ImGui::BeginListBox("##Models", ImVec2(-FLT_MIN, 6 * ImGui::GetTextLineHeightWithSpacing())))
   {
       FAILEDCHECK(GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_DYNAMIC, iID), &m_pPrototypeModels[m_iSelectModel], 2));

       int n = 1;
       for (auto& Prototype : m_pPrototypeModels[m_iSelectModel])
       {
           const bool is_selected = (m_iModelNum == n);
           if (ImGui::Selectable(Prototype.second->Get_ModelFileName().c_str(), is_selected))
               m_iModelNum = n;

           if (is_selected)
           {
               m_iSelectedComponentPrototype = Prototype;
               ImGui::SetItemDefaultFocus();
           }
           n += 1;
       }

       ImGui::EndListBox();
   }
}

void CWindow_Tool_Animation::Show_CreatedObjects()
{
#define	ANIMNAME(strNum, Name) (strNum + ". " + Name ).c_str()

    USEGAMEINSTANCE;

    int iObjectNum = 0;
    list<CGameObject*>* plistObjects = nullptr;
    ID_UINT	ObjectID;
    ObjectID = ID_UINT(PLAYER);
    plistObjects = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID); // 프로로타입 ID 를 가진 Layer들에 접근.

    if (nullptr == plistObjects)
    {
        m_pAnimObject = nullptr;
        m_pModelCom = nullptr;
        m_pAnimations.clear();
        m_pHierarchyNodes.clear();
        return;
    }

    int iNum = 0;
    string LayerIDStr, AnimName;
    for (auto& pObject : *plistObjects)
    {
        if (nullptr == pObject)
        {
            m_pAnimObject = nullptr;
            m_pModelCom = nullptr;
            m_pAnimations.clear();
            m_pHierarchyNodes.clear();
        }

        LayerIDStr = pObject->Get_Component(ID_UINT(MODEL_DYNAMIC))->Get_ModelFileName() + "_" + to_string(iNum++);

        ImGui::SetNextItemOpen(false, ImGuiCond_Once);
        if (ImGui::TreeNode((void*)(intptr_t)iObjectNum++, LayerIDStr.c_str()))
        {
            m_pAnimObject = static_cast<CAnimObject*>(pObject);
            m_pModelCom = static_cast<CModel*>(pObject->Get_Component(ID_UINT(MODEL_DYNAMIC)));

            //if (CModel::TYPE_ANIM_GROUP == m_pModelCom->Get_ModelType())
            //{
            //    m_eAnimType = Show_AnimGroup_Selector("Anim_Group");
            //    m_eWeaponType = Show_WeaponType_Selector("Weapon_Type");
            //
            //   m_pModelCom->Set_AnimationGroup(m_eAnimType, m_eWeaponType);
            //}

            m_pAnimations = m_pModelCom->Get_Animation();
            m_pHierarchyNodes = m_pModelCom->Get_Hierarchylist();

            int n = 0;
            for (auto& Animation : m_pAnimations)
            {
                AnimName = Animation->Get_AnimName();
                const bool is_selected = (m_iAnimNum == n);
                if (ImGui::Selectable(ANIMNAME(to_string(n), AnimName), is_selected))
                {
                    m_iAnimNum = n;
                }

                if (is_selected)
                {
                    m_pAnimObject->Set_Animation(m_iAnimNum);
                    ImGui::SetItemDefaultFocus();
                }
                n += 1;
            }

            if (ImGui::Button("Delete"))
            {
                pGameInstance->Add_DeadObject(pObject);
                m_pAnimObject = nullptr;
            }

            ImGui::TreePop();
        }
    }
}

void CWindow_Tool_Animation::Show_HierarhcyNodes()
{
#define	ANIMNAME(strNum, Name) (strNum + ". " + Name ).c_str()

    if (nullptr == m_pAnimObject)
        return;

    static int iPassIndex = 0;
    ImGui::RadioButton("Solid Mode", &iPassIndex, 0); ImGui::SameLine();
    ImGui::RadioButton("WireFrame Mode", &iPassIndex, 1);

    m_pAnimObject->Set_PassIndex(iPassIndex);

    int n = 0;
    string HierarchyName;
    for (auto& pHierarchyNode : m_pHierarchyNodes)
    {
        HierarchyName = pHierarchyNode->Get_Name();
        const bool is_selected = (m_iHierNum == n);
        if (ImGui::Selectable(ANIMNAME(to_string(n), HierarchyName), is_selected))
        {
            m_iHierNum = n;
        }

        if (is_selected)
        {
           // m_pAnimObject->Set_Animation(m_iAnimNum);
           // XMStoreFloat4x4(&m_HierCombineMatrix, pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix());
            XMStoreFloat4x4(&m_HierCombineMatrix, pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix() * pHierarchyNode->Get_TransformMatrix());
            m_pAnimObject->Set_BoneMatrix(XMLoadFloat4x4(&m_HierCombineMatrix));
            m_pHierarchyNode = pHierarchyNode;
            ImGui::SetItemDefaultFocus();
        }
        n += 1;
    }
}

void CWindow_Tool_Animation::Show_ProgressBar(_float fTimeDelta)
{
    if (nullptr == m_pAnimObject)
        return;

    _int CurKeyFrame = 0;

    if (ImGui::Button("Play"))
        m_bAnimationPlay = true;
    ImGui::SameLine();
    if (ImGui::Button("Stop"))
        m_bAnimationPlay = false;

    m_pAnimObject->Set_AnimPlay(m_bAnimationPlay);

    ImGui::Separator();

    if (ImGui::TreeNode("Progress Bar"))
    {
        m_fDuration = m_pAnimations[m_iAnimNum]->Get_Duration();
        m_fTickperSecond = m_pAnimations[m_iAnimNum]->Get_TickperSecond();
        float progress = 0.0f, progress_dir = 1.0f;

        if (m_iPreAnimNum != m_iAnimNum)
        {
            progress = 0.0f;
            m_fCurrentTime = 0.f;
        }

        if (m_bAnimationPlay)
        {
            m_fCurrentTime += fTimeDelta * m_fTickperSecond * m_pAnimObject->Get_AnimAccTime();
            progress = m_fCurrentTime / m_fDuration;
            if (m_pAnimations[m_iAnimNum]->Get_IsFinished())
            {
                progress = 0.0f; 
                m_fCurrentTime = 0.f;
            }
            m_iPreAnimNum = m_iAnimNum;
        }

        ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
        //ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        //ImGui::Text("Progress Bar");

        float progress_saturated = IM_CLAMP(progress, 0.0f, 1.0f);
        char buf[32];

        m_pChannels = m_pAnimations[m_iAnimNum]->GetChannel();
        
        _uint NumKeyFrame = 0;
        for (auto& pChannel : m_pChannels)
        {
            if(pChannel->Get_NumKeyframes() > NumKeyFrame)
                NumKeyFrame = pChannel->Get_NumKeyframes();
            CurKeyFrame = pChannel->Get_NumCurrentKeyFrame();
        }

        sprintf_s(buf, "%d/%d", (int)(progress_saturated * NumKeyFrame), NumKeyFrame);
        ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);

        ImGui::Separator();

        static float slider_fPlaySpeed = m_pAnimObject->Get_AnimAccTime();

        if (m_iPreAnimNum != m_iAnimNum)
        {
            progress = 0.0f;
            m_fCurrentTime = 0.f;
        }

        if (m_bAnimationPlay)
        {
            progress = m_fCurrentTime / m_fDuration;
            if (m_pAnimations[m_iAnimNum]->Get_IsFinished())
            {
                progress = 0.0f;
                m_fCurrentTime = 0.f;
            }
            m_iPreAnimNum = m_iAnimNum;
        }

        ImGui::SliderFloat("TickTime ", &m_fCurrentTime, 0.0f, m_fDuration);
        ImGui::SameLine();
        ImGui::Checkbox("MoveTickTime", &m_bMoveTickTime);
        ImGui::SliderInt("FrameKey ", &CurKeyFrame, 0.0f, NumKeyFrame);
        ImGui::SameLine();
        m_bMoveFrameKey = !m_bMoveTickTime;
        ImGui::Checkbox("MoveFrameKey", &m_bMoveFrameKey);
        ImGui::DragFloat("AnimPlaySpeed ", &slider_fPlaySpeed, 0.005f, 0.001f);

        if (!m_bAnimationPlay)
        {
            if (m_bMoveTickTime)
            {
                m_pAnimations[m_iAnimNum]->Set_AccTime(m_fCurrentTime);

                m_pModelCom->Set_AnimMotion();
            }

            if(m_bMoveFrameKey)
            {
                if (CurKeyFrame < (_int)NumKeyFrame)
                {
                    m_pAnimations[m_iAnimNum]->Set_KeyFrame(CurKeyFrame);
                    m_fCurrentTime = m_pAnimations[m_iAnimNum]->Get_AccTime();
                }

                m_pModelCom->Set_AnimMotion();
                //m_pModelCom->Reset_KeyFrame();
            }
        }

        m_pAnimObject->Set_AnimAccTime(slider_fPlaySpeed);

        ImGui::TreePop();
    }
}

void CWindow_Tool_Animation::Show_AnimGroupDesc()
{
}

void CWindow_Tool_Animation::Show_AnimTool()
{
    ImVec2 size = ImGui::GetItemRectSize();
    if (ImGui::Button("Save", ImVec2(size.x * 0.5f, 0)))
        Save_AnimationGroup();

    ImGui::SameLine();
    if (ImGui::Button("Load", ImVec2(-FLT_MIN, 0)))
        Load_AnimationGroup();

    ImGui::Separator();
    Set_AnimationGroup();

    ImGui::Separator();
    Show_AnimationGroup();
}

void CWindow_Tool_Animation::Show_WeaponTool()
{
    ImGui::Text("Selected Object Models List");
    if (ImGui::BeginListBox("##Models", ImVec2(-FLT_MIN, 6 * ImGui::GetTextLineHeightWithSpacing())))
    {
        FAILEDCHECK(GAMEINSTANCE->Get_Component_Prototypes(ID_UINT(MODEL_STATIC, MODEL_WEAPON), &m_pPrototypeModels[NONANIMMODEL_WEAPON], 2));

        int n = 1;
        for (auto& Prototype : m_pPrototypeModels[NONANIMMODEL_WEAPON])
        {
            const bool is_selected = (m_iWeaponNum == n);
            if (ImGui::Selectable(Prototype.second->Get_ModelFileName().c_str(), is_selected))
                m_iWeaponNum = n;

            if (is_selected)
            {
                m_iSelectedWeponPrototype = Prototype;
                ImGui::SetItemDefaultFocus();
            }
            n += 1;
        }

        ImGui::EndListBox();
    }
}

void CWindow_Tool_Animation::Show_CreatedWeapon()
{
#define	ANIMNAME(strNum, Name) (strNum + ". " + Name ).c_str()

    USEGAMEINSTANCE;

    int iObjectNum = 0;
    list<CGameObject*>* plistObjects = nullptr;
    ID_UINT	ObjectID;
    ObjectID = ID_UINT(WEAPON);
    plistObjects = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID); // 프로로타입 ID 를 가진 Layer들에 접근.

    if (nullptr == plistObjects)
    {
        m_pWeapon = nullptr;
        return;
    }

    int iNum = 0;
    string LayerIDStr, AnimName;
    for (auto& pObject : *plistObjects)
    {

        if (nullptr == pObject)
        {
            m_pWeapon = nullptr;
        }

        LayerIDStr = pObject->Get_Component(ID_UINT(MODEL_STATIC))->Get_ModelFileName() + "_" + to_string(iNum++);

        const bool is_selected = (m_iWeaponNum == iNum);
        if (ImGui::Selectable(LayerIDStr.c_str(), is_selected))
        {
            m_iWeaponNum = iNum;
        }

        if (is_selected)
        {
            m_pWeapon = static_cast<CWeapon*>(pObject);
            ImGui::SetItemDefaultFocus();
        }
        iNum += 1;

        if (ImGui::Button("Delete"))
            pGameInstance->Add_DeadObject(pObject);
    }
}

void CWindow_Tool_Animation::Set_WeaponMatrix()
{
    if (nullptr == m_pAnimObject
        || nullptr == m_pWeapon)
        return;

    if (ImGui::Button("Set Matrix"))
        m_pWeapon->Setup_WeaponState(m_pAnimObject, m_pHierarchyNode->Get_Name());
}

void CWindow_Tool_Animation::Create_Delete_Weapon()
{
    ImVec2 size = ImGui::GetItemRectSize();
    if (ImGui::Button("Create", ImVec2(size.x * 0.5f, 0)))
        Create_Weapon();

    ImGui::SameLine();
    if (ImGui::Button("All Delete", ImVec2(-FLT_MIN, 0)))
        Delete_All_Weapon();
}

void CWindow_Tool_Animation::Create_Weapon(void)
{
    USEGAMEINSTANCE;
    if (m_iSelectedWeponPrototype.first && m_iWeaponNum)
    {
        ID_UINT	ObjectID;
        ObjectID.iID = m_iSelectedWeponPrototype.first;

        pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(WEAPON), ID_UINT(WEAPON, WEAPON_TOOL), &m_iSelectedWeponPrototype.first);
    }
}

void CWindow_Tool_Animation::Delete_All_Weapon(void)
{
    USEGAMEINSTANCE;
    // pPrototypes : Prototype Tag에 "Map"이란 것이 들어가있는 프로토타입.
    list<CGameObject*>* plistObjects = nullptr;
    ID_UINT	ObjectID;
    ObjectID = ID_UINT(WEAPON);
    plistObjects = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID); // 프로로타입 ID 를 가진 Layer들에 접근.

    if (nullptr == plistObjects)
        return;

    // 해당 레이어 태그에 들어있는 오브젝트들 저장.
    for (auto& pObject : *plistObjects)
    {
        pGameInstance->Add_DeadObject(pObject);
    }
}

void CWindow_Tool_Animation::Save_AnimationGroup()
{
    char	szFullPath[MAX_PATH] = "";

    strcat_s(szFullPath, "../Bin/Resources/Meshes/Player/");
    strcat_s(szFullPath, "Animation.bin");

    _tchar t2[MAX_PATH];
    swprintf(t2, MAX_PATH, TEXT("%hs"), szFullPath);

    DWORD			dwByte = 0;
    HANDLE			hFile = CreateFile(t2, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (0 == hFile)
    {
        return;
    }

    _uint iNum = m_pAnimationsGroup.size();

    WriteFile(hFile, &iNum, sizeof(_uint), &dwByte, nullptr);

    for (_uint i = 0; i < iNum; ++i)
    {
        char  szName[MAX_PATH];
        strcpy_s(szName, m_pAnimationsGroup[i]->Get_AnimName());
       
        DWORD iLen = (DWORD)strlen(szName) + 1;
        WriteFile(hFile, &iLen, sizeof(DWORD), &dwByte, nullptr);
        WriteFile(hFile, szName, sizeof(char) * iLen, &dwByte, nullptr);

        _uint iNumChannels = m_pAnimationsGroup[i]->GetChannel().size();

        /* 현재 애니메이션을 재상하는데 걸리는 시간. */
        _float fDuration = m_pAnimationsGroup[i]->Get_Duration();
       _float fTickPerSecond = m_pAnimationsGroup[i]->Get_TickperSecond();

        WriteFile(hFile, &iNumChannels, sizeof(_uint), &dwByte, nullptr);
        WriteFile(hFile, &fDuration, sizeof(_float), &dwByte, nullptr);
        WriteFile(hFile, &fTickPerSecond, sizeof(_float), &dwByte, nullptr);

        for (_uint j = 0; j < iNumChannels; ++j)
        {
            char  szChannelName[MAX_PATH];
            strcpy_s(szChannelName, m_pAnimationsGroup[i]->GetChannel()[j]->Get_ChannelName());
            DWORD iLen = (DWORD)strlen(szChannelName) + 1;
            WriteFile(hFile, &iLen, sizeof(DWORD), &dwByte, nullptr);
            WriteFile(hFile, szChannelName, sizeof(char) * iLen, &dwByte, nullptr);

            _uint iNumKeyFrames = m_pAnimationsGroup[i]->GetChannel()[j]->Get_NumKeyframes();

            //WriteFile(hFile, &iHierarchyIndex, sizeof(_uint), dwByte, nullptr);
            WriteFile(hFile, &iNumKeyFrames, sizeof(_uint), &dwByte, nullptr);

            // _float3		vScale;
            // _float4		vRotation;
            // _float3		vPosition;

            for (_uint k = 0; k < iNumKeyFrames; ++k)
            {
                KEYFRAME		KeyFrame;
                ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

                KeyFrame = m_pAnimationsGroup[i]->GetChannel()[j]->Get_KeyFrames()[k];

                WriteFile(hFile, &KeyFrame, sizeof(KEYFRAME), &dwByte, nullptr);
            }
        }
    }

    CloseHandle(hFile);

    return;
}

void CWindow_Tool_Animation::Load_AnimationGroup()
{
    //m_pNodePath;
    char	szFullPath[MAX_PATH] = "";

    strcat_s(szFullPath, "../Bin/Resources/Meshes/Player/");
    strcat_s(szFullPath, "Animation.bin");

    _tchar		szFilePath[MAX_PATH] = TEXT("");

    MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szFilePath, MAX_PATH);

    // 파일 개방 함수
    HANDLE	hFile = CreateFile(szFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        MSG_BOX("CModel - LoadData() : hFile failed");
        return;
    }

    DWORD			dwByte = 0;

    //애니메이션
    _uint iNum;

    ReadFile(hFile, &iNum, sizeof(_uint), &dwByte, nullptr);

    for (_uint i = 0; i < iNum; ++i)
    {
        CAnimation* pAnimation = CAnimation::Create(hFile, &dwByte, m_pModelCom);

        if (nullptr == pAnimation)
            return;

        m_pLoadGroup.push_back(pAnimation);
        m_pAnimationsGroup.push_back(pAnimation);
    }

    CloseHandle(hFile);

    return;
}

void CWindow_Tool_Animation::Set_AnimationGroup()
{
    const _uint    iScalarStep = 1;
    ImGui::InputScalar("Input Index", ImGuiDataType_U32, &m_iSelectGroupNum, &iScalarStep, NULL, "%u");

    ImVec2 size = ImGui::GetItemRectSize();
    if (ImGui::Button("Add_Index", ImVec2(size.x * 0.5f, 0)))
     m_pAnimationsGroup.push_back(m_pAnimations[m_iSelectGroupNum]);
}

void CWindow_Tool_Animation::Show_AnimationGroup()
{
    ImVec2 size = ImGui::GetItemRectSize();
    if (ImGui::Button("Add", ImVec2(size.x * 0.5f, 0)))
        Add_AnimationGroup();

    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(-FLT_MIN, 0)))
        Clear_AnimationGroup();

    ImGui::Separator();

    string LayerIDStr, AnimName;

    int n = 0;
    for (auto& Animation : m_pAnimationsGroup)
    {
        AnimName = Animation->Get_AnimName();
        const bool is_selected = (m_iAnimGroupNum == n);
        if (ImGui::Selectable(ANIMNAME(to_string(n), AnimName), is_selected))
        {
            m_iAnimGroupNum = n;
        }

        if (is_selected)
        {
            ImGui::SetItemDefaultFocus();
        }
        n += 1;
    }

    if (ImGui::Button("Delete"))
        m_pAnimationsGroup.erase(m_pAnimationsGroup.begin() + m_iAnimGroupNum);
}

void CWindow_Tool_Animation::Add_AnimationGroup()
{
    if (nullptr == m_pAnimations[m_iAnimNum])
        return;

    m_pAnimationsGroup.push_back(m_pAnimations[m_iAnimNum]);
}

void CWindow_Tool_Animation::Clear_AnimationGroup()
{
    m_pAnimationsGroup.clear();
}

void CWindow_Tool_Animation::Create_Delete_Setting()
{
    ImVec2 size = ImGui::GetItemRectSize();
    if (ImGui::Button("Create", ImVec2(size.x * 0.5f, 0)))
        Create_Object();

    ImGui::SameLine();
    if (ImGui::Button("All Delete", ImVec2(-FLT_MIN, 0)))
        Delete_All_Object();
}

void CWindow_Tool_Animation::Create_Object(void)
{
    USEGAMEINSTANCE;
    if (m_iSelectedComponentPrototype.first && m_iModelNum)
    {
        ID_UINT	ObjectID;
        ObjectID.iID = m_iSelectedObjectPrototype.first;

        pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(PLAYER), ID_UINT(MODELTEST, PLAYER_TESTOBJ), &m_iSelectedComponentPrototype.first);
    }
}

void CWindow_Tool_Animation::Delete_All_Object(void)
{
    USEGAMEINSTANCE;
    // pPrototypes : Prototype Tag에 "Map"이란 것이 들어가있는 프로토타입.
    list<CGameObject*>* plistObjects = nullptr;
    ID_UINT	ObjectID;
    ObjectID = ID_UINT(PLAYER);
    plistObjects = pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ObjectID); // 프로로타입 ID 를 가진 Layer들에 접근.

    if (nullptr == plistObjects)
        return;

    // 해당 레이어 태그에 들어있는 오브젝트들 저장.
    for (auto& pObject : *plistObjects)
    {
        pGameInstance->Add_DeadObject(pObject);
    }

    m_pAnimObject = nullptr;
}

void CWindow_Tool_Animation::Input_KeyState(_float fTimeDelta)
{
    if (nullptr == m_pAnimObject)
        return;

    USEGAMEINSTANCE;

    if (KEY_PRESSING('Z'))
        m_pAnimObject->Get_Transform()->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), fTimeDelta, 10.f);
    if (KEY_PRESSING('X'))
        m_pAnimObject->Get_Transform()->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), -fTimeDelta, 10.f);
    if (KEY_PRESSING('C'))
        m_pAnimObject->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta, 10.f);
    if (KEY_PRESSING('V'))
        m_pAnimObject->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta, 10.f);
    if (KEY_PRESSING('B'))
        m_pAnimObject->Get_Transform()->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), fTimeDelta, 10.f);
    if (KEY_PRESSING('N'))
        m_pAnimObject->Get_Transform()->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), -fTimeDelta, 10.f);
}

_uint CWindow_Tool_Animation::Show_AnimGroup_Selector(const char* label)
{
    /*
		ANIM_DEFAULT,

		ANIM_PLAYER_FEMALE,
		ANIM_PLAYER_MALE,

		ANIM_END
        */
    const char* items[] = { "ANIM_DEFAULT", "ANIM_PLAYER_FEMALE", "ANIM_PLAYER_MALE", "ANIM_END"};
    static _int Group_current_idx = 0;
    if (ImGui::BeginCombo(label, items[Group_current_idx]))
    {
        for (int n = 0; n < ANIM_END; n++)
        {
            const bool is_selected = (Group_current_idx == n);
            if (ImGui::Selectable(items[n], is_selected))
                Group_current_idx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
            {
                m_eAnimType = Group_current_idx;
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return Group_current_idx;
}

_uint CWindow_Tool_Animation::Show_WeaponType_Selector(const char* label)
{
    /*
		WEAPON_DEFAULT,

		WEAPON_SWORD,
		WEAPON_SPEAR,
		WEAPON_GUN,
		WEAPON_HAMMER,
		WEAPON_IMMORTAL,
		WEAPON_MARK,

        WEAPON_HAND1,
        WEAPON_HAND2,
        WEAPON_HAND3,
        WEAPON_HAND4,
        WEAPON_HAND5,
        WEAPON_HAND6,
        WEAPON_HAND7,
        WEAPON_HAND8,
        WEAPON_HAND9,

		WEAPON_END,
    */
    const char* items[] = { "WEAPON_DEFAULT", "WEAPON_SWORD", "WEAPON_GUN", "WEAPON_SPEAR", "WEAPON_HAMMER", 
        "WEAPON_IMMORTAL", "WEAPON_MARK",
        
        "WEAPON_HAND1",
        "WEAPON_HAND2",
        "WEAPON_HAND3",
        "WEAPON_HAND4",
        "WEAPON_HAND5",
        "WEAPON_HAND6",
        "WEAPON_HAND7",
        "WEAPON_HAND8",
        "WEAPON_HAND9",
        
        "WEAPON_END"};
    static _int Weapon_current_idx = 0;
    if (ImGui::BeginCombo(label, items[Weapon_current_idx]))
    {
        for (int n = 0; n < WEAPON_END; n++)
        {
            const bool is_selected = (Weapon_current_idx == n);
            if (ImGui::Selectable(items[n], is_selected))
                Weapon_current_idx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
            {
                //m_eWeaponType = Weapon_current_idx;
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    
    return Weapon_current_idx;
}

string CWindow_Tool_Animation::GetObjectIDStr(_uint _ID)
{
    string RtnStr = "";

    ID_UINT ID;
    ID.iID = _ID;
    switch (ID.tNums.Num0)
    {
    case PLAYER:
        RtnStr = "PLAYER";
        break;
    case ENEMY:
        RtnStr = "ENEMY";
        break;
    case NPC:
        RtnStr = "NPC";
        break;
    case MODELTEST:
        RtnStr = "ANIMATION_TEST";
        break;
    default:
        RtnStr = "PleaseAddName";
        break;
    }

    return RtnStr;
}

string CWindow_Tool_Animation::GetComponentIDStr(_uint _ID)
{
    return string();
}

string CWindow_Tool_Animation::GetModel_RockIDStr(_uint _ID)
{
    return string();
}

string CWindow_Tool_Animation::GetModel_TileIDStr(_uint _ID)
{
    return string();
}

void CWindow_Tool_Animation::Free()
{
    for (_uint i = 0; i < ANIMMODEL_END; ++i)
        m_pGameObject[i].clear();

    for (_uint i = 0; i < ANIMMODEL_END; ++i)
        m_pPrototypeModels[i].clear();

    for (auto iter : m_pLoadGroup)
    {
        Safe_Release(iter);
    }
    m_pLoadGroup.clear();
    m_pAnimationsGroup.clear();
}

#endif