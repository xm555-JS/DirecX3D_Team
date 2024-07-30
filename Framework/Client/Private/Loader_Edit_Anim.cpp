#include "stdafx.h"
#include "..\Public\Loader_Edit_Anim.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"

#include "Camera_Free.h"
#include "Camera_Fixed.h"

#include "Sky.h"
#include "AnimObject.h"
#include "AnimGround.h"
#include "Weapon_Tool.h"

CLoader_Edit_Anim::CLoader_Edit_Anim()
{
}

HRESULT CLoader_Edit_Anim::Initialize()
{

	return S_OK;
}

HRESULT CLoader_Edit_Anim::Loading_Object()
{
	USEGAMEINSTANCE;

	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_FREE), CCamera_Free::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_FIXED), CCamera_Fixed::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MODELTEST, PLAYER_TESTOBJ), CAnimObject::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(SKY), CSky::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(BACKGROUND), CAnimGround::Create()));

	/* For.Weapon*/
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, WEAPON_TOOL), CWeapon_Tool::Create()));

	//FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, STONE1), CTestMesh1::CModel::Create()));

	return S_OK;
}

HRESULT CLoader_Edit_Anim::Loading_Texture()
{
	USEGAMEINSTANCE;

	/* For.Prototype_Component_Texture_Sky */
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(TEXTURE, TEXTURE_SKY),
		CTexture::Create(TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 7)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(TEXTURE, TEXTURE_BACKGROUND),
		CTexture::Create(TEXT("../Bin/Resources/Textures/Pattern_Grid_16x16.png"), 1)));

	return S_OK;
}

HRESULT CLoader_Edit_Anim::Loading_Model()
{
#define FULLPATH "../Bin/Resources/Meshes/Edit/"
#define	FILEPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = FULLPATH;

	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));

	USEGAMEINSTANCE;



#if defined(_PLAYERMODEL)
	//Type AnimGroup
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_01),
		CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Weapon_Skate"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "Weapon_Skate"));
	
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_01),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("FemalePlayer27_Hamer"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer27_Hamer"));
	//
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_02),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("FemalePlayer28_Immortal"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer28_Immortal"));
	//
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_03),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("FemalePlayer29_mark"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer29_mark"));
	//
	////FAILEDCHECK(pGameInstance->Load_Animation(FILEPATH("FemalePlayer29_mark"), ANIM_PLAYER_FEMALE, WEAPON_MARK));

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_02),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Female_Player_Cube/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "Anim_Cube"));
		
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_04),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Female/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "SwordAnim"));
	
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_02),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("FemalePlayer09_Spear/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer09_Spear"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_05),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("AxeAnim/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "AxeAnim"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_03),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("FemalePlayer01_2gun/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "GunAnim"));
	//
	//// Player Hand
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_06),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("SickleAnim/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "SickleAnim"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_07),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand2/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer_Hand2"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_08),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand3/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer_Hand3"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_09),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand4/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer_Hand4"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_10),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand5/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer_Hand5"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_11),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand6/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer_Hand6"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_12),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand7/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer_Hand7"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_13),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand8/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer_Hand8"));

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_14),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand9/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE, 1), "FemalePlayer_Hand9"));
#endif


	Loading_MapModel();
	Loading_Model_Weapon();

#if defined(_ENEMYMODEL)
	Loading_Model_EnemyNormal();
#endif

#if defined(_BOSSMODEL)
	Loading_Model_EnemyBoss();
#endif

#if defined(_NPCMODEL)
	Loading_Model_Npc();
#endif

	return S_OK;
}

HRESULT CLoader_Edit_Anim::Loading_Shader()
{
	USEGAMEINSTANCE;

	///* For.Prototype_Component_Shader_VtxTex */
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(SHADER, SHADER_VTXCOLOR),
	//	CShader::CModel::Create(TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements))))
	//	__debugbreak();

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(SHADER, SHADER_VTXMODEL),
		CShader::Create(TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements))))
		__debugbreak();

	///* For.Prototype_Component_Shader_VtxTex */
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(SHADER, SHADER_VTXNORTEX),
	//	CShader::CModel::Create(TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements))))
	//	__debugbreak();

	return S_OK;
}

HRESULT CLoader_Edit_Anim::Loading_DynamicModel()
{
#define FULLPATH "../Bin/Resources/Meshes/Lv1/Dynamic/"
#define	FILEPATH(path) (strFullPath + path).c_str()
	string strFullPath = FULLPATH;

	//USEGAMEINSTANCE;
	//_matrix			TransformMatrix;

	///* For.Prototype_Component_Model_Cube */
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_ILLUSION_CUBE),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Illusion_Cube/Illusion_Cube"), TransformMatrix))))
	//	__debugbreak();

	///* For.Prototype_Component_Model_Cheese */
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_CHEESE),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Cheese/Cheese"), TransformMatrix))))
	//	__debugbreak();

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_CHEESE, 1),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Cheese/Cheese1"), TransformMatrix))))
	//	__debugbreak();

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_CHEESE, 2),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Cheese/Cheese2"), TransformMatrix))))
	//	__debugbreak();


