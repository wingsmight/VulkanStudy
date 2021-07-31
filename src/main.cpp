#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <wingsmight/shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "boost/filesystem.hpp"
#include "boost/dll.hpp"

#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <cstring>

using namespace std;
namespace boostfs = boost::filesystem;

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
const char *WINDOW_TITLE = "OpenGL study";
const int VBOS_COUNT = 2;
const int VAOS_COUNT = 2;
const int EBOS_COUNT = 1;
const int INDICES_COUNT = 6;

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int createTexture(const char *imageFileName);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, NULL, NULL);
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

    // init shaders
    boost::filesystem::path vertexShaderPath("shaders/simple.vs");
    boost::filesystem::path absoluteVertexShaderPath = boost::dll::program_location().parent_path() / vertexShaderPath;
    const char *absoluteVertexShaderName = absoluteVertexShaderPath.string().c_str();

    boost::filesystem::path fragmentShaderPath("shaders/simple.fs");
    boost::filesystem::path absoluteFragmentShaderPath = boost::dll::program_location().parent_path() / fragmentShaderPath;
    const char *absoluteFragmentShaderName = absoluteFragmentShaderPath.string().c_str();

    Shader shader(absoluteVertexShaderName, absoluteFragmentShaderName);

    // initializing:
    // vertex input
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[INDICES_COUNT] = {
        0,
        1,
        3,
        1,
        2,
        3,
    };

    unsigned int vaos[VAOS_COUNT] = {};
    unsigned int ebos[EBOS_COUNT] = {};
    unsigned int vbos[VBOS_COUNT] = {};

    glGenVertexArrays(VAOS_COUNT, vaos);
    glGenBuffers(EBOS_COUNT, ebos);
    glGenBuffers(VBOS_COUNT, vbos);

    glBindVertexArray(vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // // create texture IDs
    boost::filesystem::path texture0Path("textures/moon.jpg");
    boost::filesystem::path absoluteTexture0Path = boost::dll::program_location().parent_path() / texture0Path;
    const char *absoluteTexture0Name = absoluteTexture0Path.string().c_str();
    unsigned int texture0 = createTexture(absoluteTexture0Name);

    boost::filesystem::path texture1Path("textures/englishText.png");
    boost::filesystem::path absoluteTexture1Path = boost::dll::program_location().parent_path() / texture1Path;
    const char *absoluteTexture1Name = absoluteTexture1Path.string().c_str();
    unsigned int texture1 = createTexture(absoluteTexture1Name);

    shader.use();
    shader.setInt("textureSampler0", 0);
    shader.setInt("textureSampler1", 1);

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // draw triangle
        shader.use();
        glBindVertexArray(ebos[0]);
        glDrawElements(GL_TRIANGLES, INDICES_COUNT, GL_UNSIGNED_INT, 0);

        // check and call events & swap the buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocation
    glDeleteVertexArrays(VAOS_COUNT, vaos);
    glDeleteBuffers(EBOS_COUNT, ebos);
    glDeleteBuffers(VBOS_COUNT, vbos);

    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
unsigned int createTexture(const char *imageFileName)
{
    string imageFile(imageFileName);
    //imageFileName.copy()
    cout << imageFileName << endl;

    // create texture ID
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    // load image 0
    int imageWidth, imageHeight, nrChannels;
    unsigned char *imageData = stbi_load(imageFileName, &imageWidth, &imageHeight, &nrChannels, 0);
    if (imageData)
    {
        GLint textureImageFormat;
        if (imageFile.find(".png") != string::npos)
        {
            textureImageFormat = GL_RGBA;
        }
        else
        {
            textureImageFormat = GL_RGB;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, textureImageFormat, imageWidth, imageHeight, 0, textureImageFormat, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(imageData);

    return texture;
}