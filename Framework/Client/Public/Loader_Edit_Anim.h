#pragma once

#include "Client_Defines.h"
#include "Loader.h"

class CLoader_Edit_Anim final : public CLoader
{
public:
	CLoader_Edit_Anim();
	virtual ~CLoader_Edit_Anim() = default;

public:
	HRESULT	Initialize();

public:
	virtual HRESULT Loading_Object() override;
	virtual HRESULT Loading_Texture() override;
	virtual HRESULT Loading_Model() override;
	virtual HRESULT Loading_Shader() override;

	HRESULT	Loading_DynamicModel();
	HRESULT	Loading_MapModel();

	HRESULT Loading_Model_EnemyNormal();
	HRESULT Loading_Model_EnemyBoss();
	HRESULT Loading_Model_Npc();
	HRESULT	Loading_EffectModel();


	HRESULT Loading_Model_Weapon();


public:
	static CLoader_Edit_Anim* Create();
	virtual void Free() override;
};

