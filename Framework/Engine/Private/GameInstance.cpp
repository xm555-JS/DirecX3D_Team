#include "..\Public\GameInstance.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pInput_Device(CInput_Device::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pCamera_Manager(CCamera_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pPipeLine(CPipeLine::Get_Instance())
	//, m_pPhysX_Manager(CPhysX_Manager::Get_Instance())
	, m_pKey_Manager(CKey_Manager::Get_Instance())
	, m_pEvent_Manager(CEvent_Manager::Get_Instance())
	, m_pFont_Manager(CFont_Manager::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
	, m_pFile_Manager(CFile_Manager::Get_Instance())
	, m_pPicking(CPicking::Get_Instance())
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
	, m_pFrustum(CFrustum::Get_Instance())
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
	, m_pRender_Manager(CRender_Manager::Get_Instance())
	, m_pAnimation_Manager(CAnimation_Manager::Get_Instance())
	, m_pScreenEffect_Manager(CScreenEffect_Manager::Get_Instance())
{	

	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pCamera_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pPipeLine);
	//Safe_AddRef(m_pPhysX_Manager);
	Safe_AddRef(m_pKey_Manager);
	Safe_AddRef(m_pEvent_Manager);
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pFile_Manager);
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pCollision_Manager);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pRender_Manager);
	Safe_AddRef(m_pAnimation_Manager);
	Safe_AddRef(m_pScreenEffect_Manager);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHICDESC& GraphicDesc)
{
	if (nullptr == m_pGraphic_Device)
		__debugbreak();

	FAILEDCHECK(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.isWindowMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY));
	FAILEDCHECK(m_pInput_Device->Initialize(hInst, GraphicDesc.hWnd));
	FAILEDCHECK(m_pPicking->Initialize(GraphicDesc.hWnd));
	FAILEDCHECK(m_pLevel_Manager->Initialize());
	FAILEDCHECK(m_pObject_Manager->Reserve_Container(iNumLevels));
	FAILEDCHECK(m_pComponent_Manager->Reserve_Container(iNumLevels));
	//FAILEDCHECK(m_pPhysX_Manager->Initialize());
	FAILEDCHECK(m_pFrustum->Initialize());
	FAILEDCHECK(m_pTarget_Manager->Initialize());
	FAILEDCHECK(m_pRender_Manager->Initialize());
	FAILEDCHECK(m_pCamera_Manager->Initialize(GraphicDesc.hWnd, GraphicDesc.iWinCX, GraphicDesc.iWinCY));
	

	return S_OK;	
}

HRESULT CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pInput_Device)
		__debugbreak();

	m_pKey_Manager->Tick();

	m_pInput_Device->SetUp_DeviceState();

	m_pEvent_Manager->Tick(fTimeDelta);
	m_pObject_Manager->Tick(fTimeDelta);
	m_pLevel_Manager->Tick(fTimeDelta);
	m_pLight_Manager->Tick(fTimeDelta);
	m_pCamera_Manager->Tick(fTimeDelta);

	m_pPicking->Compute_RayInWorldSpace();

	m_pPipeLine->Tick();
	m_pFrustum->Tick();
	//m_pPhysX_Manager->Tick(fTimeDelta);

	m_pEvent_Manager->LateTick(fTimeDelta);
	m_pObject_Manager->LateTick(fTimeDelta);
	m_pLevel_Manager->LateTick(fTimeDelta);
	m_pLight_Manager->LateTick(fTimeDelta);
	m_pCollision_Manager->LateTick();

	m_pScreenEffect_Manager->Tick(fTimeDelta);

	return S_OK;
}

HRESULT CGameInstance::Render_Engine()
{

	if (nullptr == m_pLevel_Manager)
		__debugbreak();

	//m_pRender_Manager->Draw_RenderGroup();

	m_pLevel_Manager->Render();


	return S_OK;
}

HRESULT CGameInstance::Clear(_uint iLevelID)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		__debugbreak();

	m_pObject_Manager->Clear(iLevelID);
	m_pComponent_Manager->Clear(iLevelID);

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		__debugbreak();

	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);

	return S_OK;
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		__debugbreak();

	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		__debugbreak();

	m_pGraphic_Device->Present();

	return S_OK;
}

ComPtr<ID3D11Device> CGameInstance::Get_Device()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_Device();
}

ComPtr<ID3D11DeviceContext> CGameInstance::Get_Context()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_Context();
}

ID3D11ShaderResourceView* CGameInstance::Get_BackBufferSRV()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_BackBufferSRV();
}

ID3D11RenderTargetView* CGameInstance::Get_BackBufferRTV()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_BackBufferRTV();
}

IDXGISwapChain* CGameInstance::Get_SwapChain()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_SwapChain();
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseKeyState(eMouseButtonID);
}

_long CGameInstance::Get_DIMouseMoveState(MOUSEMOVE eMouseMove)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMoveState(eMouseMove);
}



