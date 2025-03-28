#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    //sampler2D emission;

    float shininess;
};

struct Light {
    //both pos and dir for directional and positional lights
    //vec4 posdir;

    //flashlight
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    //Phong lighting model
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //attenuation for point light
    float constant;
    float linear;
    float quadratic;
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
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * textureColor;

    /*if (light.posdir.w < 0.1) { //account for floating point
        lightDir = normalize(-light.posdir.xyz);
    }
    else if (light.posdir.w == 1.0) {
        lightDir = normalize(light.posdir.xyz - FragPos);
    }*/

    //specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.000001), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    //spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    //float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / epsilon);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    //attenuation
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    //emission
    //vec3 emission = vec3(0.0);
    
    //if(texture(material.specular, TexCoords).r == 0.0) emission = texture(material.emission, TexCoords).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;// + emission;
    FragColor = vec4(result, 1.0);
}