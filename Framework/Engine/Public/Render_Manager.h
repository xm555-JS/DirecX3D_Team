#pragma once

#include "Base.h"

BEGIN(Engine)
class CShader;
class CGameObject;
class CRenderer;
class CRenderTarget;
class CTexture;

class CRender_Manager final : public CBase
{
	DECLARE_SINGLETON(CRender_Manager)

private:
	CRender_Manager();
	virtual ~CRender_Manager() = default;

public:
	_float		Get_SaturationRate() { return m_fSaturationRate; }
	_float		Get_Brightness() { return m_fBrightness; }
	_float		Get_DOFRate() { return m_fDOFRate; }
	_float		Get_FogRate() { return m_fFogRate; }
	_vector		Get_FogColor() { return XMLoadFloat4(&m_vFogColor); }
	CRenderer*	Get_CurRenderer() { return m_pCurRenderer; }
	_vector		Get_SunDepth(_uint iX, _uint iY);
	_bool		isUpperThanHight(_vector vPos, _float fOffset = 0.f);

	void		Set_CurRenderer(CRenderer* pRenderer);
	void		Set_SaturationRate(_float fSaturationRate) { m_fSaturationRate = fSaturationRate; }
	void		Set_Brightness(_float fBrightness) { m_fBrightness = fBrightness; }
	void		Set_DOFRate(_float fDOFRate) { m_fDOFRate = fDOFRate; }
	void		Set_FogRate(_float fFogRate) { m_fFogRate = fFogRate; }
	void		Set_FogColor(_vector vDOFColor) {XMStoreFloat4(&m_vFogColor, vDOFColor); }

	void		Set_RadialCA(_float2 vRadialCAPos, _float4 vRadialCAPower) {
		m_vRadialCAPos = vRadialCAPos; 
		m_vRadialCAPower = vRadialCAPower;
	}

	void		Set_DirCA(_float2 vR_Dir, _float2 vG_Dir, _float2 vB_Dir) {
		m_vRCADir = vR_Dir;
		m_vGCADir = vG_Dir;
		m_vBCADir = vB_Dir;
	}

	void		Set_RadialBlur(_float2 vRadialBlurPos, _float fRadialBlurPower, _float fRadialBlurStart, _float fRadialBlurWidth) {
		m_vRadialBlurPos = vRadialBlurPos;
		m_fRadialBlurPower = fRadialBlurPower;
		m_fRadialBlurStart = fRadialBlurStart;
		m_fRadialBlurWidth = fRadialBlurWidth;
	}


	HRESULT		Add_RenderGroup(RENDERGROUP	eGroup, CGameObject* pGameObject);

#ifdef _DEBUG
	void		Set_Debugfloat(_uint iIdx, _float fValue) { m_Debugfloat[iIdx] = fValue; }
	void		Set_Debugint(_uint iIdx, _int iValue) { m_Debugint[iIdx] = iValue; }
#endif // _DEBUG

public:
	virtual HRESULT Initialize();

public:
	HRESULT Draw_RenderGroup();
	void	Creat_DDSFromRT(const _tchar* pMRTTag, const _tchar* pPath);

	//void	Set_SampleObject(CGameObject* pGameObejct, _float fPosX, _float fPosY, _float fScaleX, _float fScaleY);
	//void	Set_SampleTargetState(_float fPosX, _float fPosY, _float fScaleX, _float fScaleY);

private:
	list<CGameObject*>*				m_RenderObjects;
	typedef list<CGameObject*>		RENDEROBJECTS;

private:
	class CTarget_Manager* m_pTarget_Manager = nullptr;
	class CLight_Manager* m_pLight_Manager = nullptr;

	ComPtr<ID3D11Device>					m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>				m_pContext = nullptr;

private:
	_float4						m_vCamPos;
	CRenderer*					m_pCurRenderer = nullptr;

	class CVIBuffer_Rect* m_pVIBuffer = nullptr;

private:
	CShader* m_pShader_Deferred = nullptr;
	CShader* m_pShader_Blur = nullptr;
	CShader* m_pShader_Shadow = nullptr;
	CShader* m_pShader_SSAO = nullptr;
	CShader* m_pShader_Light = nullptr;
	CShader* m_pShader_Outline = nullptr;
	CShader* m_pShader_Bloom = nullptr;
	CShader* m_pShader_Copy = nullptr;
	CShader* m_pShader_Glow = nullptr;
	CShader* m_pShader_Water = nullptr;
	CShader* m_pShader_SSR = nullptr;
	CShader* m_pShader_DOF = nullptr;
	CShader* m_pShader_DownSampling = nullptr;

