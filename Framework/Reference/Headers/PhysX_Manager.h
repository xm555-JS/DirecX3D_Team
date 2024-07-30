#pragma once
#include "Base.h"

BEGIN(Engine)

class CPhysX_Manager final : public CBase
{
	DECLARE_SINGLETON(CPhysX_Manager)

public:
	//enum PxShape
	//{
	//	SPHERE,
	//	BOX,


	//	SHAPE_END
	//}PXSHAPE;

	enum Scene
	{
		SCENE_CURRENT,
		SCENE_NEXT,
		SCENE_BEFORE,
		
		SCENE_END
	};

	typedef enum tagColliderShape
	{
		SHAPE_CHEESE,

		SHAPE_END
	}COLSHAPE;

public:
	CPhysX_Manager();
	virtual ~CPhysX_Manager() = default;

public:
	PxConvexMesh*	Get_ShapeTemplate(COLSHAPE eShape) { m_ShapeTemplate[eShape]; }
	void			Set_ShapeSphere(PxSphereGeometry* pGeometry) { m_pSphere = pGeometry; }
	void			Set_ShapeBox(PxBoxGeometry* pGeometry) { m_pBox = pGeometry; }

	void			Begin_PhysScene() { m_bSceneStart = true; }
	void			End_PhysScene() { m_bSceneStart = false; }

public:
	HRESULT	Initialize();
	void	Tick(_float fTimeDelta);

public:
	// Transform ���� (float3)��ġ�� (float4)���ʹϿ��� ����.
	HRESULT			Create_Scene(Scene eScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));
	HRESULT			Delete_Scene(Scene eScene);
	HRESULT			Change_Scene(Scene eNextScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));

	PxRigidDynamic* Create_DynamicActor(const PxTransform& t, const PxGeometry& geometry, Scene eScene, const PxReal& Density, const PxVec3& velocity = PxVec3(0), PxMaterial* pMaterial = nullptr);
	PxRigidStatic*	Create_StaticActor(const PxTransform& t, const PxGeometry& geometry, Scene eScene, PxMaterial* pMaterial = nullptr);
	
	void			Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh** ppOut);
	void			Create_ConvexMesh(PxVec3** pVertices, _uint iNumVertice, PxConvexMesh** ppOut);
	void			Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial** ppOut);
	void			Create_Shape(const PxGeometry& Geometry, PxMaterial* pMaterial, PxShape** ppOut);
	void			Create_TriAngleMesh(const PxVec3** pVertices, _uint iNumVertice, const PxU32** pIndices, _uint iNumIndice, PxTriangleMesh** ppOut);


private:
	// Foundation�� �����ϴµ� �ʿ��� ����
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;
	PxFoundation*			m_pFoundation;

	// PxFoundation�� �־�� Physics�� ������ �� �ִ�.
	PxPhysics*				m_pPhysics;

	// CPU ���ҽ��� ȿ�������� ������ �� �ֵ��� �ϱ� ���� �����ϴ� ���� ��õ
	PxDefaultCpuDispatcher*	m_pDispatcher;

	// MeshCooking�� �ϱ� ���� ����
	PxCooking*				m_pCooking = nullptr;


	// Scene
	//_uint					m_iNumScenes = 0;
	//typedef PxScene*		LPSCENE;
	PxScene*				m_pScenes[SCENE_END];
	PxScene*				m_pCurScene = nullptr;

	// m_pPhysics�� ����� createMaterial�ؼ� ���
	// �浹ü ������, Dynamic ������, ź������ �����Ͽ� ���
	PxMaterial*				m_pMaterial;

	////Visual Debugger
	PxPvd*					m_pPVD;

	PxRigidDynamic*			m_pTemp = nullptr;


private:
	PxBoxGeometry*			m_pBox = nullptr;
	PxSphereGeometry*		m_pSphere = nullptr;

	PxConvexMesh*			m_ShapeTemplate[SHAPE_END];
	_bool					m_bSceneStart = false;

private:
	//void			Create_CheeseShape();
	//void			Create_Stack(const PxTransform& t, PxU32 size, PxReal halfExtent);



public:
	virtual void Free() override;
};

END