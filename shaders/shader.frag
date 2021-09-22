#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
  FragColor = texture(texture_diffuse1, texCoord);
}
