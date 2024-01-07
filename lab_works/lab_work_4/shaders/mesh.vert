#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

uniform mat4 uMVPMatrix; // Projection * View * Model
uniform mat4 uNormalMatrix; 

out vec3 normal;
out vec3 FragPos;



void main()
{
	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );
	normal = vec3(uNormalMatrix * vec4(aVertexNormal, 1.f));
	FragPos = vec3(uMVPMatrix * vec4(aVertexPosition, 1.f));
}
