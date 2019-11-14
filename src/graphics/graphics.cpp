/*
 * graphics.c
 *
 * main function to start OpenGL and its associated helper libraries and 
 * continuously draw output to the screen
 *
 * Credit to the book "Anton's OpenGL 4 Tutorials" by Dr Anton Gerdelan for the
 * basis of this code.
 */

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "gl-utils.h"
#include "camera.hpp"
#include "graphics.h"
#include "maths_funcs.hpp"
#include "obj_parser.hpp"

#define MESH_FILE "plane.obj"
#define NUM_SPHERES 4
#define NUM_PLANES 2

#define CAM_START_POS 0.0f, 0.0f, 5.0f

int main(int argv, char** argc) {

    GLuint points_vbo, normals_vbo;
    GLuint vao;
	GLuint vs, fs, shader_program, fs_black, shader_program_black;
    GLuint *shaders, *shaders_black, *programs;
	
    int model_mat_location, view_mat_location, proj_mat_location;

    // camera vars:
    
    float aspect;
    float cam_heading = 0.0f;
    float quaternion[4];
    

    // keep track of some useful vectors that can be used for keyboard movement
    
	vec4 fd(DIR_FD);
	vec4 rt(DIR_RT);
	vec4 up(DIR_UP);
    

    // camera matricies:
    /*
    mat4 *view_mat = new mat4();
    mat4 *proj_mat = new mat4();// T, R;
    */

    mat4 proj_mat, view_mat, T, R;
    vec3 cam_pos(CAM_START_POS);
    // a world position for each sphere in the scene
    vec3 sphere_pos_wor[] = { vec3( -2.0, 0.0, 0.0 ),
                              vec3( 2.0, 0.0, 0.0 ),
							  vec3( -2.0, 0.0, -2.0 ), 
                              vec3( 1.5, 1.0, -1.0 ) };
    // world position for each plane
    vec3 plane_pos_wor[] = { vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0) };
    
    // Create geometry (from file)
    GLfloat *vp = NULL;  // array of vertex points
	GLfloat *vn = NULL;  // array of vertex normals
	GLfloat *vt = NULL;  // array of texture coordinates
	int point_count = 0;
	
    // restart log file:
    restart_gl_log();

    // initialize GLFW3 and GLEW
    start_gl(); 

    // only draw a pixel if the shape is closest to the viewer:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //load_obj_file(MESH_FILE, vp, vt, vn, point_count);
    point_count = 3;

    GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };
    float normals[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};

    //GLuint points_vbo;
	glGenBuffers( 1, &points_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
	glBufferData( GL_ARRAY_BUFFER, 9 * sizeof( GLfloat ), points, GL_STATIC_DRAW );

	//GLuint normals_vbo;
	glGenBuffers( 1, &normals_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, normals_vbo );
	glBufferData( GL_ARRAY_BUFFER, 9 * sizeof( GLfloat ), normals, GL_STATIC_DRAW );

	//GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glBindBuffer( GL_ARRAY_BUFFER, normals_vbo );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

    /*
    // generate vertex attribute object (vao):
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    // load points into GPU using Vertex Buffer Object (vbo):
    if (NULL != vp) {
		glGenBuffers(1 , &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, 
                     3*point_count*sizeof(GLfloat), 
                     vp,
					 GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	} 
    if (NULL != vn) {
        glGenBuffers(1, &normals_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, 
                     3*point_count*sizeof(GLfloat),
                     vn,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);
    }*/

    // get shaders from files, compile, and link:
    vs = compile_shader("test_vs.glsl", GL_VERTEX_SHADER);
    fs_black = compile_shader("test_fs.glsl", GL_FRAGMENT_SHADER);
    fs = compile_shader("test_fs.glsl", GL_FRAGMENT_SHADER);
    shaders = new GLuint[2];
    shaders[0] = vs;
    shaders[1] = fs;
    shader_program = link_shaders(shaders, 2);
    assert(program_is_valid(shader_program));

    shaders_black = new GLuint[2];
    shaders_black[0] = vs;
    shaders_black[1] = fs_black;
    shader_program_black = link_shaders(shaders_black, 2);
    assert(program_is_valid(shader_program_black));
    programs = new GLuint[2];
    programs[0] = shader_program;
    programs[1] = shader_program_black;

    // get Uniform variable locations from shaders:
    model_mat_location = glGetUniformLocation(shader_program, "model");
    view_mat_location = glGetUniformLocation(shader_program, "view");
    proj_mat_location = glGetUniformLocation(shader_program, "proj");
    
    // free unneeded memory used when making shaders:
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(fs_black);
    delete shaders;  
    delete shaders_black;

    /* --- CAMERA SETUP --- */
    /*
    Camera camera((float)g_gl_width,
                  (float)g_gl_height,
                  vec3(CAM_START_POS),
                  view_mat,
                  proj_mat);
                  */
    
    aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
    proj_mat = perspective(FOV_Y, aspect, CLIPPING_NEAR, CLIPPING_FAR);

    // translation matrix:
    T = translate(identity_mat4(),
				  vec3(-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2]));

    // rotation matrix:
    // make a quaternion representing negated initial camera orientation:
	create_versor(quaternion, -cam_heading, 0.0f, 1.0f, 0.0f);
    // convert the quaternion to a rotation matrix:
    quat_to_mat4(R.m, quaternion);

    // combine the inverse rotation and transformation to make a view matrix:
	//view_mat = R * T;
    view_mat = look_at(cam_pos, vec3(0.0f, 0.0f, 0.0f), up);
    /* --- END CAMERA SETUP -- */

    /* --- RENDER SETTINGS --- */
    glUseProgram(shader_program);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);

	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat.m);

	// unique model matrix for each sphere
    /*
	mat4 model_mats[NUM_SPHERES];
	for (int i=0; i<NUM_SPHERES; i++) {
		model_mats[i] = translate(identity_mat4(), sphere_pos_wor[i]);
	}*/
    mat4 model_mat = identity_mat4();

	glEnable(GL_DEPTH_TEST);  // enable depth-testing
	glDepthFunc(GL_LESS);     // interpret a smaller value as "closer"
	glEnable(GL_CULL_FACE);	  // enable face culling
	glCullFace(GL_BACK);	  // cull back face
	glFrontFace(GL_CW);      // set CCW vertex order to mean the front
	glClearColor(0.8, 0.8, 0.8, 1.0); // grey background
	//glViewport(0, 0, g_gl_width, g_gl_height);
    /* --- END RENDER SETTINGS --- */

    /* --- RENDER LOOP --- */
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

        // set shader program:
        glUseProgram(shader_program);

        // draw each sphere 
        /*
        for (int i=0; i<NUM_SPHERES; i++) {
            glUseProgram(programs[0]);
			glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mats[i].m);
			glDrawArrays(GL_TRIANGLES, 0, point_count);
		}*/
        model_mat.m[12] = sinf( current_seconds );
        glUniformMatrix4fv( model_mat_location, 1, GL_FALSE, model_mat.m );
        glDrawArrays( GL_TRIANGLES, 0, 3 );
        /*
        model_mat.m[12] = sinf( current_seconds );
		glUniformMatrix4fv( model_mat_location, 1, GL_FALSE, model_mat.m );
        */

        // bind VAO:
        glBindVertexArray(vao);

        // update other events (i.e. user inputs);
        glfwPollEvents();

    	// control keys
		bool cam_moved = false;
        vec3 move(0.0, 0.0, 0.0);
		float cam_yaw = 0.0f; // y-rotation in degrees
		float cam_pitch = 0.0f;
		float cam_roll = 0.0;
		if (glfwGetKey(g_window, GLFW_KEY_A)) {
			move.v[IDX_RT] -= CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_D)) {
			move.v[IDX_RT] += CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey(g_window, GLFW_KEY_Q)) {
			move.v[IDX_UP] += CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_E)) {
			move.v[IDX_UP] -= CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_W)) {
			move.v[IDX_FD] -= CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_S)) {
			move.v[IDX_FD] += CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_LEFT ) ) {
			cam_yaw += CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;

			// create a quaternion representing change in heading (the yaw)
			float q_yaw[4];
			create_versor(q_yaw, cam_yaw, up.v[0], up.v[1], up.v[2]);

			// add yaw rotation to the camera's current orientation
			mult_quat_quat(quaternion, q_yaw, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fd = R * vec4(DIR_FD);
			rt = R * vec4(DIR_RT);
			up = R * vec4(DIR_UP);
		}
		if (glfwGetKey(g_window, GLFW_KEY_RIGHT)) {
			cam_yaw -= CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;
			float q_yaw[4];
			create_versor(q_yaw, cam_yaw, up.v[0], up.v[1], up.v[2]);
			mult_quat_quat(quaternion, q_yaw, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fd = R * vec4(DIR_FD);
			rt = R * vec4(DIR_RT);
			up = R * vec4(DIR_UP);
		}
		if (glfwGetKey(g_window, GLFW_KEY_UP)) {
			cam_pitch += CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;
			float q_pitch[4];
			create_versor(q_pitch, cam_pitch, rt.v[0], rt.v[1], rt.v[2]);
			mult_quat_quat(quaternion, q_pitch, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fd = R * vec4(DIR_FD);
			rt = R * vec4(DIR_RT);
			up = R * vec4(DIR_UP);
		}
		if (glfwGetKey( g_window, GLFW_KEY_DOWN)) {
			cam_pitch -= CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;
			float q_pitch[4];
			create_versor(q_pitch, cam_pitch, rt.v[0], rt.v[1], rt.v[2]);
			mult_quat_quat(quaternion, q_pitch, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fd = R * vec4(DIR_FD);
			rt = R * vec4(DIR_RT);
			up = R * vec4(DIR_UP);
		}
		if ( glfwGetKey(g_window, GLFW_KEY_Z) ) {
			cam_roll -= CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;
			float q_roll[4];
			create_versor(q_roll, cam_roll, fd.v[0], fd.v[1], fd.v[2]);
			mult_quat_quat(quaternion, q_roll, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fd = R * vec4(DIR_FD);
			rt = R * vec4(DIR_RT);
			up = R * vec4(DIR_UP);
		}
		if ( glfwGetKey(g_window, GLFW_KEY_C) ) {
			cam_roll += CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;
			float q_roll[4];
			create_versor(q_roll, cam_roll, fd.v[0], fd.v[1], fd.v[2]);
			mult_quat_quat(quaternion, q_roll, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fd = R * vec4(DIR_FD);
			rt = R * vec4(DIR_RT);
			up = R * vec4(DIR_UP);
		}
		// update view matrix
		if (cam_moved) {
			quat_to_mat4(R.m, quaternion);

			// checking for fp errors
			//	printf ("dot fwd . up %f\n", dot (fwd, up));
			//	printf ("dot rgt . up %f\n", dot (rgt, up));
			//	printf ("dot fwd . rgt\n %f", dot (fwd, rgt));

			cam_pos = cam_pos + vec3(fd) * -move.v[IDX_FD];
			cam_pos = cam_pos + vec3(up) *  move.v[IDX_UP];
			cam_pos = cam_pos + vec3(rt) *  move.v[IDX_RT];
			mat4 T = translate( identity_mat4(), vec3(cam_pos));

			view_mat = inverse(R) * inverse(T);
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
		}

        /*
        if (camera.move_camera(g_window, elapsed_seconds)) {
            fprintf(stderr, " hmmm ");
            print(*view_mat);
            print(*proj_mat);
            glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat->m);
            glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat->m);
            fprintf(stderr, " mhhh" );
        }*/

        if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(g_window, 1);  // close window on esc press
        }

        // actually display the drawings:
        glfwSwapBuffers(g_window);
    } /* --- END RENDER LOOP --- */

    /* --- CLEAN UP --- */
    // close GL context and GLFW resources:
    glDeleteBuffers(1, &points_vbo);
    glDeleteBuffers(1, &normals_vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shader_program);
    glfwTerminate();

    delete vn;
    delete vp;
    delete vt;
    /* --- END CLEAN UP --- */

    return 0;
}

