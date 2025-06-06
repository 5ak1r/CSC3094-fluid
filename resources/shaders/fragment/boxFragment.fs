#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourSecondTexture;

void main() {
    FragColor = mix(texture(ourTexture, texCoord), texture(ourSecondTexture, texCoord), 0.2) * vec4(ourColor, 1.0);
}