#include "../include/glad.h"
#include "../include/window.hpp"

#include <iostream>
#include <cmath>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// vertex shader for first three triangles
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// orange
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}";

// yellow
const char *yellowFragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}";

// alternating between green and black
const char *changingFragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main() {\n"
    "   FragColor = ourColor\n;"
    "}";

// vertex shader that includes colours in the vertices array
const char *vertexColorVertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

// fragment shader that includes colours in the vertices array
const char *vertexColorFragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main() {\n"
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}";


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

// g++ -I./include src/*.c* -lglfw -lGL -lGLU
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

    // "normal" vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // vertex color vertex shader
    unsigned int vertexColorVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexColorVertexShader, 1, &vertexColorVertexShaderSource, NULL);
    glCompileShader(vertexColorVertexShader);

    glGetShaderiv(vertexColorVertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // yellow fragment shader
    unsigned int yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
    glCompileShader(yellowFragmentShader);

    glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(yellowFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // changing fragment shader
    unsigned int changingFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(changingFragmentShader, 1, &changingFragmentShaderSource, NULL);
    glCompileShader(changingFragmentShader);

    glGetShaderiv(changingFragmentShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(changingFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // vertex color fragment shader
    unsigned int vertexColorFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexColorFragmentShader, 1, &vertexColorFragmentShaderSource, NULL);
    glCompileShader(vertexColorFragmentShader);

    glGetShaderiv(vertexColorFragmentShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertexColorFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // build and compile shader programs
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED" << std::endl;
    }

    unsigned int yellowShaderProgram;
    yellowShaderProgram = glCreateProgram();
    
    glAttachShader(yellowShaderProgram, vertexShader);
    glAttachShader(yellowShaderProgram, yellowFragmentShader);
    glLinkProgram(yellowShaderProgram);

    glGetProgramiv(yellowShaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(yellowShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED" << std::endl;
    }

    unsigned int changingShaderProgram;
    changingShaderProgram = glCreateProgram();

    glAttachShader(changingShaderProgram, vertexShader);
    glAttachShader(changingShaderProgram, changingFragmentShader);
    glLinkProgram(changingShaderProgram);

    glGetProgramiv(changingShaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(yellowShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED" << std::endl;
    }

    unsigned int vertexColorShaderProgram;
    vertexColorShaderProgram = glCreateProgram();

    glAttachShader(vertexColorShaderProgram, vertexColorVertexShader);
    glAttachShader(vertexColorShaderProgram, vertexColorFragmentShader);
    glLinkProgram(vertexColorShaderProgram);

    glGetProgramiv(vertexColorShaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(vertexColorShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED" << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(vertexColorVertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(yellowFragmentShader);
    glDeleteShader(vertexColorFragmentShader);

    // vertex data and configure vertex attributes
    /*float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    unsigned int indices[] {
        0, 1, 3,
        1, 2, 3
    };*/

    /*float vertices[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left 
        0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
        // second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top 
    };*/

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
        -0.9f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, //left
        -0.45f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, //right
        -0.675f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f // top
    };

    unsigned int VAOs[4], VBOs[4]; //, EBO;
    glGenVertexArrays(4, VAOs);
    glGenBuffers(4, VBOs);

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

    // unbind since glVertexAttribPointer registers VBO as vertex attribute's bound vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);
        //wireframe mode: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); - GL_FILL to revert back if using these
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);
        
        glUseProgram(yellowShaderProgram);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(changingShaderProgram);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(changingShaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(vertexColorShaderProgram);
        glBindVertexArray(VAOs[3]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // deallocate resources when done with them
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(yellowShaderProgram);

    // GLFW terminate and clear allocated GLFW resources
    glfwTerminate();
    return 0;
}