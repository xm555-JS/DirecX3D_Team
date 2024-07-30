#pragma once
#include "Client_Defines.h"
#include "Window_Imgui.h"
#include "Effect_Manager.h"

#if defined(USE_IMGUI)

BEGIN(Engine)
class CGameInstance;
class CTexture;
class CComponent;
END

BEGIN(Client)

class CWindow_Tool_Effect final : public CWindow_Imgui
{
private:
	const char* szEasingTypes[28] = { "TYPE_Linear", "TYPE_QuadIn", "TYPE_QuadOut", "TYPE_QuadInOut", "TYPE_CubicIn", "TYPE_CubicOut", "TYPE_CubicInOut",  "TYPE_QuarticIn", "TYPE_QuarticOut", "TYPE_QuarticInOut", "TYPE_QuinticIn", "TYPE_QuinticOut", "TYPE_QuinticInOut", "TYPE_SinIn", "TYPE_SinOut", "TYPE_SinInOut", "TYPE_ExpoIn", "TYPE_ExpoOut", "TYPE_ExpoInOut", "TYPE_CircularIn", "TYPE_CircularOut", "TYPE_CircularInOut", "TYPE_ElasticIn", "TYPE_ElasticOut", "TYPE_ElasticInOut", "TYPE_BounceIn", "TYPE_BounceOut", "Disable" };

public:
	CWindow_Tool_Effect();
	virtual ~CWindow_Tool_Effect();

public:
	// CWindow_Imgui을(를) 통해 상속됨
	virtual void Tick(_float fTimeDelta) override;
	virtual void Initialize() override;
	virtual void On_EventMessege(EVENTDESC tEvent) override;

	_bool IsCameraLock(void) { return m_bIsCameraLock; }

private:
	bool show_demo_window                = true;
	bool show_another_window             = false;
	class CGameInstance* m_pGameInstance = nullptr;
	ImGuiIO io;

	void Show_Window_SaveLoad();
	void Show_Window_MeshEffect();
	void Show_Window_Particle();

#pragma region Effect
public:
	//void Set_MeshEffectTag(const _tchar* PrototypeTag) { m_MeshEffectInfo.push_back(PrototypeTag); };

private:
	_bool		m_bIsCameraLock = false;

private:
	map<_uint, CComponent*>				m_mapMeshEffectModel_Prototypes;

	class CTexture* m_pTextureCom_Mask  = nullptr;
	class CTexture* m_pTextureCom_Noise = nullptr;
	stringvec m_vecSavedFiles;

	//EFFECT_DESC m_CurrentEffectDesc;
	string					m_CurrentEffect_Name;
	vector<MESHEFFECT_DESC> m_CurrentEffectDesc_Mesh;
	vector<PARTICLE_DESC> m_CurrentEffectDesc_Particle;


	MESHEFFECT_DESC m_CurrentMeshEffectDesc;
	PARTICLE_DESC m_CurrentParticleDesc;

	_int m_iEffect_idx                 = 0;
	_int m_iJsonFile_idx               = 0;
	_int m_iMeshEffect_Component_idx   = 0;
	_int m_iParticle_Component_idx     = 0;

	char m_szFileName_Cur[128]             = "File_0";
	char m_szFileName_Last[128]            = "File_0";
	char m_szFileName_Save[128]            = "File_0";
	char m_szMetaFileName[128]             = "EffectData";
#pragma endregion Effect


public:
	virtual	void	Free() override;
};

END

#endif