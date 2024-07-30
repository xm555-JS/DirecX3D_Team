#include "stdafx.h"
#include "State_Manager.h"

// For Player
#pragma region Normal
#include "State_Player_Idle.h"
#include "State_Player_Walk.h"
#include "State_Player_Run.h"

#include "State_Player_Jump_A.h"
#include "State_Player_Jump_B.h"
#include "State_Player_Jump_Fall.h"
#include "State_Player_Jump_End.h"

#include "State_Player_Roll_Forword.h"
#include "State_Player_Roll_Forword_Air.h"
#include "State_Player_Roll_Forword_End.h"
#include "State_Player_Roll_Back.h"
#include "State_Player_Roll_Back_Air.h"

#include "State_Player_Hand_Roll_Forword.h"
#include "State_Player_Hand_Roll_Forword_Air.h"
#include "State_Player_Hand_Roll_Forword_End.h"
#include "State_Player_Hand_Roll_Back.h"
#include "State_Player_Hand_Roll_Back_Air.h"

#include "State_Player_Eat.h"

#include "State_Player_Damaged_Low.h"
#include "State_Player_Damaged_Medium.h"
#include "State_Player_Damaged_High_Begin.h"
#include "State_Player_Damaged_High_End.h"

#include "State_Player_Swim_Loop_A.h"
#include "State_Player_Swim_Loop_B.h"
#include "State_Player_Swim_Idle.h"
#include "State_Player_Swim_Dash.h"

#include "State_Player_Skate_Call.h"
#include "State_Player_Skate_Stop_Land.h"
#include "State_Player_Skate_Stop_Water.h"
#include "State_Player_Skate_Run_Hover.h"
#include "State_Player_Skate_Run_Swerve_L.h"
#include "State_Player_Skate_Run_Swerve_R.h"
#include "State_Player_Skate_Jump_Up.h"
#include "State_Player_Skate_Jump_Landing.h"

#include "State_Player_Hand_Idle.h"

#include "State_Player_Glide_End.h"
#include "State_Player_Glide_Stop.h"
#include "State_Player_Glide_Begin_F.h"
#include "State_Player_Glide_Loop_F.h"
#include "State_Player_Glide_Loop.h"
#include "State_Player_Glide_Up.h"
#include "State_Player_Glide_Up_End.h"

#include "State_Player_Mount_Drive.h"
#include "State_Player_Mount_Drive_Air.h"
#include "State_Player_Mount_Free.h"
#include "State_Player_Mount_Idle.h"
#include "State_Player_Mount_TurnL.h"
#include "State_Player_Mount_TurnR.h"
#include "State_Player_Mount_JumpStart.h"
#include "State_Player_Mount_Up.h"
#include "State_Player_Mount_Roll_Base.h"
#include "State_Player_Mount_Roll_Air.h"
#pragma endregion Normal

#pragma region Weapon Base
#include "State_Player_Attack.h"
#include "State_Player_Attack_Air.h"
#include "State_Player_Attack_Base_Charge.h"
#include "State_Player_Attack_Jump_Charge.h"
#include "State_Player_Attack_Roll.h"
#include "State_Player_Skill_Normal.h"
#include "State_Player_Skill_Special.h"
#include "State_Player_Skill_Normal_Air.h"
#include "State_Player_Skill_Special_Air.h"
#pragma endregion Weapon Base

#pragma region Sword
#include "State_Player_Sword_Idle.h"
#include "State_Player_Sword_Attack_A.h"
#include "State_Player_Sword_Attack_B.h"
#include "State_Player_Sword_Attack_C.h"
#include "State_Player_Sword_Attack_D.h"
#include "State_Player_Sword_Attack_E.h"

#include "State_Player_Sword_Attack_Air_A.h"
#include "State_Player_Sword_Attack_Air_B.h"
#include "State_Player_Sword_Attack_Air_C.h"
#include "State_Player_Sword_Attack_Air_D.h"

#include "State_Player_Sword_Attack_Base_Charge.h"
#include "State_Player_Sword_Attack_Jump_Charge_A.h"
#include "State_Player_Sword_Attack_Jump_Charge_B.h"
#include "State_Player_Sword_Attack_Jump_Charge_C.h"
#include "State_Player_Sword_Attack_Roll.h"

#include "State_Player_Sword_Skill_Normal.h"
#include "State_Player_Sword_Skill_Normal_Air.h"

#include "State_Player_Sword_Skill_Special.h"
#include "State_Player_Sword_Skill_Special_A.h"
#include "State_Player_Sword_Skill_Special_B.h"
#include "State_Player_Sword_Skill_Special_C.h"
#include "State_Player_Sword_Skill_Special_D.h"
#include "State_Player_Sword_Skill_Special_E.h"

#include "State_Player_Sword_Roll_Forword.h"
#include "State_Player_Sword_Roll_Forword_Air.h"
#include "State_Player_Sword_Roll_Forword_End.h"
#include "State_Player_Sword_Roll_Back.h"
#include "State_Player_Sword_Roll_Back_Air.h"
#pragma endregion Sword

#pragma region 2Gun
#include "State_Player_2Gun_Idle.h"
#include "State_Player_2Gun_Attack_A.h"
#include "State_Player_2Gun_Attack_B.h"
#include "State_Player_2Gun_Attack_C.h"
#include "State_Player_2Gun_Attack_D.h"
#include "State_Player_2Gun_Attack_E.h"

#include "State_Player_2Gun_Attack_Air_A.h"
#include "State_Player_2Gun_Attack_Air_B.h"
#include "State_Player_2Gun_Attack_Air_C.h"

#include "State_Player_2Gun_Attack_Jump_Charge_A.h"
#include "State_Player_2Gun_Attack_Jump_Charge_B.h"
#include "State_Player_2Gun_Attack_Jump_Charge_C.h"

#include "State_Player_2Gun_Attack_Base_Charge_Zoom_Begin.h"
#include "State_Player_2Gun_Attack_Base_Charge_Zoom.h"

#include "State_Player_2Gun_Attack_Base_Charge_Begin.h"
#include "State_Player_2Gun_Attack_Base_Charge_WalkFront.h"
#include "State_Player_2Gun_Attack_Base_Charge_WalkBack.h"
#include "State_Player_2Gun_Attack_Base_Charge_WalkLeft.h"
#include "State_Player_2Gun_Attack_Base_Charge_WalkRight.h"
#include "State_Player_2Gun_Attack_Base_Charge_Idle.h"

#include "State_Player_2Gun_Attack_Roll.h"

#include "State_Player_2Gun_Skill_Normal.h"
#include "State_Player_2Gun_Skill_Normal_Air.h"
#include "State_Player_2Gun_Skill_Special.h"
#include "State_Player_2Gun_Skill_Special_Air.h"

#include "State_Player_2Gun_Roll_Forword.h"
#include "State_Player_2Gun_Roll_Forword_Air.h"
#include "State_Player_2Gun_Roll_Forword_End.h"
#include "State_Player_2Gun_Roll_Back.h"
#include "State_Player_2Gun_Roll_Back_Air.h"
#pragma endregion 2Gun

#pragma region ShieldAxe
#include "State_Player_Axe_Idle.h"

#include "State_Player_Axe_Attack_A.h"
#include "State_Player_Axe_Attack_B.h"
#include "State_Player_Axe_Attack_C.h"
#include "State_Player_Axe_Attack_D.h"

#include "State_Player_Axe_Attack_Air_A.h"
#include "State_Player_Axe_Attack_Air_B.h"
#include "State_Player_Axe_Attack_Air_C.h"

#include "State_Player_Axe_Attack_Base_Charge.h"

#include "State_Player_Axe_Attack_Jump_Charge_A.h"
#include "State_Player_Axe_Attack_Jump_Charge_B.h"
#include "State_Player_Axe_Attack_Jump_Charge_C.h"

#include "State_Player_Axe_Attack_Roll.h"

#include "State_Player_Axe_Attack_Skill_Air_A.h"
#include "State_Player_Axe_Attack_Skill_Air_B.h"
#include "State_Player_Axe_Attack_Skill_Air_C.h"

#include "State_Player_Axe_Skill_Normal.h"

#include "State_Player_Axe_Roll_Forword.h"
#include "State_Player_Axe_Roll_Forword_Air.h"
#include "State_Player_Axe_Roll_Forword_End.h"
#include "State_Player_Axe_Roll_Back.h"
#include "State_Player_Axe_Roll_Back_Air.h"

#include "State_Player_Shield_Idle.h"

#include "State_Player_Shield_Attack_A.h"
#include "State_Player_Shield_Attack_B.h"
#include "State_Player_Shield_Attack_C.h"
#include "State_Player_Shield_Attack_D.h"

#include "State_Player_Shield_Attack_Air_A.h"
#include "State_Player_Shield_Attack_Air_B.h"
#include "State_Player_Shield_Attack_Air_C.h"

#include "State_Player_Shield_Attack_Base_Charge_A.h"
#include "State_Player_Shield_Attack_Base_Charge_B.h"
#include "State_Player_Shield_Attack_Base_Charge_C.h"

#include "State_Player_Shield_Attack_Jump_Charge_A.h"
#include "State_Player_Shield_Attack_Jump_Charge_B.h"
#include "State_Player_Shield_Attack_Jump_Charge_C.h"

#include "State_Player_Shield_Attack_Roll.h"

#include "State_Player_Shield_Attack_Skill_Air_A.h"
#include "State_Player_Shield_Attack_Skill_Air_B.h"
#include "State_Player_Shield_Attack_Skill_Air_C.h"

#include "State_Player_Shield_Skill_Normal.h"
#include "State_Player_Shield_Skill_Special.h"

#include "State_Player_Shield_Roll_Forword.h"
#include "State_Player_Shield_Roll_Forword_Air.h"
#include "State_Player_Shield_Roll_Forword_End.h"
#include "State_Player_Shield_Roll_Back.h"
#include "State_Player_Shield_Roll_Back_Air.h"
#pragma endregion ShieldAxe

#pragma region Cube
#include "State_Player_Cube_Idle.h"

#include "State_Player_Cube_Attack_A.h"
#include "State_Player_Cube_Attack_B.h"
#include "State_Player_Cube_Attack_C.h"
#include "State_Player_Cube_Attack_D.h"
#include "State_Player_Cube_Attack_E.h"

#include "State_Player_Cube_Attack_Air_A.h"
#include "State_Player_Cube_Attack_Air_B.h"
#include "State_Player_Cube_Attack_Air_C.h"

#include "State_Player_Cube_Attack_Base_Charge.h"

#include "State_Player_Cube_Attack_Jump_Charge_A.h"
#include "State_Player_Cube_Attack_Jump_Charge_B.h"
#include "State_Player_Cube_Attack_Jump_Charge_C.h"

#include "State_Player_Cube_Attack_Skill_Normal.h"
#include "State_Player_Cube_Attack_Skill_Normal_Air.h"


#include "State_Player_Cube_Attack_Skill_Special.h"
#include "State_Player_Cube_Attack_Skill_Special_Air.h"

#include "State_Player_Cube_Roll_Forword.h"
#include "State_Player_Cube_Roll_Forword_Air.h"
#include "State_Player_Cube_Roll_Forword_End.h"
#include "State_Player_Cube_Roll_Back.h"
#include "State_Player_Cube_Roll_Back_Air.h"

#pragma endregion Cube


#pragma region UI
#include "State_Player_UI_Appear.h"
//#include "State_Player_UI_Idle.h"

#include "State_Player_UI_2Gun_Appear.h"
#include "State_Player_UI_2Gun_Idle.h"

#include "State_Player_UI_Cube_Appear.h"
#include "State_Player_UI_Cube_Idle.h"

#include "State_Player_UI_Shield_Appear.h"
#include "State_Player_UI_Shield_Idle.h"

#include "State_Player_UI_Sword_Appear.h"
#include "State_Player_UI_Sword_Idle.h"

#include "State_Player_UI_None_Appear.h"
#include "State_Player_UI_None_Idle.h"

#pragma endregion UI

#pragma region MotorCycle

#include "State_MotorCycle_Idle.h"
#include "State_MotorCycle_Jump_Start.h"
#include "State_MotorCycle_Jump_Loop.h"
#include "State_MotorCycle_Jump_Run.h"
#include "State_MotorCycle_Jump_End.h"
#include "State_MotorCycle_Run.h"

#pragma endregion MotorCycle

#pragma region EnemyNormal
// For Enemy_Mutation
#include "State_Enemy_Mutation_Idle_Normal.h"
#include "State_Enemy_Mutation_Idle_Battle.h"
#include "State_Enemy_Mutation_Walk.h"
#include "State_Enemy_Mutation_Run.h"
#include "State_Enemy_Mutation_Run_Return.h"
#include "State_Enemy_Mutation_Attack_UpHit.h"
#include "State_Enemy_Mutation_Attack_DownHit.h"
#include "State_Enemy_Mutation_Turn_L.h"
#include "State_Enemy_Mutation_Turn_R.h"
#include "State_Enemy_Mutation_Walk_L.h"
#include "State_Enemy_Mutation_Walk_R.h"
#include "State_Enemy_Mutation_Walk_B.h"
#include "State_Enemy_Mutation_Damage_B.h"
#include "State_Enemy_Mutation_Damage_L.h"
#include "State_Enemy_Mutation_Damage_R.h"
#include "State_Enemy_Mutation_Damage_F.h"
#include "State_Enemy_Mutation_Dead_1.h"
#include "State_Enemy_Mutation_Dead_End.h"
#include "State_Enemy_Mutation_Eat.h"
#include "State_Enemy_Mutation_Eat_Find.h"

// For Enemy_Mutation_Mechanic
#include "State_Enemy_Mutation_Mechanic_Idle_Normal.h"
#include "State_Enemy_Mutation_Mechanic_Idle_Battle.h"
#include "State_Enemy_Mutation_Mechanic_Walk.h"
#include "State_Enemy_Mutation_Mechanic_Run.h"
#include "State_Enemy_Mutation_Mechanic_Run_Return.h"
#include "State_Enemy_Mutation_Mechanic_Attack_UpHit.h"
#include "State_Enemy_Mutation_Mechanic_Attack_DownHit.h"
#include "State_Enemy_Mutation_Mechanic_Turn_L.h"
#include "State_Enemy_Mutation_Mechanic_Turn_R.h"
#include "State_Enemy_Mutation_Mechanic_Walk_L.h"
#include "State_Enemy_Mutation_Mechanic_Walk_R.h"
#include "State_Enemy_Mutation_Mechanic_Walk_B.h"
#include "State_Enemy_Mutation_Mechanic_Damage_B.h"
#include "State_Enemy_Mutation_Mechanic_Damage_L.h"
#include "State_Enemy_Mutation_Mechanic_Damage_R.h"
#include "State_Enemy_Mutation_Mechanic_Damage_F.h"
#include "State_Enemy_Mutation_Mechanic_Dead_1.h"
#include "State_Enemy_Mutation_Mechanic_Dead_End.h"