_uint CGameInstance::Get_CurLevelIndex()
{
	assert(m_pLevel_Manager);
	return m_pLevel_Manager->Get_CurLevelIndex();
}

CLevel* CGameInstance::Get_CurLevel()
{
	assert(m_pLevel_Manager);
	return m_pLevel_Manager->Get_CurLevel();
}

HRESULT CGameInstance::Add_GameObject_Prototype(ID_UINT iPrototypeID, CGameObject* pPrototype)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Add_GameObject_Prototype(iPrototypeID, pPrototype);
	return S_OK;
}

CGameObject* CGameInstance::Add_GameObject(_uint iLevelIndex, ID_UINT iLayerID, ID_UINT iPrototypeID, void* pArg)
{
	assert(m_pObject_Manager);
	return m_pObject_Manager->Add_GameObject(iLevelIndex, iLayerID, iPrototypeID, pArg);
}

CGameObject* CGameInstance::Add_GameObject(_uint iLevelIndex, _uint iLayerID, ID_UINT tPrototypeID, void* pArg)
{
	assert(m_pObject_Manager);
	return m_pObject_Manager->Add_GameObject(iLevelIndex, iLayerID, tPrototypeID, pArg);
}

CGameObject* CGameInstance::Add_GameObject(_uint iLevelIndex, _uint iLayerID, _uint iPrototypeID, void* pArg)
{
	assert(m_pObject_Manager);
	return m_pObject_Manager->Add_GameObject(iLevelIndex, iLayerID, iPrototypeID, pArg);
}

void CGameInstance::Add_DeadObject(CGameObject* pObject)
{
	assert(m_pObject_Manager);
	return m_pObject_Manager->Add_DeadObject(pObject);
}

void CGameInstance::Add_PoolingObject(CGameObject* pObject)
{
	assert(m_pObject_Manager);
	return m_pObject_Manager->Add_PoolingObject(pObject);
}

void CGameInstance::Add_TickGroup(_uint iGroup, CGameObject* pGameObject)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Add_TickGroup(iGroup, pGameObject);
}

void CGameInstance::Add_LateTickGroup(_uint iGroup, CGameObject* pGameObject)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Add_LateTickGroup(iGroup, pGameObject);
}

void CGameInstance::Remove_TickGroup(_uint iGroup, CGameObject* pGameObject)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Remove_TickGroup(iGroup, pGameObject);
}

void CGameInstance::Remove_LateTickGroup(_uint iGroup, CGameObject* pGameObject)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Remove_LateTickGroup(iGroup, pGameObject);
}

void CGameInstance::Disable_TickGroup(_uint iGroup)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Disable_TickGroup(iGroup);
}

void CGameInstance::Disable_LateTickGroup(_uint iGroup)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Disable_LateTickGroup(iGroup);
}

void CGameInstance::Able_TickGroup(_uint iGroup)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Able_TickGroup(iGroup);
}

void CGameInstance::Able_LateTickGroup(_uint iGroup)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Able_LateTickGroup(iGroup);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, ID_UINT iLayerID, ID_UINT iComponentID, _uint iIndex)
{
	assert(m_pObject_Manager);
	return m_pObject_Manager->Get_Component(iLevelIndex, iLayerID, iComponentID, iIndex);
}

CGameObject* CGameInstance::Get_Object(_uint iLevelIndex, ID_UINT iLayerID, _uint iIndex)
{
	assert(m_pObject_Manager);
	return m_pObject_Manager->Get_Object(iLevelIndex, iLayerID, iIndex);
}

//CLayer* CGameInstance::Get_Layer(_uint iLevelIndex, ID_UINT iLayerID)
//{
//	assert(m_pObject_Manager);
//	return m_pObject_Manager->Get_Layer(iLevelIndex, iLayerID);
//}

list<class CGameObject*>* CGameInstance::Get_Objects(_uint iLevelIndex, ID_UINT iLayerID)
{
	assert(m_pObject_Manager);
	return m_pObject_Manager->Get_Objects(iLevelIndex, iLayerID);
}

HRESULT CGameInstance::Get_Prototypes(ID_UINT iPrototypeID, map<_uint, CGameObject*>* pOut, _uint iUseIDNum)
{

	assert(m_pObject_Manager);
	return m_pObject_Manager->Get_Prototypes(iPrototypeID, pOut, iUseIDNum);

}

void CGameInstance::Set_TransformID(ID_UINT iID)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Set_TransformID(iID);
}

ID_UINT CGameInstance::Get_TransformID()
{
	return m_pObject_Manager->Get_TransformID();
}

void CGameInstance::Object_Clear(_uint iLevelIndex)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Clear(iLevelIndex);
}

HRESULT CGameInstance::Reserve_TickContainer(_uint iNumTickGroups)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Reserve_TickContainer(iNumTickGroups);
	return S_OK;
}

HRESULT CGameInstance::Reserve_LateTickContainer(_uint iNumLateTickGroups)
{
	assert(m_pObject_Manager);
	m_pObject_Manager->Reserve_LateTickContainer(iNumLateTickGroups);
	return S_OK;
}


