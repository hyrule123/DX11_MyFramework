#include "Shader_Tilemap_0_header.hlsli"


// pixel shader
float4 PS_Tilemap(VTX_TILEMAP_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // �Ҽ���Ʈ, frac(_in.vUV) : Ÿ�� ��ĭ ������ �ȼ��� ������� ��ġ (0 ~ 1)
    // ������Ʈ, floor(_in.vUV): ��ü Ÿ�� �߿��� �ε���(��, ��)    
    int2 TileIdx = floor(_in.vUV);
    int BufferIdx = g_int_0 * TileIdx.y + TileIdx.x;
    float2 vUV = g_TileSBuffer[BufferIdx].vLeftTop + (g_TileSBuffer[BufferIdx].vSlice * frac(_in.vUV));
    
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_1, vUV);
    }
    
    return vOutColor;
}