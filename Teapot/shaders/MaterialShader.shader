#shader vertex
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoords;

out VS_OUT
{
    vec3 FragPos;
    vec3 Color;
    vec2 TexCoords;
    vec3 Normal;
    vec4 CrntPosLightSpace[2];
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix[2];

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Color = aColor;
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;

    for (int idx = 0; idx < 2; idx++)
        vs_out.CrntPosLightSpace[idx] = lightSpaceMatrix[idx] * vec4(vs_out.FragPos, 1.0);

    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(aPos, 1.0);
};

#shader fragment
#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_LIGHTS 4

in VS_OUT
{
    vec3 FragPos;
    vec3 Color;
    vec2 TexCoords;
    vec3 Normal;
    vec4 CrntPosLightSpace[2];
} fs_in;

uniform sampler2D shadowMapArr[2];

uniform int pointLightCount;
uniform int directionalLightCount;
uniform int spotLightCount;

uniform int activateShadow;

uniform int hasTexture;

uniform vec3 camPos;
uniform DirLight dirLights[NR_LIGHTS];
uniform PointLight pointLights[NR_LIGHTS];
uniform SpotLight spotLights[NR_LIGHTS];
uniform Material material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, int index);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int index);
float ShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace, vec3 lightDir);

void main()
{
    // properties
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(camPos - fs_in.FragPos);

    int lightIndex = 0;

    // phase 1: directional lighting
    vec3 result = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < directionalLightCount; i++)
    {
        result += CalcDirLight(dirLights[i], norm, viewDir, lightIndex);
        lightIndex++;
    }

    // phase 2: point lights
    for (int i = 0; i < pointLightCount; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fs_in.FragPos, viewDir);
    }

    // phase 3: spot light
    for (int i = 0; i < spotLightCount; i++)
    {
        result += CalcSpotLight(spotLights[i], norm, fs_in.FragPos, viewDir, lightIndex);
        lightIndex++;
    }

    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, int index)
{
    //vec3 lightDir = normalize(-light.direction);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    if (hasTexture == 1)
    {
        ambient  = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
        diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
        specular = light.specular * spec * vec3(texture(material.diffuse, fs_in.TexCoords));
    }
    else
    {
        ambient  = light.ambient;
        diffuse  = light.diffuse * diff;
        specular = light.specular * spec;
    }

    float shadow = 0.0f;
    if (activateShadow == 1)
    {
        shadow = ShadowCalculation(shadowMapArr[index], fs_in.CrntPosLightSpace[index], lightDir);
    }
    return (ambient + ((1.0f - shadow) * diffuse) + ((1.0f - shadow) * specular)) * fs_in.Color;
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    if (hasTexture == 1)
    {
        ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
        diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
        specular = light.specular * spec * vec3(texture(material.diffuse, fs_in.TexCoords));
    }
    else
    {
        ambient = light.ambient;
        diffuse = light.diffuse * diff;
        specular = light.specular * spec;
    }

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * fs_in.Color;
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int index)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    if (hasTexture == 1)
    {
        ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
        diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
        specular = light.specular * spec * vec3(texture(material.diffuse, fs_in.TexCoords));
    }
    else
    {
        ambient = light.ambient;
        diffuse = light.diffuse * diff;
        specular = light.specular * spec;
    }

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    float shadow = 0.0f;
    if (activateShadow == 1)
    {
        shadow = ShadowCalculation(shadowMapArr[index], fs_in.CrntPosLightSpace[index], lightDir);
    }
    return (ambient + ((1.0f - shadow) * diffuse) + ((1.0f - shadow) * specular)) * fs_in.Color;
}

// calculates shadow
float ShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    //float bias = max(0.05 * (1.0 - dot(fs_in.Normal, lightDir)), 0.005);
    float bias = 0.001;
    //float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    float shadow = 0.0;
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