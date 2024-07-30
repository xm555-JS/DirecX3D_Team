#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Camera_Manager.h"
#include "Timer_Manager.h"
#include "Collision_Manager.h"
#include "PhysX_Manager.h"
#include "PipeLine.h"
#include "Key_Manager.h"
#include "Event_Manager.h"
#include "Font_Manager.h"
#include "Light_Manager.h"
#include "File_Manager.h"
#include "Picking.h"
#include "Frustum.h"
#include "Target_Manager.h"
#include "Render_Manager.h"
#include "Animation_Manager.h"
#include "ScreenEffect_Manager.h"

BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* 엔진 라이브럴를 사용할때 필요한 초기화 과정으르 거칠거야. */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHICDESC& GraphicDesc);
	HRESULT Tick_Engine(_float fTimeDelta);
	HRESULT Render_Engine();
	HRESULT Clear(_uint iLevelID);

public: /* For.Graphic_Device */	
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

	ComPtr<ID3D11Device>			Get_Device();
	ComPtr<ID3D11DeviceContext>		Get_Context();
	ID3D11ShaderResourceView*		Get_BackBufferSRV();
	ID3D11RenderTargetView*			Get_BackBufferRTV();
	IDXGISwapChain*					Get_SwapChain();


public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte byKeyID);
	_byte Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID);
	_long Get_DIMouseMoveState(MOUSEMOVE eMouseMove);

public: /* For.Level_Manager */
	//HRESULT Open_Level(_uint iLevelID, class CLevel* pLevel);
	_uint			Get_CurLevelIndex();
	class CLevel*	Get_CurLevel();

public: /* For.Object_Manager */
	HRESULT				Add_GameObject_Prototype(ID_UINT iPrototypeID, class CGameObject* pPrototype);
	class CGameObject*	Add_GameObject(_uint iLevelIndex, ID_UINT iLayerID, ID_UINT iPrototypeID, void* pArg = nullptr);
	class CGameObject*  Add_GameObject(_uint iLevelIndex, _uint iLayerID, ID_UINT tPrototypeID, void* pArg = nullptr);
	class CGameObject*  Add_GameObject(_uint iLevelIndex, _uint iLayerID, _uint iPrototypeID, void* pArg = nullptr);
	void				Add_DeadObject(CGameObject * pObject);
	void				Add_PoolingObject(CGameObject * pObject);

	void				Add_TickGroup(_uint iGroup, class CGameObject* pGameObject);
	void				Add_LateTickGroup(_uint iGroup, class CGameObject* pGameObject);
	void				Remove_TickGroup(_uint iGroup, CGameObject * pGameObject);
	void				Remove_LateTickGroup(_uint iGroup, CGameObject * pGameObject);
	void				Disable_TickGroup(_uint iGroup);
	void				Disable_LateTickGroup(_uint iGroup);
	void				Able_TickGroup(_uint iGroup);
	void				Able_LateTickGroup(_uint iGroup);

	
	class CComponent*	Get_Component(_uint iLevelIndex, ID_UINT iLayerID, ID_UINT iComponentID, _uint iIndex = 0);
	class CGameObject*	Get_Object(_uint iLevelIndex, ID_UINT iLayerID, _uint iIndex = 0);
	//class CLayer*		Get_Layer(_uint iLevelIndex, ID_UINT iLayerID);
	list<class CGameObject*>* Get_Objects(_uint iLevelIndex, ID_UINT iLayerID);
	HRESULT				Get_Prototypes(ID_UINT iPrototypeID, map<_uint, CGameObject*>*pPrototypes, _uint iUseIDNum = 1);
	void				Set_TransformID(ID_UINT iID);
	ID_UINT				Get_TransformID();
	void				Object_Clear(_uint iLevelIndex);
	HRESULT				Reserve_TickContainer(_uint iNumTickGroups);
	HRESULT				Reserve_LateTickContainer(_uint iNumLateTickGroups);


