#pragma once

#include "Client_Defines.h"
#include "Unit.h"


BEGIN(Engine)
class CCollider;
class CLight;
END

BEGIN(Client)
class CState_Handler;
class CWeapon;
class CItem;

class CPlayer final : public CUnit
{
public:
	typedef struct PlayerDesc : public UnitDesc
	{
		PlayerDesc()
			: UnitDesc()
		{
		}
		PlayerDesc(UNITDESC _UnitDesc, _float _fDashGauge, _float _fStaminaGauge, _float _fSkillGauge)
			: UnitDesc(_UnitDesc)
		{
			fDashGauge = _fDashGauge;
			fStaminaGauge = _fStaminaGauge;
			fSkillGauge = _fSkillGauge;
			
		}
		PlayerDesc(ELEMENTAL_PROPERTIE _ePropertie, _float _fHP, _float _fAttack, _float _fSpeed, _float _fDashGauge, _float _fStaminaGauge, _float _fSkillGauge
		, _bool _bAbleSkill_S = false, _bool _bAbleSkill_N = true)
		{
			ePropertie = _ePropertie;
			fHP = _fHP;
			fAttack = _fAttack;
			fSpeed = _fSpeed;
			fDashGauge = _fDashGauge;
			fStaminaGauge = _fStaminaGauge;
			fSkillGauge = _fSkillGauge;
			bAbleSkill_S = _bAbleSkill_S;
			bAbleSkill_N = _bAbleSkill_N;
		}

		_float		fDashGauge = 0.f;
		_float		fStaminaGauge = 0.f;
		_float		fSkillGauge = 0.f;
		_bool		bAbleSkill_S = false;
		_bool		bAbleSkill_N = true;

	}PLAYERDESC;

	enum PLAYER_WEAPON
	{
		PLAYER_WEAPON_SWORD,
		PLAYER_WEAPON_GUN_L,
		PLAYER_WEAPON_GUN_R,
		PLAYER_WEAPON_SHIELD,
		PLAYER_WEAPON_AXE,
		PLAYER_WEAPON_SHIELDAXE,
		PLAYER_WEAPON_CUBE_L,
		PLAYER_WEAPON_CUBE_R,
		PLAYER_MOBILITY_GLIDER,
		PLAYER_MOBILITY_MOTORCYCLE,

		PLAYER_WEAPON_END,
	};


public:
	CPlayer();
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	_float		Get_TimeChangeWeapon(WEAPON_SLOT eWeaponSlot) { return m_fTimeChangeWeapon[eWeaponSlot]; }
	_float		Get_TimeSkill_N(WEAPON_TYPE ePlayerWeapon) { return m_fTimeSkill_N[ePlayerWeapon]; }
	_float		Get_CoolTime_ChangeWeapon(WEAPON_SLOT eWeaponSlot) { return m_fCoolTime_ChangeWeapon[eWeaponSlot]; }
	_float		Get_CoolTime_Skill_N(WEAPON_TYPE ePlayerWeapon) { return m_fCoolTime_Skill_N[ePlayerWeapon]; }
	_uint		Get_Slot_Weapon(_uint iIndex) { return m_eSlot_Weapon[iIndex]; }
	_uint		Get_Slot_WeaponKey(_uint iIndex) { return m_iSlot_WeaponKey[iIndex]; }
	_uint		Get_CurWeapon() { return m_eCurWeapon; }
	_bool		Get_ResetSkillGauge() { return m_bResetSkillGauge; }
	_bool		Get_Damaged(_uint iDamageLevel) { return m_bDamaged[iDamageLevel]; }
	_bool		Get_EnableKeyInput() { return m_bEnableKeyInput; }
	CWeapon*	Get_Weapon(_uint iNum) { return m_Weapons[iNum]; }
	CWeapon*	Get_CurWeaponL() { return m_pCurWeapon_L; }
	CWeapon*	Get_CurWeaponR() { return m_pCurWeapon_R; }
	CWeapon*	Get_Mobility_Glider() { return m_pMobilityGlider; }
	CWeapon*	Get_Mobility_MotorCycle() { return m_pMobilityMotorCycle; }
	WEAPON_TYPE* Get_OwnWeapon() { return m_eOwnWeapons;}
	_bool		Get_AbleSkill_S() { return ((PLAYERDESC*)m_pCurInfo)->bAbleSkill_S; }
	_bool		Get_ChangeWeapon() { return m_bWeaponChange; }
	_bool		Get_CubeBulletCreate() { return m_bCubeBulletCreate; }
	_bool       Get_InEquip_Window() { return m_bInEquip_Window; }
	_bool		Get_Change_SlotWeapon() { return m_bChange_SlotWeapon; }
	_float		Get_SwimHeight() { return m_fSwimHeight; }
	_bool		Get_Evasion() { return m_bEvasion; }

