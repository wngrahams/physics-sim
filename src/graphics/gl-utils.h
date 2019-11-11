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

#define GL_LOG_FILENAME "gl.log"

// extern variables:
extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow *g_window;

// function definitions:
bool start_gl();

bool restart_gl_log();
bool gl_log(const char* message, ...);

bool gl_log_err(const char* message, ...);

void glfw_error_callback(int error, const char *description);
void log_gl_params();
void update_fps_counter(GLFWwindow *window);
void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height).

#endif

