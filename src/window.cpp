#include "../include/glad.h"
#include "../include/window.hpp"
#include "../include/constants.hpp"
#include "../include/shader.hpp"
#include "../include/texture.hpp"
#include "../include/camera.hpp"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/mesh.hpp"
#include "../include/model.hpp"
#include "../include/particle.hpp"

#include <cstdlib>
#include <iostream>

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
    
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    Shader modelShader("resources/shaders/vertex/modelLoadNoTextures.vs", "resources/shaders/fragment/modelLoadNoTextures.fs");
    Model model("resources/models/sphere/sphere.obj");

    std::vector<glm::mat4> modelMatrices;
    std::vector<Particle*> particles;
        
    for(int i = 0; i < PARTICLE_ROW_COUNT; i++) {
        for(int j = 0; j < PARTICLE_ROW_COUNT; j++) {
            for(int k = 0; k < PARTICLE_ROW_COUNT; k++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(TRANSLATE) * glm::vec3(i, j, k));
                model = glm::scale(model, glm::vec3(SCALE));

                Particle *particle = new Particle(model[3], model);

                particles.push_back(particle);
                modelMatrices.push_back(model);
            }
        }
    }

    Particle::sortParticles(particles);
    std::unordered_map<uint32_t, uint32_t> nTable = Particle::neighbourTable(particles);
    for(auto i : particles) {
        i->getNeighbours(particles, nTable);
    }
    //Particle::getNeighbours(std::vector<Particle *> sortedParticles, std::unordered_map<uint32_t, uint32_t> &neighbourTable)
    
    // render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera movement speed
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        modelShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        modelShader.setMatrix1("projection", glm::value_ptr(projection));
        modelShader.setMatrix1("view", value_ptr(view));

        modelShader.setVec3("viewPos", camera.position);
        modelMatrices.clear();

        for(auto particle: particles) modelMatrices.push_back(particle->updatePhysics(deltaTime));

        model.DrawInstanced(modelShader, modelMatrices); 

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    modelShader.del();

    // GLFW terminate and clear allocated GLFW resources
    glfwTerminate();
    return 0;
}