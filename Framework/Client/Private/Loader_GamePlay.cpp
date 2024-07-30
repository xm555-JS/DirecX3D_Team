#include "stdafx.h"
#include "..\Public\Loader_GamePlay.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"

#include "Camera_Free.h"
#include "Camera_Fixed.h"
#include "Camera_Target_Position_Lerp.h"
#include "Camera_Action.h"

#include "Sky.h"
#include "Cloud.h"
#include "Cloud_Sky.h"

/* For.Player */
#include "Player.h"
#include "Bullet_Player_2Gun.h"
#include "Bullet_Player_2Gun_Zoom.h"
#include "Bullet_Player_2Gun_Normal.h"
#include "Bullet_Player_2Gun_Special.h"
#include "Bullet_Player_2Gun_Special_Sub.h"
#include "Bullet_Player_Cube.h"
#include "Bullet_Player_Cube_Main.h"
#include "Time_Illusion.h"

/* For.Map */
#include "Tile.h"
#include "Rock.h"
#include "Tree.h"
#include "Grass.h"
#include "MapObj_Instance.h"
#include "Terrain.h"
#include "Stair.h"
#include "Structs.h"
#include "Water.h"
#include "Water_Base.h"
#include "Terrain_NAvi.h"
#include "Item.h"
#include "Gray_Rock.h"
#include "TestPlayer.h"
#include "Grass_Desert.h"
#include "Structs_Desert.h"
#include "Rock_All.h"
#include "GrassMesh.h"
#include "Room.h"
#include "Equipment.h"
#include "Structs_All.h"
#include "Shelf_Item.h"
#include "Coin.h"
#include "Ring.h"

#include "Portal.h"
#include "Region.h"

#include "Navigation.h"

/* For.Enemy_Normal */

#include "Mutation.h"
#include "Mutation_Mechanic.h"
#include "HunterDog.h"
#include "Mutation_HunterDog.h"
#include "Habaka_Admiral.h"
#include "Mutation_Orc.h"

#include "HyenaGang_Minions.h"
#include "HyenaGang_Shooter.h"
#include "Gangster_Cap.h"


#include "MechanicalCity_DoubleEdgedSwordAdept.h"
#include "MechanicalCity_SwordGhost.h"
#include "MechanicalCity_Gunner.h"
#include "MechanicalCity_DoorKeeper.h"

#include "Leviathan_Crystal.h"


/* For.Enemy_Boss */
#include "Enemy_Boss_Dragon.h"
#include "Enemy_Boss_Weaver.h"
#include "Enemy_Boss_Robella.h"
#include "Enemy_Boss_Leviathan.h"

/* For.Enemy_Bullet*/
#include "Enemy_Bullet_Dragon.h"
#include "Enemy_Thunder_Dragon.h"

#include "Enemy_Weaver_Frozen.h"
#include "Enemy_Weaver_Hurricane.h"
#include "Enemy_Weaver_ShockWave.h"

#include "Enemy_Robella_FireBall.h"
#include "Enemy_Robella_FireBall2.h"
#include "Enemy_Robella_FireStrom.h"
#include "Enemy_Robella_FireStrom2.h"
#include "Enemy_Robella_PoisonFog.h"

#include "Enemy_Leviathan_FireBall.h"
#include "Enemy_Leviathan_Thunder.h"
#include "Enemy_Leviathan_IceSpear.h"

#include "Enemy_Arrow.h"


/* For.Enemy_Boss_Trigger */
#include "Dragon_Appear_Trigger.h"

/* For.Effect */
#include "Effect_Mesh.h"
#include "Effect_Rect.h"
#include "Effect_Point.h"
#include "Effect_Trail.h"

/* For.Weapon*/
#include "Weapon_Sword.h"
#include "Weapon_2Gun.h"
#include "Weapon_ShieldAxe.h"
#include "Weapon_CubeL.h"
#include "Weapon_CubeR.h"

/* For.Mobility*/
#include "Mobility_Glider.h"
#include "Mobility_MotorCycle.h"

/*For.Weapon_Enemy*/
#include "Weapon_Stic.h"
#include "Weapon_Bow.h"
#include "Weapon_Wastebasket.h"
#include "Weapon_Hammer.h"

 /*For.Npc*/
#include "Npc_Shirley.h"
#include "Npc_Cooker.h"

CLoader_GamePlay::CLoader_GamePlay()
{
}

HRESULT CLoader_GamePlay::Initialize()
{
	return S_OK;


}

HRESULT CLoader_GamePlay::Loading_Object()
{
	USEGAMEINSTANCE;

	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_FREE), CCamera_Free::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_FIXED), CCamera_Fixed::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_TARGET_POSITION_LERP), CCamera_Target_Position_Lerp::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(CAMERA, CAMERA_ACTION), CCamera_Action::Create()));