// For Enemy_Habaka_Admiral
#include "State_Enemy_Habaka_Admiral_Idle_Normal.h"
#include "State_Enemy_Habaka_Admiral_Idle_Battle.h"
#include "State_Enemy_Habaka_Admiral_Walk.h"
#include "State_Enemy_Habaka_Admiral_Run.h"
#include "State_Enemy_Habaka_Admiral_Run_Return.h"
#include "State_Enemy_Habaka_Admiral_Turn_L.h"
#include "State_Enemy_Habaka_Admiral_Turn_R.h"
#include "State_Enemy_Habaka_Admiral_Walk_L.h"
#include "State_Enemy_Habaka_Admiral_Walk_R.h"
#include "State_Enemy_Habaka_Admiral_Walk_B.h"
#include "State_Enemy_Habaka_Admiral_Damage_B.h"
#include "State_Enemy_Habaka_Admiral_Damage_L.h"
#include "State_Enemy_Habaka_Admiral_Damage_R.h"
#include "State_Enemy_Habaka_Admiral_Damage_F.h"
#include "State_Enemy_Habaka_Admiral_Dead_1.h"
#include "State_Enemy_Habaka_Admiral_Dead_End.h"

#include "State_Enemy_Habaka_Admiral_Attack_FrontJump.h"
#include "State_Enemy_Habaka_Admiral_Attack_Kick_A.h"


// For Enemy_HyenaGang_Minions
#include "State_Enemy_HyenaGang_Minions_Idle_Normal.h"
#include "State_Enemy_HyenaGang_Minions_Idle_Battle.h"
#include "State_Enemy_HyenaGang_Minions_Walk.h"
#include "State_Enemy_HyenaGang_Minions_Run.h"
#include "State_Enemy_HyenaGang_Minions_Run_Return.h"
#include "State_Enemy_HyenaGang_Minions_Attack_Normal_Shot.h"
#include "State_Enemy_HyenaGang_Minions_Attack_Charge_Shot.h"
#include "State_Enemy_HyenaGang_Minions_Turn_L.h"
#include "State_Enemy_HyenaGang_Minions_Turn_R.h"
#include "State_Enemy_HyenaGang_Minions_Damage_B.h"
#include "State_Enemy_HyenaGang_Minions_Damage_F.h"
#include "State_Enemy_HyenaGang_Minions_Damage_L.h"
#include "State_Enemy_HyenaGang_Minions_Damage_R.h"
#include "State_Enemy_HyenaGang_Minions_Walk_B.h"
#include "State_Enemy_HyenaGang_Minions_Walk_F.h"
#include "State_Enemy_HyenaGang_Minions_Walk_L.h"
#include "State_Enemy_HyenaGang_Minions_Walk_R.h"
#include "State_Enemy_HyenaGang_Minions_Dead_1.h"
#include "State_Enemy_HyenaGang_Minions_Dead_End.h"


// For Enemy_HyenaGang_Shooter
#include "State_Enemy_HyenaGang_Shooter_Idle_Normal.h"
#include "State_Enemy_HyenaGang_Shooter_Idle_Battle.h"
#include "State_Enemy_HyenaGang_Shooter_Walk.h"
#include "State_Enemy_HyenaGang_Shooter_Run.h"
#include "State_Enemy_HyenaGang_Shooter_Run_Return.h"
#include "State_Enemy_HyenaGang_Shooter_Attack_Normal_Shot.h"
#include "State_Enemy_HyenaGang_Shooter_Attack_Charge_Shot.h"
#include "State_Enemy_HyenaGang_Shooter_Turn_L.h"
#include "State_Enemy_HyenaGang_Shooter_Turn_R.h"
#include "State_Enemy_HyenaGang_Shooter_Damage_B.h"
#include "State_Enemy_HyenaGang_Shooter_Damage_F.h"
#include "State_Enemy_HyenaGang_Shooter_Damage_L.h"
#include "State_Enemy_HyenaGang_Shooter_Damage_R.h"
#include "State_Enemy_HyenaGang_Shooter_Walk_B.h"
#include "State_Enemy_HyenaGang_Shooter_Walk_F.h"
#include "State_Enemy_HyenaGang_Shooter_Walk_L.h"
#include "State_Enemy_HyenaGang_Shooter_Walk_R.h"
#include "State_Enemy_HyenaGang_Shooter_Dead_1.h"
#include "State_Enemy_HyenaGang_Shooter_Dead_End.h"

// For Enemy_Gangster_Cap
#include "State_Enemy_Gangster_Cap_Idle_Normal.h"
#include "State_Enemy_Gangster_Cap_Idle_Battle.h"
#include "State_Enemy_Gangster_Cap_Walk.h"
#include "State_Enemy_Gangster_Cap_Run.h"
#include "State_Enemy_Gangster_Cap_Run_Return.h"
#include "State_Enemy_Gangster_Cap_Attack_Swing.h"
#include "State_Enemy_Gangster_Cap_Attack_Dash.h"
#include "State_Enemy_Gangster_Cap_Attack_TurnTurn.h"
#include "State_Enemy_Gangster_Cap_Turn_L.h"
#include "State_Enemy_Gangster_Cap_Turn_R.h"
#include "State_Enemy_Gangster_Cap_Walk_L.h"
#include "State_Enemy_Gangster_Cap_Walk_R.h"
#include "State_Enemy_Gangster_Cap_Walk_B.h"
#include "State_Enemy_Gangster_Cap_Damage_B.h"
#include "State_Enemy_Gangster_Cap_Damage_L.h"
#include "State_Enemy_Gangster_Cap_Damage_R.h"
#include "State_Enemy_Gangster_Cap_Dead_1.h"
#include "State_Enemy_Gangster_Cap_Dead_End.h"

// For Enemy_MechanicalCity_Gunner
#include "State_Enemy_MechanicalCity_Gunner_Idle_Normal.h"
#include "State_Enemy_MechanicalCity_Gunner_Idle_Battle.h"
#include "State_Enemy_MechanicalCity_Gunner_Walk.h"
#include "State_Enemy_MechanicalCity_Gunner_Run.h"
#include "State_Enemy_MechanicalCity_Gunner_Run_Return.h"
#include "State_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin.h"
#include "State_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing.h"
#include "State_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End.h"
#include "State_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb.h"
#include "State_Enemy_MechanicalCity_Gunner_Turn_L.h"
#include "State_Enemy_MechanicalCity_Gunner_Turn_R.h"
#include "State_Enemy_MechanicalCity_Gunner_Walk_L.h"
#include "State_Enemy_MechanicalCity_Gunner_Walk_R.h"
#include "State_Enemy_MechanicalCity_Gunner_Walk_B.h"
#include "State_Enemy_MechanicalCity_Gunner_Damage_B.h"
#include "State_Enemy_MechanicalCity_Gunner_Damage_L.h"
#include "State_Enemy_MechanicalCity_Gunner_Damage_R.h"
#include "State_Enemy_MechanicalCity_Gunner_Damage_F.h"
#include "State_Enemy_MechanicalCity_Gunner_Dead_1.h"
#include "State_Enemy_MechanicalCity_Gunner_Dead_End.h"

// For Enemy_MechanicalCity_SwordGhost
#include "State_Enemy_MechanicalCity_SwordGhost_Idle_Normal.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Idle_Battle.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Walk.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Run.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Run_Return.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Attack_Stand.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Attack_Moving.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Turn_L.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Turn_R.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Walk_L.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Walk_R.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Walk_B.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Damage_B.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Damage_L.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Damage_R.h"
#include "State_Enemy_MechanicalCity_SwordGhost_Dead_End.h"

// For Enemy_MechanicalCity_DoubleEdgedSwordAdept
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Idle_Normal.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Idle_Battle.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Dash.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Swing.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Turn_L.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Turn_R.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk_L.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk_R.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk_B.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Damage_B.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Damage_L.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Damage_R.h"
#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End.h"

// For Enemy_MechanicalCity_DoorKeeper
#include "State_Enemy_MechanicalCity_DoorKeeper_Idle_Normal.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Idle_Battle.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Walk.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Run.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Run_Return.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Attack_Chop.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Turn_L.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Turn_R.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Walk_L.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Walk_R.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Walk_B.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Damage_B.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Damage_B2.h"
#include "State_Enemy_MechanicalCity_DoorKeeper_Dead_End.h"


// For Enemy_HunterDog
//#include "State_Enemy_HunterDog_Idle_Normal.h"
//#include "State_Enemy_HunterDog_Idle_Act_A.h"
//#include "State_Enemy_HunterDog_Idle_Battle.h"
//#include "State_Enemy_HunterDog_Run.h"
//#include "State_Enemy_HunterDog_Run_Return.h"
//#include "State_Enemy_HunterDog_Attack_Bite.h"
//#include "State_Enemy_HunterDog_Attack_Find_Dash_Bite.h"
//#include "State_Enemy_HunterDog_Walk.h"
//#include "State_Enemy_HunterDog_Walk_B.h"
//#include "State_Enemy_HunterDog_Walk_L.h"
//#include "State_Enemy_HunterDog_Walk_R.h"
//#include "State_Enemy_HunterDog_Walk_Turn_L.h"
//#include "State_Enemy_HunterDog_Walk_Turn_R.h"
//#include "State_Enemy_HunterDog_Damage_B.h"
//#include "State_Enemy_HunterDog_Damage_B_Back.h"
//#include "State_Enemy_HunterDog_Damage_L.h"
//#include "State_Enemy_HunterDog_Damage_R.h"
//#include "State_Enemy_HunterDog_Dead_1.h"
//#include "State_Enemy_HunterDog_Dead_End.h"

//////////////////////////////////////////////only_idle
//#include "State_Enemy_Mutation_Mechanic_Idle_Normal.h"
//#include "State_Enemy_Mutation_HunterDog_Idle_Normal.h"
//#include "State_Enemy_Habaka_Admiral_Idle_Normal.h"
//#include "State_Enemy_Mutation_Orc_Idle_Normal.h"
//#include "State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Idle_Normal.h"
//#include "State_Enemy_MechanicalCity_SwordGhost_Idle_Normal.h"
//#include "State_Enemy_MechanicalCity_DoorKeeper_Idle_Normal.h"
#pragma endregion EnemyNormal

#pragma region EnemyBoss
// For Boss
#pragma region Dragon
// For Boss_Dragon
#include "State_Dragon_Patrol.h"
#include "State_Dragon_Patrol_Apear.h"

#include "State_Dragon_Apear.h"
#include "State_Dragon_DoEnd.h"
#include "State_Dragon_RunEnd.h"

#include "State_Dragon_Chase.h"
#include "State_Dragon_Run.h"
				
#include "State_Dragon_Idle.h"
#include "State_Dragon_CameraAction.h"
				
#include "State_Dragon_TurnL.h"
#include "State_Dragon_TurnR.h"

#include "State_Dragon_Attack1.h"
#include "State_Dragon_Attack2.h"
#include "State_Dragon_Attack3.h"
#include "State_Dragon_Attack4.h"
#include "State_Dragon_Attack5.h"
#include "State_Dragon_Attack6.h"
#include "State_Dragon_Attack7.h"
#include "State_Dragon_Attack8.h"
#include "State_Dragon_Attack9.h"

#include "State_Dragon_Ult_Attack1.h"
#include "State_Dragon_Ult_Attack2.h"
#include "State_Dragon_Ult_Attack3.h"

#include "State_Dragon_Fall_Land.h"
#include "State_Dragon_Fall_Loop.h"

#include "State_Dragon_Weak_Begin.h"
#include "State_Dragon_Weak_End.h"
#include "State_Dragon_Weak_Loop.h"

#include "State_Dragon_Dead.h"
#pragma endregion Dragon

#pragma region Weaver
// For Boss Weaver
#include "State_Weaver_Ready.h"
#include "State_Weaver_Apear.h"

#include "State_Weaver_Idle.h"
#include "State_Weaver_CameraAction.h"

#include "State_Weaver_Reset.h"

#include "State_Weaver_Walk_B.h"
#include "State_Weaver_Walk_F.h"
#include "State_Weaver_Walk_L.h"
#include "State_Weaver_Walk_R.h"

#include "State_Weaver_Turn_L.h"
#include "State_Weaver_Turn_R.h"

#include "State_Weaver_Attack_01.h"
#include "State_Weaver_Attack_02.h"
#include "State_Weaver_Attack_03.h"
#include "State_Weaver_Attack_04.h"
#include "State_Weaver_Attack_05_Begin.h"
#include "State_Weaver_Attack_05_End.h"
#include "State_Weaver_Attack_06_Begin.h"
#include "State_Weaver_Attack_06_Break.h"
#include "State_Weaver_Attack_06_Loop.h"
#include "State_Weaver_Attack_06_End.h"
#include "State_Weaver_Attack_07.h"
#include "State_Weaver_Attack_08.h"
#include "State_Weaver_Attack_09.h"
#include "State_Weaver_Attack_10.h"
#include "State_Weaver_Attack_11_Begin.h"
#include "State_Weaver_Attack_11_Loop.h"
#include "State_Weaver_Attack_11_End.h"
#include "State_Weaver_Attack_12.h"

#include "State_Weaver_Shield.h"

#include "State_Weaver_Weak_Begin.h"
#include "State_Weaver_Weak_Loop.h"
#include "State_Weaver_Weak_End.h"

#include "State_Weaver_Dead.h"
#pragma endregion Weaver

#pragma region Robella
// For Boss_Robella
#include "State_Robella_Ready.h"	//Ready
#include "State_Robella_Apear.h"
#include "State_Robella_Dead.h"
#include "State_Robella_CameraAction.h"

#include "State_Robella_1Phase.h"

#include "State_Robella_1Phase_Down.h"
#include "State_Robella_1Phase_Down_Loop.h"
#include "State_Robella_1Phase_Down_Up.h"

#include "State_Robella_Change_Phase.h"

#include "State_Robella_Attack1.h"
#include "State_Robella_Attack2_Begin.h"
#include "State_Robella_Attack2_Loop.h"
#include "State_Robella_Attack2_End.h"
#include "State_Robella_Attack3.h"

#include "State_Robella_2Phase.h"

#include "State_Robella_2Phase_Down.h"
#include "State_Robella_2Phase_Down_Loop.h"
#include "State_Robella_2Phase_Down_Up.h"

#include "State_Robella_Attack4.h"
#include "State_Robella_Attack5.h"
#include "State_Robella_Attack6.h"
#include "State_Robella_Attack7.h"
#include "State_Robella_Attack8.h"
#include "State_Robella_Attack9_Begin.h"
#include "State_Robella_Attack9_End.h"

#include "State_Robella_UltAttack_Begin.h"
#include "State_Robella_UltAttack_End.h"
#pragma endregion Robella

#pragma region Leviathan

#include "State_Leviathan_Apear.h"

#include "State_Leviathan_Idle.h"
#include "State_Leviathan_Walk.h"
#include "State_Leviathan_Weak.h"
#include "State_Leviathan_Jump_TurnL.h"
#include "State_Leviathan_Jump_TurnR.h"

#include "State_Leviathan_Attack_Down_FR.h"
#include "State_Leviathan_Attack_Down_BL_A.h"
#include "State_Leviathan_Attack_Down_BL_B.h"
#include "State_Leviathan_Attack_Down_BL_C.h"
#include "State_Leviathan_Attack_Down_F_A.h"
#include "State_Leviathan_Attack_Down_F_B.h"
#include "State_Leviathan_Attack_Down_F_C.h"
#include "State_Leviathan_Attack_Up_FL.h"
#include "State_Leviathan_Attack_Up_BR.h"
#include "State_Leviathan_Attack_Up_F.h"

