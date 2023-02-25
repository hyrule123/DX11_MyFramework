#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"


#include "shaders.h"
#include "strKeys.h"

#include "CCS_SetColor.h"
#include "CCS_ParticleUpdate.h"

#include "CAnim2DAtlas.h"


CResMgr::CResMgr()
	: m_bResUpdated(true)
{
}

CResMgr::~CResMgr()
{
}



void CResMgr::init()
{
	CreateResClassTypeIndex();

	CreateDefaultMesh();
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();

	LoadDefaultTexture();

	CreateDefaultAnimAtlas();
}

void CResMgr::CreateResClassTypeIndex()
{
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), eRES_TYPE::MESH));
	//m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), eRES_TYPE::MESHDATA));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMaterial)), eRES_TYPE::MATERIAL));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CTexture)), eRES_TYPE::TEXTURE));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CAnim2DAtlas)), eRES_TYPE::ANIM2D_SPRITE));
	//m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CSound)), eRES_TYPE::SOUND));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CPrefab)), eRES_TYPE::PREFAB));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CGraphicsShader)), eRES_TYPE::GRAPHICS_SHADER));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CComputeShader)), eRES_TYPE::COMPUTE_SHADER));
}

void CResMgr::CreateDefaultMesh()
{	
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;
	Ptr<CMesh> pMesh = nullptr;


	// ==============
	// PointMesh 생성
	// ==============
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.f, 0.f);
	//v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	
	UINT idx = 0;

	pMesh = new CMesh;
	pMesh->Create(&v, 1, &idx, 1);
	AddRes(RESOURCE::MESH::POINT, pMesh);
	pMesh = nullptr;


	// =============
	// RectMesh 생성
	// =============	
	// 0 --- 1 
	// |  \  |
	// 3 --- 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.0f);
	//v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.0f);
	//v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.0f);
	//v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.0f);
	//v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(RESOURCE::MESH::RECT, pMesh);
	pMesh = nullptr;

	//Debug Rect Mesh
	vecIdx.clear();
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(RESOURCE::MESH::DEBUG_RECT, pMesh);
	//============================


	//===================
	//CircleMesh
	//===================
	pMesh = nullptr;

	//정점버퍼와 인덱스버퍼 초기화
	vecVtx.clear();
	vecIdx.clear();

	//반지름과 조각별 각도를 정하고 각도를 계산.
	float radius = 0.5f;
	int fslice = 40;
	float AngleStride = XM_2PI / fslice;

	//먼저 원의 중심점을 정점버퍼에 먼저 추가(중심점 (0.5f, 0.5f)에 반지름이 0.5f인 원을 추가할 예정)
	v.vPos = Vec3(0.f, 0.f, 0.f);
	//v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	//반복 수 ' - 1 '만큼 반복(마지막 정점은 두 번째 정점을 사용하면 된다.

	//원의 Pos값(정점의 위치)과 UV값은 1:1 대응이 되지 않음
	//y축은 서로 방향이 반대이고 중심점 좌표도 차이가 남.
	/*					<POS>
					(0.f, 0.5f)
				/					\
	(-0.5f, 0.f)	(0.f, 0.f)		(0.5f, 0.f)
				\					/
					(0.f, -0.5f)

						<UV>
					(0.5f, 0.f)
				/					\
	(0.f, 0.5f)		(0.5f, 0.5f)	(1.f, 0.5f)
				\					/
					(0.5f, 1.f)
	*/

	//오른쪽 끝부터 피자조각 모양으로 정점버퍼를 추가한다.
	//마지막 전까지 정점을 계산해서 정점버퍼에 삽입한다.
	//배열의 0번 인덱스에는 중심점이 들어가 있으므로 1부터 시작하면 i를 인덱스버퍼의 인덱스로 사용 가능
	for (int i = 0; i < fslice; ++i)
	{
		v.vPos.x = radius * cosf(AngleStride * i);
		v.vPos.y = radius * sinf(AngleStride * i);
		
		//UV는 중점 기준으로 더하거나 빼는 방식으로 해준다.
		v.vUV.x = 0.5f + v.vPos.x;
		v.vUV.y = 0.5f -(v.vPos.y);	//반대 방향

		vecVtx.push_back(v);

		//정점배열의 첫번째는 중심점이 들어가 있으므로 i + 1 == 현재 정점버퍼의 사이즈가 된다
		//인덱스는 시계 방향으로 삽입
		//마지막 인덱스는 따로 직접 삽입
		if (i == (fslice - 1))
			continue;

		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back((UINT)vecVtx.size() - 1u);
	

	//만든 정점버퍼와 인덱스버퍼를 메쉬에 삽입한다.
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(RESOURCE::MESH::CIRCLE, pMesh);
	pMesh = nullptr;


	//디버그 메쉬
	vecIdx.clear();
	for (int i = 0; i < fslice; ++i)
	{
		vecIdx.push_back(i + 1);
	}
	vecIdx.push_back(1);
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(RESOURCE::MESH::DEBUG_CIRCLE, pMesh);
}

