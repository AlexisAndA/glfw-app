#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

#define NR_LIGHT_SPACE_MATRICES 4

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace[NR_LIGHT_SPACE_MATRICES];



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix[NR_LIGHT_SPACE_MATRICES];

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    FragPosLightSpace[0] = lightSpaceMatrix[0] * vec4(FragPos, 1.0);
    FragPosLightSpace[1] = lightSpaceMatrix[1] * vec4(FragPos, 1.0);
    FragPosLightSpace[2] = lightSpaceMatrix[2] * vec4(FragPos, 1.0);
    FragPosLightSpace[3] = lightSpaceMatrix[3] * vec4(FragPos, 1.0);
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