#include "State_Leviathan_Roar_0.h"
#include "State_Leviathan_Look_Center_Roar.h"
#include "State_Leviathan_Look_Out_Roar.h"
#include "State_Leviathan_Turn.h"
#include "State_Leviathan_Jump_Turn.h"
#include "State_Leviathan_Dead.h"



#pragma endregion Leviathan

#pragma endregion EnemyBoss

// Npc
#include "State_Shirley_Idle.h"
#include "State_Shirley_Brilliant.h"
#include "State_Shirley_Greeting.h"
#include "State_Shirley_Hungry.h"
#include "State_Shirley_Surprise.h"
#include "State_Shirley_Thank.h"
#include "State_Shirley_Conversation_A.h"
#include "State_Shirley_Conversation_B.h"
#include "State_Shirley_Conversation_C.h"
#include "State_Shirley_Worry_A.h"
#include "State_Shirley_Worry_B.h"

#include "State.h"

IMPLEMENT_SINGLETON(CState_Manager)

CState_Manager::CState_Manager()
{
}


CState* CState_Manager::Clone_State(STATE_TYPE eState, void* pArg)
{
	if (nullptr == m_States[eState])
	{
		cout << eState << " State Index is empty!!" << endl;
		return nullptr;
	}
	

	return m_States[eState]->Clone(pArg);
}

