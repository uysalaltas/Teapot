#shader vertex
#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3 FragPos;
    vec3 Color;
} vs_out;

void main()
{
    vs_out.Color = aColor;
	mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(aPos, 1.0);
};

#shader fragment
#version 410 core

out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Color;
} fs_in;

float Distance(vec3 point1, vec3 point2)
{
    return sqrt( (point1.x - point2.x) * (point1.x - point2.x) +
                 (point1.y - point2.y) * (point1.y - point2.y) +
                 (point1.z - point2.z) * (point1.z - point2.z) );
}

void main()
{
    //float dist = Distance(worldPos.xyz, u_cameraPos);

    // vec3 finalColor = fs_in.color;
    // 
    // if (true)
    // {    
    //     float darkness = dist / 8.5;
	// 
	//     vec3 fogColor = vec3(0.18, 0.262, 0.455);
	//     fogColor = vec3(0, 0, 0);
	//     darkness = clamp(darkness, 0, 1);
	//     finalColor = mix(fs_in.color, fogColor, darkness);
    // }

    FragColor = vec4(fs_in.Color, 1.0);
};