HRESULT CGameInstance::Add_Component_Prototype(_uint iLevelIndex, ID_UINT iPrototypeID, CComponent* pPrototype)
{
	assert(m_pComponent_Manager);
	return m_pComponent_Manager->Add_Component_Prototype(iLevelIndex, iPrototypeID, pPrototype);
}

HRESULT CGameInstance::Add_Component_Prototype(_uint iLevelIndex, ID_UINT iPrototypeID, CComponent* pPrototype, string strModelFileName)
{
	assert(m_pComponent_Manager);
	return m_pComponent_Manager->Add_Component_Prototype(iLevelIndex, iPrototypeID, pPrototype, strModelFileName);
}

CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, ID_UINT iPrototypeID, void* pArg)
{
	assert(m_pComponent_Manager);
	return m_pComponent_Manager->Clone_Component(iLevelIndex, iPrototypeID, pArg);
}

HRESULT CGameInstance::Get_Component_Prototypes(ID_UINT iPrototypeID, map<_uint, CComponent*>* pOut, _uint iUseIDNum)
{
	assert(m_pComponent_Manager);
	return m_pComponent_Manager->Get_Component_Prototypes(iPrototypeID, pOut, iUseIDNum);
}

HRESULT CGameInstance::Get_Component_Prototypes(_uint iPrototypeID, map<_uint, CComponent*>* pOut, _uint iUseIDNum)
{
	assert(m_pComponent_Manager);
	return m_pComponent_Manager->Get_Component_Prototypes(iPrototypeID, pOut, iUseIDNum);
}

HRESULT CGameInstance::Get_Component_Prototypes(_uint iLevel, ID_UINT iPrototypeID, map<_uint, CComponent*>* pOut, _uint iUseIDNum)
{
	assert(m_pComponent_Manager);
	return m_pComponent_Manager->Get_Component_Prototypes(iLevel, iPrototypeID, pOut, iUseIDNum);
}

HRESULT CGameInstance::Get_Component_Prototypes(_uint iLevel, _uint iPrototypeID, map<_uint, CComponent*>* pOut, _uint iUseIDNum)
{
	assert(m_pComponent_Manager);
	return m_pComponent_Manager->Get_Component_Prototypes(iLevel, iPrototypeID, pOut, iUseIDNum);
}

void CGameInstance::Set_CurrentCamera(CCamera* pCurrentCamera)
{
	assert(m_pCamera_Manager);
	m_pCamera_Manager->Set_CurrentCamera(pCurrentCamera);
}

void CGameInstance::Set_NextCamera(CCamera* pNextCamera, _float fTotalMoveTime)
{
	assert(m_pCamera_Manager);
	m_pCamera_Manager->Set_NextCamera(pNextCamera, fTotalMoveTime);
}

void CGameInstance::Set_CursorFix(_bool bFixState)
{
	assert(m_pCamera_Manager);
	return m_pCamera_Manager->Set_CursorFix(bFixState);
}

CCamera* CGameInstance::Find_Camera(const _tchar* pCameraTag)
{
	assert(m_pCamera_Manager);
	return m_pCamera_Manager->Find_Camera(pCameraTag);
}

list<CCamera*>* CGameInstance::Find_MC(const _tchar* pMCTag)
{
	assert(m_pCamera_Manager);
	return m_pCamera_Manager->Find_MC(pMCTag);
}

HRESULT CGameInstance::Add_MC(const _tchar* pMCTag, const _tchar* pCameraTag)
{
	assert(m_pCamera_Manager);
	return m_pCamera_Manager->Add_MC(pMCTag, pCameraTag);
}

void CGameInstance::Add_Camera(const _tchar* pCameraTag, CCamera* pCamera)
{
	assert(m_pCamera_Manager);
	m_pCamera_Manager->Add_Camera(pCameraTag, pCamera);
}

_bool CGameInstance::Get_CursorFix()
{
	assert(m_pCamera_Manager);
	return m_pCamera_Manager->Get_CursorFix();
}

CCamera* CGameInstance::Get_CurrentCamera()
{
	assert(m_pCamera_Manager);
	return m_pCamera_Manager->Get_CurrentCamera();
}

void CGameInstance::Add_Camera_Shaking(_fvector vAxis, _float fPower, _float fPlayTime, _float fCycle)
{
	assert(m_pCamera_Manager);
	return m_pCamera_Manager->Add_Camera_Shaking(vAxis, fPower, fPlayTime, fCycle);
}

void CGameInstance::Play_ActionCamera(const _tchar* pCameraTag, _float fTotalMoveTime)
{
	assert(m_pCamera_Manager);
	m_pCamera_Manager->Play_ActionCamera(pCameraTag, fTotalMoveTime);
}

void CGameInstance::Play_MAC(const _tchar* pCameraTag, _float fTotalMoveTime)
{
	assert(m_pCamera_Manager);
	m_pCamera_Manager->Play_MAC(pCameraTag, fTotalMoveTime);
}

