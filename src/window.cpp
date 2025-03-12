#include "../include/glad.h"
#include "../include/window.hpp"
#include "../include/shader.hpp"
#include "../include/texture.hpp"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// fixes window whenever the window size is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// query GLFW when relevant keys are pressed
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// g++ -I./include lib/glad/*.c* src/*.c* -lglfw -lGL -lGLU
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

    // GLAD init, load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERROR::GLAD::FAILED_INIT" << std::endl;
        return -1;
    }

    Shader orangeTriangle("src/shaders/vertex/vertex.vs", "src/shaders/fragment/orange.fs");
    Shader yellowTriangle("src/shaders/vertex/vertex.vs","src/shaders/fragment/yellow.fs");
    Shader changingTriangle("src/shaders/vertex/vertex.vs", "src/shaders/fragment/change.fs");
    Shader verticesTriangle("src/shaders/vertex/verticesColor.vs", "src/shaders/fragment/verticesColor.fs");
    Shader boxTriangles("src/shaders/vertex/boxVertex.vs", "src/shaders/fragment/boxFragment.fs");

    float vertices[] = {
        -0.45f, -0.25f, 0.0f,  // left 
        0.0f, -0.25f, 0.0f,  // right
        -0.225f, 0.25f, 0.0f,  //top
    };

    float yellowVertices[] = {
        0.0f, -0.25f, 0.0f,  // left
        0.45f, -0.25f, 0.0f,  // right
        0.225f, 0.25f, 0.0f   // top 
    };

    float changingVertices[] = {
        0.45f, -0.25f, 0.0f, // left
        0.9f, -0.25f, 0.0f, // right
        0.675f, 0.25f, 0.0f // top
    };

    float vertexColorVertices[] = {
        // positions            // colors
        -0.9f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, //left
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

    unsigned int boxIndices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAOs[5], VBOs[5], EBO;
    glGenVertexArrays(5, VAOs);
    glGenBuffers(5, VBOs);
    glGenBuffers(1, &EBO);

    //glGenBuffers(1, &EBO);
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
    
    stbi_set_flip_vertically_on_load(true);
    Texture box("src/textures/wooden-container.jpg");
    Texture face("src/textures/face.png");

    // unbind since glVertexAttribPointer registers VBO as vertex attribute's bound vertex buffer object
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    // render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        orangeTriangle.use();
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //wireframe mode: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); - GL_FILL to revert back if using these
        
        yellowTriangle.use();
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        changingTriangle.use();

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        
        changingTriangle.setFloat4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        verticesTriangle.use();
        glBindVertexArray(VAOs[3]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        boxTriangles.use();
        boxTriangles.setInt("ourTexture", 0);
        boxTriangles.setInt("ourSecondTexture", 1);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        
        boxTriangles.setMatrix1("transform", glm::value_ptr(trans));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, box.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, face.ID);
        glBindVertexArray(VAOs[4]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleAmount = static_cast<float>(sin(glfwGetTime()));
        trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        boxTriangles.setMatrix1("transform", glm::value_ptr(trans));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // deallocate resources when done with them
    glDeleteVertexArrays(5, VAOs);
    glDeleteBuffers(5, VBOs);
    
    orangeTriangle.del();
    yellowTriangle.del();
    changingTriangle.del();
    verticesTriangle.del();
    boxTriangles.del();

    // GLFW terminate and clear allocated GLFW resources
    glfwTerminate();
    return 0;
}