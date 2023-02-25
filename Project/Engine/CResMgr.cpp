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
	// PointMesh ����
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
	// RectMesh ����
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

	//�������ۿ� �ε������� �ʱ�ȭ
	vecVtx.clear();
	vecIdx.clear();

	//�������� ������ ������ ���ϰ� ������ ���.
	float radius = 0.5f;
	int fslice = 40;
	float AngleStride = XM_2PI / fslice;

	//���� ���� �߽����� �������ۿ� ���� �߰�(�߽��� (0.5f, 0.5f)�� �������� 0.5f�� ���� �߰��� ����)
	v.vPos = Vec3(0.f, 0.f, 0.f);
	//v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	//�ݺ� �� ' - 1 '��ŭ �ݺ�(������ ������ �� ��° ������ ����ϸ� �ȴ�.

	//���� Pos��(������ ��ġ)�� UV���� 1:1 ������ ���� ����
	//y���� ���� ������ �ݴ��̰� �߽��� ��ǥ�� ���̰� ��.
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

	//������ ������ �������� ������� �������۸� �߰��Ѵ�.
	//������ ������ ������ ����ؼ� �������ۿ� �����Ѵ�.
	//�迭�� 0�� �ε������� �߽����� �� �����Ƿ� 1���� �����ϸ� i�� �ε��������� �ε����� ��� ����
	for (int i = 0; i < fslice; ++i)
	{
		v.vPos.x = radius * cosf(AngleStride * i);
		v.vPos.y = radius * sinf(AngleStride * i);
		
		//UV�� ���� �������� ���ϰų� ���� ������� ���ش�.
		v.vUV.x = 0.5f + v.vPos.x;
		v.vUV.y = 0.5f -(v.vPos.y);	//�ݴ� ����

		vecVtx.push_back(v);

		//�����迭�� ù��°�� �߽����� �� �����Ƿ� i + 1 == ���� ���������� ����� �ȴ�
		//�ε����� �ð� �������� ����
		//������ �ε����� ���� ���� ����
		if (i == (fslice - 1))
			continue;

		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back((UINT)vecVtx.size() - 1u);
	

	//���� �������ۿ� �ε������۸� �޽��� �����Ѵ�.
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(RESOURCE::MESH::CIRCLE, pMesh);
	pMesh = nullptr;


	//����� �޽�
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
	// ������ ó���� �� �ִ� 2D ���̴�
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
		pShader->SetShaderDomain(eSHADER_DOMAIN::_TRANSPARENT); //���� ������ ����ϹǷ�


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

	//�⺻ ��ƼŬ ���̴�
	pShader = new CCS_ParticleUpdate(128u, 1u, 1u);
	pShader->CreateShader((void*)g_CS_Particle_Basic, sizeof(g_CS_Particle_Basic));
	pShader->SetKey(RESOURCE::SHADER::COMPUTE::PARTICLE_UPDATE_BASIC);
	AddRes(pShader->GetKey(), pShader);
	pShader = nullptr;

	//�� ȿ�� ��ƼŬ ���̴�
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


	//�������ؽ�ó
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


