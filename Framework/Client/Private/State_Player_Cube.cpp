#include "stdafx.h"
#include "..\Public\State_Player_Cube.h"
#include "State_Manager.h"
#include "State_Handler.h"
#include "Model.h"
#include "Animation.h"
#include "HIerarchyNode.h"
#include "Player.h"
#include "GameInstance.h"
#include "Camera_Target_Position.h"
#include "Navigation.h"
#include "UI.h"

#include "Easing_Utillity.h"

CState_Player_Cube::CState_Player_Cube(CState_Manager* pState_Manager, STATE_TYPE eState)
	: CState_Player(pState_Manager, eState)
{
}

CState_Player_Cube::CState_Player_Cube(const CState_Player_Cube& rhs)
	: CState_Player(rhs)
{
}

void CState_Player_Cube::Enter(void* pArg)
{
	__super::Enter(pArg);


}

void CState_Player_Cube::Tick(_float fTimeDelta)
{


}

void CState_Player_Cube::Exit(void* pArg)
{
	__super::Exit(pArg);
}

void CState_Player_Cube::CreateCubeBullet()
{
	((CPlayer*)m_pState_Handler->Get_Player())->Set_AddCubeBullet();
}

void CState_Player_Cube::Free()
{
	__super::Free();
}
