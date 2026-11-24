#version 450

layout(binding = 0) uniform CameraData 
{
    mat4 vp;
    mat4 projection;
    mat4 view;
} cam;

layout(binding = 1) uniform UniformBufferObject 
{
    mat4 model;
} ubo;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec2 fragTexCoord;

void main() 
{
   gl_Position = cam.vp * ubo.model * vec4(inPosition, 1.0);
   fragColor = inColor;
   fragTexCoord = inTexCoord;

   //vec4 pos = cam.projection * cam.view * vec4(inPosition, 1.0);
   //gl_Position = pos.xyww;
   //fragColor = inColor;
   //fragTexCoord = inTexCoord;
}