/*
 * gl-utils.c
 *
 * Utility functions for OpenGL code: startup stuff and log functions
 *
 * Based heavily on the code provided with the textbook "Anton's OpenGL 4 
 * Tutorials" by Anton Gerdelan.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "gl-utils.h"


static double previous_seconds;  // used by fps report

/*=============================  LOG FUNCTIONS  =============================*/

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

/*=================================  SHADERS  ================================*/

/*
 * copy a shader from a text file into a char*
 */
bool parse_file_into_str(const char* filename, char* shader_str, int max_len) {
    FILE* fp;
    size_t cnt;
    fp = fopen(filename, "r");
    if (!fp) {
        gl_log(ERR_LOG_YES,
               "ERROR: could not open file \"%s\" for reading",
               filename);
        return false;
    }

    cnt = fread(shader_str, sizeof(char), max_len-1, fp);
    if ((int)cnt >= max_len-1) {
        gl_log(ERR_LOG_YES,
               "WARNING: file \"%s\" too big for buffer, so it was truncated\n",
               filename);
    }

    if (ferror(fp)) {
        gl_log(ERR_LOG_YES,
               "ERROR: error during reading of shader file \"%s\"\n",
               filename);
        fclose(fp);
        return false;
    }

    // append \0 to end of file converted to string
    shader_str[cnt] = '\0';
    fclose(fp);
    return true;
}

/*
 * print errors in shader compilation
 */
void print_shader_info_log(const GLuint shader_index) {
	int max_length = MAX_LOG_LEN;
	int actual_length = 0;
	char log[MAX_LOG_LEN];

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
    int max_length = MAX_LOG_LEN;
    int actual_length = 0;
    char log[MAX_LOG_LEN];

    glGetProgramInfoLog(sp, max_length, &actual_length, log);
    gl_log(ERR_LOG_YES,
           "Program info log for GL shader index: %i:\n%s\n",
           sp,
           log);
}

/*
 * validate shader program
 */
bool program_is_valid(GLuint sp) {
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
 * compile shader and check for compile errors
 */
GLuint compile_shader(const char* filename, GLenum type) {
    GLuint shader;
    char shader_string[MAX_SHADER_LENGTH];
    const GLchar *p;
    int params = -1;
        
    gl_log(ERR_LOG_NO, "creating shader from %s...\n", filename);

    parse_file_into_str(filename, shader_string, sizeof(shader_string));

    // compile shader:
    shader = glCreateShader(type);
    p = (const GLchar*)shader_string;
    glShaderSource(shader, 1, &p, NULL);
    glCompileShader(shader);

    // check for compile errors:
    glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        gl_log(ERR_LOG_YES, 
               "ERROR: GL shader with index %i did not compile\n",
               shader);
        print_shader_info_log(shader);
        return shader;
    }

    gl_log(ERR_LOG_NO, "shader compiled. index %i\n", shader);

    return shader;
}

/*
 * link given shaders into one program and check for linking errors.
 * if binding input attributes, do so before linking.
 */
GLuint link_shaders(GLuint* shaders, const int num_shaders) {
    GLuint shader_program;
    int params = -1;
    
    // Link all shaders:
    shader_program = glCreateProgram();
    for (int i=0; i<num_shaders; i++) {
        glAttachShader(shader_program, shaders[i]);
    }
    glLinkProgram(shader_program);

    // check for linker errors:
    glGetProgramiv(shader_program, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        gl_log(ERR_LOG_YES,
               "ERROR: could not link shader program with GL index %i\n",
               shader_program);
        print_program_info_log(shader_program);
        return shader_program;
    }
    gl_log(ERR_LOG_NO, "shader linked.");

    return shader_program;
}

