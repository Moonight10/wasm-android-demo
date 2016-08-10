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
#include "platform.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "shader.h"
#ifdef TARGET_ANDROID
#include "JNIHelper.h"
#endif
#include <stdlib.h>
#include <stdio.h>

namespace ndk_helper {

#define DEBUG (1)

#ifdef TARGET_ANDROID
bool shader::CompileShader(
    GLuint *shader, const GLenum type, const char *str_file_name,
    const std::map<std::string, std::string> &map_parameters) {
  std::vector<uint8_t> data;
  if (!JNIHelper::GetInstance()->ReadFile(str_file_name, &data)) {
    LOGI("Can not open a file:%s", str_file_name);
    return false;
  }

  const char REPLACEMENT_TAG = '*';
  // Fill-in parameters
  std::string str(data.begin(), data.end());
  std::string str_replacement_map(data.size(), ' ');

  std::map<std::string, std::string>::const_iterator it =
      map_parameters.begin();
  std::map<std::string, std::string>::const_iterator itEnd =
      map_parameters.end();
  while (it != itEnd) {
    size_t pos = 0;
    while ((pos = str.find(it->first, pos)) != std::string::npos) {
      // Check if the sub string is already touched

      size_t replaced_pos = str_replacement_map.find(REPLACEMENT_TAG, pos);
      if (replaced_pos == std::string::npos || replaced_pos > pos) {

        str.replace(pos, it->first.length(), it->second);
        str_replacement_map.replace(pos, it->first.length(), it->first.length(),
                                    REPLACEMENT_TAG);
        pos += it->second.length();
      } else {
        // The replacement target has been touched by other tag, skipping them
        pos += it->second.length();
      }
    }
    it++;
  }

  LOGI("Patched Shdader:\n%s", str.c_str());

  std::vector<uint8_t> v(str.begin(), str.end());
  str.clear();
  return shader::CompileShader(shader, type, v);
}
#endif

bool shader::CompileShader(GLuint *shader, const GLenum type,
                           const GLchar *source, const int32_t iSize) {
  if (source == NULL || iSize <= 0) return false;

  *shader = glCreateShader(type);
  glShaderSource(*shader, 1, &source, &iSize);  // Not specifying 3rd parameter
                                                // (size) could be troublesome..

  glCompileShader(*shader);

#if defined(DEBUG)
  GLint logLength;
  glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    GLchar *log = (GLchar *)malloc(logLength);
    glGetShaderInfoLog(*shader, logLength, &logLength, log);
#ifdef TARGET_ANDROID
    LOGI("Shader compile log:\n%s", log);
#else
    printf("Shader compile log:%s\n", log);
#endif
    free(log);
  }
#endif

  GLint status;
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
  if (status == 0) {
    printf("compile failure\n");
    glDeleteShader(*shader);
    return false;
  }

  return true;
}

bool shader::CompileShader(GLuint *shader, const GLenum type,
                           std::vector<uint8_t> &data) {
  if (!data.size()) return false;

  const GLchar *source = (GLchar *)&data[0];
  int32_t iSize = data.size();
  return shader::CompileShader(shader, type, source, iSize);
}

#ifdef TARGET_ANDROID
bool shader::CompileShader(GLuint *shader, const GLenum type,
                           const char *strFileName) {
  std::vector<uint8_t> data;
  bool b = JNIHelper::GetInstance()->ReadFile(strFileName, &data);
  if (!b) {
    LOGI("Can not open a file:%s", strFileName);
    return false;
  }

  return shader::CompileShader(shader, type, data);
}
#endif

bool shader::LinkProgram(const GLuint prog) {
  GLint status;

  glLinkProgram(prog);

 #if defined(DEBUG)
   GLint logLength;
   glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
   if (logLength > 0) {
     GLchar *log = (GLchar *)malloc(logLength);
     glGetProgramInfoLog(prog, logLength, &logLength, log);
 #ifdef TARGET_ANDROID
     LOGI("Program link log:\n%s", log);
 #endif
     printf("Program link log:\n%s\n", log);
     free(log);
   }
 #endif

  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  if (status == 0) {
#ifdef TARGET_ANDROID
    LOGI("Program link failed\n");
#endif
    return false;
  }

  return true;
}

bool shader::ValidateProgram(const GLuint prog) {
  GLint logLength, status;

  glValidateProgram(prog);
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
   if (logLength > 0) {
     GLchar *log = (GLchar *)malloc(logLength);
     glGetProgramInfoLog(prog, logLength, &logLength, log);
#ifdef TARGET_ANDROID
     LOGI("Program validate log:\n%s", log);
#endif
     free(log);
   }

  glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
  if (status == 0) return false;

  return true;
}

}  // namespace ndkHelper
