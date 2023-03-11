#include "S_0_H_Particle.hlsli"

//그래도 자신의 구조체를 리턴(나머지는 지오메트리 쉐이더에서 처리)
VS_INOUT VS_Particle(VS_INOUT _in)
{ 
    return _in;
}