HRESULT CState_Manager::Initialize()
{
	//Default 는 10부터 시작한다.
	ZeroMemory(m_States, sizeof(CState*) * (STATE_END));


	// For Player
#pragma region Player

#pragma region Normal

	m_States[STATE_PLAYER_IDLE] = CState_Player_Idle::Create(this, STATE_PLAYER_IDLE);
	m_States[STATE_PLAYER_HAND_IDLE] = CState_Player_Hand_Idle::Create(this, STATE_PLAYER_HAND_IDLE);
	m_States[STATE_PLAYER_WALK] = CState_Player_Walk::Create(this, STATE_PLAYER_WALK);
	m_States[STATE_PLAYER_RUN] = CState_Player_Run::Create(this, STATE_PLAYER_RUN);

	m_States[STATE_PLAYER_JUMP_A] = CState_Player_Jump_A::Create(this, STATE_PLAYER_JUMP_A);
	m_States[STATE_PLAYER_JUMP_B] = CState_Player_Jump_B::Create(this, STATE_PLAYER_JUMP_B);
	m_States[STATE_PLAYER_JUMP_FALL] = CState_Player_Jump_Fall::Create(this, STATE_PLAYER_JUMP_FALL);
	m_States[STATE_PLAYER_JUMP_END] = CState_Player_Jump_End::Create(this, STATE_PLAYER_JUMP_END);
	
	m_States[STATE_PLAYER_ROLL_FORWORD] = CState_Player_Roll_Forword::Create(this, STATE_PLAYER_ROLL_FORWORD);
	m_States[STATE_PLAYER_ROLL_FORWORD_AIR] = CState_Player_Roll_Forword_Air::Create(this, STATE_PLAYER_ROLL_FORWORD_AIR);
	m_States[STATE_PLAYER_ROLL_FORWORD_END] = CState_Player_Roll_Forword_End::Create(this, STATE_PLAYER_ROLL_FORWORD_END);
	m_States[STATE_PLAYER_ROLL_BACK] = CState_Player_Roll_Back::Create(this, STATE_PLAYER_ROLL_BACK);
	m_States[STATE_PLAYER_ROLL_BACK_AIR] = CState_Player_Roll_Back_Air::Create(this, STATE_PLAYER_ROLL_BACK_AIR);

	m_States[STATE_PLAYER_HAND_ROLL_FORWORD] = CState_Player_Hand_Roll_Forword::Create(this, STATE_PLAYER_HAND_ROLL_FORWORD);
	m_States[STATE_PLAYER_HAND_ROLL_FORWORD_AIR] = CState_Player_Hand_Roll_Forword_Air::Create(this, STATE_PLAYER_HAND_ROLL_FORWORD_AIR);
	m_States[STATE_PLAYER_HAND_ROLL_FORWORD_END] = CState_Player_Hand_Roll_Forword_End::Create(this, STATE_PLAYER_HAND_ROLL_FORWORD_END);
	m_States[STATE_PLAYER_HAND_ROLL_BACK] = CState_Player_Hand_Roll_Back::Create(this, STATE_PLAYER_HAND_ROLL_BACK);
	m_States[STATE_PLAYER_HAND_ROLL_BACK_AIR] = CState_Player_Hand_Roll_Back_Air::Create(this, STATE_PLAYER_HAND_ROLL_BACK_AIR);

	m_States[STATE_PLAYER_EAT] = CState_Player_Eat::Create(this, STATE_PLAYER_EAT);
	
	m_States[STATE_PLAYER_DAMAGED_LOW] = CState_Player_Damaged_Low::Create(this, STATE_PLAYER_DAMAGED_LOW);
	m_States[STATE_PLAYER_DAMAGED_MEDIUM] = CState_Player_Damaged_Medium::Create(this, STATE_PLAYER_DAMAGED_MEDIUM);
	m_States[STATE_PLAYER_DAMAGED_HIGH_BEGIN] = CState_Player_Damaged_High_Begin::Create(this, STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_States[STATE_PLAYER_DAMAGED_HIGH_END] = CState_Player_Damaged_High_End::Create(this, STATE_PLAYER_DAMAGED_HIGH_END);

	m_States[STATE_PLAYER_SWIM_LOOP_A] = CState_Player_Swim_Loop_A::Create(this, STATE_PLAYER_SWIM_LOOP_A);
	m_States[STATE_PLAYER_SWIM_LOOP_B] = CState_Player_Swim_Loop_B::Create(this, STATE_PLAYER_SWIM_LOOP_B);
	m_States[STATE_PLAYER_SWIM_IDLE] = CState_Player_Swim_Idle::Create(this, STATE_PLAYER_SWIM_IDLE);
	m_States[STATE_PLAYER_SWIM_DASH] = CState_Player_Swim_Dash::Create(this, STATE_PLAYER_SWIM_DASH);

	m_States[STATE_PLAYER_SKATE_CALL] = CState_Player_Skate_Call::Create(this, STATE_PLAYER_SKATE_CALL);
	m_States[STATE_PLAYER_SKATE_STOP_LAND] = CState_Player_Skate_Stop_Land::Create(this, STATE_PLAYER_SKATE_STOP_LAND);
	m_States[STATE_PLAYER_SKATE_STOP_WATER] = CState_Player_Skate_Stop_Water::Create(this, STATE_PLAYER_SKATE_STOP_WATER);
	m_States[STATE_PLAYER_SKATE_RUN] = CState_Player_Skate_Run_Hover::Create(this, STATE_PLAYER_SKATE_RUN);
	m_States[STATE_PLAYER_SKATE_RUN_SWERVE_L] = CState_Player_Skate_Run_Swerve_L::Create(this, STATE_PLAYER_SKATE_RUN_SWERVE_L);
	m_States[STATE_PLAYER_SKATE_RUN_SWERVE_R] = CState_Player_Skate_Run_Swerve_R::Create(this, STATE_PLAYER_SKATE_RUN_SWERVE_R);
	m_States[STATE_PLAYER_SKATE_JUMP] = CState_Player_Skate_Jump_Up::Create(this, STATE_PLAYER_SKATE_JUMP);
	m_States[STATE_PLAYER_SKATE_LANDING] = CState_Player_Skate_Jump_Landing::Create(this, STATE_PLAYER_SKATE_LANDING);

	m_States[STATE_PLAYER_GLIDE_END] = CState_Player_Glide_End::Create(this, STATE_PLAYER_GLIDE_END);
	m_States[STATE_PLAYER_GLIDE_STOP] = CState_Player_Glide_Stop::Create(this, STATE_PLAYER_GLIDE_STOP);
	m_States[STATE_PLAYER_GLIDE_BEGIN_F] = CState_Player_Glide_Begin_F::Create(this, STATE_PLAYER_GLIDE_BEGIN_F);
	m_States[STATE_PLAYER_GLIDE_LOOP_F] = CState_Player_Glide_Loop_F::Create(this, STATE_PLAYER_GLIDE_LOOP_F);
	m_States[STATE_PLAYER_GLIDE_LOOP] = CState_Player_Glide_Loop::Create(this, STATE_PLAYER_GLIDE_LOOP);
	m_States[STATE_PLAYER_GLIDE_UP] = CState_Player_Glide_Up::Create(this, STATE_PLAYER_GLIDE_UP);
	m_States[STATE_PLAYER_GLIDE_UP_END] = CState_Player_Glide_Up_End::Create(this, STATE_PLAYER_GLIDE_UP_END);

	m_States[STATE_PLAYER_MOUNT_UP] = CState_Player_Mount_Up::Create(this, STATE_PLAYER_MOUNT_UP);
	m_States[STATE_PLAYER_MOUNT_DRIVE] = CState_Player_Mount_Drive::Create(this, STATE_PLAYER_MOUNT_DRIVE);
	m_States[STATE_PLAYER_MOUNT_DRIVE_AIR] = CState_Player_Mount_Drive_Air::Create(this, STATE_PLAYER_MOUNT_DRIVE_AIR);
	m_States[STATE_PLAYER_MOUNT_FREE] = CState_Player_Mount_Free::Create(this, STATE_PLAYER_MOUNT_FREE);
	m_States[STATE_PLAYER_MOUNT_IDLE] = CState_Player_Mount_Idle::Create(this, STATE_PLAYER_MOUNT_IDLE);
	m_States[STATE_PLAYER_MOUNT_JUMP_START] = CState_Player_Mount_JumpStart::Create(this, STATE_PLAYER_MOUNT_JUMP_START);
	m_States[STATE_PLAYER_MOUNT_TURN_L] = CState_Player_Mount_TurnL::Create(this, STATE_PLAYER_MOUNT_TURN_L);
	m_States[STATE_PLAYER_MOUNT_TURN_R] = CState_Player_Mount_TurnR::Create(this, STATE_PLAYER_MOUNT_TURN_R);
	m_States[STATE_PLAYER_MOUNT_ROLL] = CState_Player_Mount_Roll_Base::Create(this, STATE_PLAYER_MOUNT_ROLL);
	m_States[STATE_PLAYER_MOUNT_ROLL_AIR] = CState_Player_Mount_Roll_Air::Create(this, STATE_PLAYER_MOUNT_ROLL_AIR);
#pragma endregion Normal

#pragma region Weapon Base
	m_States[STATE_PLAYER_ATTACK] = CState_Player_Attack::Create(this, STATE_PLAYER_ATTACK);
	m_States[STATE_PLAYER_ATTACK_AIR] = CState_Player_Attack_Air::Create(this, STATE_PLAYER_ATTACK_AIR);
	m_States[STATE_PLAYER_ATTACK_BASE_CHARGE] = CState_Player_Attack_Base_Charge::Create(this, STATE_PLAYER_ATTACK_BASE_CHARGE);
	m_States[STATE_PLAYER_ATTACK_JUMP_CHARGE] = CState_Player_Attack_Jump_Charge::Create(this, STATE_PLAYER_ATTACK_JUMP_CHARGE);
	m_States[STATE_PLAYER_ATTACK_ROLL] = CState_Player_Attack_Roll::Create(this, STATE_PLAYER_ATTACK_ROLL);
	m_States[STATE_PLAYER_SKILL_NORMAL] = CState_Player_Skill_Normal::Create(this, STATE_PLAYER_SKILL_NORMAL);
	m_States[STATE_PLAYER_SKILL_SPECIAL] = CState_Player_Skill_Special::Create(this, STATE_PLAYER_SKILL_SPECIAL);
	m_States[STATE_PLAYER_SKILL_NORMAL_AIR] = CState_Player_Skill_Normal_Air::Create(this, STATE_PLAYER_SKILL_NORMAL_AIR);
	m_States[STATE_PLAYER_SKILL_SPECIAL_AIR] = CState_Player_Skill_Special_Air::Create(this, STATE_PLAYER_SKILL_SPECIAL_AIR);
#pragma endregion Weapon Base

#pragma region Sword
	m_States[STATE_PLAYER_SWORD_IDLE] = CState_Player_Sword_Idle::Create(this, STATE_PLAYER_SWORD_IDLE);

	m_States[STATE_PLAYER_SWORD_ATTACK_A] = CState_Player_Sword_Attack_A::Create(this, STATE_PLAYER_SWORD_ATTACK_A);
	m_States[STATE_PLAYER_SWORD_ATTACK_B] = CState_Player_Sword_Attack_B::Create(this, STATE_PLAYER_SWORD_ATTACK_B);
	m_States[STATE_PLAYER_SWORD_ATTACK_C] = CState_Player_Sword_Attack_C::Create(this, STATE_PLAYER_SWORD_ATTACK_C);
	m_States[STATE_PLAYER_SWORD_ATTACK_D] = CState_Player_Sword_Attack_D::Create(this, STATE_PLAYER_SWORD_ATTACK_D);
	m_States[STATE_PLAYER_SWORD_ATTACK_E] = CState_Player_Sword_Attack_E::Create(this, STATE_PLAYER_SWORD_ATTACK_E);

	m_States[STATE_PLAYER_SWORD_ATTACK_AIR_A] = CState_Player_Sword_Attack_Air_A::Create(this, STATE_PLAYER_SWORD_ATTACK_AIR_A);
	m_States[STATE_PLAYER_SWORD_ATTACK_AIR_B] = CState_Player_Sword_Attack_Air_B::Create(this, STATE_PLAYER_SWORD_ATTACK_AIR_B);
	m_States[STATE_PLAYER_SWORD_ATTACK_AIR_C] = CState_Player_Sword_Attack_Air_C::Create(this, STATE_PLAYER_SWORD_ATTACK_AIR_C);
	m_States[STATE_PLAYER_SWORD_ATTACK_AIR_D] = CState_Player_Sword_Attack_Air_D::Create(this, STATE_PLAYER_SWORD_ATTACK_AIR_D);

	m_States[STATE_PLAYER_SWORD_ATTACK_BASE_CHARGE] = CState_Player_Sword_Attack_Base_Charge::Create(this, STATE_PLAYER_SWORD_ATTACK_BASE_CHARGE);
	m_States[STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_A] = CState_Player_Sword_Attack_Jump_Charge_A::Create(this, STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_A);
	m_States[STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_B] = CState_Player_Sword_Attack_Jump_Charge_B::Create(this, STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_B);
	m_States[STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_C] = CState_Player_Sword_Attack_Jump_Charge_C::Create(this, STATE_PLAYER_SWORD_ATTACK_JUMP_CHARGE_C);
	m_States[STATE_PLAYER_SWORD_ATTACK_ROLL] = CState_Player_Sword_Attack_Roll::Create(this, STATE_PLAYER_SWORD_ATTACK_ROLL);

	m_States[STATE_PLAYER_SWORD_SKILL_NORMAL] = CState_Player_Sword_Skill_Normal::Create(this, STATE_PLAYER_SWORD_SKILL_NORMAL);
	m_States[STATE_PLAYER_SWORD_SKILL_NORMAL_AIR] = CState_Player_Sword_Skill_Normal_Air::Create(this, STATE_PLAYER_SWORD_SKILL_NORMAL_AIR);

	m_States[STATE_PLAYER_SWORD_SKILL_SPECIAL] = CState_Player_Sword_Skill_Special::Create(this, STATE_PLAYER_SWORD_SKILL_SPECIAL);
	m_States[STATE_PLAYER_SWORD_SKILL_SPECIAL_A] = CState_Player_Sword_Skill_Special_A::Create(this, STATE_PLAYER_SWORD_SKILL_SPECIAL_A);
	m_States[STATE_PLAYER_SWORD_SKILL_SPECIAL_B] = CState_Player_Sword_Skill_Special_B::Create(this, STATE_PLAYER_SWORD_SKILL_SPECIAL_B);
	m_States[STATE_PLAYER_SWORD_SKILL_SPECIAL_C] = CState_Player_Sword_Skill_Special_C::Create(this, STATE_PLAYER_SWORD_SKILL_SPECIAL_C);
	m_States[STATE_PLAYER_SWORD_SKILL_SPECIAL_D] = CState_Player_Sword_Skill_Special_D::Create(this, STATE_PLAYER_SWORD_SKILL_SPECIAL_D);
	m_States[STATE_PLAYER_SWORD_SKILL_SPECIAL_E] = CState_Player_Sword_Skill_Special_E::Create(this, STATE_PLAYER_SWORD_SKILL_SPECIAL_E);

	m_States[STATE_PLAYER_SWORD_ROLL_FORWORD] = CState_Player_Sword_Roll_Forword::Create(this, STATE_PLAYER_SWORD_ROLL_FORWORD);
	m_States[STATE_PLAYER_SWORD_ROLL_FORWORD_AIR] = CState_Player_Sword_Roll_Forword_Air::Create(this, STATE_PLAYER_SWORD_ROLL_FORWORD_AIR);
	m_States[STATE_PLAYER_SWORD_ROLL_FORWORD_END] = CState_Player_Sword_Roll_Forword_End::Create(this, STATE_PLAYER_SWORD_ROLL_FORWORD_END);
	m_States[STATE_PLAYER_SWORD_ROLL_BACK] = CState_Player_Sword_Roll_Back::Create(this, STATE_PLAYER_SWORD_ROLL_BACK);
	m_States[STATE_PLAYER_SWORD_ROLL_BACK_AIR] = CState_Player_Sword_Roll_Back_Air::Create(this, STATE_PLAYER_SWORD_ROLL_BACK_AIR);
#pragma endregion Sword

#pragma region 2Gun
	m_States[STATE_PLAYER_2GUN_IDLE] = CState_Player_2Gun_Idle::Create(this, STATE_PLAYER_2GUN_IDLE);

	m_States[STATE_PLAYER_2GUN_ATTACK_A] = CState_Player_2Gun_Attack_A::Create(this, STATE_PLAYER_2GUN_ATTACK_A);
	m_States[STATE_PLAYER_2GUN_ATTACK_B] = CState_Player_2Gun_Attack_B::Create(this, STATE_PLAYER_2GUN_ATTACK_B);
	m_States[STATE_PLAYER_2GUN_ATTACK_C] = CState_Player_2Gun_Attack_C::Create(this, STATE_PLAYER_2GUN_ATTACK_C);
	m_States[STATE_PLAYER_2GUN_ATTACK_D] = CState_Player_2Gun_Attack_D::Create(this, STATE_PLAYER_2GUN_ATTACK_D);
	m_States[STATE_PLAYER_2GUN_ATTACK_E] = CState_Player_2Gun_Attack_E::Create(this, STATE_PLAYER_2GUN_ATTACK_E);

	m_States[STATE_PLAYER_2GUN_ATTACK_AIR_A] = CState_Player_2Gun_Attack_Air_A::Create(this, STATE_PLAYER_2GUN_ATTACK_AIR_A);
	m_States[STATE_PLAYER_2GUN_ATTACK_AIR_B] = CState_Player_2Gun_Attack_Air_B::Create(this, STATE_PLAYER_2GUN_ATTACK_AIR_B);
	m_States[STATE_PLAYER_2GUN_ATTACK_AIR_C] = CState_Player_2Gun_Attack_Air_C::Create(this, STATE_PLAYER_2GUN_ATTACK_AIR_C);

	m_States[STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_A] = CState_Player_2Gun_Attack_Jump_Charge_A::Create(this, STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_A);
	m_States[STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_B] = CState_Player_2Gun_Attack_Jump_Charge_B::Create(this, STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_B);
	m_States[STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_C] = CState_Player_2Gun_Attack_Jump_Charge_C::Create(this, STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_C);
	
	m_States[STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_ZOOM_BEGIN] = CState_Player_2Gun_Attack_Base_Charge_Zoom_Begin::Create(this, STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_ZOOM_BEGIN);
	m_States[STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_ZOOM] = CState_Player_2Gun_Attack_Base_Charge_Zoom::Create(this, STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_ZOOM);
	
	m_States[STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_BEGIN] = CState_Player_2Gun_Attack_Base_Charge_Begin::Create(this, STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_BEGIN);
	m_States[STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKFRONT] = CState_Player_2Gun_Attack_Base_Charge_WalkFront::Create(this, STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKFRONT);
	m_States[STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKBACK] = CState_Player_2Gun_Attack_Base_Charge_WalkBack::Create(this, STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKBACK);
	m_States[STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKLEFT] = CState_Player_2Gun_Attack_Base_Charge_WalkLeft::Create(this, STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKLEFT);
	m_States[STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKRIGHT] = CState_Player_2Gun_Attack_Base_Charge_WalkRight::Create(this, STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_WALKRIGHT);
	m_States[STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_IDLE] = CState_Player_2Gun_Attack_Base_Charge_Idle ::Create(this, STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_IDLE);

	m_States[STATE_PLAYER_2GUN_ATTACK_ROLL] = CState_Player_2Gun_Attack_Roll::Create(this, STATE_PLAYER_2GUN_ATTACK_ROLL);

	m_States[STATE_PLAYER_2GUN_SKILL_NORMAL] = CState_Player_2Gun_Skill_Normal::Create(this, STATE_PLAYER_2GUN_SKILL_NORMAL);
	m_States[STATE_PLAYER_2GUN_SKILL_NORMAL_AIR] = CState_Player_2Gun_Skill_Normal_Air::Create(this, STATE_PLAYER_2GUN_SKILL_NORMAL_AIR);
	m_States[STATE_PLAYER_2GUN_SKILL_SPECIAL] = CState_Player_2Gun_Skill_Special::Create(this, STATE_PLAYER_2GUN_SKILL_SPECIAL);
	m_States[STATE_PLAYER_2GUN_SKILL_SPECIAL_AIR] = CState_Player_2Gun_Skill_Special_Air::Create(this, STATE_PLAYER_2GUN_SKILL_SPECIAL_AIR);

	m_States[STATE_PLAYER_2GUN_ROLL_FORWORD] = CState_Player_2Gun_Roll_Forword::Create(this, STATE_PLAYER_2GUN_ROLL_FORWORD);
	m_States[STATE_PLAYER_2GUN_ROLL_FORWORD_AIR] = CState_Player_2Gun_Roll_Forword_Air::Create(this, STATE_PLAYER_2GUN_ROLL_FORWORD_AIR);
	m_States[STATE_PLAYER_2GUN_ROLL_FORWORD_END] = CState_Player_2Gun_Roll_Forword_End::Create(this, STATE_PLAYER_2GUN_ROLL_FORWORD_END);
	m_States[STATE_PLAYER_2GUN_ROLL_BACK] = CState_Player_2Gun_Roll_Back::Create(this, STATE_PLAYER_2GUN_ROLL_BACK);
	m_States[STATE_PLAYER_2GUN_ROLL_BACK_AIR] = CState_Player_2Gun_Roll_Back_Air::Create(this, STATE_PLAYER_2GUN_ROLL_BACK_AIR);

	//m_States[STATE_PLAYER_ATTACK_BASE_CHARGE] = CState_Player_Sword_Attack_Base_Charge::Create(this, STATE_PLAYER_ATTACK_BASE_CHARGE);
	//m_States[STATE_PLAYER_ATTACK_JUMP_CHARGE] = CState_Player_Sword_Attack_Jump_Charge::Create(this, STATE_PLAYER_ATTACK_JUMP_CHARGE);
	//m_States[STATE_PLAYER_ATTACK_ROLL] = CState_Player_Sword_Attack_Roll::Create(this, STATE_PLAYER_ATTACK_ROLL);
#pragma endregion 2Gun

#pragma region ShieldAxe
	m_States[STATE_PLAYER_SHIELD_IDLE] = CState_Player_Shield_Idle::Create(this, STATE_PLAYER_SHIELD_IDLE);
	m_States[STATE_PLAYER_AXE_IDLE] = CState_Player_Axe_Idle::Create(this, STATE_PLAYER_AXE_IDLE);

	m_States[STATE_PLAYER_SHIELD_ATTACK_A] = CState_Player_Shield_Attack_A::Create(this, STATE_PLAYER_SHIELD_ATTACK_A);
	m_States[STATE_PLAYER_SHIELD_ATTACK_B] = CState_Player_Shield_Attack_B::Create(this, STATE_PLAYER_SHIELD_ATTACK_B);
	m_States[STATE_PLAYER_SHIELD_ATTACK_C] = CState_Player_Shield_Attack_C::Create(this, STATE_PLAYER_SHIELD_ATTACK_C);
	m_States[STATE_PLAYER_SHIELD_ATTACK_D] = CState_Player_Shield_Attack_D::Create(this, STATE_PLAYER_SHIELD_ATTACK_D);

	m_States[STATE_PLAYER_SHIELD_ATTACK_AIR_A] = CState_Player_Shield_Attack_Air_A::Create(this, STATE_PLAYER_SHIELD_ATTACK_AIR_A);
	m_States[STATE_PLAYER_SHIELD_ATTACK_AIR_B] = CState_Player_Shield_Attack_Air_B::Create(this, STATE_PLAYER_SHIELD_ATTACK_AIR_B);
	m_States[STATE_PLAYER_SHIELD_ATTACK_AIR_C] = CState_Player_Shield_Attack_Air_C::Create(this, STATE_PLAYER_SHIELD_ATTACK_AIR_C);

	m_States[STATE_PLAYER_SHIELD_ATTACK_BASE_CHARGE_BEGIN] = CState_Player_Shield_Attack_Base_Charge_A::Create(this, STATE_PLAYER_SHIELD_ATTACK_BASE_CHARGE_BEGIN);
	m_States[STATE_PLAYER_SHIELD_ATTACK_BASE_CHARGE_LOOP] = CState_Player_Shield_Attack_Base_Charge_B::Create(this, STATE_PLAYER_SHIELD_ATTACK_BASE_CHARGE_LOOP);
	m_States[STATE_PLAYER_SHIELD_ATTACK_BASE_CHARGE_END] = CState_Player_Shield_Attack_Base_Charge_C::Create(this, STATE_PLAYER_SHIELD_ATTACK_BASE_CHARGE_END);

	m_States[STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_A] = CState_Player_Shield_Attack_Jump_Charge_A::Create(this, STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_A);
	m_States[STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_B] = CState_Player_Shield_Attack_Jump_Charge_B::Create(this, STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_B);
	m_States[STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_C] = CState_Player_Shield_Attack_Jump_Charge_C::Create(this, STATE_PLAYER_SHIELD_ATTACK_JUMP_CHARGE_C);

	m_States[STATE_PLAYER_SHIELD_ATTACK_ROLL] = CState_Player_Shield_Attack_Roll::Create(this, STATE_PLAYER_SHIELD_ATTACK_ROLL);

	m_States[STATE_PLAYER_SHIELD_ATTACK_SKILL_NORMAL] = CState_Player_Shield_Skill_Normal::Create(this, STATE_PLAYER_SHIELD_ATTACK_SKILL_NORMAL);
	m_States[STATE_PLAYER_SHIELD_ATTACK_SKILL_SPEAICAL] = CState_Player_Shield_Skill_Special::Create(this, STATE_PLAYER_SHIELD_ATTACK_SKILL_SPEAICAL);

	m_States[STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_BEGIN] = CState_Player_Shield_Attack_Skill_Air_A::Create(this, STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_BEGIN);
	m_States[STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_LOOP] = CState_Player_Shield_Attack_Skill_Air_B::Create(this, STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_LOOP);
	m_States[STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_END] = CState_Player_Shield_Attack_Skill_Air_C::Create(this, STATE_PLAYER_SHIELD_SKILL_NORMAL_AIR_END);

	m_States[STATE_PLAYER_SHIELD_ROLL_FORWORD] = CState_Player_Shield_Roll_Forword::Create(this, STATE_PLAYER_SHIELD_ROLL_FORWORD);
	m_States[STATE_PLAYER_SHIELD_ROLL_FORWORD_AIR] = CState_Player_Shield_Roll_Forword_Air::Create(this, STATE_PLAYER_SHIELD_ROLL_FORWORD_AIR);
	m_States[STATE_PLAYER_SHIELD_ROLL_FORWORD_END] = CState_Player_Shield_Roll_Forword_End::Create(this, STATE_PLAYER_SHIELD_ROLL_FORWORD_END);
	m_States[STATE_PLAYER_SHIELD_ROLL_BACK] = CState_Player_Shield_Roll_Back::Create(this, STATE_PLAYER_SHIELD_ROLL_BACK);
	m_States[STATE_PLAYER_SHIELD_ROLL_BACK_AIR] = CState_Player_Shield_Roll_Back_Air::Create(this, STATE_PLAYER_SHIELD_ROLL_BACK_AIR);

	m_States[STATE_PLAYER_AXE_ATTACK_A] = CState_Player_Axe_Attack_A::Create(this, STATE_PLAYER_AXE_ATTACK_A);
	m_States[STATE_PLAYER_AXE_ATTACK_B] = CState_Player_Axe_Attack_B::Create(this, STATE_PLAYER_AXE_ATTACK_B);
	m_States[STATE_PLAYER_AXE_ATTACK_C] = CState_Player_Axe_Attack_C::Create(this, STATE_PLAYER_AXE_ATTACK_C);
	m_States[STATE_PLAYER_AXE_ATTACK_D] = CState_Player_Axe_Attack_D::Create(this, STATE_PLAYER_AXE_ATTACK_D);

	m_States[STATE_PLAYER_AXE_ATTACK_AIR_A] = CState_Player_Axe_Attack_Air_A::Create(this, STATE_PLAYER_AXE_ATTACK_AIR_A);
	m_States[STATE_PLAYER_AXE_ATTACK_AIR_B] = CState_Player_Axe_Attack_Air_B::Create(this, STATE_PLAYER_AXE_ATTACK_AIR_B);
	m_States[STATE_PLAYER_AXE_ATTACK_AIR_C] = CState_Player_Axe_Attack_Air_C::Create(this, STATE_PLAYER_AXE_ATTACK_AIR_C);

	m_States[STATE_PLAYER_AXE_ATTACK_BASE_CHARGE] = CState_Player_Axe_Attack_Base_Charge::Create(this, STATE_PLAYER_AXE_ATTACK_BASE_CHARGE);

	m_States[STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_A] = CState_Player_Axe_Attack_Jump_Charge_A::Create(this, STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_A);
	m_States[STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_B] = CState_Player_Axe_Attack_Jump_Charge_B::Create(this, STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_B);
	m_States[STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_C] = CState_Player_Axe_Attack_Jump_Charge_C::Create(this, STATE_PLAYER_AXE_ATTACK_JUMP_CHARGE_C);

	m_States[STATE_PLAYER_AXE_ATTACK_ROLL] = CState_Player_Axe_Attack_Roll::Create(this, STATE_PLAYER_AXE_ATTACK_ROLL);

	m_States[STATE_PLAYER_AXE_ATTACK_SKILL_NORMAL] = CState_Player_Axe_Skill_Normal::Create(this, STATE_PLAYER_AXE_ATTACK_SKILL_NORMAL);

	m_States[STATE_PLAYER_AXE_SKILL_NORMAL_AIR_BEGIN] = CState_Player_Axe_Attack_Skill_Air_A::Create(this, STATE_PLAYER_AXE_SKILL_NORMAL_AIR_BEGIN);
	m_States[STATE_PLAYER_AXE_SKILL_NORMAL_AIR_LOOP] = CState_Player_Axe_Attack_Skill_Air_B::Create(this, STATE_PLAYER_AXE_SKILL_NORMAL_AIR_LOOP);
	m_States[STATE_PLAYER_AXE_SKILL_NORMAL_AIR_END] = CState_Player_Axe_Attack_Skill_Air_C::Create(this, STATE_PLAYER_AXE_SKILL_NORMAL_AIR_END);

	m_States[STATE_PLAYER_AXE_ROLL_FORWORD] = CState_Player_Axe_Roll_Forword::Create(this, STATE_PLAYER_AXE_ROLL_FORWORD);
	m_States[STATE_PLAYER_AXE_ROLL_FORWORD_AIR] = CState_Player_Axe_Roll_Forword_Air::Create(this, STATE_PLAYER_AXE_ROLL_FORWORD_AIR);
	m_States[STATE_PLAYER_AXE_ROLL_FORWORD_END] = CState_Player_Axe_Roll_Forword_End::Create(this, STATE_PLAYER_AXE_ROLL_FORWORD_END);
	m_States[STATE_PLAYER_AXE_ROLL_BACK] = CState_Player_Axe_Roll_Back::Create(this, STATE_PLAYER_AXE_ROLL_BACK);
	m_States[STATE_PLAYER_AXE_ROLL_BACK_AIR] = CState_Player_Axe_Roll_Back_Air::Create(this, STATE_PLAYER_AXE_ROLL_BACK_AIR);
#pragma endregion ShieldAxe

#pragma region Cube
	m_States[STATE_PLAYER_CUBE_IDLE] = CState_Player_Cube_Idle::Create(this, STATE_PLAYER_CUBE_IDLE);

	m_States[STATE_PLAYER_CUBE_ATTACK_A] = CState_Player_Cube_Attack_A::Create(this, STATE_PLAYER_CUBE_ATTACK_A);
	m_States[STATE_PLAYER_CUBE_ATTACK_B] = CState_Player_Cube_Attack_B::Create(this, STATE_PLAYER_CUBE_ATTACK_B);
	m_States[STATE_PLAYER_CUBE_ATTACK_C] = CState_Player_Cube_Attack_C::Create(this, STATE_PLAYER_CUBE_ATTACK_C);
	m_States[STATE_PLAYER_CUBE_ATTACK_D] = CState_Player_Cube_Attack_D::Create(this, STATE_PLAYER_CUBE_ATTACK_D);
	m_States[STATE_PLAYER_CUBE_ATTACK_E] = CState_Player_Cube_Attack_E::Create(this, STATE_PLAYER_CUBE_ATTACK_E);

	m_States[STATE_PLAYER_CUBE_ATTACK_AIR_A] = CState_Player_Cube_Attack_Air_A::Create(this, STATE_PLAYER_CUBE_ATTACK_AIR_A);
	m_States[STATE_PLAYER_CUBE_ATTACK_AIR_B] = CState_Player_Cube_Attack_Air_B::Create(this, STATE_PLAYER_CUBE_ATTACK_AIR_B);
	m_States[STATE_PLAYER_CUBE_ATTACK_AIR_C] = CState_Player_Cube_Attack_Air_C::Create(this, STATE_PLAYER_CUBE_ATTACK_AIR_C);

	m_States[STATE_PLAYER_CUBE_ATTACK_BASE_CHARGE] = CState_Player_Cube_Attack_Base_Charge::Create(this, STATE_PLAYER_CUBE_ATTACK_BASE_CHARGE);

	m_States[STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_BEGIN] = CState_Player_Cube_Attack_Jump_Charge_A::Create(this, STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_BEGIN);
	m_States[STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_LOOP] = CState_Player_Cube_Attack_Jump_Charge_B::Create(this, STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_LOOP);
	m_States[STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_END] = CState_Player_Cube_Attack_Jump_Charge_C::Create(this, STATE_PLAYER_CUBE_ATTACK_JUMP_CHARGE_END);

	m_States[STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL] = CState_Player_Cube_Attack_Skill_Normal::Create(this, STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL);
	m_States[STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL_AIR] = CState_Player_Cube_Attack_Skill_Normal_Air::Create(this, STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL_AIR);

	m_States[STATE_PLAYER_CUBE_ATTACK_SKILL_SPECIAL] = CState_Player_Cube_Attack_Skill_Special::Create(this, STATE_PLAYER_CUBE_ATTACK_SKILL_SPECIAL);
	m_States[STATE_PLAYER_CUBE_ATTACK_SKILL_SPECIAL_AIR] = CState_Player_Cube_Attack_Skill_Special_Air::Create(this, STATE_PLAYER_CUBE_ATTACK_SKILL_SPECIAL_AIR);

	m_States[STATE_PLAYER_CUBE_ROLL_FORWORD] = CState_Player_Cube_Roll_Forword::Create(this, STATE_PLAYER_CUBE_ROLL_FORWORD);
	m_States[STATE_PLAYER_CUBE_ROLL_FORWORD_AIR] = CState_Player_Cube_Roll_Forword_Air::Create(this, STATE_PLAYER_CUBE_ROLL_FORWORD_AIR);
	m_States[STATE_PLAYER_CUBE_ROLL_FORWORD_END] = CState_Player_Cube_Roll_Forword_End::Create(this, STATE_PLAYER_CUBE_ROLL_FORWORD_END);
	m_States[STATE_PLAYER_CUBE_ROLL_BACK] = CState_Player_Cube_Roll_Back::Create(this, STATE_PLAYER_CUBE_ROLL_BACK);
	m_States[STATE_PLAYER_CUBE_ROLL_BACK_AIR] = CState_Player_Cube_Roll_Back_Air::Create(this, STATE_PLAYER_CUBE_ROLL_BACK_AIR);
#pragma endregion Cube

#pragma region UI
	m_States[STATE_PLAYER_UI_APPEAR] = CState_Player_UI_Appear::Create(this, STATE_PLAYER_UI_APPEAR);
	//m_States[STATE_PLAYER_UI_IDLE] = CState_Player_UI_Idle::Create(this, STATE_PLAYER_UI_IDLE);

	m_States[STATE_PLAYER_UI_SWORD_APPEAR] = CState_Player_UI_Sword_Appear::Create(this, STATE_PLAYER_UI_SWORD_APPEAR);
	m_States[STATE_PLAYER_UI_SWORD_IDLE] = CState_Player_UI_Sword_Idle::Create(this, STATE_PLAYER_UI_SWORD_IDLE);

	m_States[STATE_PLAYER_UI_2GUN_APPEAR] = CState_Player_UI_2Gun_Appear::Create(this, STATE_PLAYER_UI_2GUN_APPEAR);
	m_States[STATE_PLAYER_UI_2GUN_IDLE] = CState_Player_UI_2Gun_Idle::Create(this, STATE_PLAYER_UI_2GUN_IDLE);

	m_States[STATE_PLAYER_UI_SHIELD_APPEAR] = CState_Player_UI_Shield_Appear::Create(this, STATE_PLAYER_UI_SHIELD_APPEAR);
	m_States[STATE_PLAYER_UI_SHIELD_IDLE] = CState_Player_UI_Shield_Idle::Create(this, STATE_PLAYER_UI_SHIELD_IDLE);

	m_States[STATE_PLAYER_UI_CUBE_APPEAR] = CState_Player_UI_Cube_Appear::Create(this, STATE_PLAYER_UI_CUBE_APPEAR);
	m_States[STATE_PLAYER_UI_CUBE_IDLE] = CState_Player_UI_Cube_Idle::Create(this, STATE_PLAYER_UI_CUBE_IDLE);

	m_States[STATE_PLAYER_UI_NONE_APPEAR] = CState_Player_UI_None_Appear::Create(this, STATE_PLAYER_UI_NONE_APPEAR);
	m_States[STATE_PLAYER_UI_NONE_IDLE] = CState_Player_UI_None_Idle::Create(this, STATE_PLAYER_UI_NONE_IDLE);
#pragma endregion UI

#pragma endregion Player

#pragma region MotorCycle

	m_States[STATE_MOTORCYCLE_IDLE] = CState_MotorCycle_Idle::Create(this, STATE_MOTORCYCLE_IDLE);
	m_States[STATE_MOTORCYCLE_JUMP_START] = CState_MotorCycle_Jump_Start::Create(this, STATE_MOTORCYCLE_JUMP_START);
	m_States[STATE_MOTORCYCLE_JUMP_LOOP] = CState_MotorCycle_Jump_Loop::Create(this, STATE_MOTORCYCLE_JUMP_LOOP);
	m_States[STATE_MOTORCYCLE_JUMP_RUN] = CState_MotorCycle_Jump_Run::Create(this, STATE_MOTORCYCLE_JUMP_RUN);
	m_States[STATE_MOTORCYCLE_JUMP_END] = CState_MotorCycle_Jump_End::Create(this, STATE_MOTORCYCLE_JUMP_END);
	m_States[STATE_MOTORCYCLE_RUN] = CState_MotorCycle_Run::Create(this, STATE_MOTORCYCLE_RUN);

#pragma endregion MotorCycle

#pragma region Enemy_Normal

#pragma region Gang_Swarm
	// For Enemy_HyenaGang_Minions
	{
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_IDLE_NORMAL] = CState_Enemy_HyenaGang_Minions_Idle_Normal::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_IDLE_NORMAL);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_IDLE_BATTLE] = CState_Enemy_HyenaGang_Minions_Idle_Battle::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_IDLE_BATTLE);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_WALK] = CState_Enemy_HyenaGang_Minions_Walk::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_WALK);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_RUN] = CState_Enemy_HyenaGang_Minions_Run::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_RUN);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_RUN_RETURN] = CState_Enemy_HyenaGang_Minions_Run_Return::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_RUN_RETURN);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_ATTACK_NORMAL_SHOT] = CState_Enemy_HyenaGang_Minions_Attack_Normal_Shot::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_ATTACK_NORMAL_SHOT);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_ATTACK_CHARGE_SHOT] = CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_ATTACK_CHARGE_SHOT);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_TURN_L] = CState_Enemy_HyenaGang_Minions_Turn_L::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_TURN_L);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_TURN_R] = CState_Enemy_HyenaGang_Minions_Turn_R::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_TURN_R);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_B] = CState_Enemy_HyenaGang_Minions_Damage_B::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_B);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_F] = CState_Enemy_HyenaGang_Minions_Damage_F::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_F);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_L] = CState_Enemy_HyenaGang_Minions_Damage_L::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_L);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_R] = CState_Enemy_HyenaGang_Minions_Damage_R::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_R);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_WALK_B] = CState_Enemy_HyenaGang_Minions_Walk_B::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_WALK_B);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_WALK_F] = CState_Enemy_HyenaGang_Minions_Walk_F::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_WALK_F);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_WALK_L] = CState_Enemy_HyenaGang_Minions_Walk_L::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_WALK_L);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_WALK_R] = CState_Enemy_HyenaGang_Minions_Walk_R::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_WALK_R);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_DEAD_1] = CState_Enemy_HyenaGang_Minions_Dead_1::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_DEAD_1);
		m_States[STATE_ENEMY_HYENAGANG_MINIONS_DEAD_END] = CState_Enemy_HyenaGang_Minions_Dead_End::Create(this, STATE_ENEMY_HYENAGANG_MINIONS_DEAD_END);


	}
	// For Enemy_HyenaGang_Shooter
	{
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_NORMAL] = CState_Enemy_HyenaGang_Shooter_Idle_Normal::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_NORMAL);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_BATTLE] = CState_Enemy_HyenaGang_Shooter_Idle_Battle::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_BATTLE);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_WALK] = CState_Enemy_HyenaGang_Shooter_Walk::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_WALK);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_RUN] = CState_Enemy_HyenaGang_Shooter_Run::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_RUN);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_RUN_RETURN] = CState_Enemy_HyenaGang_Shooter_Run_Return::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_RUN_RETURN);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_ATTACK_NORMAL_SHOT] = CState_Enemy_HyenaGang_Shooter_Attack_Normal_Shot::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_ATTACK_NORMAL_SHOT);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_ATTACK_CHARGE_SHOT] = CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_ATTACK_CHARGE_SHOT);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_TURN_L] = CState_Enemy_HyenaGang_Shooter_Turn_L::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_TURN_L);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_TURN_R] = CState_Enemy_HyenaGang_Shooter_Turn_R::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_TURN_R);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_B] = CState_Enemy_HyenaGang_Shooter_Damage_B::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_B);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_F] = CState_Enemy_HyenaGang_Shooter_Damage_F::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_F);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_L] = CState_Enemy_HyenaGang_Shooter_Damage_L::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_L);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_R] = CState_Enemy_HyenaGang_Shooter_Damage_R::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_R);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_WALK_B] = CState_Enemy_HyenaGang_Shooter_Walk_B::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_WALK_B);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_WALK_F] = CState_Enemy_HyenaGang_Shooter_Walk_F::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_WALK_F);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_WALK_L] = CState_Enemy_HyenaGang_Shooter_Walk_L::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_WALK_L);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_WALK_R] = CState_Enemy_HyenaGang_Shooter_Walk_R::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_WALK_R);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_DEAD_1] = CState_Enemy_HyenaGang_Shooter_Dead_1::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_DEAD_1);
		m_States[STATE_ENEMY_HYENAGANG_SHOOTER_DEAD_END] = CState_Enemy_HyenaGang_Shooter_Dead_End::Create(this, STATE_ENEMY_HYENAGANG_SHOOTER_DEAD_END);
	}
	// For Enemy_Gangster_Cap
	{
		m_States[STATE_ENEMY_GANGSTER_CAP_IDLE_NORMAL] = CState_Enemy_Gangster_Cap_Idle_Normal::Create(this, STATE_ENEMY_GANGSTER_CAP_IDLE_NORMAL);
		m_States[STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE] = CState_Enemy_Gangster_Cap_Idle_Battle::Create(this, STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE);

		m_States[STATE_ENEMY_GANGSTER_CAP_WALK] = CState_Enemy_Gangster_Cap_Walk::Create(this, STATE_ENEMY_GANGSTER_CAP_WALK);

		m_States[STATE_ENEMY_GANGSTER_CAP_RUN] = CState_Enemy_Gangster_Cap_Run::Create(this, STATE_ENEMY_GANGSTER_CAP_RUN);
		m_States[STATE_ENEMY_GANGSTER_CAP_RUN_RETURN] = CState_Enemy_Gangster_Cap_Run_Return::Create(this, STATE_ENEMY_GANGSTER_CAP_RUN_RETURN);

		m_States[STATE_ENEMY_GANGSTER_CAP_ATTACK_SWING] = CState_Enemy_Gangster_Cap_Attack_Swing::Create(this, STATE_ENEMY_GANGSTER_CAP_ATTACK_SWING);
		m_States[STATE_ENEMY_GANGSTER_CAP_ATTACK_DASH] = CState_Enemy_Gangster_Cap_Attack_Dash::Create(this, STATE_ENEMY_GANGSTER_CAP_ATTACK_DASH);
		m_States[STATE_ENEMY_GANGSTER_CAP_ATTACK_TURNTURN] = CState_Enemy_Gangster_Cap_Attack_TurnTurn::Create(this, STATE_ENEMY_GANGSTER_CAP_ATTACK_TURNTURN);

		m_States[STATE_ENEMY_GANGSTER_CAP_TURN_L] = CState_Enemy_Gangster_Cap_Turn_L::Create(this, STATE_ENEMY_GANGSTER_CAP_TURN_L);
		m_States[STATE_ENEMY_GANGSTER_CAP_TURN_R] = CState_Enemy_Gangster_Cap_Turn_R::Create(this, STATE_ENEMY_GANGSTER_CAP_TURN_R);

		m_States[STATE_ENEMY_GANGSTER_CAP_WALK_L] = CState_Enemy_Gangster_Cap_Walk_L::Create(this, STATE_ENEMY_GANGSTER_CAP_WALK_L);
		m_States[STATE_ENEMY_GANGSTER_CAP_WALK_R] = CState_Enemy_Gangster_Cap_Walk_R::Create(this, STATE_ENEMY_GANGSTER_CAP_WALK_R);
		m_States[STATE_ENEMY_GANGSTER_CAP_WALK_B] = CState_Enemy_Gangster_Cap_Walk_B::Create(this, STATE_ENEMY_GANGSTER_CAP_WALK_B);

		m_States[STATE_ENEMY_GANGSTER_CAP_DAMAGE_B] = CState_Enemy_Gangster_Cap_Damage_B::Create(this, STATE_ENEMY_GANGSTER_CAP_DAMAGE_B);
		m_States[STATE_ENEMY_GANGSTER_CAP_DAMAGE_L] = CState_Enemy_Gangster_Cap_Damage_L::Create(this, STATE_ENEMY_GANGSTER_CAP_DAMAGE_L);
		m_States[STATE_ENEMY_GANGSTER_CAP_DAMAGE_R] = CState_Enemy_Gangster_Cap_Damage_R::Create(this, STATE_ENEMY_GANGSTER_CAP_DAMAGE_R);

		m_States[STATE_ENEMY_GANGSTER_CAP_DEAD_1] = CState_Enemy_Gangster_Cap_Dead_1::Create(this, STATE_ENEMY_GANGSTER_CAP_DEAD_1);
		m_States[STATE_ENEMY_GANGSTER_CAP_DEAD_END] = CState_Enemy_Gangster_Cap_Dead_End::Create(this, STATE_ENEMY_GANGSTER_CAP_DEAD_END);

	}
