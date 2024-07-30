#include "..\Public\Portal.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Navigation.h"
#include "UI_PlaceMark.h"
#include "Quest_Manager.h"

#include "Effect_Base.h"
CPortal::CPortal()
	: CGameObject()
{
}

CPortal::CPortal(const CPortal& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPortal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPortal::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());

	if (nullptr != pArg)
	{
		memcpy(&m_PortalDesc, pArg, sizeof(PORTALDESC));
	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_PortalDesc.m_vPos));

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);


	switch (m_PortalDesc.m_ePortalType)
	{
	case PORTAL_VILLAGE:
	{
		// 플레이어 Look 방향으로 생성되게.
		_vector pPlayerPos = pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER))->Get_Pos();
		_vector vLook = XMVectorSet(138.44f, XMVectorGetY(pPlayerPos), 118.12f, 1.f) - pPlayerPos;
		_float fRadianAngle = acosf(XMVectorGetX(XMVector3Dot(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMVector3Normalize(vLook))));
		if (0 > XMVectorGetY(XMVector3Cross(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMVector3Normalize(vLook))))
			fRadianAngle *= -1.f;
		EFFECT_POS("Portal_Robella", nullptr, XMVectorSet(138.44f, 22.38f, 118.12f, 1.f), XMVectorSet(0.f, fRadianAngle, 0.f, 0.f), 1.f, XMVectorSet(1.f, 1.f, 1.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), &m_listEffect);
	}
	break;

	case PORTAL_DRAGON:
		EFFECT_POS("Portal_Dragon", nullptr, XMVectorSet(148.39f, 87.2f, 457.f, 1.f), XMVectorSet(0.f, XMConvertToRadians(8.4f), 0.f, 0.f));
		break;

	case PORTAL_WEAVER:
		EFFECT_POS("Portal_Weaver", nullptr, XMVectorSet(437.04f, 54.77f, 307.399f, 1.f), XMVectorSet(0.f, XMConvertToRadians(-24.595f), 0.f, 0.f));
		break;

	default:
		break;
	}

	return S_OK;
}

void CPortal::Tick(_float fTimeDelta)
{
	m_pCol_Portal->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_PortalDesc.m_bUseAble)
	{
		SetUp_Teleport();
	}
	//퀘스트있을때 PlaceMark 생성

	switch (m_PortalDesc.m_ePortalType)
	{
	case PORTAL_DRAGON:
		if (QUEST_KILL_JUMPING_FROSTDRAGON == CQuest_Manager::Get_Instance()->Get_CurQuset())
		{
			IMPL_ONCE(m_bCreate_PlaceMark, Create_UI_PlaceMark(5.f));
		}
		break;

	case PORTAL_WEAVER:
		if (QUEST_KILL_WEAVER == CQuest_Manager::Get_Instance()->Get_CurQuset())
		{
			IMPL_ONCE(m_bCreate_PlaceMark, Create_UI_PlaceMark(5.f));
		}
		break;

	default:
		break;
	}

}

void CPortal::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_ColliderGroup(ID_UINT(COL_TRIGGER_PORTAL), m_pCol_Portal);
	pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CPortal::Render()
{
#ifdef _DEBUG

	if (nullptr != m_pCol_Portal)
		m_pCol_Portal->Render();
#endif // _DEBUG
	return S_OK;
}

HRESULT CPortal::SetUp_Components()
{
	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_TRIGGER_PORTAL), (CComponent**)&m_pCol_Portal, &ColliderDesc));
	m_pCol_Portal->Set_Owner(this);

	return S_OK;
}

HRESULT CPortal::SetUp_ShaderResource()
{
	return S_OK;
}

HRESULT CPortal::SetUp_Teleport()
{
	USEGAMEINSTANCE;

	for (auto& iter : *m_pCol_Portal->Get_StayColliders())
	{
		iter->Get_Owner()->Set_Pos(XMLoadFloat4(&m_PortalDesc.m_vTargetPos));
		if (PORTAL_WEAVER == m_PortalDesc.m_ePortalType)
			((CNavigation*)iter->Get_Owner()->Get_Component(ID_UINT(NAVIGATION)))->Set_CurCellDesc(25591); // 웨버
		else
			((CNavigation*)iter->Get_Owner()->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(iter->Get_Owner()->Get_Transform());
		IMPL_ONCE(m_bPooling_PlaceMark,
			Pooling_UI_PlaceMark();
		pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_LOADING_SCREEN), ID_UINT(UI, UI_LOADING_SCREEN),
			&CUI::UIDESC(640.f, 360.f, g_iWinCX, g_iWinCY, 0.f, 21, 5, { 0.f,0.f }, this)););

		if (PORTAL_VILLAGE == m_PortalDesc.m_ePortalType)
		{
			for (auto& iter : m_listEffect)
				iter->Set_EffectOff();
			pGameInstance->Add_DeadObject(this);
		}
	}

	return S_OK;
}

void CPortal::Create_UI_PlaceMark(_float fOffsetY)
{
#ifdef _UI

	USEGAMEINSTANCE;
	CGameObject* pGameObject = nullptr;

	pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
		&CUI::UIDESC(200.f, 200.f, 38.f, 38.f, 0.f, 0, 400, { 0.f,0.f }, this));
	((CUI_PlaceMark*)pGameObject)->Set_Offset_TargetPosY(fOffsetY);
	m_PlaceMarklist.push_back(pGameObject);
	pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
		&CUI::UIDESC(200.f, 200.f, 43.f, 43.f, 0.f, 1, 400, { 0.f,0.f }, this));
	((CUI_PlaceMark*)pGameObject)->Set_Offset_TargetPosY(fOffsetY);
	m_PlaceMarklist.push_back(pGameObject);
	pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
		&CUI::UIDESC(200.f, 200.f, 32.f, 32.f, 0.f, 2, 400, { 0.f,0.f }, this));
	((CUI_PlaceMark*)pGameObject)->Set_Offset_TargetPosY(fOffsetY);
	m_PlaceMarklist.push_back(pGameObject);
	pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
		&CUI::UIDESC(200.f, 200.f, 27.f, 27.f, 0.f, 3, 400, { 0.f,0.f }, this));
	((CUI_PlaceMark*)pGameObject)->Set_Offset_TargetPosY(fOffsetY);
	m_PlaceMarklist.push_back(pGameObject);

#endif // _UI
}

void CPortal::Pooling_UI_PlaceMark()
{
	USEGAMEINSTANCE;

	for (auto& pPlaceMark : m_PlaceMarklist)
	{
		pGameInstance->Add_PoolingObject(pPlaceMark);
	}
	m_PlaceMarklist.clear();
}

CPortal* CPortal::Create()
{
	CPortal* pInstance = new CPortal();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPortal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPortal::Clone(void* pArg)
{
	CPortal* pInstance = new CPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPortal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortal::Free()
{
	__super::Free();
}
