#pragma once

#include "Client_Defines.h"
#include "Npc.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)
class CState_Handler;

class CNpc_Shirley final : public CNpc
{
public:
	enum SCRIPT
	{
		// 1회 주문 퀘스트 스크립트
		CUBE_ORDER_SCRIPT_1,
		CUBE_ORDER_SCRIPT_2,
		CUBE_ORDER_SCRIPT_3,
		CUBE_ORDER_SCRIPT_4,
		CUBE_ORDER_SCRIPT_5,
		CUBE_ORDER_SCRIPT_6,
		CUBE_ORDER_SCRIPT_7,
		CUBE_ORDER_SCRIPT_8,

		// 1회 주문 완료 
		CUBE_ORDER_SCRIPT_CLEAR,
		CUBE_ORDER_SCRIPT_CLEAR_END,

		// 상추 퀘스트 스크립트
		COLLECT_LETTUCE_SCRIPT_1,
		COLLECT_LETTUCE_SCRIPT_2,
		COLLECT_LETTUCE_SCRIPT_3,
		COLLECT_LETTUCE_SCRIPT_4,
		COLLECT_LETTUCE_SCRIPT_5,
		COLLECT_LETTUCE_SCRIPT_6,
		COLLECT_LETTUCE_SCRIPT_7,

		// 상추 퀘스트 완료 스크립트
		COLLECT_LETTUCE_SCRIPT_CLEAR,
		COLLECT_LETTUCE_SCRIPT_CLEAR_END,

		// 하이에나 패거리 퀘스트 스크립트
		KILL_GANG_SWARM_SCRIPT_1,
		KILL_GANG_SWARM_SCRIPT_2,
		KILL_GANG_SWARM_SCRIPT_3,
		KILL_GANG_SWARM_SCRIPT_4,
		KILL_GANG_SWARM_SCRIPT_5,
		KILL_GANG_SWARM_SCRIPT_6,
		KILL_GANG_SWARM_SCRIPT_7,

		// 로벨라 퀘스트 완료 스크립트
		KILL_ROBELLA_SCRIPT_CLEAR,
		KILL_ROBELLA_SCRIPT_CLEAR_END,

		// 요리 퀘스트 스크립트
		MAKE_COOKING_SCRIPT_1,
		MAKE_COOKING_SCRIPT_2,
		MAKE_COOKING_SCRIPT_3,
		MAKE_COOKING_SCRIPT_4,
		MAKE_COOKING_SCRIPT_5,

		// 요리 퀘스트 완료 스크립트
		MAKE_COOKING_SCRIPT_CLEAR,
		MAKE_COOKING_SCRIPT_CLEAR_END,

		// 뽑기 퀘스트 스크립트
		ORDER_SCRIPT_1,
		ORDER_SCRIPT_2,
		ORDER_SCRIPT_3,
		ORDER_SCRIPT_4,
		ORDER_SCRIPT_5,

		// 뽑기 퀘스트 완료 스크립트
		ORDER_SCRIPT_CLEAR,
		ORDER_SCRIPT_CLEAR_END,

		// 도시지역 정찰 퀘스트 스크립트
		KILL_MECHANICALCITY_SWARM_1,
		KILL_MECHANICALCITY_SWARM_2,
		KILL_MECHANICALCITY_SWARM_3,
		KILL_MECHANICALCITY_SWARM_4,
		KILL_MECHANICALCITY_SWARM_5,

		// 점핑 프로스트 드래곤 퀘스트 완료 스크립트
		KILL_FROST_DRAGON_SCRIPT_CLEAR,
		KILL_FROST_DRAGON_SCRIPT_CLEAR_END,

		// 요리 퀘스트 스크립트
		MAKE_COOKING_SCRIPT1_1,
		MAKE_COOKING_SCRIPT1_2,
		MAKE_COOKING_SCRIPT1_3,
		MAKE_COOKING_SCRIPT1_4,
		MAKE_COOKING_SCRIPT1_5,

		// 요리 퀘스트 완료 스크립트
		MAKE_COOKING_SCRIPT1_CLEAR,
		MAKE_COOKING_SCRIPT1_CLEAR_END,

		// 돌연변이 처치 퀘스트 스크립트
		KILL_MUTATION_SCRIPT_1,
		KILL_MUTATION_SCRIPT_2,
		KILL_MUTATION_SCRIPT_3,
		KILL_MUTATION_SCRIPT_4,
		KILL_MUTATION_SCRIPT_5,
		KILL_MUTATION_SCRIPT_6,
		KILL_MUTATION_SCRIPT_7,
		KILL_MUTATION_SCRIPT_8,
		KILL_MUTATION_SCRIPT_9,

		// 웨버 퀘스트 완료 스크립트
		KILL_WEAVER_SCRIPT_CLEAR,
		KILL_WEAVER_SCRIPT_CLEAR_END,

		SCRIPT_END
	};

public:
	CNpc_Shirley();
	CNpc_Shirley(const CNpc& rhs);
	virtual ~CNpc_Shirley() = default;

public:
	_uint Get_ScriptPhase() { return m_iScriptPhase; }
	_bool Get_Script() { return m_bScript; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void On_EventMessege(EVENTDESC tEvent) override;


private:
	_uint	m_iScriptPhase = 0;
	_bool	m_bScript = false;
	_bool	m_bNonScript = false;
	_bool	m_bQuestClear = false;
	_bool	m_bQuestPlay = false;

	_uint	m_iQuestEvent = 0;

	_uint	m_iBlackCoreNum = 0;

	_uint	m_iDead_Normal = 0;
	_uint	m_iDead_Eliete= 0;
	_uint	m_iDead_Boss = 0;

	_bool	m_bShow_UI_Complete = true;
	class CUI_Quest_Complete* m_pUI_Complete[2];

	DECL_ONCE(m_bEffect0);

private:
	void	Quest();
	void	Non_Script_Quest();
	void	Script();
	void	Collider_ToPlayer();
	void	Create_UI_Alert();

private:
	void	Pooling_UI_PlaceMark();
	void	Able_UI_PlaceMark();
	void	Show_UI_Complete();
	void	Hide_UI_Complete();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_State_Handler();

public:
	static CNpc_Shirley* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END