	/* 직교투영을 위한 정보이다. */
private:
	_float4x4					m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;
	D3D11_VIEWPORT				m_ViewPortDesc;


private:
	_float		m_fSaturationRate = 1.f;
	_float		m_fBrightness = 1.f;

	_float		m_fFogRate = 0.f;
	_float4		m_vFogColor = {1.f, 1.f, 1.f, 1.f};
	
	_float		m_fDOFRate = 1.f;
	

private: /* For Radial Blur*/
	_float2		m_vRadialBlurPos = { 0.f, 0.f };
	_float		m_fRadialBlurPower = 0.f;
	_float		m_fRadialBlurStart = 0.f;
	_float		m_fRadialBlurWidth = 0.f;

private: /* For Radial CA*/
	_float2		m_vRadialCAPos = { 0.5f, 0.5f };
	_float4		m_vRadialCAPower = { 0.f, 0.f, 0.f, 0.f };

private: /* For Dir CA*/
	_float2		m_vRCADir = { 0.f, 0.f };
	_float2		m_vGCADir = { 0.f, 0.f };
	_float2		m_vBCADir = { 0.f, 0.f };


private:
	CTexture*	m_pNoiseTex = nullptr;

private:
	Bitmap*		m_pSunDepth = nullptr;
	Bitmap*		m_pHight = nullptr;
	
	_float4x4	m_HightViewMat;
	_float4x4	m_HightProjMat;


#ifdef _DEBUG
private:
	vector<class CComponent*>	m_DebugComponents;
	_float						m_Debugfloat[16];
	_int						m_Debugint[16];
	//// For Sample Object
	//CGameObject*				m_pSampleObject = nullptr;
	//CRenderTarget*				m_pSampleTarget = nullptr;
#endif // _DEBUG

public:
	HRESULT	Clear_RenderTargets();


public: // 기본 디퍼트용, 플래그 그리기
	//HRESULT Render_Priority();
	HRESULT Render_Terrain();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_AlphaBlend();
	HRESULT Render_NonLight();
	HRESULT Render_UI();
	HRESULT Render_Effect();
	HRESULT Render_Water_Sueface();
	HRESULT	Render_Cloud_Sky();

	HRESULT Render_Lights();
	

public: // 플래그로 할당, 기본 디퍼드 사용해서 그림
	//HRESULT	Render_Capture();
	HRESULT Render_Outline();
	HRESULT Render_Bloom();
	HRESULT	Render_Blur_2Pass(wstring strInMRTTag, wstring strOutMRTTag, _float fPower = 1.f, _float fRange = 1.f, _uint iNumRepeat = 1);
	HRESULT	Render_Blur(wstring strInMRTTag, wstring strOutMRTTag, _uint iPower = 1/*, _float fRange = 1.f, _uint iNumRepeat = 1*/);
	HRESULT	Render_RadialBlur(wstring strInMRTTag, wstring strOutMRTTag, _float2 vPoint, _uint iPower = 1, _float fBlurStart = 1.f, _float fBlurWidth = 0.1f/*, _float fRange = 1.f, _uint iNumRepeat = 1*/);
	HRESULT	Render_Glow();
	HRESULT Render_Decal();
	HRESULT Render_Water();
	HRESULT	Render_Cloud();
	HRESULT Render_SSR();
	

public: // 최종 화면 연산
	HRESULT Render_Screen_Bloom();
	HRESULT Render_Screen_Blur();
	HRESULT Render_Screen_Copy(wstring strInMRTTag, wstring strOutMRTTag, ID3D11DepthStencilView* pDepthStencilView = nullptr);
	HRESULT Render_Screen_CopyBlend(wstring strSourMRTTag, wstring strDestMRTTag, wstring strOutMRTTag, _float fBlendRate = 0.5f, ID3D11DepthStencilView* pDepthStencilView = nullptr);
	HRESULT Render_Screen_HDR();
	HRESULT Render_Screen_Outline();
	HRESULT Render_Screen_Outline_Unit();
	HRESULT Render_Screen_Shadow();
	HRESULT Render_Screen_Shadow_Dir();
	HRESULT Render_Screen_SSAO();
	//HRESULT Render_Screen_DownSampling();

	HRESULT Render_Screen_Blend();



public:
	HRESULT Render_FinalTexture();



#ifdef _DEBUG
public:
	HRESULT Render_Debug();
	//HRESULT Render_SampleObejct();
	

	HRESULT Add_DebugRenderGroup(class CComponent* pComponent);
#endif // _DEBUG

private:
	_float4x4					m_DecalCamViewMatrix, m_DecalCamProjMatrix;


private:
	_float2			SetUp_ViewportPos(_vector vWorldPos);


public:
	virtual void Free() override;
};

END