#pragma endregion 

#pragma region Mutation_Swarm
	// For Enemy_Mutation
	{
		m_States[STATE_ENEMY_MUTATION_IDLE_NORMAL] = CState_Enemy_Mutation_Idle_Normal::Create(this, STATE_ENEMY_MUTATION_IDLE_NORMAL);
		m_States[STATE_ENEMY_MUTATION_IDLE_BATTLE] = CState_Enemy_Mutation_Idle_Battle::Create(this, STATE_ENEMY_MUTATION_IDLE_BATTLE);

		m_States[STATE_ENEMY_MUTATION_WALK] = CState_Enemy_Mutation_Walk::Create(this, STATE_ENEMY_MUTATION_WALK);

		m_States[STATE_ENEMY_MUTATION_RUN] = CState_Enemy_Mutation_Run::Create(this, STATE_ENEMY_MUTATION_RUN);
		m_States[STATE_ENEMY_MUTATION_RUN_RETURN] = CState_Enemy_Mutation_Run_Return::Create(this, STATE_ENEMY_MUTATION_RUN_RETURN);

		m_States[STATE_ENEMY_MUTATION_ATTACK_UPHIT] = CState_Enemy_Mutation_Attack_UpHit::Create(this, STATE_ENEMY_MUTATION_ATTACK_UPHIT);
		m_States[STATE_ENEMY_MUTATION_ATTACK_DOWNHIT] = CState_Enemy_Mutation_Attack_DownHit::Create(this, STATE_ENEMY_MUTATION_ATTACK_DOWNHIT);

		m_States[STATE_ENEMY_MUTATION_TURN_L] = CState_Enemy_Mutation_Turn_L::Create(this, STATE_ENEMY_MUTATION_TURN_L);
		m_States[STATE_ENEMY_MUTATION_TURN_R] = CState_Enemy_Mutation_Turn_R::Create(this, STATE_ENEMY_MUTATION_TURN_R);

		m_States[STATE_ENEMY_MUTATION_WALK_L] = CState_Enemy_Mutation_Walk_L::Create(this, STATE_ENEMY_MUTATION_WALK_L);
		m_States[STATE_ENEMY_MUTATION_WALK_R] = CState_Enemy_Mutation_Walk_R::Create(this, STATE_ENEMY_MUTATION_WALK_R);
		m_States[STATE_ENEMY_MUTATION_WALK_B] = CState_Enemy_Mutation_Walk_B::Create(this, STATE_ENEMY_MUTATION_WALK_B);

		m_States[STATE_ENEMY_MUTATION_DAMAGE_B] = CState_Enemy_Mutation_Damage_B::Create(this, STATE_ENEMY_MUTATION_DAMAGE_B);
		m_States[STATE_ENEMY_MUTATION_DAMAGE_L] = CState_Enemy_Mutation_Damage_L::Create(this, STATE_ENEMY_MUTATION_DAMAGE_L);
		m_States[STATE_ENEMY_MUTATION_DAMAGE_R] = CState_Enemy_Mutation_Damage_R::Create(this, STATE_ENEMY_MUTATION_DAMAGE_R);
		m_States[STATE_ENEMY_MUTATION_DAMAGE_F] = CState_Enemy_Mutation_Damage_F::Create(this, STATE_ENEMY_MUTATION_DAMAGE_F);

		m_States[STATE_ENEMY_MUTATION_DEAD_1] = CState_Enemy_Mutation_Dead_1::Create(this, STATE_ENEMY_MUTATION_DEAD_1);
		m_States[STATE_ENEMY_MUTATION_DEAD_END] = CState_Enemy_Mutation_Dead_End::Create(this, STATE_ENEMY_MUTATION_DEAD_END);

		m_States[STATE_ENEMY_MUTATION_EAT] = CState_Enemy_Mutation_Eat::Create(this, STATE_ENEMY_MUTATION_EAT);
		m_States[STATE_ENEMY_MUTATION_EAT_FIND] = CState_Enemy_Mutation_Eat_Find::Create(this, STATE_ENEMY_MUTATION_EAT_FIND);

	}
	// For Enemy_Habaka_Admiral
	{
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL] = CState_Enemy_Habaka_Admiral_Idle_Normal::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL);
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE] = CState_Enemy_Habaka_Admiral_Idle_Battle::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE);

		m_States[STATE_ENEMY_HABAKA_ADMIRAL_WALK] = CState_Enemy_Habaka_Admiral_Walk::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_WALK);

		m_States[STATE_ENEMY_HABAKA_ADMIRAL_RUN] = CState_Enemy_Habaka_Admiral_Run::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_RUN);
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_RUN_RETURN] = CState_Enemy_Habaka_Admiral_Run_Return::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_RUN_RETURN);

		m_States[STATE_ENEMY_HABAKA_ADMIRAL_TURN_L] = CState_Enemy_Habaka_Admiral_Turn_L::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_TURN_L);
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_TURN_R] = CState_Enemy_Habaka_Admiral_Turn_R::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_TURN_R);

		m_States[STATE_ENEMY_HABAKA_ADMIRAL_WALK_L] = CState_Enemy_Habaka_Admiral_Walk_L::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_WALK_L);
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_WALK_R] = CState_Enemy_Habaka_Admiral_Walk_R::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_WALK_R);
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_WALK_B] = CState_Enemy_Habaka_Admiral_Walk_B::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_WALK_B);

		m_States[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B] = CState_Enemy_Habaka_Admiral_Damage_B::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B);
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L] = CState_Enemy_Habaka_Admiral_Damage_L::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L);
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R] = CState_Enemy_Habaka_Admiral_Damage_R::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R);
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F] = CState_Enemy_Habaka_Admiral_Damage_F::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F);

		m_States[STATE_ENEMY_HABAKA_ADMIRAL_DEAD_1] = CState_Enemy_Habaka_Admiral_Dead_1::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_DEAD_1);
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_DEAD_END] = CState_Enemy_Habaka_Admiral_Dead_End::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_DEAD_END);

		m_States[STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_FRONTJUMP] = CState_Enemy_Habaka_Admiral_Attack_FrontJump::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_FRONTJUMP);
		m_States[STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_KICK_A] = CState_Enemy_Habaka_Admiral_Attack_Kick_A::Create(this, STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_KICK_A);
	}