void CResMgr::CreateDefaultGraphicsShader()
{
	// ============
	// Debug Shader
	// Topology: LineStrip
	// Rasterizer: No Culling
	// Blend State: Default
	// Shader Domain: Opaque
	// ============
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;

		pShader->SetKey(RESOURCE::SHADER::DEBUG);
		pShader->CreateShader((void*)g_VS_Debug, sizeof(g_VS_Debug), eSHADER_TYPE::__VERTEX);
		pShader->CreateShader((void*)g_PS_Debug, sizeof(g_PS_Debug), eSHADER_TYPE::__PIXEL);
		pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_NONE);
		pShader->SetDepthStencilState(eDEPTHSTENCIL_TYPE::NO_TEST_NO_WRITE);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	}

	// ===========
	// Test Shader
	// =========== 
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey(RESOURCE::SHADER::TEST);
		pShader->CreateShader((void*)g_VS_test, sizeof(g_VS_test), eSHADER_TYPE::__VERTEX);
		pShader->CreateShader((void*)g_PS_test, sizeof(g_PS_test), eSHADER_TYPE::__PIXEL);
		pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
		pShader->SetBlendState(eBLENDSTATE_TYPE::DEFAULT);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
		AddRes(pShader->GetKey(), pShader);
	}

	// ============
	// std2D Shader
	// ============
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey(RESOURCE::SHADER::STD2D);
		pShader->CreateShader((void*)g_VS_std2D, sizeof(g_VS_std2D), eSHADER_TYPE::__VERTEX);
		pShader->CreateShader((void*)g_PS_std2D, sizeof(g_PS_std2D), eSHADER_TYPE::__PIXEL);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
		AddRes(pShader->GetKey(), pShader);
	}


	// ==================
	// std2D_Light Shader
	// ==================
	// 광원을 처리할 수 있는 2D 쉐이더
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey(RESOURCE::SHADER::STD2D_LIGHT);
		pShader->CreateShader((void*)g_VS_std2D_Light, sizeof(g_VS_std2D_Light), eSHADER_TYPE::__VERTEX);
		pShader->CreateShader((void*)g_PS_std2D_Light, sizeof(g_PS_std2D_Light), eSHADER_TYPE::__PIXEL);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
		AddRes(pShader->GetKey(), pShader);
	}



	// ===============================
	// Tilemap
	// RS_TYPE : CULL_BACK
	// DS_TYPE : LESS(Default)
	// BS_TYPE : MASK

	// Parameter
	// g_CBuffer_Mtrl_Scalar.INT_0 : Tile X Count
	// g_CBuffer_Mtrl_Scalar.int_1 : Tile Y Count
	// g_tex_0 : Tile Atlas Texture
	//===============================
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey(RESOURCE::SHADER::TILEMAP);
		pShader->CreateShader((void*)g_VS_Tilemap, sizeof(g_VS_Tilemap), eSHADER_TYPE::__VERTEX);
		pShader->CreateShader((void*)g_PS_Tilemap, sizeof(g_PS_Tilemap), eSHADER_TYPE::__PIXEL);
		pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
		pShader->SetBlendState(eBLENDSTATE_TYPE::MASK);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_MASK);
		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	}


	// ============================
	// ParticleRender
	// 
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_WRITE
	// BS_TYPE : ALPHA_BLEND

	// Parameter
	// g_CBuffer_Mtrl_Scalar.INT_0 : Particle Index
	// 
	// Domain : TRANSPARENT
	// ============================
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey(RESOURCE::SHADER::PARTICLE_RENDER);
	
		pShader->CreateShader((void*)g_VS_Particle, sizeof(g_VS_Particle), eSHADER_TYPE::__VERTEX);
		pShader->CreateShader((void*)g_GS_Particle, sizeof(g_GS_Particle), eSHADER_TYPE::__GEOMETRY);
		pShader->CreateShader((void*)g_PS_Particle, sizeof(g_PS_Particle), eSHADER_TYPE::__PIXEL);

		pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
		pShader->SetDepthStencilState(eDEPTHSTENCIL_TYPE::NO_WRITE);
		pShader->SetBlendState(eBLENDSTATE_TYPE::ALPHA_BLEND);
		pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_TRANSPARENT); //알파 블렌딩을 사용하므로


		AddRes(pShader->GetKey(), pShader);
	}
}


