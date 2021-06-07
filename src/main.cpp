#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//#define VBOS_COUNT 1


const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "OpenGL study";
const int VBOS_COUNT = 1;
const int VAOS_COUNT = 1;
const int SHADER_INFO_LOG_SIZE = 512;

const char* vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 pos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
    "}\0";
const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";


char shaderInfoLog[SHADER_INFO_LOG_SIZE];
int isShaderCompiled;


void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Error while creating window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;

        glfwTerminate();
        return EXIT_FAILURE;
    }

    //vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // validate vertex shader in compile-time
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);
    if (!isShaderCompiled)
    {
        glGetShaderInfoLog(vertexShader, SHADER_INFO_LOG_SIZE, NULL, shaderInfoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << shaderInfoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // validate fragment shader in compile-time
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiled);
    if (!isShaderCompiled)
    {
        glGetShaderInfoLog(fragmentShader, SHADER_INFO_LOG_SIZE, NULL, shaderInfoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << shaderInfoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // validate linking shaders int compile-time
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isShaderCompiled);
    if (!isShaderCompiled)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, shaderInfoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::LINKING_FAILED\n" << shaderInfoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 

    // initializing:
    // vertex input
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    }; 

    unsigned int vbos[VBOS_COUNT] = {};
    unsigned int vaos[VAOS_COUNT] = {};
    glGenVertexArrays(1, vaos);
    glGenBuffers(VBOS_COUNT, vbos);

    glBindVertexArray(vbos[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        // rendering
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(vaos[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events & swap the buffer
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // de-allocation
    glDeleteVertexArrays(1, vaos);
    glDeleteBuffers(1, vbos);
    glDeleteProgram(shaderProgram);


    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}