#if defined(_PLAYER)
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(PLAYER), CPlayer::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(PLAYER, WEAPON_2GUN_BULLET), CBullet_Player_2Gun::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(PLAYER, WEAPON_2GUN_BULLET_ZOOM), CBullet_Player_2Gun_Zoom::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(PLAYER, WEAPON_2GUN_BULLET_NORMAL), CBullet_Player_2Gun_Normal::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(PLAYER, WEAPON_2GUN_BULLET_SPECIAL), CBullet_Player_2Gun_Special::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(PLAYER, WEAPON_2GUN_BULLET_SPECIAL_SUB), CBullet_Player_2Gun_Special_Sub::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(PLAYER, WEAPON_CUBE_BULLET), CBullet_Player_Cube::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(PLAYER, WEAPON_CUBE), CBullet_Player_Cube_Main::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(PLAYER, WEAPON_TIME_ILLUSION), CTime_Illusion::Create()));
#endif

#if defined(_SKYBOX)
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(SKY), CSky::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(SKY, MAP_CLOUD), CCloud::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(SKY, MAP_CLOUD_SKY), CCloud_Sky::Create()));
#endif

#if defined(_MAP)
	/* For.Map */
	//FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_TILE, MODEL_TILE), CTile::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_ROCK, MODEL_ROCK), CRock::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_ROCK_ALL, MODEL_ROCK_ALL), CRock_All::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_GRAY_ROCK, MDOEL_GRAY_ROCK), CGray_Rock::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_TREE, MODEL_TREE), CTree::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_GRASS, MODEL_GRASS), CGrass::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_GRASS_DESERT, MODEL_GRASS_DESERT), CGrass_Desert::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_GRASSMESH, MODEL_GRASSMESH), CGrassMesh::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_STAIR, MODEL_STAIR), CStair::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_STRUCTS, MODEL_STRUCTS), CStructs::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_STRUCTS_DESERT, MODEL_STRUCTS_DESERT), CStructs_Desert::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_ROOM, MODEL_ROOM), CRoom::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_EQUIPMENT, MODEL_EQUIPMENT), CEquipment::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_TEST_PLAYER, MODEL_TESTPLAYER), CTestPlayer::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_INSTANCE), CMapObj_Instance::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_PORTAL), CPortal::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_REGION), CRegion::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_STRUCTS_ALL,MODEL_STRUCTS_ALL), CStructs_All::Create()));

	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_COIN, MODEL_COIN), CCoin::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_RING, MODEL_RING), CRing::Create()));
#endif

#if defined(_ITEM)
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_ITEM, MODEL_ITEM), CItem::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_SHELF_ITEM, MODEL_SHELF_ITEM), CShelf_Item::Create()));
#endif

#if defined(_TERRAIN)
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_TERRAIN), CTerrain::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_TRIGGER), CDragon_Appear_Trigger::Create()));
	//FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_NAVI), CTerrain_Navi::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_WATER), CWater::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(MAP, MAP_WATER_BASE), CWater_Base::Create()));
#endif

#if defined(_ENEMYNORMAL)
	/* For.Enemy */
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_ARROW), CEnemy_Arrow::Create()));


	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, HYENAGANG_MINIONS), CHyenaGang_Minions::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, HYENAGANG_SHOOTER), CHyenaGang_Shooter::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, GANGSTER_CAP), CGangster_Cap::Create()));


	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION), CMutation::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION_MECHANIC), CMutation_Mechanic::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, HABAKA_ADMIRAL), CHabaka_Admiral::Create()));


	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, HUNTERDOG), CHunterDog::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION_HUNTERDOG), CMutation_HunterDog::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION_ORC), CMutation_Orc::Create()));



	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_DOUBLEEDGEDSWORDADEPT), CMechanicalCity_DoubleEdgedSwordAdept::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_SWORDGHOST), CMechanicalCity_SwordGhost::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_GUNNER), CMechanicalCity_Gunner::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_DOORKEEPER), CMechanicalCity_DoorKeeper::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL), CLeviathan_Crystal::Create()));
#endif

#if defined(_ENEMYBOSS)
	/* For.Enemy_Boss */
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON), CEnemy_Boss_FrostDragon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_WEAVER), CEnemy_Boss_Weaver::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_ROBELLA), CEnemy_Boss_Robella::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_LEVIATHAN), CEnemy_Boss_Leviathan::Create()));

	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_FROSTDRAGON), CEnemy_Bullet_FrostDragon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_THUNDER), CEnemy_Thunder_FrostDragon::Create()));

	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_FROZEN), CEnemy_Weaver_Frozen::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_HURRICANE), CEnemy_Weaver_Hurricane::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_SHOCKWAVE), CEnemy_Weaver_ShockWave::Create()));

	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIREBALL), CEnemy_Robella_FireBall::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIREBALL2), CEnemy_Robella_FireBall2::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIRESTROM), CEnemy_Robella_FireStrom::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIRESTROM2), CEnemy_Robella_FireStrom2::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_POISONFOG), CEnemy_Robella_PoisonFog::Create()));

	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_LEVIATHAN_FIREBALL), CEnemy_Leviathan_FireBall::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_LEVIATHAN_THUNDER), CEnemy_Leviathan_Thunder::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_LEVIATHAN_ICESPEAR), CEnemy_Leviathan_IceSpear::Create()));

