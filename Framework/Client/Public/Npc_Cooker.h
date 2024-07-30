#pragma once

#include "Client_Defines.h"
#include "Npc.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
class CAnimation;
END

BEGIN(Client)
class CState_Handler;
class CUI;

class CNpc_Cooker final : public CNpc
{
public:
	enum INGRDNUM
	{
		UNITSNUM1,
		TENSNUM1,
		SLASHNUM1,
		NEEDNUM1,

		UNITSNUM2,
		TENSNUM2,
		SLASHNUM2,
		NEEDNUM2,

		NUM_END
	};

public:
	CNpc_Cooker();
	CNpc_Cooker(const CNpc& rhs);
	virtual ~CNpc_Cooker() = default;

public:
	CUI* Get_UI_CookName() { return m_pUI_CookName; }
	CUI* Get_UI_CookDetail() { return m_pUI_CookDetail; }

	CUI* Get_UI_Ingrd1() { return m_pUI_Ingrd1; }
	CUI* Get_UI_Ingrd2() { return m_pUI_Ingrd2; }

	CUI* Get_UI_IngrdNum(INGRDNUM eNumType) { return m_pUI_IngrdNum[eNumType]; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void On_EventMessege(EVENTDESC tEvent) override;
	
private:
	CUI* m_pUI_CookName = nullptr;
	CUI* m_pUI_CookDetail = nullptr;

	CUI* m_pUI_Ingrd1 = nullptr;
	CUI* m_pUI_Ingrd2 = nullptr;

	CUI* m_pUI_IngrdNum[NUM_END];

	CUI* m_pProgress_UnitNum = nullptr;
	CUI* m_pProgress_TenNum = nullptr;
	CUI* m_pProgress_HundredNum = nullptr;

private:
	_uint m_iFoodType = ITEM_END;
	_uint m_iFoodNum = 0;

	_float m_fCookTime = 0.f;
	_uint  m_iAnimIdx = 2;

	vector<CAnimation*> m_vecAnims;

private:
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);
	DECL_ONCE(m_bEffect2);
	DECL_ONCE(m_bEffect3);
	DECL_ONCE(m_bEffect4);
	DECL_ONCE(m_bEffect5);

private:
	void  Interact_Cooker();
	void  Create_Cook_WIndow();
	void  Create_Cook_Progress();
	void  Create_UI_Alert();
	void  SetUp_ProgressNum();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_State_Handler();

public:
	static CNpc_Cooker* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END