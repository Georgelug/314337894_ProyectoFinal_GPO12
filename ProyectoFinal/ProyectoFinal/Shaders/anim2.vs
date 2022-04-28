#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

const float amplitude = 1;
const float frequency = 0.1;
const float frequencyOfFloating = 0.3;
const float PI = 3.14159;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;


void main()
{
	
	float floating = 1*sin(PI*frequencyOfFloating*time);

	gl_Position = projection*view*model*vec4( aPos.x , aPos.y+floating , aPos.z , 1);
	

	TexCoords=vec2(aTexCoords.x,aTexCoords.y);

}