#endif

#if defined(_TEST_SHIN)
	/* For.Enemy_Boss */
	//FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON), CEnemy_Boss_FrostDragon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_WEAVER), CEnemy_Boss_Weaver::Create()));
	//FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_ROBELLA), CEnemy_Boss_Robella::Create()));

	//FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_FROSTDRAGON), CEnemy_Bullet_FrostDragon::Create()));
	//FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_THUNDER), CEnemy_Thunder_FrostDragon::Create()));
#endif


#if defined(_EFFECT)
	/* For.Effect */
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(EFFECT, EFFECT_MESH), CEffect_Mesh::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(EFFECT, EFFECT_PARTICLE_RECT), CEffect_Rect::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(EFFECT, EFFECT_PARTICLE_POINT), CEffect_Point::Create()));
#endif
#ifdef _TEST_HAN
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(EFFECT, EFFECT_TRAIL), CEffect_Trail::Create()));
#endif // _TEST_HAN


#if defined(_WEAPON)
	/* For.Weapon*/
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, WEAPON_SWORD), CWeapon_Sword::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, WEAPON_2GUN), CWeapon_2Gun::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, WEAPON_SHIELDAXE), CWeapon_ShieldAxe::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, WEAPON_CUBE), CWeapon_CubeL::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, WEAPON_CUBE), CWeapon_CubeR::Create()));
	/* For.Mobility*/
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, MOBILITY_GLIDER), CMobility_Glider::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, MOBILITY_MOTORCYCLE), CMobility_MotorCycle::Create()));
	/* For.Weapon_Enemy*/
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, WEAPON_ENEMY_STIC), CWeapon_Stic::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, WEAPON_ENEMY_BOW), CWeapon_Bow::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, WEAPON_ENEMY_WASTEBASKET), CWeapon_Wastebasket::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(WEAPON, WEAPON_ENEMY_HAMMER), CWeapon_Hammer::Create()));
#endif

#if defined(_NPC)
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(NPC, NPC_SHIRLEY), CNpc_Shirley::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(NPC, NPC_COOKER), CNpc_Cooker::Create()));
#endif

	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_Texture()
{
	USEGAMEINSTANCE;

#if defined(_SKYBOX)
	/* For.Prototype_Component_Texture_Sky */
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_SKY),
		CTexture::Create(TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 7)));
#endif

#if defined(_MAP)
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_CLOUD),
		CTexture::Create(TEXT("../Bin/Resources/Textures/Cloud/Cloud_%d.png"), 11)));
#endif

	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN), 
	//	CTexture::Create(TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds"))));

#if defined(_TERRAIN)
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN, TYPE_DIFFUSE),
		CTexture::Create(TEXT("../Bin/Resources/Textures/Terrain/Diffuse/Diffuse_%d.png"), 9)));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_TERRAIN, TYPE_FILTER),
		CTexture::Create(TEXT("../Bin/Resources/Textures/Terrain/Filter/Filter_%d.png"), 2)));
#endif


#define	MASKPATH "../Bin/Resources/Textures/Effect/Mask/"
#define	NOISEPATH "../Bin/Resources/Textures/Effect/Noise/"
#if defined(_EFFECT)
	/* For.Effect */
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK), CTexture::Create(string(MASKPATH))));
#endif

#if defined(_NOISE_TEX)
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), CTexture::Create(string(NOISEPATH))));
#endif


	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_NORMAL, TEXTURE_WATER), CTexture::Create(TEXT("../Bin/Resources/Textures/Water/Water_Norm_%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_NOISE, TEXTURE_WATER), CTexture::Create(TEXT("../Bin/Resources/Textures/Water/Water_Distor_%d.png"), 4)));


	
	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_Model()
{
	USEGAMEINSTANCE;

	/* For.Prototype_Component_VIBuffer_Cube*/
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_CUBE), CVIBuffer_Cube::Create()));

#if defined(_PLAYER)
	Loading_Model_Player();
#endif

#if defined(_ENEMYNORMAL)
	Loading_Model_EnemyNormal();
#endif

#if defined(_ENEMYBOSS)
	Loading_Model_EnemyBoss();
#endif

#if defined(_TEST_SHIN)
	//Loading_Model_EnemyBoss();
#endif

#if defined(_MAP)
	Loading_MapModel();

#endif

#if defined(_ITEM)
	Loading_ItemModel();
#endif

#if defined(_WEAPON)
	Loading_Model_Weapon();
	Loading_Model_Bullet();
#endif

#if defined(_TERRAIN)
	Loading_MapBuffer(); 
	Loading_NonAnimModel();
#endif

#if defined(_EFFECT)
	Loading_EffectModel();
#endif

#if defined(_NPC)
	Loading_Model_Npc();