HRESULT CGameInstance::Reserve_MoveCam()
{
	assert(m_pCamera_Manager);
	return m_pCamera_Manager->Reserve_MoveCam();
}

//HRESULT CGameInstance::Open_Level(_uint iLevelID, CLevel * pLevel)
//{
//	if (nullptr == m_pLevel_Manager)
//		__debugbreak();
//
//	return m_pLevel_Manager->Open_Level(iLevelID, pLevel);
//}

_float CGameInstance::Get_TimeDelta(const _tchar* pTimerTag)
{
	NULLCHECK(m_pTimer_Manager);
	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		__debugbreak();

	return m_pTimer_Manager->Add_Timer(pTimerTag);
}

_float CGameInstance::Compute_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_Timer(pTimerTag);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformState)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, TransformState);
}

void CGameInstance::Set_Viewport(VIEWPORTDESC tDesc)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Viewport(tDesc);
}

_matrix CGameInstance::Get_Transform(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform(eState);
}

const _float4x4* CGameInstance::Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return nullptr;

	return m_pPipeLine->Get_Transform_float4x4(eState);
}

const _float4x4* CGameInstance::Get_Transform_TP(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return nullptr;

	return m_pPipeLine->Get_Transform_TP(eState);
}

_float4 CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return _float4(0.f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_CamPosition();
}

VIEWPORTDESC CGameInstance::Get_Viewport()
{
	assert(m_pPipeLine);
	return m_pPipeLine->Get_Viewport();
}

void CGameInstance::Tick_Pipeline()
{
	assert(m_pPipeLine);
	return m_pPipeLine->Tick();
}

void CGameInstance::Add_Event(const EVENTDESC& _eve)
{
	if (nullptr == m_pEvent_Manager)
	{
		MSG_BOX("m_pEvent_Manager is empty");
		return;
	}

	m_pEvent_Manager->Add_Event(_eve);
}

void CGameInstance::Add_Event(EVENT_TYPE _eEventType, void* _lParam, void* _wParam)
{
	assert(m_pEvent_Manager);
	m_pEvent_Manager->Add_Event(_eEventType, _lParam, _wParam);
}

void CGameInstance::Change_Level(_uint iLevel, CLevel* pNextLevel)
{
	if (nullptr == m_pEvent_Manager)
	{
		MSG_BOX("m_pEvent_Manager is empty");
		return;
	}

	m_pEvent_Manager->Change_Level(iLevel, pNextLevel);
}

//void CGameInstance::Add_DeadObject(CBase* pObject)
//{
//	if (nullptr == m_pEvent_Manager)
//	{
//		MSG_BOX("m_pEvent_Manager is empty");
//		return;
//	}
//
//	m_pEvent_Manager->Add_DeadObject(pObject);
//}

void CGameInstance::Add_DeadBase(CBase* pObject)
{
	if (nullptr == m_pEvent_Manager)
	{
		MSG_BOX("m_pEvent_Manager is empty");
		return;
	}

	m_pEvent_Manager->Add_DeadBase(pObject);
}

//void CGameInstance::Add_PoolingObject(CGameObject* pObject)
//{
//	NULLCHECK(m_pEvent_Manager);
//	m_pEvent_Manager->Add_PoolingObject(pObject);
//}

void CGameInstance::Add_CallbackList(CBase* pBase, EVENT_TYPE eEventType)
{
	if (nullptr == m_pEvent_Manager)
	{
		MSG_BOX("m_pEvent_Manager is empty");
		return;
	}
	m_pEvent_Manager->Add_CallbackList(pBase, eEventType);
}

void CGameInstance::Remove_CallbackList(CBase* pBase, EVENT_TYPE eEventType)
{
	if (nullptr == m_pEvent_Manager)
	{
		MSG_BOX("m_pEvent_Manager is empty");
		return;
	}
	m_pEvent_Manager->Remove_CallbackList(pBase, eEventType);
}

HRESULT CGameInstance::Add_Font(const _tchar* pFontTag, const _tchar* pFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		__debugbreak();

	return m_pFont_Manager->Add_Font(pFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor, _float fSize)
{
	if (nullptr == m_pFont_Manager)
		__debugbreak();

	return m_pFont_Manager->Render_Font(pFontTag, pString, vPosition, vColor, fSize);
}

CLight* CGameInstance::Add_Light(const LIGHTDESC& LightDesc)
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Add_Light(LightDesc);
}

CLight* CGameInstance::Add_Light(CLight* pLight)
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Add_Light(pLight);
}

const LIGHTDESC* CGameInstance::Get_LightDesc(_uint iIndex)
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Get_LightDesc(iIndex);
}

void CGameInstance::Get_RenderLights(_fvector vPosition, list<CLight*>* pLightList)
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Get_RenderLights(vPosition, pLightList);
}

const list<CLight*>* CGameInstance::Get_RenderLights()
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Get_RenderLights();
}

