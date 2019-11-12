/*
 * graphics.c
 *
 * main function to start OpenGL and its associated helper libraries and 
 * continuously draw output to the screen
 *
 * Credit to the book "Anton's OpenGL 4 Tutorials" by Dr Anton Gerdelan for the
 * basis of this code.
 */

#include <stdlib.h>

#include "gl-utils.h"
#include "graphics.h"

#define FILE_SIZE_VSHADER (1024*256)
#define FILE_SIZE_FSHADER (1024*256)


int main(int argv, char** argc) {
    GLfloat points[] = {  0.0f,  0.5f,  0.0f, 
                          0.5f, -0.5f,  0.0f, 
                         -0.5f, -0.5f,  0.0f  };
    int num_points = 3;

    GLuint vbo;
    GLuint vao;
    char vertex_shader[FILE_SIZE_VSHADER];
	char fragment_shader[FILE_SIZE_FSHADER];
	GLuint vs, fs, shader_program;
	const GLchar *p;
	int params = -1;
	GLint color_loc;

    // restart log file:
    restart_gl_log();

    // initialize GLFW3 and GLEW
    start_gl(); 

    // only draw a pixel if the shape is closest to the viewer:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // load points into GPU using Vertex Buffer Object (vbo):
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 
                 3*num_points*sizeof(GLfloat), 
                 points, 
                 GL_STATIC_DRAW);

    // generate vertex attribute object (vao):
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // load shaders from files:
    parse_file_into_str("test_vs.glsl", vertex_shader, sizeof(vertex_shader));
    parse_file_into_str("test_fs.glsl", 
                        fragment_shader, 
                        sizeof(fragment_shader));

    // TODO: move compile, link, and error checking for both to another function
    // compile vertex shader:
    vs = glCreateShader(GL_VERTEX_SHADER);
    p = (const GLchar*)vertex_shader;
    glShaderSource(vs, 1, &p, NULL);
    glCompileShader(vs);

    // check vertex shader for compile errors:
    glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR: GL shader with index %i did not compile\n", vs);
        print_shader_info_log(vs);
        return 1;
    }

    // compile fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    p = (const GLchar*)fragment_shader;
    glShaderSource(fs, 1, &p, NULL);
    glCompileShader(fs);

    // check fragment shader for compile errors:
    glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR: GL shader with index %i did not compile\n", fs);
        print_shader_info_log(fs);
        return 1;
    }

    // link shaders
    shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);

    // check for shader linking errors:
    glGetProgramiv(shader_program, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, 
                "ERROR: could not link shader program with GL index %i\n",
                shader_program);
        print_program_info_log(shader_program);
        return 1;
    }
    // END TODO

    //TODO comment this out
    print_all_shader_info(shader_program);

    // validate shader (computationally expensive; TODO: remove when done testing):
    assert(shader_is_valid(shader_program));

    // get unique location of variable 'inputColor':
    color_loc = glGetUniformLocation(shader_program, "inputColor");
    assert(color_loc > -1);

    // switch to the shader program:
    glUseProgram(shader_program);

    // assign initial color to fragment shader:
    glUniform4f(color_loc, 0.6f, 0.0f, 0.6f, 1.0f);

    // set background color:
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);  // grey

    // continually draw until window is closed 
    while (!glfwWindowShouldClose(g_window)) {
        // update fps counter:
        update_fps_counter(g_window);

        // clear drawing surface:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set shader program:
        glUseProgram(shader_program);

        // bind VAO:
        glBindVertexArray(vao);

        // draw points 0-3 from the currently bound VAO with current 
        // in-use shader:
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // update other events (i.e. user inputs);
        glfwPollEvents();
        if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(g_window, 1);  // close window on esc press
        }

        // actually display the drawings:
        glfwSwapBuffers(g_window);
    }

    // close GL context and GLFW resources:
    glfwTerminate();

    return 0;
}

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