#endif

	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_Shader()
{


	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_MapBuffer()
{
	USEGAMEINSTANCE;

	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_TERRAIN), 
	//	CVIBuffer_Terrain::Create(TEXT("../Bin/Resources/Textures/Terrain/HeightTest7.bmp"))));



	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_TERRAIN_TESS),
	//	CVIBuffer_Terrain_Tess::Create(TEXT("../Bin/Resources/Textures/Terrain/Hight/Hight_Fin.bmp"))));


//#if defined(_TESSELLATION)
//	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_TERRAIN),
//		CVIBuffer_Terrain_Tess_Custom::Create(TEXT("../Bin/Resources/Textures/Terrain/Hight/Hight_Fin.bmp"))));
//#else
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_TERRAIN),
		CVIBuffer_Terrain::Create(TEXT("../Bin/Resources/Textures/Terrain/Hight/Hight_Fin.bmp"))));
//#endif

	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_NAVI),
	//	CVIBuffer_Terrain_Custom::Create(TEXT("../Bin/Resources/Textures/Terrain/Hight/Hight_Navi.bmp"))));

	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_MapModel()
{
#define	MAPPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = "../Bin/Resources/Meshes/Lv1/Map/";

	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixIdentity();

	// 폴더 하나에 각각 이름으로 폴더가 또 존재함.
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Rock_All"), MODEL_ROCK_ALL, XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f))); // 폴더 하나에 파일 다 모아져 있음.

	Add_MapModelPrototypeFromOneFolder(MAPPATH("Rock"), MODEL_ROCK, TransformMatrix); // 폴더 하나에 파일 다 모아져 있음.
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Gray_Rock"), MDOEL_GRAY_ROCK, XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f))); // 폴더 하나에 파일 다 모아져 있음.
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Tree"), MODEL_TREE, TransformMatrix);
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Grass"), MODEL_GRASS, TransformMatrix);
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Grass_Desert"), MODEL_GRASS_DESERT, XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
	Add_MapModelPrototypeFromOneFolder(MAPPATH("GrassMesh"), MODEL_GRASSMESH, XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Stair"), MODEL_STAIR, TransformMatrix);
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Equipment"), MODEL_EQUIPMENT, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(-90.0f)));
	Add_MapModelPrototypeFromOneFolder(MAPPATH("PlayerModel"), MODEL_TESTPLAYER, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(180.0f)) * XMMatrixRotationZ(XMConvertToRadians(180.0f)));

	// 미니게임
	//Add_MapModelPrototypeFromOneFolder(MAPPATH("Minigame"), MODEL_COIN, XMMatrixScaling(0.015f, 0.015f, 0.015f));
	//Add_MapModelPrototypeFromOneFolder(MAPPATH("Minigame"), MODEL_RING, XMMatrixScaling(0.015f, 0.015f, 0.015f));

#if defined(_STRUCT)
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Structs"), MODEL_STRUCTS, TransformMatrix);
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Structs_Desert"), MODEL_STRUCTS_DESERT, XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));

	Add_MapModelPrototypeFromOneFolder(MAPPATH("Room"), MODEL_ROOM, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
#endif
#if defined(_STRUCT_ALL)
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Structs_All"), MODEL_STRUCTS_ALL, XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
#endif											

//	Add_MapModelPrototypeFromOneFolder(MAPPATH("Tree"), MODEL_TREE, XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
//	Add_MapModelPrototypeFromOneFolder(MAPPATH("Grass"), MODEL_GRASS, XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
//	Add_MapModelPrototypeFromOneFolder(MAPPATH("Stair"), MODEL_STAIR, XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
//
//#if defined(_STRUCT)
//	Add_MapModelPrototypeFromOneFolder(MAPPATH("Structs"), MODEL_STRUCTS, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
//#endif

	//Add_MapModelPrototypeFromEachFolder(MAPPATH("Tile"), MODEL_TILE, TransformMatrix); // 폴더 하나 안에 각 파일별로 폴더가 존재.

	/*For Navigation*/																																// Map_Navi
	//FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(NAVIGATION), CNavigation::Create(TEXT("../Bin/Resources/Json/Navigation/Test_Nav.bin"))));
	//FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(NAVIGATION), CNavigation::Create(TEXT("../Bin/Resources/Json/Navigation/Map_Navi.bin"))));
	//FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(NAVIGATION), CNavigation::Create(TEXT("../Bin/Resources/Json/Navigation/Map_Navi.bin"))));//Navi_Terrain_Default
	//FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(NAVIGATION), CNavigation::Create(TEXT("../Bin/Resources/Json/Navigation/Navi_Terrain_Default.bin"))));//테스트네비
	//FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(NAVIGATION), CNavigation::Create(TEXT("../Bin/Resources/Json/Navigation/Map_Navi_default_Water11.bin"))));
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(NAVIGATION), CNavigation::Create(TEXT("../Bin/Resources/Json/Navigation/Map_Navi_0111.bin"))));

	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_ItemModel()
{
#define	MAPPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = "../Bin/Resources/Meshes/Lv1/Map/";

	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f);

	Add_MapModelPrototypeFromOneFolder(MAPPATH("Item"), MODEL_ITEM, TransformMatrix);// 폴더 하나에 파일 다 모아져 있음.
	Add_MapModelPrototypeFromOneFolder(MAPPATH("Item"), MODEL_SHELF_ITEM, TransformMatrix); // 인벤토리 선반에 올려져있는 아이템 모델용

	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_Model_Player()
{
#define FULLPATH "../Bin/Resources/Meshes/Edit/"
#define	FILEPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = FULLPATH;

	USEGAMEINSTANCE;
	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationZ(XMConvertToRadians(90.0f));
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(-90.0f));
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(0.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_01),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("FemalePlayer27_Hamer"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer27_Hamer"));
	//
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_02),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("FemalePlayer28_Immortal"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer28_Immortal"));
	//
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_03),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("FemalePlayer29_mark"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer29_mark"));


	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_01),
		CModel::Create(TYPE_ANIM_GROUP, "../Bin/Resources/Meshes/Player/Player_Female1/", "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer_sword"));

	// Player Hand
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_05),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand1/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer_Hand1"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_06),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand2/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer_Hand2"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_07),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand3/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer_Hand3"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_08),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand4/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer_Hand4"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_09),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand5/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer_Hand5"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_10),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand6/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer_Hand6"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_11),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand7/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer_Hand7"));
	//
	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_PLAYER, MODEL_PLAYER_FEMAIL_12),
	//	CModel::Create(TYPE_ANIM_GROUP, FILEPATH("Player_Hand/Hand8/"), "f_player_Skeleton.fbx", TransformMatrix, ANIM_PLAYER_FEMALE), "FemalePlayer_Hand8"));

	FAILEDCHECK(pGameInstance->Load_Animation("../Bin/Resources/Meshes/Player/", ANIM_PLAYER_FEMALE));

	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_Model_EnemyNormal()
{
	USEGAMEINSTANCE;
	_matrix			TransformMatrix;

	/* For.Prototype_Component_VIBuffer_Cube*/
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(VIBUFFER, BUFFER_CUBE), CVIBuffer_Cube::Create()));

