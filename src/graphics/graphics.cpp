/*
 * graphics.c
 *
 * main function to start OpenGL and its associated helper libraries and 
 * continuously draw output to the screen
 *
 * Credit to the book "Anton's OpenGL 4 Tutorials" by Dr Anton Gerdelan for the
 * basis of this code.
 */

#include <math.h>
#include <stdlib.h>

#include "gl-utils.h"
#include "graphics.h"
#include "maths_funcs.hpp"

#define FILE_SIZE_VSHADER (1024 * 256)
#define FILE_SIZE_FSHADER (1024 * 256)

#define CLIPPING_NEAR (0.1f)
#define CLIPPING_FAR  (100.0f)
#define FOV           (67.0f * ONE_DEG_IN_RAD)
#define CAM_SPEED     (2000.0f)   // 2000 units per second
#define CAM_YAW_SPEED (200000.0f)  // 200000 degrees per second


int main(int argv, char** argc) {
    GLfloat points[] = {  0.0f,  0.5f,  0.0f, 
                          0.5f, -0.5f,  0.0f, 
                         -0.5f, -0.5f,  0.0f  };

    GLfloat colors[] = {  1.0f, 0.0f, 0.0f, 
                          0.0f, 1.0f, 0.0f, 
                          0.0f, 0.0f, 1.0f };



    int num_points = 3;

    GLuint points_vbo, colors_vbo;
    GLuint vao;
    char vertex_shader[FILE_SIZE_VSHADER];
	char fragment_shader[FILE_SIZE_FSHADER];
	GLuint vs, fs, shader_program;
	const GLchar *p;
	int params = -1;
	//GLint color_loc;

    // camera vars:
    float aspect, inverse_range, s_x, s_y, s_z, p_z, cam_yaw;
    

    // restart log file:
    restart_gl_log();

    // initialize GLFW3 and GLEW
    start_gl(); 

    // only draw a pixel if the shape is closest to the viewer:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // load points into GPU using Vertex Buffer Object (vbo):
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 
                 3*num_points*sizeof(GLfloat), 
                 points, 
                 GL_STATIC_DRAW);

    // make a second vbo for the colors
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3*num_points*sizeof(GLfloat),
                 colors,
                 GL_STATIC_DRAW);

    // generate vertex attribute object (vao):
    // we have two vertex shader input variables: 0 and 1, for points_vbo and
    // colors_vbo respectively
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

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

    GLfloat matrix[] = {
		1.5f, 0.0f, 0.0f, 0.0f, // first column
		0.0f, 1.5f, 0.0f, 0.0f, // second column
		0.0f, 0.0f, 1.5f, 0.0f, // third column
		0.0f, 0.0f, 0.0f, 1.0f	// fourth column
	};

    //int matrix_location = glGetUniformLocation(shader_program, "matrix");

    /*--------------------------create camera
	 * matrices----------------------------*/
	/* create PROJECTION MATRIX */
	// input variables
	aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
	// matrix components
	inverse_range = 1.0f/ tan(FOV*0.5f);
	s_x = inverse_range/aspect;
	s_y = inverse_range;
	s_z = -(CLIPPING_FAR + CLIPPING_NEAR)/(CLIPPING_FAR - CLIPPING_NEAR);
	p_z = -(2.0f * CLIPPING_FAR * CLIPPING_NEAR)/(CLIPPING_FAR - CLIPPING_NEAR);
	GLfloat proj_mat[] = {   s_x,  0.0f,  0.0f,  0.0f,	
                            0.0f,   s_y,  0.0f,  0.0f,
						    0.0f,  0.0f,   s_z, -1.0f, 
                            0.0f,  0.0f,   p_z,  0.0f };

    /* create VIEW MATRIX */
	float cam_pos[] = { 0.0f, 0.0f, 2.0f };  // don't start at zero 
                                             // or we will be too close
	cam_yaw = 0.0f;				// y-rotation in degrees
	mat4 T =
		translate( identity_mat4(), vec3( -cam_pos[0], -cam_pos[1], -cam_pos[2] ) );
	mat4 R = rotate_y_deg( identity_mat4(), -cam_yaw );
	mat4 view_mat = R * T;

	// get location numbers of matrices in shader programme 
	GLint view_mat_location = glGetUniformLocation(shader_program, "view" );
	GLint proj_mat_location = glGetUniformLocation(shader_program, "proj" );
	/* use program (make current in state machine) and set default matrix values*/
	/*glUseProgram( shader_programme );
	glUniformMatrix4fv( view_mat_location, 1, GL_FALSE, view_mat.m );
	glUniformMatrix4fv( proj_mat_location, 1, GL_FALSE, proj_mat );*/

    //TODO comment this out
    print_all_shader_info(shader_program);

    // validate shader (computationally expensive; TODO: remove when done testing):
    assert(shader_is_valid(shader_program));

    // get unique location of variable 'inputColor':
    //color_loc = glGetUniformLocation(shader_program, "inputColor");
    //assert(color_loc > -1);

    // switch to the shader program:
    glUseProgram(shader_program);
    //glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat);

    // assign initial color to fragment shader:
    //glUniform4f(color_loc, 0.6f, 0.0f, 0.6f, 1.0f);

    // set background color:
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);  // grey

    //glEnable(GL_CULL_FACE);  // cull face
	//glCullFace(GL_BACK);	   // cull back face
	glFrontFace(GL_CW);      // GL_CCW for counter clock-wise

    float speed = 400.0f;
    float last_position_x = 0.0f;
    float last_position_y = 0.0f;
    float last_position_z = 0.0f;
    // continually draw until window is closed 
    while (!glfwWindowShouldClose(g_window)) {

        // timer for doing animation:
        double previous_seconds = glfwGetTime();
        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        // update fps counter:
        update_fps_counter(g_window);

        // clear drawing surface:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // uncomment when not using fullscreen mode:
        //glViewport(0, 0, g_gl_width, g_gl_height);

        // set shader program:
        glUseProgram(shader_program);

        // update the matrix
		// - you could simplify this by just using sin(current_seconds)
        /*
		matrix[0] = elapsed_seconds * speed + last_position_x;
    	matrix[5] = elapsed_seconds * speed + last_position_y;
	    matrix[10] = elapsed_seconds * speed + last_position_z;

		last_position_x = matrix[0];
        last_position_y = matrix[5];
        last_position_z = matrix[10];
		if ( fabs( last_position_x ) > 4.0 ) {
			speed = -speed;
		}
		//
		// Note: this call is related to the most recently 'used' shader programme
		glUniformMatrix4fv( matrix_location, 1, GL_FALSE, matrix );*/

        // bind VAO:
        glBindVertexArray(vao);

        // draw points 0-3 from the currently bound VAO with current 
        // in-use shader:
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // update other events (i.e. user inputs);
        glfwPollEvents();

        /*-----------------------------move camera
		 * here-------------------------------*/
		// control keys
		bool cam_moved = false;
		if ( glfwGetKey( g_window, GLFW_KEY_A ) ) {
			cam_pos[0] -= CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_D ) ) {
			cam_pos[0] += CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_UP ) ) {
			cam_pos[1] += CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_DOWN ) ) {
			cam_pos[1] -= CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_W ) ) {
			cam_pos[2] -= CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_S ) ) {
			cam_pos[2] += CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_LEFT ) ) {
			cam_yaw += CAM_YAW_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_RIGHT ) ) {
			cam_yaw -= CAM_YAW_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		/* update view matrix */
		if (cam_moved) {
            printf("cam moved\n");
			mat4 T = translate(identity_mat4(), vec3( -cam_pos[0], -cam_pos[1],
																								 -cam_pos[2] ) ); // cam translation
			mat4 R = rotate_y_deg( identity_mat4(), -cam_yaw );					//
			mat4 view_mat = R * T;
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
		}

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

