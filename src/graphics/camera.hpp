/*
 * camera.hpp
 *
 * header file for camera functions
 */

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gl-utils.h"
#include "maths_funcs.hpp"

#define CLIPPING_NEAR     (0.1f)
#define CLIPPING_FAR      (1000.0f)
#define FOV_Y             (67.0f)
#define CAM_SPEED         (10000.0f)    // 5 = 1 unit per second
#define CAM_HEADING_SPEED (80000.0f)  // 100 = 30 degrees per second

class Camera {
   
public:
    mat4 *view_mat, *proj_mat;

    // constructor with parameters
    Camera(float, float, const vec3, mat4*, mat4*);
    ~Camera();

    bool move_camera(GLFWwindow*, double);

private:

    vec3 cam_pos;
    vec4 fd, rt, up;
    float aspect;
    mat4 *T, *R;
    float quaternion[4];

};

#endif