#pragma region Gang_Swarm
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, HYENAGANG_MINIONS),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/HyenaGang_Minions/", "HyenaGang_Minions.fbx", TransformMatrix), "HyenaGang_Minions"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, HYENAGANG_SHOOTER),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/HyenaGang_Shooter/", "HyenaGang_Shooter.fbx", TransformMatrix), "HyenaGang_Shooter"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, GANGSTER_CAP),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Gangster_Cap/", "Gangster_Cap.fbx", TransformMatrix), "Gangster_Cap"));
#pragma endregion

#pragma region Mutation_Swarm
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MUTATION),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Mutation/", "Mutation.fbx", TransformMatrix), "Mutation"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MUTATION_MECHANIC),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Mutation_Mechanic/", "Mutation_Mechanic.fbx", TransformMatrix), "Mutation_Mechanic"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, HABAKA_ADMIRAL),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Habaka_Admiral/", "Habaka_Admiral.fbx", TransformMatrix), "Habaka_Admiral"));
#pragma endregion


	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, HUNTERDOG),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/HunterDog/", "HunterDog.fbx", TransformMatrix), "HunterDog"));

	////////////////////////////////////////////////ONLY_IDLE
	
	
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MUTATION_HUNTERDOG),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Mutation_HunterDog/", "Mutation_HunterDog.fbx", TransformMatrix), "Mutation_HunterDog"));

	

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MUTATION_ORC),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Mutation_Orc/", "Mutation_Orc.fbx", TransformMatrix), "Mutation_Orc"));


	
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MECHANICALCITY_DOUBLEEDGEDSWORDADEPT),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/MechanicalCity_DoubleEdgedSwordAdept/", "MechanicalCity_DoubleEdgedSwordAdept.fbx", TransformMatrix), "MechanicalCity_DoubleEdgedSwordAdept"));
	
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MECHANICALCITY_SWORDGHOST),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/MechanicalCity_SwordGhost/", "MechanicalCity_SwordGhost.fbx", TransformMatrix), "MechanicalCity_SwordGhost"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MECHANICALCITY_GUNNER),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/MechanicalCity_Gunner/", "MechanicalCity_Gunner.fbx", TransformMatrix), "MechanicalCity_Gunner"));
	
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MECHANICALCITY_DOORKEEPER),
		CModel::Create(TYPE_ANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/MechanicalCity_DoorKeeper/", "MechanicalCity_DoorKeeper.fbx", TransformMatrix), "MechanicalCity_DoorKeeper"));

	
	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_Model_EnemyBoss()
{
#define FULLPATH "../Bin/Resources/Meshes/Enemy/Enemy_Boss/"
#define	FILEPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = FULLPATH;

	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	USEGAMEINSTANCE;

	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_FRANKEN),
	//	CModel::Create(TYPE_ANIM, FILEPATH("Boss_Franken"), "Boss_Hum_001.fbx", TransformMatrix), "Boss_Franken"));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_FROST_DRAGON),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Jumping_Frost_Dragon"), "boss_hum_005_Skeleton.fbx", TransformMatrix), "Frost_Dragon"));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_LEVIATHAN_40),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Leviathan_40m"), "boss_hum_035_skin_Skeleton.fbx", XMMatrixScaling(3.f, 3.f, 3.f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * TransformMatrix), "Boss_Leviathan_40m"));

	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_LEVIATHAN_90),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Leviathan_90m"), "boss_hum_035_skin_Skeleton.fbx", TransformMatrix), "Boss_Leviathan_90m"));

	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_VULCANUS),
	//	CModel::Create(TYPE_ANIM, FILEPATH("Boss_Vulcanus"), "boss_hum_008_skin_Skeleton.fbx", TransformMatrix), "Boss_Vulcanus"));

	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_ANGEL),
	//	CModel::Create(TYPE_ANIM, FILEPATH("Boss_Angel"), "boss_hum_010_skin_Skeleton.fbx", TransformMatrix), "Boss_Angel"));

	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_SEVEC),
	//	CModel::Create(TYPE_ANIM, FILEPATH("Boss_Sevec"), "boss_hum_015_skin_Skeleton.fbx", TransformMatrix), "Boss_Sevec"));

	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_FROST_MECA),
	//	CModel::Create(TYPE_ANIM, FILEPATH("Boss_Frost_Meca"), "boss_hum_017_skin_Skeleton.fbx", TransformMatrix), "Boss_Frost_Meca"));

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_NEMESIS),
	//	CModel::Create(TYPE_ANIM, FILEPATH("Boss_Nemesis"), "boss_hum_029_skin_Skeleton.fbx", TransformMatrix), "Boss_Nemesis"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_WEAVER),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Weaver"), "mon_hum_072_Skin_Skeleton.fbx", TransformMatrix), "Boss_Weaver"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_ROBELLA),
		CModel::Create(TYPE_ANIM, FILEPATH("Boss_Robella"), "BOSS_hum_006_skin_Skeleton.fbx", TransformMatrix), "Boss_Robella"));

	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_Model_Weapon()
{
#define FULLPATH "../Bin/Resources/Meshes/"
#define	FILEPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = FULLPATH;

	/*Enemy_Player*/
	USEGAMEINSTANCE;
	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_SWORD), 
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon"), "PWeapon_Sword_Level40_L1.fbx", TransformMatrix), "Weapon_Sword_1"));

	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_2GUN),
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon"), "PWeapon_Gun_SR.fbx", TransformMatrix), "Weapon_Gun_1"));

	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationZ(XMConvertToRadians(-90.0f));
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_SHIELD),
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon_ShieldAxe"), "PWeapon_Shieldaxe_Level10_a.fbx", TransformMatrix), "Weapon_Shield_1"));

	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_AXE),
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon_ShieldAxe"), "PWeapon_Shieldaxe_Level10_b.fbx", TransformMatrix), "Weapon_Axe_1"));

	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_SPEAR),
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon_IceSpear"), "Pweapon_Laserspear_SSR.fbx", TransformMatrix), "Weapon_IceSpear"));

	TransformMatrix = XMMatrixScaling(0.4f, 0.4f, 0.4f) * XMMatrixRotationY(XMConvertToRadians(-90.0f)) * XMMatrixRotationZ(XMConvertToRadians(-90.0f));
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_CUBE_L),
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon_Cube"), "Pweapon_Cube_Level10_b_L.fbx", TransformMatrix), "Weapon_CubeL"));

	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_CUBE_R),
		CModel::Create(TYPE_NONANIM, FILEPATH("Weapon_Cube"), "Pweapon_Cube_Level10_b_R.fbx", TransformMatrix), "Weapon_CubeR"));

	TransformMatrix = XMMatrixScaling(0.4f, 0.4f, 0.4f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_MOBILITY_GLIDER),
		CModel::Create(TYPE_NONANIM, FILEPATH("Mobility_Glider"), "artifact_item_021.fbx", TransformMatrix), "Mobility_Glider"));

	TransformMatrix = XMMatrixScaling(0.4f, 0.4f, 0.4f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationZ(XMConvertToRadians(90.0f));
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_ANIM_WEAPON, MODEL_MOBILITY_MOTORCYCLE),
		CModel::Create(TYPE_ANIM, FILEPATH("Mobility_MotorCycle"), "Mount_018_skin_Skeleton.fbx", TransformMatrix), "Mobility_MotorCycle"));

	/*Enemy_Weapon*/
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_ENEMY_WEAPON_STIC),
		CModel::Create(TYPE_NONANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/HyenaGang_Minions/Weapon_Stic/", "Weapon_Stic.fbx", TransformMatrix), "Weapon_Stic"));
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_ENEMY_WEAPON_BOW),
		CModel::Create(TYPE_NONANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/HyenaGang_Shooter/Weapon_Bow/", "Weapon_Bow.fbx", TransformMatrix), "Weapon_Bow"));
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_ENEMY_WEAPON_WASTEBASKET),
		CModel::Create(TYPE_NONANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Gangster_Cap/Weapon_Wastebasket/", "Weapon_Wastebasket.fbx", TransformMatrix), "Weapon_Wastebasket"));
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_ENEMY_WEAPON_HAMMER),
		CModel::Create(TYPE_NONANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/Gangster_Cap/Weapon_Hammer/", "Weapon_Hammer.fbx", TransformMatrix), "Weapon_Hammer"));



	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_Model_Bullet()
{
#define FULLPATH "../Bin/Resources/Meshes/"
#define	FILEPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = FULLPATH;

	USEGAMEINSTANCE;
	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixIdentity();
	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_NORMAL_0),
		CModel::Create(TYPE_NONANIM, FILEPATH("Bullet"), "Rocket0.fbx", TransformMatrix), "Rocket0"));

	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_NORMAL_1),
		CModel::Create(TYPE_NONANIM, FILEPATH("Bullet"), "Rocket1.fbx", TransformMatrix), "Rocket1"));

	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_NORMAL_2),
		CModel::Create(TYPE_NONANIM, FILEPATH("Bullet"), "Rocket2.fbx", TransformMatrix), "Rocket2"));

	FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_NORMAL_3),
		CModel::Create(TYPE_NONANIM, FILEPATH("Bullet"), "Rocket3.fbx", TransformMatrix), "Rocket3"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.003f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_ARROW),
		CModel::Create(TYPE_NONANIM, "../Bin/Resources/Meshes/Enemy/Enemy_Normal/HyenaGang_Shooter/Arrow/", "Arrow.fbx", TransformMatrix), "Arrow"));

	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_Model_Npc()
{
#define FULLPATH "../Bin/Resources/Meshes/Npc/"
#define	FILEPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = FULLPATH;

	USEGAMEINSTANCE;
	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_NPC, MODEL_SHIRLEY),
		CModel::Create(TYPE_ANIM, FILEPATH("Shirley"), "npc_shu_027_Skeleton.FBX", TransformMatrix), "Shirley"));

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(225.0f));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_DYNAMIC, MODEL_NPC, MODEL_COOKER),
		CModel::Create(TYPE_ANIM, FILEPATH("Cooker"), "Cooker.FBX", TransformMatrix), "Cooker"));

	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_AnimationGroup()
{
	return E_NOTIMPL;
}

