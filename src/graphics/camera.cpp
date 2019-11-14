/*
 * camera.cpp
 */

#include <stdio.h>

#include "camera.hpp"

// Constructor with parameters:
Camera::Camera(float width, 
               float height,
               vec3 starting_pos, 
               mat4* view_mat, 
               mat4* proj_mat) {


    aspect = width/height; // aspect ratio
    if (proj_mat) { delete proj_mat; }
    proj_mat = new mat4(perspective(FOV_Y, aspect, CLIPPING_NEAR, CLIPPING_FAR));
    cam_pos = starting_pos;

    fd = vec4(DIR_FD);
    rt = vec4(DIR_RT);
    up = vec4(DIR_UP);

    // translation matrix:
    T = new mat4(translate(identity_mat4(),
                           vec3(-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2])));

    // rotation matrix:
    // make a quaternion representing negated initial camera orientation:
	create_versor(quaternion, -0.0f, 0.0f, 1.0f, 0.0f);
    R = new mat4();
    // convert the quaternion to a rotation matrix:
    quat_to_mat4(R->m, quaternion);

    // combine the inverse rotation and transformation to make a view matrix:
	//view_mat = R * T;
    if (view_mat) { delete view_mat; }
    view_mat = new mat4(look_at(cam_pos, vec3(0.0f, 0.0f, 0.0f), VEC3_UP));
}

Camera::~Camera() {
    if (view_mat) {
        delete view_mat;
    } 
    if (proj_mat) {
        delete proj_mat;
    } 
    if (R) {
        delete R;
    }
    if (T) {
        delete T;
    }
}

bool Camera::move_camera(GLFWwindow* g_window, double elapsed_seconds) {

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
		quat_to_mat4(R->m, quaternion);
		fd = (*R) * vec4(DIR_FD);
        rt = (*R) * vec4(DIR_RT);
		up = (*R) * vec4(DIR_UP);
	}
	if (glfwGetKey(g_window, GLFW_KEY_RIGHT)) {
        cam_yaw -= CAM_HEADING_SPEED * elapsed_seconds;
		cam_moved = true;
		float q_yaw[4];
		create_versor(q_yaw, cam_yaw, up.v[0], up.v[1], up.v[2]);
		mult_quat_quat(quaternion, q_yaw, quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4(R->m, quaternion);
		fd = (*R) * vec4(DIR_FD);
		rt = (*R) * vec4(DIR_RT);
		up = (*R) * vec4(DIR_UP);
	}
	if (glfwGetKey(g_window, GLFW_KEY_UP)) {
		cam_pitch += CAM_HEADING_SPEED * elapsed_seconds;
		cam_moved = true;
		float q_pitch[4];
		create_versor(q_pitch, cam_pitch, rt.v[0], rt.v[1], rt.v[2]);
		mult_quat_quat(quaternion, q_pitch, quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4(R->m, quaternion);
		fd = (*R) * vec4(DIR_FD);
		rt = (*R) * vec4(DIR_RT);
		up = (*R) * vec4(DIR_UP);
	}
	if (glfwGetKey( g_window, GLFW_KEY_DOWN)) {
		cam_pitch -= CAM_HEADING_SPEED * elapsed_seconds;
		cam_moved = true;
		float q_pitch[4];
		create_versor(q_pitch, cam_pitch, rt.v[0], rt.v[1], rt.v[2]);
		mult_quat_quat(quaternion, q_pitch, quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4(R->m, quaternion);
		fd = (*R) * vec4(DIR_FD);
		rt = (*R) * vec4(DIR_RT);
		up = (*R) * vec4(DIR_UP);
    }
	if ( glfwGetKey(g_window, GLFW_KEY_Z) ) {	
        cam_roll -= CAM_HEADING_SPEED * elapsed_seconds;
		cam_moved = true;
		float q_roll[4];
		create_versor(q_roll, cam_roll, fd.v[0], fd.v[1], fd.v[2]);
		mult_quat_quat(quaternion, q_roll, quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4(R->m, quaternion);
		fd = (*R) * vec4(DIR_FD);
		rt = (*R) * vec4(DIR_RT);
		up = (*R) * vec4(DIR_UP);
	}
	if ( glfwGetKey(g_window, GLFW_KEY_C) ) {
		cam_roll += CAM_HEADING_SPEED * elapsed_seconds;
		cam_moved = true;
		float q_roll[4];
		create_versor(q_roll, cam_roll, fd.v[0], fd.v[1], fd.v[2]);
		mult_quat_quat(quaternion, q_roll, quaternion);

		// recalc axes to suit new orientation
		quat_to_mat4(R->m, quaternion);
		fd = (*R) * vec4(DIR_FD);
		rt = (*R) * vec4(DIR_RT);
		up = (*R) * vec4(DIR_UP);
	}
	// update view matrix
	if (cam_moved) {
        fprintf(stderr, "cam moved\n");
		quat_to_mat4(R->m, quaternion);

        fprintf(stderr, "calc cam pos\n");

		// checking for fp errors
		//	printf ("dot fwd . up %f\n", dot (fwd, up));
		//	printf ("dot rgt . up %f\n", dot (rgt, up));
		//	printf ("dot fwd . rgt\n %f", dot (fwd, rgt));
        
		cam_pos = cam_pos + vec3(fd) * -move.v[2];
		cam_pos = cam_pos + vec3(up) * move.v[1];
		cam_pos = cam_pos + vec3(rt) * move.v[0];

        fprintf(stderr, "done calc campos\n");

		*T = translate( identity_mat4(), vec3(cam_pos));
        
        fprintf(stderr, "done calc T\n");

        delete view_mat;
		view_mat = new mat4(inverse(*R) * inverse(*T));
        fprintf(stderr, "end cam moved\n");
	}

    return cam_moved;
}

