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

// material parameters
vec3 albedo = vec3(1.0, 1.0, 1.0);
float metallic = 1.0f;
float roughness = 0.3f;
float ao = 1.0f;

uniform vec3 lightPos;
uniform vec3 lightColors = vec3(300.0f, 300.0f, 300.0f);;

uniform sampler2D shadowMap;
uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
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
// ----------------------------------------------------------------------------
void main()
{
    vec3 N = normalize(fs_in.Normal);
    vec3 V = normalize(camPos - fs_in.WorldPos);

    // calculate reflectance at Normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    // calculate per-light radiance
    vec3 L = normalize(lightPos - fs_in.WorldPos);
    vec3 H = normalize(V + L);
    float distance = length(lightPos - fs_in.WorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColors * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again

    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    vec3 lightDir = normalize(lightPos - fs_in.WorldPos);
    float shadow = ShadowCalculation(fs_in.CrntPosLightSpace, lightDir);

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = (pow(color, vec3(1.0 / 2.2)) + (1.0 - shadow)) * fs_in.ColorObj;

    FragColor = vec4(color, 1.0);
}