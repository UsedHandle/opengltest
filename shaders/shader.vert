#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;

uniform mat4 camMat;

void main(){
    gl_Position = camMat * vec4(aPos, 1.0);
    texCoord = aTex;
}
