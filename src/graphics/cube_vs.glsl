#version 410

layout(location = 2) in vec3 vertex_pos;
layout(location = 3) in vec3 vertex_norm;
layout(location = 4) in vec2 in_color;

uniform mat4 model, view, proj;

out vec3 pos_eye, norm_eye, pass_color;

void main() {
    pos_eye = vec3 (view * model * vec4 (vertex_pos, 1.0));
    norm_eye = vec3 (view * model * vec4 (vertex_norm, 0.0));

    // position = proj * view * model (in that order):
    gl_Position = proj * vec4 (pos_eye, 1.0);

	//gl_Position = proj * view * model * vec4 (vertex_position, 1.0);
    pass_color = vec3(in_color, 0.0);
}

