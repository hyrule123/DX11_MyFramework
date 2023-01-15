#pragma once

class CCollider;

class CSpatialGrid
{
public:
    CSpatialGrid();
    ~CSpatialGrid();

private:
    //자신의 위치에 겹쳐 있는 충돌체의 주소들. 매 tick마다 새로 갱신됨.
    vector<CCollider*> m_vecpColliderInside;

public:
    void AddCollider(CCollider* _pCol);
    void ClearVecCollider();
    const vector<CCollider*>& GetVecCollider() const { return m_vecpColliderInside; }

    //ID 순으로 오름차순 정렬
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




