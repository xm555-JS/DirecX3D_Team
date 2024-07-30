#include "stdafx.h"
#include "..\Public\Enemy.h"

#include "GameInstance.h"
#include "State_Handler.h"
#include "UI_Damage_Num.h"
#include "UI_PlaceMark.h"

#include "Imgui_Manager.h"

#include "Navigation.h"


CEnemy::CEnemy()
	: CUnit()
{
}

CEnemy::CEnemy(const CEnemy& rhs)
	: CUnit(rhs)
{
}

HRESULT CEnemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_fDissolveStrength = 0.03f;
	m_fTimeDurationDissolve = 3.f;
	m_vDissolveColor = _float4(1.f, 1.f, 1.f, 1.f);

	return S_OK;
}

void CEnemy::Tick(_float fTimeDelta)
{
	SetUp_NonAttacked(fTimeDelta);

	USEGAMEINSTANCE;

	if (m_bDissolve)
		m_fTimeAccDissolve += fTimeDelta;

	if (1.f != m_fTimeRate)
		m_fTimeRate = 1.f;
}


void CEnemy::Damaged(ELEMENTAL_PROPERTIE eElement, _float fPower)
{
	USEGAMEINSTANCE;
	m_bAttacked = true;
	m_bUseRimLight = true;
	m_fTimeAttacked = 0.f;

	//크리티컬 확률적으로 발생 10이면 발생 데미지 1.5
	_uint iCritical = Random(0, 10);
	_uint iTotalDamage;

	if (10 == iCritical) // 크리티컬 o
	{
		m_fPreHp = m_pCurInfo->fHP;
		m_pCurInfo->fHP -= fPower * 1.5f;
		iTotalDamage = fPower * 1.5f;
	}
	else // 크리티컬 x
	{
		m_fPreHp = m_pCurInfo->fHP;
		m_pCurInfo->fHP -= fPower;
		iTotalDamage = fPower;
	}

	if (0.f > m_pCurInfo->fHP)
	{
		m_pCurInfo->fHP = 0.f;
	}
	pGameInstance->Add_Event(EVENT_HPCHANGE, this, &m_fPreHp);


	// 데미지가 얼마인지.	
	_uint iDamage = 0;
	list<_uint> Damagelist;
	Damagelist.clear();
	while (true)
	{
		iDamage = iTotalDamage % 10;
		Damagelist.push_back(iDamage);

		if (iTotalDamage < 10)
			break;

		iTotalDamage /= 10;
	}

	_uint iIdx = 0;
	_float fDeltaX = fRandom(-80.f, 80.f);
	_float fDeltaY = fRandom(150.f, 210.f);
	Damagelist.reverse();

	for (auto& Num : Damagelist)
	{
		CGameObject* pGameObject = nullptr;
#ifdef _UI
		if (10 == iCritical)
		{
			pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_DAMAGE_NUM), ID_UINT(UI, UI_DAMAGE_NUM),
				&CUI::UIDesc(200.f, 200.f, 200.f, 200.f, 0.f, Num, 300, { 30.f * iIdx + fDeltaX, -fDeltaY }, this, iIdx));
			((CUI_Damage_Num*)pGameObject)->Set_Element(eElement);

			((CUI_Damage_Num*)pGameObject)->Set_Critical(true);
			if (0 == iIdx)
			{
				pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_DAMAGE_NUM), ID_UINT(UI, UI_DAMAGE_NUM),
					&CUI::UIDesc(200.f, 200.f, 200.f, 200.f, 0.f, 11, 301, { -15.f + fDeltaX, -fDeltaY - 10.f }, this, iIdx));
				((CUI_Damage_Num*)pGameObject)->Set_Critical(true);
			}
		}
		else
		{
			pGameObject = pGameInstance->Add_GameObject(m_iCurLevel, ID_UINT(UI, UI_DAMAGE_NUM), ID_UINT(UI, UI_DAMAGE_NUM),
				&CUI::UIDesc(200.f, 200.f, 200.f, 200.f, 0.f, Num, 300, { 20.f * iIdx + fDeltaX, -fDeltaY }, this, iIdx));

			((CUI_Damage_Num*)pGameObject)->Set_Element(eElement);
		}