void CGameInstance::Set_Sun(CLight* pLight)
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Set_Sun(pLight);
}

void CGameInstance::Clear_Light()
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Clear();
}

HRESULT CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Render_Lights(pShader, pVIBuffer);
}

HRESULT CGameInstance::Render_Shadows(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Render_Shadows(pShader, pVIBuffer);
}

HRESULT CGameInstance::Render_AllShadows(_vector vPosition, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Render_AllShadows(vPosition, pShader, pVIBuffer);
}

HRESULT CGameInstance::Render_AllLights(_vector vPosition, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Render_AllLights(vPosition, pShader, pVIBuffer);
}

_uint CGameInstance::Get_LightNum(_fvector vPosition)
{
	assert(m_pLight_Manager);
	return m_pLight_Manager->Get_LightNum(vPosition);
}

//void CGameInstance::Remove_Light(CLight* pLight)
//{
//	assert(m_pLight_Manager);
//	return m_pLight_Manager->Remove_Light(pLight);
//}

_bool CGameInstance::Picking(CVIBuffer* pVIBuffer, CTransform* pTransform, _vector* vOut)
{
	assert(m_pPicking);
	return m_pPicking->Picking(pVIBuffer, pTransform, vOut);
}

_bool CGameInstance::Picking_Terrain(CVIBuffer* pVIBuffer, CTransform* pTransform, _vector* vOut, _float* fHight, _uint3* pOutIndices)
{
	assert(m_pPicking);
	return m_pPicking->Picking_Terrain(pVIBuffer, pTransform, vOut, fHight, pOutIndices);
}

_bool CGameInstance::Picking(CVIBuffer* pVIBuffer, _matrix WorlMatrix, _vector* vOut, _float* fHight)
{
	assert(m_pPicking);
	return m_pPicking->Picking(pVIBuffer, WorlMatrix, vOut, fHight);
}

_bool CGameInstance::Picking(CVIBuffer* pVIBuffer, CTransform* pTransform, _fvector vRayPos, _fvector vRayDir, _vector* vOut, _float* pHight)
{
	assert(m_pPicking);
	return m_pPicking->Picking(pVIBuffer, pTransform, vRayPos, vRayDir, vOut, pHight);
}

HRESULT CGameInstance::Picking(_float _fHight, _vector* vOut)
{
	assert(m_pPicking);
	return m_pPicking->Picking(_fHight, vOut);
}

HRESULT CGameInstance::Picking(_float _fHight, _fvector vRayPos, _fvector vRayDir, _vector* vOut)
{
	assert(m_pPicking);
	return m_pPicking->Picking(_fHight, vRayPos, vRayDir, vOut);
}

_float3 CGameInstance::Get_RayDir(void)
{
	assert(m_pPicking);
	return m_pPicking->Get_RayDir();
}

_float3 CGameInstance::Get_RayPos(void)
{
	assert(m_pPicking);
	return m_pPicking->Get_RayPos();
}


HRESULT CGameInstance::Open_Save_Handle(HANDLE* hFile, LPCWSTR FilePath)
{
	if (nullptr == m_pFile_Manager)
		__debugbreak();

	return m_pFile_Manager->Open_Save_Handle(hFile, FilePath);
}

HRESULT CGameInstance::Open_Load_Handle(HANDLE* hFile, LPCWSTR FilePath)
{
	if (nullptr == m_pFile_Manager)
		__debugbreak();

	return m_pFile_Manager->Open_Load_Handle(hFile, FilePath);
}

HRESULT CGameInstance::Delete_File(LPCWSTR FilePath)
{
	if (nullptr == m_pFile_Manager)
		__debugbreak();

	return m_pFile_Manager->Delete_File(FilePath);
}

wstring CGameInstance::Data_Complete_Path(const wchar_t* _strFileName)
{
	if (nullptr == m_pFile_Manager)
		return L"";

	return m_pFile_Manager->Data_Complete_Path(_strFileName);
}

HRESULT CGameInstance::Save_Json(const wchar_t* _strPath, json _json)
{
	if (nullptr == m_pFile_Manager)
		__debugbreak();

	return m_pFile_Manager->Save_Json(_strPath, _json);
}

HRESULT CGameInstance::Load_Json(const wchar_t* _strPath, json* pOut)
{
	if (nullptr == m_pFile_Manager)
		__debugbreak();

	return m_pFile_Manager->Load_Json(_strPath, pOut);
}

wstring CGameInstance::json_Complete_Path(const wchar_t* _strFileName)
{
	if (nullptr == m_pFile_Manager)
		return L"";

	return m_pFile_Manager->json_Complete_Path(_strFileName);
}

HRESULT CGameInstance::Create_Scene(CPhysX_Manager::Scene eScene, PxVec3 Gravity)
{
	assert(m_pPhysX_Manager);
	m_pPhysX_Manager->Create_Scene(eScene, Gravity);
	return S_OK;
}

