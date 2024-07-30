#include "..\Public\Npc.h"
#include "GameInstance.h"
#include "State_Handler.h"
#include "Quest_Manager.h"
#include "UI_Talk_Window.h"

CNpc::CNpc()
	: CGameObject()
{
}

CNpc::CNpc(const CNpc& rhs)
	: CGameObject(rhs)
{
}

HRESULT CNpc::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_NPC, this);
	pGameInstance->Add_LateTickGroup(LTG_NPC, this);

	return S_OK;
}

void CNpc::Tick(_float fTimeDelta)
{
	m_pState_Handler->Tick(fTimeDelta);
	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
}

void CNpc::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_NPC), m_pCol_Sphere);
	pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CNpc::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

		m_pModelCom->Render(i, m_pShaderCom, 0, "g_Bones");
	}

#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif // _DEBUG

	return S_OK;
}

void CNpc::On_EventMessege(EVENTDESC tEvent)
{
}

void CNpc::Load_UI(string strFileName)
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

HRESULT CNpc::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));

	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_NPC), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CNpc::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	return S_OK;
}

void CNpc::Create_Talk_Window(OBJECT eType, wstring strName, wstring strTalkText)
{
	USEGAMEINSTANCE;

	if (m_bOnce == true)
		return;

	json Data;

	wstring strFolder = TEXT("UI/");
	wstring strTemp = TEXT("");
	if (NPC == eType)
	{
		strTemp = towstring("Talk_Window_Npc");
	}
	else
	{
		strTemp = towstring("Talk_Window_Player");
	}
	wstring strFullName = strFolder + strTemp;

	const wchar_t* szName = strFullName.c_str();
	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(szName).c_str(), &Data);

	_uint iPrototypeID;

	for (auto& elem : Data)
	{
#if defined(_UI)
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

		// CNpc::Create_Talk_Window(OBJECT eType, wstring strName, wstring strTalkText)
		// 대화창UI 생성
		CGameObject* pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, IDPrototype, IDPrototype, &UIDesc);
		CUI* pUI = (CUI*)pGameObject;
		pUI->Set_FontText(strTemp);
		switch (UIDesc.iTextureIdx)
		{
		case 0: // 대화내용
			pUI->Set_Delfault_FontText(strTalkText);
			break;

		case 1: // Npc 이름
			pUI->Set_Delfault_FontText(strName);
			break;

		case 2: // Player 이름
			pUI->Set_Delfault_FontText(strName);
			break;

		}
		m_bOnce = true;
		m_TalkWindowlist.push_back(pUI);
#endif
	}
}

void CNpc::Delete_Talk_Window()
{
	// 대화창 삭제
	for (auto& pUI : m_TalkWindowlist)
	{
		GAMEINSTANCE->Add_DeadObject(pUI);
	}
	m_bOnce = false;
	m_TalkWindowlist.clear();
}


void CNpc::Create_UI_GuideWindow_Talk(wstring strText)
{
	USEGAMEINSTANCE;

#if defined(_UI)
	if (m_GuideWindow_Talklist.size())
		return;

	json Data;
	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(TEXT("UI/GuideWindow_Talk")).c_str(), &Data);
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
		CGameObject* pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, IDPrototype, IDPrototype, &UIDesc);
		CUI* pUI = (CUI*)pGameObject;
		pUI->Set_Delfault_FontText(strText);
		m_GuideWindow_Talklist.push_back(pUI);
	}
#endif
}

void CNpc::Delete_UI_GuideWindow_Talk()
{
	USEGAMEINSTANCE;

	if (m_GuideWindow_Talklist.size() == 0)
		return;

	for (auto& elem : m_GuideWindow_Talklist)
	{
		pGameInstance->Add_PoolingObject(elem);
	}
	m_GuideWindow_Talklist.clear();

}


void CNpc::Free()
{
	__super::Free();
}
