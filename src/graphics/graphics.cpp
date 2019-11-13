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
#include "graphics.h"
#include "maths_funcs.hpp"
#include "obj_parser.hpp"

#define MESH_FILE "sphere.obj"
#define NUM_SPHERES 4

#define CLIPPING_NEAR     (0.1f)
#define CLIPPING_FAR      (1000.0f)
#define FOV_Y             (67.0f)
#define CAM_SPEED         (10000.0f)    // 5 = 1 unit per second
#define CAM_HEADING_SPEED (80000.0f)  // 100 = 30 degrees per second
#define CAM_START_POS     0.0f, 0.0f, 5.0f


/* create a unit quaternion q from an angle in degrees a, and an axis x,y,z */
void create_versor( float *q, float degrees, float x, float y, float z ) {
	float rad = ONE_DEG_IN_RAD * degrees;
	q[0] = cosf( rad / 2.0f );
	q[1] = sinf( rad / 2.0f ) * x;
	q[2] = sinf( rad / 2.0f ) * y;
	q[3] = sinf( rad / 2.0f ) * z;
}

/* convert a unit quaternion q to a 4x4 matrix m */
void quat_to_mat4( float *m, const float *q ) {
	float w = q[0];
	float x = q[1];
	float y = q[2];
	float z = q[3];
	m[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
	m[1] = 2.0f * x * y + 2.0f * w * z;
	m[2] = 2.0f * x * z - 2.0f * w * y;
	m[3] = 0.0f;
	m[4] = 2.0f * x * y - 2.0f * w * z;
	m[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
	m[6] = 2.0f * y * z + 2.0f * w * x;
	m[7] = 0.0f;
	m[8] = 2.0f * x * z + 2.0f * w * y;
	m[9] = 2.0f * y * z - 2.0f * w * x;
	m[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

/* normalise a quaternion in case it got a bit mangled */
void normalise_quat( float *q ) {
	// norm(q) = q / magnitude (q)
	// magnitude (q) = sqrt (w*w + x*x...)
	// only compute sqrt if interior sum != 1.0
	float sum = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
	// NB: floats have min 6 digits of precision
	const float thresh = 0.0001f;
	if ( fabs( 1.0f - sum ) < thresh ) {
		return;
	}
	float mag = sqrt( sum );
	for ( int i = 0; i < 4; i++ ) {
		q[i] = q[i] / mag;
	}
}

/* multiply quaternions to get another one. result=R*S */
/* will need to normalize after */
void mult_quat_quat( float *result, const float *r, const float *s ) {
	float w = s[0] * r[0] - s[1] * r[1] - s[2] * r[2] - s[3] * r[3];
	float x = s[0] * r[1] + s[1] * r[0] - s[2] * r[3] + s[3] * r[2];
	float y = s[0] * r[2] + s[1] * r[3] + s[2] * r[0] - s[3] * r[1];
	float z = s[0] * r[3] - s[1] * r[2] + s[2] * r[1] + s[3] * r[0];
	result[0] = w;
	result[1] = x;
	result[2] = y;
    result[3] = z;
    normalise_quat(result);
}

int main(int argv, char** argc) {

    GLuint points_vbo;
    GLuint vao;
	GLuint vs, fs, shader_program;
    GLuint* shaders;
	
    int model_mat_location, view_mat_location, proj_mat_location;

    // camera vars:
    float aspect;
    float cam_heading = 0.0f;
    float quaternion[4];

    // keep track of some useful vectors that can be used for keyboard movement
	vec4 fwd( 0.0f, 0.0f, -1.0f, 0.0f );
	vec4 rgt( 1.0f, 0.0f, 0.0f, 0.0f );
	vec4 up( 0.0f, 1.0f, 0.0f, 0.0f );

    // camera matricies:
    mat4 view_mat, proj_mat, T, R;
    vec3 cam_pos(CAM_START_POS);
    // a world position for each sphere in the scene
    vec3 sphere_pos_wor[] = { vec3( -2.0, 0.0, 0.0 ),
                              vec3( 2.0, 0.0, 0.0 ),
							  vec3( -2.0, 0.0, -2.0 ), 
                              vec3( 1.5, 1.0, -1.0 ) };
    
    // Create geometry (from file)
    GLfloat *vp = NULL;  // array of vertex points
	GLfloat *vn = NULL;  // array of vertex normals
	GLfloat *vt = NULL;  // array of texture coordinates
	int point_count = 0;
	load_obj_file(MESH_FILE, vp, vt, vn, point_count);

    // restart log file:
    restart_gl_log();

    // initialize GLFW3 and GLEW
    start_gl(); 

    // only draw a pixel if the shape is closest to the viewer:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // generate vertex attribute object (vao):
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    // load points into GPU using Vertex Buffer Object (vbo):
    if (NULL != vp) {
		glGenBuffers(1 , &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, 3
                     *point_count*sizeof(GLfloat), 
                     vp,
					 GL_STATIC_DRAW );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 0 );
	} 

    // get shaders from files, compile, and link:
    vs = compile_shader("test_vs.glsl", GL_VERTEX_SHADER);
    fs = compile_shader("test_fs.glsl", GL_FRAGMENT_SHADER);
    shaders = new GLuint[2];
    shaders[0] = vs;
    shaders[1] = fs;
    shader_program = link_shaders(shaders, 2);
    assert(program_is_valid(shader_program));

    // get Uniform variable locations from shaders:
    model_mat_location = glGetUniformLocation(shader_program, "model");
    view_mat_location = glGetUniformLocation(shader_program, "view");
    proj_mat_location = glGetUniformLocation(shader_program, "proj");
    
    // free unneeded memory used when making shaders:
    glDeleteShader(vs);
    glDeleteShader(fs);
    delete shaders;  

    /* --- CAMERA SETUP --- */
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
	view_mat = R * T;
    /* --- END CAMERA SETUP -- */

    /* --- RENDER SETTINGS --- */
    glUseProgram(shader_program);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat.m);

	// unique model matrix for each sphere
	mat4 model_mats[NUM_SPHERES];
	for (int i=0; i<NUM_SPHERES; i++) {
		model_mats[i] = translate(identity_mat4(), sphere_pos_wor[i]);
	}

	glEnable(GL_DEPTH_TEST);  // enable depth-testing
	glDepthFunc(GL_LESS);     // interpret a smaller value as "closer"
	glEnable(GL_CULL_FACE);	  // enable face culling
	glCullFace(GL_BACK);	  // cull back face
	glFrontFace(GL_CCW);      // set CCW vertex order to mean the front
	glClearColor(0.2, 0.2, 0.2, 1.0); // grey background
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
        for (int i=0; i<NUM_SPHERES; i++) {
			glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mats[i].m);
			glDrawArrays(GL_TRIANGLES, 0, point_count);
		}

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
			move.v[0] -= CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_D)) {
			move.v[0] += CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey(g_window, GLFW_KEY_Q)) {
			move.v[1] += CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_E)) {
			move.v[1] -= CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_W)) {
			move.v[2] -= CAM_SPEED * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_S)) {
			move.v[2] += CAM_SPEED * elapsed_seconds;
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
			fwd = R * vec4( 0.0, 0.0, -1.0, 0.0 );
			rgt = R * vec4( 1.0, 0.0, 0.0, 0.0 );
			up = R * vec4( 0.0, 1.0, 0.0, 0.0 );
		}
		if (glfwGetKey(g_window, GLFW_KEY_RIGHT)) {
			cam_yaw -= CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;
			float q_yaw[4];
			create_versor(q_yaw, cam_yaw, up.v[0], up.v[1], up.v[2]);
			mult_quat_quat(quaternion, q_yaw, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fwd = R * vec4( 0.0, 0.0, -1.0, 0.0 );
			rgt = R * vec4( 1.0, 0.0, 0.0, 0.0 );
			up = R * vec4( 0.0, 1.0, 0.0, 0.0 );
		}
		if (glfwGetKey(g_window, GLFW_KEY_UP)) {
			cam_pitch += CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;
			float q_pitch[4];
			create_versor(q_pitch, cam_pitch, rgt.v[0], rgt.v[1], rgt.v[2]);
			mult_quat_quat(quaternion, q_pitch, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fwd = R * vec4( 0.0, 0.0, -1.0, 0.0 );
			rgt = R * vec4( 1.0, 0.0, 0.0, 0.0 );
			up = R * vec4( 0.0, 1.0, 0.0, 0.0 );
		}
		if (glfwGetKey( g_window, GLFW_KEY_DOWN)) {
			cam_pitch -= CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;
			float q_pitch[4];
			create_versor(q_pitch, cam_pitch, rgt.v[0], rgt.v[1], rgt.v[2]);
			mult_quat_quat(quaternion, q_pitch, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fwd = R * vec4( 0.0, 0.0, -1.0, 0.0 );
			rgt = R * vec4( 1.0, 0.0, 0.0, 0.0 );
			up = R * vec4( 0.0, 1.0, 0.0, 0.0 );
		}
		if ( glfwGetKey(g_window, GLFW_KEY_Z) ) {
			cam_roll -= CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;
			float q_roll[4];
			create_versor(q_roll, cam_roll, fwd.v[0], fwd.v[1], fwd.v[2]);
			mult_quat_quat(quaternion, q_roll, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fwd = R * vec4( 0.0, 0.0, -1.0, 0.0 );
			rgt = R * vec4( 1.0, 0.0, 0.0, 0.0 );
			up = R * vec4( 0.0, 1.0, 0.0, 0.0 );
		}
		if ( glfwGetKey(g_window, GLFW_KEY_C) ) {
			cam_roll += CAM_HEADING_SPEED * elapsed_seconds;
			cam_moved = true;
			float q_roll[4];
			create_versor(q_roll, cam_roll, fwd.v[0], fwd.v[1], fwd.v[2]);
			mult_quat_quat(quaternion, q_roll, quaternion);

			// recalc axes to suit new orientation
			quat_to_mat4(R.m, quaternion);
			fwd = R * vec4( 0.0, 0.0, -1.0, 0.0 );
			rgt = R * vec4( 1.0, 0.0, 0.0, 0.0 );
			up = R * vec4( 0.0, 1.0, 0.0, 0.0 );
		}
		// update view matrix
		if (cam_moved) {
			quat_to_mat4(R.m, quaternion);

			// checking for fp errors
			//	printf ("dot fwd . up %f\n", dot (fwd, up));
			//	printf ("dot rgt . up %f\n", dot (rgt, up));
			//	printf ("dot fwd . rgt\n %f", dot (fwd, rgt));

			cam_pos = cam_pos + vec3(fwd) * -move.v[2];
			cam_pos = cam_pos + vec3(up) * move.v[1];
			cam_pos = cam_pos + vec3(rgt) * move.v[0];
			mat4 T = translate( identity_mat4(), vec3(cam_pos));

			view_mat = inverse(R) * inverse(T);
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
		}

        if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(g_window, 1);  // close window on esc press
        }

        // actually display the drawings:
        glfwSwapBuffers(g_window);
    } /* --- END RENDER LOOP --- */

    /* --- CLEAN UP --- */
    // close GL context and GLFW resources:
    glDeleteBuffers(1, &points_vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shader_program);
    glfwTerminate();

    delete vn;
    delete vp;
    delete vt;
    /* --- END CLEAN UP --- */

    return 0;
}


