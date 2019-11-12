/*
 * graphics.h
 *
 * Function definitions for graphics.c
 *
 * Credit to the textbook "Anton's OpenGL 4 Tutorials" by Dr. Anton Gerdelan
 * for the basis of this code.
 */

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <stdio.h>

#include "gl-utils.h"

#define MAX_SHADER_LEN 2048

// externed variables -- keep track of window size for viewport and mouse
int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow* g_window = NULL;

const char* GL_type_to_string(GLenum type) {
    switch (type) {
		case GL_BOOL:
			return "bool";
		case GL_INT:
			return "int";
		case GL_FLOAT:
			return "float";
		case GL_FLOAT_VEC2:
			return "vec2";
		case GL_FLOAT_VEC3:
			return "vec3";
		case GL_FLOAT_VEC4:
			return "vec4";
		case GL_FLOAT_MAT2:
			return "mat2";
		case GL_FLOAT_MAT3:
			return "mat3";
		case GL_FLOAT_MAT4:
			return "mat4";
		case GL_SAMPLER_2D:
			return "sampler2D";
		case GL_SAMPLER_3D:
			return "sampler3D";
		case GL_SAMPLER_CUBE:
			return "samplerCube";
		case GL_SAMPLER_2D_SHADOW:
			return "sampler2DShadow";
		default:
			break;
		}
	return "other";
}

/*
 * print errors in shader compilation
 */
void print_shader_info_log(const GLuint shader_index) {
	int max_length = MAX_SHADER_LEN;
	int actual_length = 0;
	char log[MAX_SHADER_LEN];

    glGetShaderInfoLog(shader_index, max_length, &actual_length, log);
    gl_log(ERR_LOG_YES, 
           "Shader info log for GL shader index: %i:\n%s\n",
           shader_index,
           log);
}

/*
 * print errors in shader linking
 */
void print_program_info_log(GLuint sp) {
    int max_length = MAX_SHADER_LEN;
    int actual_length = 0;
    char log[MAX_SHADER_LEN];

    glGetProgramInfoLog(sp, max_length, &actual_length, log);
    gl_log(ERR_LOG_YES,
           "Program info log for GL shader index: %i:\n%s\n",
           sp,
           log);
}

/*
 * validate shader
 */
bool shader_is_valid(GLuint sp) {
    int params = -1;
    
    glValidateProgram(sp);
    glGetProgramiv(sp, GL_VALIDATE_STATUS, &params);
    gl_log(ERR_LOG_NO, "program %i GL_VALIDATE_STATUS = %i\n", sp, params);

    if (GL_TRUE != params) {
        fprintf(stderr, "program %i GL_VALIDATE_STATUS = %i\n", sp, params);
        print_program_info_log(sp);
        return false;
    }

    return true;
}

/*
 * print all info about a shader
 * useful for debugging
 */
void print_all_shader_info(GLuint sp) {
    int params = -1;

    fprintf(stderr, "--------------------\nshader programme %i info:\n", sp);
	glGetProgramiv(sp, GL_LINK_STATUS, &params);
	fprintf(stderr, "GL_LINK_STATUS = %i\n", params);

	glGetProgramiv(sp, GL_ATTACHED_SHADERS, &params);
	fprintf(stderr, "GL_ATTACHED_SHADERS = %i\n", params);

	glGetProgramiv(sp, GL_ACTIVE_ATTRIBUTES, &params);
	fprintf(stderr, "GL_ACTIVE_ATTRIBUTES = %i\n", params);

	for (int i=0; i<params; i++) {
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveAttrib(sp, i, 
                          max_length, &actual_length,
                          &size, &type, name);
		if (size > 1) {
			for (int j=0; j<size; j++) {
				char long_name[64];
				int location;

				sprintf(long_name, "%s[%i]", name, j);
				location = glGetAttribLocation(sp, long_name);
				fprintf(stderr, 
                        "  %i) type:%s name:%s location:%i\n", 
                        i,
                        GL_type_to_string(type),
						long_name,
                        location);
			}
		} 
        else {
			int location = glGetAttribLocation(sp, name);
			fprintf(stderr, 
                    "  %i) type:%s name:%s location:%i\n", 
                    i, 
                    GL_type_to_string(type),
					name, 
                    location);
		}
	}

	glGetProgramiv(sp, GL_ACTIVE_UNIFORMS, &params);
	fprintf(stderr, "GL_ACTIVE_UNIFORMS = %i\n", params);
	for (int i = 0; i<params; i++) {
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveUniform(sp, i, max_length, &actual_length, &size, &type, name);
		if (size > 1) {
			for (int j=0; j<size; j++) {
				char long_name[64];
				int location;

				sprintf(long_name, "%s[%i]", name, j);
				location = glGetUniformLocation( sp, long_name);
				fprintf(stderr, 
                        "  %i) type:%s name:%s location:%i\n", 
                        i, 
                        GL_type_to_string(type),
						long_name, 
                        location);
			}
		} 
        else {
			int location = glGetUniformLocation(sp, name);
			fprintf(stderr, 
                    "  %i) type:%s name:%s location:%i\n", 
                    i, 
                    GL_type_to_string(type),
					name, 
                    location);
		}
	}

	print_program_info_log(sp);
}

/*
 * copy a shader from a text file into a char*
 */
bool parse_file_into_str(const char*, char*, int);

#endif

