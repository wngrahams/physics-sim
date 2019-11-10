/* 
 * hellotri.c
 * 
 * just following tutorial for basic triangle
 */

#include <GL/glew.h>  // include GLEW
#define GLFW_DLL
#include <GLFW/glfw3.h>  // include GLFW helper library

#include <fstream>
#include <iostream>
#include <stdio.h>

#define GLSL_LOG_FILE "glsl.log"

#define FRAGMENT_SHADER_FILE "./test_fs.glsl"
#define VERTEX_SHADER_FILE "./test_vs.glsl"

int main() {
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    } 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if (!window) {
    	fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
                                  
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    /* OTHER STUFF GOES HERE NEXT */
  
    GLfloat points[] = {
        1.0f,  1.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        0.0f,  0.0f,  0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f,  0.0f
    };

    GLfloat points2[] = {
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f
    };

	// load points into GPU using Vertex Buffer Object (VBO)
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), points, GL_STATIC_DRAW);

	// Generate a Vertex Array Object for our triangle
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // vertex shader: describes where the 3d points should end up on the display
    // read in vertex shader from file:
    // something is wrong here, not sure what
    /*
    std::ifstream is_vs;
    int len_vs;
    char* buf_vs;
    is_vs.open(VERTEX_SHADER_FILE);  // open vs file
    is_vs.seekg(0, std::ios_base::end);  // seek to the end
    len_vs = is_vs.tellg();  // get length of file
    is_vs.seekg(0, std::ios_base::beg);  // seek to the beginning
    buf_vs = new char[len_vs];  // allocate char array 
    is_vs.read(buf_vs, len_vs);
    is_vs.close();*/

    const char* vertex_shader =
            "#version 330\n"
            "in vec3 vp;"
            "void main() {"
            "  gl_Position = vec4(vp.x, vp.y, vp.z, 1.0);"
            "}";

    // fragments shader sets the color of each fragment
    // read in fragment shader from file:
    // something is wrong here, not sure what
    /*
    std::ifstream is_fs;
    int len_fs;
    char* buf_fs;
    is_fs.open(FRAGMENT_SHADER_FILE);  // open fs file
    is_fs.seekg(0, std::ios_base::end);  // seek to the end
    len_fs = is_fs.tellg();  // get length of file
    is_fs.seekg(0, std::ios_base::beg);  // seek to the beginning
    buf_fs = new char[len_fs];  // allocate char array 
    is_fs.read(buf_fs, len_fs);
    is_fs.close();
    std::cout << buf_fs << std::endl;*/
    const char* fragment_shader =
            "#version 400\n"
            "out vec4 frag_colour;"
            "void main() {"
            "  frag_colour = vec4(0.5, 0, 0, 0.1);"
            "}";
    std::cout << std::endl;
    std::cout << fragment_shader << std::endl;

    // load and compile vertex shader and fragment shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    // combine shaders into GPU shader programme
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    // char arrays are no longer needed, free them:
    //delete buf_vs;
    //delete buf_fs;

    // continually draw until window is closed
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);  // set background color
    while(!glfwWindowShouldClose(window)) {
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_programme);
        glBindVertexArray(vao);

        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // update other events like input handling 
        glfwPollEvents();
        
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }

    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

bool gl_log(const char* filename, const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(filename, "a");
    if(!file) {
        fprintf(
        stderr,
        "ERROR: could not open GLSL log file %s for appending\n",
        filename
        );
        return false;
    }
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

bool gl_log_err(const char* filename, const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(filename, "a");
    if(!file) {
        fprintf(stderr,
        "ERROR: could not open GLSL log file %s for appending\n",
        filename);
        return false;
    }
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

bool restart_glsl_log(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "ERROR: Could not open GLSL Log file: %s\n", filename);
        return false;
    }

    time_t now = time(NULL);
    char* date = ctime(&now);
    fprintf(file, "GLSL log. local time %s\n", date);
    fclose(file);
    return true;
}

