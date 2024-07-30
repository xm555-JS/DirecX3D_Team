#pragma once
#include "Client_Defines.h"
#include "Window_Imgui.h"

#if defined(USE_IMGUI)

BEGIN(Engine)
class CModel;
class CAnimation;
class CHierarchyNode;
class CChannel;
class CComponent;
END

BEGIN(Client)

class CAnimObject;
class CWeapon;

class CWindow_Tool_Animation final : public CWindow_Imgui
{

private:
	enum MODEL_ANIM
	{
		ANIMMODEL_PLAYER,
		ANIMMODEL_NPC,
		ANIMMODEL_NORMALMON,
		ANIMMODEL_BOSSMON,
		ANIMMODEL_CAMERA,
		ANIMMODEL_BULLET,
		ANIMMODEL_WEAPON,
		NONANIMMODEL_WEAPON,
		ANIMMODEL_END
	};

public:
	CWindow_Tool_Animation();
	virtual ~CWindow_Tool_Animation();

public:
	// CWindow_Imgui을(를) 통해 상속됨
	virtual void Tick(_float fTimeDelta) override;
	virtual void Initialize() override;
	virtual void On_EventMessege(EVENTDESC tEvent) override;

public:
	void	Show_ModelCreate();
	void	Show_HierarhcyNodes();
	void	Show_CreatedObjects();
	void	Show_ProgressBar(_float fTimeDelta);
	void	Show_AnimGroupDesc();
	void	Show_AnimTool();

	void	Create_Delete_Setting();
	void	Create_Object(void);
	void	Delete_All_Object(void);

public:	//AnimGroup
	void	Show_AnimationGroup();

	void	Add_AnimationGroup();
	void	Clear_AnimationGroup();

	void	Save_AnimationGroup();
	void	Load_AnimationGroup();

	void	Set_AnimationGroup();

	//Weapon

	void	Show_WeaponTool();
	void	Show_CreatedWeapon();
	void	Set_WeaponMatrix();

	void	Create_Delete_Weapon();
	void	Create_Weapon(void);
	void	Delete_All_Weapon(void);

public:
	void	Input_KeyState(_float fTimeDelta);

public:
	_uint	Show_AnimGroup_Selector(const char* label);
	_uint	Show_WeaponType_Selector(const char* label);
	
private:
	bool show_demo_window = true;
	bool show_another_window = false;

private:
	map<_uint, CGameObject*>			m_pPrototypes[ANIMMODEL_END];
	map<_uint, CComponent*>				m_pPrototypeModels[ANIMMODEL_END];
	list<CGameObject*>					m_pGameObject[ANIMMODEL_END];
	vector<CModel*>						m_pModels;
	vector<CAnimation*>					m_pAnimations;
	vector<CAnimation*>					m_pLoadGroup;
	vector<CAnimation*>					m_pAnimationsGroup;
	vector<CChannel*>					m_pChannels;
	vector<CHierarchyNode*>				m_pHierarchyNodes;

	_uint								m_iTypeID[ANIMMODEL_END];
	pair<_uint, class CGameObject*>		m_iSelectedObjectPrototype;
	pair<_uint, class CComponent*>		m_iSelectedComponentPrototype;
	pair<_uint, class CComponent*>		m_iSelectedWeponPrototype;

	CModel*								m_pModelCom = nullptr;
	CAnimObject*						m_pAnimObject = nullptr;
	CHierarchyNode*						m_pHierarchyNode = nullptr;
	CWeapon*							m_pWeapon = nullptr;
	CModel*								m_pWeaponModel = nullptr;

private://Animation Create And Play
	_float	m_fCurrentTime = 0.f;
	_float	m_fTickperSecond = 0.f;
	_float	m_fMaxAnimationTime = 0.f;
	_float	m_fSpeed = 1.f;
	_float	m_fDuration = 0.f;

	_bool	m_bStop = false;
	_bool	m_bHold = false;

	_int	m_CurrentAnimationIndex = 0;

	_uint	m_iModelNum = NULL;
	_uint	m_iWeaponNum = NULL;
	_uint	m_iAnimNum = NULL;
	_uint	m_iAnimGroupNum = NULL;
	_uint	m_iSelectGroupNum = NULL;
	_uint	m_iPreAnimNum = NULL;
	_uint	m_iHierNum = NULL;
	_float	m_fObjectRotate[3] = { 0, 0, 0 };

	_float4x4	m_HierCombineMatrix;

private://AnimTool
	_float	m_vX[4] = {};
	_float	m_vY[4] = {};
	_float	m_vZ[4] = {};
	_float	m_vW[4] = {};

private:
	_int	m_eAnimType = 0;
	_int	m_eWeaponType = 0;

private:
	WEAPON_TYPE		m_eWeaponAnim = WEAPON_END;
	ANIM_GROUP		m_eAnimGroup = ANIM_END;

private:
	_int	m_iSelectModel= ANIMMODEL_END;
	_bool	m_bAnimationPlay = false;
	_bool	m_bMoveTickTime = false;
	_bool	m_bMoveFrameKey = false;

private:
	string GetObjectIDStr(_uint _ID);
	string GetComponentIDStr(_uint _ID);
	string GetModel_RockIDStr(_uint _ID);
	string GetModel_TileIDStr(_uint _ID);

public:
	virtual	void	Free() override;
};

END

#endif