public: /* For.Component_Mananger */
	HRESULT Add_Component_Prototype(_uint iLevelIndex, ID_UINT iPrototypeID, class CComponent* pPrototype);
	HRESULT Add_Component_Prototype(_uint iLevelIndex, ID_UINT iPrototypeID, CComponent * pPrototype, string strModelFileName);
	class CComponent* Clone_Component(_uint iLevelIndex, ID_UINT iPrototypeID, void* pArg = nullptr);
	HRESULT			  Get_Component_Prototypes(ID_UINT iPrototypeID, map<_uint, CComponent*>*pOut, _uint iUseIDNum = 1);
	HRESULT			  Get_Component_Prototypes(_uint iPrototypeID, map<_uint, CComponent*>*pOut, _uint iUseIDNum = 1);
	HRESULT			  Get_Component_Prototypes(_uint iLevel, ID_UINT iPrototypeID, map<_uint, CComponent*>*pOut, _uint iUseIDNum = 1);
	HRESULT			  Get_Component_Prototypes(_uint iLevel, _uint iPrototypeID, map<_uint, CComponent*>*pOut, _uint iUseIDNum = 1);

public: /* For.Camera_Manager */
	_bool		Get_CursorFix();
	CCamera*	Get_CurrentCamera();
	void		Set_CurrentCamera(CCamera* pCurrentCamera);
	void		Set_NextCamera(CCamera * pNextCamera, _float fTotalMoveTime);
	void		Set_CursorFix(_bool bFixState);
	CCamera*			Find_Camera(const _tchar * pCameraTag);
	list<CCamera*>*		Find_MC(const _tchar * pMCTag);
	HRESULT		Add_MC(const _tchar * pMCTag, const _tchar * pCameraTag);
	void		Add_Camera(const _tchar * pCameraTag, CCamera * pCamera);
	void		Add_Camera_Shaking(_fvector vAxis, _float fPower, _float fPlayTime, _float fCycle);
	void		Play_ActionCamera(const _tchar * pCameraTag, _float fTotalMoveTime);
	void		Play_MAC(const _tchar * pCameraTag, _float fTotalMoveTime);
	HRESULT		Reserve_MoveCam();
	

public: /* For.Timer_Manager */
	_float	Get_TimeDelta(const _tchar * pTimerTag);
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Compute_Timer(const _tchar* pTimerTag);

