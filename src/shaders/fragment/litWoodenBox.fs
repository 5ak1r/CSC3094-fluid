#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;

    float shininess;
};

struct Light {
    vec4 posdir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main() {

    vec3 textureColor = texture(material.diffuse, TexCoords).rgb;

    //ambient lighting
    vec3 ambient = light.ambient * textureColor;

    //diffusion lighting
    vec3 norm = normalize(Normal);
    
    vec3 lightDir;

    if (light.posdir.w < 0.1) { //account for floating point
        lightDir = normalize(-light.posdir.xyz);
    }
    else if (light.posdir.w == 1.0) {
        lightDir = normalize(light.posdir.xyz - FragPos);
    }
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * textureColor;

    //specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.00001), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    //emission
    vec3 emission = vec3(0.0);
    
    if(texture(material.specular, TexCoords).r == 0.0) emission = texture(material.emission, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}