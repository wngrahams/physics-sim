#version 410

//uniform vec4 inputColor;  // uniform means its coming from the cpu
in vec3 color;
out vec4 frag_color;

void main() {
	frag_color = vec4(color, 1.0);
}