#pragma endregion 

#pragma region MechanicalCity_Swarm

	// For Enemy_MechanicalCity_Gunner
	{
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_NORMAL] = CState_Enemy_MechanicalCity_Gunner_Idle_Normal::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_NORMAL);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_BATTLE] = CState_Enemy_MechanicalCity_Gunner_Idle_Battle::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_BATTLE);

		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_WALK] = CState_Enemy_MechanicalCity_Gunner_Walk::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_WALK);

		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_RUN] = CState_Enemy_MechanicalCity_Gunner_Run::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_RUN);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_RUN_RETURN] = CState_Enemy_MechanicalCity_Gunner_Run_Return::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_RUN_RETURN);

		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_BEGIN] = CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Begin::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_BEGIN);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_ING] = CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_ING);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_END] = CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_END);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_THROWBOMB] = CState_Enemy_MechanicalCity_Gunner_Attack_ThrowBomb::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_THROWBOMB);

		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_TURN_L] = CState_Enemy_MechanicalCity_Gunner_Turn_L::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_TURN_L);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_TURN_R] = CState_Enemy_MechanicalCity_Gunner_Turn_R::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_TURN_R);

		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_WALK_L] = CState_Enemy_MechanicalCity_Gunner_Walk_L::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_WALK_L);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_WALK_R] = CState_Enemy_MechanicalCity_Gunner_Walk_R::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_WALK_R);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_WALK_B] = CState_Enemy_MechanicalCity_Gunner_Walk_B::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_WALK_B);

		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B] = CState_Enemy_MechanicalCity_Gunner_Damage_B::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L] = CState_Enemy_MechanicalCity_Gunner_Damage_L::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R] = CState_Enemy_MechanicalCity_Gunner_Damage_R::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F] = CState_Enemy_MechanicalCity_Gunner_Damage_F::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F);

		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_1] = CState_Enemy_MechanicalCity_Gunner_Dead_1::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_1);
		m_States[STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_END] = CState_Enemy_MechanicalCity_Gunner_Dead_End::Create(this, STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_END);

	}
	// For Enemy_MechanicalCity_SwordGhost
	{
		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_IDLE_NORMAL] = CState_Enemy_MechanicalCity_SwordGhost_Idle_Normal::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_IDLE_NORMAL);
		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_IDLE_BATTLE] = CState_Enemy_MechanicalCity_SwordGhost_Idle_Battle::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_IDLE_BATTLE);

		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_WALK] = CState_Enemy_MechanicalCity_SwordGhost_Walk::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_WALK);

		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_RUN] = CState_Enemy_MechanicalCity_SwordGhost_Run::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_RUN);
		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_RUN_RETURN] = CState_Enemy_MechanicalCity_SwordGhost_Run_Return::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_RUN_RETURN);

		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_ATTACK_MOVING] = CState_Enemy_MechanicalCity_SwordGhost_Attack_Moving::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_ATTACK_MOVING);
		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_ATTACK_STAND] = CState_Enemy_MechanicalCity_SwordGhost_Attack_Stand::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_ATTACK_STAND);

		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_TURN_L] = CState_Enemy_MechanicalCity_SwordGhost_Turn_L::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_TURN_L);
		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_TURN_R] = CState_Enemy_MechanicalCity_SwordGhost_Turn_R::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_TURN_R);

		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_WALK_L] = CState_Enemy_MechanicalCity_SwordGhost_Walk_L::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_WALK_L);
		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_WALK_R] = CState_Enemy_MechanicalCity_SwordGhost_Walk_R::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_WALK_R);
		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_WALK_B] = CState_Enemy_MechanicalCity_SwordGhost_Walk_B::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_WALK_B);

		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_B] = CState_Enemy_MechanicalCity_SwordGhost_Damage_B::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_B);
		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_L] = CState_Enemy_MechanicalCity_SwordGhost_Damage_L::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_L);
		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_R] = CState_Enemy_MechanicalCity_SwordGhost_Damage_R::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DAMAGE_R);

		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DEAD_END] = CState_Enemy_MechanicalCity_SwordGhost_Dead_End::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_DEAD_END);

	}

	// For Enemy_MechanicalCity_DoubleEdgedSwordAdept
	{
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_NORMAL] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Idle_Normal::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_NORMAL);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_BATTLE] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Idle_Battle::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_BATTLE);

		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_WALK] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_WALK);

		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_RUN] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_RUN);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_RUN_RETURN] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Run_Return::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_RUN_RETURN);

		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_DASH] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Dash::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_DASH);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SWING] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Swing::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SWING);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_BEGIN] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Begin::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_BEGIN);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_ING] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_Ing::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_ING);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_END] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Attack_Special_End::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_ATTACK_SPECIAL_END);

		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_TURN_L] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Turn_L::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_TURN_L);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_TURN_R] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Turn_R::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_TURN_R);

		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_WALK_L] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk_L::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_WALK_L);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_WALK_R] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk_R::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_WALK_R);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_WALK_B] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk_B::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_WALK_B);

		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Damage_B::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Damage_L::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Damage_R::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R);

		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DEAD_END] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Dead_End::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DEAD_END);

	}

	// For Enemy_MechanicalCity_DoorKeeper
	{
		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_NORMAL] = CState_Enemy_MechanicalCity_DoorKeeper_Idle_Normal::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_NORMAL);
		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_BATTLE] = CState_Enemy_MechanicalCity_DoorKeeper_Idle_Battle::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_BATTLE);

		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_WALK] = CState_Enemy_MechanicalCity_DoorKeeper_Walk::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_WALK);

		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_RUN] = CState_Enemy_MechanicalCity_DoorKeeper_Run::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_RUN);
		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_RUN_RETURN] = CState_Enemy_MechanicalCity_DoorKeeper_Run_Return::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_RUN_RETURN);

		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_CHOP] = CState_Enemy_MechanicalCity_DoorKeeper_Attack_Chop::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_CHOP);
		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_JUMPCHOP] = CState_Enemy_MechanicalCity_DoorKeeper_Attack_JumpChop::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_JUMPCHOP);
		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_kEEPHITTING] = CState_Enemy_MechanicalCity_DoorKeeper_Attack_KeepHitting::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_ATTACK_kEEPHITTING);

		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_TURN_L] = CState_Enemy_MechanicalCity_DoorKeeper_Turn_L::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_TURN_L);
		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_TURN_R] = CState_Enemy_MechanicalCity_DoorKeeper_Turn_R::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_TURN_R);

		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_WALK_L] = CState_Enemy_MechanicalCity_DoorKeeper_Walk_L::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_WALK_L);
		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_WALK_R] = CState_Enemy_MechanicalCity_DoorKeeper_Walk_R::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_WALK_R);
		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_WALK_B] = CState_Enemy_MechanicalCity_DoorKeeper_Walk_B::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_WALK_B);

		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DAMAGE_B] = CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DAMAGE_B);
		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DAMAGE_B2] = CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DAMAGE_B2);

		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DEAD_END] = CState_Enemy_MechanicalCity_DoorKeeper_Dead_End::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DEAD_END);

	}