HRESULT CGameInstance::Delete_Scene(CPhysX_Manager::Scene eScene)
{
	assert(m_pPhysX_Manager);
	m_pPhysX_Manager->Delete_Scene(eScene);
	return S_OK;
}

HRESULT CGameInstance::Change_Scene(CPhysX_Manager::Scene eNextScene, PxVec3 Gravity)
{
	assert(m_pPhysX_Manager);
	m_pPhysX_Manager->Change_Scene(eNextScene, Gravity);
	return S_OK;
}

void CGameInstance::Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh** ppOut)
{
	assert(m_pPhysX_Manager);
	m_pPhysX_Manager->Create_CylinderMesh(fRadiusBelow, fRadiusUpper, fHight, ppOut);
}

void CGameInstance::Create_ConvexMesh(PxVec3** pVertices, _uint iNumVertice, PxConvexMesh** ppOut)
{
	assert(m_pPhysX_Manager);
	m_pPhysX_Manager->Create_ConvexMesh(pVertices, iNumVertice, ppOut);
}

void CGameInstance::Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial** ppOut)
{
	assert(m_pPhysX_Manager);
	m_pPhysX_Manager->Create_Material(fStaticFriction, fDynamicFriction, fRestitution, ppOut);
}

void CGameInstance::Create_Shape(const PxGeometry& Geometry, PxMaterial* pMaterial, PxShape** ppOut)
{
	assert(m_pPhysX_Manager);
	return m_pPhysX_Manager->Create_Shape(Geometry, pMaterial, ppOut);
}

PxRigidDynamic* CGameInstance::Create_DynamicActor(const PxTransform& t, const PxGeometry& geometry, CPhysX_Manager::Scene eScene, const PxReal& Density, const PxVec3& velocity, PxMaterial* pMaterial)
{
	assert(m_pPhysX_Manager);
	return m_pPhysX_Manager->Create_DynamicActor(t, geometry, eScene, Density, velocity, pMaterial);
}

PxRigidStatic* CGameInstance::Create_StaticActor(const PxTransform& t, const PxGeometry& geometry, CPhysX_Manager::Scene eScene, PxMaterial* pMaterial)
{
	assert(m_pPhysX_Manager);
	return m_pPhysX_Manager->Create_StaticActor(t, geometry, eScene, pMaterial);
}

void CGameInstance::Begin_PhysScene()
{
	assert(m_pPhysX_Manager);
	return m_pPhysX_Manager->Begin_PhysScene();
}

void CGameInstance::End_PhysScene()
{
	assert(m_pPhysX_Manager);
	return m_pPhysX_Manager->End_PhysScene();
}

HRESULT CGameInstance::Add_ColliderGroup(ID_UINT iLayer, CCollider* pCollider)
{
	if (nullptr == m_pCollision_Manager)
		__debugbreak();

	return m_pCollision_Manager->Add_ColliderGroup(iLayer, pCollider);
}

void CGameInstance::Check_Collider(ID_UINT iTempLayer, ID_UINT iSourLayer)
{
	assert(m_pCollision_Manager);
	return m_pCollision_Manager->Check_Collider(iTempLayer, iSourLayer);
}

list<CCollider*>* CGameInstance::Find_ColliderLayer(ID_UINT iLayerID)
{
	assert(m_pCollision_Manager);
	return m_pCollision_Manager->Find_ColliderLayer(iLayerID);
}

_bool CGameInstance::isIn_Frustum_InWorldSpace(_fvector vWorldPoint, _float fRange)
{
	assert(m_pFrustum);
	return m_pFrustum->isIn_Frustum_InWorldSpace(vWorldPoint, fRange);
}

//HRESULT CGameInstance::Add_RenderTarget(const _tchar * pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
//{
//	assert(m_pTarget_Manager);
//	return m_pTarget_Manager->Add_RenderTarget(pTargetTag, iWidth, iHeight, eFormat, vClearColor);
//}
//
//HRESULT CGameInstance::Add_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
//{
//	assert(m_pTarget_Manager);
//	return m_pTarget_Manager->Add_MRT(pMRTTag, pTargetTag);
//}
//
//HRESULT CGameInstance::Begin_MRT(const _tchar * pMRTTag, ID3D11DepthStencilView * pDepthStencilView)
//{
//	assert(m_pTarget_Manager);
//	return m_pTarget_Manager->Begin_MRT(pMRTTag, pDepthStencilView);
//}

HRESULT CGameInstance::Begin_MRT(list<CRenderTarget*>* pMRTList, ID3D11DepthStencilView* pDepthStencilView)
{
	assert(m_pTarget_Manager);
	return m_pTarget_Manager->Begin_MRT(pMRTList, pDepthStencilView);
}

HRESULT CGameInstance::End_MRT()
{
	assert(m_pTarget_Manager);
	return m_pTarget_Manager->End_MRT();
}

