/*
 * graphics.cpp
 *
 * functions to load up resources for OpenGL (and GLEW and glfw3), and to add
 * cubes as specified
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

#define CUBE_FILE "../../res/cube.obj"
#define PLANE_FILE "../../res/plane.obj"
#define NUM_PLANES 2

#define NUM_SHADER_PROGRAMS 3

#define CAM_START_POS 0.0f, -3.0f, 3.0f

int main(int argv, char** argc) {
//void graphics_init(const int num_cubes, const float* starting_positions) {

    int num_cubes = 2;

    /* ----------- VARIABLES ---------- */
    // vao vars:
    GLuint vao;

    // vbo vars:
    GLuint points_vbo_plane, normals_vbo_plane;
    GLuint *points_vbos_cube, *normals_vbos_cube;
    points_vbos_cube = new GLuint[num_cubes];
    normals_vbos_cube = new GLuint[num_cubes];

    // shader vars:
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
    float heading = 0.0f;
    float quaternion[4];
    //Cam_vars* camera_vars = new Cam_vars();
    
    // keep track of some useful vectors that can be used for keyboard movement
	vec4 fd(DIR_FD);
	vec4 rt(DIR_RT);
	vec4 up(DIR_UP);

    // camera matricies:
    mat4 proj_mat, view_mat, T, R;
    vec3 cam_pos(CAM_START_POS);

    // world position for each plane
    vec3 plane_pos_wor[] = { vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.002) };

    float starting_positions[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f };

    // get cube world positions from args:
    vec3 cube_pos_wor[num_cubes];
    for (int i=0; i<num_cubes*3; i+=3) {
        //cube_pos_wor[i] = vec3(0.0, 0.0, 0.0);
        cube_pos_wor[i] = vec3(starting_positions[i], 
                               starting_positions[i+1],
                               starting_positions[i+2]
                              );
    }
    /* ---------- END VARIABLES ---------- */

    /* ---------- SETUP AND LOAD ---------- */
    // setup GL environment:
    // restart log file:
    if (!restart_gl_log()) {
        fprintf(stderr, "Could not restart log file!\n");
    }

    // initialize GLFW3 and GLEW
    if (!start_gl()) {
        fprintf(stderr, "Could not setup GLFW3 and/or GLEW");
    }

    // only draw a pixel if the shape is closest to the viewer:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // load geometry from files:
    GLfloat *vp_plane = NULL;  // array of vertex points
    GLfloat *vn_plane = NULL;  // array of vertex normals
    GLfloat *vt_plane = NULL;  // array of texture coordinates
	int point_count_plane = 0;

    GLfloat *vp_cube = NULL;
    GLfloat *vn_cube = NULL;
    GLfloat *vt_cube = NULL;
    int point_count_cube = 0;

    load_obj_file(PLANE_FILE, vp_plane, vt_plane, vn_plane, point_count_plane);
    load_obj_file(CUBE_FILE,  vp_cube,  vt_cube,  vn_cube,  point_count_cube );

    // copy points so each cube has its own version:
    GLfloat **vp_cubes = new GLfloat*[num_cubes];
    GLfloat **vn_cubes = new GLfloat*[num_cubes];
    for (int i=0; i<num_cubes; i++) {
        vp_cubes[i] = new GLfloat[3 * point_count_cube];
        vn_cubes[i] = new GLfloat[3 * point_count_cube];
        for (int j=0; j<(3*point_count_cube); j++) {
            vp_cubes[i][j] = vp_cube[j];
            vn_cubes[i][j] = vn_cube[j];
        }
    }
    /* ---------- END SETUP AND LOAD ---------- */

    /* ---------- VAO & VBO ---------- */
    // setup VAO:
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    // send points to GPU using VBO:
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

    // allocate array of VBOs for cubes:
    for (int i=0; i<num_cubes; i++) {
        if (NULL != vp_cubes[i]) {
            glGenBuffers(1 , &(points_vbos_cube[i]));
		    glBindBuffer(GL_ARRAY_BUFFER, points_vbos_cube[i]);
		    glBufferData(GL_ARRAY_BUFFER, 
                         3*point_count_cube*sizeof(GLfloat), 
                         vp_cubes[i],
		    			 GL_DYNAMIC_DRAW
                        );
		    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		    glEnableVertexAttribArray(2);
	    } 
        if (NULL != vn_cubes[i]) {
            glGenBuffers(1, &(normals_vbos_cube[i]));
            glBindBuffer(GL_ARRAY_BUFFER, normals_vbos_cube[i]);
            glBufferData(GL_ARRAY_BUFFER, 
                         3*point_count_cube*sizeof(GLfloat),
                         vn_cubes[i],
                         GL_DYNAMIC_DRAW
                        );
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, 0, NULL);
            glEnableVertexAttribArray(3);
        }
    }
    /* ---------- END VAO & VBO ---------- */

    /* ---------- SHADERS ---------- */
    // compile and link shaders:
    vs_ground = compile_shader("ground_vs.glsl", GL_VERTEX_SHADER);
    fs_ground = compile_shader("ground_fs.glsl", GL_FRAGMENT_SHADER);
    vs_grid = compile_shader("grid_vs.glsl", GL_VERTEX_SHADER);
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

    // free memory that is no longer needed:
    glDeleteShader(vs_ground);
    glDeleteShader(fs_ground);
    glDeleteShader(vs_grid);
    glDeleteShader(fs_grid);
    glDeleteShader(vs_cube);
    glDeleteShader(fs_cube);
    delete shaders_ground;  
    delete shaders_grid;
    delete shaders_cube;
    /* ---------- END SHADERS ---------- */

    /* ---------- CAMERA SETUP ---------- */
    aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
    proj_mat = perspective(FOV_Y, aspect, CLIPPING_NEAR, CLIPPING_FAR);

    // translation matrix:
    T = translate(identity_mat4(),
				  vec3(-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2]));

    // rotation matrix:
    // make a quaternion representing negated initial camera orientation:
	create_versor(quaternion, -heading, 0.0f, 1.0f, 0.0f);
    // convert the quaternion to a rotation matrix:
    quat_to_mat4(R.m, quaternion);

    // combine the inverse rotation and transformation to make a view matrix:
	//view_mat = R * T;
    view_mat = look_at(cam_pos, vec3(0.0f, 0.0f, 0.0f), up);
    /* ---------- END CAMERA SETUP ---------- */

    /* ---------- RENDER SETTINGS ---------- */
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
    mat4 model_mats_cube[num_cubes];
    for (int i=0; i<num_cubes; i++) {
        model_mats_cube[i] = translate(identity_mat4(), cube_pos_wor[i]);
    }

    glEnable(GL_DEPTH_TEST);  // enable depth-testing
    glDepthFunc(GL_LESS);     // interpret a smaller value as "closer"
    glEnable(GL_CULL_FACE);	  // enable face culling
    glCullFace(GL_BACK);	  // cull back face
    glFrontFace(GL_CCW);      // set CCW vertex order to mean the front
    glClearColor(0.8, 0.8, 0.8, 1.0); // grey background
    /* ---------- END RENDER SETTINGS ---------- */