HRESULT CLoader_GamePlay::Loading_EffectModel()
{
#define	EFFECTPATH "../Bin/Resources/Meshes/Effect/"

	_matrix TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f)); //XMMatrixIdentity();

	stringvec vecPath, vecFile_fbx;
	read_directory(EFFECTPATH, vecPath);
	for (auto& iter : vecPath)
	{
#if 0
		// 매쉬 추가할때 이걸로 한번 돌리면 bmesh 생성됨.
		if (!iter.substr(iter.length() - 3).compare("fbx") || !iter.substr(iter.length() - 3).compare("FBX")) // fbx
		{
			vecFile_fbx.push_back(iter);
		}
#else
		if (!iter.substr(iter.length() - 5).compare("bMesh")) // bMesh 확장자 이름만 뽑아내기.
		{
			iter = iter.substr(0, iter.length() - 5) + "fbx";
			vecFile_fbx.push_back(iter);
		}
#endif // 0
	}
	_uint iModelNum2 = 1;
	_uint iModelNum3 = 0;
	for (auto& FileName : vecFile_fbx)
	{
		FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_STATIC, ID_UINT(MODEL_STATIC, MODEL_EFFECT, iModelNum2++, iModelNum3), CModel::Create(TYPE_NONANIM, EFFECTPATH, FileName.c_str(), TransformMatrix), FileName));
		if (255 < iModelNum2)
		{
			iModelNum2 = 1;
			iModelNum3++;
		}
	}

	return S_OK;
}

