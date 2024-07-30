#pragma once

namespace Engine
{
	enum EVENT_TYPE
	{
		LEVEL_CHANGE,			//lParam	_uint	iLevelIndex	
								//wParam	CLevel*	pNextLevel

		OBJECT_DEAD,			//lParam	CLevel*	Level
								//wParam	CGameObject*	pObejct

		EVENT_KEY,				//lParam	_uint	iLevelIndex	
								//wParam	CLevel*	pNextLevel

		EVENT_PICKING,			//lParam	CGameObject	pGameObject
								//wParam	_float3		vPosition

		EVENT_CALLPICKING,		//lParam	CGameObject	pGameObject
								//wParam	_float3		vPosition

		EVENT_PICKSTATEMODE,	//lParam	_bool	bState
								//wParam	nullptr

		EVENT_COLLISIONENTER,	//lParam	list<CCollider*>	ColList
								//wParam	nullptr

		EVENT_COLLISIONSTAY,	//lParam	list<CCollider*>	ColList
								//wParam	nullptr

		EVENT_COLLISIONEXIT,	//lParam	list<CCollider*>	ColList
								//wParam	nullptr

		EVENT_CHANGESKYBOX,		//lParam	_uint iSkybox Index
								//wParam	nullptr

		EVENT_HPCHANGE,			//lParam	Cunit* pUnit
								//wParam    _float fPreHp	

		EVENT_STAMINACHANGE,	//lParam	_float fPreStamina
								//wParam	_float fNextStamina

		EVENT_IMGUIWINDOWSTATE,	//lParam	_bool	bWindowState
								//wParam	nullptr

		EVENT_SKILLCHANGE_L,	//lParam	_uint iPreSlot_WeaponKey	
								//wParam	_uint iNextSlot_WeaponKey

		EVENT_SKILLCHANGE_R,	//lParam	_uint iPreSlot_WeaponKey	
								//wParam	_uint iNextSlot_WeaponKey

		EVENT_INVEN_WINDOW,		//lParam	_bool	bOpenWindow
								//wParam	nullptr

		EVENT_USE_ITEM,			//lParam	_uint	iItemType
								//wParam	nullptr

		EVENT_PLAYER_DAMAGED,	//lParam	_uint	iDamage
								//wParam	nullptr

		EVENT_MONSTER_DEAD,		//lParam	nullptr
								//wParam	nullptr

		EVENT_QUEST,			//lParam	nullptr
								//wParam	nullptr

		OBJECT_POOLING,							//lParam	CGameObject	pGameObject
												//wParam	nullptr
		
		EVENT_COOK,								//lParam	ITEM_TYPE	eFood
												//wParam	_uint		iFoodNum

		EVENT_WEAPON_CHANGE,					//lParam	WEAPON_TYPE eType
												//wParam	_bool		bEquiped_Weapon

		EVENT_TEST,				// lParam	CEnemy* pEnemy
								// wParma	nullptr

		EVENT_LEVIATHAN_APEAR,	// lParam	nullptr
								// wParma	nullptr

		EVENT_LEVIATHAN_DEAD,	// lParam	nullptr
								// wParma	nullptr

		GAME_EXIT,
		EVENT_END
	};

