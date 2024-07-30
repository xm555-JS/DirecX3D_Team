#pragma once
#include "Client_Defines.h"
#include "Window_Imgui.h"
#include "Cell.h"

#if defined(USE_IMGUI)

BEGIN(Engine)
class CCell;
class CShader;
END

BEGIN(Client)

class CWindow_Tool_Map final : public CWindow_Imgui
{
private:
	enum TAB_STATE { OBJECT, NAVIGATION, DYNAMIC_MODEL, TERRAIN, TAB_STATE_END };
	enum PICKING_MODE { Y_PICKING, TILE_PICKING, MESH_PICKING, DELETE_PICKING, PICKING_MODE_END };
	enum DYMAMIC_PICKING_MODE { D_Y_PICKING, D_TILE_PICKING, D_PICKING_MODE_END };

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	ImGuiIO io;

public:
	CWindow_Tool_Map();
	virtual ~CWindow_Tool_Map();

public:
	// CWindow_Imgui을(를) 통해 상속됨
	virtual void Tick(_float fTimeDelta) override;
	virtual void Initialize() override;
	virtual void On_EventMessege(EVENTDESC tEvent) override;
	virtual void Render(void) override;

private:
	void	Save_Load_Data();
	void	Show_Save_Window();
	void	Show_Load_Window();

private:
	bool show_demo_window = true;
	bool show_another_window = false;
	TAB_STATE m_eTabState = TAB_STATE_END;

	_float	m_fPicking_Height = 0.1f;
	_float	m_fPicking_Speed = 0.2f;

#pragma region Object
private:
	enum TURN_MODE { Y_TURN, Z_TURN, X_TURN, TURN_MODE_END };

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
			, pGameObject(nullptr) {}

	}GAMEOBJECT_DESC;

private:
	void	Show_ObjectPrototypes(_float fTimeDelta);
	void	Show_CreatedObjects(_float fTimeDelta);

	void	Save_ObjectData(void);
	void	Load_ObjectData(void);

	void	Create_Delete_Setting();
	void	Create_Object(void);
	void	Delete_AllObject(void);
	void	Picking_Setting(_float fTimedelta);
	void	Y_Picking(_float fTimedelta);
	void	Tile_Picking(_float fTimedelta);
	void	Mesh_Picking(_float fTimedelta);
	void	Picking_Delete(_float fTimedelta);
	_uint	Navi_Picking();
	string	Get_StaticObjectIDStr(_uint _ID);

private:
	map<_uint, CGameObject*>			m_pPrototypes;
	_uint								m_iObject_idx = NULL;
	_uint								m_iModel_idx = NULL;
	pair<_uint, class CGameObject*>		m_iSelectedObjectPrototype;

	map<_uint, CGameObject*>			m_pTilePrototypes;
	map<_uint, CGameObject*>			m_pMeshPrototypes;

	_bool								m_bEffect0 = false;
	_bool								m_bCreate = false;

	_float								m_fObjectTurnAngle = 0.f;
	_float								m_fObjectScale[3] = { 1,1,1 };

	ID_UINT								m_iTerrainID = 0;

	int									m_iTurnMode = 0;

	_float								m_fCreateSpeedTime=0.f;
	vector<_float4>						m_vecObjPos;
	_bool								m_bColRender = false;
#pragma endregion Object


#pragma region Navigation
private:
	enum SETTING_MODE { CREATE_POINT, SELECT_POINT, DELETE_POINT, PICKING_POINT, SETTING_MODE_END };
	enum SETTING_CELL { CELL_NORMAL, CELL_NOMOVE, CELL_END };

	typedef struct tagPointDesc
	{
		_float3 Point;
		XMVECTORF32 Color;
		tagPointDesc()
		{
			Point = _float3(0.f, 0.f, 0.f);
			Color = DirectX::Colors::Green;
		}
	}POINT_DESC;

	typedef struct tagCellPoint
	{
		_float3 PointA;
		_float3 PointB;
		_float3 PointC;
		tagCellPoint()
		{
			PointA = _float3(0.f, 0.f, 0.f);
			PointB = _float3(0.f, 0.f, 0.f);
			PointC = _float3(0.f, 0.f, 0.f);
		}
		tagCellPoint(_float3 _PointA, _float3 _PointB, _float3 _PointC)
		{
			PointA = _PointA;
			PointB = _PointB;
			PointC = _PointC;
		}
	}CELL_POINT;

	//_float						m_fYHeight = 0;
	vector<POINT_DESC>			m_vecPoints;

	vector<_float3>				m_vecSelectedPoint;
	vector<CELL_POINT>			m_vecCells;

	vector<class CCell*>			m_Cells;
	typedef vector<class CCell*>	CELLS;
	class CShader* m_pShaderCell = nullptr;

	_float	m_fPickingYHeight = 0;
	_uint	m_iCellProperty = CELL_END;

	_bool	m_bCellCenterRender = false;
	_bool	m_bCellPointRender = false;

	void	Navigation_Setting(void);

	void	Save_NavigationData(void);
	void	Load_NavigationData(void);

	void	Create_NaviCell_Terrain();
	void	Create_CellPoint_YHeightPicking();
	void	Create_CellPoint_TilePicking();
	void	Select_CellPoint();
	void	Select_CellCenter();
	void	Delete_CellPoint();
	void	Delete_AllCellPoint();

	// Color
	BasicEffect* m_pEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	//
#pragma endregion Navigation

#pragma region Dynamic_Model
private:
	enum DYNAMIC_ID { PLAYER_ID, ENEMY_ID, BOSS_ID, DYNAMIC_ID_END };
	enum MODEL_ANIM
	{
		ANIMMODEL_PLAYER,
		ANIMMODEL_NORMALMON,
		ANIMMODEL_BOSS,
		ANIMMODEL_END
	};

	void	Dynamic_Model_Setting(void);

	void Dynamic_Model_Create_Delete_Setting();

	void	Dynamic_Model_Picking_Setting();
	void	Dynamic_Model_Create_Object(void);
	void	Dynamic_Model_Tile_Picking();
	void	Dynamic_Model_Y_Picking();
	void	Show_Created_Dynamic_Objects(_float fTimeDelta);

	void	Save_Dynamic_ModelData(void);
	void	Load_Dynamic_ModelData(void);

	map<_uint, CGameObject*>			m_pEnemyPrototypes;
	_uint								m_iModelNum = NULL;
	_uint								m_iSelectModel = ANIMMODEL_END;
	ID_UINT								m_iSelectedDynamicPrototypeID;

	/*For.Boss*/
	map<_uint, CGameObject*>			m_pBossPrototypes;

#pragma endregion Dynamic_Model

#pragma region Terrain

public:
	void	Terrain_Setting(void);

	void	Terrain_Create_Delete_Setting();
	void	Terrain_Create_Object(void);
	void	Terrain_Delete_Object(void);
	void	Terrain_Picking();

	void	Save_Terrain_Data(void);
	void	Load_Terrain_Data(void);

	void	Check_SameCell();


private:
	_uint m_iNumX = 0, m_iNumZ = 0;
	_float m_fSizeX = 0.f, m_fSizeZ = 0.f;
	TERRAIN_DESC m_TerrainDesc;

#pragma endregion

private:
	char m_strFileName[128] = "File_0";


	_float m_fObjectRotate = 0.f;

private:
	void	Setup_ReversePoint(CCell::CELLDESC* pCelldesc);

public:
	virtual	void	Free() override;
};

END

#endif