HRESULT CLoader_GamePlay::Loading_NonAnimModel()
{
#define FULLPATH "../Bin/Resources/Meshes/"
#define	FILEPATH(path) (strFullPath + path + "/").c_str()
	string strFullPath = FULLPATH;

	USEGAMEINSTANCE;
	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixIdentity();
	//FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, MODEL_TRIGGER, TRIGGER_DRAGON),
	//	CModel::Create(TYPE_NONANIM, FILEPATH("Trigger"), "SM_core_02_zn.fbx", TransformMatrix), "Dragon_Trigger"));

	return S_OK;
}

void CLoader_GamePlay::Add_MapModelPrototypeFromOneFolder(string Path, _int eModelID, _matrix TransformMatrix)
{
	stringvec vecPath, vecFile_fbx;
	read_directory(Path, vecPath);
	for (auto& File : vecPath)
	{
		if (!File.substr(File.length() - 3).compare("fbx") || !File.substr(File.length() - 3).compare("FBX"))
		{
			vecFile_fbx.push_back(File);
		}
		else if (!File.substr(File.length() - 5).compare("bMesh"))
		{
			if (!File.substr(File.length() - 5).compare("bMesh"))
			{
				File = File.substr(0, File.length() - 5) + "fbx";
			}
			vecFile_fbx.push_back(File);
		}
	}

	_uint iModelNum2 = 1;
	_uint iModelNum3 = 0;

	_uint iModelNum = 1; // 0 으로 하면 ID와 겹칠수 있음.
	_uint iFileNum = 0;

	for (auto& FileName : vecFile_fbx)
	{
		FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, eModelID, iModelNum2, iModelNum3), CModel::Create(TYPE_NONANIM, Path.c_str(), FileName.c_str(), TransformMatrix), FileName));
		FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC_INSTANCE, eModelID, iModelNum2++, iModelNum3),
			CModel::Create(TYPE_NONANIM_INSTANCE, Path.c_str(), FileName.c_str(), TransformMatrix), to_string(iFileNum++)));
		if (255 < iModelNum2)
		{
			iModelNum2 = 1;
			iModelNum3++;
		}

	}
}

