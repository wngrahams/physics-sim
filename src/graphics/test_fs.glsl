#version 410

//uniform vec4 inputColor;  // uniform means its coming from the cpu
in float dist;
out vec4 frag_color;

void main() {
	//frag_color = vec4(color, 1.0);

    frag_colour = vec4 (1.0, 0.0, 0.0, 1.0);
	// use z position to shader darker to help perception of distance
	frag_colour.xyz *= dist;
}