#pragma endregion 
	///////////////////////////////////////////////only_idle
	{
		/*m_States[STATE_ENEMY_MUTATION_HUNTERDOG_IDLE_NORMAL] = CState_Enemy_Mutation_HunterDog_Idle_Normal::Create(this, STATE_ENEMY_MUTATION_HUNTERDOG_IDLE_NORMAL);
		m_States[STATE_ENEMY_MUTATION_ORC_IDLE_NORMAL] = CState_Enemy_Mutation_Orc_Idle_Normal::Create(this, STATE_ENEMY_MUTATION_ORC_IDLE_NORMAL);
		m_States[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_NORMAL] = CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Idle_Normal::Create(this, STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_NORMAL);
		m_States[STATE_ENEMY_MECHANICALCITY_SWORDGHOST_IDLE_NORMAL] = CState_Enemy_MechanicalCity_SwordGhost_Idle_Normal::Create(this, STATE_ENEMY_MECHANICALCITY_SWORDGHOST_IDLE_NORMAL);
		m_States[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_NORMAL] = CState_Enemy_MechanicalCity_DoorKeeper_Idle_Normal::Create(this, STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_NORMAL);*/

	}

#pragma endregion Enemy_Normal

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
#pragma region Enemy_Boss

	//// For Boss
	//m_States[STATE_BOSS_SITDOWN] = CState_Boss_SitDown::Create(this, STATE_BOSS_SITDOWN);
	//m_States[STATE_BOSS_AWAKE] = CState_Boss_Awake::Create(this, STATE_BOSS_AWAKE);
	//m_States[STATE_BOSS_IDLE] = CState_Boss_Idle::Create(this, STATE_BOSS_IDLE);
	//
	//m_States[STATE_BOSS_ATTACK] = CState_Boss_Attack::Create(this, STATE_BOSS_ATTACK);
	//m_States[STATE_BOSS_SWINGATTACK] = CState_Boss_SwingAttack::Create(this, STATE_BOSS_SWINGATTACK);
	//m_States[STATE_BOSS_JUMPATTACK] = CState_Boss_JumpAttack::Create(this, STATE_BOSS_JUMPATTACK);
	//m_States[STATE_BOSS_KEEPATTACK] = CState_Boss_KeepAttack::Create(this, STATE_BOSS_KEEPATTACK);
	//m_States[STATE_BOSS_DASH] = CState_Boss_Dash::Create(this, STATE_BOSS_DASH);
	//m_States[STATE_BOSS_ULTRAATTACK] = CState_Boss_UltraAttack::Create(this, STATE_BOSS_ULTRAATTACK);
	//m_States[STATE_BOSS_SHIELD] = CState_Boss_Shield::Create(this, STATE_BOSS_SHIELD);


	/*
		STATE_BOSS_ANGEL_READY,				//22
		STATE_BOSS_ANGEL_STAND,				//10
		STATE_BOSS_ANGEL_IDLE,				//13
		STATE_BOSS_ANGEL_BACK,				//16
		STATE_BOSS_ANGEL_TURNL,				//14
		STATE_BOSS_ANGEL_TURNR,				//15
		STATE_BOSS_ANGEL_RUN,				//17
		STATE_BOSS_ANGEL_RUNL,				//18
		STATE_BOSS_ANGEL_RUNR,				//19

		STATE_BOSS_ANGEL_SWING_ATTACK,		//5

		STATE_BOSS_ANGEL_LASER_ATTACK,		//7
		STATE_BOSS_ANGEL_LASER_ATTACKA,		//0 -> 1, 2
		STATE_BOSS_ANGEL_LASER_ATTACKB,		//1
		STATE_BOSS_ANGEL_LASER_ATTACKC,		//2

		STATE_BOSS_ANGEL_SKILL_ATTACKA,		//3
		STATE_BOSS_ANGEL_SKILL_ATTACKB,		//4

		STATE_BOSS_ANGEL_ULT_ATTACK,		//8
		STATE_BOSS_ANGEL_ULT_ATTACK_END,	//9

		STATE_BOSS_ANGEL_SHIELD,			//12
		STATE_BOSS_ANGEL_RESET,				//11

		STATE_BOSS_ANGEL_DEAD,				//21
		*/
	//// For Boss_Angel
	//m_States[STATE_BOSS_ANGEL_READY] = CState_Angel_Apear_Ready::Create(this, STATE_BOSS_ANGEL_READY);
	//m_States[STATE_BOSS_ANGEL_STAND] = CState_Angel_Apear::Create(this, STATE_BOSS_ANGEL_STAND);
	//m_States[STATE_BOSS_ANGEL_IDLE] = CState_Angel_Idle::Create(this, STATE_BOSS_ANGEL_IDLE);
	//
	//m_States[STATE_BOSS_ANGEL_BACK] = CState_Angel_BackWard::Create(this, STATE_BOSS_ANGEL_BACK);
	//m_States[STATE_BOSS_ANGEL_TURNL] = CState_Angel_Turn_L::Create(this, STATE_BOSS_ANGEL_TURNL);
	//m_States[STATE_BOSS_ANGEL_TURNR] = CState_Angel_Turn_R::Create(this, STATE_BOSS_ANGEL_TURNR);
	//
	//m_States[STATE_BOSS_ANGEL_RUN] = CState_Angel_Run::Create(this, STATE_BOSS_ANGEL_RUN);
	//m_States[STATE_BOSS_ANGEL_RUNL] = CState_Angel_Run_L::Create(this, STATE_BOSS_ANGEL_RUNL);
	//m_States[STATE_BOSS_ANGEL_RUNR] = CState_Angel_Run_R::Create(this, STATE_BOSS_ANGEL_RUNR);

	// For Boss_Dragon
	m_States[STATE_BOSS_DRAGON_CAM_ACTION] = CState_Dragon_CameraAction::Create(this, STATE_BOSS_DRAGON_CAM_ACTION);

	m_States[STATE_BOSS_DRAGON_PATROL] = CState_Dragon_Patrol::Create(this, STATE_BOSS_DRAGON_PATROL);
	m_States[STATE_BOSS_DRAGON_PATROL_APEAR] = CState_Dragon_Patrol_Apear::Create(this, STATE_BOSS_DRAGON_PATROL_APEAR);

	m_States[STATE_BOSS_DRAGON_APEAR] = CState_Dragon_Apear::Create(this, STATE_BOSS_DRAGON_APEAR);
	m_States[STATE_BOSS_DRAGON_DOEND] = CState_Dragon_DoEnd::Create(this, STATE_BOSS_DRAGON_DOEND);
	m_States[STATE_BOSS_DRAGON_RUNEND] = CState_Dragon_RunEnd::Create(this, STATE_BOSS_DRAGON_RUNEND);

	m_States[STATE_BOSS_DRAGON_CHASE] = CState_Dragon_Chase::Create(this, STATE_BOSS_DRAGON_CHASE);
	m_States[STATE_BOSS_DRAGON_RUN] = CState_Dragon_Run::Create(this, STATE_BOSS_DRAGON_RUN);
	m_States[STATE_BOSS_DRAGON_IDLE] = CState_Dragon_Idle::Create(this, STATE_BOSS_DRAGON_IDLE);

	m_States[STATE_BOSS_DRAGON_TURNL] = CState_Dragon_TurnL::Create(this, STATE_BOSS_DRAGON_TURNL);
	m_States[STATE_BOSS_DRAGON_TURNR] = CState_Dragon_TurnR::Create(this, STATE_BOSS_DRAGON_TURNR);

	m_States[STATE_BOSS_DRAGON_ATTACK_01] = CState_Dragon_Attack1::Create(this, STATE_BOSS_DRAGON_ATTACK_01);
	m_States[STATE_BOSS_DRAGON_ATTACK_02] = CState_Dragon_Attack2::Create(this, STATE_BOSS_DRAGON_ATTACK_02);
	m_States[STATE_BOSS_DRAGON_ATTACK_03] = CState_Dragon_Attack3::Create(this, STATE_BOSS_DRAGON_ATTACK_03);
	m_States[STATE_BOSS_DRAGON_ATTACK_04] = CState_Dragon_Attack4::Create(this, STATE_BOSS_DRAGON_ATTACK_04);
	m_States[STATE_BOSS_DRAGON_ATTACK_05] = CState_Dragon_Attack5::Create(this, STATE_BOSS_DRAGON_ATTACK_05);
	//m_States[STATE_BOSS_DRAGON_ATTACK_06] = CState_Dragon_Attack6::Create(this, STATE_BOSS_DRAGON_ATTACK_06);
	//m_States[STATE_BOSS_DRAGON_ATTACK_07] = CState_Dragon_Attack7::Create(this, STATE_BOSS_DRAGON_ATTACK_07);
	m_States[STATE_BOSS_DRAGON_ATTACK_08] = CState_Dragon_Attack8::Create(this, STATE_BOSS_DRAGON_ATTACK_08);
	m_States[STATE_BOSS_DRAGON_ATTACK_09] = CState_Dragon_Attack9::Create(this, STATE_BOSS_DRAGON_ATTACK_09);

	m_States[STATE_BOSS_DRAGON_ULT_ATTACK_1] = CState_Dragon_Ult_Attack1::Create(this, STATE_BOSS_DRAGON_ULT_ATTACK_1);
	m_States[STATE_BOSS_DRAGON_ULT_ATTACK_2] = CState_Dragon_Ult_Attack2::Create(this, STATE_BOSS_DRAGON_ULT_ATTACK_2);

	m_States[STATE_BOSS_DRAGON_WEAK_BEGIN] = CState_Dragon_Weak_Begin::Create(this, STATE_BOSS_DRAGON_WEAK_BEGIN);
	m_States[STATE_BOSS_DRAGON_FALL_LAND] = CState_Dragon_Fall_Land::Create(this, STATE_BOSS_DRAGON_FALL_LAND);
	m_States[STATE_BOSS_DRAGON_WEAK_LOOP] = CState_Dragon_Weak_Loop::Create(this, STATE_BOSS_DRAGON_WEAK_LOOP);
	m_States[STATE_BOSS_DRAGON_WEAK_END] = CState_Dragon_Weak_End::Create(this, STATE_BOSS_DRAGON_WEAK_END);

	m_States[STATE_BOSS_DRAGON_DEAD] = CState_Dragon_Dead::Create(this, STATE_BOSS_DRAGON_DEAD);



	// For Boss_Weaver
	m_States[STATE_BOSS_WEAVER_IDLE] = CState_Weaver_Idle::Create(this, STATE_BOSS_WEAVER_IDLE);
	m_States[STATE_BOSS_WEAVER_CAM_ACTION] = CState_Weaver_CameraAction::Create(this, STATE_BOSS_WEAVER_CAM_ACTION);

	m_States[STATE_BOSS_WEAVER_APEAR] = CState_Weaver_Apear::Create(this, STATE_BOSS_WEAVER_APEAR);
	m_States[STATE_BOSS_WEAVER_READY] = CState_Weaver_Ready::Create(this, STATE_BOSS_WEAVER_READY);
	m_States[STATE_BOSS_WEAVER_RESET] = CState_Weaver_Reset::Create(this, STATE_BOSS_WEAVER_RESET);

	m_States[STATE_BOSS_WEAVER_WALK_F] = CState_Weaver_Walk_F::Create(this, STATE_BOSS_WEAVER_WALK_F);
	m_States[STATE_BOSS_WEAVER_WALK_B] = CState_Weaver_Walk_B::Create(this, STATE_BOSS_WEAVER_WALK_B);
	m_States[STATE_BOSS_WEAVER_WALK_L] = CState_Weaver_Walk_L::Create(this, STATE_BOSS_WEAVER_WALK_L);
	m_States[STATE_BOSS_WEAVER_WALK_R] = CState_Weaver_Walk_R::Create(this, STATE_BOSS_WEAVER_WALK_R);

	m_States[STATE_BOSS_WEAVER_TURN_L] = CState_Weaver_Turn_L::Create(this, STATE_BOSS_WEAVER_TURN_L);
	m_States[STATE_BOSS_WEAVER_TURN_R] = CState_Weaver_Turn_R::Create(this, STATE_BOSS_WEAVER_TURN_R);

	m_States[STATE_BOSS_WEAVER_ATTACK_01] = CState_Weaver_Attack_01::Create(this, STATE_BOSS_WEAVER_ATTACK_01);
	m_States[STATE_BOSS_WEAVER_ATTACK_02] = CState_Weaver_Attack_02::Create(this, STATE_BOSS_WEAVER_ATTACK_02);
	m_States[STATE_BOSS_WEAVER_ATTACK_03] = CState_Weaver_Attack_03::Create(this, STATE_BOSS_WEAVER_ATTACK_03);
	m_States[STATE_BOSS_WEAVER_ATTACK_04] = CState_Weaver_Attack_04::Create(this, STATE_BOSS_WEAVER_ATTACK_04);
	m_States[STATE_BOSS_WEAVER_ATTACK_05_1] = CState_Weaver_Attack_05_Begin::Create(this, STATE_BOSS_WEAVER_ATTACK_05_1);
	m_States[STATE_BOSS_WEAVER_ATTACK_05_2] = CState_Weaver_Attack_05_End::Create(this, STATE_BOSS_WEAVER_ATTACK_05_2);
	m_States[STATE_BOSS_WEAVER_ATTACK_06_BEGIN] = CState_Weaver_Attack_06_Begin::Create(this, STATE_BOSS_WEAVER_ATTACK_06_BEGIN);
	m_States[STATE_BOSS_WEAVER_ATTACK_06_BREAK] = CState_Weaver_Attack_06_Break::Create(this, STATE_BOSS_WEAVER_ATTACK_06_BREAK);
	m_States[STATE_BOSS_WEAVER_ATTACK_06_LOOP] = CState_Weaver_Attack_06_Loop::Create(this, STATE_BOSS_WEAVER_ATTACK_06_LOOP);
	m_States[STATE_BOSS_WEAVER_ATTACK_06_END] = CState_Weaver_Attack_06_End::Create(this, STATE_BOSS_WEAVER_ATTACK_06_END);
	m_States[STATE_BOSS_WEAVER_ATTACK_07] = CState_Weaver_Attack_07::Create(this, STATE_BOSS_WEAVER_ATTACK_07);
	m_States[STATE_BOSS_WEAVER_ATTACK_08] = CState_Weaver_Attack_08::Create(this, STATE_BOSS_WEAVER_ATTACK_08);
	m_States[STATE_BOSS_WEAVER_ATTACK_09] = CState_Weaver_Attack_09::Create(this, STATE_BOSS_WEAVER_ATTACK_09);
	m_States[STATE_BOSS_WEAVER_ATTACK_10] = CState_Weaver_Attack_10::Create(this, STATE_BOSS_WEAVER_ATTACK_10);
	m_States[STATE_BOSS_WEAVER_ATTACK_11_BEGIN] = CState_Weaver_Attack_11_Begin::Create(this, STATE_BOSS_WEAVER_ATTACK_11_BEGIN);
	m_States[STATE_BOSS_WEAVER_ATTACK_11_LOOP] = CState_Weaver_Attack_11_Loop::Create(this, STATE_BOSS_WEAVER_ATTACK_11_LOOP);
	m_States[STATE_BOSS_WEAVER_ATTACK_11_END] = CState_Weaver_Attack_11_End::Create(this, STATE_BOSS_WEAVER_ATTACK_11_END);
	m_States[STATE_BOSS_WEAVER_ATTACK_12] = CState_Weaver_Attack_12::Create(this, STATE_BOSS_WEAVER_ATTACK_12);

	m_States[STATE_BOSS_WEAVER_SHIELD] = CState_Weaver_Shield::Create(this, STATE_BOSS_WEAVER_SHIELD);

	m_States[STATE_BOSS_WEAVER_WEAK_BEGIN] = CState_Weaver_Weak_Begin::Create(this, STATE_BOSS_WEAVER_WEAK_LOOP);
	m_States[STATE_BOSS_WEAVER_WEAK_LOOP] = CState_Weaver_Weak_Loop::Create(this, STATE_BOSS_WEAVER_READY);
	m_States[STATE_BOSS_WEAVER_WEAK_END] = CState_Weaver_Weak_End::Create(this, STATE_BOSS_WEAVER_WEAK_END);

	m_States[STATE_BOSS_WEAVER_DEAD] = CState_Weaver_Dead::Create(this, STATE_BOSS_WEAVER_DEAD);


	//Robella
	m_States[STATE_BOSS_ROBELLA_READY] = CState_Robella_Ready::Create(this, STATE_BOSS_ROBELLA_READY);
	m_States[STATE_BOSS_ROBELLA_APEAR] = CState_Robella_Apear::Create(this, STATE_BOSS_ROBELLA_APEAR);
	m_States[STATE_BOSS_ROBELLA_DEAD] = CState_Robella_Dead::Create(this, STATE_BOSS_ROBELLA_DEAD);
	m_States[STATE_BOSS_ROBELLA_CAM_ACTION] = CState_Robella_CameraAction::Create(this, STATE_BOSS_ROBELLA_CAM_ACTION);

	m_States[STATE_BOSS_ROBELLA_IDLE] = CState_Robella_1Phase::Create(this, STATE_BOSS_ROBELLA_IDLE);

	m_States[STATE_BOSS_ROBELLA_1PHASE_DOWN] = CState_Robella_1Phase_Down::Create(this, STATE_BOSS_ROBELLA_1PHASE_DOWN);
	m_States[STATE_BOSS_ROBELLA_1PHASE_DOWN_LOOP] = CState_Robella_1Phase_Down_Loop::Create(this, STATE_BOSS_ROBELLA_1PHASE_DOWN_LOOP);
	m_States[STATE_BOSS_ROBELLA_1PHASE_DOWN_UP] = CState_Robella_1Phase_Down_Up::Create(this, STATE_BOSS_ROBELLA_1PHASE_DOWN_UP);

	m_States[STATE_BOSS_ROBELLA_CHANGE_PHASE] = CState_Robella_Change_Phase::Create(this, STATE_BOSS_ROBELLA_CHANGE_PHASE);

	m_States[STATE_BOSS_ROBELLA_ATTACK_01] = CState_Robella_Attack1::Create(this, STATE_BOSS_ROBELLA_ATTACK_01);
	m_States[STATE_BOSS_ROBELLA_ATTACK_02_BEGIN] = CState_Robella_Attack2_Begin::Create(this, STATE_BOSS_ROBELLA_ATTACK_02_BEGIN);
	m_States[STATE_BOSS_ROBELLA_ATTACK_02_LOOP] = CState_Robella_Attack2_Loop::Create(this, STATE_BOSS_ROBELLA_ATTACK_02_LOOP);
	m_States[STATE_BOSS_ROBELLA_ATTACK_02_END] = CState_Robella_Attack2_End::Create(this, STATE_BOSS_ROBELLA_ATTACK_02_END);
	m_States[STATE_BOSS_ROBELLA_ATTACK_03] = CState_Robella_Attack3::Create(this, STATE_BOSS_ROBELLA_ATTACK_03);

	m_States[STATE_BOSS_ROBELLA_IDLE2] = CState_Robella_2Phase::Create(this, STATE_BOSS_ROBELLA_IDLE2);

	m_States[STATE_BOSS_ROBELLA_2PHASE_DOWN] = CState_Robella_2Phase_Down::Create(this, STATE_BOSS_ROBELLA_2PHASE_DOWN);
	m_States[STATE_BOSS_ROBELLA_2PHASE_DOWN_LOOP] = CState_Robella_2Phase_Down_Loop::Create(this, STATE_BOSS_ROBELLA_2PHASE_DOWN_LOOP);
	m_States[STATE_BOSS_ROBELLA_2PHASE_DOWN_UP] = CState_Robella_2Phase_Down_Up::Create(this, STATE_BOSS_ROBELLA_2PHASE_DOWN_UP);

	m_States[STATE_BOSS_ROBELLA_ATTACK_04] = CState_Robella_Attack4::Create(this, STATE_BOSS_ROBELLA_ATTACK_04);
	m_States[STATE_BOSS_ROBELLA_ATTACK_05] = CState_Robella_Attack5::Create(this, STATE_BOSS_ROBELLA_ATTACK_05);
	m_States[STATE_BOSS_ROBELLA_ATTACK_06] = CState_Robella_Attack6::Create(this, STATE_BOSS_ROBELLA_ATTACK_06);
	m_States[STATE_BOSS_ROBELLA_ATTACK_07] = CState_Robella_Attack7::Create(this, STATE_BOSS_ROBELLA_ATTACK_07);
	m_States[STATE_BOSS_ROBELLA_ATTACK_08] = CState_Robella_Attack8::Create(this, STATE_BOSS_ROBELLA_ATTACK_08);
	m_States[STATE_BOSS_ROBELLA_ATTACK_09_BEGIN] = CState_Robella_Attack9_Begin::Create(this, STATE_BOSS_ROBELLA_ATTACK_09_BEGIN);
	m_States[STATE_BOSS_ROBELLA_ATTACK_09_END] = CState_Robella_Attack9_End::Create(this, STATE_BOSS_ROBELLA_ATTACK_09_END);

	m_States[STATE_BOSS_ROBELLA_ATTACK_ULT_READY] = CState_Robella_UltAttack_Begin::Create(this, STATE_BOSS_ROBELLA_ATTACK_ULT_READY);
	m_States[STATE_BOSS_ROBELLA_ATTACK_ULT] = CState_Robella_UltAttack_End::Create(this, STATE_BOSS_ROBELLA_ATTACK_ULT);


	//Leviathan
	m_States[STATE_BOSS_LEVIATHAN_APEAR] = CState_Leviathan_Apear::Create(this, STATE_BOSS_LEVIATHAN_APEAR);

	m_States[STATE_BOSS_LEVIATHAN_IDLE] = CState_Leviathan_Idle::Create(this, STATE_BOSS_LEVIATHAN_IDLE);
	m_States[STATE_BOSS_LEVIATHAN_WALK] = CState_Leviathan_Walk::Create(this, STATE_BOSS_LEVIATHAN_WALK);
	m_States[STATE_BOSS_LEVIATHAN_WEAK] = CState_Leviathan_Weak::Create(this, STATE_BOSS_LEVIATHAN_WEAK);
	m_States[STATE_BOSS_LEVIATHAN_JUMP_TURNL] = CState_Leviathan_Jump_TurnL::Create(this, STATE_BOSS_LEVIATHAN_JUMP_TURNL);
	m_States[STATE_BOSS_LEVIATHAN_JUMP_TURNR] = CState_Leviathan_Jump_TurnR::Create(this, STATE_BOSS_LEVIATHAN_JUMP_TURNR);

	m_States[STATE_BOSS_LEVIATHAN_ATTACK_DOWN_FR] = CState_Leviathan_Attack_Down_FR::Create(this, STATE_BOSS_LEVIATHAN_ATTACK_DOWN_FR);
	m_States[STATE_BOSS_LEVIATHAN_ATTACK_DOWN_BL_A] = CState_Leviathan_Attack_Down_BL_A::Create(this, STATE_BOSS_LEVIATHAN_ATTACK_DOWN_BL_A);
	m_States[STATE_BOSS_LEVIATHAN_ATTACK_DOWN_BL_B] = CState_Leviathan_Attack_Down_BL_B::Create(this, STATE_BOSS_LEVIATHAN_ATTACK_DOWN_BL_B);
	m_States[STATE_BOSS_LEVIATHAN_ATTACK_DOWN_BL_C] = CState_Leviathan_Attack_Down_BL_C::Create(this, STATE_BOSS_LEVIATHAN_ATTACK_DOWN_BL_C);
	m_States[STATE_BOSS_LEVIATHAN_ATTACK_DOWN_F_A] = CState_Leviathan_Attack_Down_F_A::Create(this, STATE_BOSS_LEVIATHAN_ATTACK_DOWN_F_A);
	m_States[STATE_BOSS_LEVIATHAN_ATTACK_DOWN_F_B] = CState_Leviathan_Attack_Down_F_B::Create(this, STATE_BOSS_LEVIATHAN_ATTACK_DOWN_F_B);
	m_States[STATE_BOSS_LEVIATHAN_ATTACK_DOWN_F_C] = CState_Leviathan_Attack_Down_F_C::Create(this, STATE_BOSS_LEVIATHAN_ATTACK_DOWN_F_C);
	m_States[STATE_BOSS_LEVIATHAN_ATTACK_UP_FL] = CState_Leviathan_Attack_Up_FL::Create(this, STATE_BOSS_LEVIATHAN_ATTACK_UP_FL);
	m_States[STATE_BOSS_LEVIATHAN_ATTACK_UP_BR] = CState_Leviathan_Attack_Up_BR::Create(this, STATE_BOSS_LEVIATHAN_ATTACK_UP_BR);
	m_States[STATE_BOSS_LEVIATHAN_ATTACK_UP_F] = CState_Leviathan_Attack_Up_F::Create(this, STATE_BOSS_LEVIATHAN_ATTACK_UP_F);

	m_States[STATE_BOSS_LEVIATHAN_ROAR_0] = CState_Leviathan_Roar_0::Create(this, STATE_BOSS_LEVIATHAN_ROAR_0);

	m_States[STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR] = CState_Leviathan_Look_Center_Roar::Create(this, STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR);
	m_States[STATE_BOSS_LEVIATHAN_LOOK_OUT_ROAR] = CState_Leviathan_Look_Out_Roar::Create(this, STATE_BOSS_LEVIATHAN_LOOK_OUT_ROAR);
	m_States[STATE_BOSS_LEVIATHAN_TURN] = CState_Leviathan_Turn::Create(this, STATE_BOSS_LEVIATHAN_TURN);
	m_States[STATE_BOSS_LEVIATHAN_JUMP_TURN] = CState_Leviathan_Jump_Turn::Create(this, STATE_BOSS_LEVIATHAN_JUMP_TURN);

	m_States[STATE_BOSS_LEVIATHAN_DEAD] = CState_Leviathan_Dead::Create(this, STATE_BOSS_LEVIATHAN_DEAD);
	
	//m_States[STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR] = CState_Leviathan_Look_Center_Roar::Create(this, STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR);
	

