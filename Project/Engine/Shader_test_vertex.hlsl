#include "Shader_global_header.hlsli"
#include "Shader_test_header.hlsli"

// vertex shader
// LocalSpace ��ü�� NDC ��ǥ��� �̵�
VS_OUT test_VS(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // �Է����� ���� ������ǥ�� ������� ���� ���ؼ� ���
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vOutColor = _in.vColor;
    output.vOutUV = _in.vUV;
    
    return output;
}