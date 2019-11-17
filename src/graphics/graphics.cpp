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

#define CUBE_FILE "cube.obj"
#define PLANE_FILE "plane.obj"
#define NUM_CUBES 4
#define NUM_PLANES 2

#define NUM_SHADER_PROGRAMS 3

#define CAM_START_POS 0.0f, 8.0f, 8.0f

int main(int argv, char** argc) {

    GLuint points_vbo_plane, normals_vbo_plane;
    GLuint points_vbo_cube, normals_vbo_cube;
    GLuint vao;
    GLuint vs_ground, fs_ground;
    GLuint vs_grid, fs_grid;
    GLuint vs_cube, fs_cube;
    GLuint shader_program_ground, shader_program_grid, shader_program_cube;
    GLuint *shaders_ground, *shaders_grid, *shaders_cube;
    GLuint *programs;
	
    int model_mat_location_ground, 
        view_mat_location_ground, 
        proj_mat_location_ground;
    int model_mat_location_grid, view_mat_location_grid, proj_mat_location_grid;
    int model_mat_location_cube, view_mat_location_cube, proj_mat_location_cube;

    // camera vars:
    float aspect;
    float cam_heading = 0.0f;
    float quaternion[4];
    
    // keep track of some useful vectors that can be used for keyboard movement
	vec4 fd(DIR_FD);
	vec4 rt(DIR_RT);
	vec4 up(DIR_UP);

    // camera matricies:
    mat4 proj_mat, view_mat, T, R;
    vec3 cam_pos(CAM_START_POS);

    // a world position for each cube in the scene
    vec3 cube_pos_wor[] = {   vec3( 0.0, 0.0, 5.005 ),
                              vec3( 2.0, 0.0, 3.0 ),
							  vec3( -2.0, 0.0, 2.0 ), 
                              vec3( 1.5, 1.0, 1.0 ) };
    // world position for each plane
    vec3 plane_pos_wor[] = { vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.002) };

    // rectangle for testing indicies:
    GLfloat rectangle_points[] = {
                                -1.0, -1.0, 2.0,
                                1.0, -1.0, 2.0,
                                -1.0, 1.0, 2.0,
                                1.0, 1.0, 2.0,
                                -1.0, 1.0, -2.0,
                                1.0, -1.0, -2.0,
                                -1.0, -1.0, -2.0,
                                1.0, -1.0, -2.0 };
    GLfloat rectangle_normals[] = {
                                0.0, 0.0, 1.0,
                                1.0, 1.0, 0.0,
                                0.0, 0.0, -1.0,
                                0.0, -1.0, 0.0,
                                1.0, 0.0, 0.0,
                                -1.0, 0.0, 0.0 };
    GLshort rectangle_indicies[] = {
                                0, 1, 2, //0,
                                2, 1, 3, //0,
                                2, 3, 4, //1,
                                4, 3, 5, //1,
                                4, 5, 6, //2,
                                6, 5, 7, //2,
                                6, 7, 0, //3,
                                0, 7, 1, //3,
                                1, 7, 3, //4,
                                3, 7, 5, //4,
                                6, 0, 4, //5,
                                4, 0, 2};//, //5 };

    // END TRIANGLE
    
    // Create geometry (from file)
    GLfloat *vp_plane = NULL;  // array of vertex points
    GLfloat *vn_plane = NULL;  // array of vertex normals
    GLfloat *vt_plane = NULL;  // array of texture coordinates
	int point_count_plane = 0;

    GLfloat *vp_cube = NULL;
    GLfloat *vn_cube = NULL;
    GLfloat *vt_cube = NULL;
    int point_count_cube = 0;
	
    // restart log file:
    restart_gl_log();

    // initialize GLFW3 and GLEW
    start_gl(); 

    // only draw a pixel if the shape is closest to the viewer:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    load_obj_file(PLANE_FILE, vp_plane, vt_plane, vn_plane, point_count_plane);
    load_obj_file(CUBE_FILE,  vp_cube,  vt_cube,  vn_cube,  point_count_cube );

     
    // print all points for debugging
    for (int i=0; i<point_count_cube*3; i+=3) {
        printf("point %d: ", i/3);
        print(vec3(vp_cube[i], vp_cube[i+1], vp_cube[i+2]));
        printf("\n");
    }

    for (int i=0; i<point_count_cube*3; i+=3) {
        printf("normal %d: ", i/3);
        print(vec3(vn_cube[i], vn_cube[i+1], vn_cube[i+2]));
        printf("\n");
    }
    

	//GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    // load points into GPU using Vertex Buffer Object (vbo):
    if (NULL != vp_plane) {
		glGenBuffers(1 , &points_vbo_plane);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo_plane);
		glBufferData(GL_ARRAY_BUFFER, 
                     3*point_count_plane*sizeof(GLfloat), 
                     vp_plane,
					 GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	} 
    if (NULL != vn_plane) {
        glGenBuffers(1, &normals_vbo_plane);
        glBindBuffer(GL_ARRAY_BUFFER, normals_vbo_plane);
        glBufferData(GL_ARRAY_BUFFER, 
                     3*point_count_plane*sizeof(GLfloat),
                     vn_plane,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, NULL);
        glEnableVertexAttribArray(1);
    }
    
    if (NULL != vp_cube) {
        glGenBuffers(1 , &points_vbo_cube);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo_cube);
		glBufferData(GL_ARRAY_BUFFER, 
                     3*point_count_cube*sizeof(GLfloat), 
                     vp_cube,
					 GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);
	} 
    if (NULL != vn_cube) {
        glGenBuffers(1, &normals_vbo_cube);
        glBindBuffer(GL_ARRAY_BUFFER, normals_vbo_cube);
        glBufferData(GL_ARRAY_BUFFER, 
                     3*point_count_cube*sizeof(GLfloat),
                     vn_cube,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, 0, NULL);
        glEnableVertexAttribArray(3);
    }
    
    /* 
    GLuint verticies_vbo_rect;
    glGenBuffers(1, &verticies_vbo_rect);
    glBindBuffer(GL_ARRAY_BUFFER, verticies_vbo_rect);
    glBufferData(GL_ARRAY_BUFFER,
                    3*8*sizeof(GLfloat),
                    rectangle_points,
                    GL_STATIC_DRAW);
    GLuint indicies_vbo_rect;
    glGenBuffers(1, &indicies_vbo_rect);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies_vbo_rect);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                    sizeof(rectangle_indicies), rectangle_indicies,
                    GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    */

    // get shaders from files, compile, and link:
    vs_ground = compile_shader("ground_vs.glsl", GL_VERTEX_SHADER);
    vs_grid = compile_shader("grid_vs.glsl", GL_VERTEX_SHADER);
    fs_ground = compile_shader("ground_fs.glsl", GL_FRAGMENT_SHADER);
    fs_grid = compile_shader("grid_fs.glsl", GL_FRAGMENT_SHADER);
    vs_cube = compile_shader("cube_vs.glsl", GL_VERTEX_SHADER);
    fs_cube = compile_shader("cube_fs.glsl", GL_FRAGMENT_SHADER);

    shaders_ground = new GLuint[2];
    shaders_ground[0] = vs_ground;
    shaders_ground[1] = fs_ground;
    shader_program_ground = link_shaders(shaders_ground, 2);
    assert(program_is_valid(shader_program_ground));

    shaders_grid = new GLuint[2];
    shaders_grid[0] = vs_grid;
    shaders_grid[1] = fs_grid;
    shader_program_grid = link_shaders(shaders_grid, 2);
    assert(program_is_valid(shader_program_grid));

    shaders_cube = new GLuint[2];
    shaders_cube[0] = vs_cube;
    shaders_cube[1] = fs_cube;
    shader_program_cube = link_shaders(shaders_cube, 2);
    assert(program_is_valid(shader_program_cube));

    programs = new GLuint[NUM_SHADER_PROGRAMS];
    programs[0] = shader_program_ground;
    programs[1] = shader_program_grid;
    programs[2] = shader_program_cube;

    // get Uniform variable locations from shaders:
    model_mat_location_ground = glGetUniformLocation(shader_program_ground, 
                                                     "model");
    view_mat_location_ground  = glGetUniformLocation(shader_program_ground, 
                                                     "view");
    proj_mat_location_ground  = glGetUniformLocation(shader_program_ground, 
                                                     "proj");
    model_mat_location_grid   = glGetUniformLocation(shader_program_grid, 
                                                     "model");
    view_mat_location_grid    = glGetUniformLocation(shader_program_grid,
                                                     "view");
    proj_mat_location_grid    = glGetUniformLocation(shader_program_grid,
                                                     "proj");
    model_mat_location_cube   = glGetUniformLocation(shader_program_cube, 
                                                     "model");
    view_mat_location_cube    = glGetUniformLocation(shader_program_cube, 
                                                     "view");
    proj_mat_location_cube    = glGetUniformLocation(shader_program_cube, 
                                                     "proj");
    

    // free unneeded memory used when making shaders:
    glDeleteShader(vs_ground);
    glDeleteShader(fs_ground);
    glDeleteShader(vs_grid);
    glDeleteShader(fs_grid);
    glDeleteShader(vs_cube);
    glDeleteShader(fs_cube);
    delete shaders_ground;  
    delete shaders_grid;
    delete shaders_cube;

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
	//view_mat = R * T;
    view_mat = look_at(cam_pos, vec3(0.0f, 0.0f, 0.0f), up);
    /* --- END CAMERA SETUP -- */

    /* --- RENDER SETTINGS --- */
    glUseProgram(shader_program_ground);
	glUniformMatrix4fv(view_mat_location_ground, 1, GL_FALSE, view_mat.m);
	glUniformMatrix4fv(proj_mat_location_ground, 1, GL_FALSE, proj_mat.m);

    glUseProgram(shader_program_grid);
    glUniformMatrix4fv(view_mat_location_grid, 1, GL_FALSE, view_mat.m);
    glUniformMatrix4fv(proj_mat_location_grid, 1, GL_FALSE, proj_mat.m);

    glUseProgram(shader_program_cube);
    glUniformMatrix4fv(view_mat_location_cube, 1, GL_FALSE, view_mat.m);
    glUniformMatrix4fv(proj_mat_location_cube, 1, GL_FALSE, proj_mat.m);

	// unique model matrix for each plane
	mat4 model_mats_plane[NUM_PLANES];
	for (int i=0; i<NUM_PLANES; i++) {
		model_mats_plane[i] = translate(identity_mat4(), plane_pos_wor[i]);
	}
    // unique model matrix for each cube
    mat4 model_mats_cube[NUM_CUBES];
    for (int i=0; i<NUM_CUBES; i++) {
        model_mats_cube[i] = translate(identity_mat4(), cube_pos_wor[i]);
    }

	glEnable(GL_DEPTH_TEST);  // enable depth-testing
	glDepthFunc(GL_LESS);     // interpret a smaller value as "closer"
	//glEnable(GL_CULL_FACE);	  // enable face culling
	//glCullFace(GL_BACK);	  // cull back face
	glFrontFace(GL_CCW);      // set CCW vertex order to mean the front
	glClearColor(0.8, 0.8, 0.8, 1.0); // grey background
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

        // draw ground:
        glUseProgram(shader_program_ground);
        glUniformMatrix4fv(model_mat_location_ground,
                           1,
                           GL_FALSE,
                           model_mats_plane[0].m
                          );
        glDrawArrays(GL_TRIANGLES, 0, point_count_plane);

        // draw grid:
        glUseProgram(shader_program_grid);
        glUniformMatrix4fv(model_mat_location_grid,
                           1,
                           GL_FALSE,
                           model_mats_plane[1].m
                          );
        glDrawArrays(GL_LINE_LOOP, 0, point_count_plane);

        glUseProgram(shader_program_cube);
        // draw each cube:
        for (int i=0; i<NUM_CUBES; i++) {
            glUniformMatrix4fv(model_mat_location_cube,
                              1,
                              GL_FALSE,
                              model_mats_cube[i].m
                              );
            glDrawArrays(GL_TRIANGLES, 0, point_count_cube);
        }

        // bind VAO:
        glBindVertexArray(vao);

        // point 5
        vp_cube[15] += 0.001;
        vp_cube[16] += 0.001;
        vp_cube[17] += 0.001;

        //v1 = p3 - p5, v2 = p4 - p5
        vec3 p5 = vec3(vp_cube[15], vp_cube[16], vp_cube[17]);
        vec3 p4 = vec3(vp_cube[12], vp_cube[13], vp_cube[14]);
        vec3 p3 = vec3(vp_cube[9], vp_cube[10], vp_cube[11]);
        vec3 v1 = p3 - p5;
        vec3 v2 = p4 - p5;
        vec3 n = normalise(cross(v1, v2));
        vn_cube[15] = n.v[0];
        vn_cube[16] = n.v[1];
        vn_cube[17] = n.v[2];

        // point 7
        vp_cube[21] += 0.001;
        vp_cube[22] += 0.001;
        vp_cube[23] += 0.001;

        //v1 = p3 - p5, v2 = p4 - p5
        vec3 p7 = vec3(vp_cube[21], vp_cube[22], vp_cube[23]);
        vec3 p6 = vec3(vp_cube[18], vp_cube[19], vp_cube[20]);
        vec3 p8 = vec3(vp_cube[24], vp_cube[25], vp_cube[26]);
        v1 = p8 - p7;
        v2 = p6 - p7;
        n = normalise(cross(v1, v2));
        vn_cube[21] = n.v[0];
        vn_cube[22] = n.v[1];
        vn_cube[23] = n.v[2];

        // point 10
        vp_cube[30] += 0.001;
        vp_cube[31] += 0.001;
        vp_cube[32] += 0.001;

        vec3 p10 = vec3(vp_cube[30], vp_cube[31], vp_cube[32]);
        vec3 p9  = vec3(vp_cube[27], vp_cube[28], vp_cube[29]);
        vec3 p11 = vec3(vp_cube[33], vp_cube[34], vp_cube[35]);
        v1 = p9 - p10;
        v2 = p11 - p10;
        n = normalise(cross(v1, v2));
        vn_cube[30] = n.v[0];
        vn_cube[31] = n.v[1];
        vn_cube[32] = n.v[2];

        // point 26
        vp_cube[78] += 0.001;
        vp_cube[79] += 0.001;
        vp_cube[80] += 0.001;

        vec3 p26 = vec3(vp_cube[78], vp_cube[79], vp_cube[80]);
        vec3 p24 = vec3(vp_cube[72], vp_cube[73], vp_cube[74]);
        vec3 p25 = vec3(vp_cube[75], vp_cube[76], vp_cube[77]);
        v1 = p24 - p26;
        v2 = p25 - p26;
        n = normalise(cross(v1, v2));
        vn_cube[78] = n.v[0];
        vn_cube[79] = n.v[1];
        vn_cube[80] = n.v[2];

        // poin 27
        vp_cube[81] += 0.001;
        vp_cube[82] += 0.001;
        vp_cube[83] += 0.001;

        vec3 p27 = vec3(vp_cube[81], vp_cube[82], vp_cube[83]);
        vec3 p28 = vec3(vp_cube[84], vp_cube[85], vp_cube[86]);
        vec3 p29 = vec3(vp_cube[87], vp_cube[88], vp_cube[89]);
        v1 = p28 - p27;
        v2 = p29 - p27;
        n = normalise(cross(v1, v2));
        vn_cube[81] = n.v[0];
        vn_cube[82] = n.v[1];
        vn_cube[83] = n.v[2];

        
        //printf("

        
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo_cube);
        glBufferData(GL_ARRAY_BUFFER, 
                    3*point_count_cube*sizeof(GLfloat), 
                    vp_cube,
                    GL_DYNAMIC_DRAW);
                    



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
            
            glUseProgram(shader_program_ground);
			glUniformMatrix4fv(view_mat_location_ground, 1, GL_FALSE, view_mat.m);
            glUseProgram(shader_program_grid);
            glUniformMatrix4fv(view_mat_location_grid, 1, GL_FALSE, view_mat.m);
            glUseProgram(shader_program_cube);
            glUniformMatrix4fv(view_mat_location_cube, 1, GL_FALSE, view_mat.m);
		}

        if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(g_window, 1);  // close window on esc press
        }

        // actually display the drawings:
        glfwSwapBuffers(g_window);
    } /* --- END RENDER LOOP --- */

    /* --- CLEAN UP --- */
    // close GL context and GLFW resources: 
    glDeleteBuffers(1, &points_vbo_plane);
    glDeleteBuffers(1, &normals_vbo_plane);
    glDeleteBuffers(1, &points_vbo_cube);
    glDeleteBuffers(1, &normals_vbo_cube);
    glDeleteVertexArrays(1, &vao);
    for (int i=0; i<NUM_SHADER_PROGRAMS; i++) {
        glDeleteProgram(programs[i]);
    }
    delete programs;
    glfwTerminate();

    delete vn_plane;
    delete vp_plane;
    delete vt_plane;
    delete vn_cube;
    delete vp_cube;
    delete vt_cube;

    /* --- END CLEAN UP --- */

    return 0;
}