void CResMgr::CreateDefaultComputeShader()
{
	Ptr<CComputeShader> pShader = new CCS_SetColor(32u, 32u, 1u);
	pShader->CreateShader((void*)g_CS_SetColor, sizeof(g_CS_SetColor));
	pShader->SetKey(RESOURCE::SHADER::COMPUTE::SET_COLOR);
	AddRes(pShader->GetKey(), pShader);
	pShader = nullptr;

	//기본 파티클 쉐이더
	pShader = new CCS_ParticleUpdate(128u, 1u, 1u);
	pShader->CreateShader((void*)g_CS_Particle_Basic, sizeof(g_CS_Particle_Basic));
	pShader->SetKey(RESOURCE::SHADER::COMPUTE::PARTICLE_UPDATE_BASIC);
	AddRes(pShader->GetKey(), pShader);
	pShader = nullptr;

	//비 효과 파티클 쉐이더
	pShader = new CCS_ParticleUpdate(128u, 1u, 1u);
	pShader->CreateShader((void*)g_CS_Particle_RainDrop, sizeof(g_CS_Particle_RainDrop));
	pShader->SetKey(RESOURCE::SHADER::COMPUTE::PARTICLE_UPDATE_RAINDROP);
	AddRes(pShader->GetKey(), pShader);
	pShader = nullptr;
}


void CResMgr::CreateDefaultMaterial()
{
	//Debug Material
	{
		//Rectangle
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetShader(FindRes<CGraphicsShader>(RESOURCE::SHADER::DEBUG));
		pMtrl->SetInstancedRender(true);

		pMtrl->SetKey(RESOURCE::MATERIAL::DEBUG_RECT);
		AddRes(pMtrl->GetKey(), pMtrl);

		//Circle
		pMtrl = new CMaterial;
		pMtrl->SetShader(FindRes<CGraphicsShader>(RESOURCE::SHADER::DEBUG));
		pMtrl->SetInstancedRender(true);

		pMtrl->SetKey(RESOURCE::MATERIAL::DEBUG_CIRCLE);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// Test Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetShader(FindRes<CGraphicsShader>(RESOURCE::SHADER::TEST));
		pMtrl->SetInstancedRender(true);

		pMtrl->SetKey(RESOURCE::MATERIAL::TEST);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// std2D Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(RESOURCE::SHADER::STD2D));
		pMtrl->SetInstancedRender(true);

		pMtrl->SetKey(RESOURCE::MATERIAL::STD2D);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// std2DLight Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial();
		//pMtrl->SetInstancedRender(true);
		pMtrl->SetShader(FindRes<CGraphicsShader>(RESOURCE::SHADER::STD2D_LIGHT));

		pMtrl->SetKey(RESOURCE::MATERIAL::STD2D_LIGHT);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// Tilemap Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(RESOURCE::SHADER::TILEMAP));

		pMtrl->SetKey(RESOURCE::MATERIAL::TILEMAP);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	//Particle Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(RESOURCE::SHADER::PARTICLE_RENDER));
		pMtrl->SetKey(RESOURCE::MATERIAL::PARTICLE_RENDER);
		AddRes(pMtrl->GetKey(), pMtrl);
	}



	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial();
		pMtrl->SetInstancedRender(true);
		pMtrl->SetShader(FindRes<CGraphicsShader>(RESOURCE::SHADER::STD2D));

		pMtrl->SetKey(RESOURCE::MATERIAL::CORSAIR);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial();
		pMtrl->SetInstancedRender(true);
		pMtrl->SetShader(FindRes<CGraphicsShader>(RESOURCE::SHADER::STD2D));

		pMtrl->SetKey(RESOURCE::MATERIAL::MARINE);
		AddRes(pMtrl->GetKey(), pMtrl);
	}
}