	void		Set_TimeSkill_N(WEAPON_TYPE ePlayerWeapon) { m_fTimeSkill_N[ePlayerWeapon] = m_fCoolTime_Skill_N[ePlayerWeapon]; }
	void		Set_Slot_Weapon(_uint iIndex, WEAPON_TYPE eWeapon) { m_eSlot_Weapon[iIndex] = eWeapon; }
	void		Set_Slot_WeaponKey(_uint iIndex, _uint iKey) { m_iSlot_WeaponKey[iIndex] = iKey; }
	void		Set_Damaged(_uint iDamageLevel, _bool bDamaged) { m_bDamaged[iDamageLevel] = bDamaged; }
	void		Set_AddCubeBullet();
	void		Set_EnableKeyInput(_bool bEnableKeyInput);// { m_bEnableKeyInput = bEnableKeyInput; }
	//void		Set_ReserveEnableKeyInput();
	void		Set_NearTarget();
	void		Set_ResetSkillGauge(_bool bReset) { m_bResetSkillGauge = bReset; }
	void		Set_SkillGauge(_float fGauge) { ((PLAYERDESC*)m_pCurInfo)->fSkillGauge += fGauge / 5000.f; }
	void		Set_UsedSkill_S() { ((PLAYERDESC*)m_pCurInfo)->bAbleSkill_S = false; }
	void		Set_Shield(_float fShield) { m_fShield = fShield; }
	void		Set_InEquip_Window(_bool bIsIn) { m_bInEquip_Window = bIsIn; }
	void		Set_Change_SlotWeapon(_bool bChange) { m_bChange_SlotWeapon = bChange; }
	void		Set_SwimHeight(_float _fSwimHeight) { m_fSwimHeight = _fSwimHeight; }
	void		Set_WeaponShadow(CLight* pLight);
	void		Set_Evasion(_bool bEvation) { m_bEvasion = bEvation; }



	void		Reset_CubeBullet();
	void		Reset_WeaponChange();
	void		SetUp_OwnWeapon(WEAPON_TYPE eWeaponType);
	void		SetUp_UI_Weapon(_uint iWeaponType);

public:
	void		Keep_Realpos() { m_vRealPos = Get_Pos(); }
	void		SetUp_Realpos() { Set_Pos(m_vRealPos); }
	void		SetUp_Cur_HandWeapon();

	void		Keep_RealCellIdx();
	void		SetUp_RealCellIdx();
	void		Healing();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render(void* pArg);
	virtual void On_EventMessege(EVENTDESC tEvent);

	//_uint Get_JoonAI_Test() { return m_iJoon_AITest; }

public:
	virtual void	Damaged(ELEMENTAL_PROPERTIE eElement, _float fPower);

private:
	CCollider*			m_pCol_Sphere = nullptr;
	CCollider*			m_pCol_Gun = nullptr;
	CCollider*			m_pCol_Body = nullptr;
	CCollider*			m_pCol_UIIntrc = nullptr;
	CCollider*			m_pCol_Volume = nullptr;
	CCollider*			m_pCol_Range = nullptr;

private:
	WEAPON_TYPE			m_eOwnWeapons[4];
	WEAPON_TYPE			m_eSlot_Weapon[WPN_SLOT_END];
	_uint				m_iSlot_WeaponKey[WPN_SLOT_END];
	WEAPON_TYPE			m_eCurWeapon = WEAPON_END;
	WEAPON_TYPE			m_eNextWeapon = WEAPON_END;
	WEAPON_SLOT			m_eNextWeaponSlot = WPN_SLOT_END;
	_byte				m_iNextKey = 0;
	
