#include "stdafx.h"
#include "..\Public\Unit.h"

#include "GameInstance.h"
#include "State_Handler.h"
#include "UI_PlaceMark.h"

CUnit::CUnit()
	: CGameObject()
{
}

CUnit::CUnit(const CUnit& rhs)
	: CGameObject(rhs)
{
}

HRESULT CUnit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUnit::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));


	if (nullptr != pArg)
	{
		m_pInfo = new UNITDESC();
		m_pCurInfo = new UNITDESC();
		memcpy(m_pInfo, pArg, sizeof(UNITDESC));
		memcpy(m_pCurInfo, pArg, sizeof(UNITDESC));
	}

	return S_OK;
}

//void CUnit::Create_UI_Place_Mark(_float fOffsetY)
//{
//#ifdef _UI
//
//	USEGAMEINSTANCE;
//	CGameObject* pGameObject = nullptr;
//
//	pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
//		&CUI::UIDESC(200.f, 200.f, 38.f, 38.f, 0.f, 0, 400, { 0.f,0.f }, this));
//	((CUI_PlaceMark*)pGameObject)->Set_Offset_TargetPosY(fOffsetY);
//	pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
//		&CUI::UIDESC(200.f, 200.f, 43.f, 43.f, 0.f, 1, 400, { 0.f,0.f }, this));
//	((CUI_PlaceMark*)pGameObject)->Set_Offset_TargetPosY(fOffsetY);
//	pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
//		&CUI::UIDESC(200.f, 200.f, 32.f, 32.f, 0.f, 2, 400, { 0.f,0.f }, this));
//	((CUI_PlaceMark*)pGameObject)->Set_Offset_TargetPosY(fOffsetY);
//	pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
//		&CUI::UIDESC(200.f, 200.f, 27.f, 27.f, 0.f, 3, 400, { 0.f,0.f }, this));
//	((CUI_PlaceMark*)pGameObject)->Set_Offset_TargetPosY(fOffsetY);
//
//#endif // _UI
//}

void CUnit::Free()
{
	Safe_Delete(m_pInfo);
	Safe_Delete(m_pCurInfo);

	__super::Free();
}

