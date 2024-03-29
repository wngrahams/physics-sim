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

#include "gl-utils.h"

// externed variables -- keep track of window size for viewport and mouse
int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow* g_window = NULL;

// function definitions
void calculate_normals_from_points(GLfloat*, GLfloat*, int);

#endif