//#define FULLPATH "../Bin/Resources/Meshes/Lv2/Dynamic/"
//#define	FILEPATH(path) (strFullPath + path).c_str()
//	strFullPath = FULLPATH;

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_DOOR),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Door/"), "Door.fbx", TransformMatrix))))
	//	__debugbreak();

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_FAKECUBE),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Illusion_Cube_Fake/"), "Illusion_Cube_Fake.fbx", TransformMatrix))))
	//	__debugbreak();

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_PAWN),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Chess_Pawn/"), "Chess_Pawn.fbx", TransformMatrix))))
	//	__debugbreak();

	///* For.Prototype_Component_Model_Chess */
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_KING),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Chess_King/"), "Chess_King.fbx", TransformMatrix))))
	//	__debugbreak();

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_MOON),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Moon/"), "Moon.fbx", TransformMatrix))))
	//	__debugbreak();	

	//TransformMatrix = XMMatrixScaling(0.02f, 0.02f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_ENDDOOR),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Moon/"), "EndDoor.fbx", TransformMatrix))))
	//	__debugbreak();

	return S_OK;
}

HRESULT CLoader_Edit_Anim::Loading_MapModel()
{

	return S_OK;
}

HRESULT CLoader_Edit_Anim::Loading_Model_EnemyNormal()
{

	USEGAMEINSTANCE;
	_matrix			TransformMatrix;

#pragma region Gang_Swarm
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, HYENAGANG_MINIONS),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/HyenaGang_Minions/", "HyenaGang_Minions.fbx", TransformMatrix), "HyenaGang_Minions"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, HYENAGANG_SHOOTER),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/HyenaGang_Shooter/", "HyenaGang_Shooter.fbx", TransformMatrix), "HyenaGang_Shooter"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, GANGSTER_CAP),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Gangster_Cap/", "Gangster_Cap.fbx", TransformMatrix), "Gangster_Cap"));
#pragma endregion

#pragma region Mutation_Swarm
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MUTATION),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Mutation/", "Mutation.fbx", TransformMatrix), "Mutation"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MUTATION_MECHANIC),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Mutation_Mechanic/", "Mutation_Mechanic.fbx", TransformMatrix), "Mutation_Mechanic"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, HABAKA_ADMIRAL),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Habaka_Admiral/", "Habaka_Admiral.fbx", TransformMatrix), "Habaka_Admiral"));
#pragma endregion

#pragma region MechanicalCity_Swarm
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MECHANICALCITY_DOORKEEPER),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/MechanicalCity_DoorKeeper/", "MechanicalCity_DoorKeeper.fbx", TransformMatrix), "MechanicalCity_DoorKeeper"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MECHANICALCITY_DOUBLEEDGEDSWORDADEPT),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/MechanicalCity_DoubleEdgedSwordAdept/", "MechanicalCity_DoubleEdgedSwordAdept.fbx", TransformMatrix), "MechanicalCity_DoubleEdgedSwordAdept"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MECHANICALCITY_GUNNER),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/MechanicalCity_Gunner/", "MechanicalCity_Gunner.fbx", TransformMatrix), "MechanicalCity_Gunner"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MECHANICALCITY_SWORDGHOST),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/MechanicalCity_SwordGhost/", "MechanicalCity_SwordGhost.fbx", TransformMatrix), "MechanicalCity_SwordGhost"));
#pragma endregion

	return S_OK;
}

HRESULT CLoader_Edit_Anim::Loading_Model_EnemyBoss()
{
#define FULLPATH "../Bin/Resources/Meshes/Enemy/Enemy_Boss/"
#define	FILEPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = FULLPATH;

	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));

	USEGAMEINSTANCE;

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_FRANKEN),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Franken"), "Boss_Hum_001.fbx", TransformMatrix), "Boss_Franken"));

	TransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_FROST_DRAGON),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Jumping_Frost_Dragon"), "boss_hum_005_Skeleton.fbx", TransformMatrix), "Frost_Dragon"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_VULCANUS),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Vulcanus"), "boss_hum_008_skin_Skeleton.fbx", TransformMatrix), "Boss_Vulcanus"));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_ANGEL),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Angel"), "boss_hum_010_skin_Skeleton.fbx", TransformMatrix), "Boss_Angel"));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_SEVEC),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Sevec"), "boss_hum_015_skin_Skeleton.fbx", TransformMatrix), "Boss_Sevec"));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_FROST_MECA),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Frost_Meca"), "boss_hum_017_skin_Skeleton.fbx", TransformMatrix), "Boss_Frost_Meca"));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_NEMESIS),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Nemesis"), "boss_hum_029_skin_Skeleton.fbx", TransformMatrix), "Boss_Nemesis"));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_RUDOLF),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Rudolf"), "boss_hum_030_skin_Skeleton.fbx", TransformMatrix), "Boss_Rudolf"));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_WEAVER),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Weaver"), "mon_hum_072_Skin_Skeleton.fbx", TransformMatrix), "Boss_Weaver"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_ROBELLA),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Robella"), "BOSS_hum_006_skin_Skeleton.FBX", TransformMatrix), "Boss_Robella"));

	TransformMatrix = XMMatrixScaling(0.001f, 0.001f, 0.001f);
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_LEVIATHAN_40),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Leviathan_40m"), "boss_hum_035_skin_Skeleton.fbx", TransformMatrix), "Boss_Leviathan_40m"));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_LEVIATHAN_90),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Leviathan_90m"), "boss_hum_035_skin_Skeleton.fbx", TransformMatrix), "Boss_Leviathan_90m"));

	TransformMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_BULLET, MODEL_ENEMY_BULLET_DRAGON),
		CModel::Create(TYPE_ANIM, FILEPATH("Bullet_Dragon"), "boss_hum_035_skin_Skeleton.FBX", TransformMatrix), "Bullet_Dragon"));

	return S_OK;
}

