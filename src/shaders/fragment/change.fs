#version 330 core

// takes input from the program to change the colour depending on input

out vec4 FragColor;
uniform vec4 ourColor;

void main() {
   FragColor = ourColor;
}