void CLoader_GamePlay::Add_MapModelPrototypeFromEachFolder(string Path, _int eModelID, _matrix TransformMatrix)
{
	_uint iModelNum = 1; // 0 으로 하면 ID와 겹칠수 있음.
	stringvec vecPath, vecFile, vecFile_fbx;
	read_directory(Path, vecPath);
	for (auto& folder : vecPath) // folder : 각각 타일 폴더 명들.
	{
		StringvecClear(vecFile);
		read_directory(Path + folder, vecFile); // vecFile : folder안에 있는 파일 명들.
		StringvecClear(vecFile_fbx);
		for (auto& File : vecFile)
		{
			if (!File.substr(File.length() - 3).compare("fbx") || !File.substr(File.length() - 3).compare("FBX"))
			{
				vecFile_fbx.push_back(File);
			}
			else if (!File.substr(File.length() - 5).compare("bMesh"))
			{
				if (!File.substr(File.length() - 5).compare("bMesh"))
				{
					File = File.substr(0, File.length() - 5) + "fbx";
				}
				vecFile_fbx.push_back(File);
			}
		}
		_uint iFileNum = 0;
		for (auto& FileName : vecFile_fbx)	// fbx 파일 명.
		{
			FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, eModelID, iModelNum),
				CModel::Create(TYPE_NONANIM, (Path + folder + "/").c_str(), FileName.c_str(), TransformMatrix), (folder + "_" + to_string(iFileNum))));

			FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC_INSTANCE, eModelID, iModelNum++),
				CModel::Create(TYPE_NONANIM_INSTANCE, (Path + folder + "/").c_str(), FileName.c_str(), TransformMatrix), (folder + "_" + to_string(iFileNum++))));
		}
	}
}

void CLoader_GamePlay::Add_MapModelPrototypeFromEachFolder_Tile(string Path, _int eModelID, _matrix TransformMatrix)
{
	_uint iModelNum = 1; // 0 으로 하면 ID와 겹칠수 있음.
	stringvec vecPath, vecFile, vecFile_fbx;
	read_directory(Path, vecPath);
	for (auto& folder : vecPath) // folder : 각각 타일 폴더 명들.
	{
		StringvecClear(vecFile);
		read_directory(Path + folder, vecFile); // vecFile : folder안에 있는 파일 명들.
		StringvecClear(vecFile_fbx);
		for (auto& File : vecFile)
		{
			string test = File.substr(File.length() - 4);
			// Fbx나 bMesh 둘중 하나만 있어야 함!!! 둘다 있음 오류남.
			if (!File.substr(File.length() - 3).compare("fbx") || !File.substr(File.length() - 3).compare("FBX") || !File.substr(File.length() - 5).compare("bMesh"))
			{
				if (!File.substr(File.length() - 5).compare("bMesh"))
				{
					File = File.substr(0, File.length() - 5) + "fbx";
				}
				vecFile_fbx.push_back(File);
			}
		}
		_uint iFileNum = 0;
		for (auto& FileName : vecFile_fbx)	// fbx 파일 명.
		{
			FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC, eModelID, iModelNum),
				CModel::Create(TYPE_NONANIM, (Path + folder + "/").c_str(), FileName.c_str(), TransformMatrix), (folder + "_" + to_string(iFileNum))));

			FAILEDCHECK(GAMEINSTANCE->Add_Component_Prototype(LEVEL_GAMEPLAY, ID_UINT(MODEL_STATIC_INSTANCE, eModelID, iModelNum++),
				CModel::Create(TYPE_NONANIM_INSTANCE, (Path + folder + "/").c_str(), FileName.c_str(), TransformMatrix), (folder + "_" + to_string(iFileNum++))));
		}
	}
}

CLoader_GamePlay* CLoader_GamePlay::Create()
{
	CLoader_GamePlay* pInstance = new CLoader_GamePlay();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLoader_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_GamePlay::Free()
{
}

