#version 410

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_norm;
layout(location = 2) in vec3 ac;
layout(location = 3) in vec3 drc;

uniform mat4 model, view, proj;

out vec3 pos_eye, norm_eye, ambient_color, diffuse_reflectance_color;

void main() {
    pos_eye = vec3 (view * model * vec4 (vertex_pos, 1.0));
    norm_eye = vec3 (view * model * vec4 (vertex_norm, 0.0));

    // position = proj * view * model (in that order):
    gl_Position = proj * vec4 (pos_eye, 1.0);

	//gl_Position = proj * view * model * vec4 (vertex_position, 1.0);

    ambient_color = ac;
    diffuse_reflectance_color = drc;
}

