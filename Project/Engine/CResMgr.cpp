#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"

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
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), RES_TYPE::MESH));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), RES_TYPE::MESHDATA));
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMaterial)), RES_TYPE::MATERIAL));
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CTexture)), RES_TYPE::TEXTURE));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CSound)), RES_TYPE::SOUND));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CPrefab)), RES_TYPE::PREFAB));
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CGraphicsShader)), RES_TYPE::GRAPHICS_SHADER));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CComputeShader)), RES_TYPE::COMPUTE_SHADER));
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
	for (int i = 0; i < fslice - 1; ++i)
	{
		v.vPos.x = radius * cosf(AngleStride * i);
		v.vPos.y = radius * sinf(AngleStride * i);
		v.vUV.x = v.vPos.x + 0.5f;
		v.vUV.y = -(v.vPos.y) + 0.5f;	//�ݴ� ����

		vecVtx.push_back(v);

		//�����迭�� ù��°�� �߽����� �� �����Ƿ� i + 1 == ���� ���������� ����� �ȴ�
		//�ε����� �ð� �������� ����
		//������ �ε����� ���� ���� ����
		if (i == (fslice - 2))
			break;

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
	Ptr<CGraphicsShader> pShader = nullptr;

	// ===========
	// Test Shader
	// ===========
	pShader = new CGraphicsShader;
	pShader->SetKey(L"TestShader");
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_Test");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_Test");

	AddRes(L"TestShader", pShader);
}

void CResMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Test Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TestShader"));
	AddRes(L"TestMtrl", pMtrl);
}

void CResMgr::LoadDefaultTexture()
{
	Ptr<CTexture> pTexture = Load<CTexture>(L"PlayerTex", L"texture\\Fighter.bmp");	
}