public: /* For.Pipeline*/
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformState);
	void Set_Viewport(VIEWPORTDESC tDesc);
	_matrix Get_Transform(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_TP(CPipeLine::TRANSFORMSTATE eState);
	_float4 Get_CamPosition();
	VIEWPORTDESC Get_Viewport();
	void	Tick_Pipeline();


public: /* For.Event_Manager*/
	void	Add_Event(const EVENTDESC& _eve);
	void	Add_Event(EVENT_TYPE _eEventType, void* _lParam, void* _wParam);
	void	Change_Level(_uint iLevel, CLevel* pNextLevel);
	//void	Add_DeadObject(CBase * pObject);
	void	Add_DeadBase(CBase * pObject);
	//void	Add_PoolingObject(CGameObject * pObject);
	void	Add_CallbackList(CBase* pBase, EVENT_TYPE eEventType);
	void	Remove_CallbackList(CBase* pBase, EVENT_TYPE eEventType);


public: /* For.Font_Manager */
	HRESULT Add_Font(const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor, _float fSize = 1.f);

public: /* For.Light_Manager */
	CLight* Add_Light(const LIGHTDESC & LightDesc);
	CLight*	Add_Light(CLight* pLight);
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	void	Get_RenderLights(_fvector vPosition, list<CLight*>* pLightList);
	const list<CLight*>* Get_RenderLights();
	void	Set_Sun(CLight* pLight);
	void	Clear_Light();
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Render_Shadows(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Render_AllLights(_vector vPosition, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Render_AllShadows(_vector vPosition, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	_uint	Get_LightNum(_fvector vPosition);

	//void	Remove_Light(CLight* pLight);

	
public: /* For.Picking */
	_bool Picking(class CVIBuffer* pVIBuffer, class CTransform* pTransform, _vector * vOut);
	_bool Picking_Terrain(class CVIBuffer* pVIBuffer, class CTransform* pTransform, _vector * vOut = nullptr, _float* fHight = nullptr, _uint3* pOutIndices = nullptr);
	_bool Picking(CVIBuffer* pVIBuffer, _matrix WorlMatrix, _vector* vOut = nullptr, _float* fHight = nullptr);
	_bool Picking(CVIBuffer* pVIBuffer, CTransform* pTransform, _fvector vRayPos, _fvector vRayDir, _vector* vOut = nullptr, _float* pHight = nullptr);
	HRESULT Picking(_float _fHight, _vector * vOut);
	HRESULT Picking(_float _fHight, _fvector vRayPos, _fvector vRayDir, _vector* vOut);
	_float3 Get_RayDir(void);
	_float3 Get_RayPos(void);

public: /* For.File_Manager */
	HRESULT Open_Save_Handle(HANDLE* hFile, LPCWSTR FilePath);
	HRESULT Open_Load_Handle(HANDLE* hFile, LPCWSTR FilePath);
	HRESULT	Delete_File(LPCWSTR FilePath);
	wstring Data_Complete_Path(const wchar_t* _strFileName);
	HRESULT	Save_Json(const wchar_t* _strPath, json _json);
	HRESULT	Load_Json(const wchar_t* _strPath, json* pOut);
	wstring json_Complete_Path(const wchar_t* _strFileName);

public: /* For.PhysX_Manager*/
	HRESULT	Create_Scene(CPhysX_Manager::Scene eScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));
	HRESULT	Delete_Scene(CPhysX_Manager::Scene eScene);
	HRESULT	Change_Scene(CPhysX_Manager::Scene eNextScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));
	void			Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh** ppOut);
	void			Create_ConvexMesh(PxVec3** pVertices, _uint iNumVertice, PxConvexMesh** ppOut);
	void			Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial** ppOut);
	void			Create_Shape(const PxGeometry & Geometry, PxMaterial* pMaterial, PxShape ** ppOut);
	PxRigidDynamic* Create_DynamicActor(const PxTransform& t, const PxGeometry& geometry, CPhysX_Manager::Scene eScene, const PxReal& Density, const PxVec3& velocity = PxVec3(0), PxMaterial* pMaterial = nullptr);
	PxRigidStatic*	Create_StaticActor(const PxTransform& t, const PxGeometry& geometry, CPhysX_Manager::Scene eScene, PxMaterial* pMaterial = nullptr);
	void			Begin_PhysScene();
	void			End_PhysScene();


public: /* For.Collsion_Mananger */
	HRESULT Add_ColliderGroup(ID_UINT iLayer, CCollider* pCollider);
	void	Check_Collider(ID_UINT iTempLayer, ID_UINT iSourLayer);
	list<CCollider*>* Find_ColliderLayer(ID_UINT iLayerID);


public: /* For.Frustum */
	_bool isIn_Frustum_InWorldSpace(_fvector vWorldPoint, _float fRange = 0.f);

public: /* For.Target_Manage */
	//HRESULT	Add_RenderTarget(const _tchar* pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	//HRESULT	Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	//HRESULT	Begin_MRT(const _tchar* pMRTTag, ID3D11DepthStencilView* pDepthStencilView = nullptr);
	HRESULT Begin_MRT(list<CRenderTarget*>* pMRTList, ID3D11DepthStencilView* pDepthStencilView = nullptr);
	HRESULT	End_MRT();
	//class CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	//list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);
	//ID3D11ShaderResourceView* Get_SRV(const _tchar* pTargetTag);
	class CRenderTarget*	Create_RenderTarget(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);

//#ifdef _DEBUG
//	HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
//#endif // _DEBUG

public: /* For.Render_Manager */
	void		Set_CurRenderer(CRenderer* pRenderer);
	HRESULT		Add_RenderGroup(RENDERGROUP	eGroup, CGameObject* pGameObject);
	HRESULT		Draw_RenderGroup();
	_float		Get_SaturationRate();
	_float		Get_Brightness();
	_float		Get_DOFRate();
	_float		Get_FogRate();
	_vector		Get_FogColor();
	CRenderer*	Get_CurRenderer();
	void		Set_SaturationRate(_float fSaturationRate);
	void		Set_Brightness(_float fBrightness);
	void		Set_DOFRate(_float fDOFRate);
	void		Set_FogRate(_float fDOFRate);
	void		Set_FogColor(_vector vDOFColor);
	void		Set_RadialCA(_float2 vRadialCAPos, _float4 vRadialCAPower);
	void		Set_DirCA(_float2 vR_Dir, _float2 vG_Dir, _float2 vB_Dir);
	void		Set_RadialBlur(_float2 vRadialBlurPos, _float fRadialBlurPower, _float fRadialBlurStart, _float fRadialBlurWidth);
	void		Creat_DDSFromRT(const _tchar* pMRTTag, const _tchar* pPath);
	_bool		isUpperThanHight(_vector vPos, _float fOffset = 0.f);
	

	//void		Set_SampleObject(CGameObject* pGameObejct, _float fPosX, _float fPosY, _float fScaleX, _float fScaleY);
	//void		Set_SampleTargetState(_float fPosX, _float fPosY, _float fScaleX = 100.f, _float fScaleY = 100.f);