HRESULT CLoader_Edit_Anim::Loading_Model_Npc()
{
#define FULLPATH "../Bin/Resources/Meshes/Npc/"
#define	FILEPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = FULLPATH;

	USEGAMEINSTANCE;
	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationZ(XMConvertToRadians(90.0f))* XMMatrixRotationY(XMConvertToRadians(270.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_NPC, MODEL_SHIRLEY),
		CModel::Create(TYPE_ANIM, FILEPATH("Shirley"), "npc_shu_027_Skeleton.FBX", TransformMatrix), "Shirley"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_NPC, MODEL_COOKER),
		CModel::Create(TYPE_ANIM, FILEPATH("Cooker"), "Cooker.FBX", TransformMatrix), "Cooker"));

	return S_OK;
}

HRESULT CLoader_Edit_Anim::Loading_Model_Weapon()
{
#define FULLPATH "../Bin/Resources/Meshes/"
#define	FILEPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = FULLPATH;

	USEGAMEINSTANCE;
	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_SWORD),
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon"), "PWeapon_Sword_Level40_L1.fbx", TransformMatrix), "Weapon_Sword_1"));

	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_2GUN),
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon"), "PWeapon_Gun_SR.fbx", TransformMatrix), "Weapon_Gun_1"));

	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationZ(XMConvertToRadians(-90.0f));
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_SHIELD),
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon_ShieldAxe"), "PWeapon_Shieldaxe_Level10_a.fbx", TransformMatrix), "Weapon_Shield_1"));

	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_AXE),
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon_ShieldAxe"), "PWeapon_Shieldaxe_Level10_b.fbx", TransformMatrix), "Weapon_Axe_1"));

	//Mobility
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_MOBILITY_GLIDER),
		CModel::Create(TYPE_NONANIM, FILEPATH("Mobility_Glider"), "artifact_item_021.fbx", TransformMatrix), "Mobility_Glider"));

	TransformMatrix = XMMatrixScaling(0.004f, 0.004f, 0.004f);// *XMMatrixRotationX(XMConvertToRadians(-90.0f))* XMMatrixRotationZ(XMConvertToRadians(90.0f));
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_DYNAMIC, MODEL_ANIM_WEAPON, MODEL_MOBILITY_MOTORCYCLE),
		CModel::Create(TYPE_ANIM, FILEPATH("Mobility_MotorCycle"), "Mount_018_skin_Skeleton.fbx", TransformMatrix), "Mobility_MotorCycle"));

	/*Weapon_Enemy_Normal*/
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_ENEMY_WEAPON_STIC),
		CModel::Create(TYPE_NONANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/HyenaGang_Minions/Weapon_Stic/", "Weapon_Stic.fbx", TransformMatrix), "Weapon_Stic"));
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_ENEMY_WEAPON_BOW),
		CModel::Create(TYPE_NONANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/HyenaGang_Shooter/Weapon_Bow/", "Weapon_Bow.fbx", TransformMatrix), "Weapon_Bow"));
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_ENEMY_WEAPON_WASTEBASKET),
		CModel::Create(TYPE_NONANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Gangster_Cap/Weapon_Wastebasket/", "Weapon_Wastebasket.fbx", TransformMatrix), "Weapon_Wastebasket"));
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_EDIT_ANIM, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_ENEMY_WEAPON_HAMMER),
		CModel::Create(TYPE_NONANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Gangster_Cap/Weapon_Hammer/", "Weapon_Hammer.fbx", TransformMatrix), "Weapon_Hammer"));

	return S_OK;
}


CLoader_Edit_Anim * CLoader_Edit_Anim::Create()
{
	CLoader_Edit_Anim*		pInstance = new CLoader_Edit_Anim();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLoader_Edit_Anim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Edit_Anim::Free()
{
}

