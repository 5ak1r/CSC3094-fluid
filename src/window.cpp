#include "../include/glad.h"
#include "../include/window.hpp"
#include "../include/shader.hpp"
#include "../include/texture.hpp"
#include "../include/camera.hpp"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
float lastX = (float)WIDTH / 2;
float lastY = (float)HEIGHT / 2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// fixes window whenever the window size is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// monitor mouse movements
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// zooming
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// query GLFW when relevant keys are pressed
void processInput(GLFWwindow *window)
{
    const float cameraSpeed = 2.5f * deltaTime;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // movement
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

int main() {

    // GLFW init and config
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // GLFW create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL) {
        std::cout << "ERROR::GLFW::WINDOW::FAILED_INIT" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);   
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD init, load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERROR::GLAD::FAILED_INIT" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    
    stbi_set_flip_vertically_on_load(true);

    Shader orangeTriangle("src/shaders/vertex/vertex.vs", "src/shaders/fragment/orange.fs");
    Shader yellowTriangle("src/shaders/vertex/vertex.vs","src/shaders/fragment/yellow.fs");
    Shader changingTriangle("src/shaders/vertex/vertex.vs", "src/shaders/fragment/change.fs");
    Shader verticesTriangle("src/shaders/vertex/verticesColor.vs", "src/shaders/fragment/verticesColor.fs");
    Shader boxTriangles("src/shaders/vertex/boxVertex.vs", "src/shaders/fragment/boxFragment.fs");
    Shader nonRainbowBoxTriangles("src/shaders/vertex/nonRainbowBoxVertex.vs", "src/shaders/fragment/nonRainbowBoxFragment.fs");
    Shader lightingBoxTriangles("src/shaders/vertex/lighting.vs", "src/shaders/fragment/lighting.fs");
    Shader litObjectTriangles("src/shaders/vertex/litObject.vs", "src/shaders/fragment/litObject.fs");
    Shader litWoodenBoxTriangles("src/shaders/vertex/litWoodenBox.vs", "src/shaders/fragment/litWoodenBox.fs");
    Shader multipleLightTriangles("src/shaders/vertex/multipleLights.vs", "src/shaders/fragment/multipleLights.fs");

    Texture woodenContainer("src/textures/wooden-container.jpg");
    Texture face("src/textures/face.png");
    Texture woodenBox("src/textures/wooden-box.png");
    Texture woodenBoxSpecular("src/textures/wooden-box-specular.png");
    Texture woodenBoxEmission("src/textures/matrix.jpg");

    float vertices[] = {
        -0.45f, -0.25f, 0.0f,  // left 
        0.0f,   -0.25f, 0.0f,  // right
        -0.225f, 0.25f, 0.0f,  //top
    };

    float yellowVertices[] = {
        0.0f,  -0.25f, 0.0f,  // left
        0.45f, -0.25f, 0.0f,  // right
        0.225f, 0.25f, 0.0f   // top 
    };

    float changingVertices[] = {
        0.45f, -0.25f, 0.0f, // left
        0.9f,  -0.25f, 0.0f, // right
        0.675f, 0.25f, 0.0f // top
    };

    float vertexColorVertices[] = {
        // positions            // colors
        -0.9f,  -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, //left
        -0.45f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, //right
        -0.675f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f // top
    };

    float boxVertices[] = {
        // positions      // colors       // textures
        0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };

    float nonRainbowBoxVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float lightingVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

        

    float litWoodenBoxVertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };



    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    unsigned int boxIndices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAOs[8], VBOs[8], EBO;
    glGenVertexArrays(8, VAOs);
    glGenBuffers(8, VBOs);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(yellowVertices), yellowVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(changingVertices), changingVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColorVertices), vertexColorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAOs[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(VAOs[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nonRainbowBoxVertices), nonRainbowBoxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    glBindVertexArray(VAOs[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightingVertices), lightingVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAOs[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(litWoodenBoxVertices), litWoodenBoxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int multipleVBO, multipleVAO;

    glGenVertexArrays(1, &multipleVAO);
    glGenBuffers(1, &multipleVBO);

    glBindBuffer(GL_ARRAY_BUFFER, multipleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(litWoodenBoxVertices), litWoodenBoxVertices, GL_STATIC_DRAW);

    glBindVertexArray(multipleVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, multipleVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    multipleLightTriangles.use();
    multipleLightTriangles.setInt("material.diffuse", 0);
    multipleLightTriangles.setInt("material.specular", 1);
    // unbind since glVertexAttribPointer registers VBO as vertex attribute's bound vertex buffer object
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    litWoodenBoxTriangles.use();
    litWoodenBoxTriangles.setInt("material.diffuse", 0); // set outside as this does not change
    litWoodenBoxTriangles.setInt("material.specular", 1);
    //litWoodenBoxTriangles.setInt("material.emission", 2);
    
    // render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera movement speed
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //lightPos.x = glm::sin(currentFrame);
        //lightPos.y = glm::cos(currentFrame);
        //lightPos.z = glm::sin(currentFrame) + glm::cos(currentFrame);

        orangeTriangle.use();
        glBindVertexArray(VAOs[0]);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //wireframe mode: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); - GL_FILL to revert back if using these
        
        yellowTriangle.use();
        glBindVertexArray(VAOs[1]);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        changingTriangle.use();

        float timeValue = glfwGetTime();
        float greenValue = (glm::sin(timeValue) / 2.0f) + 0.5f;
        
        changingTriangle.setFloat4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAOs[2]);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        verticesTriangle.use();
        glBindVertexArray(VAOs[3]);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        
        boxTriangles.use();
        boxTriangles.setInt("ourTexture", 0);
        boxTriangles.setInt("ourSecondTexture", 1);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        
        boxTriangles.setMatrix1("transform", glm::value_ptr(trans));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodenContainer.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, face.ID);

        glBindVertexArray(VAOs[4]);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // transform matrix
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleAmount = static_cast<float>(glm::sin(glfwGetTime()));
        trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));

        // model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // view matrix
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();

        // projection matrix
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        boxTriangles.setMatrix1("transform", glm::value_ptr(trans));
        boxTriangles.setMatrix1("projection", glm::value_ptr(projection));
        boxTriangles.setMatrix1("model", glm::value_ptr(model));
        boxTriangles.setMatrix1("view", glm::value_ptr(view));

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAOs[5]);

        nonRainbowBoxTriangles.use();
        nonRainbowBoxTriangles.setInt("ourTexture", 0);
        nonRainbowBoxTriangles.setInt("ourSecondTexture", 1);
        
        nonRainbowBoxTriangles.setMatrix1("projection", glm::value_ptr(projection));

        for(unsigned int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            if (i % 3 == 0) angle = glfwGetTime() * 25.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            nonRainbowBoxTriangles.setMatrix1("model", glm::value_ptr(model));

            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        /* HERE float radius = 10.0f;
        float camX = static_cast<float>(glm::sin(glfwGetTime()) * radius);
        float camZ = static_cast<float>(glm::cos(glfwGetTime()) * radius);
        view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); */

        nonRainbowBoxTriangles.setMatrix1("view", glm::value_ptr(view));

        lightingBoxTriangles.use();
        lightingBoxTriangles.setMatrix1("view", glm::value_ptr(view));
        lightingBoxTriangles.setMatrix1("projection", glm::value_ptr(projection));

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lightingBoxTriangles.setMatrix1("model", glm::value_ptr(model));

        glm::vec3 lightColor = glm::vec3(1.0f);
        /*lightColor.x = glm::sin(glfwGetTime() * 2.0f);
        lightColor.y = glm::sin(glfwGetTime() * 0.7f);
        lightColor.z = glm::sin(glfwGetTime() * 1.3f);

        lightingBoxTriangles.setVec3("lightColor", lightColor);*/

        //glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(VAOs[6]);

        litObjectTriangles.use();
        litObjectTriangles.setVec3("viewPos", camera.position);

        litObjectTriangles.setFloat3("material.ambient", 1.0f, 0.5f, 0.31f);
        litObjectTriangles.setFloat3("material.diffuse", 1.0f, 0.5f, 0.31f);
        litObjectTriangles.setFloat3("material.specular", 0.5f, 0.5f, 0.5f);
        litObjectTriangles.setFloat("material.shininess", 32.0f);

        glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);

        litObjectTriangles.setVec3("light.position", lightPos);
        litObjectTriangles.setVec3("light.ambient",  ambientColor);
        litObjectTriangles.setVec3("light.diffuse",  diffuseColor);
        litObjectTriangles.setFloat3("light.specular", 1.0f, 1.0f, 1.0f);

        model = glm::mat4(1.0f);

        litObjectTriangles.setMatrix1("view", glm::value_ptr(view));
        litObjectTriangles.setMatrix1("projection", glm::value_ptr(projection));
        litObjectTriangles.setMatrix1("model", glm::value_ptr(model));

        //glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        glBindVertexArray(VAOs[7]);

        litWoodenBoxTriangles.use();
        litWoodenBoxTriangles.setVec3("light.position", camera.position);
        litWoodenBoxTriangles.setVec3("light.direction", camera.front);
        litWoodenBoxTriangles.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        litWoodenBoxTriangles.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        litWoodenBoxTriangles.setVec3("viewPos", camera.position);

        litWoodenBoxTriangles.setVec3("light.ambient",  glm::vec3(0.1f));
        litWoodenBoxTriangles.setVec3("light.diffuse",  glm::vec3(0.8f));
        litWoodenBoxTriangles.setVec3("light.specular", glm::vec3(1.0f));

        litWoodenBoxTriangles.setFloat("light.constant", 1.0f);
        litWoodenBoxTriangles.setFloat("light.linear", 0.09f);
        litWoodenBoxTriangles.setFloat("light.quadratic", 0.032f);

        litWoodenBoxTriangles.setFloat("material.shininess", 32.0f);

        litWoodenBoxTriangles.setMatrix1("projection", glm::value_ptr(projection));
        litWoodenBoxTriangles.setMatrix1("view", glm::value_ptr(view));

        model = glm::mat4(1.0f);
        litWoodenBoxTriangles.setMatrix1("model", glm::value_ptr(model));
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodenBox.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, woodenBoxSpecular.ID);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, woodenBoxEmission.ID);

        /*for(unsigned int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            litWoodenBoxTriangles.setMatrix1("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/

        multipleLightTriangles.use();
        multipleLightTriangles.setVec3("viewPos", camera.position);
        multipleLightTriangles.setFloat("material.shininess", 32.0f);

        // directional light
        multipleLightTriangles.setFloat3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        multipleLightTriangles.setFloat3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        multipleLightTriangles.setFloat3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        multipleLightTriangles.setFloat3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        
        // point light 1
        multipleLightTriangles.setVec3("pointLights[0].position", pointLightPositions[0]);
        multipleLightTriangles.setFloat3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        multipleLightTriangles.setFloat3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        multipleLightTriangles.setFloat3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        multipleLightTriangles.setFloat("pointLights[0].constant", 1.0f);
        multipleLightTriangles.setFloat("pointLights[0].linear", 0.09f);
        multipleLightTriangles.setFloat("pointLights[0].quadratic", 0.032f);
        
        // point light 2
        multipleLightTriangles.setVec3("pointLights[1].position", pointLightPositions[1]);
        multipleLightTriangles.setFloat3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        multipleLightTriangles.setFloat3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        multipleLightTriangles.setFloat3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        multipleLightTriangles.setFloat("pointLights[1].constant", 1.0f);
        multipleLightTriangles.setFloat("pointLights[1].linear", 0.09f);
        multipleLightTriangles.setFloat("pointLights[1].quadratic", 0.032f);
        
        // point light 3
        multipleLightTriangles.setVec3("pointLights[2].position", pointLightPositions[2]);
        multipleLightTriangles.setFloat3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        multipleLightTriangles.setFloat3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        multipleLightTriangles.setFloat3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        multipleLightTriangles.setFloat("pointLights[2].constant", 1.0f);
        multipleLightTriangles.setFloat("pointLights[2].linear", 0.09f);
        multipleLightTriangles.setFloat("pointLights[2].quadratic", 0.032f);
        
        // point light 4
        multipleLightTriangles.setVec3("pointLights[3].position", pointLightPositions[3]);
        multipleLightTriangles.setFloat3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        multipleLightTriangles.setFloat3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        multipleLightTriangles.setFloat3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        multipleLightTriangles.setFloat("pointLights[3].constant", 1.0f);
        multipleLightTriangles.setFloat("pointLights[3].linear", 0.09f);
        multipleLightTriangles.setFloat("pointLights[3].quadratic", 0.032f);
        
        // spotLight
        multipleLightTriangles.setVec3("spotLight.position", camera.position);
        multipleLightTriangles.setVec3("spotLight.direction", camera.front);
        multipleLightTriangles.setFloat3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        multipleLightTriangles.setFloat3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        multipleLightTriangles.setFloat3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        multipleLightTriangles.setFloat("spotLight.constant", 1.0f);
        multipleLightTriangles.setFloat("spotLight.linear", 0.09f);
        multipleLightTriangles.setFloat("spotLight.quadratic", 0.032f);
        multipleLightTriangles.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        multipleLightTriangles.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        multipleLightTriangles.setMatrix1("projection", glm::value_ptr(projection));
        multipleLightTriangles.setMatrix1("view", glm::value_ptr(view));

        model = glm::mat4(1.0f);
        multipleLightTriangles.setMatrix1("model", glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodenBox.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, woodenBoxSpecular.ID);

        glBindVertexArray(multipleVAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            multipleLightTriangles.setMatrix1("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightingBoxTriangles.use();
        lightingBoxTriangles.setMatrix1("projection", glm::value_ptr(projection));
        lightingBoxTriangles.setMatrix1("view", glm::value_ptr(view));

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightingBoxTriangles.setMatrix1("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // deallocate resources when done with them
    glDeleteVertexArrays(8, VAOs);
    glDeleteBuffers(8, VBOs);

    glDeleteVertexArrays(1, &multipleVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &multipleVBO);
    
    orangeTriangle.del();
    yellowTriangle.del();
    changingTriangle.del();
    verticesTriangle.del();
    boxTriangles.del();
    nonRainbowBoxTriangles.del();
    lightingBoxTriangles.del();
    litObjectTriangles.del();
    litWoodenBoxTriangles.del();
    multipleLightTriangles.del();

    // GLFW terminate and clear allocated GLFW resources
    glfwTerminate();
    return 0;
}