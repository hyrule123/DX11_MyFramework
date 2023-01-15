#pragma once

class CCollider;

class CSpatialGrid
{
public:
    CSpatialGrid();
    ~CSpatialGrid();

private:
    //�ڽ��� ��ġ�� ���� �ִ� �浹ü�� �ּҵ�. �� tick���� ���� ���ŵ�.
    vector<CCollider*> m_vecpColliderInside;

public:
    void AddCollider(CCollider* _pCol);
    void ClearVecCollider();
    const vector<CCollider*>& GetVecCollider() const { return m_vecpColliderInside; }

    //ID ������ �������� ����
    void SortVecCollider();

    
};

inline void CSpatialGrid::AddCollider(CCollider* _pCol)
{
    m_vecpColliderInside.push_back(_pCol);
}

inline void CSpatialGrid::ClearVecCollider()
{
    m_vecpColliderInside.clear();
}