#ifdef _DEBUG
	void		Set_Debugfloat(_uint iIdx, _float fValue);
	void		Set_Debugint(_uint iIdx, _int iValue);
	HRESULT		Add_DebugRenderGroup(class CComponent* pComponent);
#endif

public: /* For.ScreenEffect_Manager*/
	void	Add_DirCA(_float2 vDir, _float4 vRGBPower, _float fTotalPlayTime, _float fCycleTime, _bool bLoop = false, _bool bContinue = false);
	void	Add_RadialCA(_float2 vPoint, _float4 vPower, _float fTotalPlayTime, _float fCycleTime, _bool bLoop = false, _bool bContinue = false);
	void	Add_RadialBlur(_float2 vPoint, _float fPower, _float fTotalPlayTime, _float fCycleTime, _float fRadialBlurStart, _float fRadialBlurWidth, _bool bLoop = false, _bool bContinue = false);

	void	Stop_DirCA();
	void	Stop_RadialCA();
	void	Stop_RadialBlur();

public: /* For.Key_Mananger */
	bool	Key_Pressing(int _iKey);
	bool	Key_Down(int _iKey);
	bool	Key_Up(int _iKey);
	bool	Key_None(int _iKey);
	bool	Key_Toggle(int _iKey);

	void	Make_Key_Down(int _iKey);
	void	Make_Key_Up(int _iKey);
	void	Make_Toggle_On(int _iKey);
	void	Make_Toggle_Off(int _iKey);

	void	Key_Lock(int _iKey);
	void	Key_Unlock(int _iKey);

	void	All_Key_Lock();
	void	All_Key_Unlock();

public:/* For.Animation_Manager */
	HRESULT	Reserve_AnimContainer(_uint iNumGroup);
	HRESULT SetUp_HierarchyNodePtr(_uint iAnimGroup, class CModel* pModel);
	//HRESULT SetUp_HierarchyNodePtr(_uint iAnimGroup, class CModel* pModel);
	HRESULT	Load_Animation(const char* pModelFilePath, _uint iAnimGroup);

private:
	CGraphic_Device*				m_pGraphic_Device = nullptr;
	CInput_Device*					m_pInput_Device = nullptr;
	CLevel_Manager*					m_pLevel_Manager = nullptr;
	CObject_Manager*				m_pObject_Manager = nullptr;
	CComponent_Manager*				m_pComponent_Manager = nullptr;
	CCamera_Manager*				m_pCamera_Manager = nullptr;
	CTimer_Manager*					m_pTimer_Manager = nullptr;
	CPipeLine*						m_pPipeLine = nullptr;
	CPhysX_Manager*					m_pPhysX_Manager = nullptr;
	CKey_Manager*					m_pKey_Manager = nullptr;
	CEvent_Manager*					m_pEvent_Manager = nullptr;
	CFont_Manager*					m_pFont_Manager = nullptr;
	CLight_Manager*					m_pLight_Manager = nullptr;
	CFile_Manager*					m_pFile_Manager = nullptr;
	CPicking*						m_pPicking = nullptr;
	CCollision_Manager*				m_pCollision_Manager = nullptr;
	CFrustum*						m_pFrustum = nullptr;
	CTarget_Manager*				m_pTarget_Manager = nullptr;
	CRender_Manager*				m_pRender_Manager = nullptr;
	CAnimation_Manager*				m_pAnimation_Manager = nullptr;
	CScreenEffect_Manager*			m_pScreenEffect_Manager = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END