	enum MODELTYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_ANIM_GROUP, TYPE_NONANIM_INSTANCE, TYPE_ANIM_INSTANCE, TYPE_END };

	enum RENDERGROUP
	{
		RENDER_PRIORITY,
		RENDER_TERRAIN,
		RENDER_NONALPHABLEND,
		RENDER_TARGET_NONALPHABLEND,
		RENDER_DECAL,
		RENDER_NONLIGHT,
		RENDER_ALPHABLEND,
		RENDER_UI,
		RENDER_UISOLID,
		RENDER_CAPTURE,
		RENDER_EFFECT,
		RENDER_EFFECT_DISTORTION,
		RENDER_WATER_SURFACE,
		RENDER_CLOUD_SKY,
		RENDER_CLOUD,

		RENDER_END
	};

	enum RENDERTYPE
	{
		RENDERTYPE_DEFAULT,
		RENDERTYPE_DEPTH,


		RENDERTYPE_END
	};


	typedef struct EventDesc
	{
		EventDesc() {}
		EventDesc(EVENT_TYPE _eEventType, void* _lParam, void* _wParam)
		{
			eEventType = _eEventType;
			lParam = _lParam;
			wParam = _wParam;
		}

		EVENT_TYPE	eEventType;
		void* lParam;
		void* wParam;
		//DWORD_PTR	lParam;
		//DWORD_PTR	wParam;
	}EVENTDESC;

	enum MOUSEBUTTON { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_X, MBS_END };
	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	enum ENGINE_SHADER
	{
		SHADER_BLOOM,
		SHADER_BLUR,
		SHADER_GLOW,
		SHADER_HDR,
		SHADER_OUTLINE,
		SHADER_SHADOW,
		SHADER_SSAO,
		SHADER_DEFERRED,
		SHADER_DECAL,
		SHADER_LIGHT,
		SHADER_SATURATION,
		SHADER_BRIGHTNESS,


		SHADER_END,
	};


	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		float			fTime;
	} KEYFRAME;

	enum class COLLIDERTYPE { DYNAMIC, STATIC, DECORATION, ZONE, YFIXED_DYNAMIC, TYPE_END };
	enum class COLLIDERSHAPE { SPHERE, BOX, CYLINDER, CONVECMESH, SHAPE_END };


	typedef struct ViewportDesc
	{
		DWORD       X;
		DWORD       Y;            /* Viewport Top left */
		DWORD       Width;
		DWORD       Height;       /* Viewport Dimensions */
		float       MinZ;         /* Min/max of clip Volume */
		float       MaxZ;
	}VIEWPORTDESC;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_SPOT, TYPE_END };
		enum OPTION { OPTION_SUN, OPTION_TARGET, OPTION_END };

		TYPE			eType = TYPE_END;

		XMFLOAT4		vDirection;

		XMFLOAT4		vPosition;
		float			fRange;

		float			fRangeIn;
		float			fRangeOut;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

		OPTION			eOption;
		unsigned int	iDepthCX;
		unsigned int	iDepthCY;
	}LIGHTDESC;

	typedef struct tagRenderDesc
	{
		void*		lParam;
		void*		wParam;
		RENDERTYPE	eType;
	}RENDERDESC;

	typedef struct tagMaterialDesc
	{
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
	}MTRLDESC;

	typedef struct tagLineIndices32
	{
		unsigned long _1, _2;
	}LINEINDICES32;

	typedef struct tagLineIndices16
	{
		unsigned short _1, _2;
	}LINEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long _1, _2, _3;
	}FACEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short _1, _2, _3;
	}FACEINDICES16;

	typedef struct tagModelMaterial
	{
		class CTexture* pTextures[AI_TEXTURE_TYPE_MAX];
	}MODEL_MATERIAL;

	typedef struct tagVertex_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertex_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEX_DECLARATION;

	// For.Instance
	typedef struct tagVertex_Point
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;
	}VTXPOINT;

	typedef struct tagVertex_Instance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vPosition;
	}VTXINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Point_Instance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	}VTXPOINT_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Texture_Instance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEX_INSTANCE_DECLARATION;

	// For Navigation
	typedef struct tagVertrx_Position
	{
		XMFLOAT3	vPosition;
	}VTXPOS;
	typedef struct ENGINE_DLL tagVertex_Position_Declaration
	{
		static const unsigned int		iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	}VTXPOS_DECLARATION;

	typedef struct tagVertex_Normal_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Normal_Texture_Declaration
	{
		static const unsigned int		iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXNORTEX_DECLARATION;


	typedef struct tagVertex_Model
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagtagVertex_Model_Declaration
	{
		static const unsigned int		iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXMODEL_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Model_Instance_Declaration
	{
		static const unsigned int		iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXMODEL_INSTANCE_DECLARATION;


	typedef struct tagVertex_AnimModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex;
		XMFLOAT4		vBlendWeight;
	}VTXANIM;

	typedef struct ENGINE_DLL tagVertex_AnimModel_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXANIM_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Anim_Instance_Declaration
	{
		static const unsigned int		iNumElements = 10;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	}VTXANIM_INSTANCE_DECLARATION;




	typedef struct tagVertex_Cube_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexUV;
	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertex_Cube_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXCUBETEX_DECLARATION;


	typedef struct tagVertex_Normal_Cube_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTexUV;
	}VTXNORCUBETEX;

	typedef struct ENGINE_DLL tagVertex_Normal_Cube_Texture_Declaration
	{
		static const unsigned int		iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXNORCUBETEX_DECLARATION;
	

	// typedef struct tagVertex_Trail
	// {
	// 	XMFLOAT3		vPosition;
	// 	unsigned int	iIndex;
	// }VTXTRAIL;
	// 
	// typedef struct ENGINE_DLL tagVertex_Normal_Trail
	// {
	// 	static const unsigned int		iNumElements = 2;
	// 	static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	// } VTXTRAIL_DECLARATION;


	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		unsigned int iWinCX, iWinCY;
		HWND	hWnd;
		WINMODE	isWindowMode;
	}GRAPHICDESC;



	// 저장해야하는값 :  Collider type (dynamic or static), Pos, Shape, xyzSize, Density
	typedef struct tagSaveObjectDesc
	{
		void* pObject;
		void* pConvecMesh;

		// 실제로 json으로 저장할 값
		COLLIDERTYPE		eType;
		XMVECTOR			vPosition;
		XMVECTOR			vAngles;
		COLLIDERSHAPE		eShape;
		XMVECTOR			vScale;
		float				fDensity;

		int					iObjectID_fir;
		int					iObjectID_num;
		int					iOption;

	}SAVEDESC;


	typedef struct tagIDNums
	{
		BYTE	Num0;
		BYTE	Num1;
		BYTE	Num2;
		BYTE	Num3;
		//char	Num4;
		//char	Num5;
		//char	Num6;
		//char	Num7;
	}IDNUMS;

	typedef union ID_uint
	{
		bool operator == (ID_uint rhs)
		{
			return (iID == rhs.iID);
		}

		ID_uint& operator = (const ID_uint& rhs)
		{
			iID = 0;
			tNums.Num0 = rhs.tNums.Num0;
			tNums.Num1 = rhs.tNums.Num1;
			tNums.Num2 = rhs.tNums.Num2;
			tNums.Num3 = rhs.tNums.Num3;
			return *this;
		}

		ID_uint& operator = (const unsigned int& rhs)
		{
			iID = rhs;
			return *this;
		}

		ID_uint()
		{
			iID = 0;
		}
		ID_uint(int x)
		{
			iID = 0;
			tNums.Num0 = x;
		}
		ID_uint(int x, int y)
		{
			iID = 0;
			tNums.Num0 = x;
			tNums.Num1 = y;
		}
		ID_uint(int x, int y, int z)
		{
			iID = 0;
			tNums.Num0 = x;
			tNums.Num1 = y;
			tNums.Num2 = z;
		}
		ID_uint(int x, int y, int z, int w)
		{
			iID = 0;
			tNums.Num0 = x;
			tNums.Num1 = y;
			tNums.Num2 = z;
			tNums.Num3 = w;
		}

		IDNUMS			tNums;
		unsigned int	iID;
	}ID_UINT;

	typedef struct tagMaterialsDataDesc
	{
		DWORD iLen;
		char* szName;
	}MATERIALSDATADESC;

	typedef struct tagHierarchyNodeDataDesc
	{
		_uint iDepth;
		_uint iIndex;
		_int iParentIndex;
		char szName[MAX_PATH];
		_float4x4 OffsetMatrix;
		_float4x4 TransformationMatrix;
		_float4x4 CombinedTransformationMatrix;
	}HIERARCHYNODEDATADESC;

	typedef struct tagMeshContainerDataDesc
	{
		_uint iMaterialIndex;
		char szName[MAX_PATH];
		_uint iNumVertices;
		_float4* pVerticesPos;
		VTXANIM* pVertices;
		_uint iNumBones;
		_uint* Bones;

		_uint iNumPrimitive;
		FACEINDICES32* pIndices;

	}MESHCONTAINERDATADESC;

	typedef struct tagAnimModelDataDesc
	{
		// 메터리얼(텍스쳐)
		_uint iNumMaterials;
		MATERIALSDATADESC* Materials[AI_TEXTURE_TYPE_MAX];

		// 하이라키노드
		_uint iNumHierarchyNode;
		HIERARCHYNODEDATADESC* HierarchyNodes;

		// 메시컨테이너
		_uint iNumMeshContainers;
		MESHCONTAINERDATADESC* MeshContainers;

	}ANIMDATADESC;

	typedef struct tagParticleDesc
	{
		_uint	iMaskIndex;
		_uint	iNumInstance;
		_uint	iBillBoardMode; // 0: Normal, 1: BillBoard, 3: Directional(Up is Dir)
		_float2 fLifeTime; // float2 : StartTimeOffset, Duration
		_bool	bIsLoop;
		_bool	bIsReverseXY_Mask;
		_uint	iUVScaleXCur;
		_uint	iUVScaleYCur;
		_uint	iUVScaleXMax;
		_uint	iUVScaleYMax;
		// Diffuse Color
		_float4 vStartDiffuseColorRGBA_Min;
		_float4 vStartDiffuseColorRGBA_Max;
		_uint	iDiffuseColorEasing_End;
		_uint	iDiffuseColorEasing_Middle;
		_float	fMiddleDiffuseColorTimePoint;
		_float4 vMiddleDiffuseColorRGBA;
		_float4 vEndDiffuseColorRGBA;
		// Glow Color
		_bool	bIsGlowFollowDiffuseColor;
		_float4 vStartGlowColorRGBA_Min;
		_float4 vStartGlowColorRGBA_Max;
		_uint	iGlowColorEasing_End;
		_uint	iGlowColorEasing_Middle;
		_float	fMiddleGlowColorTimePoint;
		_float4 vMiddleGlowColorRGBA;
		_float4 vEndGlowColorRGBA;
		// Shader
		_bool	bIsGlow;
		_bool	bIsBlur;
		//Scale
		_float3 vStartScaleXYZ_Min;
		_float3 vStartScaleXYZ_Max;
		_uint	iScaleEasing_End;
		_uint	iScaleEasing_Middle;
		_float	fMiddleScaleTimePoint;
		_float3 vMiddleScaleXYZ;
		_float3 vEndScaleXYZ;
		// Position Offset
		_float3 vPositionOffset;
		// Start Point Mode
		_uint	iStartPointMode; // 0: Point, 1: Directional(Cube), 3: Directional(Sphere)
		_float3 vRangeXYZ_Min;
		_float3 vRangeXYZ_Max;
		_float	fRangeRadius;
		// Moving Mode;
		_uint	iMovingMode;  // 0: Speed, 1: Accelation, 3: Positioning
		_float3 vStartSpeedXYZ_Min;
		_float3 vStartSpeedXYZ_Max;
		_uint	iSpeedEasing_End;
		_uint	iSpeedEasing_Middle;
		_float	fMiddleSpeedTimePoint;
		_float3 vMiddleSpeedXYZ;
		_float3 vEndSpeedXYZ;
		_float3	vTargetPosition;
		_bool	bIsGatheringOnce;
		_uint	iPositioningEasing_End;
		_uint	iPositioningEasing_Middle;
		_float	fMiddlePositioningTimePoint;
		// Each Particle Accelation
		_float2 fEachAccelation_MinMax; // (Random)float2 : Min, Max
		// Whole Particle Accelation
		_float3 fWholeAccelation;
		// Rotate(Right, Up, Look)
		_float3 vRotateRUL_Min; // (Random)
		_float3 vRotateRUL_Max;
		// Each Particle Rotate Accelation
		_float3 fRotateAccelation_Min; // (Random)float3 : Min, Max
		_float3 fRotateAccelation_Max;

		tagParticleDesc()
			: iMaskIndex(1)
			, iNumInstance(10)
			, iBillBoardMode(0)
			, fLifeTime(0.f, 2.f)
			, bIsLoop(false)
			, bIsReverseXY_Mask(false)
			, iUVScaleXCur(0)
			, iUVScaleYCur(0)
			, iUVScaleXMax(1)
			, iUVScaleYMax(1)
			, vStartDiffuseColorRGBA_Min(1.f, 1.f, 1.f, 1.f)
			, vStartDiffuseColorRGBA_Max(1.f, 1.f, 1.f, 1.f)
			, iDiffuseColorEasing_End(27)
			, iDiffuseColorEasing_Middle(27)
			, fMiddleDiffuseColorTimePoint(0.f)
			, vMiddleDiffuseColorRGBA(1.f, 1.f, 1.f, 1.f)
			, vEndDiffuseColorRGBA(1.f, 1.f, 1.f, 1.f)
			, bIsGlowFollowDiffuseColor(true)
			, vStartGlowColorRGBA_Min(1.f, 1.f, 1.f, 1.f)
			, vStartGlowColorRGBA_Max(1.f, 1.f, 1.f, 1.f)
			, iGlowColorEasing_End(27)
			, iGlowColorEasing_Middle(27)
			, fMiddleGlowColorTimePoint(0.f)
			, vMiddleGlowColorRGBA(1.f, 1.f, 1.f, 1.f)
			, vEndGlowColorRGBA(1.f, 1.f, 1.f, 1.f)
			, bIsGlow(true)
			, bIsBlur(false)
			, vStartScaleXYZ_Min(1.f, 1.f, 1.f)
			, vStartScaleXYZ_Max(1.f, 1.f, 1.f)
			, iScaleEasing_End(27)
			, iScaleEasing_Middle(27)
			, fMiddleScaleTimePoint(0.f)
			, vMiddleScaleXYZ(1.f, 1.f, 1.f)
			, vEndScaleXYZ(1.f, 1.f, 1.f)
			, vPositionOffset(0.f, 0.f, 0.f)
			, iStartPointMode(0) // 0: Point, 1: Directional(Cube), 3: Directional(Sphere)
			, vRangeXYZ_Min(-1.f, -1.f, -1.f)
			, vRangeXYZ_Max(1.f, 1.f, 1.f)
			, iMovingMode(0)
			, vStartSpeedXYZ_Min(-10.f, -10.f, -10.f)
			, vStartSpeedXYZ_Max(10.f, 10.f, 10.f)
			, iSpeedEasing_End(27)
			, iSpeedEasing_Middle(27)
			, fMiddleSpeedTimePoint(0.f)
			, vMiddleSpeedXYZ(1.f, 1.f, 1.f)
			, vEndSpeedXYZ(1.f, 1.f, 1.f)
			, vTargetPosition(0.f, 0.f, 0.f)
			, bIsGatheringOnce(false)
			, iPositioningEasing_End(27)
			, iPositioningEasing_Middle(27)
			, fMiddlePositioningTimePoint(0.f)
			, fEachAccelation_MinMax(0.f, 0.f)
			, fWholeAccelation(0.f, 0.f, 0.f)
			, vRotateRUL_Min(0.f, 0.f, 0.f)
			, vRotateRUL_Max(0.f, 0.f, 0.f)
			, fRotateAccelation_Min(0.f, 0.f, 0.f)
			, fRotateAccelation_Max(0.f, 0.f, 0.f)
		{}

	}PARTICLE_DESC;

	typedef struct tagMeshEffectDesc
	{
		_uint	iMeshIndex;
		_uint	iNoiseIndex;
		_uint	iMaskIndex;
		_bool	bIsBillBoard;
		_float2 fLifeTime; // float2 : StartTimeOffset, Duration
		_bool	bIsLoop;
		// UV Speed(Nosie)
		_bool	bIsReverseXY_Noise;
		_bool	bIsClampSampling_Noise;
		_float2 fUVSpeedXY_Noise;
		_uint	iUVScaleXMax_Noise;
		_uint	iUVScaleYMax_Noise;
		//UV Speed(Mask)
		_bool	bIsReverseXY_Mask;
		_bool	bIsClampSampling_Mask;
		_bool	bIsUVSpeedMode; // Mode: UVSpeed or Sprite
		_float2 fUVSpeedXY_Mask;
		// Sprite(Mask)
		_uint	iUVScaleXMax_Mask;
		_uint	iUVScaleYMax_Mask;
		_float	fSpriteSpeed;
		// Diffuse Color
		_float4 vStartDiffuseColorRGBA;
		_uint	iDiffuseColorEasing_End;
		_uint	iDiffuseColorEasing_Middle;
		_float	fMiddleDiffuseColorTimePoint;
		_float4 vMiddleDiffuseColorRGBA;
		_float4 vEndDiffuseColorRGBA;
		// Shader
		_bool	bIsSoftEffect;
		_bool	bIsGlow;
		// Glow Color
		_bool	bIsGlowFollowDiffuseColor;
		_float4 vStartGlowColorRGBA;
		_uint	iGlowColorEasing_End;
		_uint	iGlowColorEasing_Middle;
		_float	fMiddleGlowColorTimePoint;
		_float4 vMiddleGlowColorRGBA;
		_float4 vEndGlowColorRGBA;
		// Distortion
		_bool	bIsDistortion;
		_float	fStartDistortionStrength;
		_uint	iDistortionStrengthEasing_End;
		_uint	iDistortionStrengthEasing_Middle;
		_float	fMiddleDistortionStrengthTimePoint;
		_float	fMiddleDistortionStrength;
		_float	fEndDistortionStrength;
		// RimLight
		_bool	bIsRimLight;
		_float4 vStartRimLightColorRGBA;
		_uint	iRimLightColorEasing_End;
		_uint	iRimLightColorEasing_Middle;
		_float	fMiddleRimLightColorTimePoint;
		_float4 vMiddleRimLightColorRGBA;
		_float4 vEndRimLightColorRGBA;
		// Dissolve
		_bool	bIsDissolve;
		_uint	iDissolveNoiseTextureIndex;
		_uint	iDissolveColorTextureIndex;
		_float4 vDissolveColorRGBA;
		_float	fDissolveStrength;
		// 미사용
		_bool	bIsTrail;
		_float4 vTrailColor;
		_float	fTrailStrengh;
		_bool	bIsBlur;
		// Position Offset
		_float3 vPositionOffset;
		//Speed
		_bool	bIsSpeedEasingMode;
		_float3 vStartSpeedXYZ;
		_uint	iSpeedEasing_End;
		_uint	iSpeedEasing_Middle;
		_float	fMiddleSpeedTimePoint;
		_float3 vMiddleSpeedXYZ;
		_float3 vEndSpeedXYZ;
		// Accelation
		_float3 fSpeedAccelation;
		//Scale
		_float3 vStartScaleXYZ;
		_uint	iScaleEasing_End;
		_uint	iScaleEasing_Middle;
		_float	fMiddleScaleTimePoint;
		_float3 vMiddleScaleXYZ;
		_float3 vEndScaleXYZ;
		// Rotate(Right, Up, Look)
		_float3 vRotationRUL;
		// Rotate Speed(Right, Up, Look)
		_float3 vRotationSpeedRUL;
		// Rotate Accelation
		_float3 fRotatationAccelation;

		tagMeshEffectDesc()
			: iMeshIndex(0)
			, iNoiseIndex(0)
			, iMaskIndex(0)
			, bIsBillBoard(false)
			, fLifeTime(0.f, 2.f)
			, bIsLoop(false)

			, bIsReverseXY_Noise(false)
			, bIsClampSampling_Noise(false)    
			, fUVSpeedXY_Noise(0.f, 0.f)
			, iUVScaleXMax_Noise(1)
			, iUVScaleYMax_Noise(1)   

			, bIsUVSpeedMode(true)
			, bIsReverseXY_Mask(false)
			, bIsClampSampling_Mask(false)
			, fUVSpeedXY_Mask(0.f, 0.f)
			, iUVScaleXMax_Mask(1)
			, iUVScaleYMax_Mask(1)
			, fSpriteSpeed(1.f)
			, vStartDiffuseColorRGBA(1.f, 1.f, 1.f, 1.f)
			, iDiffuseColorEasing_End(27)
			, iDiffuseColorEasing_Middle(27)
			, fMiddleDiffuseColorTimePoint(0.f)
			, vMiddleDiffuseColorRGBA(1.f, 1.f, 1.f, 1.f)
			, vEndDiffuseColorRGBA(1.f, 1.f, 1.f, 1.f)

			, bIsGlow(false)
			, bIsGlowFollowDiffuseColor(true)
			, vStartGlowColorRGBA(1.f, 1.f, 1.f, 1.f)
			, iGlowColorEasing_End(27)
			, iGlowColorEasing_Middle(27)
			, fMiddleGlowColorTimePoint(0.f)
			, vMiddleGlowColorRGBA(1.f, 1.f, 1.f, 1.f)
			, vEndGlowColorRGBA(1.f, 1.f, 1.f, 1.f)
			, bIsSoftEffect(true)

			, bIsDistortion(false)
			, fStartDistortionStrength(0.f)
			, iDistortionStrengthEasing_End(27)
			, iDistortionStrengthEasing_Middle(27)
			, fMiddleDistortionStrengthTimePoint(0.f)
			, fMiddleDistortionStrength(0.f)
			, fEndDistortionStrength(0.f)
			
			, bIsRimLight(false)
			, vStartRimLightColorRGBA(1.f, 1.f, 1.f, 1.f)
			, iRimLightColorEasing_End(27)
			, iRimLightColorEasing_Middle(27)
			, fMiddleRimLightColorTimePoint(0.f)
			, vMiddleRimLightColorRGBA(1.f, 1.f, 1.f, 1.f)
			, vEndRimLightColorRGBA(1.f, 1.f, 1.f, 1.f)

			, bIsDissolve(false)
			, iDissolveNoiseTextureIndex(0)
			, iDissolveColorTextureIndex(0)
			, vDissolveColorRGBA(1.f, 1.f, 1.f, 1.f)
			, fDissolveStrength(0.f)

			, bIsTrail(false)
			, vTrailColor(1.f, 1.f, 1.f, 1.f)
			, fTrailStrengh(0.f)

			, bIsBlur(false)

			, vPositionOffset(0.f, 0.f, 0.f)
			, bIsSpeedEasingMode(true)
			, vStartSpeedXYZ(0.f, 0.f, 0.f)
			, iSpeedEasing_End(27)
			, iSpeedEasing_Middle(27)
			, fMiddleSpeedTimePoint(0.f)
			, vMiddleSpeedXYZ(0.f, 0.f, 0.f)
			, vEndSpeedXYZ(0.f, 0.f, 0.f)
			, fSpeedAccelation(0.f, 0.f, 0.f)
			, vStartScaleXYZ(1.f, 1.f, 1.f)
			, iScaleEasing_End(27)
			, iScaleEasing_Middle(27)
			, fMiddleScaleTimePoint(0.f)
			, vMiddleScaleXYZ(0.f, 0.f, 0.f)
			, vEndScaleXYZ(0.f, 0.f, 0.f)
			, vRotationRUL(0.f, 0.f, 0.f)
			, vRotationSpeedRUL(0.f, 0.f, 0.f)
			, fRotatationAccelation(0.f, 0.f, 0.f)
		{}
	}MESHEFFECT_DESC;

	//typedef struct tagEffectDesc
	//{
	//	string		Effect_Name;
	//	vector<PARTICLE_DESC> ParticleDesc;
	//	vector<MESHEFFECT_DESC> MeshEffectDesc;
	//}EFFECT_DESC;

	typedef struct tagTerrainCustomDesc
	{
		_uint iNumVerticesX;
		_uint iNumVerticesZ;
		_float fSizeX;
		_float fSizeZ;
		_float3 vPos;
		_tchar* pHeightMapFilePath;

		tagTerrainCustomDesc()
			: iNumVerticesX(0)
			, iNumVerticesZ(0)
			, fSizeX(0.f)
			, fSizeZ(0.f)
			, vPos(0.f, 0.f, 0.f)
			, pHeightMapFilePath(nullptr) {}

	}TERRAIN_DESC;
}
