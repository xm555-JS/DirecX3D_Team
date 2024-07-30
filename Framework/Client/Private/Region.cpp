#include "..\Public\Region.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Navigation.h"
#include "Imgui_Manager.h"
#include "UI_PlaceName.h"
#include "Minigame_Manager.h"
#include "Sound_Manager.h"
CRegion::CRegion()
	: CGameObject()
{
}

CRegion::CRegion(const CRegion& rhs)
	: CGameObject(rhs)
{
}

HRESULT CRegion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRegion::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));
	
	if (nullptr != pArg)
	{
		memcpy(&m_tRegionDesc, pArg, sizeof(REGIONDESC));
	}
	FAILEDCHECK(SetUp_Components());

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_tRegionDesc.vPos));

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	pGameInstance->Add_CallbackList(this, EVENT_LEVIATHAN_APEAR);
	pGameInstance->Add_CallbackList(this, EVENT_LEVIATHAN_DEAD);

	return S_OK;
}

void CRegion::Tick(_float fTimeDelta)
{
	m_pCol_Region->Update(m_pTransformCom->Get_WorldMatrix());

	if (m_bLeviathan_Apear)
		return;

	for (auto& iter : *m_pCol_Region->Get_EnterColliders())
	{

		CUI_PlaceName* pPlaceName = (CUI_PlaceName*)GAMEINSTANCE->Get_Object(m_iCurLevel, ID_UINT(UI, UI_PLACENAME));
		if (pPlaceName)
		{
			pPlaceName->Appear_PlaceName(m_tRegionDesc.eRegionType);
		}

		if (!m_bStartBGM)
		{
			if (m_tRegionDesc.eRegionType == REGION_VILLAGE)
			{
				MGR(CSound_Manager)->ChangeBGM(TEXT("Start_Island.mp3"), 0.3f, 0.1f);
				m_bStartBGM = true;
			}
			else if (m_tRegionDesc.eRegionType == REGION_ROBELLA)
			{
				MGR(CSound_Manager)->ChangeBGM(TEXT("Robella_Island.wav"), 0.3f, 0.1f);
				m_bStartBGM = true;
			}
			else if (m_tRegionDesc.eRegionType == REGION_DRAGON)
			{
				MGR(CSound_Manager)->ChangeBGM(TEXT("Stone_Island.mp3"), 0.3f, 0.1f);
				m_bStartBGM = true;
			}
			else if (m_tRegionDesc.eRegionType == REGION_WEAVER)
			{
				MGR(CSound_Manager)->ChangeBGM(TEXT("City_Island.mp3"), 0.3f, 0.1f);
				m_bStartBGM = true;
			}
		}
		else if (m_bStartBGM)
		{
			if (m_tRegionDesc.eRegionType == REGION_VILLAGE)
			{
				MGR(CSound_Manager)->ChangeBGM(TEXT("Start_Island.mp3"), 0.3f, 0.1f);
			}
			else if (m_tRegionDesc.eRegionType == REGION_ROBELLA)
			{
				MGR(CSound_Manager)->ChangeBGM(TEXT("Robella_Island.wav"), 0.3f, 0.1f);
			}
			else if (m_tRegionDesc.eRegionType == REGION_DRAGON)
			{
				MGR(CSound_Manager)->ChangeBGM(TEXT("Stone_Island.mp3"), 0.3f, 0.1f);
			}
			else if (m_tRegionDesc.eRegionType == REGION_WEAVER)
			{
				MGR(CSound_Manager)->ChangeBGM(TEXT("City_Island.mp3"), 0.3f, 0.1f);
			}


		}

	}
}

void CRegion::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_ColliderGroup(ID_UINT(COL_REGION), m_pCol_Region);
	pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CRegion::Render()
{
#ifdef _DEBUG
	if (nullptr != m_pCol_Region)
		m_pCol_Region->Render();
#else
#ifdef _COLLIDER_RENDER
	if (nullptr != m_pCol_Region)
		m_pCol_Region->Render();
#endif // DEBUG
#endif // _DEBUG
	return S_OK;
}

void CRegion::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_LEVIATHAN_APEAR == tEvent.eEventType)
	{
		m_bLeviathan_Apear = true;
	}
	if (EVENT_LEVIATHAN_DEAD == tEvent.eEventType)
	{
		MGR(CSound_Manager)->ChangeBGM(TEXT("Start_Island.mp3"), 0.3f, 0.1f);
	}
}

HRESULT CRegion::SetUp_Components()
{
	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = m_tRegionDesc.vCol_Scale;
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	if (REGION_VILLAGE == m_tRegionDesc.eRegionType || REGION_DRAGON == m_tRegionDesc.eRegionType)
	{
		FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_REGION), (CComponent**)&m_pCol_Region, &ColliderDesc));
	}
	else
	{
		FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_AABB), ID_UINT(COLLIDER, COLLIDER_AABB, COL_REGION), (CComponent**)&m_pCol_Region, &ColliderDesc));
	}

	m_pCol_Region->Set_Owner(this);
	return S_OK;
}

HRESULT CRegion::SetUp_ShaderResource()
{
	return S_OK;
}

CRegion* CRegion::Create()
{
	CRegion* pInstance = new CRegion();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRegion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRegion::Clone(void* pArg)
{
	CRegion* pInstance = new CRegion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CRegion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRegion::Free()
{
	USEGAMEINSTANCE;
	pGameInstance->Remove_CallbackList(this, EVENT_LEVIATHAN_APEAR);
	pGameInstance->Remove_CallbackList(this, EVENT_LEVIATHAN_DEAD);

	__super::Free();
}
