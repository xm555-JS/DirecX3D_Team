#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CQuest final : public CGameObject
{
private:
	CQuest();
	CQuest(const CQuest& rhs);
	virtual ~CQuest() = default;

public:
	void		Set_Complete_Quest() { m_bComplete = true; }
	void		Set_Achieve_Quest() { m_bAchieve = true; }
	void		Set_Quest_Type(QUEST_TYPE eQuest_Type) { m_eQuest_Type = eQuest_Type; }
	QUEST_TYPE  Get_Quest_Type() { return m_eQuest_Type; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void Create_UI_Quest_Notice(_float2 vPos, _float2 vSize, _uint iTextureIdx);
	void Create_UI_Quest();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	QUEST_TYPE m_eQuest_Type = QUEST_END;
	_bool      m_bComplete = false;
	_bool	   m_bAchieve = false;
	class CUI* m_pUI_Quest = nullptr;
	class CUI_Quest_Notice* m_pUI_Notice = nullptr;

	DECL_ONCE(m_bAchieveOnce);

public:
	static CQuest* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

