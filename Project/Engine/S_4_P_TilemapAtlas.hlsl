#include "S_H_TilemapAtlas.hlsli"


// pixel shader
float4 PS_Tilemap_Atlas(VTX_TILEMAP_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // 소수파트, frac(_in.vUV) : 타일 한칸 내에서 픽셀의 상대적인 위치 (0 ~ 1)
    // 정수파트, floor(_in.vUV): 전체 타일 중에서 인덱스(행, 열)    
    int2 TileIdx = floor(_in.vUV);
    int BufferIdx = g_CBuffer_Mtrl_Scalar.INT_0 * TileIdx.y + TileIdx.x;
    float2 vUV = g_SBuffer_Tile[BufferIdx].vLeftTop + (g_SBuffer_Tile[BufferIdx].vSlice * frac(_in.vUV));
    
	if (g_CBuffer_Mtrl_Tex.bTEX_0)
    {
        vOutColor = g_tex_0.Sample(g_Sampler_1, vUV);
    }
    
    return vOutColor;
}
