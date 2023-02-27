#pragma once

//Shader Header
#ifdef _DEBUG

#include "CompiledShaderHeader/Shader_Debug_1_Vertex_Debug.h"
#include "CompiledShaderHeader/Shader_Debug_2_Pixel_Debug.h"

#include "CompiledShaderHeader/Shader_Test_1_Vertex_Debug.h"
#include "CompiledShaderHeader/Shader_Test_2_Pixel_Debug.h"

#include "CompiledShaderHeader/Shader_STD2D_1_Vertex_Debug.h"
#include "CompiledShaderHeader/Shader_STD2D_2_Pixel_Debug.h"

#include "CompiledShaderHeader/Shader_STD2D_Light_1_Vertex_Debug.h"
#include "CompiledShaderHeader/Shader_STD2D_Light_2_Pixel_Debug.h"

#include "CompiledShaderHeader/Shader_Tilemap_1_Vertex_Debug.h"
#include "CompiledShaderHeader/Shader_Tilemap_2_Pixel_Debug.h"

#include "CompiledShaderHeader/Shader_SetColor_Compute_Debug.h"

#include "CompiledShaderHeader/Shader_Particle_Basic_Compute_Debug.h"
#include "CompiledShaderHeader/Shader_Particle_RainDrop_Compute_Debug.h"
#include "CompiledShaderHeader/Shader_SCMapLoader_Compute_Debug.h"

#include "CompiledShaderHeader/Shader_Particle_1_Vertex_Debug.h"
#include "CompiledShaderHeader/Shader_Particle_2_Geometry_Debug.h"
#include "CompiledShaderHeader/Shader_Particle_3_Pixel_Debug.h"


#else

#include "CompiledShaderHeader/Shader_Debug_1_Vertex.h"
#include "CompiledShaderHeader/Shader_Debug_2_Pixel.h"

#include "CompiledShaderHeader/Shader_Test_1_Vertex.h"
#include "CompiledShaderHeader/Shader_Test_2_Pixel.h"

#include "CompiledShaderHeader/Shader_STD2D_1_Vertex.h"
#include "CompiledShaderHeader/Shader_STD2D_2_Pixel.h"

#include "CompiledShaderHeader/Shader_STD2D_Light_1_Vertex.h"
#include "CompiledShaderHeader/Shader_STD2D_Light_2_Pixel.h"

#include "CompiledShaderHeader/Shader_Tilemap_1_Vertex.h"
#include "CompiledShaderHeader/Shader_Tilemap_2_Pixel.h"

#include "CompiledShaderHeader/Shader_SetColor_Compute.h"

#include "CompiledShaderHeader/Shader_Particle_Basic_Compute.h"
#include "CompiledShaderHeader/Shader_Particle_RainDrop_Compute.h"
#include "CompiledShaderHeader/Shader_SCMapLoader_Compute.h"

#include "CompiledShaderHeader/Shader_Particle_1_Vertex.h"
#include "CompiledShaderHeader/Shader_Particle_2_Geometry.h"
#include "CompiledShaderHeader/Shader_Particle_3_Pixel.h"

#endif