//CRenderTarget * CGameInstance::Find_RenderTarget(const _tchar * pTargetTag)
//{
//	assert(m_pTarget_Manager);
//	return m_pTarget_Manager->Find_RenderTarget(pTargetTag);
//}
//
//list<class CRenderTarget*>* CGameInstance::Find_MRT(const _tchar * pMRTTag)
//{
//	assert(m_pTarget_Manager);
//	return m_pTarget_Manager->Find_MRT(pMRTTag);
//}
//
//ID3D11ShaderResourceView * CGameInstance::Get_SRV(const _tchar * pTargetTag)
//{
//	assert(m_pTarget_Manager);
//	return m_pTarget_Manager->Get_SRV(pTargetTag);
//}

CRenderTarget * CGameInstance::Create_RenderTarget(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	assert(m_pTarget_Manager);
	return m_pTarget_Manager->Create_RenderTarget(iWidth, iHeight, eFormat, vClearColor);
}

void CGameInstance::Set_CurRenderer(CRenderer* pRenderer)
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->Set_CurRenderer(pRenderer);
}

HRESULT CGameInstance::Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject)
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->Add_RenderGroup(eGroup, pGameObject);
}

HRESULT CGameInstance::Draw_RenderGroup()
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->Draw_RenderGroup();
}

_float CGameInstance::Get_SaturationRate()
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->Get_SaturationRate();
}

_float CGameInstance::Get_Brightness()
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->Get_Brightness();
}

_float CGameInstance::Get_DOFRate()
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->Get_DOFRate();
}

_float CGameInstance::Get_FogRate()
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->Get_FogRate();
}

_vector CGameInstance::Get_FogColor()
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->Get_FogColor();
}

CRenderer* CGameInstance::Get_CurRenderer()
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->Get_CurRenderer();
}

void CGameInstance::Set_SaturationRate(_float fSaturationRate)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Set_SaturationRate(fSaturationRate);
}

void CGameInstance::Set_Brightness(_float fBrightness)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Set_Brightness(fBrightness);
}

void CGameInstance::Set_DOFRate(_float fDOFRate)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Set_DOFRate(fDOFRate);
}

void CGameInstance::Set_FogRate(_float fDOFRate)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Set_FogRate(fDOFRate);
}

void CGameInstance::Set_FogColor(_vector vDOFColor)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Set_FogColor(vDOFColor);
}

void CGameInstance::Set_RadialCA(_float2 vRadialCAPos, _float4 vRadialCAPower)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Set_RadialCA(vRadialCAPos, vRadialCAPower);
}

void CGameInstance::Set_DirCA(_float2 vR_Dir, _float2 vG_Dir, _float2 vB_Dir)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Set_DirCA(vR_Dir, vG_Dir, vB_Dir);
}

void CGameInstance::Set_RadialBlur(_float2 vRadialBlurPos, _float fRadialBlurPower, _float fRadialBlurStart, _float fRadialBlurWidth)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Set_RadialBlur(vRadialBlurPos, fRadialBlurPower, fRadialBlurStart, fRadialBlurWidth);
}

void CGameInstance::Creat_DDSFromRT(const _tchar* pMRTTag, const _tchar* pPath)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Creat_DDSFromRT(pMRTTag, pPath);
}

_bool CGameInstance::isUpperThanHight(_vector vPos, _float fOffset)
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->isUpperThanHight(vPos, fOffset);
}

#ifdef _DEBUG
void CGameInstance::Set_Debugfloat(_uint iIdx, _float fValue)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Set_Debugfloat(iIdx, fValue);
}

void CGameInstance::Set_Debugint(_uint iIdx, _int iValue)
{
	assert(m_pRender_Manager);
	m_pRender_Manager->Set_Debugint(iIdx, iValue);
}

HRESULT CGameInstance::Add_DebugRenderGroup(CComponent* pComponent)
{
	assert(m_pRender_Manager);
	return m_pRender_Manager->Add_DebugRenderGroup(pComponent);
}
#endif // DEBUG

void CGameInstance::Add_DirCA(_float2 vDir, _float4 vRGBPower, _float fTotalPlayTime, _float fCycleTime, _bool bLoop, _bool bContinue)
{
	assert(m_pScreenEffect_Manager);
	return m_pScreenEffect_Manager->Add_DirCA(vDir, vRGBPower, fTotalPlayTime, fCycleTime, bLoop, bContinue);
}

void CGameInstance::Add_RadialCA(_float2 vPoint, _float4 vPower, _float fTotalPlayTime, _float fCycleTime, _bool bLoop, _bool bContinue)
{
	assert(m_pScreenEffect_Manager);
	return m_pScreenEffect_Manager->Add_RadialCA(vPoint, vPower, fTotalPlayTime, fCycleTime, bLoop, bContinue);
}

void CGameInstance::Add_RadialBlur(_float2 vPoint, _float fPower, _float fTotalPlayTime, _float fCycleTime, _float fRadialBlurStart, _float fRadialBlurWidth, _bool bLoop, _bool bContinue)
{
	assert(m_pScreenEffect_Manager);
	return m_pScreenEffect_Manager->Add_RadialBlur(vPoint, fPower, fTotalPlayTime, fCycleTime, fRadialBlurStart, fRadialBlurWidth, bLoop, bContinue);
}