#pragma endregion Enemy_Boss



#pragma region Npc

	m_States[STATE_NCP_SHIRLEY_IDLE] = CState_Shirley_Idle::Create(this, STATE_NCP_SHIRLEY_IDLE);
	m_States[STATE_NCP_SHIRLEY_BRILLIANT] = CState_Shirley_Brilliant::Create(this, STATE_NCP_SHIRLEY_BRILLIANT);
	m_States[STATE_NCP_SHIRLEY_GREETING] = CState_Shirley_Greeting::Create(this, STATE_NCP_SHIRLEY_GREETING);
	m_States[STATE_NCP_SHIRLEY_HUNGRY] = CState_Shirley_Hungry::Create(this, STATE_NCP_SHIRLEY_HUNGRY);
	m_States[STATE_NCP_SHIRLEY_SURPRISE] = CState_Shirley_Surprise::Create(this, STATE_NCP_SHIRLEY_SURPRISE);
	m_States[STATE_NCP_SHIRLEY_THANK] = CState_Shirley_Thank::Create(this, STATE_NCP_SHIRLEY_THANK);
	m_States[STATE_NCP_SHIRLEY_CONVERSATION_A] = CState_Shirley_Conversation_A::Create(this, STATE_NCP_SHIRLEY_CONVERSATION_A);
	m_States[STATE_NCP_SHIRLEY_CONVERSATION_B] = CState_Shirley_Conversation_B::Create(this, STATE_NCP_SHIRLEY_CONVERSATION_B);
	m_States[STATE_NCP_SHIRLEY_CONVERSATION_C] = CState_Shirley_Conversation_C::Create(this, STATE_NCP_SHIRLEY_CONVERSATION_C);
	m_States[STATE_NCP_SHIRLEY_WORRY_A] = CState_Shirley_Worry_A::Create(this, STATE_NCP_SHIRLEY_WORRY_A);
	m_States[STATE_NCP_SHIRLEY_WORRY_B] = CState_Shirley_Worry_B::Create(this, STATE_NCP_SHIRLEY_WORRY_B);

#pragma endregion Npc

	return S_OK;
}

void CState_Manager::Free()
{
	__super::Free();

	for (auto& pState : m_States)
	{
		Safe_Release(pState);
	}
}
