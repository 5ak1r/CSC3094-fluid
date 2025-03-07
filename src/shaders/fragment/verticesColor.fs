#version 330 core

// takes input from the vertices array

in vec3 ourColor;
out vec4 FragColor;

void main() {
   FragColor = vec4(ourColor, 1.0);
}
