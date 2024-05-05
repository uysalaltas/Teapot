#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

vec2 aTexCoords = vec2(1.0f, 1.0f);

out VS_OUT
{
    vec3 WorldPos;
    vec3 Normal;
    vec3 ColorObj;
    vec4 CrntPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    vs_out.ColorObj = aColor;
    vs_out.WorldPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal   = mat3(transpose(inverse(model))) * aNormal;
    vs_out.CrntPosLightSpace = lightSpaceMatrix * vec4(vs_out.WorldPos, 1.0);

    gl_Position = projection * view * vec4(vs_out.WorldPos, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in VS_OUT
{
    vec3 WorldPos;
    vec3 Normal;
    vec3 ColorObj;
    vec4 CrntPosLightSpace;
} fs_in;

uniform vec3 lightPos;
vec3 lightColor = vec3(1.0, 1.0, 1.0);

uniform sampler2D shadowMap;
uniform vec3 camPos;

uniform float ambientStrength = 0.5f;
uniform float specularStrength = 0.3f;
uniform float shininess = 100.0f;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(fs_in.Normal, lightDir)), 0.005);
    //float bias = 0.001;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    //float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

void main()
{
    // Calculate ambient light
    vec3 ambient = ambientStrength * lightColor;
    // Calculate normalized light direction
    vec3 lightDir = normalize(lightPos - fs_in.WorldPos);
    // Calculate diffuse light
    float diffuseIntensity = max(dot(fs_in.Normal, lightDir), 0.0);
    vec3 diffuse = diffuseIntensity * lightColor;
    // Calculate view direction
    vec3 viewDir = normalize(camPos - fs_in.WorldPos);
    // Calculate half-vector (for Blinn-Phong specular)
    vec3 halfDir = normalize(lightDir + viewDir);
    // Calculate specular light (using Blinn-Phong)
    float spec = pow(max(dot(fs_in.Normal, halfDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;
    float shadow = ShadowCalculation(fs_in.CrntPosLightSpace, lightDir);
    // Final color calculation (ambient + diffuse + specular)
    vec3 result = (ambient + (1.0f - shadow) * diffuse) * fs_in.ColorObj + specular;
    // Output final color
    FragColor = vec4(result, 1.0);
}