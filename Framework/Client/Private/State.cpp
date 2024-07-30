#include "stdafx.h"
#include "..\Public\State.h"
#include "Model.h"
#include "State_Manager.h"
#include "State_Handler.h"
#include "Unit.h"
#include "Animation.h"
#include "Channel.h"
#include "Collider.h"
#include "UI.h"
#include "GameInstance.h"

CState::CState(CState_Manager* pState_Manager, STATE_TYPE eState)
	: m_pState_Manager(pState_Manager)
	, m_eState(eState)
{
}

CState::CState(const CState& rhs)
	: m_pState_Manager(rhs.m_pState_Manager)
	, m_ReserveStates(rhs.m_ReserveStates)
	, m_ForceStates(rhs.m_ForceStates)
	, m_eState(rhs.m_eState)
{
}

void CState::Reset_SoundOption()
{
	for (auto& bOnce : m_bSound)
		bOnce = true;

	for (auto& fSound : m_fSound)
		fSound = 0.f;

	for (auto& eLoopCh : m_eSoundChannel)
		eLoopCh = CSound_Manager::CHANNELID::CH_END;
}

void CState::Set_ReserveState(vector<STATE_TYPE>* ReserveState)
{
	m_ReserveStates.clear();
	m_ReserveStates = *ReserveState;
}

void CState::Tick(_float fTimeDelta)
{
	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);

	STATE_TYPE	eNextState = CState::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pState_Handler->Change_State(eNextState);
	}
}

STATE_TYPE CState::Ready_NextState()
{
	for (auto& eState : m_ForceStates)
	{
		if (m_pState_Manager->Get_State(eState)->Check_Condition(m_pState_Handler))
			return eState;
	}

	if (m_bEndCurState)
	{
		for (auto& eState : m_ReserveStates)
		{
			if (m_pState_Manager->Get_State(eState)->Check_Condition(m_pState_Handler))
				return eState;
		}
	}

	return STATE_END;
}

#if 0
void CState::Create_Effect_Positioning(string strEffectName, _float4x4* matParent, _fvector vPos, _fvector vRotation, _float fPlaySpeed, _fvector vScale, _fvector vRevolution)
{
	_matrix OffsetMatrix = XMMatrixScaling(XMVectorGetX(vScale), XMVectorGetY(vScale), XMVectorGetZ(vScale));
	OffsetMatrix *= XMMatrixRotationRollPitchYawFromVector(vRotation);
	OffsetMatrix.r[3] = vPos;
	OffsetMatrix *= XMMatrixRotationRollPitchYawFromVector(vRevolution);

	_matrix ParentMatrix = XMLoadFloat4x4(matParent);

	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	_matrix TargetMatrix = OffsetMatrix * ParentMatrix;
	EFFECT_EXCUTE(strEffectName.c_str(), TargetMatrix, fPlaySpeed);
}


void CState::Create_Effect_Positioning_Rotaion(string strEffectName, _float4x4* matParent, _fvector vPos, _matrix matRotation, _float fPlaySpeed, _fvector vScale, _fvector vRevolution)
{
	_matrix OffsetMatrix = XMMatrixScaling(XMVectorGetX(vScale), XMVectorGetY(vScale), XMVectorGetZ(vScale));
	OffsetMatrix *= matRotation;
	OffsetMatrix.r[3] = vPos;
	OffsetMatrix *= XMMatrixRotationRollPitchYawFromVector(vRevolution);

	_matrix ParentMatrix = XMLoadFloat4x4(matParent);

	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	_matrix TargetMatrix = OffsetMatrix * ParentMatrix;
	EFFECT_EXCUTE(strEffectName.c_str(), TargetMatrix, fPlaySpeed);

}
#endif // 0

_uint CState::Get_CurKeyFrame(_uint iChannelIndex)
{
	_uint iCurKeyFrame = 0;
	iCurKeyFrame = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[iChannelIndex]->Get_NumCurrentKeyFrame();
	return iCurKeyFrame;
}

_bool CState::Get_IsFinished(_uint iChannelIndex)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		return true;
	return false;
}

void CState::Load_UI(wstring strFileName)
{
	USEGAMEINSTANCE;
	json Data;

	wstring strFolder = TEXT("UI/");
	wstring strTemp = strFileName;
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

		CGameObject* pGameObject = pGameInstance->Add_GameObject(m_pState_Handler->Get_Owner()->Get_CurLevel(), IDPrototype, IDPrototype, &UIDesc);
		CUI* pUI = (CUI*)pGameObject;
		pUI->Set_FontText(strTemp);
	}
}

void CState::Set_CurKeyFrame(_uint iKeyFrame)
{
	m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Set_KeyFrame(iKeyFrame);
}

void CState::Free()
{
	__super::Free();
}
	