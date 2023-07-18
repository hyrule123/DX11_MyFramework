#pragma once

#include "S_H_SetColor.hlsli"
#include "S_H_Initialize.hlsli"
#include "S_H_ParticleBasic.hlsli"

#ifdef _DEBUG

#include "DefaultShader/S_0_V_Debug_Debug.h"
#include "DefaultShader/S_4_P_Debug_Debug.h"

#include "DefaultShader/S_0_V_ParticleBasic_Debug.h"
#include "DefaultShader/S_3_G_ParticleBasic_Debug.h"
#include "DefaultShader/S_4_P_ParticleBasic_Debug.h"

#include "DefaultShader/S_0_V_STD2D_Debug.h"
#include "DefaultShader/S_4_P_STD2D_Debug.h"

#include "DefaultShader/S_0_V_STD2DLight_Debug.h"
#include "DefaultShader/S_4_P_STD2DLight_Debug.h"

#include "DefaultShader/S_0_V_TilemapAtlas_Debug.h"
#include "DefaultShader/S_4_P_TilemapAtlas_Debug.h"

#include "DefaultShader/S_0_V_TilemapComplete_Debug.h"
#include "DefaultShader/S_4_P_TilemapComplete_Debug.h"

#include "DefaultShader/S_C_Initalize_Debug.h"
#include "DefaultShader/S_C_SetColor_Debug.h"
#include "DefaultShader/S_C_ParticleBasic_Debug.h"

#else

#include "DefaultShader/S_0_V_Debug_Release.h"
#include "DefaultShader/S_4_P_Debug_Release.h"

#include "DefaultShader/S_0_V_Particle_Release.h"
#include "DefaultShader/S_3_G_Particle_Release.h"
#include "DefaultShader/S_4_P_Particle_Release.h"

#include "DefaultShader/S_0_V_STD2D_Release.h"
#include "DefaultShader/S_4_P_STD2D_Release.h"

#include "DefaultShader/S_0_V_STD2DLight_Release.h"
#include "DefaultShader/S_4_P_STD2DLight_Release.h"

#include "DefaultShader/S_0_V_TilemapAtlas_Release.h"
#include "DefaultShader/S_4_P_TilemapAtlas_Release.h"

#include "DefaultShader/S_0_V_TilemapComplete_Release.h"
#include "DefaultShader/S_4_P_TilemapComplete_Release.h"

#include "DefaultShader/S_C_Initalize_Release.h"
#include "DefaultShader/S_C_SetColor_Release.h"
#include "DefaultShader/S_C_ParticleBasic_Release.h"

#endif
