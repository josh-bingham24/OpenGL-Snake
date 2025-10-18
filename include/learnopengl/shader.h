#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <cglm/cglm.h>

typedef struct Shader {
  // the program ID
  unsigned int ID;

  // function ptrs
  // void (*use)(struct Shader *shader);
  // void (*setBool)(struct Shader *shader, const char *name, bool value);
  // void (*setInt)(struct Shader *shader, const char *name, int value);
  // void (*setFloat)(struct Shader *shader, const char *name, float value);
} Shader;


// function definitions
void useShader(Shader *self) { 
    glUseProgram(self->ID);
}

void setBool(Shader *self, const char *name, bool value) {
    glUniform1i(glGetUniformLocation(self->ID, name), (int)value);
}

void setInt(Shader *self, const char *name, int value) {
    glUniform1i(glGetUniformLocation(self->ID, name), value);
}

void setFloat(Shader *self, const char *name, float value) {
    glUniform1f(glGetUniformLocation(self->ID, name), value);
}

// TODO: add mat4 set function
void setMat4(Shader *self, const char *name, mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(self->ID, name), 1, GL_FALSE, (const float*)value);
}

// initialize shader
void Shader_init(Shader *shader, char *vertexPath, char *fragmentPath) {

  char cwd[1024];
  // 1. retrieve the vertex/fragment source code from filePath
  FILE *vFilePtr;
  FILE *fFilePtr;

  char vertexLine[50];
  char fragmentLine[50];

  char fullVertexCode[UINT16_MAX];
  char fullFragmentCode[UINT16_MAX];

  fullVertexCode[0] = '\0';
  fullFragmentCode[0] = '\0';

  // read files
    vFilePtr = fopen(vertexPath, "r");
    fFilePtr = fopen(fragmentPath, "r");
    if (!vFilePtr) {
      perror("Failed to open vertex file");
      return;
    }
    if (!fFilePtr) {
      perror("Failed to open fragment file");
      return;
    }

    while (fgets(vertexLine, sizeof(vertexLine), vFilePtr)) {
      strcat(fullVertexCode, vertexLine);
    }

    while (fgets(fragmentLine, sizeof(fragmentLine), fFilePtr)) {
      strcat(fullFragmentCode, fragmentLine);
    }

    fclose(vFilePtr);
    fclose(fFilePtr);

    const char *vShaderCode = fullVertexCode;
    const char *fShaderCode = fullFragmentCode;

    // 2. compile shaders
    unsigned int vertex, fragment, id;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    };

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment, 512, NULL, infoLog);
      printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    };

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(id, 512, NULL, infoLog);
      printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    shader->ID = id;
    // shader->use = use;
    // shader->setBool = setBool;
    // shader->setInt = setInt;
    // shader->setFloat = setFloat;
}

#endif