	CWeapon*			m_Weapons[PLAYER_WEAPON_END];
	CWeapon*			m_pCurWeapon_L = nullptr;
	CWeapon*			m_pCurWeapon_R = nullptr;
	CWeapon*			m_pMobilityGlider = nullptr;
	CWeapon*			m_pMobilityMotorCycle = nullptr;
	//ITEM_TYPE			m_eSlot_item[5][5];

	_float				m_fCoolTime_ChangeWeapon[WPN_SLOT_END];
	_float				m_fCoolTime_Skill_N[WEAPON_END];
	
	_float				m_fTimeChangeWeapon[WPN_SLOT_END];
	_float				m_fTimeSkill_N[WEAPON_END];

	_bool				m_bWeaponChange = false;

	// 무기창 슬롯에 있는 무기 바뀔때만 true되는 변수
	_bool				m_bChange_SlotWeapon = true;
	_bool				m_bInEquip_Window = false;

private:
	// 인벤토리 이동시킬때 위치랑 셀 인덱스 저장용
	_vector					m_vRealPos;
	_uint					m_iRealCellIdx;

	_bool					m_bResetSkillGauge = false;
	_float					m_fPreStamina = 0.f;
	_float					m_fStaminaTime = 0.f;
	_float					m_fRecoverhpTime = 0.f;
	_float					m_fWaitRecoverTime = 0.f;

	list<class CUI*>		m_GuideWindow_Handlist;
	list<class CUI_Solid*>	m_Enemy_Indicatorlist;

	_bool					m_bDamaged[3] = {false,};
	_float					m_fDamage;
	_float					m_fShield = 1.f;
	_float					m_fShieldTime = 0.f;

	_bool					m_bEnableKeyInput = true;
	_uint					m_iReserveEnableKeyInput = 0;
	//_float					m_fEnableKeyTime = 0.f;

private:
	_float					m_fDissolveTimeAcc;
	_bool					m_bUseDissolve = false;

private:
	_float					m_fTargetTime = 0.2f;
	_bool					m_bDissolve = false;

private:
	_uint					m_iCubeHitCnt = 0;
	_uint					m_iCubeBulletCnt = 0;
	_bool					m_bCubeBulletCreate = false;
	_bool					m_bCubeBulletGetPos[5] = { false, };
	_float					m_fCubeLerpTime = 0.f;
	_float4					m_vCubePos[5];

	_float					m_fSwimHeight = 9.270f;

private:
		_bool			m_bSoundSkate = true;
		_float			m_fSoundSkate = 0.f;

private:
	_bool					m_bEvasion = false;

private:
	DECL_ONCE(m_bScreenEffect0);
	DECL_ONCE(m_bScreenEffect1);

private:
	_float					m_fNavigationAccTime = 0.f;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	HRESULT SetUp_State_Handler();

private:
	void	Input_Key(_float fTimeDelta);

	void	Change_Weapon(_byte iKey, WEAPON_SLOT eWeaponSlot);
	void	Change_Weapon_Slot();
	void	Input_WeaponKey();
	void	Input_DashKey(_float fTimeDelta);
	void	Input_StaminaKey(_float fTimeDelta);
	void	SetUp_SkillGauge();
	void	SetUp_CoolTime(_float fTimeDelta);

	void	Gain_Item();
	void	Create_UI_Enemy_Indicator();
	void	Create_UI_Alert(wstring strText, _uint iTextureIdx = 0, _bool bIcon = false);

	void	Push_Other(CCollider* pTargetCol, CCollider* pVolumeCol, CTransform* pTargetTransform, CNavigation* pTargetNavigation);

	void	Set_CubeBulletPos();

public:
	static CPlayer* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END