/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
\******************************************************************************/
#ifndef _GL_UTILS_H_
#define _GL_UTILS_H_

#include <GL/glew.h>		/* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <stdarg.h>
/* #include <stdbool.h> // for visual studio i had to comment this out and define
 * pure-C bool :( */
#define bool int
#define true 1
#define false 0

#define GL_LOG_FILE "gl.log"

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow *g_window;

bool start_gl();

bool restart_gl_log();

bool gl_log( const char *message, ... );

/* same as gl_log except also prints to stderr */
bool gl_log_err( const char *message, ... );

void glfw_error_callback( int error, const char *description );

void log_gl_params();

void _update_fps_counter( GLFWwindow *window );

void glfw_framebuffer_size_callback( GLFWwindow *window, int width, int height );

#endif

