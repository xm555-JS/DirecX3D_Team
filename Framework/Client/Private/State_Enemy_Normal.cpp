#include "stdafx.h"
#include "..\Public\State_Enemy_Normal.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Channel.h"
#include "Navigation.h"
#include "Enemy.h"
#include "HIerarchyNode.h"
CState_Enemy_Normal::CState_Enemy_Normal(CState_Manager* pManager, STATE_TYPE eState)
	: CState_AI(pManager, eState)
{
}

CState_Enemy_Normal::CState_Enemy_Normal(const CState_Enemy_Normal& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Enemy_Normal::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Enemy_Normal::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Enemy_Normal::Enter(void* pArg)
{
	m_pOwner = m_pTargetModel->Get_Owner();
	m_pOwnerTranformCom = m_pOwner->Get_Transform();
	m_pTarget = m_pState_Handler->Get_Player();

	if (nullptr != m_pOwner->Get_Component(ID_UINT(NAVIGATION)))
		m_pNaviCom = static_cast<CNavigation*>(m_pOwner->Get_Component(ID_UINT(NAVIGATION)));
}

void CState_Enemy_Normal::Tick(_float fTimeDelta)
{
	STATE_TYPE	eNextState = CState_AI::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pTargetModel->Get_Owner()->Get_Transform()->Reset_RootBoneDiff();

		m_pState_Handler->Change_State(eNextState);

		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, m_strRootBoneName, &m_RootBoneMatrix);
		m_pTargetModel->Set_ModelEasingRatio(0.2f);

		m_pTargetModel->Set_RootEasing(true);

	}
	else
	{
		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, m_strRootBoneName, &m_RootBoneMatrix);
	}

	CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode(m_strRootBoneName.c_str());
	XMStoreFloat4x4(&m_RootBoneMatrix, pHierarchyNode->Get_RealCombinedMatrix() * XMLoadFloat4x4(&m_pTargetModel->Get_TransformMatrix()));

	m_pTargetModel->Get_Owner()->Get_Transform()->Set_RootBoneMatrix(m_RootBoneMatrix, m_pNaviCom);
}

void CState_Enemy_Normal::Exit(void* pArg)
{
}

_bool CState_Enemy_Normal::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

void CState_Enemy_Normal::Set_DamagedAction(_bool* bCheckHit, _float fSpeed)
{
	_float	fOwnerSpeed = ((CUnit*)m_pState_Handler->Get_Owner())->Get_UnitInfo()->fSpeed;
	_float	fRunSpeed = fOwnerSpeed * fSpeed;
	CNavigation* pOwnerNavi = (CNavigation*)m_pState_Handler->Get_Owner()->Get_Component(ID_UINT(NAVIGATION));
	CTransform* pTransform = m_pState_Handler->Get_Owner()->Get_Transform();

	pTransform->Go_Back(1.f, fRunSpeed, pOwnerNavi);

	*bCheckHit = true;
}

void CState_Enemy_Normal::Free()

{
}

#pragma region JoonMake

_float CState_Enemy_Normal::Get_PlayerPosLength()//사용
{
	_vector vPlayerPos = m_pState_Handler->Get_Player()->Get_Pos();
	_vector vOwnerPos = m_pState_Handler->Get_Owner()->Get_Pos();
	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	return fLength;
}

_float CState_Enemy_Normal::Get_StartPosLength(CState_Handler* _pState_Handler)/*몬스터, 생성지점 거리*/
{
	_vector vStartPos = ((CEnemy*)_pState_Handler->Get_Owner())->Get_StartPos();
	_vector vOwnerPos = _pState_Handler->Get_Owner()->Get_Pos();
	_float fLength = XMVectorGetX((XMVector3Length(vStartPos - vOwnerPos)));

	return fLength;
}

_vector CState_Enemy_Normal::Get_LookAtPlayerPos_Dir()//사용
{
	_vector vPlayerPos = m_pState_Handler->Get_Player()->Get_Pos();
	_vector vOwnerPos = m_pState_Handler->Get_Owner()->Get_Pos();

	_vector vDir = XMVector3Normalize(vPlayerPos - vOwnerPos);

	return vDir;
}

_vector CState_Enemy_Normal::Get_LookAtStartPos_Dir()
{
	_vector vStartPos = ((CEnemy*)m_pState_Handler->Get_Owner())->Get_StartPos();
	_vector vOwnerPos = m_pState_Handler->Get_Owner()->Get_Pos();

	_vector vDir = XMVector3Normalize(vStartPos - vOwnerPos);

	return vDir;
}

_uint CState_Enemy_Normal::Get_CurrentKeyFrame()
{
	return m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame();
}

_bool CState_Enemy_Normal::Is_AnimFinished()//사용
{
	return m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished();
}

_bool CState_Enemy_Normal::Is_Damaged()//사용
{
	_float fPreHp = ((CEnemy*)m_pState_Handler->Get_Owner())->Get_PreHp();
	_float fCurrentHp = ((CEnemy*)m_pState_Handler->Get_Owner())->Get_UnitCurInfo()->fHP;
	if (fPreHp > fCurrentHp)
	{
		return true;
	}

	return false;
}

_bool CState_Enemy_Normal::Is_Damaged(CState_Handler* _pState_Handler)//사용
{
	_float fPreHp = ((CEnemy*)_pState_Handler->Get_Owner())->Get_PreHp();
	_float fCurrentHp = ((CEnemy*)_pState_Handler->Get_Owner())->Get_UnitCurInfo()->fHP;
	if (fPreHp > fCurrentHp)
	{
		return true;
	}

	return false;
}

_bool CState_Enemy_Normal::Is_OwnerDead(CState_Handler* _pState_Handler)
{
	if (((CEnemy*)_pState_Handler->Get_Owner())->Get_UnitCurInfo()->fHP <= 0)
		return true;

	return false;
}

void CState_Enemy_Normal::Set_CurrentHp()
{
	_float fPreHp = ((CEnemy*)m_pState_Handler->Get_Owner())->Get_PreHp();
	_float fCurrentHp = ((CEnemy*)m_pState_Handler->Get_Owner())->Get_UnitCurInfo()->fHP;
	((CEnemy*)m_pState_Handler->Get_Owner())->Set_PreHp(fCurrentHp);
}



#pragma endregion JoonMake
