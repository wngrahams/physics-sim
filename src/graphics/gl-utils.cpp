/*
 * gl-utils.c
 *
 * Utility functions for OpenGL code: startup stuff and log functions
 *
 * Based heavily on the code provided with the textbook "Anton's OpenGL 4 
 * Tutorials" by Anton Gerdelan.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "gl-utils.h"

#define GL_LOG_FILE "gl.log"

static double previous_seconds;  // used by fps report

/*=============================  LOG FUNCTIONS  =============================*/
bool restart_gl_log() {
    time_t time_current;
    char* date;
    FILE* fp = fopen(GL_LOG_FILE, "w");
    if (!fp) {
        fprintf(stderr, 
                "ERROR: could not open log file \"%s\" for writing.\n",
                GL_LOG_FILE);
        return false;
    }

    time_current = time(NULL);
    date = ctime(&time_current);
    fprintf(fp, "%s\nLocal time: %s\n", GL_LOG_FILE, date);
    fclose(fp);
    return true;
}

bool gl_log(const bool err_log, const char* message, ...) {
    va_list argptr;
    FILE* fp = fopen(GL_LOG_FILE, "a");
    if (!fp) {
        fprintf(stderr,
                "ERROR: could not open log file \"%s\" for writing.\n",
                GL_LOG_FILE);
        return false;
    }

    va_start(argptr, message);
    vfprintf(fp, message, argptr);
    va_end(argptr);
    if (err_log) {
        va_start(argptr, message);
        vfprintf(stderr, message, argptr);
        va_end(argptr);
    }
    fclose(fp);
    return true;
}

/*=========================  GLFW3 AND GLEW STARTUP  =========================*/
bool start_gl() {
    const GLubyte *renderer;
    const GLubyte *version;

    gl_log(ERR_LOG_NO, "starting GLFW %s", glfwGetVersionString());

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // fullscreen:
    /*
    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    const GLFWvidmode* vmode = glfwGetVideoMode(mon);
    g_window = glfwCreateWindow (vmode->width,
                                 vmode->height,
                                 "physics-sim",
                                 mon,
                                 NULL);
    */

    // windowed:
    /**/
    g_window = glfwCreateWindow(g_gl_width, 
                                g_gl_height, 
                                "physics-sim", 
                                NULL, 
                                NULL);
    /**/

    if(!g_window) {
        fprintf(stderr, "ERROR: cound not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }

    glfwSetFramebufferSizeCallback(g_window, glfw_framebuffer_size_callback);
    glfwMakeContextCurrent(g_window);

    glfwWindowHint(GLFW_SAMPLES, 4);

    // Start GLEW:
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    renderer = glGetString(GL_RENDERER); // get renderer string
    version = glGetString(GL_VERSION);  // get version string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    gl_log(ERR_LOG_NO, "renderer: %s\nversion: %s\n", renderer, version);
    previous_seconds = glfwGetTime();

    return true;
}

// function for error callback
void glfw_error_callback(int error, const char* description) {
    fputs(description, stderr);
    gl_log(ERR_LOG_YES, "%s\n", description);
}

// function for framebuffer size callback
void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    g_gl_width = width;
    g_gl_height = height;
    gl_log(ERR_LOG_NO, 
           "Framebuffer size changed to: width %i height %i\n",
           width,
           height);

    /* UPDATE ANY PERSPECTIVE MATRICIES HERE: */
}

void update_fps_counter(GLFWwindow* window) {
    static int frame_count;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;

    if (elapsed_seconds > 0.3) {
        double fps;
        char tmp[64];

        previous_seconds = current_seconds;
        fps = ((double)frame_count)/elapsed_seconds;
        sprintf(tmp, "physics-sim @ fps: %.1f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count++;
}