void CResMgr::LoadDefaultTexture()
{
	Load<CTexture>(RESOURCE::TEXTURE::FIGHTER, L"texture/Fighter.bmp");
	Load<CTexture>(RESOURCE::TEXTURE::HOS, L"texture/HOS.png");

	Load<CTexture>(RESOURCE::TEXTURE::BEHEADED_ATLAS, L"texture/beheaded.png");
	Load<CTexture>(RESOURCE::TEXTURE::BEHEADED_ATLAS_NORMAL, L"texture/beheaded_n.png");

	Load<CTexture>(RESOURCE::TEXTURE::TILE_ATLAS, L"texture/TILE.bmp");
	Load<CTexture>(RESOURCE::TEXTURE::LINK_ATLAS, L"texture/link.png");

	Load<CTexture>(RESOURCE::TEXTURE::REAVER_ATLAS, L"texture/trilob(Reaver).bmp");


	Load<CTexture>(RESOURCE::TEXTURE::CORSAIR_ATLAS, L"texture/corsair.bmp");
	Load<CTexture>(RESOURCE::TEXTURE::MARINE_ATLAS, L"texture/marine.bmp");


	//노이즈텍스처
	Load<CTexture>(RESOURCE::TEXTURE::NOISE_TEXTURE_0, L"texture/noise/noise_01.png");
}

void CResMgr::CreateDefaultAnimAtlas()
{
	{
		Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
		Atlas->SetAtlasTexture(FindRes<CTexture>(RESOURCE::TEXTURE::REAVER_ATLAS));

		Atlas->SetNewAnimUV_SC_Redundant(9u, 0u, 9u);
		Atlas->AddAnim2D_SC_Redundant(RESOURCE::ANIM2D::REAVERMOVE, 0u, 9u, 3.f);

		AddRes<CAnim2DAtlas>(RESOURCE::TEXTURE::REAVER_ATLAS, Atlas);
	}

	{
		Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
		Atlas->SetAtlasTexture(FindRes<CTexture>(RESOURCE::TEXTURE::CORSAIR_ATLAS));

		Atlas->SetNewAnimUV(17u, 5u, 0u, 17u, 0u, 5u);
		Atlas->AddAnim2D(RESOURCE::ANIM2D::CORSAIRMOVE, 0u, 17u, 0u, 5u, 0.3f, eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP);

		AddRes<CAnim2DAtlas>(RESOURCE::TEXTURE::CORSAIR_ATLAS, Atlas);
	}

	{
		Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
		Atlas->SetAtlasTexture(FindRes<CTexture>(RESOURCE::TEXTURE::MARINE_ATLAS));

		Atlas->SetNewAnimUV_SC_Redundant(14u, 0u, 14u);
		Atlas->AddAnim2D_SC_Redundant(RESOURCE::ANIM2D::MARINE_IDLE, 0u, 1u, 1.f);
		Atlas->AddAnim2D_SC_Redundant(RESOURCE::ANIM2D::MARINE_ATTACKSTART, 1u, 2u, 0.5f);

		Atlas->AddAnim2D_SC_Redundant(RESOURCE::ANIM2D::MARINE_MOVE, 4u, 9u, 1.f);

		Atlas->AddAnim2D_SC_Redundant(RESOURCE::ANIM2D::MARINE_ATTACK, 2u, 2u, 0.1f);


		AddRes<CAnim2DAtlas>(RESOURCE::TEXTURE::MARINE_ATLAS, Atlas);
	}
}