#endif  

		iIdx++;
	}
}

void CEnemy::On_Dead()
{
	USEGAMEINSTANCE;
	pGameInstance->Add_Event(EVENT_MONSTER_DEAD, this, nullptr);
}

void CEnemy::Create_UI_Place_Mark(_float fOffsetY)
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

void CEnemy::Pooling_UI_PlaceMark()
{
#ifdef _UI
	USEGAMEINSTANCE;
	//list<CGameObject*>* pGameObjectlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_PLACEMARK));
	//for (auto& pPlaceMark : *pGameObjectlist)
	//{
	//	pGameInstance->Add_PoolingObject(pPlaceMark);
	//}
	//pGameObjectlist = pGameInstance->Get_Objects(m_iCurLevel, ID_UINT(UI, UI_DIST_NUM));
	//for (auto& pDist_Num : *pGameObjectlist)
	//{
	//	pGameInstance->Add_PoolingObject(pDist_Num);
	//}
	for (auto& pPlaceMark : m_PlaceMarklist)
	{
		pGameInstance->Add_PoolingObject(pPlaceMark);
	}
	m_PlaceMarklist.clear();
#endif // _UI
}

HRESULT CEnemy::SetUp_ShaderResource()
{
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDissolve", &m_bDissolve, sizeof(_bool)));
	FAILEDCHECK(m_pDissolveTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DissolveNoiseTexture", m_iDissolveTextureNoise));
	FAILEDCHECK(m_pDissolveTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DissolveColorTexture", m_iDissolveTextureColor));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fDissolveStrength", &m_fDissolveStrength, sizeof(_float)));
	_float fTimeAccDissove = 0.f;
	fTimeAccDissove = m_fTimeAccDissolve / m_fTimeDurationDissolve; // 디졸드 지속하고 싶은 시간 만큼 나눠줌.
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fTimeAccDissolve", &fTimeAccDissove, sizeof(_float)));


	_bool	bUseToon = true;
	_bool	bUseOutline = true;
	//_bool	bUseRimLight = true;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bToon", &bUseToon, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bOutline", &bUseOutline, sizeof(_bool)));

	_float	fLineBrightness = 0.583f;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fLineBrightness", &fLineBrightness, sizeof(_float)));

	return S_OK;
}

void CEnemy::SetUp_NonAttacked(_float fTimeDelta)
{
	m_fTimeAttacked += fTimeDelta;

	if (5.f < m_fTimeAttacked)
	{
		m_bAttacked = false;
	}

	if (0.05f < m_fTimeAttacked)
	{
		m_bUseRimLight = false;
	}
}

void CEnemy::Push_EachOther(CCollider* pTargetCol, CCollider* pVolumeCol, CTransform* pTargetTransform, CNavigation* pTargetNavigation)
{
	_float fTargetColRadius = pTargetCol->Get_ColliderDesc()->vScale.x * 0.5f;
	_float fColRadius = pVolumeCol->Get_ColliderDesc()->vScale.x * 0.5f;

	CGameObject* pTarget = pTargetCol->Get_Owner();

	_vector vTargetPos = pTargetCol->Get_Owner()->Get_Pos();
	_vector vPos = pVolumeCol->Get_Owner()->Get_Pos();
	_vector vDir = vPos - vTargetPos;
	_vector vTargetDir = vTargetPos - vPos;

	_float fLength = XMVectorGetX(XMVector3Length(vDir));
	_float fPushLength = (fColRadius + fTargetColRadius - fLength) * 0.5f;

	vDir = XMVector3Normalize(vDir) * fPushLength;
	vTargetDir = XMVector3Normalize(vTargetDir) * fPushLength;

	vPos += vDir;
	vTargetPos += vTargetDir;

	//pTargetTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	//((CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM)))->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);

	pTargetTransform->MoveTo(vPos, pTargetNavigation);
	CNavigation* pTargetNaviCom = dynamic_cast<CNavigation*>(pTarget->Get_Component(ID_UINT(NAVIGATION)));
	((CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM)))->MoveTo(vTargetPos, pTargetNaviCom);
}

void CEnemy::Free()
{
	__super::Free();

}

