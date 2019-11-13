/*
 * gl-utils.h
 *
 * Header file containing functions used in graphics code, including debug/log
 * funcitons
 *
 * Based heavily on the code provided with the textbook "Anton's OpenGL 4 
 * Tutorials" by Anton Gerdelan.
 */

#ifndef __GL_UTILS_H__
#define __GL_UTILS_H__

#include <GL/glew.h>     // GL Extension Wrangler
#include <GLFW/glfw3.h>  // GLFW library for OpenGL

#include <stdarg.h>      // for ... arguments
#include <stdbool.h>     // for bool types

#define ERR_LOG_YES true
#define ERR_LOG_NO  false
#define GL_LOG_FILE "gl.log"
#define MAX_LOG_LEN 2048
#define MAX_SHADER_LENGTH 262144

// extern variables:
extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow *g_window;

// function definitions:
const char* GL_type_to_string(GLenum);
bool restart_gl_log();
bool gl_log(const bool, const char*, ...);

bool start_gl();
void glfw_error_callback(int, const char*);
void glfw_framebuffer_size_callback(GLFWwindow*, int, int);
void update_fps_counter(GLFWwindow*);

bool parse_file_into_str(const char*, char*, int);
void print_shader_info_log(const GLuint);
void print_program_info_log(GLuint);
bool program_is_valid(GLuint);
void print_all_shader_info(GLuint);
GLuint compile_shader(const char*, GLenum);
GLuint link_shaders(GLuint*, const int);

#endif

