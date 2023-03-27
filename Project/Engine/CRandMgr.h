#pragma once

#include "CSingleton.h"

#include <random>

class CRandMgr :
    public CSingleton<CRandMgr>
{
    SINGLETON(CRandMgr);

private:
    std::mt19937 m_RandEngine;

public:
    template <typename T>
    T GetRand(T _tMin, T _tMax)
    {
        static_assert(std::is_arithmetic<T>::value, "Variable Type is not arithmetic!!");

        //들어온 T값이 int일 경우: int 타입 분포를 사용
        //들어온 T값이 float일 경우: float 타입 분포를 사용
        using Distribution = typename std::conditional<std::is_floating_point<T>::value,
            std::uniform_real_distribution<float>,
            std::uniform_int_distribution<int>>::type;

        Distribution dist(_tMin, _tMax);

        return static_cast<T>(dist(m_RandEngine));
    }
};