//void CGameInstance::Set_SampleObject(CGameObject* pGameObejct, _float fPosX, _float fPosY, _float fScaleX, _float fScaleY)
//{
//	assert(m_pRender_Manager);
//	return m_pRender_Manager->Set_SampleObject(pGameObejct, fPosX, fPosY, fScaleX, fScaleY);
//}
//
//void CGameInstance::Set_SampleTargetState(_float fPosX, _float fPosY, _float fScaleX, _float fScaleY)
//{
//	assert(m_pRender_Manager);
//	return m_pRender_Manager->Set_SampleTargetState(fPosX, fPosY, fScaleX, fScaleY);
//}


//#ifdef _DEBUG
//HRESULT CGameInstance::Ready_Debug(const _tchar * pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
//{
//	assert(m_pTarget_Manager);
//	return m_pTarget_Manager->Ready_Debug(pTargetTag, fX, fY, fSizeX, fSizeY);
//}
//#endif // _DEBUG

void CGameInstance::Stop_DirCA()
{
	assert(m_pScreenEffect_Manager);
	return m_pScreenEffect_Manager->Stop_DirCA();
}

void CGameInstance::Stop_RadialCA()
{
	assert(m_pScreenEffect_Manager);
	return m_pScreenEffect_Manager->Stop_RadialCA();
}

void CGameInstance::Stop_RadialBlur()
{
	assert(m_pScreenEffect_Manager);
	return m_pScreenEffect_Manager->Stop_RadialBlur();
}

bool CGameInstance::Key_Pressing(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Key_Pressing(_iKey);
}

bool CGameInstance::Key_Down(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Key_Down(_iKey);
}

bool CGameInstance::Key_Up(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Key_Up(_iKey);
}

bool CGameInstance::Key_None(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Key_None(_iKey);
}

bool CGameInstance::Key_Toggle(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Key_Toggle(_iKey);
}

void CGameInstance::Make_Key_Down(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Make_Key_Down(_iKey);
}

void CGameInstance::Make_Key_Up(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Make_Key_Up(_iKey);
}

void CGameInstance::Make_Toggle_On(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Make_Toggle_On(_iKey);
}

void CGameInstance::Make_Toggle_Off(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Make_Toggle_Off(_iKey);
}

void CGameInstance::Key_Lock(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Key_Lock(_iKey);
}

void CGameInstance::Key_Unlock(int _iKey)
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->Key_Unlock(_iKey);
}

void CGameInstance::All_Key_Lock()
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->All_Key_Lock();
}

void CGameInstance::All_Key_Unlock()
{
	assert(m_pKey_Manager);
	return m_pKey_Manager->All_Key_Unlock();
}

HRESULT CGameInstance::Reserve_AnimContainer(_uint iNumGroup)
{
	assert(m_pAnimation_Manager);
	return m_pAnimation_Manager->Reserve_Container(iNumGroup);
}

HRESULT CGameInstance::SetUp_HierarchyNodePtr(_uint iAnimGroup, CModel* pModel)
{
	assert(m_pAnimation_Manager);
	return m_pAnimation_Manager->SetUp_HierarchyNodePtr(iAnimGroup, pModel);
}

//HRESULT CGameInstance::SetUp_HierarchyNodePtr(_uint iAnimGroup, CModel* pModel)
//{
//	assert(m_pAnimation_Manager);
//	return m_pAnimation_Manager->SetUp_HierarchyNodePtr(iAnimGroup, pModel);
//}

HRESULT CGameInstance::Load_Animation(const char* pModelFilePath, _uint iAnimGroup)
{
	assert(m_pAnimation_Manager);
	return m_pAnimation_Manager->Load_Animation(pModelFilePath, iAnimGroup);
}


void CGameInstance::Release_Engine()
{
	_uint RefCnt = 0;

	RefCnt = CScreenEffect_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CObject_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CComponent_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CCollision_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CAnimation_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CCamera_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CLevel_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CPipeLine::Get_Instance()->Destroy_Instance();

	RefCnt = CPicking::Get_Instance()->Destroy_Instance();

	RefCnt = CFont_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CTimer_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CFile_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CLight_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CFrustum::Get_Instance()->Destroy_Instance();

	RefCnt = CEvent_Manager::Get_Instance()->Destroy_Instance();

	//RefCnt = CPhysX_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CTarget_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CRender_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CInput_Device::Get_Instance()->Destroy_Instance();

	RefCnt = CKey_Manager::Get_Instance()->Destroy_Instance();

	RefCnt = CGraphic_Device::Get_Instance()->Destroy_Instance();

	RefCnt = CGameInstance::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pScreenEffect_Manager);
	Safe_Release(m_pAnimation_Manager);
	Safe_Release(m_pFile_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pCamera_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPicking);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pEvent_Manager);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pRender_Manager);
	//Safe_Release(m_pPhysX_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pGraphic_Device);
}
