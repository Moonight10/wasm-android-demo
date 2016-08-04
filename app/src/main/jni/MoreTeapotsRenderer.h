/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//--------------------------------------------------------------------------------
// MoreTeapotsRenderer.h
// Renderer for teapots
//--------------------------------------------------------------------------------
#ifndef _MoreTeapotsRenderer_H
#define _MoreTeapotsRenderer_H

//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <random>
#include <vector>

#include <EGL/egl.h>
#include <GLES/gl.h>

#define CLASS_NAME "android/app/NativeActivity"
#define APPLICATION_CLASS_NAME "com/sample/moreteapots/MoreTeapotsApplication"

//#include "NDKHelper.h"

#include "gl3stub.h"    //GLES3 stubs
//#include "GLContext.h"  //EGL & OpenGL manager
#include "shader.h"     //Shader compiler support
#include "vecmath.h"  //Vector math support, C++ implementation n current version

extern "C" {
    #include "platform_asset_utils.h"
    #include "platform_file_utils.h"
};

//#include "tapCamera.h"        //Tap/Pinch camera control
//#include "JNIHelper.h"        //JNI support
//#include "gestureDetector.h"  //Tap/Doubletap/Pinch detector
//#include "perfMonitor.h"      //FPS counter
//#include "interpolator.h"     //Interpolator

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

struct TEAPOT_VERTEX {
  float pos[3];
  float normal[3];
};

enum SHADER_ATTRIBUTES {
  ATTRIB_VERTEX,
  ATTRIB_NORMAL,
  ATTRIB_COLOR,
  ATTRIB_UV
};

struct SHADER_PARAMS {
  GLuint program_;
  GLuint light0_;
  GLuint material_diffuse_;
  GLuint material_ambient_;
  GLuint material_specular_;

  GLuint matrix_projection_;
  GLuint matrix_view_;
};

struct TEAPOT_MATERIALS {
  float specular_color[4];
  float ambient_color[3];
};

class MoreTeapotsRenderer {
  int32_t num_indices_;
  int32_t num_vertices_;
  GLuint ibo_;
  GLuint vbo_;
  GLuint ubo_;
    int32_t origin_num_indices_;
    int32_t origin_num_vertices_;
    int32_t num_instances_;

  SHADER_PARAMS shader_param_;
  bool LoadShaders(SHADER_PARAMS* params, const char* strVsh,
                   const char* strFsh);
#ifdef GL3
  bool LoadShadersES3(SHADER_PARAMS* params, const char* strVsh,
                      const char* strFsh,
                      std::map<std::string, std::string>& shaderParameters);
#endif

  ndk_helper::Mat4 mat_projection_;
  ndk_helper::Mat4 mat_view_;
  std::vector<ndk_helper::Mat4> vec_mat_models_;
  std::vector<ndk_helper::Vec3> vec_colors_;
  std::vector<ndk_helper::Vec2> vec_rotations_;
  std::vector<ndk_helper::Vec2> vec_current_rotations_;

//  ndk_helper::TapCamera* camera_;

  int32_t teapot_x_;
  int32_t teapot_y_;
  int32_t teapot_z_;
  int32_t ubo_matrix_stride_;
  int32_t ubo_vector_stride_;
  bool geometry_instancing_support_;
  bool arb_support_;

  std::string ToString(const int32_t i);

 public:
  MoreTeapotsRenderer();
  virtual ~MoreTeapotsRenderer();
  void Init(const int32_t numX, const int32_t numY, const int32_t numZ);
  void Render();
  void Update(float dTime);
//  bool Bind(ndk_helper::TapCamera* camera);
  void Unload();
  void UpdateViewport();
};

#endif