//}

//void graphics_draw() {
    while (!glfwWindowShouldClose(g_window)) {

        // update timers
        static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;
		update_fps_counter(g_window);

        // clear drawing surface:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw ground:
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo_plane);
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

        //glUseProgram(shader_program_cube);
        // draw each cube:
        for (int i=0; i<num_cubes; i++) {
            glBindBuffer(GL_ARRAY_BUFFER, points_vbos_cube[i]);
            glUseProgram(shader_program_cube);
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
        vp_cubes[1][15] += 0.001;
        vp_cubes[1][16] += 0.001;
        vp_cubes[1][17] += 0.001;

        //v1 = p3 - p5, v2 = p4 - p5
        vec3 p5 = vec3(vp_cubes[1][15], vp_cubes[1][16], vp_cubes[1][17]);
        vec3 p4 = vec3(vp_cubes[1][12], vp_cubes[1][13], vp_cubes[1][14]);
        vec3 p3 = vec3(vp_cubes[1][9], vp_cubes[1][10], vp_cubes[1][11]);
        vec3 v1 = p3 - p5;
        vec3 v2 = p4 - p5;
        vec3 n = normalise(cross(v1, v2));
        vn_cubes[1][15] = n.v[0];
        vn_cubes[1][16] = n.v[1];
        vn_cubes[1][17] = n.v[2];

        // point 7
        vp_cubes[1][21] += 0.001;
        vp_cubes[1][22] += 0.001;
        vp_cubes[1][23] += 0.001;

        //v1 = p3 - p5, v2 = p4 - p5
        vec3 p7 = vec3(vp_cubes[1][21], vp_cubes[1][22], vp_cubes[1][23]);
        vec3 p6 = vec3(vp_cubes[1][18], vp_cubes[1][19], vp_cubes[1][20]);
        vec3 p8 = vec3(vp_cubes[1][24], vp_cubes[1][25], vp_cubes[1][26]);
        v1 = p8 - p7;
        v2 = p6 - p7;
        n = normalise(cross(v1, v2));
        vn_cubes[1][21] = n.v[0];
        vn_cubes[1][22] = n.v[1];
        vn_cubes[1][23] = n.v[2];

        // point 10
        vp_cubes[1][30] += 0.001;
        vp_cubes[1][31] += 0.001;
        vp_cubes[1][32] += 0.001;

        vec3 p10 = vec3(vp_cubes[1][30], vp_cubes[1][31], vp_cubes[1][32]);
        vec3 p9  = vec3(vp_cubes[1][27], vp_cubes[1][28], vp_cubes[1][29]);
        vec3 p11 = vec3(vp_cubes[1][33], vp_cubes[1][34], vp_cubes[1][35]);
        v1 = p9 - p10;
        v2 = p11 - p10;
        n = normalise(cross(v1, v2));
        vn_cubes[1][30] = n.v[0];
        vn_cubes[1][31] = n.v[1];
        vn_cubes[1][32] = n.v[2];

        // point 26
        vp_cubes[1][78] += 0.001;
        vp_cubes[1][79] += 0.001;
        vp_cubes[1][80] += 0.001;

        vec3 p26 = vec3(vp_cubes[1][78], vp_cubes[1][79], vp_cubes[1][80]);
        vec3 p24 = vec3(vp_cubes[1][72], vp_cubes[1][73], vp_cubes[1][74]);
        vec3 p25 = vec3(vp_cubes[1][75], vp_cubes[1][76], vp_cubes[1][77]);
        v1 = p24 - p26;
        v2 = p25 - p26;
        n = normalise(cross(v1, v2));
        vn_cubes[1][78] = n.v[0];
        vn_cubes[1][79] = n.v[1];
        vn_cubes[1][80] = n.v[2];

        // poin 27
        vp_cubes[1][81] += 0.001;
        vp_cubes[1][82] += 0.001;
        vp_cubes[1][83] += 0.001;

        vec3 p27 = vec3(vp_cubes[1][81], vp_cubes[1][82], vp_cubes[1][83]);
        vec3 p28 = vec3(vp_cubes[1][84], vp_cubes[1][85], vp_cubes[1][86]);
        vec3 p29 = vec3(vp_cubes[1][87], vp_cubes[1][88], vp_cubes[1][89]);
        v1 = p28 - p27;
        v2 = p29 - p27;
        n = normalise(cross(v1, v2));
        vn_cubes[1][81] = n.v[0];
        vn_cubes[1][82] = n.v[1];
        vn_cubes[1][83] = n.v[2];
        
        glBindBuffer(GL_ARRAY_BUFFER, points_vbos_cube[1]);
        glBufferData(GL_ARRAY_BUFFER, 
                     3*point_count_cube*sizeof(GLfloat), 
                     vp_cubes[1],
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
    }
//}

//void graphics_cleanup() {
    // close GL context and GLFW resources: 
    glDeleteBuffers(1, &points_vbo_plane);
    glDeleteBuffers(1, &normals_vbo_plane);
    for (int i=0; i<num_cubes; i++) {
        glDeleteBuffers(1, &(points_vbos_cube[i]));
        glDeleteBuffers(1, &(normals_vbos_cube[i]));
    }
    delete [] points_vbos_cube;
    delete [] normals_vbos_cube;
    //glDeleteBuffers(1, &points_vbo_cube);
    //glDeleteBuffers(1, &normals_vbo_cube);
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

    for (int i=0; i<num_cubes; i++) {
        delete [] vp_cubes[i];
        delete [] vn_cubes[i];
    }

    delete [] vp_cubes;
    delete [] vn_cubes;
}
