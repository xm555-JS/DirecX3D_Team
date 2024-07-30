#pragma once

#include "Client_Defines.h"
#include "Loader.h"

class CLoader_GamePlay final : public CLoader
{
public:
	CLoader_GamePlay();
	virtual ~CLoader_GamePlay() = default;

public:
	HRESULT	Initialize();

public:
	virtual HRESULT Loading_Object() override;
	virtual HRESULT Loading_Texture() override;
	virtual HRESULT Loading_Model() override;
	virtual HRESULT Loading_Shader() override;

	HRESULT	Loading_MapBuffer();
	HRESULT	Loading_MapModel();
	HRESULT Loading_ItemModel();
	void Add_MapModelPrototypeFromOneFolder(string Path, _int eModelID, _matrix TransformMatrix);
	void Add_MapModelPrototypeFromEachFolder(string Path, _int eModelID, _matrix TransformMatrix);
	void Add_MapModelPrototypeFromEachFolder_Tile(string Path, _int eModelID, _matrix TransformMatrix);
	HRESULT	Loading_EffectModel();
	HRESULT	Loading_NonAnimModel();

	HRESULT	Loading_Model_Player();
	HRESULT Loading_Model_EnemyNormal();
	HRESULT Loading_Model_EnemyBoss();
	HRESULT Loading_Model_Weapon();
	HRESULT Loading_Model_Bullet();
	HRESULT Loading_Model_Npc();

	HRESULT	Loading_AnimationGroup();

public:
	static CLoader_GamePlay* Create();
	virtual void Free() override;
};

