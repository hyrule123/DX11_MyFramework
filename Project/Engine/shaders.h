#pragma once

//Shader Header
#ifdef _DEBUG

#include "CompiledShaderHeader/Shader_Debug_1_vertex_Debug.h"
#include "CompiledShaderHeader/Shader_Debug_2_pixel_Debug.h"

#include "CompiledShaderHeader/Shader_test_1_vertex_Debug.h"
#include "CompiledShaderHeader/Shader_test_2_pixel_Debug.h"

#include "CompiledShaderHeader/Shader_std2D_1_vertex_Debug.h"
#include "CompiledShaderHeader/Shader_std2D_2_pixel_Debug.h"

#include "CompiledShaderHeader/Shader_std2D_Light_1_vertex_Debug.h"
#include "CompiledShaderHeader/Shader_std2D_Light_2_pixel_Debug.h"

#include "CompiledShaderHeader/Shader_Tilemap_1_vertex_Debug.h"
#include "CompiledShaderHeader/Shader_Tilemap_2_pixel_Debug.h"

#include "CompiledShaderHeader/Shader_SetColor_compute_Debug.h"

#include "CompiledShaderHeader/Shader_Particle_compute_Debug.h"

#include "CompiledShaderHeader/Shader_Particle_1_vertex_Debug.h"
#include "CompiledShaderHeader/Shader_Particle_2_geometry_Debug.h"
#include "CompiledShaderHeader/Shader_Particle_3_pixel_Debug.h"


#else

#include "CompiledShaderHeader/Shader_Debug_1_Vertex.h"
#include "CompiledShaderHeader/Shader_Debug_1_Pixel.h"

#include "CompiledShaderHeader/Shader_test_1_vertex.h"
#include "CompiledShaderHeader/Shader_test_2_pixel.h"

#include "CompiledShaderHeader/Shader_std2D_1_vertex.h"
#include "CompiledShaderHeader/Shader_std2D_2_pixel.h"

#include "CompiledShaderHeader/Shader_std2D_Light_1_vertex.h"
#include "CompiledShaderHeader/Shader_std2D_Light_2_pixel.h"

#include "CompiledShaderHeader/Shader_Tilemap_1_vertex.h"
#include "CompiledShaderHeader/Shader_Tilemap_2_pixel.h"

#include "CompiledShaderHeader/Shader_SetColor_compute.h"

#include "CompiledShaderHeader/Shader_Particle_compute.h"

#include "CompiledShaderHeader/Shader_Particle_1_vertex.h"
#include "CompiledShaderHeader/Shader_Particle_2_geometry.h"
#include "CompiledShaderHeader/Shader_Particle_3_pixel.h"

#endif
