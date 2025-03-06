#version 330 core

// takes input from the vertices array

out vec4 FragColor;
in vec3 ourColor;

void main() {
   FragColor = vec4(ourColor, 1.0);
}
