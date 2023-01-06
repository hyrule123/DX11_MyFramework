#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"


//Shader Header
#ifdef _DEBUG

#include "CompiledShaderHeader/Shader_test_vertex_Debug.h"
#include "CompiledShaderHeader/Shader_test_pixel_Debug.h"

#include "CompiledShaderHeader/Shader_std2D_vertex_Debug.h"
#include "CompiledShaderHeader/Shader_std2D_pixel_Debug.h"

#else

#include "CompiledShaderHeader/Shader_test_vertex.h"
#include "CompiledShaderHeader/Shader_test_pixel.h"

#include "CompiledShaderHeader/Shader_std2D_vertex.h"
#include "CompiledShaderHeader/Shader_std2D_pixel.h"

#endif

CResMgr::CResMgr()
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
	CreateDefaultMaterial();

	LoadDefaultTexture();
}

void CResMgr::CreateResClassTypeIndex()
{
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), eRES_TYPE::MESH));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), eRES_TYPE::MESHDATA));
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMaterial)), eRES_TYPE::MATERIAL));
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CTexture)), eRES_TYPE::TEXTURE));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CSound)), eRES_TYPE::SOUND));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CPrefab)), eRES_TYPE::PREFAB));
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CGraphicsShader)), eRES_TYPE::GRAPHICS_SHADER));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CComputeShader)), eRES_TYPE::COMPUTE_SHADER));
}

void CResMgr::CreateDefaultMesh()
{	
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;


	Ptr<CMesh> pMesh = nullptr;
	// =============
	// RectMesh ����
	// =============	
	// 0 --- 1 
	// |  \  |
	// 3 --- 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.0f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.0f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.0f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.0f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
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
	AddRes(L"RectMesh", pMesh);
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
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
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
		v.vUV.x = v.vPos.x + 0.5f;
		v.vUV.y = -(v.vPos.y) + 0.5f;	//�ݴ� ����

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
	AddRes(L"CircleMesh", pMesh);

}

void CResMgr::CreateDefaultGraphicsShader()
{
	// ===========
	// Test Shader
	// =========== 
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey(L"TestShader");
		pShader->CreateShader((void*)g_VS_test, sizeof(g_VS_test), eSHADERTYPE_VERTEX);
		pShader->CreateShader((void*)g_PS_test, sizeof(g_PS_test), eSHADERTYPE_PIXEL);
		pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_NONE);
		AddRes(L"TestShader", pShader);
	}

	// ===========
	// std2D Shader
	// ===========
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey(L"std2DShader");
		pShader->CreateShader((void*)g_VS_std2D, sizeof(g_VS_std2D), eSHADERTYPE_VERTEX);
		pShader->CreateShader((void*)g_PS_std2D, sizeof(g_PS_std2D), eSHADERTYPE_PIXEL);
		AddRes(pShader->GetKey(), pShader);

		pShader->Set
	}

}

void CResMgr::CreateDefaultMaterial()
{
	// Test Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial;
		pMtrl->SetKey(L"TestMtrl");
		pMtrl->SetShader(FindRes<CGraphicsShader>(L"TestShader"));
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// std2D Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial;
		pMtrl->SetKey(L"std2DMtrl");
		pMtrl->SetShader(FindRes<CGraphicsShader>(L"std2DShader"));
		AddRes(pMtrl->GetKey(), pMtrl);
	}
}

void CResMgr::LoadDefaultTexture()
{
	Ptr<CTexture> pTexture = Load<CTexture>(L"PlayerTex", L"texture\\Fighter.bmp");	
}