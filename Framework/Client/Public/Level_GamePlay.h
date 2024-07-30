#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
	class CCamera;
	class CGameObject;
	class CLight;
	class CShader;
END

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
public:
	// Objects 
	typedef struct tagGameObjectDesc
	{
		_uint iObjectID;
		_uint iModelID;
		_matrix WorldMatrix;
		CGameObject* pGameObject;

		tagGameObjectDesc()
			: iObjectID(0)
			, iModelID(0)
			, WorldMatrix(XMMatrixIdentity())
			, pGameObject(nullptr)	{}

	}GAMEOBJECT_DESC;

public:
	CLevel_GamePlay();
	virtual ~CLevel_GamePlay() = default;

public:
	virtual	vector<CCamera*>*	Get_FreeCameras() override { return &m_Cameras[CAMERA_FREE]; }
	CCamera*					Get_PlayerCamera() { return m_pPlayerTargetCamera; }
	CCamera*					Get_InvenCamera() { return m_pInvenCamera; }
	CCamera*					Get_CookCamera() { return m_pCookCamera;}
	CCamera*					Get_EquipCamera() { return m_pEquipCamera;}
	CCamera*					Get_ShirleyCamera() { return m_pShirleyCamera; }

	virtual	void	Add_FreeCamera() override;
	HRESULT			Ready_Layer_MiniGame(MINIGAME_TYPE eType);

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float TimeDelta) override;
	virtual void LateTick(_float TimeDelta) override;
	virtual HRESULT Render() override;
	virtual void On_EventMessege(EVENTDESC tEvent) override;


private:
	vector<CCamera*>	m_Cameras[CAMERA_END];
	CCamera*			m_pPlayerTargetCamera = nullptr;
	CCamera*			m_pInvenCamera = nullptr;
	CCamera*			m_pCookCamera = nullptr;
	CCamera*			m_pEquipCamera = nullptr;
	CCamera*			m_pShirleyCamera = nullptr;

	_uint				m_iModelNum = 0;

	_bool				m_bStartCreateCrystal = false;
	DECL_ONCE(m_bCrystal_Robella);
	DECL_ONCE(m_bCrystal_Dragon);
	DECL_ONCE(m_bCrystal_Weaver);
	_float				m_fTimeAcc_Crytal = 0.f;

	//CGameObject*		m_pPlyaer = nullptr;


private:
	HRESULT		Ready_Lights();
	HRESULT		Ready_Layer_Sky(ID_UINT iID);
	HRESULT		Ready_Layer_Camera(ID_UINT iID);
	HRESULT		Ready_Layer_BackGround(ID_UINT iID);
	HRESULT		Ready_Layer_Player(ID_UINT iID);
	HRESULT		Ready_Layer_EnemyNormal(ID_UINT iID);
	HRESULT		Ready_Layer_EnemyBoss(ID_UINT iID);
	HRESULT		Ready_Layer_Map(ID_UINT iID);
	HRESULT		Ready_Layer_UI(ID_UINT iLayerID);
	HRESULT		Ready_Layer_Item(ID_UINT iLayerID);
	HRESULT		Ready_Layer_Npc(ID_UINT iLayerID);
	HRESULT		Ready_Layer_ActionCamera(ID_UINT iID);
	

	void		Load_UI(string strFileName);
	void		Load_Navigation();

	void					Add_MapObjData(_uint iModelID, GAMEOBJECT_DESC tInfo);
	list<GAMEOBJECT_DESC>*	Find_MapList(_uint iModelID);

private:
	void		Render_Light_Depth(CLight* pLight);
	void		Move_LightRevolution(CLight* pLight, _fvector vAxis, _fvector vCurPos, _float fSpeed);
	void		Update_LightState();
	void		CreateCrystal(_float fTimeDelta);

private:
	typedef	list<GAMEOBJECT_DESC>		GAMEOBJDESCS;
	map<_uint, GAMEOBJDESCS>			m_MapObjLists;

private:
	CLight*						m_pSun[NUMSUNLAYER];
	CLight*						m_pPlayerLight = nullptr;
	CShader*					m_pDepthShader = nullptr;
	CGameObject*				m_pPlayer = nullptr;

private:
	_float						m_fBrightness_Ori = 0.f;
	_float						m_fSaturationRate_Ori = 0.f;

private:
	_bool						m_bLeviathanPhase = false;
	_float						m_fScreenTimeAcc = 0.f;

	//_float						m_fTickTime_Record = 0.f;

private:
	DECL_ONCE(m_bSun);

	//CCamera*					m_pTempCamera = nullptr;
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);

private:
	// 필드의 중심에서 light 로 향하는 방향벡터
	_float4						m_vLightRevolutionAxis;


public:
	static CLevel_GamePlay* Create();
	virtual void Free() override;
};

END